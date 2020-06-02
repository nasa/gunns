#ifndef UtGunnsFluidCapacitor_EXISTS
#define UtGunnsFluidCapacitor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_CAPACITOR    Gunns Fluid Capacitor Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Capacitor
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidCapacitor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidCapacitor and befriend UtGunnsFluidCapacitor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidCapacitor : public GunnsFluidCapacitor
{
    public:
        FriendlyGunnsFluidCapacitor();
        virtual ~FriendlyGunnsFluidCapacitor();
        friend class UtGunnsFluidCapacitor;
        friend class UtGunnsFluidDistributedIf;
};
inline FriendlyGunnsFluidCapacitor::FriendlyGunnsFluidCapacitor() : GunnsFluidCapacitor() {};
inline FriendlyGunnsFluidCapacitor::~FriendlyGunnsFluidCapacitor() {}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Node class - inherits from GunnsFluidNode and befriends UtGunnsFluidCapacitor.
///
/// @details  Fluid node that befriends the unit test. This is so attributes can be modified
///           directly in order to verify accessor functionality.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidNodeCapUt : public GunnsFluidNode
{
    public:
        FriendlyGunnsFluidNodeCapUt();
        virtual ~FriendlyGunnsFluidNodeCapUt();
        friend class UtGunnsFluidCapacitor;
};
inline FriendlyGunnsFluidNodeCapUt::FriendlyGunnsFluidNodeCapUt() : GunnsFluidNode() {};
inline FriendlyGunnsFluidNodeCapUt::~FriendlyGunnsFluidNodeCapUt() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Capacitor  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidCapacitor: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidCapacitor(const UtGunnsFluidCapacitor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidCapacitor& operator =(const UtGunnsFluidCapacitor& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidCapacitor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessorsAndSetters);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testZeroDtStep);
        CPPUNIT_TEST(testPortRules);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsFluidCapacitorConfigData* mConfigData;

        /// --     Pointer to nominal input data
        GunnsFluidCapacitorInputData*  mInputData;

        /// --     Test Article
        FriendlyGunnsFluidCapacitor*   mArticle;

        /// --     Capacitor Name
        std::string                    mLinkName;

        /// --     Link Initial Volume
        double                         mInitialVolume;

        /// --     Link Expansion Scale Factor
        double                         mExpansionScaleFactor;

        /// --     Network Nodes
        FriendlyGunnsFluidNodeCapUt    mNodes[2];

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

        /// --     Predefined fluid properties
        DefinedFluidProperties*        mFluidProperties;
        /// --     Fluid 1 config data
        PolyFluidConfigData*           mFluidConfig;
        /// --     Fluid 1 input data
        PolyFluidInputData*            mFluidInput1;
        /// --     Fluid 2input data
        PolyFluidInputData*            mFluidInput2;
        /// --     Fluid Fractions
        double*                        mFractions;

    public:
        UtGunnsFluidCapacitor();
        virtual ~UtGunnsFluidCapacitor();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testAccessorsAndSetters();
        void testStep();
        void testComputeFlows();
        void testZeroDtStep();
        void testPortRules();
        void testRestart();
};

///@}

#endif
