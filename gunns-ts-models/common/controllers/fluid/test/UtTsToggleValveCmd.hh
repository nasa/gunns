#ifndef UtTsToggleValveCmd_EXISTS
#define UtTsToggleValveCmd_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_FLUID_TsToggleValveCmd     Toggle Valve Command Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Powered Toggle Valve command.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsToggleValveCmd.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsToggleValveController and befriend UtTsToggleValveController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsToggleValveCmd : public TsToggleValveCmd
{
    public:
        FriendlyTsToggleValveCmd(const bool enable = false, const bool toggle = false);
        virtual ~FriendlyTsToggleValveCmd();
        friend class UtTsToggleValveCmd;
};
inline FriendlyTsToggleValveCmd::FriendlyTsToggleValveCmd(const bool enable, const bool toggle) : TsToggleValveCmd(enable, toggle) {};
inline FriendlyTsToggleValveCmd::~FriendlyTsToggleValveCmd() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Powered Toggle Valve Command Unit Tests.
///
/// @details  This class provides the unit tests for the Powered Toggle Valve command within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsToggleValveCmd: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Powered Toggle Valve command unit test.
        UtTsToggleValveCmd();
        /// @brief    Default destructs this Powered Toggle Valve command unit test.
        virtual ~UtTsToggleValveCmd();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests nominal construction.
        void testNominalConstruction();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update method.
        void testUpdateNominal();
        /// @brief    Tests update method with queue.
        void testUpdateQueue();
    protected:
    private:
        CPPUNIT_TEST_SUITE(UtTsToggleValveCmd);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateNominal);
        CPPUNIT_TEST(testUpdateQueue);
        CPPUNIT_TEST_SUITE_END();
        FriendlyTsToggleValveCmd*  mArticle;   /**< (--)  Pointer to the article under test. */
        double                     mTimeStep;  /**< (s)   Nominal time step. */
        static int                 TEST_ID;    /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsToggleValveCmd(const UtTsToggleValveCmd&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsToggleValveCmd& operator =(const UtTsToggleValveCmd&);
};

/// @}

#endif
