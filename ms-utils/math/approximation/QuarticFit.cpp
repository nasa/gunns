/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "QuarticFit.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Quartic curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuarticFit::QuarticFit()
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
/// @details  Constructs this Quartic curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuarticFit::QuarticFit(const double a,    const double b,    const double c,
                       const double d,    const double e,
                       const double minX, const double maxX, const std::string &name)
    :
    TsApproximation(minX, maxX, -FLT_EPSILON, +FLT_EPSILON),
    mA(a),
    mB(b),
    mC(c),
    mD(d),
    mE(e)
{
    init(a, b, c, d, e, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Quartic curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuarticFit::~QuarticFit()
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
/// @details  Initializes this Quartic curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void QuarticFit::init(const double a,    const double b,    const double c,
                      const double d,    const double e,
                      const double minX, const double maxX, const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Initialize the coefficients.
    mA = a;
    mB = b;
    mC = c;
    mD = d;
    mE = e;
}

