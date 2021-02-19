#ifndef UtGunnsFluidBalloon_EXISTS
#define UtGunnsFluidBalloon_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CAPACITOR_BALLOON    Balloon Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CAPACITOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Balloon link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/capacitor/GunnsFluidBalloon.hh"
#include "UtGunnsFluidTank.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidBalloon and befriend UtGunnsFluidBalloon.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidBalloon : public GunnsFluidBalloon
{
    public:
        FriendlyGunnsFluidBalloon();
        virtual ~FriendlyGunnsFluidBalloon();
        friend class UtGunnsFluidBalloon;
};
inline FriendlyGunnsFluidBalloon::FriendlyGunnsFluidBalloon() : GunnsFluidBalloon() {};
inline FriendlyGunnsFluidBalloon::~FriendlyGunnsFluidBalloon() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Balloon unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Balloon link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidBalloon: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Quick Disconnect Hose unit test.
        UtGunnsFluidBalloon();
        /// @brief    Default destructs this Quick Disconnect Hose unit test.
        virtual ~UtGunnsFluidBalloon();
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
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests nominal step method.
        void testStep();
        /// @brief    Tests nominal updateFluid method.
        void testUpdateFluid();
        /// @brief    Tests edit of balloon volume.
        void testEditVolume();
        /// @brief    Tests edit of temperature and total pressure.
        void testEditTemperatureAndPressures();
        /// @brief    Tests edit of partial pressure rates.
        void testEditPartialPressureRate();
        /// @brief    Tests the computePressureCorrection method.
        void testPressureCorrectionPos();
        /// @brief    Tests the computePressureCorrection method.
        void testPressureCorrectionNeg();
        /// @brief    Tests the checkSpecificPortRules method.
        void testSpecificPortRules();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidBalloon);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testEditVolume);
        CPPUNIT_TEST(testEditTemperatureAndPressures);
        CPPUNIT_TEST(testEditPartialPressureRate);
        CPPUNIT_TEST(testPressureCorrectionPos);
        CPPUNIT_TEST(testPressureCorrectionNeg);
        CPPUNIT_TEST(testSpecificPortRules);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 4, N_FLUIDS = 3};
        FluidProperties::FluidType   tTypes[N_FLUIDS];      /**< (--)     Constituent fluid types array. */
        DefinedFluidProperties*      tFluidProperties;      /**< (--)     Predefined fluid properties. */
        PolyFluidConfigData*         tFluidConfig;          /**< (--)     Fluid config data. */
        double                       tFractGas[N_FLUIDS];   /**< (--)     Gas constituent mass fractions. */
        double                       tFractLiq[N_FLUIDS];   /**< (--)     Liquid constituent mass fractions. */
        PolyFluidInputData*          tFluidInputGas;        /**< (--)     Fluid input data for gas nodes. */
        PolyFluidInputData*          tFluidInputLiq;        /**< (--)     Fluid input data for liquid nodes. */
        std::vector<GunnsBasicLink*> tLinks;                /**< (--)     Link vector. */
        std::string                  tName;                 /**< (--)     Nominal name. */
        FriendlyGunnsFluidNode       tNodes[N_NODES];       /**< (--)     Nominal connected nodes. */
        GunnsNodeList                tNodeList;             /**< (--)     Network node structure. */
        int                          tPort0;                /**< (--)     Nominal inlet port index. */
        int                          tPort1;                /**< (--)     Nominal outlet port index. */
        double                       tExpansionScaleFactor; /**< (--)     Nominal scale factor for isentropic gas cooling. */
        double                       tDpdtFilterGain;       /**< (--)     Nominal dP/dt filter gain. */
        double                       tThermalDampingMass;   /**< (--)     Nominal thermal damping mass. */
        double                       tEditFluxTarget;       /**< (--)     Nominal edit flux target tolerance. */
        float                        tSurfaceArea;          /**< (K)      Balloon shell surface area. */
        float                        tShellRadius;          /**< (m)      Balloon shell distance to center. */
        double                       tInflatability;        /**< (m3/kPa) Nominal inflatability. */
        double                       tMaxVolume;            /**< (m3)     Nominal maximum volume. */
        double                       tShellTemperature;     /**< (--)     Nominal configuration for owning shell heat flux. */
        float                        tBiasHeatFlux;         /**< (W)      User bias for extra heat flux to the fluid. */
        GunnsFluidBalloonConfigData* tConfigData;           /**< (--)     Pointer to nominal configuration data. */
        GunnsFluidBalloonInputData*  tInputData;            /**< (--)     Pointer to nominal input data. */
        FriendlyGunnsFluidBalloon*   tArticle;              /**< (--)     Pointer to the friendly Balloon under test. */
        double                       tTimeStep;             /**< (s)      Nominal time step. */
        static int                   TEST_ID;               /**< (--)     Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidBalloon(const UtGunnsFluidBalloon&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidBalloon& operator =(const UtGunnsFluidBalloon&);
};

///@}

#endif
