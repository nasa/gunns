#ifndef UtGunnsBasicConductor_EXISTS
#define UtGunnsBasicConductor_EXISTS

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

#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicConductor and befriend UtGunnsBasicConductor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicConductor : public GunnsBasicConductor
{
    public:
        FriendlyGunnsBasicConductor();
        virtual ~FriendlyGunnsBasicConductor();
        friend class UtGunnsBasicConductor;
};
inline FriendlyGunnsBasicConductor::FriendlyGunnsBasicConductor() : GunnsBasicConductor() {};
inline FriendlyGunnsBasicConductor::~FriendlyGunnsBasicConductor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Conductor  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicConductor: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsBasicConductor(const UtGunnsBasicConductor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsBasicConductor& operator =(const UtGunnsBasicConductor& that);

        CPPUNIT_TEST_SUITE(UtGunnsBasicConductor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testAccessMethods);

        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsBasicConductorConfigData* mConfigData;

        /// --     Pointer to nominal input data
        GunnsBasicConductorInputData*  mInputData;

        /// --     Test Article
        FriendlyGunnsBasicConductor*   mArticle;

        /// --     Conductor Name
        std::string                    mLinkName;

        /// --     Link Conductivity
        double                         mInitialConductivity;

        /// --     Link Blockage
        double                         mBlockage;

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
        UtGunnsBasicConductor();
        virtual ~UtGunnsBasicConductor();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testStep();
        void testComputeFlows();
        void testAccessMethods();
};

///@}

#endif
