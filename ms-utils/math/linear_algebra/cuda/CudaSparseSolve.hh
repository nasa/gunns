#ifndef CUDASPARSESOLVE_HH
#define CUDASPARSESOLVE_HH

/**
@file
@brief     CUDA Sparse System Solution declarations

@defgroup  TSM_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA_SPARSE CUDA Sparse System Solution
@ingroup   TSM_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA

@details
PURPOSE:
- (This class solves a set of linear algebraic equations on an Nvidia GPU using the CUDA cusparse
   library's sparse matrix Cholesky factorization method.  This is best for sparse matrices, but
   also works for dense matrices.)

ICG: (No)
SWIG: (No)

REFERENCE:
- ((https://docs.nvidia.com/cuda/cuda-runtime-api/index.html)
   (https://docs.nvidia.com/cuda/cusparse/index.html)
   (https://docs.nvidia.com/cuda/cusolver/index.html))

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((math/linear_algebra/cuda/CudaSparseSolve.o))

PROGRAMMERS:
- ((Alex Lin) (NASA) (2017-07))

@{
*/

#include "math/linear_algebra/CholeskyLdu.hh"
#include "math/linear_algebra/cuda/CudaIfUtils.hh"
#include <cusolverSp.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    CUDA Sparse System Solution Utility
///
/// @details  Refer to class PURPOSE.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CudaSparseSolve : public CholeskyLdu, public CudaIfUtils
{
    public:
        /// @brief Default constructor.
        CudaSparseSolve();
        /// @brief Default destructor.
        virtual ~CudaSparseSolve();
        /// @brief Changes matrix [A] into sparse form on the GPU but does not decompose it.
        virtual void Decompose(double* A, int n);
        /// @brief Decomposes the sparse [A] and solves [A]{x} = {b} for {x} on the GPU.
        virtual void Solve(double* LDU, double B[], double x[], int n);

    protected:
        cusparseHandle_t   p_handle;       /**< (--) Handle to the cuSparse context. */
        cusolverSpHandle_t solver_handle;  /**< (--) Handle to the cuSolverSP context. */
        cusparseMatDescr_t mat_desc;       /**< (--) Structure describing the sparse matrix. */
        int                current_n;      /**< (--) Current size of n we have allocated space for. */
        double*            d_A_dense;      /**< (--) Pointer to the dense-form [A] matrix on the GPU. */
        int                nnz;            /**< (--) Total number of non-zero elements. */
        int*               d_nnzPerVector; /**< (--) Array of number of non-zero elements per vector on the GPU. */
        double*            d_A;            /**< (--) Pointer to the sparse-form [A] matrix on the GPU. */
        int*               d_A_RowIndices; /**< (--) Array of the start of the sparse columns on the GPU. */
        int*               d_A_ColIndices; /**< (--) Array of number of the column indices of the non-zero elements on the GPU. */
        double*            d_b;            /**< (--) Pointer to the {b} vector on the GPU. */
        double*            d_x;            /**< (--) Pointer to the {x} vector on the GPU. */
};

/// @}

#endif
