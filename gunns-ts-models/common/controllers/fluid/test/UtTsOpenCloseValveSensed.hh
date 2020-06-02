#ifndef UtTsOpenCloseValveSensed_EXISTS
#define UtTsOpenCloseValveSensed_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_FLUID_TsOpenCloseValveSensed     Open/Close Valve Sensed Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS_FLUID
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Powered Open/Close Valve sensed position.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsOpenCloseValveSensed.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Powered Open/Close Valve Sensed Unit Tests.
///
/// @details  This class provides the unit tests for the Powered Open/Close Valve sensed within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsOpenCloseValveSensed: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Powered Open/Close Valve sensed unit test.
        UtTsOpenCloseValveSensed();
        /// @brief    Default destructs this Powered Open/Close Valve sensed unit test.
        virtual ~UtTsOpenCloseValveSensed();
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
        CPPUNIT_TEST_SUITE(UtTsOpenCloseValveSensed);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalConstruction);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testAssignment);
        CPPUNIT_TEST_SUITE_END();
        TsOpenCloseValveSensed*  mArticle;  /**< (--)  Pointer to the article under test. */
        static int               TEST_ID;   /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsOpenCloseValveSensed(const UtTsOpenCloseValveSensed&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsOpenCloseValveSensed& operator =(const UtTsOpenCloseValveSensed&);
};

/// @}

#endif
