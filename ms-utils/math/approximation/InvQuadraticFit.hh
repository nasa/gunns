#ifndef InvQuadraticFit_EXISTS
#define InvQuadraticFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_INVERSE_QUADRATIC Univariate Inverse Quadratic Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Inverse Quadratic curve fit.)

 LIBRARY DEPENDENCY:
-   ((InvQuadraticFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate inverse quadratic curve fit.
///
/// @details  Provides the univariate inverse quadratic curve fit model.
///           \verbatim
///                b     c
///           a + --- + ---
///                       2
///                x     x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class InvQuadraticFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(InvQuadraticFit);
    public:
        /// @brief Default constructs this inverse quadratic  curve fit model.
        InvQuadraticFit();
        /// @brief Constructs this inverse quadratic curve fit model taking coefficient and range
        /// arguments.
        InvQuadraticFit(const double a,    const double b, const double c,
                        const double minX, const double maxX, const std::string &name  = "InvQuadraticFit");
        /// @brief Default destructs this inverse quadratic curve fit model.
        virtual ~InvQuadraticFit();
        /// @brief    Initialization method
        void init(const double a, const double b, const double c,
                  const double minX, const double maxX, const std::string &name  = "InvQuadraticFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mC;     /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        InvQuadraticFit(const InvQuadraticFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        InvQuadraticFit& operator =(const InvQuadraticFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Inverse Quadratic model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double InvQuadraticFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA + (mB + mC / x) / x;
}

#endif

