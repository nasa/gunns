#ifndef QuadraticFit_EXISTS
#define QuadraticFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_QUADRATIC Univariate Quadratic Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Quadratic curve fit.)

 LIBRARY DEPENDENCY:
-   ((QuadraticFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate quadratic curve fit.
///
/// @details  Provides the Univariate Quadratic curve fit model.
///           \verbatim
///                            2
///           a + b * x + c * x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class QuadraticFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(QuadraticFit);
    public:
        /// @brief Default constructs this quadratic curve fit model.
        QuadraticFit();
        /// @brief Constructs this quadratic curve fit model taking coefficient and range arguments.
        QuadraticFit(const double a,    const double b,    const double c,
                     const double minX, const double maxX,
                     const std::string &name = "QuadraticFit");
        /// @brief Default destructs this quadratic curve fit model.
        virtual ~QuadraticFit();
        /// @brief    Initialization method
        void init(const double a,    const double b,    const double c,
                  const double minX, const double maxX,
                  const std::string &name = "QuadraticFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mC;     /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0) ;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuadraticFit(const QuadraticFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuadraticFit& operator =(const QuadraticFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Quadratic model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double QuadraticFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA + (mB + mC * x) * x;
}

#endif
