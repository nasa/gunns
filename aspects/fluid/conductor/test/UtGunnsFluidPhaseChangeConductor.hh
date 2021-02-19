#ifndef UtGunnsFluidPhaseChangeConductor_EXISTS
#define UtGunnsFluidPhaseChangeConductor_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_PHASE_CHANGE   Phase Change Conductor Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Phase Change Conductor link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/fluid/conductor/GunnsFluidPhaseChangeConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidPhaseChangeConductor and befriend UtGunnsFluidPhaseChangeConductor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidPhaseChangeConductor : public GunnsFluidPhaseChangeConductor
{
    public:
        FriendlyGunnsFluidPhaseChangeConductor();
        virtual ~FriendlyGunnsFluidPhaseChangeConductor();
        friend class UtGunnsFluidPhaseChangeConductor;
};
inline FriendlyGunnsFluidPhaseChangeConductor::FriendlyGunnsFluidPhaseChangeConductor() : GunnsFluidPhaseChangeConductor() {};
inline FriendlyGunnsFluidPhaseChangeConductor::~FriendlyGunnsFluidPhaseChangeConductor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Phase Change Conductor unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Phase Change Conductor link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidPhaseChangeConductor: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Phase Change Conductor unit test.
        UtGunnsFluidPhaseChangeConductor();
        /// @brief    Default constructs this Check Phase Change Conductor unit test.
        virtual ~UtGunnsFluidPhaseChangeConductor();
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
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests transport fluid method.
        void testTransportFluid();
        /// @brief    Tests transport fluid method with phase-change heat.
        void testTransportFluidWithHeat();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidPhaseChangeConductor);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testTransportFluid);
        CPPUNIT_TEST(testTransportFluidWithHeat);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 3, N_FLUIDS = 3};
        FluidProperties::FluidType   tTypes[N_FLUIDS];         /**< (--) Constituent fluid types array.                       */
        double                       tFractions0[N_FLUIDS];    /**< (--) Constituent fluid mass fractions array.              */
        double                       tFractions1[N_FLUIDS];    /**< (--) Constituent fluid mass fractions array.              */
        DefinedFluidProperties*      tFluidProperties;         /**< (--) Predefined fluid properties.                         */
        PolyFluidConfigData*         tFluidConfig;             /**< (--) Fluid config data.                                   */
        PolyFluidInputData*          tFluidInput0;             /**< (--) Fluid input data for node 0.                         */
        PolyFluidInputData*          tFluidInput1;             /**< (--) Fluid input data for node 1.                         */
        std::vector<GunnsBasicLink*> tLinks;                   /**< (--) Link vector.                                         */
        std::string                  tName;                    /**< (--) Nominal name.                                        */
        GunnsFluidNode               tNodes[N_NODES];          /**< (--) Nominal connected nodes.                             */
        GunnsNodeList                tNodeList;                /**< (--) Network node structure.                              */
        int                          tPort0;                   /**< (--) Nominal inlet port index.                            */
        int                          tPort1;                   /**< (--) Nominal outlet port index.                           */
        double                       tMaxConductivity;         /**< (m2) Nominal maximum conductivity.                        */
        FluidProperties::FluidType   tLiquidType;              /**< (--) Nominal liquid phase-change type.                    */
        FluidProperties::FluidType   tGasType;                 /**< (--) Nominal gas phase-change type.                       */
        double                       tEfficiency;              /**< (--) Nominal phase-change efficiency. */
        GunnsFluidPhaseChangeConductorConfigData* tConfigData; /**< (--) Pointer to nominal configuration data.               */
        bool                         tMalfBlockageFlag;        /**< (--) Blockage malfunction flag.                           */
        double                       tMalfBlockageValue;       /**< (--) Blockage malfunction value.                          */
        double                       tWallTemperature;         /**< (--) Initial wall temperature. */
        GunnsFluidPhaseChangeConductorInputData*  tInputData;  /**< (--) Pointer to nominal input data.                       */
        FriendlyGunnsFluidPhaseChangeConductor*   tArticle;    /**< (--) Pointer to the friendly Phase Change Conductor under test.            */
        double                       tFlowRate;                /**< (s)  Nominal flow rate.                                   */
        static int                   TEST_ID;                  /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidPhaseChangeConductor(const UtGunnsFluidPhaseChangeConductor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidPhaseChangeConductor& operator =(const UtGunnsFluidPhaseChangeConductor&);
};

///@}

#endif
