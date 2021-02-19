#ifndef TsApproximation_EXISTS
#define TsApproximation_EXISTS

/**
 @brief     Approximation base class.

 @defgroup  TSM_UTILITIES_MATH_APPROXIMATION_BASE Approximation Base Class
 @ingroup   TSM_UTILITIES_MATH_APPROXIMATION

 @details
 PURPOSE:
 - (Provides the abstract base class for approximation by curve fits and interpolators.)

 LIBRARY DEPENDENCY:
-   ((TsApproximation.o))

 PROGRAMMERS:
 - ((Eric Ferguson) (L-3 Communications) (Initial) (2011-07))

 @{
 */

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "math/MsMath.hh"
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Abstract base class for curve fits and interpolators.
///
/// @details  Provides the abstract base class for univariate and bivariate approximations
///           (curve fits and interpolators) within specified valid ranges.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(TsApproximation);
    public:
        /// @brief    Default constructs this curve fit/interpolator approximation.
        TsApproximation();
        /// @brief    Constructs this curve fit/interpolator approximation taking range arguments.
        TsApproximation(const double minX, const double maxX, const double minY, const double maxY,
                        const std::string &name = "TsApproximation");
        /// @brief    Default destructs this curve fit/interpolator approximation.
        virtual ~TsApproximation();
        /// @brief    Initialization method
        void init(const double minX, const double maxX, const double minY, const double maxY,
                  const std::string &name = "TsApproximation");
        /// @brief    Returns the approximation with bounding on out of bounds input.
        double get(const double x, const double y = 0);
        /// @brief    Returns the approximation with exception on out of bounds input.
        double getExceptional(const double x, const double y = 0);
        /// @brief   Returns initialization flag.
        bool isInitialized() const;
    protected:
        double mMinX;      /**<    (--) trick_chkpnt_io(**) Approximation valid range lower limit for first variable.  */
        double mMaxX;      /**<    (--) trick_chkpnt_io(**) Approximation valid range upper limit for first variable.  */
        double mMinY;      /**<    (--) trick_chkpnt_io(**) Approximation valid range lower limit for second variable. */
        double mMaxY;      /**<    (--) trick_chkpnt_io(**) Approximation valid range upper limit for second variable. */
        bool   mInitFlag;  /**< *o (--) trick_chkpnt_io(**) Init status: T- if initialized and valid                   */
        std::string mName; /**< *o (--) trick_chkpnt_io(**) Object name for error messages                             */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Returns the approximation for the specified variables.
        ///
        /// @param[in]    x  (--)  First independent variable for curve fit.
        /// @param[in]    y  (--)  Second independent variable for curve fit 9optional, defaults to 0).
        ///
        /// @return   Approximated variable at specified input.
        ///
        /// @note     A derived class must implement this method with its specific curve fit or interpolation.
        ///
        /// @details  Returns this approximation for the specified variables.
        ////////////////////////////////////////////////////////////////////////////////////////////
        virtual double evaluate(const double x, const double y = 0.0) = 0;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsApproximation(const TsApproximation&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsApproximation& operator =(const TsApproximation&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for approximation.
/// @param[in]    y  (--)  Second independent variable for approximation.
///
/// @return   Approximated dependent variable at specified input.
///
/// @details  Returns an approximation for the specified variables with bounding to the valid range.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsApproximation::get(const double x, const double y) {
    const double z = MsMath::limitRange(mMinX, x, mMaxX);
    const double w = MsMath::limitRange(mMinY, y, mMaxY);
    return evaluate(z, w);
}

#endif
