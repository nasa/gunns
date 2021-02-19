#ifndef GunnsGasFanCurve_EXISTS
#define GunnsGasFanCurve_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_POTENTIAL_GAS_FAN_Curve Fan Curve Model
@ingroup   TSM_GUNNS_FLUID_POTENTIAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Implements a 5th-order polynomial for use by the GunnsGasFan model as the fan/system performance
   curves.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsGasFanCurve.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Install) (2012-07))

@{
*/

#include "math/root_finding/BrentMethod.hh"
#include "math/MsMath.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fan Curve Model
///
/// @details  This class serves two purposes: it implements a 5th-order polynomial for impeller-like
///           class to use for its fan-system curves, which also acts as a target function for the
///           Brent's root-finding method, and finds the root (zero) of the curve.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsGasFanCurve : public BrentMethod
{
        TS_MAKE_SIM_COMPATIBLE(GunnsGasFanCurve);

    public:
        /// @brief  Default constructs this Gas Fan Curve.
        GunnsGasFanCurve();
        /// @brief  Default destructs this Gas Fan Curve.
        virtual ~GunnsGasFanCurve();
        /// @brief  Sets the polynomial coefficients for use by the evaluate method.
        void           setCoeffs(double* coeffs);
        /// @brief  Evaluates the polynomial.
        virtual double evaluate(const double x);
        /// @brief  Finds the root of the given fan curve by improving the given root.
        void           improveRoot(double& realRoot, double* realCoeffs, double maxQ);
        /// @brief  Returns the number of iterations in the last Laguerre method call.
        int            getLaguerreIterations() const;
        /// @brief  Returns the number of iterations in the last Brent method call.
        int            getBrentIterations() const;

    protected:
        double* mCoeffs;                    /**< (--) Coefficients of the 5th-order polynomial.   */
        int     mIterLaguerre;              /**< (--) Number of Laguerre method iterations taken. */
        int     mIterBrent;                 /**< (--) Number of Brent method iterations taken.    */

    private:
        /// @brief    Copy constructor unavailable since declared private and not implemented.
        GunnsGasFanCurve(const GunnsGasFanCurve&);
        /// @brief    Assignment operator unavailable since declared private and not implemented.
        GunnsGasFanCurve& operator =(const GunnsGasFanCurve&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] coeffs (--) Pointer to an array of 6 real coefficients for a 5th-order polynomial.
///
/// @details  The array pointer and size are stored for later use by the evaluate method.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsGasFanCurve::setCoeffs(double* coeffs)
{
    mCoeffs = coeffs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) The number of iterations in the last Laguerre method call.
///
/// @details  Returns the number of iterations in the last Laguerre method call.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsGasFanCurve::getLaguerreIterations() const
{
    return mIterLaguerre;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) The number of iterations in the last Brent method call.
///
/// @details  Returns the number of iterations in the last Brent method call.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsGasFanCurve::getBrentIterations() const
{
    return mIterBrent;
}

#endif
