#ifndef TsOutOfBoundsException_EXISTS
#define TsOutOfBoundsException_EXISTS

/**
@file
@brief     Out of Bounds exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_OUTOFBOUNDS Out of Bounds Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the exception class used to define the condition when a component is given a
            value that is out of range for the design of that component.)

@brief
LIBRARY DEPENDENCY:
-((TsOutOfBoundsException.o))

PROGRAMMERS:
-(((Jeff Wilson) (L3) (May 2008) (CxTF)))
@{
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception used to define the condition when a component is given a value that is out
///           of range for the design of that component.
///
/// @details  This should be used in situations where operating conditions cause the component to
///           exceed the design range based upon inputs from other models or resulted in
///           physics-based calculations from external values. This exception is not to be used for
///           initialization issues.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsOutOfBoundsException: public TsBaseException
{
    TS_MAKE_SIM_COMPATIBLE(TsOutOfBoundsException);
    public:
        /// @brief  Constructs this exception, specifying the description or sub-type, thrower and
        ///         root cause.
        TsOutOfBoundsException(const std::string& message = "",
                               const std::string& thrower = "",
                               const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsOutOfBoundsException(const TsOutOfBoundsException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsOutOfBoundsException() throw ();
        /// @brief  Assigns this exception.
        TsOutOfBoundsException& operator =(const TsOutOfBoundsException& that) throw ();
};

/// @}

#endif
