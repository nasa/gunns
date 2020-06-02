#ifndef QuadLinInvFit_EXISTS
#define QuadLinInvFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_QUAD_LIN_INV Bivariate Quadratic Linear Inverse Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Bivariate Quadratic, Linear Inverse curve fit.)

 LIBRARY DEPENDENCY:
-   ((QuadLinInvFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Bivariate Quadratic, Linear Inverse curve fit.
///
/// @details  Provides the Bivariate Quadratic, Linear Inverse curve fit model.
///           \verbatim
///                                     2
///               y - (a + c * x + e * x )
///              --------------------------
///                                   2
///                  b + d * x + f * x
///           which is the inverse of the Bivariate Quadratic, Linear fit model
///                                                               2
///              y = (a + b * z) + (c + d * z) * x  + (e + f * z) * x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class QuadLinInvFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(QuadLinInvFit);
    public:
        /// @brief Default constructs this Bivariate Quadratic, Linear Inverse curve fit model.
        QuadLinInvFit();
        /// @brief Constructs this Bivariate Quadratic, Linear Inverse curve fit model taking
        ///        coefficient and range arguments.
    QuadLinInvFit(const double a,    const double b,    const double c,
                  const double d,    const double e,    const double f,
                  const double minX, const double maxX, const double minY, const double maxY,
                  const std::string &name = "QuadLinInvFit");
        /// @brief Default destructs this Bivariate Quadratic, Linear Inverse curve fit model.
        virtual ~QuadLinInvFit();
        /// @brief    Initialization method
        void init(const double a,    const double b,    const double c,
                  const double d,    const double e,    const double f,
                  const double minX, const double maxX, const double minY, const double maxY,
                  const std::string &name = "QuadLinInvFit");
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
        QuadLinInvFit(const QuadLinInvFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        QuadLinInvFit& operator =(const QuadLinInvFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for curve fit.
/// @param[in]    y  (--)  Second independent variable for curve fit.
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Bivariate Quadratic, Linear Inverse model curve fit for the specified
///           variables.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double QuadLinInvFit::evaluate(const double x, const double y)
{
    return (y - mA - (mC + mE * x) * x) / (mB + (mD + mF * x) * x);
}

#endif

