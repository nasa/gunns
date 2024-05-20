/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>
#include <cmath>

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"

#include "QuadLinInvFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Bivariate Quadratic, Linear Inverse curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadLinInvFit::QuadLinInvFit()
    :
    TsApproximation(),
    mA(0.0),
    mB(0.0),
    mC(0.0),
    mD(0.0),
    mE(0.0),
    mF(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    d     (--) Fourth coefficient for curve fit model.
/// @param[in]    e     (--) Fifth coefficient for curve fit model.
/// @param[in]    f     (--) Sixth coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    minY  (--) Curve fit model valid range lower limit for second variable.
/// @param[in]    maxY  (--) Curve fit model valid range upper limit for second variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @details  Constructs this Bivariate Quadratic, Linear Inverse curve fit model taking
///           coefficient and range arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadLinInvFit::QuadLinInvFit(const double a,    const double b,    const double c,
                             const double d,    const double e,    const double f,
                             const double minX, const double maxX,
                             const double minY, const double maxY,
                             const std::string &name)
    :
    TsApproximation(minX, maxX, minY, maxY),
    mA(a),
    mB(b),
    mC(c),
    mD(d),
    mE(e),
    mF(f)
{
    init(a, b, c, d, e, f, minX, maxX, minY, maxY, name );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Bivariate Quadratic, Linear Inverse curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadLinInvFit::~QuadLinInvFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    d     (--) Fourth coefficient for curve fit model.
/// @param[in]    e     (--) Fifth coefficient for curve fit model.
/// @param[in]    f     (--) Sixth coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    minY  (--) Curve fit model valid range lower limit for second variable.
/// @param[in]    maxY  (--) Curve fit model valid range upper limit for second variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Initializes this Bivariate Quadratic, Linear curve fit model taking coefficient, range and
///           name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void QuadLinInvFit::init(const double a,    const double b,    const double c,
                         const double d,    const double e,    const double f,
                         const double minX, const double maxX,
                         const double minY, const double maxY,
                         const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, minY, maxY, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Throw a TsInitializationException exception on a singularity (divide by 0) in the allegedly valid range.
    const double discrim = d * d - 4 * f;
    const double flt_epsilon = static_cast<double>(FLT_EPSILON);
    if (discrim > flt_epsilon) {
        // Either check that both real roots are sufficiently outside valid range
        const double arg = 0.5 * sqrt(discrim);
        TS_GENERIC_IF_ERREX((minX <= -d + arg + flt_epsilon && -d + arg - flt_epsilon <= maxX), TsInitializationException,
                            "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");
        TS_GENERIC_IF_ERREX((minX <= -d - arg + flt_epsilon && -d - arg - flt_epsilon <= maxX), TsInitializationException,
                            "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");
    } else if (discrim > -flt_epsilon) {
        // Or check that an (almost) single real root is sufficiently outside valid range
        TS_GENERIC_IF_ERREX((minX <= -0.5 * d + flt_epsilon && -0.5 * d - flt_epsilon < maxX), TsInitializationException,
                            "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");
    }

    /// - Initialize the coefficients with validated values.
    mA = a;
    mB = b;
    mC = c;
    mD = d;
    mE = e;
    mF = f;

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
