/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
 (
     (simulation/hs/TsHsMngr.o)
     (software/exceptions/TsInitializationException.o)
     (software/exceptions/TsOutOfBoundsException.o)
 )
 **************************************************************************************************/

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "GenericMacros.hh"

#include "TsApproximation.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this curve fit/interpolator approximation.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsApproximation::TsApproximation()
    :
    mMinX(0.0),
    mMaxX(0.0),
    mMinY(0.0),
    mMaxY(0.0),
    mInitFlag(false),
    mName()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    minX  (--) approximation valid range lower limit for first variable.
/// @param[in]    maxX  (--) approximation valid range upper limit for first variable.
/// @param[in]    minY  (--) approximation valid range lower limit for second variable.
/// @param[in]    maxY  (--) approximation valid range upper limit for second variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @details  Constructs this approximation taking range arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsApproximation::TsApproximation(const double minX, const double maxX,
        const double minY, const double maxY, const std::string &name)
        :
        mMinX(minX),
        mMaxX(maxX),
        mMinY(minY),
        mMaxY(maxY),
        mInitFlag(false),
        mName()
{
    init(minX, maxX, minY, maxY, name);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this curve fit/interpolator approximation.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsApproximation::~TsApproximation()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    minX  (--) approximation valid range lower limit for first variable.
/// @param[in]    maxX  (--) approximation valid range upper limit for first variable.
/// @param[in]    minY  (--) approximation valid range lower limit for second variable.
/// @param[in]    maxY  (--) approximation valid range upper limit for second variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @details  Initializes approximation taking range arguments and a name
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsApproximation::init(const double minX, const double maxX,
    const double minY, const double maxY, const std::string &name)
{
    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Allocate and initialize the object name or throw a TsInitializationException exception if empty.
    TS_GENERIC_NAME_ERREX(name);

    /// - Throw a TsInitializationException exception on empty valid range for first variable.
    TS_GENERIC_IF_ERREX((minX >= maxX), TsInitializationException,
                        "Invalid Input Argument", "Empty valid range for first variable.");

    /// - Throw a TsInitializationException exception on empty valid range for second variable.
    TS_GENERIC_IF_ERREX((minY >= maxY), TsInitializationException,
                        "Invalid Input Argument", "Empty valid range for second variable.");

    /// - Initialize the coefficients with validated values.
    mMinX     = minX;
    mMaxX     = maxX;
    mMinY     = minY;
    mMaxY     = maxY;

    /// - Set the initialization complete flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for curve fit/interpolation.
/// @param[in]    y  (--)  Second independent variable for curve fit/interpolation.
///
/// @return   Fitted variable at specified input.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns this approximation for the specified variables with out of range exception.
////////////////////////////////////////////////////////////////////////////////////////////
double TsApproximation::getExceptional(const double x, const double y)
{
    /// - Throw a TsOutOfBoundsException exception if first independent variable is out of range.
    TS_GENERIC_IF_ERREX((x < mMinX || x > mMaxX), TsOutOfBoundsException,
                        "Invalid Input Argument", "First independent variable (x) is outside valid range.");

    /// - Throw a TsOutOfBoundsException exception if second independent variable is out of range.
    TS_GENERIC_IF_ERREX((y < mMinY || y > mMaxY), TsOutOfBoundsException,
                        "Invalid Input Argument", "Second independent variable (y) is outside valid range.");

    return evaluate(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Initialization status
///
/// @details  Returns the current initialization status.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsApproximation::isInitialized() const
{
    return mInitFlag;
}

