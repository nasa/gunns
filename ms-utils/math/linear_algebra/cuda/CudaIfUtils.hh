#ifndef CUDAIFUTILS_HH
#define CUDAIFUTILS_HH

/**
@file
@brief     CUDA Interface Utilities declarations

@defgroup  TSM_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA_IF_UTILS CUDA Interface Utilities
@ingroup   TSM_UTILITIES_MATH_LINEAR_ALGEBRA_CUDA

@details
PURPOSE:
- (This class provides utility functions for interfacing with CUDA, such as return code error
   checking.  Error status returned from CUDA are translated into an exception thrown and detailed
   message output to H&S.)

ICG: (No)
SWIG: (No)

REFERENCE:
- ((https://docs.nvidia.com/cuda/cuda-runtime-api/index.html)
   (https://docs.nvidia.com/cuda/cusparse/index.html)
   (https://docs.nvidia.com/cuda/cusolver/index.html))

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((math/linear_algebra/CudaIfUtils.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2018-10))

@{
*/

#include <cuda_runtime.h>
#include <cusolverSp.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    CUDA Interface Utilities
///
/// @details  Refer to class PURPOSE.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CudaIfUtils
{
    public:
        /// @brief Default constructor.
        CudaIfUtils();
        /// @brief Default destructor.
        virtual ~CudaIfUtils();
        /// @brief Checks the given CUDA runtime library return call for errors.
        void checkReturn(cudaError_t err, const char *file, const int line);
        /// @brief Checks the given CUDA cusparse library return call for errors.
        void checkReturn(cusparseStatus_t err, const char *file, const int line);
        /// @brief Checks the given CUDA cusolver library return call for errors.
        void checkReturn(cusolverStatus_t err, const char *file, const int line);

    protected:
        /// @brief Gets the return code in string format from CUDA cusparse library calls.
        static const char* getCudaReturnString(cusparseStatus_t err);
        /// @brief Gets the return code in string format from CUDA cusolver library calls.
        static const char* getCudaReturnString(cusolverStatus_t err);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] err (--) cusparse return code enumeration.
///
/// @returns  char* (--) cusparse return code in string form.
///
/// @details  Returns the given enumeration value as a human-readable string.  This overloaded
///           version supports the return codes from the cusparse library.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* CudaIfUtils::getCudaReturnString(cusparseStatus_t err)
{
    switch (err)
    {
        case CUSPARSE_STATUS_SUCCESS:
            return "CUSPARSE_STATUS_SUCCESS";

        case CUSPARSE_STATUS_NOT_INITIALIZED:
            return "CUSPARSE_STATUS_NOT_INITIALIZED";

        case CUSPARSE_STATUS_ALLOC_FAILED:
            return "CUSPARSE_STATUS_ALLOC_FAILED";

        case CUSPARSE_STATUS_INVALID_VALUE:
            return "CUSPARSE_STATUS_INVALID_VALUE";

        case CUSPARSE_STATUS_ARCH_MISMATCH:
            return "CUSPARSE_STATUS_ARCH_MISMATCH";

        case CUSPARSE_STATUS_MAPPING_ERROR:
            return "CUSPARSE_STATUS_MAPPING_ERROR";

        case CUSPARSE_STATUS_EXECUTION_FAILED:
            return "CUSPARSE_STATUS_EXECUTION_FAILED";

        case CUSPARSE_STATUS_INTERNAL_ERROR:
            return "CUSPARSE_STATUS_INTERNAL_ERROR";

        case CUSPARSE_STATUS_MATRIX_TYPE_NOT_SUPPORTED:
            return "CUSPARSE_STATUS_MATRIX_TYPE_NOT_SUPPORTED";

        case CUSPARSE_STATUS_ZERO_PIVOT:
            return "CUSPARSE_STATUS_ZERO_PIVOT";

        default:
            return "<unknown>";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] err (--) cusolver return code enumeration.
///
/// @returns  char* (--) cusolver return code in string form.
///
/// @details  Returns the given enumeration value as a human-readable string.  This overloaded
///           version supports the return codes from the cusolver library.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* CudaIfUtils::getCudaReturnString(cusolverStatus_t err)
{
    switch (err)
    {
        case CUSOLVER_STATUS_SUCCESS:
            return "CUSOLVER_STATUS_SUCCESS";

        case CUSOLVER_STATUS_NOT_INITIALIZED:
            return "CUSOLVER_STATUS_NOT_INITIALIZED";

        case CUSOLVER_STATUS_ALLOC_FAILED:
            return "CUSOLVER_STATUS_ALLOC_FAILED";

        case CUSOLVER_STATUS_INVALID_VALUE:
            return "CUSOLVER_STATUS_INVALID_VALUE";

        case CUSOLVER_STATUS_ARCH_MISMATCH:
            return "CUSOLVER_STATUS_ARCH_MISMATCH";

        case CUSOLVER_STATUS_MAPPING_ERROR:
            return "CUSOLVER_STATUS_MAPPING_ERROR";

        case CUSOLVER_STATUS_EXECUTION_FAILED:
            return "CUSOLVER_STATUS_EXECUTION_FAILED";

        case CUSOLVER_STATUS_INTERNAL_ERROR:
            return "CUSOLVER_STATUS_INTERNAL_ERROR";

        case CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED:
            return "CUSOLVER_STATUS_MATRIX_TYPE_NOT_SUPPORTED";

        case CUSOLVER_STATUS_NOT_SUPPORTED:
            return "CUSOLVER_STATUS_NOT_SUPPORTED";

        case CUSOLVER_STATUS_ZERO_PIVOT:
            return "CUSOLVER_STATUS_ZERO_PIVOT";

        case CUSOLVER_STATUS_INVALID_LICENSE:
            return "CUSOLVER_STATUS_INVALID_LICENSE";

        default:
            return "<unknown>";
    }
}

#endif
