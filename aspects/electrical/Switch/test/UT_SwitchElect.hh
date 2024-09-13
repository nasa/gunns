#ifndef UT_SWITCHELECT_EXISTS
#define UT_SWITCHELECT_EXISTS

/// @defgroup UT_SWITCH_ELECT EPS SwitchElect class Unit Test
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
     (aspects/electrical/Switch/SwitchElect.o)
)


PROGRAMMERS:
    (
     ((Philip Trottman) (USA) (March 2012) (TS21) (Initial Version))
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
#include "aspects/electrical/Switch/SwitchElect.hh"
#include "core/GunnsBasicLink.hh"
#include "core/GunnsBasicConductor.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// create a Friend class instance of the Switch class to access
/// all it's data.  Use it in unit tests
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlySwitchElect: public SwitchElect {
public:
    FriendlySwitchElect();
    virtual ~FriendlySwitchElect();
    friend class UT_SwitchElect;
};

inline FriendlySwitchElect::FriendlySwitchElect() : SwitchElect() {}
inline FriendlySwitchElect::~FriendlySwitchElect() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// object that actually runs the tests
////////////////////////////////////////////////////////////////////////////////////////////////////
class UT_SwitchElect : public  CppUnit::TestFixture {
public:
    enum epsNodeNames {
        testSource = 0,
        testDestination = 1,
        testNumNodes = 2
    };

    UT_SwitchElect();
    virtual ~UT_SwitchElect();

    // required by CPPUNIT:
    void setUp();

    void tearDown();

    // my tests
    void testDefaultConstruction();
    void testConfigData();
    void testInputData();
    void testUpdateState();
    void testUpdateFlux();
    void testSwitchIsTripped();
    void testSwitchIsClosed();
    void testSetSwitchCommandedClosed();
    void testSetSwitchTripReset();
    void testGetCurrent();
    void testGetVoltages();



private:
    CPPUNIT_TEST_SUITE(UT_SwitchElect);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testDefaultConstruction);
    CPPUNIT_TEST(testUpdateState);
    CPPUNIT_TEST(testUpdateFlux);
    CPPUNIT_TEST(testSwitchIsTripped);
    CPPUNIT_TEST(testSwitchIsClosed);
    CPPUNIT_TEST(testSetSwitchCommandedClosed);
    CPPUNIT_TEST(testSetSwitchTripReset);
    CPPUNIT_TEST(testGetCurrent);
    CPPUNIT_TEST(testGetVoltages);
    CPPUNIT_TEST_SUITE_END();

    SwitchElectConfigData* tConfigData;
    SwitchElectConfigData* tempConfigData;
    SwitchElectInputData* tInputData;
    SwitchElectInputData* tempInputData;

    FriendlySwitchElect* tArticle;

    std::vector<GunnsBasicLink*> tLinks;
    //std::vector<GunnsBasicConductor*> tLinks;
    GunnsNodeList tNodeList;
    GunnsBasicNode tNodes[testNumNodes];

    int* tPortMap;
    int tNumPorts;

    // config data
    std::string tName;
    double tSwitchOnResistance;
    bool tOverCurrentProtection;
    bool tUnderVoltProtection;
    double tMinVoltage;
    bool tIsTwoPortSwitch;
    int tPortAssigned;
    int tTripPriority;

    // input data
    bool tMalfBlockageFlag;
    double tMalfBlockageValue;
    bool tSwitchMalfFlag;
    int tSwitchMalf;
    bool tSwitchIsClosed;
    double tPosTripLimit;
    double tNegTripLimit;

    double tTolerance;

    UT_SwitchElect(const UT_SwitchElect &that);
    UT_SwitchElect &operator = (const UT_SwitchElect &that);

};

/// @}
#endif /* UT_SWITCHELECT_EXISTS */
