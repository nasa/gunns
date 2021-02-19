#ifndef QuarticFit_EXISTS
#define QuarticFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_QUARTIC Univariate Quartic Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Quartic curve fit.)

 LIBRARY DEPENDENCY:
-   ((QuarticFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate quartic curve fit.
///
/// @details  Provides the univariate quartic curve fit model.
///           \verbatim
///                            2        3        4
///           a + b * x + c * x  + d * x  + e * x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class QuarticFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(QuarticFit);
    public:
        /// @brief Default constructs this quartic curve fit model.
        QuarticFit();
        /// @brief Constructs this quartic curve fit model taking coefficient and range arguments.
        QuarticFit(const double a, const double b,    const double c,    const double d,
                   const double e, const double minX, const double maxX, const std::string &name = "QuarticFit");
        /// @brief Default destructs this quartic curve fit model.
        virtual ~QuarticFit();
        /// @brief    Initialization method
        void init(const double a, const double b,    const double c,    const double d,
                  const double e, const double minX, const double maxX, const std::string &name = "QuarticFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mC;     /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        double mD;     /**< (--) trick_chkpnt_io(**) Fourth coefficient for curve fit model. */
        double mE;     /**< (--) trick_chkpnt_io(**) Fifth coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0) ;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuarticFit(const QuarticFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuarticFit& operator =(const QuarticFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Quartic model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double QuarticFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA + (mB + (mC + (mD + mE * x) * x) * x) * x;
}

#endif

