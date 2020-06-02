/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"

#include "ShowmateFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Showmate curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
ShowmateFit::ShowmateFit()
    :
    TsApproximation(),
    mA(0.0),
    mB(0.0),
    mC(0.0),
    mD(0.0),
    mE(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    d     (--) Fourth coefficient for curve fit model.
/// @param[in]    e     (--) Fifth coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @note     The coefficients are for an input argument that will be scaled by 0.001.
///
/// @details  Constructs this Showmate curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
ShowmateFit::ShowmateFit(const double a,    const double b,    const double c,
                         const double d,    const double e,
                         const double minX, const double maxX, const std::string &name)
    :
    TsApproximation(),
    mA(a),
    mB(b),
    mC(c),
    mD(d),
    mE(e)
{
    init(a, b, c, d, e, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Showmate curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
ShowmateFit::~ShowmateFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    d     (--) Fourth coefficient for curve fit model.
/// @param[in]    e     (--) Fifth coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @note     The coefficients are for an input argument that will be scaled by 0.001.
///
/// @details  Initializes this Showmate curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ShowmateFit::init(const double a,    const double b,    const double c,
                       const double d,    const double e,
                       const double minX, const double maxX, const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Throw a TsInitializationException exception on a singularity (divide by 0) in the allegedly valid range.
    TS_GENERIC_IF_ERREX((mMinX < FLT_EPSILON && -FLT_EPSILON < mMaxX), TsInitializationException,
                        "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");

    /// - Initialize the coefficients with validated values.
    mA = a;
    mB = b;
    mC = c;
    mD = d;
    mE = e;

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
