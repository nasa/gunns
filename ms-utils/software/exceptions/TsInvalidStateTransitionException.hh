#ifndef TsInvalidStateTransitionException_EXISTS
#define TsInvalidStateTransitionException_EXISTS

/**
@file
@brief     Invalid State Machine State Transition exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_TRANSITION State Machine Invalid State Transition Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the exception class used to represent an invalid state transition in a state machine.)

@brief
LIBRARY DEPENDENCY:
-((TsInvalidStateTransitionException.o))

PROGRAMMERS:
-((Robert G. Phillips) (Tietronix) (Apr 2012) (TS21))
@{
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception used to define the condition when a state machine is given an invalid state
///           transition.
///
/// @details  This exception is thrown when the StateMachine is asked to transition to a new state,
///           and that state is not allowed from the current state.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsInvalidStateTransitionException: public TsBaseException
{
    TS_MAKE_SIM_COMPATIBLE(TsInvalidStateTransitionException);
    public:
        /// @brief  Constructs this exception, specifying the description or sub-type, thrower and
        ///         root cause.
        TsInvalidStateTransitionException(const std::string& message = "",
                                          const std::string& thrower = "",
                                          const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsInvalidStateTransitionException(const TsInvalidStateTransitionException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsInvalidStateTransitionException() throw ();
        /// @brief  Assigns this exception.
        TsInvalidStateTransitionException& operator =(const TsInvalidStateTransitionException& that) throw ();
};

/// @}

#endif
