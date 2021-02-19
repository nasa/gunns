#ifndef UtGunnsFluidLeak_EXISTS
#define UtGunnsFluidLeak_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_LEAK     Leak Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Leak link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidLeak.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidLeak and befriend UtGunnsFluidLeak.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidLeak : public GunnsFluidLeak
{
    public:
        FriendlyGunnsFluidLeak();
        virtual ~FriendlyGunnsFluidLeak();
        friend class UtGunnsFluidLeak;
};
inline FriendlyGunnsFluidLeak::FriendlyGunnsFluidLeak() : GunnsFluidLeak() {};
inline FriendlyGunnsFluidLeak::~FriendlyGunnsFluidLeak() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Leak unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Leak link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidLeak: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Leak unit test.
        UtGunnsFluidLeak();
        /// @brief    Default destructs this Leak unit test.
        virtual ~UtGunnsFluidLeak();
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
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update state method.
        void testUpdateState();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidLeak);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        ///  @brief   --     Constituent fluid types array.
        FluidProperties::FluidType     mTypes[N_FLUIDS];
        /// @brief    --     Constituent fluid mass fractions array.
        double                         mFractions[N_FLUIDS];
        /// @brief    --     Predefined fluid properties.
        DefinedFluidProperties*        mFluidProperties;
        ///  @brief   --     Fluid config data.
        PolyFluidConfigData*           mFluidConfig;
        /// @brief    --     Fluid input data for node 0.
        PolyFluidInputData*            mFluidInput0;
        /// @brief    --     Fluid input data for node 1.
        PolyFluidInputData*            mFluidInput1;
        /// @brief    --     Link vector.
        std::vector<GunnsBasicLink*>   mLinks;
        /// @brief    --     Nominal name.
        std::string                    mName;
        /// @brief    --     Nominal connected nodes.
        GunnsFluidNode                 mNodes[N_NODES];
        /// @brief    --     Network node structure.
        GunnsNodeList                  mNodeList;
        /// @brief    --     Nominal inlet port index.
        int                            mPort0;
        /// @brief    --     Nominal outlet port index.
        int                            mPort1;
        /// @brief    (m2)   Nominal maximum conductivity.
        double                         mMaxConductivity;
        /// @brief    --     Nominal scale factor for isentropic gas cooling.
        double                         mExpansionScaleFactor;
        /// @brief    --     Pointer to nominal configuration data.
        GunnsFluidLeakConfigData*      mConfigData;
        /// @brief    --     Blockage malfunction flag.
        bool                           mMalfBlockageFlag;
        /// @brief    --     Blockage malfunction value.
        double                         mMalfBlockageValue;
        /// @brief    --     Initial leak hole malfunction flag for this Leak.
        bool                           mMalfLeakHoleFlag;
        /// @brief    (m2)   Initial leak hole malfunction value for this Leak.
        double                         mMalfLeakHoleValue;
        /// @brief    --     Initial leak rate malfunction flag for this Leak.
        bool                           mMalfLeakRateFlag;
        /// @brief    (kg/s) Initial leak rate malfunction value for this Leak.
        double                         mMalfLeakRateValue;
        /// @brief    --     Pointer to nominal input data.
        GunnsFluidLeakInputData*       mInputData;
        /// @brief    --     Pointer to the friendly Leak under test.
        FriendlyGunnsFluidLeak*        mArticle;
        /// @brief    (m2)   Previous leak hole value for this Leak.
        double                         mPreviousLeakHole;
        /// @brief    (kg/s) Previous leak rate value for this Leak.
        double                         mPreviousLeakRate;
        /// @brief    (m2)   Conductivity equivalent to the leak rate for this Leak.
        double                         mLeakConductivity;
        /// @brief    (kg/s) Nominal mass flow rate.
        double                         mFlowRate;
        /// @brief    (s)    Nominal time step.
        double                         mTimeStep;
        /// @brief    --     Nominal tolerance for comparison of expected and returned values.
        double                         mTolerance;
        static int                     TEST_ID; /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidLeak(const UtGunnsFluidLeak&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidLeak& operator =(const UtGunnsFluidLeak&);
};

///@}

#endif
