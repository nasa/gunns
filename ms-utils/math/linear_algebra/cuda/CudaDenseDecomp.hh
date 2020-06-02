#ifndef CUDACHOLESKYLDU_HH
#define CUDACHOLESKYLDU_HH

/**
@file
@brief     CUDA Dense Matrix Decomposition declarations

@defgroup  TSM_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA_DENSE CUDA Dense Matrix Decomposition
@ingroup   TSM_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA

@details
PURPOSE:
- (This class decomposes a square symmetric matrix on an Nvidia GPU using the CUDA cusolver
   library's Bunch-Kaufman LDL factorization method.  This is best used for dense matrices, but also
   works for sparse matrices.)

ICG: (No)
SWIG: (No)

REFERENCE:
- ((https://docs.nvidia.com/cuda/cuda-runtime-api/index.html)
   (https://docs.nvidia.com/cuda/cusolver/index.html))

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((math/linear_algebra/CudaDenseDecomp.o))

PROGRAMMERS:
- ((Alex Lin) (NASA) (2017-07))

@{
*/

#include "math/linear_algebra/CholeskyLdu.hh"
#include "math/linear_algebra/cuda/CudaIfUtils.hh"
#include <cusolverDn.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    CUDA Dense Matrix Decomposition Utility
///
/// @details  Refer to class PURPOSE.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CudaDenseDecomp : public CholeskyLdu, public CudaIfUtils
{
    public:
        /// @brief Default constructor.
        CudaDenseDecomp();
        /// @brief Default destructor.
        virtual ~CudaDenseDecomp();
        /// @brief Decomposes the admittance matrix [A].
        virtual void Decompose(double *A, int n);

    protected:
        cusolverDnHandle_t solver_handle; /**< (--) Handle to the cuSolverDN context. */
        int                current_n;     /**< (--) Current size of n we have allocated space for. */
        int*               devInfo;       /**< (--) Device return status information. */
        double*            d_A;           /**< (--) Pointer to GPU memory for our matrix. */
        int                work_size;     /**< (--) Size of workspace calculated by CUDA. */
        double*            work;          /**< (--) Pointer to GPU work space. */
        int*               ipiv;          /**< (--) Pointer to GPU pivot table. */
};

/// @}

#endif
