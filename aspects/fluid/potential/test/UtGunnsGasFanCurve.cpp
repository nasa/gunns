/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((aspects/fluid/potential/GunnsGasFanCurve.o))
***************************************************************************************************/

#include "software/exceptions/TsNumericalException.hh"
#include "strings/UtResult.hh"

#include "UtGunnsGasFanCurve.hh"

/// @details  Test identification number.
int UtGunnsGasFanCurve::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Gas Fan Curve unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsGasFanCurve::UtGunnsGasFanCurve()
    :
    CppUnit::TestFixture(),
    tArticle()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gas Fan Curve unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsGasFanCurve::~UtGunnsGasFanCurve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFanCurve::setUp()
{
    tArticle = new FriendlyGunnsGasFanCurve();

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFanCurve::tearDown()
{
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the default construction of the GUNNS Gas Fan Curve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFanCurve::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    CPPUNIT_ASSERT(0 == tArticle->mCoeffs);
    CPPUNIT_ASSERT(0 == tArticle->mIterLaguerre);
    CPPUNIT_ASSERT(0 == tArticle->mIterBrent);

    /// - New/delete for code coverage.
    GunnsGasFanCurve* article = new GunnsGasFanCurve();
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the setCoeffs method of the GUNNS Gas Fan Curve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFanCurve::testSetCoeffs()
{
    UT_RESULT;

    double coeffs[6] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    tArticle->setCoeffs(coeffs);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(coeffs[0], tArticle->mCoeffs[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(coeffs[1], tArticle->mCoeffs[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(coeffs[2], tArticle->mCoeffs[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(coeffs[3], tArticle->mCoeffs[3], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(coeffs[4], tArticle->mCoeffs[4], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(coeffs[5], tArticle->mCoeffs[5], 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the evaluate method of the GUNNS Gas Fan Curve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFanCurve::testEvaluate()
{
    UT_RESULT;

    double coeffs[6] = {1.0, -2.0, 3.0, -4.0, 5.0, -6.0};
    tArticle->setCoeffs(coeffs);

    double x = 42.0;
    double expected = coeffs[0] + (coeffs[1] + (coeffs[2]
                   + (coeffs[3] + (coeffs[4] +  coeffs[5] * x) * x) * x) * x) * x;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->evaluate(x), DBL_EPSILON);

    /// - Verify the code form is equivalent to expanded-out algebra:
    expected = coeffs[0] + (coeffs[1]*x) + (coeffs[2]*x*x) + (coeffs[3]*x*x*x) + (coeffs[4]*x*x*x*x)
             + (coeffs[5]*x*x*x*x*x);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->evaluate(x), DBL_EPSILON);

    /// - Verify evaluate returns zero if passed a null-pointer to coefficients.
    double* nullCoeffs = 0;
    expected           = 0.0;
    tArticle->setCoeffs(nullCoeffs);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, tArticle->evaluate(x), DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the improveRoot method of the GUNNS Gas Fan Curve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFanCurve::testImproveRoot()
{
    UT_RESULT;

    /// - This curve exercises both Laguerre and Brent's.  It roughly resembles an ISS IMV fan.  It
    ///   has a real root at q = 0.0904418832603137, and complex roots at q = 0.0113537358291104 and
    ///   q = 0.0964552700239473 (m3/s), from from a free online root-finding site:
    ///   http://xrjunque.nom.es/precis/rootfinder.aspx
    ///   Laguerre's initially converges on a complex root, so improveRoot must try Brent's.
    double coeffs[6] = {0.357, -24.6528, 1167.09, -21093.2, 168250.0, -549729.0};
    double root      = 0.0;
    double expected  = 0.0904418832603137;
    tArticle->improveRoot(root, coeffs, 1000.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, root, FLT_EPSILON);
    CPPUNIT_ASSERT(0 <  tArticle->mIterLaguerre);
    CPPUNIT_ASSERT(0 <  tArticle->mIterBrent);

    /// - Now adjust the curve slightly to simulate the addition of a system curve that we must find
    ///   the intersection for.  The system curve is a polynomial so we only adjust the 2nd-order
    ///   coeff.  This is expected to only need the Laguerre method to improve the previous root.
    ///   This new curve has the real root at q = 0.063028342963641 (m3/s).
    coeffs[2]        = 1100.0;
    expected         = 0.063028342963641;
    tArticle->improveRoot(root, coeffs, 1000.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, root, FLT_EPSILON);
    CPPUNIT_ASSERT(0 <  tArticle->mIterLaguerre);
    CPPUNIT_ASSERT(0 == tArticle->mIterBrent);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests exceptions thrown by the improveRoot method of the GUNNS Gas Fan Curve class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFanCurve::testImproveRootExceptions()
{
    UT_RESULT;

    /// - Test a curve that has no roots at all.  This will cause Laguerre's method to throw and we
    ///   verify the catch & re-throw from improveRoot.
    double coeffs[6]        = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double root             = 0.0;
    tArticle->mIterLaguerre = -1;
    tArticle->mIterBrent    = -1;
    CPPUNIT_ASSERT_THROW(tArticle->improveRoot(root, coeffs, 1000.0), TsNumericalException);
    CPPUNIT_ASSERT(0 == tArticle->mIterLaguerre);
    CPPUNIT_ASSERT(0 == tArticle->mIterBrent);

    /// - Test a curve that goes into Brent's method but we give it bad bounds for the root, and
    ///   verify improveRoot catches & re-throws the exception from Brent's.
    double coeffs2[6]       = {0.357, -24.6528, 1167.09, -21093.2, 168250.0, -549729.0};
    root                    = 0.0;
    tArticle->mIterLaguerre = -1;
    tArticle->mIterBrent    = -1;
    CPPUNIT_ASSERT_THROW(tArticle->improveRoot(root, coeffs2, 0.01), TsNumericalException);
    CPPUNIT_ASSERT(0 <  tArticle->mIterLaguerre);
    CPPUNIT_ASSERT(0 == tArticle->mIterBrent);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Gas Fan Curve getter methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsGasFanCurve::testAccessors()
{
    UT_RESULT;

    /// @test    The getLaguerreIterations and getBrentIterations methods.
    tArticle->mIterLaguerre = 42;
    tArticle->mIterBrent    = 3;
    CPPUNIT_ASSERT( 42 == tArticle->getLaguerreIterations());
    CPPUNIT_ASSERT( 3  == tArticle->getBrentIterations());

    UT_PASS_LAST;
}
