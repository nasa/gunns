/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include "ProductFit.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Bivariate Product curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
ProductFit::ProductFit()
    :
    TsApproximation(),
    mA(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    minY  (--) Curve fit model valid range lower limit for second variable.
/// @param[in]    maxY  (--) Curve fit model valid range upper limit for second variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Constructs this Bivariate Product curve fit model taking coefficient and range
///           arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
ProductFit::ProductFit(const double a,
                       const double minX, const double maxX,
                       const double minY, const double maxY,
                       const std::string &name)
    :
    TsApproximation(),
    mA(a)
{
    init(a, minX, maxX, minY, maxY, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Bivariate Product curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
ProductFit::~ProductFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    minY  (--) Curve fit model valid range lower limit for second variable.
/// @param[in]    maxY  (--) Curve fit model valid range upper limit for second variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Initializes this Product curve fit model taking coefficient, range and name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void ProductFit::init(const double a,
                      const double minX, const double maxX,
                      const double minY, const double maxY,
                      const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, minY, maxY, name);

    /// - Initialize the coefficient.
    mA = a;
}
