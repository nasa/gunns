#ifndef UtGunnsFluidLiquidWaterSensor_EXISTS
#define UtGunnsFluidLiquidWaterSensor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_LIQUID_WATER_SENSOR    Liquid Water Sensor Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Liquid Water Sensor link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidLiquidWaterSensor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidLiquidWaterSensor and befriend UtGunnsFluidLiquidWaterSensor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidLiquidWaterSensor : public GunnsFluidLiquidWaterSensor
{
    public:
        FriendlyGunnsFluidLiquidWaterSensor();
        virtual ~FriendlyGunnsFluidLiquidWaterSensor();
        friend class UtGunnsFluidLiquidWaterSensor;
};
inline FriendlyGunnsFluidLiquidWaterSensor::FriendlyGunnsFluidLiquidWaterSensor()
        : GunnsFluidLiquidWaterSensor() {};
inline FriendlyGunnsFluidLiquidWaterSensor::~FriendlyGunnsFluidLiquidWaterSensor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sensor unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Liquid Water Sensor link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidLiquidWaterSensor: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Sensor unit test.
        UtGunnsFluidLiquidWaterSensor();
        /// @brief    Default destructs this Sensor unit test.
        virtual ~UtGunnsFluidLiquidWaterSensor();
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
        CPPUNIT_TEST_SUITE(UtGunnsFluidLiquidWaterSensor);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 3, N_FLUIDS = 2};
        ///  @brief   --     Constituent fluid types array.
        FluidProperties::FluidType     mTypes[N_FLUIDS];
        /// @brief    --     Constituent fluid mass fractions array.
        double                         mFractions[N_FLUIDS];
        /// @brief    --     Predefined fluid properties.
        DefinedFluidProperties*        mFluidProperties;
        ///  @brief   --     Fluid config data.
        PolyFluidConfigData*           mFluidConfig;
        ///  @brief   --     Fluid config data with no H2O.
        PolyFluidConfigData*           mFluidConfig2;
        /// @brief    --     Fluid input data.
        PolyFluidInputData*            mFluidInput;
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
        /// (m2)   Nominal maximum conductivity
        double                         mMaxConductivity;
        /// @brief    --     Nominal scale factor for isentropic gas cooling.
        double                         mExpansionScaleFactor;
        /// @brief    --     Pointer to nominal configuration data.
        GunnsFluidLiquidWaterSensorConfigData*    mConfigData;
        /// @brief    --     Blockage malfunction flag.
        bool                           mMalfBlockageFlag;
        /// @brief    --     Blockage malfunction value.
        double                         mMalfBlockageValue;
        /// @brief    --     Pointer to nominal input data.
        GunnsFluidLiquidWaterSensorInputData*     mInputData;
        /// @brief    --     Pointer to the friendly Sensor under test.
        FriendlyGunnsFluidLiquidWaterSensor*      mArticle;
        static int                                TEST_ID; /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidLiquidWaterSensor(const UtGunnsFluidLiquidWaterSensor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidLiquidWaterSensor& operator =(const UtGunnsFluidLiquidWaterSensor&);
};

///@}

#endif
