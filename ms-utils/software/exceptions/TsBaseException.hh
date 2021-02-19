#ifndef TsBaseException_EXISTS
#define TsBaseException_EXISTS

/**
@file
@brief     Base exception declarations.

@defgroup  TSM_UTILITIES_SOFTWARE_EXCEPTIONS_BASE Base Exception
@ingroup   TSM_UTILITIES_SOFTWARE_EXCEPTIONS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the base class for all exceptions defined in the TS21 project.)

@brief
LIBRARY DEPENDENCY:
-((TsBaseException.o))

PROGRAMMERS:
-(((Jeff Wilson) (L3) (May 2008) (CxTF)))
@{
*/

#include <stdexcept>
#include <string>

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Exception base class.
///
/// @details  This class is the base class for all exceptions used in Ts21. All new exceptions
///           should derive from this class.
///
///           The methods in this class are designed to give a little information or a lot of
///           information, depending upon the needs of the using class.
///
///           The user of these exceptions should use them to convey as much detail as is available
///           to its users to fully describe the conditions that resulted in the exception. These
///           exceptions will be useful to ensuring that the code will not break if error conditions
///           arise and that error conditions are handled appropriately when they do occur.
///
///           Note that there are several empty "throw" statements: throw(). This means that the
///           methods are not allowed to throw exceptions. Therefore, if an exception occurs in
///           those methods, execution will cease. This is  desired since the exception framework
///           defined here is specifically designed to log issues. If it dies, we need to know why
///           and fix it before running the sim.
///
/// @par      Example 1 (specify at construction):
/// @code
///             LIBRARY DEPENDENCY:
///                 ((software/exceptions/TsInitializationException.o))
///              ...
///             #include "software/exceptions/TsInitializationException.hh"
///             ...
///             throw TsInitializationException("Invalid Initialization Data",
///                                             "earth",
///                                             "surface temperature < 0");
/// @endcode
/// @par      Example 2 (default construct and specify with setters):
/// @code
///             LIBRARY DEPENDENCY:
///                 ((software/exceptions/TsInitializationException.o))
///             ...
///             #include "software/exceptions/TsInitializationException.hh"
///             ...
///             TsInitializationException e;
///             e.setMessage("Invalid Initialization Data");
///             e.setThrowingEntityName("earth");
///             e.setCause("surface temperature < 0");
///             throw e;
/// @endcode
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsBaseException: public std::exception
{
    TS_MAKE_SIM_COMPATIBLE(TsBaseException);
    public:
        /// @brief  Constructs this exception, specifying the name, description or sub-type,
        ///         thrower and root cause.
        TsBaseException(const std::string& name    = "",
                        const std::string& message = "",
                        const std::string& thrower = "",
                        const std::string& cause   = "") throw ();
        /// @brief  Copy constructs this exception.
        TsBaseException(const TsBaseException &that) throw ();
        /// @brief  Default destructs this exception.
        virtual ~TsBaseException() throw ();
        /// @brief  Assigns this exception.
        TsBaseException& operator =(const TsBaseException& that) throw ();
        /// @brief  Gets the concatenated attributes of this exception.
        virtual const char* what() const throw();
        /// @brief  Gets the name of this exception.
        std::string getName() const throw ();
        /// @brief  Gets the description or sub-type of this exception.
        std::string getMessage() const throw ();
        /// @brief  Gets the name of the thrower (class, object or function) of this exception.
        std::string getThrowingEntityName() const throw ();
        /// @brief  Gets the root cause of this exception.
        std::string getCause() const throw ();
        /// @brief  Gets the attributes of this exception.
        std::string toString() const throw ();
        /// @brief  Sets the description or sub-type of this exception.
        void setMessage(const std::string& message) throw ();
        /// @brief  Sets the name of the thrower (class, object or function) of this exception.
        void setThrowingEntityName(const std::string& throwerName) throw ();
        /// @brief  Sets the root cause of this exception.
        void setCause(const std::string& rootCause) throw ();
    protected:
        /// @brief  The name of this exception.
        std::string mName;
        /// @brief  The description or sub-type of this exception.
        std::string mMessage;
        /// @brief  The name of the thrower (class, object or function) of this exception.
        std::string mThrower;
        /// @brief  The root cause of this exception.
        std::string mCause;
        /// @brief  The concatenated attributes of this exception.
        std::string mWhat;
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     string  (--)  The name of this exception.
///
/// @details    Gets the string containing the name of this exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::string TsBaseException::getName() const throw ()
{
    return mName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     string  (--)  The description or sub-type of this exception.
///
/// @details    Gets the string containing the description or sub-type of this exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::string TsBaseException::getMessage() const throw ()
{
    return mMessage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     string  (--)  The thrower of this exception.
///
/// @details    Gets the string containing the name of the thrower (class, object, function) of this exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::string TsBaseException::getThrowingEntityName() const throw ()
{
    return mThrower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     string  (--)  The root cause of this exception.
///
/// @details    Gets the string containing the root cause of this exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::string TsBaseException::getCause() const throw ()
{
    return mCause;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  message  (--)  The description or sub-type of this exception.
///
/// @return     void
///
/// @details    Sets the string containing the description or sub-type of this exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsBaseException::setMessage(const std::string& message) throw ()
{
    mMessage = message;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  thrower  (--)  The thrower of this exception.
///
/// @return     void
///
/// @details    Sets the string containing the name of the thrower (class, object, function) of this exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsBaseException::setThrowingEntityName(const std::string& thrower) throw ()
{
    mThrower = thrower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  cause  (--)  The root cause of this exception.
///
/// @return     void
///
/// @details    Sets the string containing the root cause of this exception and updates the
///             concatenated attributes string.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TsBaseException::setCause(const std::string& cause) throw ()
{
    mCause = cause;
    mWhat  = mName + ", " + mMessage + " thrown by " + mThrower + " because " + mCause;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     const char*  (--)  A string representation of this exception.
///
/// @warning    The value being returned is only generated by the constructor and setCause methods.
///
/// @details    Gets the const char* containing the concatenated attributes of this exception
///             formatted as "<mName>, <mMessage> thrown by <mThrower> because <mCause>"
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* TsBaseException::what() const throw ()
{
    return mWhat.c_str();
}

#endif
