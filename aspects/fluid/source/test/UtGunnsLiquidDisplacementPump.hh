#ifndef UtGunnsLiquidDisplacementPump_EXISTS
#define UtGunnsLiquidDisplacementPump_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_LIQUID_DISPLACEMENT_PUMP Liquid Displacement Pump Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Liquid Displacement Pump link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsLiquidDisplacementPump.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsLiquidDisplacementPump and befriend UtGunnsLiquidDisplacementPump.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsLiquidDisplacementPump : public GunnsLiquidDisplacementPump
{
    public:
        FriendlyGunnsLiquidDisplacementPump();
        virtual ~FriendlyGunnsLiquidDisplacementPump();
        friend class UtGunnsLiquidDisplacementPump;
};
inline FriendlyGunnsLiquidDisplacementPump::FriendlyGunnsLiquidDisplacementPump() : GunnsLiquidDisplacementPump() {};
inline FriendlyGunnsLiquidDisplacementPump::~FriendlyGunnsLiquidDisplacementPump() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Liquid Displacement Pump unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Liquid Displacement Pump link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsLiquidDisplacementPump: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Liquid Displacement Pump unit test.
        UtGunnsLiquidDisplacementPump();
        /// @brief    Default destructs this Liquid Displacement Pump unit test.
        virtual ~UtGunnsLiquidDisplacementPump();
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
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update state method.
        void testUpdateState();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsLiquidDisplacementPump);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        FluidProperties::FluidType   tTypes[N_FLUIDS];      /**< (--) Constituent fluid types array */
        double                       tFractions[N_FLUIDS];  /**< (--) Constituent fluid mass fractions array */
        DefinedFluidProperties*      tFluidProperties;      /**< (--) Predefined fluid properties */
        PolyFluidConfigData*         tFluidConfig;          /**< (--) Fluid config data */
        PolyFluidInputData*          tFluidInput0;          /**< (--) Fluid input data for node 0 */
        PolyFluidInputData*          tFluidInput1;          /**< (--) Fluid input data for node 1 */
        std::vector<GunnsBasicLink*> tLinks;                /**< (--) Link vector */
        std::string                  tName;                 /**< (--) Nominal name */
        GunnsFluidNode               tNodes[N_NODES];       /**< (--) Nominal connected nodes */
        GunnsNodeList                tNodeList;             /**< (--) Network node structure */
        int                          tPort0;                /**< (--) Nominal inlet port index */
        int                          tPort1;                /**< (--) Nominal outlet port index */
        double                       tCycleVolume;          /**< (m)  Volume of fluid displaced per cycle */
        double                       tDriveRatio;           /**< (--) Gear ratio of motor to impeller speed */
        double                       tThermalLength;        /**< (m)  Impeller length for thermal convection */
        double                       tThermalDiameter;      /**< (m)  Impeller inner diameter for thermal convection */
        double                       tSurfaceRoughness;     /**< (m)  Impeller wall surface roughness for thermal convection */
        bool                         tCheckValveActive;     /**< (--) Check valve active flag */
        FluidProperties::FluidType   tLiquidType;           /**< (--) Type of the operating liquid */
        double                    tAutoCavitationDuration;  /**< (s)  Duration of auto-cavitation effect */
        double                    tAutoCavitationAmplitude; /**< (m3/s) Amplitude of auto-cavitation noise */
        GunnsLiquidDisplacementPumpConfigData* tConfigData; /**< (--) Pointer to nominal config data */
        bool                         tBlockageFlag;         /**< (--) Blockage malf flag */
        double                       tBlockage;             /**< (--) Blockage malf value */
        double                       tFlowDemand;           /**< (kg/s) Initial flow demand */
        double                       tMotorSpeed;           /**< (rev/min) Initial motor speed */
        double                       tWallTemperature;      /**< (K)  Initial wall temperature */
        bool                         tEnableAutoCavitation; /**< (--) Enables the automatic cavitation model */
        GunnsLiquidDisplacementPumpInputData* tInputData;   /**< (--) Pointer to nominal input data */
        FriendlyGunnsLiquidDisplacementPump*  tArticle;     /**< (--) Pointer to test article */
        double                       tTimeStep;             /**< (--) Nominal time step */
        static int                   TEST_ID;               /**< (--) Test identification number. */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsLiquidDisplacementPump(const UtGunnsLiquidDisplacementPump&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsLiquidDisplacementPump& operator =(const UtGunnsLiquidDisplacementPump&);
};

///@}

#endif
