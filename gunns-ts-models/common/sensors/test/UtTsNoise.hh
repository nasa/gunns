#ifndef UtTsNoise_EXISTS
#define UtTsNoise_EXISTS


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TS_SENSOR_NOISE    Noise Unit Tests
/// @ingroup  UT_TS_SENSOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the noise model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "common/sensors/TsNoise.hh"


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Noise unit tests.
///
/// @details  This class provides the unit tests for the Noise model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsNoise : public CppUnit::TestFixture {

public:
    UtTsNoise();
    virtual ~UtTsNoise();
    void setUp();
    void tearDown();
    void testRange();
    void testMean();
    void testRepeatability();
    void testRandom();
    void testAccessors();
private:
    CPPUNIT_TEST_SUITE(UtTsNoise);
    CPPUNIT_TEST(testRange);
    CPPUNIT_TEST(testMean);
    CPPUNIT_TEST(testRepeatability);
    CPPUNIT_TEST(testRandom);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST_SUITE_END();
    /// @brief Copy constructor is not implemented.
    UtTsNoise(const UtTsNoise &rhs);
    /// @brief Assignment operator is not implemented.
    UtTsNoise& operator= (const UtTsNoise &rhs);
};

/// @}

#endif
