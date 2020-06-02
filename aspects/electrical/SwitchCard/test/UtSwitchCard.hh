#ifndef UtSwitchCard_EXISTS
#define UtSwitchCard_EXISTS

/// @defgroup UT_SWITCH_CARD_ELECT EPS SwitchCardElect class Unit Test
/// @ingroup  UT_GUNNS
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @{

/*!******************************** TRICK HEADER ***************************

@details
PURPOSE:
(
(Philip Trottman: This is the unit test class for the SwitchCard model)
(which is used to model the ISS RPCM, DCSU, and MBSU devices)
)

REFERENCE:
((list here, if any))

ASSUMPTIONS AND LIMITATIONS:
((list here, if any))
 *
LIBRARY DEPENDENCY:
(
     (software/exceptions/TsInitializationException.o)
     (aspects/electrical/Switch/SwitchCardElect.o)
     (core/GunnsBasicLink.o)
     (aspects/electrical/UserLoad/UserLoadBase.o)
     (aspects/electrical/UserLoad/ResistiveLoad.o)
     (aspects/electrical/UserLoad/ConstantPowerLoad.o)
     (common/sensors/SensorAnalog.o)
)

PROGRAMMERS:
    (
     ((Philip Trottman) (USA) (January 26, 2012) (TS21) (Initial Version))
    )

 ***************************************************************************/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <string>
#include "aspects/electrical/SwitchCard/SwitchCardElect.hh"
#include "core/GunnsBasicLink.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "aspects/electrical/UserLoad/ResistiveLoad.hh"
#include "aspects/electrical/UserLoad/ConstantPowerLoad.hh"
#include "aspects/electrical/IPS/GunnsElectIps.hh"
#include "common/sensors/SensorAnalog.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Create class to contain the test article loads.
///////////////////////////////////////////////////////////////////////////////////////////////////
class EpsTestLoads {
public:
    TS_MAKE_SIM_COMPATIBLE(EpsTestLoads);

    EpsTestLoads();
    virtual ~EpsTestLoads();

private:
    EpsTestLoads(const EpsTestLoads&);
    EpsTestLoads &operator = (const EpsTestLoads&);

public:
    //ResistiveLoad testRpcmSwitch1;
    ConstantPowerLoad testRpcmSwitch2;
    //ResistiveLoad testRpcmSwitch3;
    ResistiveLoad testRpcmSwitch4;

};

class EpsTestLoadsConfig {
    EpsTestLoadsConfig(const EpsTestLoadsConfig&);
    EpsTestLoadsConfig& operator =(const EpsTestLoadsConfig&);

public:
    TS_MAKE_SIM_COMPATIBLE(EpsTestLoadsConfig);

    EpsTestLoadsConfig();
    virtual ~EpsTestLoadsConfig();

    ConstantPowerLoadConfigData testRpcmSwitch2Config;
    ResistiveLoadConfigData testRpcmSwitch4Config;
};

class EpsTestLoadsInput {
    EpsTestLoadsInput(const EpsTestLoadsInput&);
    EpsTestLoadsInput& operator =(const EpsTestLoadsInput&);

public:
    TS_MAKE_SIM_COMPATIBLE(EpsTestLoadsInput);

    EpsTestLoadsInput();
    virtual ~EpsTestLoadsInput();

    ConstantPowerLoadInputData testRpcmSwitch2Input;
    ResistiveLoadInputData testRpcmSwitch4Input;
};

class FriendlySwitchCard : public SwitchCardElect {
public:
    FriendlySwitchCard();
    virtual ~FriendlySwitchCard();

    friend class UtSwitchCard;
};

inline FriendlySwitchCard::FriendlySwitchCard() : SwitchCardElect() {}
inline FriendlySwitchCard::~FriendlySwitchCard() {}

class UtSwitchCard : public CppUnit::TestFixture {
    UtSwitchCard(const UtSwitchCard&);
    UtSwitchCard& operator =(const UtSwitchCard&);

    CPPUNIT_TEST_SUITE(UtSwitchCard);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testInitialization);
    CPPUNIT_TEST(testNoSwitchesClosed);
    CPPUNIT_TEST(testCloseSwitchOne);
    CPPUNIT_TEST(testCloseSwitchTwo);
    CPPUNIT_TEST(testCloseSwitchThree);
    CPPUNIT_TEST(testCloseSwitchFour);
    CPPUNIT_TEST(testCloseAllSwitches);
    CPPUNIT_TEST(testOpenTwoPortSwitch);
    CPPUNIT_TEST(testOpenLoadSwitch);
    CPPUNIT_TEST(testTripTwoPortSwitch);
    CPPUNIT_TEST(testTripLoadSwitch);
    CPPUNIT_TEST(testLossOfInputVolts);
    CPPUNIT_TEST(testTripPriority);
    CPPUNIT_TEST(testPowerSupplyFail);
    CPPUNIT_TEST(testMagicPower);
    CPPUNIT_TEST(testBadInitData);
    CPPUNIT_TEST(testInputUndervolt);
    CPPUNIT_TEST(testInputOvervolt);
    CPPUNIT_TEST(testClearUndervoltTrip);
//    CPPUNIT_TEST(testIps);
    CPPUNIT_TEST_SUITE_END();

public:
    enum epsNodeNames {
        testSource = 0,
        testRpcm1 = 1,
        testRpcm2 = 2,
        testGround = 3,
        testNumNodes = 4
    };

    UtSwitchCard();
    virtual ~UtSwitchCard();

    // needed for CPPUNIT
    void setUp();
    void tearDown();

    // test procedures
    void testConfigData();
    void testInputData();
    void testDefaultConstruction();
    void testInitialization();
    void testNoSwitchesClosed();
    void testCloseSwitchOne();
    void testCloseSwitchTwo();
    void testCloseSwitchThree();
    void testCloseSwitchFour();
    void testCloseAllSwitches();
    void testOpenTwoPortSwitch();
    void testOpenLoadSwitch();
    void testTripTwoPortSwitch();
    void testTripLoadSwitch();
    void testLossOfInputVolts();
    void testTripPriority();
    void testBadInitData();
    void testPowerSupplyFail();
    void testMagicPower();
    void testInputUndervolt();
    void testInputOvervolt();
    void testClearUndervoltTrip();
    //void testIps();

    void stepTheModel();

    FriendlySwitchCard* tArticle;
    SwitchCardElectConfigData* tConfigData;
    SwitchCardElectInputData* tInputData;

    EpsTestLoads tLoads;
    EpsTestLoadsConfig tLoadsConfig;
    EpsTestLoadsInput tLoadsInput;
    std::vector<UserLoadBase*> tNetworkLoads;
    UserLoadBase * tCardLoads[1][4];

    //GunnsElectIps tPowerSupply;

    std::vector<GunnsBasicLink*> tLinks;
    GunnsNodeList tNodeList;
    GunnsBasicNode tNodes[testNumNodes];

    int* tPortMap;
    int tNumPorts;

    // class variable go here
    // config data
    std::string tName;
    int tNumSwitches;
    int tNumTwoPortSwitches;
    double tPowerSupplyLoad;
    SensorAnalogConfigData tInputVoltSensorConfigData;
    SensorAnalogConfigData tInputCurrentSensorConfigData;
    SensorAnalogConfigData tSwitchVoltSensorConfigData;
    SensorAnalogConfigData tSwitchCurrentSensorConfigData;

    //input data
    bool tMalfBlockageFlag;
    double tMalfBlockageValue;
    double tSwitchPosTripLimit;
    double tSwitchNegTripLimit;
    double tMinInputVoltage;
    double tMaxConductance;
    SensorAnalogInputData tInputVoltSensorInputData;
    SensorAnalogInputData tInputCurrentSensorInputData;
    SensorAnalogInputData tSwitchVoltSensorInputData;
    SensorAnalogInputData tSwitchCurrentSensorInputData;

    // test tolerance for floats/doubles
    double tTolerance;

    double tTimeStep;
};

/// @}
#endif /*UtSwitchCard_EXISTS*/
