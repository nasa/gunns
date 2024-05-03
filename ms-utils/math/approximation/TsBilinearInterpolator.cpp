/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o))
 **************************************************************************************************/

#include "GenericMacros.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
//#include <sstream>

#include "TsBilinearInterpolator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this interpolator model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBilinearInterpolator::TsBilinearInterpolator()
    :
    TsApproximation(),
    mX(0),
    mY(0),
    mZ(0),
    mM(0),
    mN(0),
    mI(0),
    mJ(0)
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
/// @details  Constructs this Bilinear interpolator model taking coefficient and range arguments.
///           Determines whether both independent arrays are ordered, and swaps from descending to
///           ascending order if necessary.  getExceptional should be called to get a first-pass
///           value to minimize the time required for the initial search.
/// @throws TsInitializationException
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBilinearInterpolator::TsBilinearInterpolator(const double* x,    const double* y, const double* z,
                                               const int     m,    const int     n,
                                               const double  minX, const double  maxX,
                                               const double  minY, const double  maxY,
                                               const std::string &name)
:
        TsApproximation(),
        mX(0),
        mY(0),
        mZ(0),
        mM(m),
        mN(n),
        mI(0),
        mJ(0)
{
    init(x, y, z, m, n, minX, maxX, minY, maxY, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Bilinear interpolation.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsBilinearInterpolator::~TsBilinearInterpolator()
{
    cleanup();
//    for (int i = 0; i < mM; ++i) {
//        TS_DELETE_ARRAY(mZ[i]);
//    }
//    TS_DELETE_ARRAY(mZ);
//    TS_DELETE_ARRAY(mY);
//    TS_DELETE_ARRAY(mX);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes dynamic memory.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsBilinearInterpolator::cleanup()
{
    for (int i = 0; i < mM; ++i) {
        delete [] mZ[i];
        mZ[i] = 0;
    } {
        delete [] mZ;
        mZ = 0;
    } {
        delete [] mY;
        mY = 0;
    } {
        delete [] mX;
        mX = 0;
    }
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
/// @details  Initializes this Bilinear interpolator model taking coefficient and range arguments.
///           Determines whether both independent arrays are ordered, and swaps from descending to
///           ascending order if necessary. getExceptional should be called to get a first-pass
///           value to minimize the time required for the initial search.
/// @throws TsInitializationException
void TsBilinearInterpolator::init(const double* x,    const double* y, const double* z,
                                  const int     m,    const int     n,
                                  const double  minX, const double  maxX,
                                  const double  minY, const double  maxY,
                                  const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, minY, maxY, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    mM = m;
    mN = n;

    /// - Throw a TsInitializationException exception on first independent variable (x) array length (m) < 2.
    TS_GENERIC_IF_ERREX((m < 2), TsInitializationException,
                        "Invalid Input Argument", "first independent variable (x) array length (m) < 2.");

    /// - Throw a TsInitializationException exception on second independent variable (y) array length (n) < 2.
    TS_GENERIC_IF_ERREX((n < 2), TsInitializationException,
                        "Invalid Input Argument", "second independent variable (y) array length (n) < 2.");

    /// - Throw a TsInitializationException exception on null pointer to first independent variable (x) array.
    TS_GENERIC_IF_ERREX((0 == x), TsInitializationException,
                        "Invalid Input Argument", "null pointer to first independent variable (x) array.");

    /// - Throw a TsInitializationException exception on null pointer to second independent variable (y) array.
    TS_GENERIC_IF_ERREX((0 == y), TsInitializationException,
                        "Invalid Input Argument", "null pointer to second independent variable (y) array.");

    /// - Throw a TsInitializationException exception on null pointer to dependent variable (z) array.
    TS_GENERIC_IF_ERREX((0 == z), TsInitializationException,
                        "Invalid Input Argument", "null pointer to dependent variable (z) array.");

    /// - Validate first independent variable (x) array
    for (int i = 0; i < (mM - 2); ++i) {
        /// - Throw an exception if first independent variable (x) array is not strictly ordered (numbers in array decrease then increase or are equal).
        TS_GENERIC_IF_ERREX(((x[i] >= x[i + 1]) && (x[i + 2] >= x[i + 1])), TsInitializationException,
                            "Invalid Input Argument", "first independent variable (x) array not strictly ordered.");
        TS_GENERIC_IF_ERREX((x[i] <= x[i + 1]) && (x[i + 2] <= x[i + 1]), TsInitializationException,
                            "Invalid Input Argument", "first independent variable (x) array not strictly ordered.");
        /// - Throw an exception if the difference between first independent variable (x) array values aren't large enough.
        TS_GENERIC_IF_ERREX(((fabs(x[i]-x[i+1]) < DBL_EPSILON)), TsInitializationException,
                            "Invalid Input Argument", "difference between first independent variable (x) array values not large enough.");
    }

    /// - Validate second independent variable (y) array
    for (int i = 0; i < (mN - 2); ++i) {
        /// - Throw an exception if second independent variable (y) array is not strictly ordered (numbers in array decrease then increase or are equal).
        TS_GENERIC_IF_ERREX(((y[i] >= y[i + 1]) && (y[i + 2] >= y[i + 1])), TsInitializationException,
                            "Invalid Input Argument", "second independent variable (x) array not strictly ordered.");
        TS_GENERIC_IF_ERREX((y[i] <= y[i + 1]) && (y[i + 2] <= y[i + 1]), TsInitializationException,
                            "Invalid Input Argument", "second independent variable (x) array not strictly ordered.");
        /// - Throw an exception if the difference between second independent variable (y) array values aren't large enough.
        TS_GENERIC_IF_ERREX(((fabs(y[i]-y[i+1]) < DBL_EPSILON)), TsInitializationException,
                            "Invalid Input Argument", "difference between second independent variable (y) array values not large enough.");
    }

    mX = new double[mM];
    mY = new double[mN];
    mZ = new double*[mM];
//    TS_NEW_PRIM_ARRAY_EXT(mX, mM, double, name + ".mX");
//    TS_NEW_PRIM_ARRAY_EXT(mY, mN, double, name + ".mY");
//    TS_NEW_PRIM_POINTER_ARRAY_EXT(mZ, mM, double, name + ".mZ");
    for (int i = 0; i < mM; ++i) {
        mZ[i] = new double[mN];
//        std::ostringstream oss;
//        oss << i;
//        TS_NEW_PRIM_ARRAY_EXT(mZ[i], mN, double, name + ".mZ[" + oss.str() + "]");
    }
    int zz = 0;
    for (int i = 0; i < mM; ++i) {
        for (int j = 0; j < mN; ++j) {
            mZ[i][j] = z[zz];
            ++zz;
        }
    }

    /// - Create X array in ascending order.
    if (x[1] < x[0]) {
        /// - X array is strictly descending,so make ascending and change order of
        ///   Z array to reflect changes made to X.
        for (int i = 0; i < mM/2+1; ++i) {
            mX[i] = x[mM - i - 1];
            mX[mM - i - 1] = x[i];
            for (int j = 0; j < mN; ++j) {
                std::swap(mZ[i][j], mZ[mM - i - 1][j]);
            }
        }
    } else {
        /// - X array is strictly ascending and need only be created.
        for (int i = 0; i < mM; ++i) {
            mX[i] = x[i];
        }
    }

    /// - Create Y array in ascending order.
    if (y[1] < y[0]) {
        /// - Y array is strictly descending,so make ascending and change order of
        ///   Z array to reflect changes made to X.
        for (int i = 0; i < mN/2+1; ++i) {
            mY[i] = y[mN - i - 1];
            mY[mN - i - 1] = y[i];
            for (int j = 0; j < mM; ++j) {
                std::swap(mZ[j][i], mZ[j][mN - i - 1]);
            }
        }
    } else {
        /// - Y array is strictly ascending and need only be created.
        for (int i = 0; i < mN ; ++i) {
            mY[i] = y[i];
        }
    }

    /// - Throw an exception if mMinX < mX[0] OR mMaxX > mX[mM-1].
    ///   mX is used to ensure input array is in strictly ascending order.
    if ((mMinX < mX[0])||(mMaxX > mX[mM-1])) {
//        for (int i = 0; i < mM; ++i) {
//            TS_DELETE_ARRAY(mZ[i]);
//        }
//        TS_DELETE_ARRAY(mZ);
//        TS_DELETE_ARRAY(mY);
//        TS_DELETE_ARRAY(mX);
        cleanup();
        TS_GENERIC_ERREX(TsInitializationException, "Invalid Input Argument", "first independent variable (x) array does not cover valid range.");
    }

    /// - Throw an exception if mMinY < mY[0] OR mMaxY > mY[mM-1].
    ///   mY is used to ensure input array is in strictly ascending order.
    if ((mMinY < mY[0])||(mMaxY > mY[mN-1])) {
//        for (int i = 0; i < mM; ++i) {
//            TS_DELETE_ARRAY(mZ[i]);
//        }
//        TS_DELETE_ARRAY(mZ);
//        TS_DELETE_ARRAY(mY);
//        TS_DELETE_ARRAY(mX);
        cleanup();
        TS_GENERIC_ERREX(TsInitializationException, "Invalid Input Argument", "second independent variable (y) array does not cover valid range.");
    }

    mM = m;
    mN = n;
    mI = 0;
    mJ = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for bilinear interpolation.
/// @param[in]    y  (--)  Second independent variable for bilinear interpolation.
///
/// @return   bilinear interpolated dependent variable value at specified input.
///
/// @details  Returns this bilinear interpolated for the specified variable.
///           Saves previous indices mI and mJ.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsBilinearInterpolator::evaluate(const double x, const double y)
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

    /// - Find index i such that mY[i] <= y < mY[i+1] starting at mJ from previous call.
    ///   Note that constructor guarantees mY is in strictly ascending order.
    if (y >= mY[mJ+1]) {
        // If y increased enough, search up.
        for (int i = mJ + 1; i < mN; ++i) {
            if (mY[i] >= y) {
                mJ = i - 1;
                break;
            }
        }
    } else if (y < mY[mJ]) {
        // If y decreased enough, search down.
        for (int i = mJ - 1; i >= 0; --i) {
            if (mY[i] <= y) {
                mJ = i;
                break;
            }
        }
    }

    /// - Return the bilinearly interpolated value
    // No threat of division by zero since constructor guarantees mX[i+1] > mX[i] and mY[j+1] > mY[j]
    const double XDifInv = 1.0 / (mX[mI+1] - mX[mI]);
    const double Z1      = ((mX[mI+1] - x) * mZ[mI][mJ]   + (x - mX[mI]) * mZ[mI+1][mJ]);
    const double Z2      = ((mX[mI+1] - x) * mZ[mI][mJ+1] + (x - mX[mI]) * mZ[mI+1][mJ+1]);
    return (Z1 * (mY[mJ+1] - y) + Z2 * (y - mY[mJ])) * XDifInv / (mY[mJ+1] - mY[mJ]);
}
