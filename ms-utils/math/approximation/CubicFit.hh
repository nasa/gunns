#ifndef CubicFit_EXISTS
#define CubicFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_CUBIC Univariate Cubic Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Cubic curve fit.)

 LIBRARY DEPENDENCY:
-   ((CubicFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate cubic curve fit.
///
/// @details  Provides the univariate cubic curve fit model.
///           \verbatim
///                            2        3
///           a + b * x + c * x  + d * x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class CubicFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(CubicFit);
    public:
        /// @brief Default constructs this cubic curve fit model.
        CubicFit();
        /// @brief Constructs this cubic curve fit model taking coefficient and range arguments.
        CubicFit(const double a, const double b,    const double c,
                 const double d, const double minX, const double maxX,
                 const std::string &name = "CubicFit");
        /// @brief Default destructs this cubic curve fit model.
        virtual ~CubicFit();
        /// @brief    Initialization method
        void init(const double a,    const double b,    const double c,    const double d,
                const double minX, const double maxX, const std::string &name = "CubicFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mC;     /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        double mD;     /**< (--) trick_chkpnt_io(**) Fourth coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CubicFit(const CubicFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CubicFit& operator =(const CubicFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Cubic model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double CubicFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA + (mB + (mC + mD * x) * x) * x;
}

#endif
