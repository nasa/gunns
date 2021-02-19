#ifndef QuotientFit_EXISTS
#define QuotientFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_QUOTIENT Univariate Quotient Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Bivariate Quotient curve fit.)

 LIBRARY DEPENDENCY:
-   ((QuotientFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Bivariate Quotient curve fit.
///
/// @details  Provides the Bivariate Quotient curve fit model.
///           \verbatim
///                y
///           a * ---
///                x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class QuotientFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(QuotientFit);
    public:
        /// @brief Default constructs this Quotient curve fit model.
        QuotientFit();
        /// @brief Constructs this Quotient curve fit model taking coefficient and range arguments.
        QuotientFit(const double a,    const double minX, const double maxX,
                    const double minY, const double maxY, const std::string &name = "QuotientFit");
        /// @brief Default destructs this Quotient curve fit model.
        virtual ~QuotientFit();
        /// @brief    Initialization method
        void init(const double a,    const double minX, const double maxX,
                  const double minY, const double maxY, const std::string &name = "QuotientFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y) ;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuotientFit(const QuotientFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuotientFit& operator =(const QuotientFit&);
};

/// @}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for curve fit.
/// @param[in]    y  (--)  Second independent variable for curve fit.
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Bivariate Quotient model curve fit for the specified variables.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double QuotientFit::evaluate(const double x, const double y)
{
    return mA * y / x;
}

#endif

