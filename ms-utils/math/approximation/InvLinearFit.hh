#ifndef InvLinearFit_EXISTS
#define InvLinearFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_INVERSE_LINEAR Univariate Inverse Linear Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Inverse Linear curve fit.)

 LIBRARY DEPENDENCY:
-   ((InvLinearFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate inverse linear curve fit.
///
/// @details  Provides the univariate inverse linear curve fit model.
///           \verbatim
///                      c
///           a x + b + ---
///                      x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class InvLinearFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(InvLinearFit);
    public:
        /// @brief Default constructs this inverse linear curve fit model.
        InvLinearFit();
        /// @brief Constructs this inverse linear curve fit model taking coefficient and range
        ///        arguments.
        InvLinearFit(const double a,    const double b, const double c,
                     const double minX, const double maxX, const std::string &name = "InvLinearFit");
        /// @brief Default destructs this inverse linear curve fit model.
        virtual ~InvLinearFit();
        /// @brief    Initialization method
        void init(const double a, const double b, const double c,
                  const double minX, const double maxX, const std::string &name  = "InvLinearFit");
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
        InvLinearFit(const InvLinearFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        InvLinearFit& operator =(const InvLinearFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Inverse Linear model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double InvLinearFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA * x + mB + mC / x;
}

#endif
