#ifndef FluidHvapFit_EXISTS
#define FluidHvapFit_EXISTS

/**
@file
@brief    Fluid Heat of Vaporization Curve Fit declarations

@defgroup  TSM_UTILITIES_PROPERTIES_FLUID_TSAT_FIT Fluid Heat of Vaporization Curve Fit
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the class for a Fluid Heat of Vaporization curve fit.)

 LIBRARY DEPENDENCY:
- ((FluidHvapFit.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2015-01))

@{
*/

#include "math/approximation/TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Heat of Vaporization curve fit.
///
/// @details  Provides the Fluid Heat of Vaporization curve fit model.
///           \verbatim
///                    (-alpha * x)         beta
///           L = a * e            * (1 - x)
///           x = Tr, reduced temperature (T/Tc)
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class FluidHvapFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(FluidHvapFit);
    public:
        /// @brief Default constructs this Fluid Heat of Vaporization curve fit model.
        FluidHvapFit();
        /// @brief Constructs this Fluid Heat of Vaporization curve fit model taking coefficient and
        ///        range arguments.
        FluidHvapFit(const double a,    const double alpha, const double beta,
                     const double minX, const double maxX,
                     const std::string &name = "FluidHvapFit");
        /// @brief Default destructs this Fluid Heat of Vaporization curve fit model.
        virtual ~FluidHvapFit();
        /// @brief    Initialization method
        void init(const double a,    const double alpha, const double beta,
                  const double minX, const double maxX,
                  const std::string &name = "FluidHvapFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mAlpha; /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mBeta;  /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0);
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        FluidHvapFit(const FluidHvapFit&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        FluidHvapFit& operator =(const FluidHvapFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  Reduced temperature (T/Tc) to evaluate the curve fit at.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Latent heat of vaporization (kJ/kg) of the fluid at the specified reduced temperature.
///
/// @details  Returns the Latent heat of vaporization for the specified reduced temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidHvapFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA * exp(-mAlpha * x) * pow((1.0 - x), mBeta);
}

#endif
