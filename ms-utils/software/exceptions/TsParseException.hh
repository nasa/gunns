#ifndef TsParseException_EXISTS
#define TsParseException_EXISTS

/**
@file
@brief     Parse exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_PARSE Parse Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the exception class used to represent issues that occur when attempting to
            parse files, especially xml.)

@brief
LIBRARY DEPENDENCY:
-((TsParseException.o))

PROGRAMMERS:
-(((Jeff Wilson) (L3) (May 2008) (CxTF)))
@{
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception used to represent issues that occur when attempting to parse files,
///           especially xml file.
///
/// @details  Exception used to record issues in parsing files that contain information that is
///           required to initialize or update your models. Examples of issues that should use this
///           exception are missing or corrupt files or xml files that are not "well-formed".
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsParseException: public TsBaseException
{
    TS_MAKE_SIM_COMPATIBLE(TsParseException);
    public:
        /// @brief  Constructs this exception, specifying the description or sub-type, thrower and
        ///         root cause.
        TsParseException(const std::string& message = "",
                         const std::string& thrower = "",
                         const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsParseException(const TsParseException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsParseException() throw ();
        /// @brief  Assigns this exception.
        TsParseException& operator =(const TsParseException& that) throw ();
};

/// @}

#endif
