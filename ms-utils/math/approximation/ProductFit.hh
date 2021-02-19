#ifndef ProductFit_EXISTS
#define ProductFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_PRODUCT Univariate Product Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Bivariate Product curve fit.)

 LIBRARY DEPENDENCY:
-   ((ProductFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Bivariate Product curve fit.
///
/// @details  Provides the Bivariate Product curve fit model.
///           \verbatim
///           a * x * y
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class ProductFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(ProductFit);
    public:
        /// @brief Default constructs this Bivariate Product curve fit model.
        ProductFit();
        /// @brief Constructs this Bivariate Product curve fit model taking coefficient and range
        ///        arguments.
        ProductFit(const double a,    const double minX, const double maxX,
                   const double minY, const double maxY,
                   const std::string &name = "ProductFit");
        /// @brief Default destructs this Bivariate Product curve fit model.
        virtual ~ProductFit();
        /// @brief    Initialization method
        void init(const double a,    const double minX, const double maxX,
                  const double minY, const double maxY,
                  const std::string &name = "ProductFit");
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
        virtual double evaluate(const double x, const double y);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ProductFit(const ProductFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ProductFit& operator =(const ProductFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for curve fit.
/// @param[in]    y  (--)  Second independent variable for curve fit.
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Bivariate Product model curve fit for the specified variables.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double ProductFit::evaluate(const double x, const double y)
{
    return mA * x * y;
}

#endif
