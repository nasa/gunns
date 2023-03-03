/**
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((aspects/dynamics/GunnsDynEulerFactory.o))
*/

#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "strings/UtResult.hh"
#include <cmath>
#include <iostream>

#include "UtGunnsDynEuler.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"
#include "math/UnitConversion.hh"

/// @details  Test identification number.
int UtGunnsDynEuler::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Euler Angles unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynEuler::UtGunnsDynEuler()
    :
    CppUnit::TestFixture(),
    tEulerAngles(),
    tFactory()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Euler Angles unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynEuler::~UtGunnsDynEuler()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynEuler::setUp()
{
    /// - Define nominal euler angles.
    tEulerAngles[0] = 0.0;
    tEulerAngles[1] = 0.0;
    tEulerAngles[2] = 0.0;

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynEuler::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Euler Angles Factory given an invalid sequence.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynEuler::testInvalidSequence()
{
    UT_RESULT;

    CPPUNIT_ASSERT_THROW(tFactory.create(static_cast<GunnsDynEulerFactory::Sequences>(42)),
                         TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Euler Angles update method handling singularity conditions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynEuler::testSingularity()
{
    UT_RESULT;

    GunnsDynEulerBase* article = tFactory.create(GunnsDynEulerFactory::XYZ);

    /// @test the +pi/2 singularity inside the asin function range.
    tEulerAngles[0] = 0.0;
    tEulerAngles[1] = UnitConversion::PI_OVER_2;
    tEulerAngles[2] = 0.0;

    double quatOne = sqrt( (1.0 - DBL_EPSILON) / 2.0 );
    double quat[4] = {quatOne, 0.0, -quatOne, 0.0};     // this is the quat for {0, 90, 0} rotations

    double* euler = article->update(quat);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[0], euler[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[1], euler[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[2], euler[2], DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(1, article->getSingularityCondition());

    /// @test the +pi/2 singularity outside the asin function range but within acceptable tolerance.
    quatOne = sqrt( (1.0 + 5.0E-16) / 2.0 );
    quat[0] =  quatOne;
    quat[2] = -quatOne;

    euler = article->update(quat);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[0], euler[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[1], euler[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[2], euler[2], DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(3, article->getSingularityCondition());

    /// @test the -pi/2 singularity inside the asin function range.
    tEulerAngles[0] = 0.0;
    tEulerAngles[1] = -UnitConversion::PI_OVER_2;
    tEulerAngles[2] = 0.0;

    quatOne = sqrt( (1.0 - DBL_EPSILON) / 2.0 );
    quat[0] = quatOne;
    quat[2] = quatOne;

    euler = article->update(quat);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[0], euler[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[1], euler[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[2], euler[2], DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(2, article->getSingularityCondition());

    /// @test the -pi/2 singularity outside the asin function range but within acceptable tolerance.
    quatOne = sqrt( (1.0 + 5.0E-16) / 2.0 );
    quat[0] = quatOne;
    quat[2] = quatOne;

    euler = article->update(quat);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[0], euler[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[1], euler[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tEulerAngles[2], euler[2], DBL_EPSILON);
    CPPUNIT_ASSERT_EQUAL(4, article->getSingularityCondition());

    /// @test the error & exception thrown when outside the acceptable tolerance of asin range.
    quatOne = sqrt( (1.0 + 2.0E-15) / 2.0);
    quat[0] = quatOne;
    quat[2] = quatOne;

    CPPUNIT_ASSERT_THROW(article->update(quat), TsNumericalException);
    CPPUNIT_ASSERT(std::isnan(euler[0]));
    CPPUNIT_ASSERT(std::isnan(euler[1]));
    CPPUNIT_ASSERT(std::isnan(euler[2]));
    CPPUNIT_ASSERT_EQUAL(5, article->getSingularityCondition());

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Euler Angles update method in normal conditions, for all sequences.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynEuler::testUpdate()
{
    UT_RESULT;

    GunnsDynEulerFactory::Sequences sequences[6] = {
            GunnsDynEulerFactory::XYZ,
            GunnsDynEulerFactory::XZY,
            GunnsDynEulerFactory::YXZ,
            GunnsDynEulerFactory::YZX,
            GunnsDynEulerFactory::ZXY,
            GunnsDynEulerFactory::ZYX
    };

    for (unsigned int seq=0; seq<6; ++seq) {
        std::stringstream ss;
        ss << "Sequence: " << sequences[seq];

        GunnsDynEulerBase* article = tFactory.create(sequences[seq]);

        double quat[4];
        GunnsDynUtils::zeroV(quat, 4);

        /// @test angles (0, 0, 0)
        tEulerAngles[0] = 0.0;
        tEulerAngles[1] = 0.0;
        tEulerAngles[2] = 0.0;
        article->computeQuat(quat, tEulerAngles);
        double* euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL(0, article->getSingularityCondition());

        /// @test angles (90, 0, 0) deg.
        tEulerAngles[0] = UnitConversion::PI_OVER_2;
        tEulerAngles[1] = 0.0;
        tEulerAngles[2] = 0.0;
        article->computeQuat(quat, tEulerAngles);
        euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);

        /// @test angles (-90, 0, 0) deg.
        tEulerAngles[0] = -UnitConversion::PI_OVER_2;
        tEulerAngles[1] = 0.0;
        tEulerAngles[2] = 0.0;
        article->computeQuat(quat, tEulerAngles);
        euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);

        /// @test angles (0, 0, 90) deg.
        tEulerAngles[0] = 0.0;
        tEulerAngles[1] = 0.0;
        tEulerAngles[2] = UnitConversion::PI_OVER_2;
        article->computeQuat(quat, tEulerAngles);
        euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);

        /// @test angles (0, 0, -90) deg.
        tEulerAngles[0] = 0.0;
        tEulerAngles[1] = 0.0;
        tEulerAngles[2] = -UnitConversion::PI_OVER_2;
        article->computeQuat(quat, tEulerAngles);
        euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);

        /// @test angles (0, 90, 0) deg.
        tEulerAngles[0] = 0.0;
        tEulerAngles[1] = UnitConversion::PI_OVER_2;
        tEulerAngles[2] = 0.0;
        article->computeQuat(quat, tEulerAngles);
        euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);

        /// @test angles (0, -90, 0) deg.
        tEulerAngles[0] = 0.0;
        tEulerAngles[1] = -UnitConversion::PI_OVER_2;
        tEulerAngles[2] = 0.0;
        article->computeQuat(quat, tEulerAngles);
        euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);

        /// @test angles (45, 45, 45) deg.
        tEulerAngles[0] = UnitConversion::PI_OVER_4;
        tEulerAngles[1] = UnitConversion::PI_OVER_4;
        tEulerAngles[2] = UnitConversion::PI_OVER_4;
        article->computeQuat(quat, tEulerAngles);
        euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL(0, article->getSingularityCondition());

        /// @test angles (-45, -45, -45) deg.
        tEulerAngles[0] = -UnitConversion::PI_OVER_4;
        tEulerAngles[1] = -UnitConversion::PI_OVER_4;
        tEulerAngles[2] = -UnitConversion::PI_OVER_4;
        article->computeQuat(quat, tEulerAngles);
        euler = article->update(quat);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[0], euler[0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[1], euler[1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(ss.str(), tEulerAngles[2], euler[2], DBL_EPSILON);
        CPPUNIT_ASSERT_EQUAL(0, article->getSingularityCondition());
    }

    UT_PASS_LAST;
}
