#ifndef TsInitializationException_EXISTS
#define TsInitializationException_EXISTS

/**
@file
@brief     Initialization exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_INITIALIZATION Initialization Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the exception class used to represent issues that occur when attempting to
            initialize models with data that is invalid.)

@brief
LIBRARY DEPENDENCY:
-((TsInitializationException.o))

PROGRAMMERS:
-(((Jeff Wilson) (L3) (May 2008) (CxTF)))
@{
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception used to represent issues that occur when attempting to initialize models
///           with data that is invalid.
///
/// @details  Initialization data is defined as data that is mission/flight/scenario-specific
///           (generally constant). As such, the data:
///               Does not generally change during simulation execution.
///               Might change from run to run.
///           Therefore, users will need to change them.
///
///           In some cases, it might be useful to capture certain initial state values as constant
///           input parameters. This allows saving of the initial state for later comparison.
///
///           This catagory of data is treated separately in our code from data (called
///           initialization) data, that DOES change during a sim.
///
///           This exception is specifically created for use in setting these specific data values.
///
///           For reference on Configuration and Initialization params, see the TS-21 M&S wiki for
///           details.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsInitializationException: public TsBaseException
{
    TS_MAKE_SIM_COMPATIBLE(TsInitializationException);
    public:
        /// @brief  Constructs this exception, specifying the description or sub-type, thrower and
        ///         root cause.
        TsInitializationException(const std::string& message = "",
                                  const std::string& thrower = "",
                                  const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsInitializationException(const TsInitializationException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsInitializationException() throw ();
        /// @brief  Assigns this exception.
        TsInitializationException& operator =(const TsInitializationException& that) throw ();
};

/// @}

#endif
