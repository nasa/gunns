#ifndef QuadraticRootFit_EXISTS
#define QuadraticRootFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_QUADRATIC_ROOT Univariate Quadratic Root Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Quadratic Root curve fit.)

 LIBRARY DEPENDENCY:
-   ((QuadraticRootFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate Quadratic Root curve fit.
///
/// @details  Provides the Univariate Quadratic Root curve fit model.
///           \verbatim
///                     __________
///               p * \/q + r * x
///              ------------------
///                      s
///           \endverbatim
///           This is the most positive root of the quadratic equation
///           \verbatim
///                   2
///              b * y  + a * y = x
///           where
///              p = -a
///                   2
///              q = a
///              r = 4 * b
///                     1
///              s = -------
///                   2 * b
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class QuadraticRootFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(QuadraticRootFit);
    public:
        /// @brief Default constructs this Quadratic Root curve fit model.
        QuadraticRootFit();
        /// @brief Constructs this Quadratic Root curve fit model taking coefficient and range
        ///        arguments.
        QuadraticRootFit(const double a, const double b, const double minX, const double maxX,
                         const std::string &name = "QuadraticRootFit");
        /// @brief Default destructs this Univariate Quadratic Root curve fit model.
        virtual ~QuadraticRootFit();
        /// @brief    Initialization method
        void init(const double a,    const double b,
                  const double minX, const double maxX,
                  const std::string &name = "QuadraticRootFit");
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
        QuadraticRootFit(const QuadraticRootFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuadraticRootFit& operator =(const QuadraticRootFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Univariate Quadratic Root model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double QuadraticRootFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return (mA + sqrt(mB + mC * x)) * mD;
}

#endif

