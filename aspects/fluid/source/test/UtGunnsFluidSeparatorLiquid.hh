#ifndef UtGunnsFluidSeparatorLiquid_EXISTS
#define UtGunnsFluidSeparatorLiquid_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_SEPARATOR_LIQUID   Separator Liquid Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Separator Liquid link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsFluidSeparatorLiquid.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidSeparatorLiquid and befriend UtGunnsFluidSeparatorLiquid.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidSeparatorLiquid : public GunnsFluidSeparatorLiquid
{
    public:
        FriendlyGunnsFluidSeparatorLiquid();
        virtual ~FriendlyGunnsFluidSeparatorLiquid();
        friend class UtGunnsFluidSeparatorLiquid;
};
inline FriendlyGunnsFluidSeparatorLiquid::FriendlyGunnsFluidSeparatorLiquid() : GunnsFluidSeparatorLiquid() {};
inline FriendlyGunnsFluidSeparatorLiquid::~FriendlyGunnsFluidSeparatorLiquid() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hatch unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Separator Liquid link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidSeparatorLiquid: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Separator Liquid unit test.
        UtGunnsFluidSeparatorLiquid();
        /// @brief    Default constructs this Separator Liquid unit test.
        virtual ~UtGunnsFluidSeparatorLiquid();
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
        /// @brief    Tests update fluid method (nominal).
        void testUpdateFluidNominal();
        /// @brief    Tests update fluid method (no flow).
        void testProcessOutputs();
        /// @brief    Tests specific port mapping rules.
        void testPortMapping();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidSeparatorLiquid);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateFluidNominal);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testPortMapping);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        /// @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        /// @brief    --     Constituent fluid types array.
        FluidProperties::FluidType           mTypes[N_FLUIDS];
        /// @brief    --     Constituent fluid mass fractions array.
        double                               mFractions[N_FLUIDS];
        /// @brief    --     Predefined fluid properties.
        DefinedFluidProperties*              mFluidProperties;
        /// @brief    --     Fluid config data.
        PolyFluidConfigData*                 mFluidConfig;
        /// @brief    --     Fluid input data for node 0.
        PolyFluidInputData*                  mFluidInput0;
        /// @brief    --     Fluid input data for node 1.
        PolyFluidInputData*                  mFluidInput1;
        /// @brief    --     Link vector.
        std::vector<GunnsBasicLink*>         mLinks;
        /// @brief    --     Nominal name.
        std::string                          mName;
        /// @brief    --     Nominal connected nodes.
        GunnsFluidNode                       mNodes[N_NODES];
        /// @brief    --     Network node structure.
        GunnsNodeList                        mNodeList;
        /// @brief    --     Nominal inlet port index.
        int                                  mPort0;
        /// @brief    --     Nominal outlet port index.
        int                                  mPort1;
        /// @brief    --     Nominal max conductivity.
        double                               mMaxConductivity;
        /// @brief    --     Nominal expansion scale factor.
        double                               mExpansionScaleFactor;
        /// @brief    --     Liquid phase fluid type of constituent being separated.
        FluidProperties::FluidType           mLiquidType;
        /// @brief    --     Index of gas phase fluid type of constituent being separated.
        int                                  mLiquidIndex;
        /// @brief    --     Pointer to nominal configuration data.
        GunnsFluidSeparatorLiquidConfigData* mConfigData;
        /// @brief    --     Blockage malfunction flag.
        bool                                 mMalfBlockageFlag;
        /// @brief    --     Blockage malfunction value.
        double                               mMalfBlockageValue;
//        ///  @brief   (kg/s) Initial flow demand
//        double                               mFlowDemand;
        /// @brief    (K)    Temperature of incoming liquid.
        double                               mTransferTemperature;
        /// @brief    (kg/s) Mass flow rate of incoming liquid.
        double                               mTransferFlowRate;
        /// @brief    (kPa)  Supply pressure on incoming liquid.
        double                               mSourcePressure;
        /// @brief    --     Pointer to nominal input data.
        GunnsFluidSeparatorLiquidInputData*  mInputData;
        /// @brief    --     Pointer to the friendly Hatch under test.
        FriendlyGunnsFluidSeparatorLiquid*   mArticle;
        /// @brief    (s)    Nominal flow rate.
        double                               mFlowRate;
        /// @brief    (s)    Nominal time step.
        double                               mTimeStep;
        /// @brief    --     Nominal tolerance for comparison of expected and returned values.
        double                               mTolerance;
        static int                           TEST_ID; /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSeparatorLiquid(const UtGunnsFluidSeparatorLiquid&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidSeparatorLiquid& operator =(const UtGunnsFluidSeparatorLiquid&);
};

///@}

#endif
