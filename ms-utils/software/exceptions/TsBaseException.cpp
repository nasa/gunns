/**
@file
@brief     Base exception implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
()
*/

#include "TsBaseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name     (--)  The name of this exception.
/// @param[in]  message  (--)  The description of this exception.
/// @param[in]  thrower  (--)  The thrower (class, object or function) of this exception.
/// @param[in]  cause    (--)  The root cause of this exception.
///
/// @details  Constructor for this base exception used to set a message containing a description or
///           sub-type of the exception being thrown, the name of the thrower (class, object or
///           function) throwing the exception and a root cause or the physical condition that
///           resulted in the exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBaseException::TsBaseException(const std::string& name,
                                 const std::string& message,
                                 const std::string& thrower,
                                 const std::string& cause) throw ()
    :
    std::exception(),
    mName(name),
    mMessage(message),
    mThrower(thrower),
    mCause(cause),
    mWhat(name + ", " + message + " thrown by " + thrower + " because " + cause)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Reference to object to copy from.
///
/// @details    Copy constructs this base exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBaseException::TsBaseException(const TsBaseException &that) throw ()
    :
    std::exception(that),
    mName(that.mName),
    mMessage(that.mMessage),
    mThrower(that.mThrower),
    mCause(that.mCause),
    mWhat(that.mWhat)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this base exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBaseException::~TsBaseException() throw ()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Reference to object to assign from.
///
/// @return     Reference to object assigned to.
///
/// @details    Assigns this base exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBaseException& TsBaseException::operator =(const TsBaseException& that) throw ()
{
    if (this != &that) {
        this->exception::operator =(that);
        mName    = that.mName;
        mMessage = that.mMessage;
        mThrower = that.mThrower;
        mCause   = that.mCause;
        mWhat    = that.mWhat;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     string  (--)  A string representation of this exception.
///
/// @details    Gets the string containing the concatenated attributes of this exception
///             formatted as "<mName>, <mMessage> thrown by <mThrower> because <mCause>"
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string TsBaseException::toString() const throw ()
{
    return (mName + ", " + mMessage + " thrown by " + mThrower + " because " + mCause);
}
