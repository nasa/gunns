#ifndef UtGunnsFluidAccumGas_exists
#define UtGunnsFluidAccumGas_exists

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_ACCUM_GAS    UtGunnsFluidAccumGas Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GunnsFluidAccum
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/capacitor/GunnsFluidAccumGas.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidAccum and befriend UtGunnsFluidAccumGas.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidAccumGas : public GunnsFluidAccumGas
{
    public:
        FriendlyGunnsFluidAccumGas();
        virtual ~FriendlyGunnsFluidAccumGas();
        friend class UtGunnsFluidAccumGas;
};
inline FriendlyGunnsFluidAccumGas::FriendlyGunnsFluidAccumGas()
    : GunnsFluidAccumGas() {};
inline FriendlyGunnsFluidAccumGas::~FriendlyGunnsFluidAccumGas() {}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid External Supply unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidAccum class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidAccumGas: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidAccumGas(const UtGunnsFluidAccumGas& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidAccumGas& operator =(const UtGunnsFluidAccumGas& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidAccumGas);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testLiquidCapacitance);
        CPPUNIT_TEST(testGasConductivityLimits);
        CPPUNIT_TEST(testComputeFlowsNomFlowIn);
        CPPUNIT_TEST(testComputeFlowsNomFlowOut);
        CPPUNIT_TEST(testTemperatureInFlow);
        CPPUNIT_TEST(testTemperatureOutFlow);
        CPPUNIT_TEST(testTemperatureThermal);
        CPPUNIT_TEST(testPressureEdit);
        CPPUNIT_TEST(testTempertureEdit);
        CPPUNIT_TEST(testMalfBellowsRupture);
        CPPUNIT_TEST(testHoldAccum);
        CPPUNIT_TEST(testPressurizerOrideBellows);
        CPPUNIT_TEST(testBellowsEdit);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();

        /// --     Predefined fluid properties
        DefinedFluidProperties*      tFluidProperties;

        PolyFluidConfigData*         tAccumFluidConfigData;

        PolyFluidInputData*          tLiquidFluidInputData;
        PolyFluidInputData*          tGasFluidInputData;

        /// --     Fluid Fractions
        double*                      tMassFractions;
        double*                      tGasMassFractions;

        /// --     Pointer to nominal configuration data
        GunnsFluidAccumGasConfigData* tConfigData;

        /// --     Pointer to nominal input data
        GunnsFluidAccumGasInputData*  tInputData;

        /// --     Test Article
        FriendlyGunnsFluidAccumGas*   tModel;

        /// --     Conductor Name
        std::string                  tLinkName;

        /// --     Link Max Conductivity
        double                       tMaxConductivity;
        double                       tMinConductivityScale;
        double                       tGasMaxConductivity;
        double                       tGasMinConductivity;

        /// --     Link Expansion Scale Factor
        double                       tExpansionScaleFactor;

        /// --     Link Expansion Scale Factor
        double                       tAccumVolume;

        /// --     Network Nodes
        GunnsFluidNode               tNodes[3];

        /// --     Node List
        GunnsNodeList                tNodeList;

        /// --     Network Links
        std::vector<GunnsBasicLink*> tLinks;

        /// --     Nominal inlet port index
        int                          tPort0;

        /// --     Nominal outlet port index
        int                          tPort1;

        /// (s)    Nominal time step
        double                       tTimeStep;

        /// --     Nominal tolerance for comparison of expected and returned values
        double                       tTolerance;

        /// --     Initial Bellows Position
        double                       tInitialBellowsPosition;
        double                       tInitialPressure;
        double                       tInitialTemperature;

        double tMinChamberVolPercent;
        double tMinChamberVolDeadBandPercent;
        double tForceBellowsMaxRate;
        double tEditHoldTime;
        double tMinTemperature;
        double tMaxTemperature;

        double tMaxPressure;
        double tMaxForcePressureRate;
        double tSpringCoeff0;
        double tSpringCoeff1;
        double tSpringCoeff2;
        double tFillModePressureThreshold;
        double tEffCondScaleRate;

        static int TEST_ID; /**< (--)  Test identification number. */

    public:
        UtGunnsFluidAccumGas();
        virtual ~UtGunnsFluidAccumGas();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testStep();
        void testLiquidCapacitance();
        void testGasConductivityLimits();
        void testComputeFlowsNomFlowIn();
        void testComputeFlowsNomFlowOut();
        void testTemperatureInFlow();
        void testTemperatureOutFlow();
        void testTemperatureThermal();
        void testPressureEdit();
        void testTempertureEdit();
        void testMalfBellowsRupture();
        void testHoldAccum();
        void testPressurizerOrideBellows();
        void testBellowsEdit();
        void testRestart();
};

///@}

#endif
