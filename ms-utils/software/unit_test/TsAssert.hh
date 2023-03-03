#ifndef TsAssert_EXISTS
#define TsAssert_EXISTS
/**
@defgroup  TSM_UTILITIES_SOFTWARE_UNIT_TEST_ASSERT Assert Macros
@ingroup   TSM_UTILITIES_SOFTWARE_UNIT_TEST

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
    (Additional Test Macros for CPP_UNIT )

PROGRAMMERS:
    ( ((Stephanie de Wet) (L3) (July 2009)     (CxTF) (--))
      ((Stephanie de Wet) (L3) (March 2011)    (TS21) (Change to macros))
      ((Ryan Prentice)    (L3) (November 2012) (TS21) (Added the auto_delta function and the
                                                       TS_ASSERT_DOUBLES_EQUAL macro))
    )

@{
*******************************************************************************/

#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// Compute the delta needed by the CppUnit CPPUNIT_ASSERT_DOUBLES_EQUAL macro based on the number
/// of significant figures the user wants to compare the expected value to the actual value.
///
/// @param[in] param    (--) The "expected" parameter passed to CPPUNIT_ASSERT_DOUBLES_EQUAL
/// @param[in] sig_figs (--) The number of non-zero significant figures the user wants to use to
///                          compare the expected with the actual
///
/// @return The delta needed for the CPPUNIT_ASSERT_DOUBLES_EQUAL
////////////////////////////////////////////////////////////////////////////////////////////////////
static double autoDelta(
        double param,
        int sig_figs)
{
    // Exit if the expected value is garbage
    if (std::isnan(param) || std::isinf(param)) {
        return param;
    }

    // Basically, never match if the sig figs is 0
    if (sig_figs == 0) {

        // XXX: Might be better to throw an exception here.
        return __FLT_MIN__;
    }

    double abs_param = std::abs(param);
    double result = abs_param;

    int i = 0;
    double delta = 0.0;

    const double one = 1.0;
    const double ten = 10.0;
    const double one_over_ten = 1.0/10.0;

    // Return 0.1 if the input parameter is 0
    if (abs_param == 0.0) {

        delta = 0.1;
    }
    // Handle the case where the input parameter is less than 1
    else if (abs_param <= one) {

        while (result < one) {

            ++i;
            result *= ten;
        }

        delta = std::pow(ten, -i - (sig_figs-1));
    }
    // Handle the case where the input parameter is greater than 1
    else {

        while (result > one) {

            ++i;
            result *= one_over_ten;
        }

        delta = std::pow(ten, i - sig_figs+1);
    }

    return delta;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// Wrapper macro to be able to pass the number of significant figure you would like to use for
/// comparison in a CPPUNIT_ASSERT_DOUBLES_EQUAL test instead of the absolute tolerance.
///
/// @param[in] expected (--) CPPUNIT_ASSERT_DOUBLES_EQUAL expected value
/// @param[in] actual   (--) CPPUNIT_ASSERT_DOUBLES_EQUAL actual value
/// @param[in] sig_figs (--) Number of significant figures the user would like to use to compute the
///                          tolerance for CPPUNIT_ASSERT_DOUBLES_EQUAL
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_ASSERT_DOUBLES_EQUAL_SIG_FIG(expected, actual, sig_figs) \
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, autoDelta(expected, sig_figs))

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// Wrapper macro to use the TS_ASSERT_DOUBLES_EQUAL macro for an array with 3 elements. This name
/// is a little confusing, since it is so close to TS_ASSERT_VECTOR_DOUBLES_EQUAL. It should
/// probably be changed in the future.
///
/// @param[in] expected (--) Expected 3-vector
/// @param[in] actual   (--) Actual 3-vector
/// @param[in] sig_figs (--) Significant figures used for comparison
////////////////////////////////////////////////////////////////////////////////////////////////////
#define TS_ASSERT_VECTOR_DOUBLES_EQUAL_SIG_FIG(expected, actual, sig_figs) \
        TS_ASSERT_DOUBLES_EQUAL_SIG_FIG( (expected)[0], (actual)[0], (sig_figs) ); \
        TS_ASSERT_DOUBLES_EQUAL_SIG_FIG( (expected)[1], (actual)[1], (sig_figs) ); \
        TS_ASSERT_DOUBLES_EQUAL_SIG_FIG( (expected)[2], (actual)[2], (sig_figs) )

//------------------------------------------------------------------------------------------
///  \param  expected   Expected 3-vector
///  \param  actual     Actual 3-vector
///  \param  tol        Scalar tolerance value
///
///  \details  Tests pass if the vectors are equal within the given tolerance
//------------------------------------------------------------------------------------------
#define TS_ASSERT_VECTOR_DOUBLES_EQUAL(expected, actual, tol) \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[0], (actual)[0], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[1], (actual)[1], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[2], (actual)[2], (tol) );

//------------------------------------------------------------------------------------------
///  \param  expected   Expected 3x3 matrix
///  \param  actual     Actual 3x3 matrix
///  \param  tol        Scalar tolerance value
///
///  \details  Tests pass if the matrices are equal within the given tolerance
//------------------------------------------------------------------------------------------
#define TS_ASSERT_MATRIX_DOUBLES_EQUAL(expected, actual, tol) \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[0][0], (actual)[0][0], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[0][1], (actual)[0][1], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[0][2], (actual)[0][2], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[1][0], (actual)[1][0], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[1][1], (actual)[1][1], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[1][2], (actual)[1][2], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[2][0], (actual)[2][0], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[2][1], (actual)[2][1], (tol) ); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL( (expected)[2][2], (actual)[2][2], (tol) );

/// @}

#endif  /* End of TsAssert_EXISTS - no more code after this line! */
