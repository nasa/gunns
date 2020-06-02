#ifndef TsNumericalException_EXISTS
#define TsNumericalException_EXISTS

/**
@file
@brief     Numerical exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_NUMERICAL Numerical Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the exception class used to represent issues that occur when attempting to
            perform calculations that will result in mathematical errors.)

@brief
LIBRARY DEPENDENCY:
-((TsNumericalException.o))

PROGRAMMERS:
-(((Jeff Wilson) (L3) (May 2008) (CxTF)))
@{
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception used to represent issues that occur when attempting to perform calculations
///           that will result in mathematical errors.
///
/// @details  Exception used to record  any undesired math conditions that may occur when
///           performing calculations in the code. For example, situations involving math
///           conditions that are not allowed such as divide by zero, sqrt of -1, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsNumericalException: public TsBaseException
{
    TS_MAKE_SIM_COMPATIBLE(TsNumericalException);
    public:
        /// @brief  Constructs this exception, specifying the description or sub-type, thrower and
        ///         root cause.
        TsNumericalException(const std::string& message = "",
                             const std::string& thrower = "",
                             const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsNumericalException(const TsNumericalException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsNumericalException() throw ();
        /// @brief  Assigns this exception.
        TsNumericalException& operator =(const TsNumericalException& that) throw ();
};

/// @}

#endif
