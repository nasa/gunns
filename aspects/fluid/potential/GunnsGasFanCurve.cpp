/***************************************** TRICK HEADER ********************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
     ((math/root_finding/BrentMethod.o)
      (math/root_finding/LaguerreMethod.o))

 PROGRAMMERS:
     ((Jason Harvey) (L-3 Communications) (2012-07) (Initial Delivery))
 **************************************************************************************************/

#include <cfloat>
#include "GunnsGasFanCurve.hh"
#include "math/MsMath.hh"
#include "math/root_finding/LaguerreMethod.hh"
#include "software/exceptions/TsNumericalException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Gas Fan Curve object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFanCurve::GunnsGasFanCurve()
    :
    BrentMethod(),
    mCoeffs(0),
    mIterLaguerre(0),
    mIterBrent(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Gas Fan Curve object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsGasFanCurve::~GunnsGasFanCurve()
{
    //nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] realRoot   (m3/s) Root to improve
/// @param[in]     realCoeffs (kPa)  Polynomial coefficients to find the root for
/// @param[in]     maxQ       (m3/s) Maximum fan flow rate possible
///
/// @throws  TsNumericalException, if the curve has no real roots between zero and maxQ
///
/// @details Uses Laguerre's method to improve the given real positive root of the fan curve with
///          the given polynomial coefficients.  A well-conditioned fan curve will usually converge
///          within 2-3 Laguerre iterations in a normal network, peaking around 4-5 iterations for
///          large system or fan transients.  If Laguerre's converges on a complex or negative root,
///          then Brent's method is used to find the positive real root.  Brent's method takes
///          longer so is only used as a last resort.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsGasFanCurve::improveRoot(double& realRoot, double* realCoeffs, double maxQ)
{
    mIterBrent    = 0;
    mIterLaguerre = 0;

    try {
        /// - First try Laguerre's method to improve the root.  Load the given coefficients into the
        ///   scalar components of a complex array for Laguerre's method.
        std::complex<double> root = realRoot;
        std::complex<double> coeffs[6];
        for (int order=0; order<6; ++order) {
            coeffs[order] = realCoeffs[order];
        }
        mIterLaguerre = LaguerreMethod::solve(root, coeffs, 6);

        if (MsMath::isInRange(0.0, root.real(), maxQ) and fabs(root.imag()) < DBL_EPSILON) {
            realRoot = root.real();

        } else try {
            /// - If Laguerre failed to converge on a valid root, try Brent's method.  Store the
            ///   given coefficients into the curve object.
            setCoeffs(realCoeffs);
            mIterBrent = solve(realRoot, 0.0, (maxQ + static_cast<double>(FLT_EPSILON)), static_cast<double>(FLT_EPSILON));

        } catch (...) {
            /// - Throw an exception if no suitable root was found.  For a properly configured fan
            ///   curve, this will never happen.
            throw TsNumericalException("Re-throwing", "GunnsGasFanCurve::improveRoot",
                    "BrentMethod threw exception.");
        }

    } catch (...) {
        /// - Throw an exception if Laguerre could not find any roots at all.
        throw TsNumericalException("Re-throwing", "GunnsGasFanCurve::improveRoot",
                "LaguerreMethod threw exception.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] q    (m3/s) Volumetric flow rate to evaluate the fan-system curve delta-pressure for.
///
/// @returns  double (kPa) Delta-pressure produced by the fan at the given flow rate.
///
/// @details  Evaluates the n-order polynomial using stored coefficients.  Overrides the pure
///           virtual method in RootFinding, which provides a target function for the Brent's
///           method.  Although this is a generic implementation of a polynomial and could be
///           considered unit-less, it is only intended for use by the GunnsGasFan class.  The
///           coefficients should point to an array matching the previously given size.  We check
///           for null-pointer in case setCoeffs was never called.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsGasFanCurve::evaluate(const double q)
{
    double result = 0.0;
    for(int order = 0; order < 6; order ++){
        if (mCoeffs) {
            result += mCoeffs[order]*pow(q , order) ;
        }
    }
    return result;
}
