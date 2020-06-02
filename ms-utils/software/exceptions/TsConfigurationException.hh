#ifndef TsConfigurationException_EXISTS
#define TsConfigurationException_EXISTS

/**
@file
@brief     Configuration exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_CONFIGURATION Configuration Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the exception class used to represent issues that occur when input data is not found.)

@brief
LIBRARY DEPENDENCY:
-((TsConfigurationException.o))

PROGRAMMERS:
-(((Jeff Wilson) (L3) (May 2008) (CxTF)))
@{
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception used when input data cannot be found.
///
/// @details  This should be used in situations where input data is not found. This may result from
///           a null pointer, bad path or url to the supplier of the data. For malformed input data,
///           use TsParseException and for invalid input data use TsInitializationException.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsConfigurationException: public TsBaseException
{
    TS_MAKE_SIM_COMPATIBLE(TsConfigurationException);
    public:
        /// @brief  Constructs this exception, specifying the description or sub-type, thrower and
        ///         root cause.
        TsConfigurationException(const std::string& message = "",
                                 const std::string& thrower = "",
                                 const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsConfigurationException(const TsConfigurationException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsConfigurationException() throw ();
        /// @brief  Assigns this exception.
        TsConfigurationException& operator =(const TsConfigurationException& that) throw ();
};

/// @}

#endif
