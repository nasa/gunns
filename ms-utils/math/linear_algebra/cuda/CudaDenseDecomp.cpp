/**
@file
@brief     CUDA Dense Matrix Decomposition implementation

LIBRARY DEPENDENCY:
  ((math/linear_algebra/CholeskyLdu.o)
   (math/linear_algebra/cuda/CudaIfUtils.o))
*/

#include "CudaDenseDecomp.hh"
#include "software/exceptions/TsNumericalException.hh"
#include <sstream>
#include <cuda_runtime.h>
#include <cublas_v2.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this CUDA Dense Matrix Decomposition.
////////////////////////////////////////////////////////////////////////////////////////////////////
CudaDenseDecomp::CudaDenseDecomp()
    :
    solver_handle(),
    current_n(0),
    devInfo(0),
    d_A(0),
    work_size(0),
    work(0),
    ipiv(0)
{
    cusolverDnCreate(&solver_handle);
    cudaMalloc(&devInfo, sizeof(int));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this CUDA Dense Matrix Decomposition.
////////////////////////////////////////////////////////////////////////////////////////////////////
CudaDenseDecomp::~CudaDenseDecomp()
{
    if (work) {
        cudaFree(work) ;
    }
    if (ipiv) {
        cudaFree(ipiv) ;
    }
    if (d_A) {
        cudaFree(d_A) ;
    }
    if (devInfo) {
        cudaFree(devInfo) ;
    }
    cusolverDnDestroy(solver_handle);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] A (--) On input, the pointer to the first element of the matrix A[n][n].  On
///                       output, the matrix A is replaced by the lower triangular, diagonal, and
///                       upper triangular matrices of the LDL' factorization of A.
/// @param[in]     n (--) The number of rows of the matrix A.
///
/// @throws  TsInitializationException, TsNumericalException
///
/// @details Copies the matrix A to the GPU, decomposes it on the GPU, copies it back to the host.
///          Re-allocates GPU memory if the size of A has increased.  Throws exceptions on errors in
///          the GPU processes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CudaDenseDecomp::Decompose(double *A, int n)
{
    /// - Re-allocate memory if the input matrix is larger than the allocations we currently have.
    if (n > current_n) {

        if (d_A) {
            cudaFree(d_A) ;
        }
        checkReturn(cudaMalloc(&d_A, n * n * sizeof(double)), __FILE__, __LINE__);

        if (ipiv) {
            cudaFree(ipiv) ;
        }
        checkReturn(cudaMalloc(&ipiv, n * sizeof(int)), __FILE__, __LINE__);

        /// - Compute the workspace size needed.
        checkReturn(cusolverDnDsytrf_bufferSize(solver_handle, n, d_A, n, &work_size), __FILE__, __LINE__);

        if (work) {
            cudaFree(work) ;
        }
        checkReturn(cudaMalloc(&work, work_size * sizeof(double)), __FILE__, __LINE__);
        current_n = n ;
    }

    /// - Copy the host matrix to the GPU.
    checkReturn(cudaMemcpy(d_A, A, n * n * sizeof(double), cudaMemcpyHostToDevice), __FILE__, __LINE__);

    /// - Decompose the matrix.
    checkReturn(cusolverDnDsytrf(solver_handle, CUBLAS_FILL_MODE_LOWER, n, d_A, n, ipiv, work,
            work_size, devInfo), __FILE__, __LINE__);

    /// - Get success/fail info from the GPU and throw if errors.
    int devInfo_h = 0;
    cudaMemcpy(&devInfo_h, devInfo, sizeof(int), cudaMemcpyDeviceToHost);

    if (devInfo_h != 0) {
        std::ostringstream msg;
        msg << "cusolverDnDsytrf error, devInfo = " << devInfo_h;
        throw(TsNumericalException("", "CudaDenseDecomp::Decompose", msg.str()));
    }

    /// - Copy the decomposed matrix back to the host.
    checkReturn(cudaMemcpy(A, d_A, n * n * sizeof(double), cudaMemcpyDeviceToHost), __FILE__, __LINE__);
}
