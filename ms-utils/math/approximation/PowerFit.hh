#ifndef PowerFit_EXISTS
#define PowerFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_POWER Univariate Power Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Power curve fit.)

 LIBRARY DEPENDENCY:
-   ((PowerFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate power curve fit.
///
/// @details  Provides the univariate power curve fit model.
///           \verbatim
///                x
///           a * b
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class PowerFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(PowerFit);
    public:
        /// @brief Default constructs this power curve fit model.
        PowerFit();
        /// @brief Constructs this power curve fit model taking coefficient and range arguments.
        PowerFit(const double a,    const double b,  const double minX, const double maxX,
                 const std::string &name = "PowerFit");
        /// @brief Default destructs this power curve fit model.
        virtual ~PowerFit();
        /// @brief    Initialization method
        void init(const double a, const double b, const double minX, const double maxX,
                  const std::string &name = "PowerFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        PowerFit(const PowerFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        PowerFit& operator =(const PowerFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Power model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PowerFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA * pow(mB, x);
}

#endif
