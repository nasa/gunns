#ifndef FluidTsatFit_EXISTS
#define FluidTsatFit_EXISTS

/**
@file
@brief    Fluid Saturation Temperature Curve Fit declarations

@defgroup  TSM_UTILITIES_PROPERTIES_FLUID_TSAT_FIT Fluid Saturation Temperature Curve Fit
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the class for a Fluid Saturation Temperature curve fit.)

 LIBRARY DEPENDENCY:
- ((FluidTsatFit.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2015-01))

@{
*/

#include "math/approximation/TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Saturation Temperature curve fit.
///
/// @details  Solves a quadratic root of the Saturation Pressure curve fit model (a quadratic
///           polynomial).  This reuses the same polynomial coefficients as the Saturation Pressure
///           curve.  Saturation Pressure Ps = f(Ts) and Saturation Temperature is the exactly
///           reversible function Ts = f(Ps).
///           \verbatim
///                             ________________________
///              (C2) * ( B - \/B2 - C4 * (A - log10(x)) )
///
///           \endverbatim
///           This is the "negative" root of the quadratic equation
///           \verbatim
///              c * y^2 + b * y + a = log10(x)
///           where
///              B  = -b
///              B2 = b^2
///              C4 = 4 * c
///              C2 = 1 / (2 * c)
///              A  = a
///
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class FluidTsatFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(FluidTsatFit);
    public:
        /// @brief Default constructs this Fluid Saturation Temperature curve fit model.
        FluidTsatFit();
        /// @brief Constructs this Fluid Saturation Temperature curve fit model taking coefficient
        ///        and range arguments.
        FluidTsatFit(const double a, const double b, const double c, const double minX,
                     const double maxX, const std::string &name = "FluidTsatFit");
        /// @brief Default destructs this Fluid Saturation Temperature curve fit model.
        virtual ~FluidTsatFit();
        /// @brief    Initialization method
        void init(const double a,    const double b,    const double c,
                  const double minX, const double maxX,
                  const std::string &name = "FluidTsatFit");
    protected:
        double mC2;    /**< (--) trick_chkpnt_io(**) Quadratic equation term (0.5/c). */
        double mC4;    /**< (--) trick_chkpnt_io(**) Quadratic equation term (4*c). */
        double mB;     /**< (--) trick_chkpnt_io(**) Quadratic equation term (-b).  */
        double mB2;    /**< (--) trick_chkpnt_io(**) Quadratic equation term (b^2). */
        double mA;     /**< (--) trick_chkpnt_io(**) Quadratic equation term (a).   */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0) ;
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        FluidTsatFit(const FluidTsatFit&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        FluidTsatFit& operator =(const FluidTsatFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (kPa) Saturation pressure.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Tc/Ts, the inverse of the reduced saturated temperature.
///
/// @details  Returns the inverse of the reduced saturated temperature, Tc/Ts.  Ts can then be found
///           by dividing critical temperature Tc by this result.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidTsatFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return (mB - sqrt(mB2 - mC4 * (mA - log10(x)))) * mC2;
}

#endif

