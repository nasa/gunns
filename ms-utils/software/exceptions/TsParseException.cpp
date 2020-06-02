/**
@file
@brief     Parse exception implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
((TsBaseException.o))
*/

#include "TsParseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  message  (--)  The description of this exception.
/// @param[in]  thrower  (--)  The thrower (class, object or function) of this exception.
/// @param[in]  cause    (--)  The root cause of this exception.
///
/// @details    Constructor for this parse exception used to set a message containing a description or
///             sub-type of the exception being thrown, the name of the thrower (class, object or
///             function) throwing the exception and a root cause or the physical condition that
///             resulted in the exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsParseException::TsParseException(const std::string& message,
                                   const std::string& thrower,
                                   const std::string& cause) throw ()
    :
    TsBaseException("TsParseException", message, thrower, cause)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Reference to object to copy from.
///
/// @details    Copy constructs this parse exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsParseException::TsParseException(const TsParseException& that) throw ()
    :
    TsBaseException(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this parse exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsParseException::~TsParseException() throw ()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Reference to object to assign from.
///
/// @return     Reference to object assigned to.
///
/// @details    Assigns this parse exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsParseException& TsParseException::operator =(const TsParseException& that) throw ()
{
    if (this != &that) {
        this->TsBaseException::operator=(that);
    }
    return *this;
}
