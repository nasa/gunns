#ifndef QuinticFit_EXISTS
#define QuinticFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_QUINTIC Univariate Quintic Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Quintic curve fit.)

 LIBRARY DEPENDENCY:
-   ((QuinticFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-07))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate quintic curve fit.
///
/// @details  Provides the univariate quintic curve fit model.
///           \verbatim
///                            2        3        4        5
///           a + b * x + c * x  + d * x  + e * x  + f * x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class QuinticFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(QuinticFit);
    public:
        /// @brief Default constructs this quintic curve fit model.
        QuinticFit();
        /// @brief Constructs this quintic curve fit model taking coefficient and range arguments.
        QuinticFit(const double a, const double b, const double c,    const double d,
                   const double e, const double f, const double minX, const double maxX,
                   const std::string &name = "QuinticFit");
        /// @brief Default destructs this quintic curve fit model.
        virtual ~QuinticFit();
        /// @brief    Initialization method
        void init(const double a, const double b,    const double c,    const double d,
                  const double e, const double f, const double minX, const double maxX,
                  const std::string &name = "QuinticFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mC;     /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        double mD;     /**< (--) trick_chkpnt_io(**) Fourth coefficient for curve fit model. */
        double mE;     /**< (--) trick_chkpnt_io(**) Fifth coefficient for curve fit model. */
        double mF;     /**< (--) trick_chkpnt_io(**) Sixth coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0) ;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuinticFit(const QuinticFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuinticFit& operator =(const QuinticFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Quintic model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double QuinticFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA + (mB + (mC + (mD + (mE + mF * x) * x) * x) * x) * x;
}

#endif

