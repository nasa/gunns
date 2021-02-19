#ifndef UtGunnsFluidConductor_EXISTS
#define UtGunnsFluidConductor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_CONDUCTOR    Gunns Fluid Conductor Unit Test
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

#include "core/GunnsFluidConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidConductor and befriend UtGunnsFluidConductor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidConductor : public GunnsFluidConductor
{
    public:
        FriendlyGunnsFluidConductor();
        virtual ~FriendlyGunnsFluidConductor();
        friend class UtGunnsFluidConductor;
};
inline FriendlyGunnsFluidConductor::FriendlyGunnsFluidConductor() : GunnsFluidConductor() {};
inline FriendlyGunnsFluidConductor::~FriendlyGunnsFluidConductor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Conductor  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidConductor: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidConductor(const UtGunnsFluidConductor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidConductor& operator =(const UtGunnsFluidConductor& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidConductor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testComputeFlowsWithInternalFluid);
        CPPUNIT_TEST(testTuning);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsFluidConductorConfigData* mConfigData;

        /// --     Pointer to nominal input data
        GunnsFluidConductorInputData* mInputData;

        /// --     Test Article
        FriendlyGunnsFluidConductor*  mArticle;

        /// --     Conductor Name
        std::string                  mLinkName;

        /// --     Link Max Conductivity
        double                       mMaxConductivity;

        /// --     Link Expansion Scale Factor
        double                       mExpansionScaleFactor;

        /// --     Network Nodes
        GunnsFluidNode               mNodes[3];

        /// --     Node List
        GunnsNodeList                mNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*> mLinks;

        /// --     Nominal inlet port index
        int                          mPort0;

        /// --     Nominal outlet port index
        int                          mPort1;

        /// (s)    Nominal time step
        double                       mTimeStep;

        /// --     Nominal tolerance for comparison of expected and returned values
        double                       mTolerance;

        /// --     Predefined fluid properties
        DefinedFluidProperties*      mFluidProperties;
        /// --     Fluid 1 config data
        PolyFluidConfigData*         mFluidConfig;
        /// --     Fluid 1 input data
        PolyFluidInputData*          mFluidInput1;
        /// --     Fluid 2input data
        PolyFluidInputData*          mFluidInput2;
        /// --     Fluid Fractions
        double*                      fractions;


    public:
        UtGunnsFluidConductor();
        virtual ~UtGunnsFluidConductor();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testStep();
        void testComputeFlows();
        void testComputeFlowsWithInternalFluid();
        void testTuning();
        void testAccessMethods();
};

///@}

#endif
