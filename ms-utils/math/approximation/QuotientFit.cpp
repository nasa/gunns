/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
***************************************************************************************************/

#include <cfloat>

#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"

#include "QuotientFit.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Bivariate Quotient curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuotientFit::QuotientFit()
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
/// @throws   TsInitializationException
///
/// @details  Constructs this Bivariate Quotient curve fit model taking coefficient, range and name arguments.
///           arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuotientFit::QuotientFit(const double a,
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
/// @details  Default destructs this Bivariate Quotient curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
QuotientFit::~QuotientFit()
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
/// @throws   TsInitializationException
///
/// @details  Initializes this Bivariate Quotient curve fit model taking coefficient, range and
///           name arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void QuotientFit::init(const double a,
        const double minX, const double maxX,
        const double minY, const double maxY,
        const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, minY, maxY, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Throw a TsInitializationException exception on a singularity (divide by 0) in the allegedly valid range.
    TS_GENERIC_IF_ERREX((mMinX < static_cast<double>(FLT_EPSILON) && -static_cast<double>(FLT_EPSILON) < mMaxX), TsInitializationException,
                        "Invalid Input Argument", "Singularity (divide by 0) in the allegedly valid range.");

    /// - Initialize the coefficient with validated value.
   mA = a;

   /// - Set the initialization complete flag.
   mInitFlag = true;
}
