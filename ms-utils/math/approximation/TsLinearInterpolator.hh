#ifndef TsLinearInterpolator_EXISTS
#define TsLinearInterpolator_EXISTS

/**
 @defgroup  TSM_UTILITIES_MATH_APPROXIMATION_LINEAR_INTERPOLATOR Univariate Linear Interpolator
 @ingroup   TSM_UTILITIES_MATH_APPROXIMATION

 @details
 PURPOSE:
 - (Provides a class for a one-dimensional linear interpolation.)

 LIBRARY DEPENDENCY:
-   ((TsLinearInterpolator.o))

 PROGRAMMERS:
 - ((Eric Ferguson) (L-3 Communications) (Initial) (2011-08))

 @{
 */

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Linear interpolator derived from abstract base class TsApproximation.
///
/// @details  Provides a one-dimensional linear interpolator when requesting values from a table.
///           \verbatim
///           z + ((z    -z ) * (x - x )/(x    -x )
///            i     i+1   i          i    i+1   i
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsLinearInterpolator: public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(TsLinearInterpolator);
    public:
        /// @brief    Default constructs this interpolator model.
        TsLinearInterpolator();
        /// @brief    Constructs this interpolator model taking range arguments.
        TsLinearInterpolator(const double* x,    const double* z,     const int n,
                             const double  minX, const double  maxX,
                             const std::string &name = "TsLinearInterpolator");
        /// @brief    Default destructs this interpolator model.
        virtual ~TsLinearInterpolator();
        /// @brief   Initialization method
        void init(const double* x,    const double* z,  const int n,
                  const double  minX, const double  maxX, const std::string &name = "TsLinearInterpolator");
    protected:
        double* mX; /**< ** (--) trick_chkpnt_io(**) Array of values for the independent variable. */
        double* mZ; /**< ** (--) trick_chkpnt_io(**) Array of values for the dependent variable. */
        int     mM; /**<    (--) trick_chkpnt_io(**) Length of the independent and dependent variable arrays. */
        int     mI; /**<    (--)                     Previous interpolation index (mX[mI] <= x < mX[mI+1]). */
        /// @brief    Returns the linear interpolated value for the specified variables.
        virtual double evaluate(const double x, const double = 0.0);
        /// @brief    returns index to use in interpolation
        static int selectCell(const double x, const double mX[], const int size, int cIndex = 0);
        /// @brief    validates the input array x is sequentially ordered (increasing or decreasing)
        void  validateOrdered(const int n, const double x[]);
        /// @brief    Deletes dynamic memory
        void  cleanup();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsLinearInterpolator(const TsLinearInterpolator&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsLinearInterpolator& operator =(const TsLinearInterpolator&);
};

/// @}

#endif

