#ifndef UtGunnsResistiveLoad_EXISTS
#define UtGunnsResistiveLoad_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_ELECTRICAL_RESISTIVE_LOAD    Gunns Resistive Load Unit Test
/// @ingroup  UT_GUNNS_ELECTRICAL_RESISTIVE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Resistive Load
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include "aspects/electrical/resistive/GunnsResistiveLoad.hh"
#include "core/GunnsBasicNode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsResistiveLoad and befriend UtGunnsResistiveLoad.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsResistiveLoad : public GunnsResistiveLoad
{
    public:
        FriendlyGunnsResistiveLoad();
        virtual ~FriendlyGunnsResistiveLoad();
        friend class UtGunnsResistiveLoad;
};
inline FriendlyGunnsResistiveLoad::FriendlyGunnsResistiveLoad() : GunnsResistiveLoad() {};
inline FriendlyGunnsResistiveLoad::~FriendlyGunnsResistiveLoad() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Resistive Load  unit tests.
////
/// @details  This class provides the unit tests for the Gunns Resistive Load within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsResistiveLoad: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsResistiveLoad(const UtGunnsResistiveLoad& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsResistiveLoad& operator =(const UtGunnsResistiveLoad& that);

        CPPUNIT_TEST_SUITE(UtGunnsResistiveLoad);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testProcessOutputs);

        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsResistiveLoadConfigData* mConfigData;

        /// --     Pointer to nominal input data
        GunnsResistiveLoadInputData*  mInputData;

        /// --     Test Article
        FriendlyGunnsResistiveLoad*   mArticle;

        /// --     Resistor Name
        std::string                    mLinkName;

        /// --     Link Resistance
        double                         mResistance;

        /// --     Link Electrical Efficiency
        double                         mElectricalEfficiency;

        /// --     Resistive Load Under Voltage
        double                         mUnderVoltageLimit;

        /// --     Resistive Load Switch Resistance
        double                         mLoadSwitchResistance;

        /// --     Resistive Load Pos Trip Limit
        double                         mPosTripLimit;

        /// --     Resistive Load Neg Trip Limit
        double                         mNegTripLimit;

        /// --     Resistive Load Trip Priority
        int                            mTripPriority;

        /// --     Link Power Valid Status
        double                         mPowerValid;

        /// --     Link Blockage
        double                         mBlockage;

        /// --     Load switch initial position
        bool                           mInitialSwitchPosition;


        /// --     Network Nodes
        GunnsBasicNode                 mNodes[2];

        /// --     Node List
        GunnsNodeList                  mNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*>   mLinks;

        /// --     Nominal inlet port index
        int                            mPort0;

        /// --     Nominal outlet port index
        int                            mPort1;

        /// (s)    Nominal time step
        double                         mTimeStep;

        /// --     Nominal tolerance for comparison of expected and returned values
        double                         mTolerance;

    public:
        UtGunnsResistiveLoad();
        virtual ~UtGunnsResistiveLoad();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testUpdateState();
        void testProcessOutputs();
};

///@}

#endif
