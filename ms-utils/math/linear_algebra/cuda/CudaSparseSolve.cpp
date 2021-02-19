/**
@file
@brief     CUDA Sparse System Solution declarations

LIBRARY DEPENDENCY:
  ((math/linear_algebra/CholeskyLdu.o)
   (math/linear_algebra/cuda/CudaIfUtils.o))
*/

#include "CudaSparseSolve.hh"
#include "software/exceptions/TsNumericalException.hh"
#include <sstream>
#include <cfloat>
#include <cuda_runtime.h>
#include <cublas_v2.h>

CudaSparseSolve::CudaSparseSolve()
    :
    current_n(0),
    d_A_dense(0),
    nnz(0),
    d_nnzPerVector(0),
    d_A(0),
    d_A_RowIndices(0),
    d_A_ColIndices(0),
    d_b(0),
    d_x(0)
{
    /// - Create handles to CUDA contexts
    cusparseCreate(&p_handle);
    cusolverSpCreate(&solver_handle);

    /// - Create description of sparse matrix
    cusparseCreateMatDescr(&mat_desc);
    cusparseSetMatIndexBase(mat_desc, CUSPARSE_INDEX_BASE_ZERO);
    cusparseSetMatType(mat_desc, CUSPARSE_MATRIX_TYPE_GENERAL);
    cusparseSetMatFillMode(mat_desc, CUSPARSE_FILL_MODE_LOWER);
    cusparseSetMatDiagType(mat_desc, CUSPARSE_DIAG_TYPE_NON_UNIT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this CUDA Sparse Solve.
////////////////////////////////////////////////////////////////////////////////////////////////////
CudaSparseSolve::~CudaSparseSolve()
{
    if (d_A_ColIndices) {
        cudaFree(d_A_ColIndices) ;
    }
    if (d_A_RowIndices) {
        cudaFree(d_A_RowIndices) ;
    }
    if (d_A) {
        cudaFree(d_A) ;
    }
    if (d_x) {
        cudaFree(d_x) ;
    }
    if (d_b) {
        cudaFree(d_b) ;
    }
    if (d_nnzPerVector) {
        cudaFree(d_nnzPerVector) ;
    }
    if (d_A_dense) {
        cudaFree(d_A_dense) ;
    }
    if (mat_desc) {
        cusparseDestroyMatDescr(mat_desc);
    }
    if (solver_handle) {
        cusolverSpDestroy(solver_handle);
    }
    if (p_handle) {
        cusparseDestroy(p_handle);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] A (--) On input, the pointer to the first element of the matrix A[n][n].
///                       This routine does not modify the input matrix
/// @param[in]     n (--) The number of rows and/or columns of the matrix A.
///
/// @throws   TsInitializationException, TsNumericalException
///
/// @details  This routine creates sparse matrix from the input matrix A.  This is done on the GPU.
///           Throws exceptions on errors in the GPU processes.
///
/// @note     This doesn't actually decompose the matrix, we're just using it as a convenient
///           interface to get the matrix to the GPU in the desired format.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CudaSparseSolve::Decompose(double* A, int n)
{
    if (n > current_n) {
        /// - Allocate CUDA memory for dense matrix.
        if (d_A_dense) {
            cudaFree(d_A_dense);
        }
        checkReturn(cudaMalloc(&d_A_dense, n * n * sizeof(*d_A_dense)), __FILE__, __LINE__);
        /// - Allocate CUDA memory for number non zero elements per row.
        if (d_nnzPerVector) {
            cudaFree(d_nnzPerVector);
        }
        checkReturn(cudaMalloc(&d_nnzPerVector, n * sizeof(*d_nnzPerVector)), __FILE__, __LINE__);
        /// - Allocate CUDA memory for B vector.
        if (d_b) {
            cudaFree(d_b);
        }
        checkReturn(cudaMalloc(&d_b, n * sizeof(double)), __FILE__, __LINE__);
        /// - Allocate CUDA memory for x vector.
        if (d_x) {
            cudaFree(d_x);
        }
        checkReturn(cudaMalloc(&d_x, n * sizeof(double)), __FILE__, __LINE__);
        current_n = n;
    }
    /// - Copy dense matrix to device and determine number of non zeros.
    checkReturn(cudaMemcpy(d_A_dense, A, n * n * sizeof(*d_A_dense), cudaMemcpyHostToDevice),
                           __FILE__, __LINE__);
    checkReturn(cusparseDnnz(p_handle, CUSPARSE_DIRECTION_ROW, n, n, mat_desc, d_A_dense, n,
                             d_nnzPerVector, &nnz), __FILE__, __LINE__);

    /// - Allocate CUDA memory for sparse matrix A.
    if (d_A) {
        cudaFree(d_A);
    }
    checkReturn(cudaMalloc(&d_A, nnz * sizeof(*d_A)), __FILE__, __LINE__);
    /// - Allocate CUDA memory for rows indices in A.
    if (d_A_RowIndices) {
        cudaFree(d_A_RowIndices);
    }
    checkReturn(cudaMalloc(&d_A_RowIndices, (n + 1) * sizeof(*d_A_RowIndices)), __FILE__, __LINE__);
    /// - Allocate CUDA memory for column indices in A.
    if (d_A_ColIndices) {
        cudaFree(d_A_ColIndices);
    }
    checkReturn(cudaMalloc(&d_A_ColIndices, nnz * sizeof(*d_A_ColIndices)), __FILE__, __LINE__);

    /// - Create sparse matrix from dense.
    checkReturn(cusparseDdense2csr(p_handle, n, n, mat_desc, d_A_dense, n, d_nnzPerVector, d_A,
                                   d_A_RowIndices, d_A_ColIndices), __FILE__, __LINE__);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] LDU (--) Not used
/// @param[in]     B   (--) Pointer to the column vector B.0
/// @param[out]    x   (--) Solution to the equation Ax = B.
/// @param[in]     n   (--) The number of rows or columns of the matrix LDU.
///
/// @throws   TsInitializationException, TsNumericalException
///
/// @details  This routine uses the sparse matrix, class variable d_A and input parameter B
///           to solve the linear equation Ax = B.  d_A is already stored on the GPU.  Copy
///           B to the GPU and solve for x.  x is copied off of the GPU back to host pointer x.
///           Throws exceptions on errors in the GPU processes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CudaSparseSolve::Solve(double* LDU __attribute__((unused)), double B[], double x[], int n)
{
    /// - Copy B to CUDA memory.
    checkReturn(cudaMemcpy(d_b, B, n * sizeof(double), cudaMemcpyHostToDevice), __FILE__, __LINE__);

    /// - Solve the system.  The tolerance value of DBL_EPSILON * 1.0E-15 matches GUNNS minimum
    ///   value of a row diagonal.
    int singularity;
    cusolverStatus_t err = cusolverSpDcsrlsvchol(solver_handle, n, nnz, mat_desc,
                                                 d_A, d_A_RowIndices, d_A_ColIndices, d_b,
                                                 DBL_EPSILON * 1.0E-15,
                                                 0, d_x, &singularity);

    /// - Handle errors with the solution.
    if (singularity != -1) {
        std::ostringstream msg;
        msg << "cusolverSpDcsrlsvchol matrix is not positive definite, k >= " << singularity << ".";
        throw(TsNumericalException("", "CudaSparseSolve::Solve", msg.str()));
    }
    checkReturn(err, __FILE__, __LINE__);

    /// - Copy x to host memory.
    checkReturn(cudaMemcpy(x, d_x, n * sizeof(double), cudaMemcpyDeviceToHost), __FILE__, __LINE__);
}
