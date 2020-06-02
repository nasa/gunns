#ifndef TsInvalidStateException_EXISTS
#define TsInvalidStateException_EXISTS

/**
@file
@brief     Invalid State Machine State exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_STATE Invalid State Machine State Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the exception class used to represent an invalid state in a state machine.)

@brief
LIBRARY DEPENDENCY:
-((TsInvalidStateException.o))

PROGRAMMERS:
-((Robert G. Phillips) (Tietronix) (Apr 2012) (TS21))
@{
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception used to represent an invalid state in a state machine.
///
/// @details  Since, generality, the StateMachine class uses ints as state Ids, whether an Id is
///           valid or not is determined by the instantiated subclasses of StateFactory and
///           StateMachineState, as they are called by StateMachine.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsInvalidStateException: public TsBaseException
{
    TS_MAKE_SIM_COMPATIBLE(TsInvalidStateException);
    public:
        /// @brief  Constructs this exception, specifying the description or sub-type, thrower and
        ///         root cause.
        TsInvalidStateException(const std::string& message = "",
                                const std::string& thrower = "",
                                const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsInvalidStateException(const TsInvalidStateException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsInvalidStateException() throw ();
        /// @brief  Assigns this exception.
        TsInvalidStateException& operator =(const TsInvalidStateException& that) throw ();
};

/// @}

#endif
