#ifndef LinearFit_EXISTS
#define LinearFit_EXISTS
/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_LINEAR Univariate Linear Curve Fit
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides the class for a Univariate Linear curve fit.)

 LIBRARY DEPENDENCY:
-   ((LinearFit.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Prototype) (2011-03))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Univariate linear curve fit.
///
/// @details  Provides the univariate linear curve fit model.
///           \verbatim
///           a + b * x
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class LinearFit : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(LinearFit);
    public:
        /// @brief Default constructs this linear curve fit model.
        LinearFit();
        /// @brief Constructs this linear curve fit model taking coefficient and range arguments.
        LinearFit(const double a, const double b, const double minX, const double maxX,
                  const std::string &name = "LinearFit");
        /// @brief Default destructs this linear curve fit model.
        virtual ~LinearFit();
        /// @brief    Initialization method
        void init(const double a, const double b, const double minX, const double maxX,
                  const std::string &name = "LinearFit");
        /// @brief Returns the first coefficient of this linear curve fit model.
        double getA() const;
        /// @brief Returns the second coefficient of this linear curve fit model.
        double getB() const;
    protected:
        double mA;     /**< (--) trick_chkpnt_io(**) First coefficient for curve fit model. */
        double mB;     /**< (--) trick_chkpnt_io(**) Second coefficient for curve fit model. */
        /// @brief Returns the model fit for the specified variables.
         virtual double evaluate(const double x, const double y = 0.0);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        LinearFit(const LinearFit&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        LinearFit& operator =(const LinearFit&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   First (bias) coefficient of this Linear curve fit model.
///
/// @details  Returns the first (bias) coefficient of this Linear curve fit model,
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double LinearFit::getA() const
{
    return mA;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Second (scale factor) coefficient of this Linear curve fit model.
///
/// @details  Returns the second (scale factor) coefficient of this Linear curve fit model,
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double LinearFit::getB() const
{
    return mB;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   x  (--)  First independent variable for curve fit.
/// @param[in]   y  (--)  Second independent variable for curve fit (defaults to zero since not used).
///
/// @return   Fitted variable at specified input.
///
/// @details  Returns this Linear model fit for the specified variable.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double LinearFit::evaluate(const double x, const double y __attribute__((unused)))
{
    return mA + mB * x;
}

#endif
