/************************** TRICK HEADER ***********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "LinearFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Linear curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
LinearFit::LinearFit()
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
/// @details  Constructs this Linear curve fit model taking coefficient and range arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
LinearFit::LinearFit(const double a,    const double b,
                     const double minX, const double maxX,
                     const std::string &name)
    :
    TsApproximation(),
    mA(a),
    mB(b)
{
    init(a, b, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Linear Curve Fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
LinearFit::~LinearFit()
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
/// @details  Initializes this Linear curve fit model taking coefficient, range and name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void LinearFit::init(const double a,    const double b,
                   const double minX, const double maxX,
                   const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Initialize the coefficients.
    mA = a;
    mB = b;
}
