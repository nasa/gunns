#ifndef SutherlandFit_EXISTS
#define SutherlandFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_SUTHERLAND Univariate Sutherland Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Sutherland curve fit.)

 LIBRARY DEPENDENCY:
-   ((SutherlandFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate Sutherland curve fit.
///
/// @details  Provides the univariate Sutherland curve fit model.
///           \verbatim
///                  3/2
///                 x
///           a * -------
///                b + x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class SutherlandFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(SutherlandFit);
    public:
        /// @brief Default constructs this Sutherland curve fit model.
        SutherlandFit();
        /// @brief Constructs this Sutherland curve fit model taking coefficient and range
        ///        arguments.
        SutherlandFit(const double a, const double b, const double minX, const double maxX,
                      const std::string &name = "SutherlandFit");
        /// @brief Default destructs this Sutherland curve fit model.
        virtual ~SutherlandFit();
        /// @brief    Initialization method
        void init(const double a, const double b, const double minX, const double maxX,
                  const std::string &name = "SutherlandFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y = 0.0) ;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        SutherlandFit(const SutherlandFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        SutherlandFit& operator =(const SutherlandFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Sutherland model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SutherlandFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA * sqrt(x * x * x) / (mB + x);
}

#endif
