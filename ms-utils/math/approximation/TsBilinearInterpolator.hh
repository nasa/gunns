#ifndef TsBilinearInterpolator_EXISTS
#define TsBilinearInterpolator_EXISTS

/**
@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_BILINEAR_INTERPOLATOR Bivariate Linear Interpolator
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides a class for a two-dimensional linear interpolation. )

 LIBRARY DEPENDENCY:
-   ((TsBilinearInterpolator.o))

 PROGRAMMERS:
- ((Eric Ferguson) (L-3 Communications) (Initial) (2011-08))

@{
*/

#include "TsApproximation.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Bilinear interpolation derived from abstract base class TsApproximation.
///
/// @details  Provides a two-dimensional linear interpolation when requesting values from a table
///           \verbatim
///              Z  * (y    - y) / (y   - y )  + Z  * (y - y ) / (y    - y)
///               1     j+1          j+1   j      2         j      j+1    j
///           where
///              Z  = z      * (x   - x) / (x   - x )  + z       * (x - x ) / (x   - x )
///               1    i,j       i+1         i+1   i       i+1,j         i      i+1   i
///
///              Z  = z      * (x   - x) / (x   - x ) + z        * (x - x ) / (x   - x )
///               2    i,j+1     i+1         i+1   i     i+1,j+1         i      i+1   i
///           \endverbatim
///
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsBilinearInterpolator : public TsApproximation {
    TS_MAKE_SIM_COMPATIBLE(TsBilinearInterpolator);
    public:
        /// @brief    Default constructs this bilinear interpolator model.
        TsBilinearInterpolator();
        /// @brief    Constructs this bilinear interpolator model taking range arguments.
        TsBilinearInterpolator(const double* x,    const double* y,    const double* z,
                               const int     m,    const int     n,
                               const double  minX, const double  maxX,
                               const double  mixY, const double  maxY,
                               const std::string &name = "TsBilinearInterpolator");
        /// @brief    Default destructs this bilinear interpolator model.
        virtual ~TsBilinearInterpolator();
        /// @brief   Initialization method
        void init(const double* x,    const double* y,    const double* z,
                  const int     m,    const int     n,
                  const double  minX, const double  maxX,
                  const double  mixY, const double  maxY,
                  const std::string &name = "TsBilinearInterpolator");
    protected:
        double*  mX; /**< ** (--) trick_chkpnt_io(**) Array of values for the first independent variable. */
        double*  mY; /**< ** (--) trick_chkpnt_io(**) Array of values for the second independent variable. */
        double** mZ; /**< ** (--) trick_chkpnt_io(**) Array of values for the dependent array. */
        int      mM; /**<    (--) trick_chkpnt_io(**) Length of the first independent variable array. */
        int      mN; /**<    (--) trick_chkpnt_io(**) Length of the second independent variable array. */
        int      mI; /**<    (--)                     Previous interpolation index (mX[mI] <= x < mX[mI+1]). */
        int      mJ; /**<    (--)                     Previous interpolation index (mY[mJ] <= y < mY[mJ+1]). */
        /// @brief    Returns the bilinear interpolated value for the specified variables.
        virtual double evaluate(const double x, const double y) ;
        /// @brief    Deletes dynamic memory
        void  cleanup();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsBilinearInterpolator(const TsBilinearInterpolator&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsBilinearInterpolator& operator =(const TsBilinearInterpolator&);
};

/// @}

#endif



