#ifndef UtGunnsFluidSource_EXISTS
#define UtGunnsFluidSource_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_SOURCE    Gunns Fluid Source Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Fluid Source
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSource and befriend UtGunnsFluidSource.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSource : public GunnsFluidSource
{
    public:
        FriendlyGunnsFluidSource();
        virtual ~FriendlyGunnsFluidSource();
        friend class UtGunnsFluidSource;
};
inline FriendlyGunnsFluidSource::FriendlyGunnsFluidSource() : GunnsFluidSource() {};
inline FriendlyGunnsFluidSource::~FriendlyGunnsFluidSource() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Basis Source  unit tests.
////
/// @details  This class provides the unit tests for the Base Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSource: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidSource(const UtGunnsFluidSource& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidSource& operator =(const UtGunnsFluidSource& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidSource);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessorsAndGetters);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testComputeFlowsWithInternalFluid);
        CPPUNIT_TEST_SUITE_END();

        /// --     Pointer to nominal configuration data
        GunnsFluidSourceConfigData*  mConfigData;

        /// --     Pointer to nominal configuration data
        GunnsFluidSourceInputData*   mInputData;

        /// --     Test Article
        FriendlyGunnsFluidSource*    mArticle;

        /// --     Source Name
        std::string                  mLinkName;

        /// --     Link Initial Flow Demand
        double                       mInitialFlowDemand;

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

    public:
        UtGunnsFluidSource();
        virtual ~UtGunnsFluidSource();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testAccessorsAndGetters();
        void testStep();
        void testComputeFlows();
        void testComputeFlowsWithInternalFluid();
};

///@}

#endif
