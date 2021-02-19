#ifndef TsUnknownException_EXISTS
#define TsUnknownException_EXISTS

/**
@file
@brief     Unknown exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_UNKNOWN Unknown Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the exception class used to represent issues when a root cause is unknown, but
            only as a last resort and should be used sparingly.)

@brief
LIBRARY DEPENDENCY:
-((TsUnknownException.o))

PROGRAMMERS:
-(((Jeff Wilson) (L3) (May 2008) (CxTF)))
@{
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception used to represent issues when a root cause is unknown, but only as a last
///           resort and should be used sparingly.)
///
/// @details  Exception used to represent issues that occur when attempting to initialize models
///           with data that cannot be found or loaded. For example, if initializing to a specified
///           data file and the file does not exist or can't be opened, this exception should be
///           used.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsUnknownException: public TsBaseException
{
    TS_MAKE_SIM_COMPATIBLE(TsUnknownException);
    public:
        /// @brief  Constructs this exception, specifying the description or sub-type, thrower and
        ///         root cause.
        TsUnknownException(const std::string& message = "",
                           const std::string& thrower = "",
                           const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsUnknownException(const TsUnknownException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsUnknownException() throw ();
        /// @brief  Assigns this exception.
        TsUnknownException& operator =(const TsUnknownException& that) throw ();
};

/// @}

#endif
