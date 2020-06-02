#ifndef UtGunnsFluidAccum_exists
#define UtGunnsFluidAccum_exists

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_ACCUM    GunnsFluidAccum Unit Test
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

#include "aspects/fluid/capacitor/GunnsFluidAccum.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidAccum and befriend UtGunnsFluidAccum.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidAccum : public GunnsFluidAccum
{
    public:
        FriendlyGunnsFluidAccum();
        virtual ~FriendlyGunnsFluidAccum();
        friend class UtGunnsFluidAccum;
        friend class UtGunnsFluidEvaporation;
};
inline FriendlyGunnsFluidAccum::FriendlyGunnsFluidAccum()
    : GunnsFluidAccum() {};
inline FriendlyGunnsFluidAccum::~FriendlyGunnsFluidAccum() {}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid External Supply unit tests.
////
/// @details  This class provides the unit tests for the GunnsFluidAccum class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidAccum: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidAccum(const UtGunnsFluidAccum& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidAccum& operator =(const UtGunnsFluidAccum& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluidAccum);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testComputeFlowsNomFlowIn);
        CPPUNIT_TEST(testComputeFlowsNomFlowOut);
        CPPUNIT_TEST(testEffConductivityFull);
        CPPUNIT_TEST(testEffConductivityEmpty);
        CPPUNIT_TEST(testEffConductivityNom);
        CPPUNIT_TEST(testEffConductivityDeadBand);
        CPPUNIT_TEST(testEffCondDeadBandWithFillModeThresh);
        CPPUNIT_TEST(testEffConductivityStuckBellows);
        CPPUNIT_TEST(testEffConductivityBadVolumes);
        CPPUNIT_TEST(testComputeConductivity);
        CPPUNIT_TEST(testTemperatureInFlow);
        CPPUNIT_TEST(testTemperatureOutFlow);
        CPPUNIT_TEST(testTemperatureThermal);
        CPPUNIT_TEST(testBellowsEdit);
        CPPUNIT_TEST(testTemperatureEdit);
        CPPUNIT_TEST(testMalfStickBellows);
        CPPUNIT_TEST(testMalfStickBellowsToPos);
        CPPUNIT_TEST(testPressurizerBellowsOride);
        CPPUNIT_TEST(testHoldFlags);
        CPPUNIT_TEST(testCheckSpecificPortRules);
        CPPUNIT_TEST(testCheckSpecificPortRulesGasPort0);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST_SUITE_END();

        /// --     Predefined fluid properties
        DefinedFluidProperties*      tFluidProperties;

        PolyFluidConfigData*         tLiquidFluidConfigData;

        PolyFluidInputData*          tLiquidFluidInputData;

        /// --     Fluid Fractions
        double*                      tMassFractions;

        /// --     Pointer to nominal configuration data
        GunnsFluidAccumConfigData* tConfigData;

        /// --     Pointer to nominal input data
        GunnsFluidAccumInputData*  tInputData;

        /// --     Test Article
        FriendlyGunnsFluidAccum*   tModel;

        /// --     Conductor Name
        std::string                  tLinkName;

        /// --     Link Max Conductivity
        double                       tMaxConductivity;
        double                       tMinConductivityScale;

        /// --     Link Expansion Scale Factor
        double                       tExpansionScaleFactor;

        /// --     Link Expansion Scale Factor
        double                       tAccumVolume;

        /// --     Network Nodes
        GunnsFluidNode               tNodes[2];

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

        double tMinChamberVolPercent;
        double tMinChamberVolDeadBandPercent;
        double tForceBellowsMaxRate;
        double tEditHoldTime;
        double tMinTemperature;
        double tMaxTemperature;

        double tMaxPressure;
        double tSpringCoeff0;
        double tSpringCoeff1;
        double tSpringCoeff2;
        double tFillModePressureThreshold;
        double tEffCondScaleRate;
        static int TEST_ID;  /**< (--)  Test identification number. */

    public:
        UtGunnsFluidAccum();
        virtual ~UtGunnsFluidAccum();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testStep();
        void testComputeFlowsNomFlowIn();
        void testComputeFlowsNomFlowOut();
        void testEffConductivityFull();
        void testEffConductivityEmpty();
        void testEffConductivityNom();
        void testEffConductivityDeadBand();
        void testEffCondDeadBandWithFillModeThresh();
        void testEffConductivityStuckBellows();
        void testEffConductivityBadVolumes();
        void testComputeConductivity();
        void testTemperatureInFlow();
        void testTemperatureOutFlow();
        void testTemperatureThermal();
        void testBellowsEdit();
        void testTemperatureEdit();
        void testMalfStickBellows();
        void testMalfStickBellowsToPos();
        void testPressurizerBellowsOride();
        void testHoldFlags();
        void testCheckSpecificPortRules();
        void testCheckSpecificPortRulesGasPort0();
        void testAccessMethods();
        void testRestart();
};

///@}

#endif
