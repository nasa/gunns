/**
@copyright Copyright 2025 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@file
@brief     CUDA Interface Utillities implementation

LIBRARY DEPENDENCY:
  ()
*/

#include "CudaIfUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this CUDA Interface Utilities.
////////////////////////////////////////////////////////////////////////////////////////////////////
CudaIfUtils::CudaIfUtils()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this CUDA Interface Utilities.
////////////////////////////////////////////////////////////////////////////////////////////////////
CudaIfUtils::~CudaIfUtils()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] err  (--) cuda runtime library return code enumeration.
/// @param[in] file (--) Source code file string from the caller.
/// @param[in] line (--) Source code line number from the caller.
///
/// @throws   TsInitialization Exception
///
/// @details  This checks the given function return enum for errors.  For good status code, nothing
///           is done.  For any error status, an exception is thrown and an error message is sent to
///           H&S.
///
///           This overloaded method services the CUDA runtime library return type, cudaError_t.  We
///           put the integer value of the returned enum into the return string.  Meanings of the
///           values can be found in the CUDA runtime library documentation for the cudaError enum.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CudaIfUtils::checkReturn(cudaError_t err, const char *file, const int line)
{
    if(cudaSuccess != err) {
        std::ostringstream msg;
        msg << "line " << line << ", cuda error " << err << ": " << cudaGetErrorString(err);
        throw(TsInitializationException("", file, msg.str()));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] err  (--) cusparse library return code enumeration.
/// @param[in] file (--) Source code file string from the caller.
/// @param[in] line (--) Source code line number from the caller.
///
/// @throws   TsInitialization Exception
///
/// @details  This checks the given function return enum for errors.  For good status code, nothing
///           is done.  For any error status, an exception is thrown and an error message is sent to
///           H&S.
///
///           This overloaded method services the CUDA cusparse library return type,
///           cusparseStatus_t.  Meanings of the values can be found in the cusparse runtime library
///           documentation for the cusparseStatus_t enum.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CudaIfUtils::checkReturn(cusparseStatus_t err, const char *file, const int line)
{
    if(CUSPARSE_STATUS_SUCCESS != err) {
        std::ostringstream msg;
        msg << "line " << line << ", cusparse error " << err;
        throw(TsNumericalException("", file, msg.str()));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] err  (--) cusolver library return code enumeration.
/// @param[in] file (--) Source code file string from the caller.
/// @param[in] line (--) Source code line number from the caller.
///
/// @throws   TsInitialization Exception
///
/// @details  This checks the given function return enum for errors.  For good status code, nothing
///           is done.  For any error status, an exception is thrown and an error message is sent to
///           H&S.
///
///           This overloaded method services the CUDA cusolver library return type,
///           cusolverStatus_t.  Meanings of the values can be found in the cusolver runtime library
///           documentation for the cusolverStatus_t enum.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CudaIfUtils::checkReturn(cusolverStatus_t err, const char *file, const int line)
{
    if(CUSOLVER_STATUS_SUCCESS != err) {
        std::ostringstream msg;
        msg << "line " << line << ", cusolver error " << err;
        throw(TsNumericalException("", file, msg.str()));
    }
}
