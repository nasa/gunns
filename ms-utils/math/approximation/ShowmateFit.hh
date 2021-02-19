#ifndef ShowmateFit_EXISTS
#define ShowmateFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_SHOWMATE Univariate Showmate Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Showmate curve fit.)

 LIBRARY DEPENDENCY:
-   ((ShowmateFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate Showmate curve fit.
///
/// @details  Provides the univariate Showmate curve fit model.
///           \verbatim
///                            2        3        2
///           a + b * x + c * x  + d * x  + e / x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class ShowmateFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(ShowmateFit);
    public:
        /// @brief Default constructs this Showmate curve fit model.
        ShowmateFit();
        /// @brief Constructs this Showmate curve fit model taking coefficient and range arguments.
        ShowmateFit(const double a, const double b, const double c, const double d, const double e,
                    const double minX, const double maxX, const std::string &name = "ShowmateFit");
        /// @brief Default destructs this Showmate curve fit model.
        virtual ~ShowmateFit();
        /// @brief    Initialization method
        void init(const double a, const double b, const double c, const double d, const double e,
                    const double minX, const double maxX, const std::string &name = "ShowmateFit");
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
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ShowmateFit(const ShowmateFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ShowmateFit& operator =(const ShowmateFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Showmate model fit for the specified variable.
///
/// @note     The input argument is scaled by 0.001.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double ShowmateFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA + (mB + (mC + mD * 0.001 * x) * 0.001 * x) * 0.001 * x + 1000000.0 * mE / (x * x);
}

#endif
