#ifndef UtGunnsFluidSimpleRocket_EXISTS
#define UtGunnsFluidSimpleRocket_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_SIMPLE_ROCKET   Simple Rocket Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Simple Rocket link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidSimpleRocket.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSimpleRocket and befriend UtGunnsFluidSimpleRocket.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSimpleRocket : public GunnsFluidSimpleRocket
{
    public:
        FriendlyGunnsFluidSimpleRocket();
        virtual ~FriendlyGunnsFluidSimpleRocket();
        friend class UtGunnsFluidSimpleRocket;
};
inline FriendlyGunnsFluidSimpleRocket::FriendlyGunnsFluidSimpleRocket() : GunnsFluidSimpleRocket() {};
inline FriendlyGunnsFluidSimpleRocket::~FriendlyGunnsFluidSimpleRocket() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Simple Rocket unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Simple Rocket link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSimpleRocket: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Simple Rocket unit test.
        UtGunnsFluidSimpleRocket();
        /// @brief    Default constructs this Simple Rocket unit test.
        virtual ~UtGunnsFluidSimpleRocket();
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
        /// @brief    Tests initialize method with a combustion model.
        void testCombustionInitialization();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests step method during combustion, no combustion model.
        void testStep();
        /// @brief    Tests moding of ignition state, non-hypergolic.
        void testStepIgnition();
        /// @brief    Tests moding of ignition state for hypergolic engine.
        void testStepIgnitionHypergolic();
        /// @brief    Tests step method during combustion with combustion model.
        void testStepCombustion();
        /// @brief    Tests compute flows method.
        void testComputeFlows();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSimpleRocket);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testCombustionInitialization);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testStepIgnition);
        CPPUNIT_TEST(testStepIgnitionHypergolic);
        CPPUNIT_TEST(testStepCombustion);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        /// @brief    Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 5, N_FLUIDS = 3};
        FluidProperties::FluidType        tTypes[N_FLUIDS];        /**< (--)  Constituent fluid types array. */
        double                            tFractions[N_FLUIDS];    /**< (--)  Constituent fluid mass fractions array. */
        double                            tFractions2[N_FLUIDS];   /**< (--)  Constituent fluid mass fractions array for node 2. */
        double                            tFractions3[N_FLUIDS];   /**< (--)  Constituent fluid mass fractions array for node 3. */
        DefinedFluidProperties*           tFluidProperties;        /**< (--)  Predefined fluid properties. */
        PolyFluidConfigData*              tFluidConfig;            /**< (--)  Fluid config data. */
        PolyFluidInputData*               tFluidInput0;            /**< (--)  Fluid input data for node 0. */
        PolyFluidInputData*               tFluidInput1;            /**< (--)  Fluid input data for node 1. */
        PolyFluidInputData*               tFluidInput2;            /**< (--)  Fluid input data for node 2. */
        PolyFluidInputData*               tFluidInput3;            /**< (--)  Fluid input data for node 3. */
        std::vector<GunnsBasicLink*>      tLinks;                  /**< (--)  Links vector. */
        std::string                       tName;                   /**< (--)  Nominal name. */
        GunnsFluidNode                    tNodes[N_NODES];         /**< (--)  Nominal connected nodes. */
        GunnsNodeList                     tNodeList;               /**< (--)  Network node structure. */
        int                               tPort0;                  /**< (--)  Nominal inlet port index. */
        int                               tPort1;                  /**< (--)  Nominal outlet port index. */
        double                            tThroatArea;             /**< (m2)  Nominal config data. */
        double                            tAreaRatio;              /**< (--)  Nominal config data. */
        double                            tCharacteristicVelocity; /**< (m/s) Nominal config data. */
        double                            tCombustionTemperature;  /**< (K)   Nominal config data. */
        double                            tCombustionGamma;        /**< (--)  Nominal config data. */
        double                            tThrustTuning;           /**< (--)  Nominal config data. */
        double                            tRecombinationTuning;    /**< (--)  Nominal config data. */
        bool                              tNeedsIgnition;          /**< (--)  Nominal config data. */
        CombustFactory::CombustorType     tCombustType;            /**< (--)  Nominal config data. */
        GunnsFluidSimpleRocketConfigData* tConfigData;             /**< (--)  Pointer to nominal configuration data. */
        bool                              tMalfBlockageFlag;       /**< (--)  Nominal input data. */
        double                            tMalfBlockageValue;      /**< (--)  Nominal input data. */
        bool                              tIgnition;               /**< (--)  Nominal input data. */
        bool                              tCombustion;             /**< (--)  Nominal input data. */
        GunnsFluidSimpleRocketInputData*  tInputData;              /**< (--)  Pointer to nominal input data */
        FriendlyGunnsFluidSimpleRocket*   tArticle;                /**< (--)  Pointer to the friendly class under test. */
        static int                        TEST_ID;                 /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSimpleRocket(const UtGunnsFluidSimpleRocket&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSimpleRocket& operator =(const UtGunnsFluidSimpleRocket&);
};

///@}

#endif
