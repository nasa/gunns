#ifndef ExponentialFit_EXISTS
#define ExponentialFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_EXPONENTIAL Univariate Exponential Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Exponential curve fit.)

 LIBRARY DEPENDENCY:
-   ((ExponentialFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate exponential curve fit.
///
/// @details  Provides the univariate exponential curve fit model.
///           \verbatim
///             _              _
///            |      b     c   |
///            | a + --- + ---  |
///            |             2  |
///            |_     x     x  _|
///           e
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class ExponentialFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(ExponentialFit);
    public:
        /// @brief Default constructs this exponential curve fit model.
        ExponentialFit();
        /// @brief Constructs this exponential curve fit model taking coefficient and range
        ///        arguments.
        ExponentialFit(const double a, const double b, const double c,
                       const double minX, const double maxX, const std::string &name = "ExponentialFit");
        /// @brief Default destructs this exponential curve fit model.
        virtual ~ExponentialFit();
        /// @brief    Initialization method
        void init(const double a, const double b, const double c,
                  const double minX, const double maxX, const std::string &name  = "ExponentialFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mC;     /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ExponentialFit(const ExponentialFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ExponentialFit& operator =(const ExponentialFit&);
};

/// @}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Exponential model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double ExponentialFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return exp(mA + (mB + mC / x) / x);
}

#endif
