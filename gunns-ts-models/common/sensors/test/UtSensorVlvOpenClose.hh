#ifndef UtSensorVlvOpenClose_EXISTS
#define UtSensorVlvOpenClose_EXISTS

/**
@defgroup  UT_ECLSS_FLUID_SensorVlvOpenClose  Unit tests for Imv Valve Sensor set
@ingroup   UT_ECLSS_FLUID

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
 - (UtSensorVlvOpenClose.o)

 PROGRAMMERS:
 - ((Chuck Sorensen) (LZT) (Dec 5, 2011) (TS21) (initial))
@{
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "../SensorVlvOpenClose.hh"

/// @brief unit test for Valve open and close Sensor set
class UtSensorVlvOpenClose : public CppUnit::TestFixture {

public:

    UtSensorVlvOpenClose();
    virtual ~UtSensorVlvOpenClose();
    void setUp();
    void tearDown();
    void testInitialization();
    void testConfigConstructor();
    void testAccessors();
    void testUpdate();



protected:

    SensorVlvOpenClose           mSensors;     //*< (--) sensors
    SensorVlvOpenCloseConfigData mConfig;      //*< (--) sensors config
    SensorVlvOpenCloseInputData  mInput;       //*< (--) sensors input
    SensorBooleanAiConfigData   mOpenConfig;  //*< (--) open sensor config
    SensorBooleanAiConfigData   mCloseConfig; //*< (--) close sensor config

private:
    CPPUNIT_TEST_SUITE(UtSensorVlvOpenClose);
    CPPUNIT_TEST(testInitialization);
    CPPUNIT_TEST(testConfigConstructor);
    CPPUNIT_TEST(testUpdate);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST_SUITE_END();

    /// @brief keep private, never used
    UtSensorVlvOpenClose(const UtSensorVlvOpenClose &rhs);

    /// @brief keep private, never used
    UtSensorVlvOpenClose& operator= (const UtSensorVlvOpenClose &rhs);

};


/// @}


#endif //UtSensorVlvOpenClose_EXISTS
