/************************** TRICK HEADER **********************************************************
 LIBRARY DEPENDENCY:
    ((TsApproximation.o)
     (simulation/hs/TsHsMsg.o))
 **************************************************************************************************/

#include "GenericMacros.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "TsLinearInterpolator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Linear Interpolator model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLinearInterpolator::TsLinearInterpolator()
    :
    TsApproximation(),
    mX(0),
    mZ(0),
    mM(0),
    mI(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x     (--) Independent variable array.
/// @param[in]    z     (--) Dependent variable array.
/// @param[in]    n     (--) Size of variable arrays.
/// @param[in]    minX  (--) Interpolator model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Interpolator model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Constructs this Linear interpolator model taking coefficient and range arguments.
///           Determines whether independent array is strictly ordered, and swaps from descending to
///           ascending order if necessary.  getExceptional should be called to get a first-pass value to minimize
///           the time required for the initial search.
/// @throws TsInitializationException
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLinearInterpolator::TsLinearInterpolator(const double* x,   const double* z,   const int n,
                                           const double minX, const double maxX,
                                           const std::string &name)
        :
        TsApproximation(),
        mX(0),
        mZ(0),
        mM(n),
        mI(0)
{
    init(x, z, n, minX, maxX, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Linear Interpolator model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsLinearInterpolator::~TsLinearInterpolator()
{
    cleanup();
//    TS_DELETE_ARRAY(mX);
//    TS_DELETE_ARRAY(mZ);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes dynamic memory.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsLinearInterpolator::cleanup()
{
    {
        delete [] mX;
        mX = 0;
    } {
        delete [] mZ;
        mZ = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] n (--) size of the array
/// @param[in] x (--) array
/// @details  Validates the array size is at least two, and the the array is strictly ordered -
///           either increasing or decreasing as the difference between subsequent cells is at
///           least DBL_EPSILON.
/// @throws TsInitializationException
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsLinearInterpolator::validateOrdered(const int n, const double x[])
{
    /// - Throw a TsInitializationException exception on array length (n) < 2.
    TS_GENERIC_IF_ERREX((n < 2), TsInitializationException,
                        "Invalid Input Argument", "independent variable (x) array length (n) < 2.");

    /// - Throw a TsInitializationException exception on null pointer to independent variable (x) array.
    TS_GENERIC_IF_ERREX((0 == x), TsInitializationException,
                        "Invalid Input Argument", "null pointer to independent variable (x) array.");

    for (int i = 0; i < (n - 2); ++i) {
        /// - Throw a TsInitializationException exception on independent variable (x) array not strictly ordered.
        TS_GENERIC_IF_ERREX(((x[i] >= x[i + 1]) && (x[i + 2] >= x[i + 1])), TsInitializationException,
                            "Invalid Input Argument", "independent variable (x) array not strictly ordered.");
        TS_GENERIC_IF_ERREX(((x[i] <= x[i + 1]) && (x[i + 2] <= x[i + 1])), TsInitializationException,
                            "Invalid Input Argument", "independent variable (x) array not strictly ordered.");
        /// - Throw an exception if the difference between independent variable (x) array values aren't large enough.
        TS_GENERIC_IF_ERREX(((fabs(x[i]-x[i+1]) < DBL_EPSILON)), TsInitializationException,
                            "Invalid Input Argument", "difference between independent variable (x) array values not large enough.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x     (--) Independent variable array.
/// @param[in]    z     (--) Dependent variable array.
/// @param[in]    n     (--) Size of variable arrays.
/// @param[in]    minX  (--) Interpolator model valid range lower limit for first variable.
/// @param[in]    maxX  (--) Interpolator model valid range upper limit for first variable.
/// @param[in]    name  (--) name for the instance.
///
/// @details  Initialization of linear interpolator. Validates parameters and creates copy of
/// the x and y data.  getExceptional should be called to get a first-pass value to minimize
/// the time required for the initial search.
/// @throws TsInitializationException
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsLinearInterpolator::init(const double *x, const double *z, const int n,
                                const double minX, const double maxX, const std::string &name)
{
    /// - Initialize the parent
    TsApproximation::init(minX, maxX, -FLT_EPSILON, +FLT_EPSILON, name);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - validate x and n
    validateOrdered(n, x);

    /// - Throw a TsInitializationException exception on null pointer to dependent variable (z) array.
    TS_GENERIC_IF_ERREX((0 == z), TsInitializationException,
                        "Invalid Input Argument", "null pointer to dependent variable (z) array.");

    mM = n;
    cleanup();
//    TS_DELETE_ARRAY(mX);
//    TS_DELETE_ARRAY(mZ);

    mX = new double[mM];
    mZ = new double[mM];
//    TS_NEW_PRIM_ARRAY_EXT(mX, mM, double, name + ".mX");
//    TS_NEW_PRIM_ARRAY_EXT(mZ, mM, double, name + ".mZ");

    /// - Create array in ascending order.
    if (x[1] < x[0]) {
        /// - X array is strictly descending, so make ascending and change order of
        ///   Z array to reflect changes made to X.
        for (int i = 0; i < mM; ++i) {
            mX[i] = x[mM - i - 1];
            mZ[i] = z[mM - i - 1];
        }
    } else {
        /// - X array is strictly ascending and need only be created.
        for (int i = 0; i < mM; ++i) {
            mX[i] = x[i];
            mZ[i] = z[i];
        }
    }

    mMinX = minX;
    mMaxX = maxX;
    mMinY = -FLT_EPSILON;
    mMaxY =  FLT_EPSILON;

    /// - Throw an exception if independent variable (x) array does not cover valid range.
    ///   mX is used to ensure input array is in strictly ascending order.
    if ((mMinX < mX[0])||(mMaxX > mX[mM-1])) {
        cleanup();
//        TS_DELETE_ARRAY(mZ);
//        TS_DELETE_ARRAY(mX);
        TS_GENERIC_ERREX(TsInitializationException, "Invalid Input Argument", "independent variable (x) array does not cover valid range.");
    }

    /// - Set the flag to indicate successful initialization.
    mInitFlag = true;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      x      (--) value to find index for cell
/// @param[in]      mX     (--) array of cell bounds
/// @param[in]      size   (--) number of elements of mX
/// @param[in,out]  cIndex (--) previous index found
/// @details  determines index:
///      mIndex  criteria
///         0       x < mX[0]
///         i    mX[i] <= x <mX[i+1]
///         N-1  mX[N-1] < x, where N is the table size
/// Searches linearly staring from cIndex - steps to next cell
/// Provides a basis for refactoring out the search algorithm. An alternate approach could be to use
/// bisection (if there is no expectation that the next x value will be close to the current value.
/// @return index of cell for x
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int  TsLinearInterpolator::selectCell(const double x, const double mX[], const int size, int cIndex)
{

    if (x >= mX[cIndex+1]) {
        cIndex++;
        // If x increased enough, search up.
        for (; cIndex < size-1; ++cIndex) {
            if (mX[cIndex] > x) {
                break;
            }
        }
        cIndex--;
    } else if (x < mX[cIndex] && cIndex > 0) {
        // If x decreased enough, search down.
       cIndex--;
        for (; cIndex > 0; --cIndex) {
            if (mX[cIndex] <= x) {
                break;
            }
        }
    }
    return cIndex;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x  (--)  First independent variable for interpolator model.
/// @param[in]    y  (--)  Second independent variable for interpolator model (defaults to zero since not used).
///
/// @return   Interpolated dependent variable value at specified input.
///
/// @details  Returns this linear interpolator model for the specified variable.
///           The user of this method is responsible for ensuring initialization has occurred.
///           Saves previous index mI.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double TsLinearInterpolator::evaluate(const double x, const double y __attribute__((unused)))
{

    mI = selectCell(x, mX, mM, mI);
    /// - Return the linearly interpolated value.
    return mZ[mI] + (mZ[mI+1] - mZ[mI]) * (x - mX[mI]) / (mX[mI+1] - mX[mI]);
}


