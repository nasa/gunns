/**
@copyright Copyright 2020 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((aspects/dynamics/GunnsDynUtilsFactory.o))
*/

#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "strings/UtResult.hh"
#include <cmath>
#include <iostream>

#include "UtGunnsDynUtils.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"
#include "aspects/dynamics/GunnsDynEuler123.hh"
#include "math/UnitConversion.hh"

/// @details  Test identification number.
int UtGunnsDynUtils::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Dynamics math utilities unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynUtils::UtGunnsDynUtils()
    :
    CppUnit::TestFixture()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Dynamics math utilities unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynUtils::~UtGunnsDynUtils()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynUtils::setUp()
{
    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynUtils::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Dynamics math utilities dotV method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynUtils::testDotV()
{
    UT_RESULT_FIRST;

    const double v1[3] = {1.0,  0.5, 0.0};
    const double v2[3] = {0.1, 10.0, 2.0};
    const double expectedDot = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    const double result = GunnsDynUtils::dotV(v1, v2, 3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDot, result, DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Dynamics math utilities MtoQ method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynUtils::testMtoQ()
{
    UT_RESULT;

    /// @test a normal rotation matrix.
    const double eulerAngles[3] = {30.0 * UnitConversion::RAD_PER_DEG,
                                  -15.0 * UnitConversion::RAD_PER_DEG,
                                  160.0 * UnitConversion::RAD_PER_DEG};

    double qIn[4] = {0.0, 0.0, 0.0, 0.0};
    GunnsDynEuler123 eulerSequence;
    eulerSequence.computeQuat(qIn, eulerAngles);
    double matrix[9];
    GunnsDynUtils::QtoM(matrix, qIn);
    double qOut[4] = {0.0, 0.0, 0.0, 0.0};
    GunnsDynUtils::MtoQ(qOut, matrix);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qIn[0], qOut[0], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qIn[1], qOut[1], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qIn[2], qOut[2], FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qIn[3], qOut[3], FLT_EPSILON);

    /// @test Identity matrix.
    double mIdentity[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    GunnsDynUtils::MtoQ(qOut, mIdentity);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, qOut[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, qOut[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, qOut[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, qOut[3], DBL_EPSILON);

    /// @test bad matrix.
    GunnsDynUtils::scaleV(mIdentity, mIdentity, -1.0, 9);
    CPPUNIT_ASSERT_THROW(GunnsDynUtils::MtoQ(qOut, mIdentity), TsNumericalException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Dynamics math utilities normalizeQ method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynUtils::testNormalizeQ()
{
    UT_RESULT;

    double qIn[4] = {0.5, -0.1, 0.4, 0.2};
    double magQ = GunnsDynUtils::magV(qIn, 4);
    double qNorm[4];
    GunnsDynUtils::scaleV(qNorm, qIn, 1.0 / magQ, 4);
    double qResult[4];
    GunnsDynUtils::setV(qResult, qIn, 4);
    GunnsDynUtils::normalizeQ(qResult);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qNorm[0], qResult[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qNorm[1], qResult[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qNorm[2], qResult[2], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(qNorm[3], qResult[3], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Dynamics math utilities normalizeV method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynUtils::testNormalizeV()
{
    UT_RESULT;

    /// @test a success result.
    double vec3[3] = {3.0, 4.0, 5.0};
    const double vec3Mag = sqrt(9.0 + 16.0 + 25.0);
    const double normVec3[3] = {vec3[0] / vec3Mag, vec3[1] / vec3Mag, vec3[2] / vec3Mag};
    CPPUNIT_ASSERT_NO_THROW(GunnsDynUtils::normalizeV(vec3));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(normVec3[0], vec3[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(normVec3[1], vec3[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(normVec3[2], vec3[2], DBL_EPSILON);

    /// @test a fail result, and that vector is unchanged.
    const double small = 0.1 * DBL_EPSILON;
    double smallVec[3] = {small, small, small};
    CPPUNIT_ASSERT_THROW(GunnsDynUtils::normalizeV(smallVec), TsNumericalException);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(small, smallVec[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(small, smallVec[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(small, smallVec[2], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Dynamics math utilities normalizeVSuccess method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynUtils::testNormalizeVSuccess()
{
    UT_RESULT;

    /// @test a success result.
    double vec2[2] = {3.0, 4.0};
    const double vec2Mag = 5.0;
    const double normVec2[2] = {vec2[0] / vec2Mag, vec2[1] / vec2Mag};
    CPPUNIT_ASSERT(true == GunnsDynUtils::normalizeVSuccess(vec2, 2));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(normVec2[0], vec2[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(normVec2[1], vec2[1], DBL_EPSILON);

    /// @test a fail result, and that vector is unchanged.
    const double small = 0.1 * DBL_EPSILON;
    double vec3[3] = {small, small, small};
    CPPUNIT_ASSERT(false == GunnsDynUtils::normalizeVSuccess(vec3, 3));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(small, vec3[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(small, vec3[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(small, vec3[2], DBL_EPSILON);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Dynamics math utilities setM method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynUtils::testSetM()
{
    UT_RESULT;

    const double m2[4] = {0.1, 0.2, 0.3, 0.4};
    double mResult[4]  = {0.0, 0.0, 0.0, 0.0};
    GunnsDynUtils::setM(mResult, m2, 2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m2[0], mResult[0], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m2[1], mResult[1], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m2[2], mResult[2], 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m2[3], mResult[3], 0.0);

    UT_PASS_LAST;
}
