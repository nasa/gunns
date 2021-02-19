/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>
#include <cmath>

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"

#include "RationalFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Rational curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
RationalFit::RationalFit()
    :
    TsApproximation(),
    mA(0.0),
    mB(0.0),
    mC(0.0),
    mD(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    d     (--) Fourth coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @details  Constructs this Rational curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
RationalFit::RationalFit(const double a,    const double b,    const double c,    const double d,
                         const double minX, const double maxX, const std::string &name)
    :
    TsApproximation(minX, maxX, -FLT_EPSILON, +FLT_EPSILON),
    mA(a),
    mB(b),
    mC(c),
    mD(d)
{
    init(a, b, c, d, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Rational curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
RationalFit::~RationalFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    d     (--) Fourth coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Rational curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void RationalFit::init(const double a,    const double b,    const double c,    const double d,
                       const double minX, const double maxX, const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Throw a TsInitializationException exception on a singularity (divide by 0) in the allegedly valid range.
    const double discrim = c * c - 4 * d;
    if (discrim > FLT_EPSILON) {
        // Either check that both real roots are sufficiently outside valid range
        const double arg = 0.5 * sqrt(discrim);
        TS_GENERIC_IF_ERREX((minX <= -c + arg + FLT_EPSILON && -c + arg - FLT_EPSILON <= maxX), TsInitializationException,
                            "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");
        TS_GENERIC_IF_ERREX((minX <= -c - arg + FLT_EPSILON && -c - arg - FLT_EPSILON <= maxX), TsInitializationException,
                            "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");
    } else if (discrim > -FLT_EPSILON) {
        // Or check that an (almost) single real root is sufficiently outside valid range
        TS_GENERIC_IF_ERREX((minX <= -0.5 * c + FLT_EPSILON && -0.5 * c - FLT_EPSILON < maxX), TsInitializationException,
                            "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");
    }

    /// - Initialize the coefficients with validated values.
    mA = a;
    mB = b;
    mC = c;
    mD = d;

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
