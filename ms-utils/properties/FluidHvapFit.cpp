/**
@file
@brief    Fluid Heat of Vaporization Curve Fit implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((math/approximation/TsApproximation.o))
*/

#include "FluidHvapFit.hh"

#include "GenericMacros.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Heat of Vaporization curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidHvapFit::FluidHvapFit()
    :
    TsApproximation(),
    mA(0.0),
    mAlpha(0.0),
    mBeta(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    alpha (--) Second coefficient for curve fit model.
/// @param[in]    beta  (--) Beta coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Constructs this Fluid Heat of Vaporization curve fit model taking coefficient and
///           range arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidHvapFit::FluidHvapFit(const double a,
                           const double alpha, const double beta,
                           const double minX,  const double maxX,
                           const std::string &name)
    :
    TsApproximation(minX, maxX, -FLT_EPSILON, +FLT_EPSILON),
    mA(a),
    mAlpha(alpha),
    mBeta(beta)
{
    init(a, alpha, beta, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Heat of Vaporization curve fit model.
////////////////////////////////////////////////////////////////////////////////////////////////////
FluidHvapFit::~FluidHvapFit()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    a     (--) First coefficient for curve fit model.
/// @param[in]    alpha (--) Second coefficient for curve fit model.
/// @param[in]    beta  (--) Beta coefficient for curve fit model.
/// @param[in]    minX  (--) Curve fit model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Curve fit model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Initializes this Fluid Heat of Vaporization curve fit model taking coefficient, range
///           and name arguments
////////////////////////////////////////////////////////////////////////////////////////////////////
void FluidHvapFit::init(const double a,
                        const double alpha, const double beta,
                        const double minX,  const double maxX,
                        const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Initialize the coefficients.
    mA     = a;
    mAlpha = alpha;
    mBeta  = beta;

    /// - Throw a TsInitializationException exception on magnitude of alpha too large.
    TS_GENERIC_IF_ERREX(fabs(alpha) > 50.0, TsInitializationException,
                        "Invalid Input Argument", "magnitude of alpha > 50.");

    /// - Throw a TsInitializationException exception on magnitude of beta too large.
    TS_GENERIC_IF_ERREX(fabs(beta) > 50.0, TsInitializationException,
                        "Invalid Input Argument", "magnitude of beta > 50.");

    /// - Set the initialization complete flag.
    mInitFlag = true;
}
