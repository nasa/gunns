#ifndef RationalFit_EXISTS
#define RationalFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_RATIONAL Univariate Rational Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Rational curve fit.)

 LIBRARY DEPENDENCY:
-   ((RationalFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate rational (linear/quadratic) curve fit.
///
/// @details  Provides the univariate rational (linear/quadratic) curve fit model.
///           \verbatim
///                a + b * x
///           --------------------
///                             2
///            1 + c * x + d * x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class RationalFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(RationalFit);
    public:
        /// @brief Default constructs this rational curve fit model.
        RationalFit();
        /// @brief Constructs this rational curve fit model taking coefficient and range arguments.
        RationalFit(const double a,    const double b,    const double c, const double d,
                    const double minX, const double maxX, const std::string &name = "RationalFit");
        /// @brief Default destructs this rational curve fit model.
        virtual ~RationalFit();
        /// @brief    Initialization method
        void init(const double a,    const double b,    const double c, const double d,
                  const double minX, const double maxX, const std::string &name = "RationalFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mC;     /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        double mD;     /**< (--) trick_chkpnt_io(**) Fourth coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0) ;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        RationalFit(const RationalFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        RationalFit& operator =(const RationalFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Rational model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double RationalFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return (mA + mB * x) / (1.0 + (mC + mD * x) * x);
}

#endif
