#ifndef UtGunnsFluidTank_EXISTS
#define UtGunnsFluidTank_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CAPACITOR_TANK    Tank Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CAPACITOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @todo - complete unit tests
///
/// @details  Unit Tests for the GUNNS Fluid Tank link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/capacitor/GunnsFluidTank.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidTank and befriend UtGunnsFluidTank.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidTank : public GunnsFluidTank
{
    public:
        FriendlyGunnsFluidTank();
        virtual ~FriendlyGunnsFluidTank();
        friend class UtGunnsFluidTank;
};
inline FriendlyGunnsFluidTank::FriendlyGunnsFluidTank() : GunnsFluidTank() {};
inline FriendlyGunnsFluidTank::~FriendlyGunnsFluidTank() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidNode and befriend UtGunnsFluidTank.
///
/// @details  Class derived from an object used in the test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidNode : public GunnsFluidNode
{
    public:
        FriendlyGunnsFluidNode();
        virtual ~FriendlyGunnsFluidNode();
        friend class UtGunnsFluidTank;
        friend class UtGunnsFluidBalloon;
};
inline FriendlyGunnsFluidNode::FriendlyGunnsFluidNode() : GunnsFluidNode() {};
inline FriendlyGunnsFluidNode::~FriendlyGunnsFluidNode() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Tank unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Tank link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidTank: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Quick Disconnect Hose unit test.
        UtGunnsFluidTank();
        /// @brief    Default destructs this Quick Disconnect Hose unit test.
        virtual ~UtGunnsFluidTank();
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
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests edit of temperature only.
        void testEditTemperature();
        /// @brief    Tests update state method - edit temperature and pressure.
        void testEditTemperatureAndPressure();
        /// @brief    Tests update state method - edit temperature and partial pressures.
        void testEditTemperatureAndPartialPressure();
        /// @brief    Tests update fluid method - edit partial pressures and rates.
        void testEditPartialPressureRate();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests the processOutputs method.
        void testProcessOutputs();
        /// @brief    Tests the dP/dt filter method.
        void testDpdt();
        /// @brief    Tests calculation of the shell heat flux.
        void testOwnShellFlux();
        /// @brief    Tests modifier methods.
        void testModifiers();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidTank);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testEditTemperature);
        CPPUNIT_TEST(testEditTemperatureAndPressure);
        CPPUNIT_TEST(testEditTemperatureAndPartialPressure);
        CPPUNIT_TEST(testEditPartialPressureRate);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testDpdt);
        CPPUNIT_TEST(testOwnShellFlux);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        ///  @brief   --     Constituent fluid types array.
        FluidProperties::FluidType   mTypes[N_FLUIDS];
        /// @brief    --     Constituent fluid mass fractions array.
        double                       mFractions[N_FLUIDS];
        /// @brief    --     Predefined fluid properties.
        DefinedFluidProperties*      mFluidProperties;
        ///  @brief   --     Fluid config data.
        PolyFluidConfigData*         mFluidConfig;
        /// @brief    --     Fluid input data for node 0.
        PolyFluidInputData*          mFluidInput0;
        /// @brief    --     Fluid input data for node 1.
        PolyFluidInputData*          mFluidInput1;
        /// @brief    --     Link vector.
        std::vector<GunnsBasicLink*> mLinks;
        /// @brief    --     Nominal name.
        std::string                  mName;
        /// @brief    --     Nominal connected nodes.
        FriendlyGunnsFluidNode       mNodes[N_NODES];
        /// @brief    --     Network node structure.
        GunnsNodeList                mNodeList;
        /// @brief    --     Nominal inlet port index.
        int                          mPort0;
        /// @brief    --     Nominal outlet port index.
        int                          mPort1;
        /// @brief    (m3)   Nominal volume.
        double                       mVolume;
        /// @brief    --     Nominal configuration for owning shell heat flux.
        double                       mShellTemperature;
        /// @brief    (W)    User bias for extra heat flux to the fluid.
        float                        mBiasHeatFlux;
        /// @brief    (K)    Tank shell surface area.
        float                        mSurfaceArea;
        /// @brief    (m)    Tank shell distance to center.
        float                        mShellRadius;
        /// @brief    --     Nominal scale factor for isentropic gas cooling.
        double                       mExpansionScaleFactor;
        /// @brief    --     Nominal dP/dt filter gain.
        double                       mDpdtFilterGain;
        /// @brief    --     Nominal thermal damping mass.
        double                       mThermalDampingMass;
        /// @brief    --     Nominal edit flux target tolerance.
        double                       mEditFluxTarget;
        /// @brief    --     Pointer to nominal configuration data.
        GunnsFluidTankConfigData*    mConfigData;
        /// @brief    --     Pointer to nominal input data.
        GunnsFluidTankInputData*     mInputData;
        /// @brief    --     Pointer to the friendly Tank under test.
        FriendlyGunnsFluidTank*      mArticle;
        /// @brief    (kg/s) Nominal mass flow rate.
        double                       mFlowRate;
        /// @brief    (s)    Nominal time step.
        double                       mTimeStep;
        /// @brief    --     Nominal tolerance for comparison of expected and returned values.
        double                       mTolerance;
        static int                   TEST_ID;                               /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidTank(const UtGunnsFluidTank&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidTank& operator =(const UtGunnsFluidTank&);
};

///@}

#endif
