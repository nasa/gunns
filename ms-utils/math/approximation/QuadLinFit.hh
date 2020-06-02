#ifndef QuadLinFit_EXISTS
#define QuadLinFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_QUAD_LIN Bivariate Quadratic Linear Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
 PURPOSE:
- (Provides the class for a Bivariate Quadratic, Linear curve fit.)

 LIBRARY DEPENDENCY:
-   ((QuadLinFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Bivariate Quadratic, Linear curve fit.
///
/// @details  Provides the Bivariate Quadratic, Linear curve fit model.
///           \verbatim
///                                                           2
///           (a + b * y) + (c + d * y) * x  + (e + f * y) * x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class QuadLinFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(QuadLinFit);
    public:
        /// @brief Default constructs this Bivariate Quadratic, Linear curve fit model.
        QuadLinFit();
        /// @brief Constructs this Bivariate Quadratic, Linear curve fit model taking coefficient
        /// and range arguments.
        QuadLinFit(const double a,    const double b,    const double c,
                   const double d,    const double e,    const double f,
                   const double minX, const double maxX, const double minY, const double maxY,
                   const std::string &name = "QuadLinFit");
        /// @brief Default destructs this Bivariate Quadratic, Linear curve fit model.
        virtual ~QuadLinFit();
        /// @brief    Initialization method
        void init(const double a,    const double b,    const double c,
                  const double d,    const double e,    const double f,
                  const double minX, const double maxX, const double minY, const double maxY,
                  const std::string &name = "QuadLinFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        double mC;     /**< (--) trick_chkpnt_io(**) Third coefficient for curve fit model. */
        double mD;     /**< (--) trick_chkpnt_io(**) Fourth coefficient for curve fit model. */
        double mE;     /**< (--) trick_chkpnt_io(**) Fifth coefficient for curve fit model. */
        double mF;     /**< (--) trick_chkpnt_io(**) Sixth coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y) ;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuadLinFit(const QuadLinFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuadLinFit& operator =(const QuadLinFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for curve fit.
/// @param[in]    y  (--)  Second independent variable for curve fit.
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Bivariate Quadratic, Linear model curve fit for the specified variables.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double QuadLinFit::evaluate(const double x, const double y)
{
    return (mA + mB * y) + ((mC + mD * y) + (mE + mF * y) * x) * x;
}

#endif
