#ifndef UtParseTool_EXISTS
#define UtParseTool_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_UTILITIES_PARSING_ParseTool    ParseTool Unit Tests
/// @ingroup   UT_TSM_UTILITIES_PARSING
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the ParseTool class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ParseTool unit tests.
///
/// @details  This class provides the unit tests for the Math class within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtParseTool : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Check Valve unit test.
        UtParseTool();
        /// @brief    Default destructs this Check Valve unit test.
        virtual ~UtParseTool();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief  Tests that a string is cleared of quotes and preceding/trailing whitespace.
        void testTrim();
        /// @brief  Tests convertToDouble() method.
        void testConvertToDouble();
        /// @brief  Tests that a line is split according to a delimiting character.
        void testSplitLineByCharacter();
        /// @brief  Tests parseLines() method.
        void testParseLines();
    private:
        CPPUNIT_TEST_SUITE(UtParseTool);
        CPPUNIT_TEST(testTrim);
        CPPUNIT_TEST(testConvertToDouble);
        CPPUNIT_TEST(testSplitLineByCharacter);
        CPPUNIT_TEST(testParseLines);
        CPPUNIT_TEST_SUITE_END();

        int tMaxLineWidth; /**< (--) the max amount of character per line (default 50000) */
        double tTol;       /**< (--) Tolerance for comparison of expected and returned values */
        std::vector<std::string> tStringVector;  /**< (--) vector of comma-sep data from a string */

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtParseTool(const UtParseTool&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtParseTool& operator =(const UtParseTool&);
};

/// @}

#endif
