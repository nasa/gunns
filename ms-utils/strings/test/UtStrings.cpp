/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ()
***************************************************************************************************/

#include <iostream>
#include <string>

#include "strings/Strings.hh"

#include "UtStrings.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this strings unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtStrings::UtStrings()
    :
    CppUnit::TestFixture(),
    tolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this strings unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtStrings::~UtStrings()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtStrings::setUp()
{
    tolerance = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtStrings::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for float to string.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtStrings::testFloatToString()
{
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n Strings Test 01: Float to String Tests                    ";

    float convert    = 3.14159;
    float minValue   = 0.0;
    float maxValue   = 4.0;
    std::string expected = "3";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::floatToString(convert, 0, minValue, maxValue));

    expected = "3.1";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::floatToString(convert, 1, minValue, maxValue));

    expected = "3.14";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::floatToString(convert, 2, minValue, maxValue));

    expected = "3.142";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::floatToString(convert, 3, minValue, maxValue));

    expected = "3.1416";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::floatToString(convert, 4, minValue, maxValue));

    convert  = -3.0;
    expected = "0.0";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::floatToString(convert, 1, minValue, maxValue));

    convert  = 5.0;
    expected = "4.00";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::floatToString(convert, 2, minValue, maxValue));

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for int to string.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtStrings::testIntToString()
{
    std::cout << "\n Strings Test 02: Int to String Tests                      ";

    float convert    = 3;
    float minValue   = 0;
    float maxValue   = 1234;
    std::string expected = "3";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::intToString(convert, minValue, maxValue));

    convert  = 1234;
    expected = "1234";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::intToString(convert, minValue, maxValue));

    convert  = -3;
    expected = "0";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::intToString(convert, minValue, maxValue));

    convert  = 12345;
    expected = "1234";
    CPPUNIT_ASSERT_EQUAL(expected, Strings::intToString(convert, minValue, maxValue));

    std::cout << "... Pass";
}

