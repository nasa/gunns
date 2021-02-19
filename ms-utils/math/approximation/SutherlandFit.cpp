/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"

#include "SutherlandFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Sutherland curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
SutherlandFit::SutherlandFit()
    :
    TsApproximation(),
    mA(0.0),
    mB(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @details  Constructs this Sutherland curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
SutherlandFit::SutherlandFit(const double a,    const double b,
                             const double minX, const double maxX, const std::string &name)
    :
    TsApproximation(),
    mA(a),
    mB(b)
{
    init(a, b, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sutherland curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
SutherlandFit::~SutherlandFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Sutherland curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void SutherlandFit::init(const double a,    const double b,
                         const double minX, const double maxX, const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Throw a TsInitializationException exception on a singularity (divide by 0) in the allegedly valid range.
    TS_GENERIC_IF_ERREX((b + mMinX < FLT_EPSILON && -FLT_EPSILON < b - mMaxX), TsInitializationException,
                        "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");

    /// - Initialize the coefficients with validated values.
    mA = a;
    mB = b;

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
