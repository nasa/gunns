#ifndef UtGunnsLiquidCentrifugalPump_EXISTS
#define UtGunnsLiquidCentrifugalPump_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_POTENTIAL_LIQUID_CENTRIFUGAL_PUMP Liquid Centrifugal Pump Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_POTENTIAL
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Liquid Centrifugal Pump link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/potential/GunnsLiquidCentrifugalPump.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsLiquidCentrifugalPump and befriend UtGunnsLiquidCentrifugalPump.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsLiquidCentrifugalPump : public GunnsLiquidCentrifugalPump
{
    public:
        FriendlyGunnsLiquidCentrifugalPump();
        virtual ~FriendlyGunnsLiquidCentrifugalPump();
        friend class UtGunnsLiquidCentrifugalPump;
};
inline FriendlyGunnsLiquidCentrifugalPump::FriendlyGunnsLiquidCentrifugalPump() : GunnsLiquidCentrifugalPump() {};
inline FriendlyGunnsLiquidCentrifugalPump::~FriendlyGunnsLiquidCentrifugalPump() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Liquid Centrifugal Pump unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Liquid Centrifugal Pump link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsLiquidCentrifugalPump: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Liquid Centrifugal Pump unit test.
        UtGunnsLiquidCentrifugalPump();
        /// @brief    Default destructs this Liquid Centrifugal Pump unit test.
        virtual ~UtGunnsLiquidCentrifugalPump();
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
        CPPUNIT_TEST_SUITE(UtGunnsLiquidCentrifugalPump);
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
        double                       tMaxConductivity;      /**< (m2) Nominal maximum conductivity */
        double                       tExpansionScaleFactor; /**< (--) Nominal scale factor for isentropic gas cooling */
        double                       tReferenceDensity;     /**< (kg/m3) Reference inlet density for performance curve */
        double                       tReferenceSpeed;       /**< (rev/min) Reference fan speed for performance curve */
        double                       tReferenceCoeff0;      /**< (kPa) Reference performance curve 0th-order coefficient */
        double                       tReferenceCoeff1;      /**< (kPa) Reference performance curve 1st-order coefficient */
        double                       tReferenceCoeff2;      /**< (kPa) Reference performance curve 2nd-order coefficient */
        double                       tReferenceCoeff3;      /**< (kPa) Reference performance curve 3rd-order coefficient */
        double                       tReferenceCoeff4;      /**< (kPa) Reference performance curve 4th-order coefficient */
        double                       tReferenceCoeff5;      /**< (kPa) Reference performance curve 5th-order coefficient */
        double                       tBestEfficiency;       /**< (--)  (0-1) Efficiency at best efficiency point at reference */
        double                       tReferenceQBep;        /**< (m3/s) Volume flow rate at best efficiency point at reference */
        double                       tFilterGain;           /**< (--) Flow filter gain for system curve estimate */
        double                       tDriveRatio;           /**< (--) Gear ratio of motor to impeller speed */
        double                       tThermalLength;        /**< (m)  Impeller length for thermal convection */
        double                       tThermalDiameter;      /**< (m)  Impeller inner diameter for thermal convection */
        double                       tSurfaceRoughness;     /**< (m)  Impeller wall surface roughness for thermal convection */
        bool                         tCheckValveActive;     /**< (--) Check valve active flag */
        FluidProperties::FluidType   tLiquidType;           /**< (--) Type of the operating liquid */
        double                    tAutoCavitationDuration;  /**< (s)  Duration of auto-cavitation effect */
        double                    tAutoCavitationAmplitude; /**< (kPa) Amplitude of auto-cavitation noise */
        GunnsLiquidCentrifugalPumpConfigData* tConfigData;  /**< (--) Pointer to nominal config data */
        bool                         tBlockageFlag;         /**< (--) Blockage malf flag */
        double                       tBlockage;             /**< (--) Blockage malf value */
        double                       tSourcePressure;       /**< (kPa) Initial source pressure */
        double                       tMotorSpeed;           /**< (rev/min) Initial motor speed */
        double                       tWallTemperature;      /**< (K) Initial wall temperature */
        bool                         tEnableAutoCavitation; /**< (--) Enables the automatic cavitation model */
        GunnsLiquidCentrifugalPumpInputData* tInputData;    /**< (--) Pointer to nominal input data */
        FriendlyGunnsLiquidCentrifugalPump*  tArticle;      /**< (--) Pointer to test article */
        double                       tReferenceQ;           /**< (m3/s) Reference performance curve max flow rate */
        double                       tTimeStep;             /**< (--) Nominal time step */
        static int                   TEST_ID;               /**< (--) Test identification number. */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsLiquidCentrifugalPump(const UtGunnsLiquidCentrifugalPump&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsLiquidCentrifugalPump& operator =(const UtGunnsLiquidCentrifugalPump&);
};

///@}

#endif
