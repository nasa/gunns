#ifndef UtGunnsFluidFlowController_EXISTS
#define UtGunnsFluidFlowController_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_FLUID_FLOW_CONTROLLER   Fluid Flow Controller Unit Tests
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Flow Controller link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsFluidFlowController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidFlowController and befriend UtGunnsFluidFlowController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidFlowController : public GunnsFluidFlowController
{
    public:
        FriendlyGunnsFluidFlowController();
        virtual ~FriendlyGunnsFluidFlowController();
        friend class UtGunnsFluidFlowController;
};
inline FriendlyGunnsFluidFlowController::FriendlyGunnsFluidFlowController() : GunnsFluidFlowController() {};
inline FriendlyGunnsFluidFlowController::~FriendlyGunnsFluidFlowController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Flow Controller unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Flow Controller link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidFlowController: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Flow Controller unit test.
        UtGunnsFluidFlowController();
        /// @brief    Default constructs this Check Flow Controller unit test.
        virtual ~UtGunnsFluidFlowController();
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
        /// @brief    Tests updateState method.
        void testUpdateState();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidFlowController);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        ///  @brief   Enumeration for the number of nodes and fluid constituents.
        enum {N_NODES = 2, N_FLUIDS = 1};
        FluidProperties::FluidType          tTypes[N_FLUIDS];        /**< (--)   Constituent fluid types array. */
        double                              tFractions[N_FLUIDS];    /**< (--)   Constituent fluid mass fractions array. */
        DefinedFluidProperties*             tFluidProperties;        /**< (--)   Predefined fluid properties. */
        PolyFluidConfigData*                tFluidConfig;            /**< (--)   Fluid config data. */
        PolyFluidInputData*                 tFluidInput;             /**< (--)   Fluid input data. */
        std::vector<GunnsBasicLink*>        tLinks;                  /**< (--)   Link vector. */
        std::string                         tName;                   /**< (--)   Nominal name. */
        GunnsFluidNode                      tNodes[N_NODES];         /**< (--)   Nominal connected nodes. */
        GunnsNodeList                       tNodeList;               /**< (--)   Network node structure. */
        int                                 tPort0;                  /**< (--)   Nominal inlet port index. */
        int                                 tPort1;                  /**< (--)   Nominal outlet port index. */
        double                              tMaxConductivity;        /**< (m2)   Nominal maximum conductivity. */
        double                              tExpansionScaleFactor;   /**< (--)   Nominal expansion scale factor. */
        double                              tFilterProportionalGain; /**< (--)   Nominal filter gain. */
        bool                                tEnableReverseControl;   /**< (--)   Nominal enable reverse flag. */
        GunnsFluidFlowControllerConfigData* tConfigData;             /**< (--)   Pointer to nominal configuration data. */
        bool                                tMalfBlockageFlag;       /**< (--)   Blockage malfunction flag. */
        double                              tMalfBlockageValue;      /**< (--)   Blockage malfunction value. */
        bool                                tMalfFlowRateFlag;       /**< (--)   Flow rate malfunction flag. */
        double                              tMalfFlowRateValue;      /**< (kg/s) Flow rate malfunction value. */
        double                              tFlowRateCommand;        /**< (kg/s) Nominal flow rate command. */
        GunnsFluidFlowControllerInputData*  tInputData;              /**< (--)   Pointer to nominal input data. */
        FriendlyGunnsFluidFlowController*   tArticle;                /**< (--)   Pointer to the friendly Flow Controller under test. */
        static int                          TEST_ID;                 /**< (--)   Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidFlowController(const UtGunnsFluidFlowController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsFluidFlowController& operator =(const UtGunnsFluidFlowController&);
};

///@}

#endif
