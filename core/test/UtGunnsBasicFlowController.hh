#ifndef UtGunnsBasicFlowController_EXISTS
#define UtGunnsBasicFlowController_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_Basic_FLOW_CONTROLLER   Basic Flow Controller Unit Tests
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Basic Flow Controller link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "core/GunnsBasicFlowController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsBasicFlowController and befriend UtGunnsBasicFlowController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsBasicFlowController : public GunnsBasicFlowController
{
    public:
        FriendlyGunnsBasicFlowController();
        virtual ~FriendlyGunnsBasicFlowController();
        friend class UtGunnsBasicFlowController;
};
inline FriendlyGunnsBasicFlowController::FriendlyGunnsBasicFlowController() : GunnsBasicFlowController() {};
inline FriendlyGunnsBasicFlowController::~FriendlyGunnsBasicFlowController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Flow Controller unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Basic Flow Controller link
///           model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsBasicFlowController: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Flow Controller unit test.
        UtGunnsBasicFlowController();
        /// @brief    Default constructs this Check Flow Controller unit test.
        virtual ~UtGunnsBasicFlowController();
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
        CPPUNIT_TEST_SUITE(UtGunnsBasicFlowController);
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
        ///  @brief   Enumeration for the number of nodes.
        enum {N_NODES = 2};
        std::vector<GunnsBasicLink*>        tLinks;                /**< (--) Link vector. */
        std::string                         tName;                 /**< (--) Nominal name. */
        GunnsBasicNode                      tNodes[N_NODES];       /**< (--) Nominal connected nodes. */
        GunnsNodeList                       tNodeList;             /**< (--) Network node structure. */
        int                                 tPort0;                /**< (--) Nominal inlet port index. */
        int                                 tPort1;                /**< (--) Nominal outlet port index. */
        double                              tDefaultConductivity;  /**< (--) Nominal default conductivity. */
        double                              tNonLinear;            /**< (--) Nominal expansion scale factor. */
        bool                                tEnableReverseControl; /**< (--) Nominal enable reverse flag. */
        GunnsBasicFlowControllerConfigData* tConfigData;           /**< (--) Pointer to nominal configuration data. */
        bool                                tMalfBlockageFlag;     /**< (--) Blockage malfunction flag. */
        double                              tMalfBlockageValue;    /**< (--) Blockage malfunction value. */
        bool                                tMalfFluxFlag;         /**< (--) Flux malfunction flag. */
        double                              tMalfFluxValue;        /**< (--) Flux malfunction value. */
        double                              tFluxCommand;          /**< (--) Nominal flow rate command. */
        GunnsBasicFlowControllerInputData*  tInputData;            /**< (--) Pointer to nominal input data. */
        FriendlyGunnsBasicFlowController*   tArticle;              /**< (--) Pointer to the friendly Flow Controller under test. */
        static int                          TEST_ID;               /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsBasicFlowController(const UtGunnsBasicFlowController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsBasicFlowController& operator =(const UtGunnsBasicFlowController&);
};

///@}

#endif
