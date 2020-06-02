#ifndef UtTsPidController_EXISTS
#define UtTsPidController_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_TsPidController  PID Controller Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the PID Controller model.
///
///@{
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsPidController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsPidController and befriend UtTsPidController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsPidController : public TsPidController
{
    public:
        FriendlyTsPidController();
        virtual ~FriendlyTsPidController();
        friend class UtTsPidController;
};
inline FriendlyTsPidController::FriendlyTsPidController() : TsPidController() {};
inline FriendlyTsPidController::~FriendlyTsPidController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PID Controller unit tests.
///
/// @details  This class provides the unit tests for the PID Controller model within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsPidController : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this PID Controller unit test.
        UtTsPidController();
        /// @brief    Default destructs this PID Controller unit test.
        virtual ~UtTsPidController();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config data.
        void testConfigData();
        /// @brief    Tests input data.
        void testInputData();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update method.
        void testUpdate();
        /// @brief    Tests update method with input argument.
        void testUpdateWithInput();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();

    private:
        CPPUNIT_TEST_SUITE(UtTsPidController);
        CPPUNIT_TEST(testConfigData);
        CPPUNIT_TEST(testInputData);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testUpdateWithInput);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        double                     tGainP;                   /**< (--)  Proportional gain. */
        double                     tGainI;                   /**< (1/s) Integral gain. */
        double                     tGainD;                   /**< (s)   Derivative gain. */
        double                     tInterval;                /**< (s)   Control update interval. */
        double                     tLimitLow;                /**< (--)  Lower limit on output value. */
        double                     tLimitHigh;               /**< (--)  Upper limit on output value. */
        TsPidControllerConfigData* tConfigData;              /**< (--)  Pointer to nominal configuration data. */
        double                     tInput;                   /**< (--)  Initial input process variable. */
        double                     tSetpoint;                /**< (--)  Initial setpoint value to control to. */
        double                     tTimer;                   /**< (s)   Initial timer for control update interval. */
        double                     tIntegral;                /**< (s)   Initial controller integral value. */
        double                     tOutput;                  /**< (--)  Initial output control value. */
        TsPidControllerInputData*  tInputData;               /**< (--)  Pointer to nominal input data. */
        std::string                tName;                    /**< (--)  Object name. */
        FriendlyTsPidController*   tArticle;                 /**< (--)  Pointer to article under test. */
        double                     tTimeStep;                /**< (s)   Nominal time step. */
        static int                 TEST_ID;                  /**< (--)  Test identification number. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        UtTsPidController(const UtTsPidController&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        UtTsPidController& operator =(const UtTsPidController&);
};

/// @}

#endif
