#ifndef UtGunnsBasicCapacitor_EXISTS
#define UtGunnsBasicCapacitor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_BASIC_SOURCE    Gunns Fluid Conductor Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Conductor
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsBasicCapacitor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicCapacitor and befriend UtGunnsBasicCapacitor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicCapacitor : public GunnsBasicCapacitor
{
    public:
        FriendlyGunnsBasicCapacitor();
        virtual ~FriendlyGunnsBasicCapacitor();
        friend class UtGunnsBasicCapacitor;
};
inline FriendlyGunnsBasicCapacitor::FriendlyGunnsBasicCapacitor() : GunnsBasicCapacitor() {};
inline FriendlyGunnsBasicCapacitor::~FriendlyGunnsBasicCapacitor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Conductor  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicCapacitor: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicCapacitor(const UtGunnsBasicCapacitor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicCapacitor& operator =(const UtGunnsBasicCapacitor& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicCapacitor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testZeroDtStep);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testModifierMethods);

        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsBasicCapacitorConfigData* mConfigData;

        /// --     Pointer to nominal input data
        GunnsBasicCapacitorInputData*  mInputData;

        /// --     Test Article
        FriendlyGunnsBasicCapacitor*   mArticle;

        /// --     Link Name
        std::string                    mName;

        /// --     The initial capacitance of the capacitor.
        double                         mInitialCapacitance;

        /// --      The initial potential value of the capacitor.
        double                         mInitialPotential;

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
        UtGunnsBasicCapacitor();
        virtual ~UtGunnsBasicCapacitor();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testStep();
        void testComputeFlows();
        void testZeroDtStep();
        void testAccessMethods();
        void testModifierMethods();
};

///@}

#endif
