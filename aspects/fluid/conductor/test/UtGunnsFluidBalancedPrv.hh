#ifndef UtGunnsFluidBalancedPrv_EXISTS
#define UtGunnsFluidBalancedPrv_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_BALANCED_PRV  Balanced Pressure Reducing Valve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Balanced Pressure Reducing Valve link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../GunnsFluidBalancedPrv.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidBalancedPrv and befriend UtGunnsFluidBalancedPrv.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidBalancedPrv : public GunnsFluidBalancedPrv
{
    public:
        FriendlyGunnsFluidBalancedPrv();
        virtual ~FriendlyGunnsFluidBalancedPrv();
        friend class UtGunnsFluidBalancedPrv;
};
inline FriendlyGunnsFluidBalancedPrv::FriendlyGunnsFluidBalancedPrv() : GunnsFluidBalancedPrv() {};
inline FriendlyGunnsFluidBalancedPrv::~FriendlyGunnsFluidBalancedPrv() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Balanced Pressure Reducing Valve unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Balanced Pressure Reducing
///           Valve link model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidBalancedPrv: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Balanced Pressure Reducing Valve unit test.
        UtGunnsFluidBalancedPrv();
        /// @brief    Default constructs this Balanced Pressure Reducing Valve unit test.
        virtual ~UtGunnsFluidBalancedPrv();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfig();
        /// @brief    Tests input data.
        void testInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests step method edge cases.
        void testStepEdgeCases();
        /// @brief    Tests compute flows method.
        void testComputeFlows();
        /// @brief    Tests compute flows method edge cases.
        void testComputeFlowsEdgeCases();
        /// @brief    Tests setter and getter methods.
        void testAccessors();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidBalancedPrv);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testStepEdgeCases);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testComputeFlowsEdgeCases);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        /// @brief    Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 5, N_FLUIDS = 2};
        FluidProperties::FluidType       tTypes[N_FLUIDS];       /**< (--)  Constituent fluid types array. */
        double                           tFractions0[N_FLUIDS];  /**< (--)  Constituent fluid mass fractions array. */
        double                           tFractions1[N_FLUIDS];  /**< (--)  Constituent fluid mass fractions array for node 2. */
        DefinedFluidProperties*          tFluidProperties;       /**< (--)  Predefined fluid properties. */
        PolyFluidConfigData*             tFluidConfig;           /**< (--)  Fluid config data. */
        PolyFluidInputData*              tFluidInput0;           /**< (--)  Fluid input data for node 0. */
        PolyFluidInputData*              tFluidInput1;           /**< (--)  Fluid input data for node 1. */
        std::vector<GunnsBasicLink*>     tLinks;                 /**< (--)  Links vector. */
        std::string                      tName;                  /**< (--)  Nominal name. */
        GunnsFluidNode                   tNodes[N_NODES];        /**< (--)  Nominal connected nodes. */
        GunnsNodeList                    tNodeList;              /**< (--)  Network node structure. */
        int                              tPort0;                 /**< (--)  Nominal inlet port index. */
        int                              tPort1;                 /**< (--)  Nominal outlet port index. */
        int                              tPort2;                 /**< (--)  Nominal reference pressure port index. */
        double                           tExitPressureDroop;     /**< (m2)  Nominal config data. */
        GunnsFluidBalancedPrvConfigData* tConfigData;            /**< (--)  Pointer to nominal configuration data. */
        bool                             tMalfBlockageFlag;      /**< (--)  Nominal input data. */
        double                           tMalfBlockageValue;     /**< (--)  Nominal input data. */
        bool                             tMalfPressureBiasFlag;  /**< (--)  Nominal input data. */
        double                           tMalfPressureBiasValue; /**< (kPa) Nominal input data. */
        double                           tPressureSetpoint;      /**< (kPa) Nominal input data. */
        GunnsFluidBalancedPrvInputData*  tInputData;             /**< (--)  Pointer to nominal input data */
        FriendlyGunnsFluidBalancedPrv*   tArticle;               /**< (--)  Pointer to the friendly class under test. */
        static int                       TEST_ID;                /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidBalancedPrv(const UtGunnsFluidBalancedPrv&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidBalancedPrv& operator =(const UtGunnsFluidBalancedPrv&);
};

///@}

#endif
