#ifndef UT_SWITCH_EXISTS_
#define UT_SWITCH_EXISTS_

/// @defgroup UT_SWITCH_HH EPS Switch class Unit Test
/// @ingroup  UT_GUNNS
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @{
/*!******************************** TRICK HEADER ***************************

@details
PURPOSE:
(
(Philip Trottman: This is the unit test class for the Switch model)
(which is used by the ISS RPCM, DCSU, MBSU, and BCDU devices)
)

REFERENCE:
((list here, if any))

ASSUMPTIONS AND LIMITATIONS:
((list here, if any))
 *
LIBRARY DEPENDENCY:
(
     (software/exceptions/TsInitializationException.o)
     (aspects/electrical/Switch/Switch.o)
)


PROGRAMMERS:
    (
     ((Philip Trottman) (USA) (January 26, 2012) (TS21) (Initial Version))
    )
 ***************************************************************************/



/*!
 * @brief TODO(Philip Trottman): Brief Description.
 *
 * @details TODO(Philip Trottman): Detailed description.
 *
 * @see TODO(Philip Trottman): Any References
 *
 */

#include <iostream>
#include "aspects/electrical/Switch/Switch.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// create a Friend class instance of the Switch class to access
/// all it's data.  Use it in unit tests
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySwitch: public Switch {
public:
    FriendlySwitch();
    virtual ~FriendlySwitch();
    friend class UT_Switch;
};

inline FriendlySwitch::FriendlySwitch() : Switch() {}
inline FriendlySwitch::~FriendlySwitch() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// object that actually runs the tests
////////////////////////////////////////////////////////////////////////////////////////////////////
class UT_Switch : public  CppUnit::TestFixture {
public:
    UT_Switch();
    virtual ~UT_Switch();

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
    CPPUNIT_TEST_SUITE(UT_Switch);
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

    SwitchConfigData* tConfigData;
    SwitchConfigData* tempConfigData;
    SwitchInputData* tInputData;
    SwitchInputData* tempInputData;

    FriendlySwitch* tArticle;

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

    UT_Switch(const UT_Switch &that);
    UT_Switch &operator = (const UT_Switch &that);

};
/// @}
#endif /* UT_SWITCH_EXISTS_ */
