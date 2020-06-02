#ifndef UtGunnsFluidSublimator_EXISTS
#define UtGunnsFluidSublimator_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_SUBLIMATOR   Sublimator Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Sublimator link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidSublimator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSublimator and befriend UtGunnsFluidSublimator.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSublimator : public GunnsFluidSublimator
{
    public:
        FriendlyGunnsFluidSublimator();
        virtual ~FriendlyGunnsFluidSublimator();
        friend class UtGunnsFluidSublimator;
};
inline FriendlyGunnsFluidSublimator::FriendlyGunnsFluidSublimator() : GunnsFluidSublimator() {};
inline FriendlyGunnsFluidSublimator::~FriendlyGunnsFluidSublimator() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sublimator unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Sublimator link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSublimator: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Sublimator unit test.
        UtGunnsFluidSublimator();
        /// @brief    Default constructs this Sublimator unit test.
        virtual ~UtGunnsFluidSublimator();
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
        /// @brief    Tests step method (nominal).
        void testStepNominal();
        /// @brief    Tests step method (no feed).
        void testStepNoFeed();
        /// @brief    Tests step method (no vent).
        void testStepNoVent();
        /// @brief    Tests computeFlows method.
        void testComputeFlows();
        /// @brief    Tests specific port mapping rules.
        void testPortMapping();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests the access methods.
        void testAccessMethods();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSublimator);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testStepNominal);
        CPPUNIT_TEST(testStepNoFeed);
        CPPUNIT_TEST(testStepNoVent);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST_SUITE_END();
        /// @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 4, N_FLUIDS = 3};
        /// @brief    --     Constituent fluid types array.
        FluidProperties::FluidType           tTypes[N_FLUIDS];
        /// @brief    --     Constituent fluid mass fractions array.
        double                               tFractionsFeed[N_FLUIDS];
        /// @brief    --     Constituent fluid mass fractions array.
        double                               tFractionsVent[N_FLUIDS];
        /// @brief    --     Predefined fluid properties.
        DefinedFluidProperties*              tFluidProperties;
        /// @brief    --     Fluid config data.
        PolyFluidConfigData*                 tFluidConfig;
        /// @brief    --     Fluid input data for node 0.
        PolyFluidInputData*                  tFluidInput0;
        /// @brief    --     Fluid input data for node 1.
        PolyFluidInputData*                  tFluidInput1;
        /// @brief    --     Link vector.
        std::vector<GunnsBasicLink*>         tLinks;
        /// @brief    --     Nominal name.
        std::string                          tName;
        /// @brief    --     Nominal connected nodes.
        GunnsFluidNode                       tNodes[N_NODES];
        /// @brief    --     Network node structure.
        GunnsNodeList                        tNodeList;
        /// @brief    --     Nominal inlet port index.
        int                                  tPort0;
        /// @brief    --     Nominal outlet port index.
        int                                  tPort1;
        /// @brief    --     Gas fluid type of constituent being adsorbed.
        FluidProperties::FluidType           tGasType;
        /// @brief   (kJ/kg) Heat of vaporization of the operating fluid
        double                               tHeatOfVaporization;
        /// @brief   (kJ/kg) Heat of fusion (melting) of the operating fluid
        double                               tHeatOfFusion;
        /// @brief   (K)     Temperature of the operating fluid triple point
        double                               tTriplePointTemperature;
        /// @brief   (kg)    Maximum mass of ice the sublimator can form
        double                               tMaxIceMass;
        /// @brief   (--)    (0-1) fraction of maximum ice mass for complete plate coverage
        double                               tIceCoverageFraction;
        /// @brief   (J/s/K) Maximum conductivity of the GunnsThermalPotential
        double                               tMaxThermalConductivity;
        /// @brief   (m2)    Effective conductivity of plate with no ice coverage
        double                               tPlateConductivity;
        /// @brief   (--)    Thermal conductivity filter gain
        double                               tThermalConductivityGain;
        /// @brief    --     Pointer to nominal configuration data.
        GunnsFluidSublimatorConfigData*      tConfigData;
        /// @brief    (kg)   Initial mass of ice in the sublimator
        double                               tIceMass;
        /// @brief    (K)    Initial temperature of the sublimator structure
        double                               tStructureTemperature;
        /// @brief    --     Pointer to nominal input data.
        GunnsFluidSublimatorInputData*       tInputData;
        /// @brief    --     Pointer to the friendly Sublimator under test.
        FriendlyGunnsFluidSublimator*        tArticle;
        /// @brief    (s)    Nominal time step.
        double                               tTimeStep;
        static int                           TEST_ID; /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSublimator(const UtGunnsFluidSublimator&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSublimator& operator =(const UtGunnsFluidSublimator&);
};

///@}

#endif
