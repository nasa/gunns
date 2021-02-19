#ifndef TsBilinearInterpolatorReverse_EXISTS
#define TsBilinearInterpolatorReverse_EXISTS

/**
@file
@brief     Bivariate Linear Reverse Interpolator declarations

@defgroup  TSM_UTILITIES_MATH_APPROXIMATION_BILINEAR_INTERPOLATOR_REVERSE Bivariate Linear Reverse Interpolator
@ingroup   TSM_UTILITIES_MATH_APPROXIMATION

@details
PURPOSE:
- (Provides a class for a two-dimensional linear reverse interpolation)

 LIBRARY DEPENDENCY:
- ((TsBilinearInterpolatorReverse.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2013-08))

@{
*/

#include "TsBilinearInterpolator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Bilinear reverse interpolation derived from base class TsBilinearInterpolator.
///
/// @details  Whereas the base class does a "forward" look-up of z from the table z = f(x, y), this
///           object takes the same table, and looks up y given x & z.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsBilinearInterpolatorReverse : public TsBilinearInterpolator {
    TS_MAKE_SIM_COMPATIBLE(TsBilinearInterpolatorReverse);
    public:
        /// @brief  Default constructs this bilinear reverse interpolator model.
        TsBilinearInterpolatorReverse();
        /// @brief  Constructs this bilinear reverse interpolator model taking range arguments.
        TsBilinearInterpolatorReverse(const double* x,    const double* y,    const double* z,
                                      const int     m,    const int     n,
                                      const double  minX, const double  maxX,
                                      const double  mixY, const double  maxY,
                                      const std::string &name = "TsBilinearInterpolatorReverse");
        /// @brief  Default destructs this bilinear reverse interpolator model.
        virtual ~TsBilinearInterpolatorReverse();
        /// @brief  Initialization method.
        void init();

    protected:
        /// @brief  Returns the bilinear reverse interpolated value for the specified variables.
        virtual double evaluate(const double x, const double z);
        /// @brief  Returns true if x is between end1 and end2 inclusive, and regardless of value
        ///         of end1 and end2 relative to each other.
        bool isBetween(const double end1, const double x, const double end2);

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsBilinearInterpolatorReverse(const TsBilinearInterpolatorReverse&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        TsBilinearInterpolatorReverse& operator =(const TsBilinearInterpolatorReverse&);
};

/// @}

#endif
