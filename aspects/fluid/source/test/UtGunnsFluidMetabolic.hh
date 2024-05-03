#ifndef UtGunnsFluidMetabolic_EXISTS
#define UtGunnsFluidMetabolic_EXISTS


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_METABOLIC    Metabolic Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Metabolic link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidMetabolic.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidMetabolic and befriend UtGunnsFluidMetabolic.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidMetabolic : public GunnsFluidMetabolic {
    public:
        FriendlyGunnsFluidMetabolic();
        virtual ~FriendlyGunnsFluidMetabolic();
        friend class UtGunnsFluidMetabolic;
};
inline FriendlyGunnsFluidMetabolic::FriendlyGunnsFluidMetabolic() : GunnsFluidMetabolic() {}
inline FriendlyGunnsFluidMetabolic::~FriendlyGunnsFluidMetabolic() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Metabolic unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Metabolic link model within
///           the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidMetabolic: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Metabolic unit test.
        UtGunnsFluidMetabolic();
        /// @brief    Default destructs this Metabolic unit test.
        virtual ~UtGunnsFluidMetabolic();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests multiple initialization.
        void testMultipleInitialization();
        /// @brief    Tests accessor methods.
        void testAccess();
        /// @brief    Tests modifier methods.
        void testModify();
        /// @brief    Tests update fluid method.
        void testUpdateFluid();
        /// @brief    Tests specific port mapping rules.
        void testPortMapping();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
   private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidMetabolic);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testMultipleInitialization);
        CPPUNIT_TEST(testAccess);
        CPPUNIT_TEST(testModify);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        enum { N_CONSTITUENTS = 8 };
        /// @brief    --       Defined fluid properties
        DefinedFluidProperties*        mFluidProperties;
        /// @brief    --       Array of Fluid Types
        FluidProperties::FluidType     mTypes[UtGunnsFluidMetabolic::N_CONSTITUENTS];
        /// @brief    --       Array of fluid mass fractions
        double                         mFractions[UtGunnsFluidMetabolic::N_CONSTITUENTS];
        /// @brief    --       Fluid config data
        PolyFluidConfigData*           mFluidConfig;
        /// @brief    --       Fluid input data
        PolyFluidInputData*            mFluidInput;
        /// @brief    --       Link vector
        std::vector<GunnsBasicLink*>   mLinks;
        /// @brief    --       Nominal name
        std::string                    mName;
        /// @brief    --       Fluid nodes
        GunnsFluidNode                 mNodes[3];
        /// @brief    --       Node List
        GunnsNodeList                  mNodeList;
        /// @brief    --       Nominal input atmosphere
        PolyFluid*                     mInput;
        /// @brief    --       Nominal output atmosphere
        PolyFluid*                     mOutput;
        /// @brief    (kg/s)   Initial flow demand
        double                         mInitialFlowDemand;
        /// @brief    (kg/s)   Nominal O2 consumption rate for a crew member
        double                         mConsumptionRateO2;
        /// @brief    (kg/s)   Nominal CO2 production rate for a crew member
        double                         mProductionRateCO2;
        /// @brief    (kg/s)   Nominal CO2 production rate for a crew member
        double                         mProductionRateH2O;
        /// @brief    (TBD/s)  Nominal heat production rate for a crew member
        double                         mProductionRateHeat;
        /// @brief    --       Nominal sleep production/consumption scale factor
        double                         mSleepScaleFactor;
        /// @brief    --       Nominal recovery production/consumption scale factor
        double                         mRecoveryScaleFactor;
        /// @brief    --       Nominal exercise production/consumption scale factor
        double                         mExerciseScaleFactor;
        /// @brief    --       Nominal Gunns Metabolic configuration data
        GunnsFluidMetabolicConfigData* mConfigData;
        /// @brief    --       Nominal number of crew members in nominal state
        double                         mNNominal;
        /// @brief    --       Nominal number of crew members in sleep state
        double                         mNSleep;
        /// @brief    --       Nominal number of crew members in recovery state
        double                         mNRecovery;
        /// @brief    --       Nominal number of crew members in exercise state
        double                         mNExercise;
        /// @brief    --       Nominal Gunns Metabolic input data
        GunnsFluidMetabolicInputData*  mInputData;
        /// @brief    --       Nominal Gunns Metabolic
        FriendlyGunnsFluidMetabolic*   mArticle;
        /// @brief    (kg/s)   Nominal mass flow rate
        double                         mFlowRate;
        /// @brief    (s)      Nominal time step
        double                         mTimeStep;
        /// @brief    --       Nominal tolerance for comparison of expected and returned values
        double                         mTolerance;
        static int                     TEST_ID;  /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidMetabolic(const UtGunnsFluidMetabolic&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidMetabolic& operator =(const UtGunnsFluidMetabolic&);
};

///@}

#endif
