/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include "QuadLinFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Bivariate Quadratic, Linear curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadLinFit::QuadLinFit()
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
/// @details  Constructs this Bivariate Quadratic, Linear curve fit model taking coefficient and
///           range arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadLinFit::QuadLinFit(const double a,    const double b,    const double c,
                       const double d,    const double e,    const double f,
                       const double minX, const double maxX,
                       const double minY, const double maxY,
                       const std::string &name)
    :
    TsApproximation(),
    mA(a),
    mB(b),
    mC(c),
    mD(d),
    mE(e),
    mF(f)
{
    init(a, b, c, d, e, f,minX, maxX, minY, maxY, name );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Bivariate Quadratic, Linear curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadLinFit::~QuadLinFit()
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
/// @details  Initializes this Bivarirate Quadratic, Linear curve fit model taking coefficient, range and
///           name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void QuadLinFit::init(const double a,    const double b,    const double c,
        const double d,    const double e,    const double f,
        const double minX, const double maxX,
        const double minY, const double maxY,
        const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, minY, maxY, name);

    /// - Initialize the coefficients.
    mA = a;
    mB = b;
    mC = c;
    mD = d;
    mE = e;
    mF = f;
}
