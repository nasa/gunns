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
    std::cout << "-----------------------------------------------------------------------------";
    std::cout << "\n Strings Test 01: testFloatToString ..........................";

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
    std::cout << "\n Strings Test 01: testIntToString ............................";

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for string split.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtStrings::testSplit()
{
    std::cout << "\n Strings Test 03: testSplit ..................................";

    std::string input = "  Different hearts\nBeat on different strings ";
    std::string expected[8] = {"", "", "Different", "hearts\nBeat", "on", "different", "strings", ""};
    std::vector<std::string> actual = Strings::split(input, " ");
    CPPUNIT_ASSERT(8 == actual.size());
    CPPUNIT_ASSERT_EQUAL(expected[0], actual[0]);
    CPPUNIT_ASSERT_EQUAL(expected[1], actual[1]);
    CPPUNIT_ASSERT_EQUAL(expected[2], actual[2]);
    CPPUNIT_ASSERT_EQUAL(expected[3], actual[3]);
    CPPUNIT_ASSERT_EQUAL(expected[4], actual[4]);
    CPPUNIT_ASSERT_EQUAL(expected[5], actual[5]);
    CPPUNIT_ASSERT_EQUAL(expected[6], actual[6]);
    CPPUNIT_ASSERT_EQUAL(expected[7], actual[7]);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for string trim.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtStrings::testTrim()
{
    std::cout << "\n Strings Test 04: testTrim ...................................";

    std::string input = " \t\n\t But there are times\nFor you and me\nWhen all such things agree \n";
    std::string expected = "But there are times\nFor you and me\nWhen all such things agree";
    std::string actual = Strings::trim(input);
    CPPUNIT_ASSERT_EQUAL(expected, actual);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the method for string ends with.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtStrings::testEndsWith()
{
    std::cout << "\n Strings Test 05: endsWith ...................................";

    const std::string str = "system.subsystem.aspect.network.structure.surface";
    std::string end = "structure.surface";
    CPPUNIT_ASSERT(Strings::endsWith(str, "structure.surface"));
    CPPUNIT_ASSERT(Strings::endsWith(str, "e"));
    CPPUNIT_ASSERT(Strings::endsWith(str, ""));
    CPPUNIT_ASSERT(Strings::endsWith(str, str));
    CPPUNIT_ASSERT(not Strings::endsWith(str, " " + str));
    CPPUNIT_ASSERT(not Strings::endsWith(str, "\n"));
    CPPUNIT_ASSERT(not Strings::endsWith(str, " "));
    CPPUNIT_ASSERT(not Strings::endsWith(str, "E"));
    CPPUNIT_ASSERT(not Strings::endsWith(str, "Structure.surface"));

    std::cout << "... Pass";
}
