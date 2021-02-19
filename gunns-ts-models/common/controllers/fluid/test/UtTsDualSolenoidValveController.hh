#ifndef UtTsDualSolenoidValveController_EXISTS
#define UtTsDualSolenoidValveController_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_TsDualSolenoidValveController  Dual Solenoid Valve Controller Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Dual Solenoid Valve Controller model.
///
///@{
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsDualSolenoidValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsDualSolenoidValveController and befriend UtTsDualSolenoidValveController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsDualSolenoidValveController : public TsDualSolenoidValveController
{
    public:
        FriendlyTsDualSolenoidValveController();
        virtual ~FriendlyTsDualSolenoidValveController();
        friend class UtTsDualSolenoidValveController;
};
inline FriendlyTsDualSolenoidValveController::FriendlyTsDualSolenoidValveController() : TsDualSolenoidValveController() {};
inline FriendlyTsDualSolenoidValveController::~FriendlyTsDualSolenoidValveController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve Controller unit tests.
///
/// @details  This class provides the unit tests for the Dual Solenoid Valve Controller model within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsDualSolenoidValveController : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Dual Solenoid Valve Controller unit test.
        UtTsDualSolenoidValveController();
        /// @brief    Default destructs this Dual Solenoid Valve Controller unit test.
        virtual ~UtTsDualSolenoidValveController();
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
        /// @brief    Tests update method (nominal).
        void testUpdateNominal();
        /// @brief    Tests update method (both solenoids driven).
        void testUpdateDualDrive();
        /// @brief    Tests update method (malfunctions).
        void testUpdateMalfunctions();
        /// @brief    Tests initialization exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtTsDualSolenoidValveController);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateNominal);
        CPPUNIT_TEST(testUpdateDualDrive);
        CPPUNIT_TEST(testUpdateMalfunctions);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        double                                   tMinCmdPosition;      /**< (--) Minimum valid valve position. */
        double                                   tMaxCmdPosition;      /**< (--) Maximum valid valve  position. */
        double                                   tMinFluidPosition;    /**< (--) Minimum valid valve flow area fraction. */
        double                                   tMaxFluidPosition;    /**< (--) Maximum valid valve  flow area fraction. */
        TsDualSolenoidValveController::LatchType tLatch;               /**< (--) Latching type. */
        TsDualSolenoidValveControllerConfigData* tConfig;              /**< (--) Pointer to nominal configuration data. */
        double                                   tCmdPosition;         /**< (--) Valve position. */
        bool                                     tManualPositionFlag;  /**< (--) Manual override position flag. */
        double                                   tManualPositionValue; /**< (--) Manual override position value. */
        bool                                     tOpenSolenoidCmd;     /**< (--) Initial open channel powered flag  */
        bool                                     tCloseSolenoidCmd;    /**< (--) Initial close channel powered flag */
        TsDualSolenoidValveControllerInputData*  tInput;               /**< (--) Pointer to nominal input data. */
        std::string                              tName;                /**< (--) Object name. */
        FriendlyTsDualSolenoidValveController*   tArticle;             /**< (--) Pointer to article under test. */
        double                                   tTimeStep;            /**< (s)  Nominal time step. */
        static int                               TEST_ID;              /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsDualSolenoidValveController(const UtTsDualSolenoidValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsDualSolenoidValveController& operator =(const UtTsDualSolenoidValveController&);
};

/// @}

#endif
