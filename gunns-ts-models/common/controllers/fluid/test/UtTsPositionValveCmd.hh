#ifndef UtTsPositionValveCmd_EXISTS
#define UtTsPositionValveCmd_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_FLUID_TsPositionValveCmd     Position Valve Command Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Powered Position Valve command.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsPositionValveCmd.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Powered Position Valve Command Unit Tests.
///
/// @details  This class provides the unit tests for the Powered Position Valve command within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsPositionValveCmd: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Powered Position Valve command unit test.
        UtTsPositionValveCmd();
        /// @brief    Default destructs this Powered Position Valve command unit test.
        virtual ~UtTsPositionValveCmd();
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
        CPPUNIT_TEST_SUITE(UtTsPositionValveCmd);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testAssignment);
        CPPUNIT_TEST_SUITE_END();
        TsPositionValveCmd*  mArticle;  /**< (--)  Pointer to the article under test. */
        static int           TEST_ID;   /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsPositionValveCmd(const UtTsPositionValveCmd&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsPositionValveCmd& operator =(const UtTsPositionValveCmd&);
};

/// @}

#endif
