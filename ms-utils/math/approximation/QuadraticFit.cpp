/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "QuadraticFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Quadratic curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadraticFit::QuadraticFit()
    :
    TsApproximation(),
    mA(0.0),
    mB(0.0),
    mC(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Constructs this Quadratic curve fit model taking coefficient and range arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadraticFit::QuadraticFit(const double a,    const double b,    const double c,
                           const double minX, const double maxX,
                           const std::string &name)
    :
    TsApproximation(),
    mA(a),
    mB(b),
    mC(c)
{
    init(a, b, c, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Quadratic curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuadraticFit::~QuadraticFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    b     (--) Second coefficient for curve fit model.
/// @param[in]    c     (--) Third coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Initializes this Quadratic curve fit model taking coefficient, range and
///           name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void QuadraticFit::init(const double a,    const double b,    const double c,
                        const double minX, const double maxX,
                        const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Initialize the coefficients.
    mA = a;
    mB = b;
    mC = c;
}
