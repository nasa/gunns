#ifndef UtTsOpenCloseValveCmd_EXISTS
#define UtTsOpenCloseValveCmd_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_FLUID_TsOpenCloseValveCmd     Open/Close Valve Command Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Powered Open/Close Valve command.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsOpenCloseValveCmd.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Powered Open/Close Valve Command Unit Tests.
///
/// @details  This class provides the unit tests for the Powered Open/Close Valve command within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsOpenCloseValveCmd: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Powered Open/Close Valve command unit test.
        UtTsOpenCloseValveCmd();
        /// @brief    Default destructs this Powered Open/Close Valve command unit test.
        virtual ~UtTsOpenCloseValveCmd();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests nominal construction.
        void testNominalConstruction();
        /// @brief    Tests copy construction.
        void testCopyConstruction();
        /// @brief    Tests assignment.
        void testAssignment();
    protected:
    private:
        CPPUNIT_TEST_SUITE(UtTsOpenCloseValveCmd);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testAssignment);
        CPPUNIT_TEST_SUITE_END();
        TsOpenCloseValveCmd*  mArticle;  /**< (--)  Pointer to the article under test. */
        static int            TEST_ID;   /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsOpenCloseValveCmd(const UtTsOpenCloseValveCmd&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsOpenCloseValveCmd& operator =(const UtTsOpenCloseValveCmd&);
};

/// @}

#endif
