#ifndef UtGunnsGasTurbine_EXISTS
#define UtGunnsGasTurbine_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_GAS_TURBINE   Gas Turbine Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Gas turbine link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsGasTurbine.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsGasTurbine and befriend UtGunnsGasTurbine.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsGasTurbine : public GunnsGasTurbine
{
    public:
        FriendlyGunnsGasTurbine();
        virtual ~FriendlyGunnsGasTurbine();
        friend class UtGunnsGasTurbine;
};
inline FriendlyGunnsGasTurbine::FriendlyGunnsGasTurbine() : GunnsGasTurbine() {};
inline FriendlyGunnsGasTurbine::~FriendlyGunnsGasTurbine() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gas Turbine unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Gas Turbine link model
///           within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsGasTurbine: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Gas Turbine unit test.
        UtGunnsGasTurbine();
        /// @brief    Default destructs this Gas Turbine unit test.
        virtual ~UtGunnsGasTurbine();
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
        /// @brief    Tests Restart model method.
        void testRestartModel();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update state method.
        void testUpdateState();
        /// @brief    Tests update fluid method.
        void testUpdateFluid();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsGasTurbine);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestartModel);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testUpdateFluid);
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
        double                       tReferenceTemp;        /**< (K)  Reference inlet temperature for corrected mass flow */
        double                       tReferencePress;       /**< (kPa) Reference inlet pressure for corrected mass flow */
        double                       tMaxFlowLowSpeed;      /**< (kg/s) Low maximum corrected flow rate  */
        double                       tLowSpeedCoeff1;       /**< (--) Low Speed 1st curve fit coefficient   */
        double                       tLowSpeedCoeff2;       /**< (--) Low Speed 2nd curve fit coefficient              */
        double                       tEffCoeffLowSpeed0;    /**< (--) Low Speed Efficiency performance curve 0th-order coefficient  */
        double                       tEffCoeffLowSpeed1;    /**< (--) Low Speed Efficiency performance curve 1st-order coefficient  */
        double                       tEffCoeffLowSpeed2;    /**< (--) Low Speed Efficiency performance curve 2nd-order coefficient  */
        double                       tEffCoeffLowSpeed3;    /**< (--) Low Speed Efficiency performance curve 3rd-order coefficient  */
        double                       tEffCoeffLowSpeed4;    /**< (--) Low Speed Efficiency performance curve 4th-order coefficient  */
        double                       tEffCoeffLowSpeed5;    /**< (--) Low Speed Efficiency performance curve 5th-order coefficient  */
        double                       tMaxFlowHighSpeed;     /**< (kg/s) High maximum corrected flow rate  */
        double                       tHighSpeedCoeff1;      /**< (--) High Speed 1st curve fit coefficient   */
        double                       tHighSpeedCoeff2;      /**< (--) High Speed 2nd curve fit coefficient              */
        double                       tEffCoeffHighSpeed0;   /**< (--) High Speed Efficiency performance curve 0th-order coefficient */
        double                       tEffCoeffHighSpeed1;   /**< (--) High Speed Efficiency performance curve 1st-order coefficient */
        double                       tEffCoeffHighSpeed2;   /**< (--) High Speed Efficiency performance curve 2nd-order coefficient */
        double                       tEffCoeffHighSpeed3;   /**< (--) High Speed Efficiency performance curve 3rd-order coefficient */
        double                       tEffCoeffHighSpeed4;   /**< (--) High Speed Efficiency performance curve 4th-order coefficient */
        double                       tEffCoeffHighSpeed5;   /**< (--) High Speed Efficiency performance curve 5th-order coefficient */
        double                       tCorrectedSpeedLow;    /**< (rev/min) Lowest corrected speed on turbine map.                        */
        double                       tCorrectedSpeedHigh;   /**< (rev/min) Highest corrected speed on turbine map.                       */
        double                       tMinEffLimitLowSpeed;  /**< (--) Lower limit on low speed efficiency curve. */
        double                       tMinEffLimitHighSpeed; /**< (--) Lower limit on high speed efficiency curve. */
        double                       tMaxEffLimitLowSpeed;  /**< (--) Upper limit on low speed efficiency curve. */
        double                       tMaxEffLimitHighSpeed; /**< (--) Upper limit on high speed efficiency curve. */
        double                       tDriveRatio;           /**< (--) Gear ratio of motor to impeller speed */
        double                       tFilterGain;           /**< (--) Low pass filter gain */
        double                       tThermalLength;        /**< (m)  Impeller length for thermal convection */
        double                       tThermalDiameter;      /**< (m)  Impeller inner diameter for thermal convection */
        double                       tSurfaceRoughness;     /**< (m)  Impeller wall surface roughness for thermal convection */
        GunnsGasTurbineConfigData*   tConfigData;           /**< (--) Pointer to nominal config data */
        bool                         tBlockageFlag;         /**< (--) Blockage malf flag */
        double                       tBlockage;             /**< (--) Blockage malf value */
        double                       tMotorSpeed;           /**< (rev/min) Initial motor speed */
        double                       tWallTemperature;      /**< (K) Initial wall temperature */
        GunnsGasTurbineInputData*    tInputData;            /**< (--) Pointer to nominal input data */
        FriendlyGunnsGasTurbine*     tArticle;              /**< (--) Pointer to test article */
        double                       tChokedMassFlow;       /**< (kg/s) Reference performance curve max flow rate */
        double                       tTimeStep;             /**< (--) Nominal time step */
        static const double          PI;                    /**< (--) Units conversion constant */
        static int                   TEST_ID;               /**< (--) Test identification number. */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsGasTurbine(const UtGunnsGasTurbine&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsGasTurbine& operator =(const UtGunnsGasTurbine&);
};

///@}

#endif
