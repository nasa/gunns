#ifndef UtGunnsGasDisplacementPump_EXISTS
#define UtGunnsGasDisplacementPump_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_SOURCE_GAS_DISPLACEMENT_PUMP Gas Displacement Pump Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_SOURCE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Gas Displacement Pump link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/source/GunnsGasDisplacementPump.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsGasDisplacementPump and befriend UtGunnsGasDisplacementPump.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsGasDisplacementPump : public GunnsGasDisplacementPump
{
    public:
        FriendlyGunnsGasDisplacementPump();
        virtual ~FriendlyGunnsGasDisplacementPump();
        friend class UtGunnsGasDisplacementPump;
};
inline FriendlyGunnsGasDisplacementPump::FriendlyGunnsGasDisplacementPump() : GunnsGasDisplacementPump() {};
inline FriendlyGunnsGasDisplacementPump::~FriendlyGunnsGasDisplacementPump() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gas Displacement Pump unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Gas Displacement Pump link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsGasDisplacementPump: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Gas Displacement Pump unit test.
        UtGunnsGasDisplacementPump();
        /// @brief    Default destructs this Gas Displacement Pump unit test.
        virtual ~UtGunnsGasDisplacementPump();
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
        /// @brief    Tests update fluid method.
        void testUpdateFluid();
        /// @brief    Tests compute flows method.
        void testComputeFlows();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsGasDisplacementPump);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testUpdateFluid);
        CPPUNIT_TEST(testComputeFlows);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 2};
        FluidProperties::FluidType   tTypes[N_FLUIDS];     /**< (--) Constituent fluid types array */
        double                       tFractions[N_FLUIDS]; /**< (--) Constituent fluid mass fractions array */
        DefinedFluidProperties*      tFluidProperties;     /**< (--) Predefined fluid properties */
        PolyFluidConfigData*         tFluidConfig;         /**< (--) Fluid config data */
        PolyFluidInputData*          tFluidInput0;         /**< (--) Fluid input data for node 0 */
        PolyFluidInputData*          tFluidInput1;         /**< (--) Fluid input data for node 1 */
        std::vector<GunnsBasicLink*> tLinks;               /**< (--) Link vector */
        std::string                  tName;                /**< (--) Nominal name */
        GunnsFluidNode               tNodes[N_NODES];      /**< (--) Nominal connected nodes */
        GunnsNodeList                tNodeList;            /**< (--) Network node structure */
        int                          tPort0;               /**< (--) Nominal inlet port index */
        int                          tPort1;               /**< (--) Nominal outlet port index */
        double                       tCycleVolume;         /**< (m3) Volume of fluid displaced per cycle */
        double                       tDriveRatio;          /**< (--) Gear ratio of motor to impeller speed */
        double                       tThermalLength;       /**< (m)  Impeller length for thermal convection */
        double                       tThermalDiameter;     /**< (m)  Impeller inner diameter for thermal convection */
        double                       tSurfaceRoughness;    /**< (m)  Impeller wall surface roughness for thermal convection */
        bool                         tCheckValveActive;    /**< (--) Check valve active flag */
        GunnsGasDisplacementPumpConfigData* tConfigData;   /**< (--) Pointer to nominal config data */
        bool                         tBlockageFlag;        /**< (--) Blockage malf flag */
        double                       tBlockage;            /**< (--) Blockage malf value */
        double                       tFlowDemand;          /**< (kg/s) Initial flow demand */
        double                       tMotorSpeed;          /**< (rev/min) Initial motor speed */
        double                       tWallTemperature;     /**< (K) Initial wall temperature */
        GunnsGasDisplacementPumpInputData* tInputData;     /**< (--) Pointer to nominal input data */
        FriendlyGunnsGasDisplacementPump*  tArticle;       /**< (--) Pointer to test article */
        double                       tTimeStep;            /**< (--) Nominal time step */
        static const double          PI;                   /**< (--) Units conversion constant */
        static int                   TEST_ID;              /**< (--) Test identification number. */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsGasDisplacementPump(const UtGunnsGasDisplacementPump&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsGasDisplacementPump& operator =(const UtGunnsGasDisplacementPump&);
};

///@}

#endif
