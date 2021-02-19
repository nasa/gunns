#ifndef UtGunnsFluidCondensingHxSeparator_EXISTS
#define UtGunnsFluidCondensingHxSeparator_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_CONDENSING_HX_SEPARATOR  Condensing HX & Separator Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Condensing HX & Separator link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidCondensingHxSeparator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidCondensingHxSeparator and befriend UtGunnsFluidCondensingHxSeparator.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidCondensingHxSeparator : public GunnsFluidCondensingHxSeparator
{
    public:
        FriendlyGunnsFluidCondensingHxSeparator();
        virtual ~FriendlyGunnsFluidCondensingHxSeparator();
        friend class UtGunnsFluidCondensingHxSeparator;
};
inline FriendlyGunnsFluidCondensingHxSeparator::FriendlyGunnsFluidCondensingHxSeparator()
    : GunnsFluidCondensingHxSeparator() {};
inline FriendlyGunnsFluidCondensingHxSeparator::~FriendlyGunnsFluidCondensingHxSeparator() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Condensing HX & Separator unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Fluid Condensing HX & Separator link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidCondensingHxSeparator: public CppUnit::TestFixture {
    public:
        /// @brief    Default constructs this Condensing HX & Separator Flow Path unit test.
        UtGunnsFluidCondensingHxSeparator();
        /// @brief    Default destructs this Condensing HX & Separator Flow Path unit test.
        virtual ~UtGunnsFluidCondensingHxSeparator();
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
        void testInitialization();
        /// @brief    Tests restart methods.
        void testRestart();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests step method.
        void testStep();
        /// @brief    Tests computeHeatTransferCoefficient method.
        void testHtc();
        /// @brief    Tests the condensing heat exchanger in forward flow regimes.
        void testHxFlow();
        /// @brief    Tests the condensing heat exchanger in the no-flow case.
        void testHxNoFlow();
        /// @brief    Tests the slurper model.
        void testSlurper();
        /// @brief    Tests water separator model.
        void testWaterSeparator();
        /// @brief    Tests the processOutputs method.
        void testProcessOutputs();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests the HX condensation pooling rate override malfunction.
        void testPoolRateOverride();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidCondensingHxSeparator);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testHtc);
        CPPUNIT_TEST(testHxFlow);
        CPPUNIT_TEST(testHxNoFlow);
        CPPUNIT_TEST(testSlurper);
        CPPUNIT_TEST(testWaterSeparator);
        CPPUNIT_TEST(testProcessOutputs);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testPoolRateOverride);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 3, N_FLUIDS = 3};
        FluidProperties::FluidType                 tTypes[N_FLUIDS];     /**< (--)      Constituent fluid types array */
        double                                     tFractions[N_FLUIDS]; /**< (--)      Constituent fluid mass fractions array */
        DefinedFluidProperties*                    tFluidProperties;     /**< (--)      Predefined fluid properties */
        PolyFluidConfigData*                       tFluidConfig;         /**< (--)      Fluid config data */
        PolyFluidInputData*                        tFluidInput;          /**< (--)      Fluid input data */
        PolyFluidInputData*                        tFluidInput1;         /**< (--)      Fluid input data */
        std::vector<GunnsBasicLink*>               tLinks;               /**< (--)      Link vector */
        std::string                                tName;                /**< (--)      Test name */
        GunnsFluidNode                             tNodes[N_NODES];      /**< (--)      Test connected nodes */
        GunnsNodeList                              tNodeList;            /**< (--)      Test node structure */
        int                                        tPort0;               /**< (--)      Test inlet port index */
        int                                        tPort1;               /**< (--)      Test outlet port index */
        double                                     tMaxConductivity;     /**< (m2)      Nominal config data */
        int                                        tHxNumSegments;       /**< (--)      Nominal config data */
        double                                     tHxDryHtc;            /**< (W/K)     Nominal config data */
        double                                     tHxWetHtcFactor;      /**< (one/kg)  Nominal config data */
        double                                     tHxWetFlowFactor;     /**< (one/kg)  Nominal config data */
        double                                     tHxEvaporationCoeff;  /**< (kg/s/kPa) Nominal config data */
        double                                     tSlurperFlowMassDry;  /**< (kg)      Nominal config data */
        double                                     tSlurperFlowMassWet;  /**< (kg)      Nominal config data */
        double                                     tSlurperFlowRateRef;  /**< (kg/s)    Nominal config data */
        double                                     tWsMassExponent;      /**< (--)      Nominal config data */
        double                                     tWsMaxCondensate;     /**< (kg)      Nominal config data */
        double                                     tWsReferenceSpeed;    /**< (rev/min) Nominal config data */
        double                                     tWsReferencePressure; /**< (kPa)     Nominal config data */
        double                                     tWsReferenceTorque;   /**< (N*m)     Nominal config data */
        double                                     tWsDriveRatio;        /**< (--)      Nominal config data */
        double                                     tWsEvaporationCoeff;  /**< (kg/s/kPa) Nominal config data */
        GunnsFluidCondensingHxSeparatorConfigData* tConfigData;          /**< (--)      Nominal config data */
        bool                                       tMalfBlockageFlag;    /**< (--)      Nominal input data */
        double                                     tMalfBlockageValue;   /**< (--)      Nominal input data */
        double                                     tWallTemperature;     /**< (K)       Nominal input data */
        double                                     tWsMotorSpeed;        /**< (rev/min) Nominal input data */
        double                                     tWsCondensateMass;    /**< (kg)      Nominal input data */
        double                                     tHxCondensateMass;    /**< (kg)      Nominal input data */
        double                                     tTransferFlowRate;    /**< (kg/s)    Nominal input data */
        GunnsFluidCondensingHxSeparator::SlurperStates tSlurperState;    /**< (--)      Nominal input data */
        GunnsFluidCondensingHxSeparatorInputData*  tInputData;           /**< (--)      nominal input data */
        FriendlyGunnsFluidCondensingHxSeparator*   tArticle;             /**< (--)      Test article */
        double                                     tTimeStep;            /**< (s)       Test time step */
        static int                                 TEST_ID;              /**< (--)      Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidCondensingHxSeparator(const UtGunnsFluidCondensingHxSeparator&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidCondensingHxSeparator& operator =(const UtGunnsFluidCondensingHxSeparator&);
};

///@}

#endif
