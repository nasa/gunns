#ifndef UtStrings_Exist
#define UtStrings_Exist

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_UTILITIES_MATH_MATH    Strings Unit Tests
/// @ingroup   UT_TSM_UTILITIES_MATH
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Strings class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Strings unit tests.
///
/// @details  This class provides the unit tests for the Strings class within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtStrings : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this unit test.
        UtStrings();
        /// @brief    Default destructs this unit test.
        virtual ~UtStrings();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Test float to string method.
        void testFloatToString();
        /// @brief    Test int to string method.
        void testIntToString();
        /// @brief    Test split method.
        void testSplit();
        /// @brief    Test trim method.
        void testTrim();
        /// @brief    Test endsWith method.
        void testEndsWith();

    private:
        CPPUNIT_TEST_SUITE(UtStrings);
        CPPUNIT_TEST(testFloatToString);
        CPPUNIT_TEST(testIntToString);
        CPPUNIT_TEST(testSplit);
        CPPUNIT_TEST(testTrim);
        CPPUNIT_TEST(testEndsWith);
        CPPUNIT_TEST_SUITE_END();
        double tolerance;

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtStrings(const UtStrings&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtStrings& operator =(const UtStrings&);
};

/// @}

#endif
