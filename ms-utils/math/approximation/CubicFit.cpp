/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "CubicFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Cubic curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
CubicFit::CubicFit()
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
/// @details  Constructs this Cubic curve fit model taking coefficient, range and name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
CubicFit::CubicFit(const double a,    const double b,    const double c,    const double d,
                   const double minX, const double maxX, const std::string &name)
    :
    TsApproximation(),
    mA(a),
    mB(b),
    mC(c),
    mD(d)
{
    init(a, b, c, d, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Cubic curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
CubicFit::~CubicFit()
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
/// @details  Initializes this Cubic curve fit model taking coefficient, range and name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void CubicFit::init(const double a,    const double b,    const double c,    const double d,
                   const double minX, const double maxX, const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Initialize the coefficients.
    mA = a;
    mB = b;
    mC = c;
    mD = d;
}
