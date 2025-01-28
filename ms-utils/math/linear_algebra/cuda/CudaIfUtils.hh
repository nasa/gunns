#ifndef CUDAIFUTILS_HH
#define CUDAIFUTILS_HH

/**
@copyright Copyright 2025 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

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
- (((Jason Harvey) (CACI) (Initial) (2018-10))
   ((Jason Harvey) (CACI) (CUDA 12.2) (2025-01)))

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
};

/// @}

#endif
