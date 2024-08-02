/**
@file
@brief     Bivariate Linear Reverse Interpolator implementation

LIBRARY DEPENDENCY:
  ((TsBilinearInterpolator.o))
*/

#include "TsBilinearInterpolatorReverse.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this interpolator model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBilinearInterpolatorReverse::TsBilinearInterpolatorReverse()
    :
    TsBilinearInterpolator()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x     (--) Independent variable array x.
/// @param[in]    y     (--) Independent variable array y.
/// @param[in]    z     (--) Dependent multidimensional variable array.
/// @param[in]    m     (--) Size of x variable array.
/// @param[in]    n     (--) Size of y variable array.
/// @param[in]    minX  (--) Interpolator model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Interpolator model valid range upper limit for first variable.
/// @param[in]    minY  (--) Interpolator model valid range lower limit for second variable.
/// @param[in]    maxY  (--) Interpolator model valid range upper limit for second variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Constructs this Bilinear reverse interpolator model taking coefficient and range
///           arguments.  Constructs and initializes the object.
///
/// @throws   TsInitializationException
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBilinearInterpolatorReverse::TsBilinearInterpolatorReverse(
                                               const double* x,    const double* y, const double* z,
                                               const int     m,    const int     n,
                                               const double  minX, const double  maxX,
                                               const double  minY, const double  maxY,
                                               const std::string &name)
    :
    TsBilinearInterpolator(x, y, z, m, n, minX, maxX, minY, maxY, name)
{
    init();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Bilinear interpolation.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBilinearInterpolatorReverse::~TsBilinearInterpolatorReverse()
{
    // Nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes this Bilinear reverse interpolator model.  Scans the table for minimum &
///           maximum values of z, which will be used during the evaluate method to limit the input
///           z argument.
///
/// @note     The min & max values of z in the table are stored in the base class's mMinY and mMaxY
///           terms, since the TsApproximation::get function limits the 2nd argument (normally y but
///           in our case z) to be within mMinY & mMaxY.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsBilinearInterpolatorReverse::init()
{
    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Loop over all table points and store the minimum & maximum z values for bounds limiting
    ///   during evaluate.
    mMinY =  1.0E16;
    mMaxY = -1.0E16;
    for (int i=0; i<mM; ++i) {
        for (int j=0; j<mN; ++j) {
            if (mZ[i][j] < mMinY) {
                mMinY = mZ[i][j];
            } else if (mZ[i][j] > mMaxY) {
                mMaxY = mZ[i][j];
            }
        }
    }

    /// - Set the initialization complete flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for bilinear interpolation.
/// @param[in]    z  (--)  Second independent variable for bilinear interpolation.
///
/// @return   Bilinear interpolated dependent variable y value at specified input x & z.
///
/// @details  Using the same type of table for z = f(x, y) as in the TsBilinearInterpolator class,
///           this returns the value y given x and z.  It searches the y axis starting from the
///           previous y result.  If there are multiple solutions for y, this returns the first one
///           it finds.  If there are no solutions for y, this returns the y that would result in
///           z = f(x,y) being closest to the given z.
///
/// @note     If there are multiple solutions for y at the given x & z, this is not guaranteed to
///           return the one you want.  This class is best used for tables that have unique
///           solutions for y at every (x,z).
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsBilinearInterpolatorReverse::evaluate(const double x, const double z)
{
    /// - Find index i such that mX[i] <= x < mX[i+1] starting at mI from previous call.
    ///   Note that constructor guarantees mX is in strictly ascending order.
    if (x >= mX[mI+1]) {
        // If x increased enough, search up.
        for (int i = mI + 1; i < mM; ++i) {
            if (mX[i] >= x) {
                mI = i - 1;
                break;
            }
        }
    } else if (x < mX[mI]) {
        // If x decreased enough, search down.
        for (int i = mI - 1; i >= 0; --i) {
            if (mX[i] <= x) {
                mI = i;
                break;
            }
        }
    }

    /// - Store the fractional distance of the x argument across the bounding x scale points.
    //    No threat of division by zero since constructor guarantees mX[i+1] > mX[i].
    const double xFrac = (x - mX[mI]) / (mX[mI+1] - mX[mI]);

    /// - Initialize a return value of y.
    double y = mY[mJ];

    /// - Loop over adjacent pairs of y rows, and interpolate for values of z on the y scale values
    ///   at the given x.  Find a pair of these z values that bound the input argument z.  Start
    ///   with the previous y bounds of the last solution.  If the new solution is not within these
    ///   initial bounds, determine the search direction based on the local slope of the data.  Once
    ///   the search direction is determined, we will keep going in this direction until the
    ///   bounding set is found.
    int direction = 0;
    double zDelta = 1.0E16;
    double zTail  = mZ[mI][mJ]   + xFrac * (mZ[mI+1][mJ]   - mZ[mI][mJ]);
    double zHead  = mZ[mI][mJ+1] + xFrac * (mZ[mI+1][mJ+1] - mZ[mI][mJ+1]);
    for (int i=0; i<mN-1; ++i) {

        /// - Determine if z is between the interpolated z values at the current y bounds.  Note
        ///   that zTail can be either greater or less than zHead, order doesn't matter.
        if (isBetween(zTail, z, zHead)) {
            if (zHead != zTail) {
                /// - Interpolate between the bounding z values.
                y = mY[mJ] + (mY[mJ+1] - mY[mJ]) * (z - zTail) / (zHead - zTail);
            } else {
                /// - If the bounding z values are exactly equal, then there are an infinite number
                ///   of solutions for y = f(x, z) in this range, so the best we can do is pick the
                ///   middle of the range.
                y = 0.5 * (mY[mJ] + mY[mJ+1]);
            }
            break;
        } else {
            /// - Set y to the nearest of the z points, in case a solution is never found.
            const double zHeadD = fabs(z - zHead);
            const double zTailD = fabs(z - zTail);
            if (zHeadD < zDelta) {
                y = mY[mJ+1];
                zDelta = zHeadD;
            }
            if (zTailD < zDelta) {
                y = mY[mJ];
                zDelta = zTailD;
            }

            /// - If the first y location failed to bound the z input, we'll be searching up or down
            ///   the y scale.  Search in the direction pointing towards the z input based on the
            ///   local slope.
            if (0 == direction) {
                if (zTailD > zHeadD) {
                    direction =  1;
                } else {
                    direction = -1;
                }
            }

            /// - Increment the y scale points in the search direction and interpolate for new
            ///   bounding z values.  Wrap mJ around to the other end of the scale when an end is
            ///   passed.
            mJ += direction;
            if (mJ < 0) {
                // Searching backwards past the beginning, reset to the end.
                mJ = mN-2;
                zTail = mZ[mI][mJ]   + xFrac * (mZ[mI+1][mJ]   - mZ[mI][mJ]);
                zHead = mZ[mI][mJ+1] + xFrac * (mZ[mI+1][mJ+1] - mZ[mI][mJ+1]);
            } else if (mJ > mN-2) {
                // Searching forwards past the end, reset to the beginning.
                mJ = 0;
                zTail = mZ[mI][mJ]   + xFrac * (mZ[mI+1][mJ]   - mZ[mI][mJ]);
                zHead = mZ[mI][mJ+1] + xFrac * (mZ[mI+1][mJ+1] - mZ[mI][mJ+1]);
            } else if (direction > 0) {
                // Searching forward.
                zTail = zHead;
                zHead = mZ[mI][mJ+1] + xFrac * (mZ[mI+1][mJ+1] - mZ[mI][mJ+1]);
            } else {
                // Searching backward.
                zHead = zTail;
                zTail = mZ[mI][mJ]   + xFrac * (mZ[mI+1][mJ]   - mZ[mI][mJ]);
            }
        }
    }
    return y;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool (--) True if x is between end1 and end2, inclusive.
///
/// @details  Returns true if x is between end1 and end2, inclusive.  End1 and end2 can be in any
///           order.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsBilinearInterpolatorReverse::isBetween(const double end1, const double x, const double end2)
{
    if (end1 <= end2) {
        return MsMath::isInRange(end1, x, end2);
    } else {
        return MsMath::isInRange(end2, x, end1);
    }
}
