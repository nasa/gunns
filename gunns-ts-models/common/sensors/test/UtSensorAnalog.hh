#ifndef UtSensorAnalog_EXISTS
#define UtSensorAnalog_EXISTS

/**
@defgroup  <classname>  some text goes here
@ingroup   <ingroup>

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 - ()

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ()

 PROGRAMMERS:
 - ((Jason Harvey) (L-3 Communications) (2012-04) (TS21) (Initial))
@{
*/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "common/sensors/SensorAnalog.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from SensorAnalog and befriend UtSensorAnalog.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySensorAnalog : public SensorAnalog
{
    public:
        FriendlySensorAnalog();
        virtual ~FriendlySensorAnalog();
        friend class UtSensorAnalog;
};
inline FriendlySensorAnalog::FriendlySensorAnalog() : SensorAnalog() {}
inline FriendlySensorAnalog::~FriendlySensorAnalog() {}

class UtSensorAnalogNoise
{
    public:
        UtSensorAnalogNoise();
        virtual ~UtSensorAnalogNoise();
        static double testNoise() {return 1.0;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Unit tests for TsSensorBase
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtSensorAnalog : public CppUnit::TestFixture {

public:
    UtSensorAnalog(void);
    virtual ~UtSensorAnalog(void);
    void setUp();
    void tearDown();
    void testConfigData();
    void testInputData();
    void testConstruction();
    void testInitialize();
    void testInitializeExceptions();
    void testAccessors();
    void testInput();
    void testScale();
    void testBias();
    void testDrift();
    void testNoise();
    void testResolution();
    void testUpdateNominal();
    void testUpdateNoPower();
    void testMalfFailTo();
    void testMalfStuck();
    void testMalfPerfect();
    void testMalfInteractions();
    void testExternalNoiseFunctions();
    void testSense();
    void testTruthOutput();

private:
    CPPUNIT_TEST_SUITE(UtSensorAnalog);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testConstruction);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testInitializeExceptions);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testInput);
    CPPUNIT_TEST(testScale);
    CPPUNIT_TEST(testBias);
    CPPUNIT_TEST(testDrift);
    CPPUNIT_TEST(testNoise);
    CPPUNIT_TEST(testResolution);
    CPPUNIT_TEST(testUpdateNominal);
    CPPUNIT_TEST(testUpdateNoPower);
    CPPUNIT_TEST(testMalfFailTo);
    CPPUNIT_TEST(testMalfStuck);
    CPPUNIT_TEST(testMalfPerfect);
    CPPUNIT_TEST(testMalfInteractions);
    CPPUNIT_TEST(testExternalNoiseFunctions);
    CPPUNIT_TEST(testSense);
    CPPUNIT_TEST(testTruthOutput);
    CPPUNIT_TEST_SUITE_END();

    std::string               tName;                      /**< (--) Nominal config data */
    float                     tMinRange;                  /**< (--) Nominal config data */
    float                     tMaxRange;                  /**< (--) Nominal config data */
    float                     tOffValue;                  /**< (--) Nominal config data */
    float                     tNominalBias;               /**< (--) Nominal config data */
    float                     tNominalScale;              /**< (--) Nominal config data */
    float                     tNominalNoiseScale;         /**< (--) Nominal config data */
    float                     tNominalNoiseMult;          /**< (--) Nominal config data */
    float                     tNominalResolution;         /**< (--) Nominal config data */
    double                    (*tNominalNoiseFunction)(); /**< (--) Nominal config data */
    UnitConversion::Type      tUnitConversion;            /**< (--) Nominal config data */
    SensorAnalogConfigData*   tNominalConfig;             /**< (--) Nominal config data */
    bool                      tPowerFlag;                 /**< (--) Nominal input data */
    double                    tTruthInput;                /**< (--) Nominal input data */
    SensorAnalogInputData*    tNominalInput;              /**< (--) Nominal input data */
    double                    tTimeStep;                  /**< (--) Time step */

    /// @brief Copy constructor is not implemented.
    UtSensorAnalog(const UtSensorAnalog &rhs);

    /// @brief Assignment operator is not implemented.
    UtSensorAnalog& operator= (const UtSensorAnalog &rhs);
};

/// @}

#endif //UtSensorAnalog_EXISTS
