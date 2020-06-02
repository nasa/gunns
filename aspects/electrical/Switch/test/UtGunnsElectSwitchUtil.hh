#ifndef UtGunnsElectSwitchUtil_EXISTS_
#define UtGunnsElectSwitchUtil_EXISTS_

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_SWITCH_UTIL Electrical Switch Utility class Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  This is a copy of UtGunnsElectSwitchUtil, with the name change from Switch to GunnsElectSwitchUtil.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "aspects/electrical/Switch/GunnsElectSwitchUtil.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// create a Friend class instance of the GunnsElectSwitchUtil class to access
/// all it's data.  Use it in unit tests
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsElectSwitchUtil: public GunnsElectSwitchUtil {
public:
    FriendlyGunnsElectSwitchUtil();
    virtual ~FriendlyGunnsElectSwitchUtil();
    friend class UtGunnsElectSwitchUtil;
};

inline FriendlyGunnsElectSwitchUtil::FriendlyGunnsElectSwitchUtil() : GunnsElectSwitchUtil() {}
inline FriendlyGunnsElectSwitchUtil::~FriendlyGunnsElectSwitchUtil() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// object that actually runs the tests
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsElectSwitchUtil : public  CppUnit::TestFixture {
public:
    UtGunnsElectSwitchUtil();
    virtual ~UtGunnsElectSwitchUtil();

    // required by CPPUNIT:
    void setUp();

    void tearDown();

    // my tests
    void testDefaultConstruction();
    void testInitialization();
    void testConfigData();
    void testInputData();

    void testCloseCmd();            // that a switch closes when command is received
    void testOpenCmd();             // that a switch opens when command is received
    void testNominalCurrentFlow();  // that a switch flows current that is under the trip setpoint
    void testNoCurrentWhenOpen();   // that the switch does not flow current when open
    void testOverCurrentTrip();     // that a switch opens when the current is above the trip point
    //void testUnderVoltTrip();       // that a switch opens when the voltage is less than the min
    void testGetPowerDissipation(); // return power dissipated by the switch
    void testGetCurrent();          // return current passing through the switch
    void testGetConductance();      // return switch conductance
    void testIsClosed();            // return open/closed state of the switch
    void testIsTripped();           // return trip status of the switch
    void testIsWaitingToTripPos();  // return that switch will trip but not in current minor step
    void testIsWaitingToTripNeg();  // return that switch will trip but not in current minor step
    void testIsTwoPortSwitch();     // return whether this switch powers another RPCM
    void testMalfFailOpen();        // test malf where switch fails opened (won't close)
    void testMalfFailClosed();      // test malf where switch fails closed (won't open when tripped)
    //void testIsVoltTrip();          // return whether an under-volt trip happened
    void testIsPosTrip();           // return whether a positive current trip happened
    void testIsNegTrip();           // return whether a negative current trip happened
    void testMalfSwitchResistanceLow();  // test malf where switch resistance changes
    void testMalfSwitchResistanceHigh();   // test malf where switch resistance changes
    void testTripReset();           // test that trip reset function clears out trips
    void testBadInitData();         // test init function safety checks
    void testMalfPosTripLimit();    // test malf to change positive trip limit
    void testMalfNegTripLimit();    // test malf to change negative trip limit
    void testGetPortAssigned();     // test for two port switches
    void testPowerSupplyValid();    // test that switch opens when power supply is invalid
    void testgetOutputVolts();      // test switch output volts (0 if open, value passed in computeFlow if closed

private:
    CPPUNIT_TEST_SUITE(UtGunnsElectSwitchUtil);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testInitialization);
    CPPUNIT_TEST(testOpenCmd);
    CPPUNIT_TEST(testCloseCmd);
    CPPUNIT_TEST(testNominalCurrentFlow);
    CPPUNIT_TEST(testNoCurrentWhenOpen);
    CPPUNIT_TEST(testOverCurrentTrip);
    //CPPUNIT_TEST(testUnderVoltTrip);
    CPPUNIT_TEST(testGetPowerDissipation);
    CPPUNIT_TEST(testGetCurrent);
    CPPUNIT_TEST(testGetConductance);
    CPPUNIT_TEST(testIsClosed);
    CPPUNIT_TEST(testIsTripped);
    CPPUNIT_TEST(testIsWaitingToTripPos);
    CPPUNIT_TEST(testIsWaitingToTripNeg);
    CPPUNIT_TEST(testIsTwoPortSwitch);
    CPPUNIT_TEST(testMalfFailClosed);
    CPPUNIT_TEST(testMalfFailOpen);
    //CPPUNIT_TEST(testIsVoltTrip);
    CPPUNIT_TEST(testIsPosTrip);
    CPPUNIT_TEST(testIsNegTrip);
    CPPUNIT_TEST(testMalfSwitchResistanceLow);
    CPPUNIT_TEST(testMalfSwitchResistanceHigh);
    CPPUNIT_TEST(testTripReset);
    CPPUNIT_TEST(testBadInitData);
    CPPUNIT_TEST(testMalfPosTripLimit);
    CPPUNIT_TEST(testMalfNegTripLimit);
    CPPUNIT_TEST(testGetPortAssigned);
    CPPUNIT_TEST(testPowerSupplyValid);
    CPPUNIT_TEST(testgetOutputVolts);
    CPPUNIT_TEST_SUITE_END();

    GunnsElectSwitchUtilConfigData* tConfigData;
    GunnsElectSwitchUtilConfigData* tempConfigData;
    GunnsElectSwitchUtilInputData* tInputData;
    GunnsElectSwitchUtilInputData* tempInputData;

    FriendlyGunnsElectSwitchUtil* tArticle;

    // config data
    double tSwitchOnResistance;
    bool tOverCurrentProtection;
    bool tUnderVoltProtection;
    double tMinVoltage;
    bool tIsTwoPortSwitch;
    int tPortAssigned;
    int tTripPriority;

    // input data
    bool tSwitchMalfFlag;
    int tSwitchMalf;
    bool tSwitchIsClosed;
    double tPosTripLimit;
    double tNegTripLimit;

    double tTolerance;

    UtGunnsElectSwitchUtil(const UtGunnsElectSwitchUtil &that);
    UtGunnsElectSwitchUtil &operator = (const UtGunnsElectSwitchUtil &that);

};

/// @}

#endif
