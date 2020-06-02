#ifndef UtTsTValveController_EXISTS
#define UtTsTValveController_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_CONTROLLER_FLUID_T_VALVE_CONTROLLER  Manual T-Valve (3 way, 3 position) Controller Model Unit Tests
/// @ingroup  UT_TSM_CONTROLLER_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Manual T-Valve (3 way, 3 position) Controller Model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "common/controllers/fluid/TsTValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsTValveController and befriend UtTsTValveController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsTValveController : public TsTValveController
{
    public:
        FriendlyTsTValveController();
        virtual ~FriendlyTsTValveController();
        friend class UtTsTValveController;
};
inline FriendlyTsTValveController::FriendlyTsTValveController() : TsTValveController() {};
inline FriendlyTsTValveController::~FriendlyTsTValveController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Manual T-Valve (3 way, 3 position) Controller Model unit tests.
///
/// @details  This class provides the unit tests for the TS21 Fluid Controller Manual T-Valve
///           (3 way, 3 position) Controller Model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsTValveController: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Manual T-Valve Controller Model unit test.
        UtTsTValveController();
        /// @brief    Default destructs this Manual T-Valve Controller Model unit test.
        virtual ~UtTsTValveController();
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
        /// @brief    Tests update  method.
        void testUpdate();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtTsTValveController);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        std::string                    mName;       /**< (--)  Test article name. */
        TsTValveControllerConfigData*  mConfig;     /**< (--)  Pointer to nominal configuration data. */
        TsTValveControllerInputData*   mInput;      /**< (--)  Pointer to nominal input data. */
        FriendlyTsTValveController*    mArticle;    /**< (--)  Pointer to the unit under test. */
        double                         mTimeStep;   /**< (s)   Nominal time step. */
        double                         mTolerance;  /**< (--)  Nominal tolerance for comparison of expected and returned values.*/
        static int                     TEST_ID;     /**< (--)  Test identification number. */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsTValveController(const UtTsTValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsTValveController& operator =(const UtTsTValveController&);
};

///@}

#endif
