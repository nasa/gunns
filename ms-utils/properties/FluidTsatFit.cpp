/**
@file
@brief    Fluid Saturation Temperature Curve Fit implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((math/approximation/TsApproximation.o))
*/

#include "FluidTsatFit.hh"

#include <cfloat>

#include "GenericMacros.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Saturation Temperature curve fit model .
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidTsatFit::FluidTsatFit()
    :
    TsApproximation(),
    mC2(0.0),
    mC4(0.0),
    mB(0.0),
    mB2(0.0),
    mA(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) 0th-order polynomial coefficient for curve fit model.
/// @param[in]    b     (--) 1st-order polynomial coefficient for curve fit model.
/// @param[in]    c     (--) 2nd-order polynomial coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @throws   TsInitializationException
///
/// @note     The arguments are the coefficients of the corresponding quadratic equation.
///
/// @details  Constructs this Fluid Saturation Temperature curve fit model taking coefficient and
///           range arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidTsatFit::FluidTsatFit(const double a,    const double b,    const double c,
                           const double minX, const double maxX,
                           const std::string &name)
    :
    TsApproximation(minX, maxX, -FLT_EPSILON, +FLT_EPSILON),
    mC2(0.0),
    mC4(0.0),
    mB(0.0),
    mB2(0.0),
    mA(0.0)
{
    init(a, b, c, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Saturation Temperature curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidTsatFit::~FluidTsatFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) 0th-order polynomial coefficient for curve fit model.
/// @param[in]    b     (--) 1st-order polynomial coefficient for curve fit model.
/// @param[in]    c     (--) 2nd-order polynomial coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Initializes this Fluid Saturation Temperature curve fit model taking coefficient,
///           range and name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void FluidTsatFit::init(const double a,    const double b,    const double c,
                        const double minX, const double maxX,
                        const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Build a message string with the arguments in it to include in error messages.
    std::ostringstream args;
    args << " Args: " << a << " " << b << " " << c << " " << minX << " " << maxX << ".";

    /// - Throw a TsInitializationException exception on a divide by 0 in the arguments.
    TS_GENERIC_IF_ERREX((fabs(c) < DBL_EPSILON), TsInitializationException,
                        "Invalid Input Argument", "abs(c) < DBL_EPSILON." + args.str());

    /// - Initialize the coefficients with validated values.
    mC2 = 0.5 / c;
    mC4 = 4.0 * c;
    mB  = -b;
    mB2 = b * b;
    mA  = a;

    /// - Throw a TsInitializationException exception on square root of negative number at minX.
    TS_GENERIC_IF_ERREX(mB2 - mC4 * (mA - log10(minX)) < 0.0, TsInitializationException,
                        "Invalid Input Argument", "square root of negative number at minX." + args.str());

    /// - Throw a TsInitializationException exception on square root of negative number at maxX.
    TS_GENERIC_IF_ERREX(mB2 - mC4 * (mA - log10(maxX)) < 0.0, TsInitializationException,
                        "Invalid Input Argument", "square root of negative number at maxX." + args.str());

    /// - Throw a TsInitializationException exception if minX evaluates to <= 0.
    TS_GENERIC_IF_ERREX(evaluate(minX) < DBL_EPSILON, TsInitializationException,
                        "Invalid Input Argument", "evaluates to <= zero at minX." + args.str());

    /// - Throw a TsInitializationException exception if maxX evaluates to <= 0.
    TS_GENERIC_IF_ERREX(evaluate(maxX) < DBL_EPSILON, TsInitializationException,
                        "Invalid Input Argument", "evaluates to <= zero at maxX." + args.str());

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
