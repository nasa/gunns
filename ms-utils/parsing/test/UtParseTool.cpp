/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((software/exceptions/TsParseException.o)
     (parsing/ParseTool.o))
***************************************************************************************************/
#include <iostream>
#include "software/exceptions/TsParseException.hh"
#include "parsing/ParseTool.hh"
#include "UtParseTool.hh"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this math unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtParseTool::UtParseTool()
    :
    CppUnit::TestFixture(),
    tMaxLineWidth(0),
    tTol(0.0),
    tStringVector()
{
    /// - Clear the vector of strings.
    tStringVector.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this math unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtParseTool::~UtParseTool()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtParseTool::setUp()
{
    /// - Maximum characters per line
    tMaxLineWidth = 500;

    // show no mercy
    tTol = 0.0;

    /// - clear the data vector
    tStringVector.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtParseTool::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that a string is cleared of quotes and preceding/trailing whitespace.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtParseTool::testTrim()
{
    std::cout << "\n -----------------------------------------------------------------------------";
    std::cout << "\n ParseTool 01: Testing trim() function .................................";

    /// - Set testString.
    std::string testString = "         no\"de 0   ";
    /// - Trim testString.
    testString = ParseTool::trim(testString);
    /// @test  The testString is appropriately trimmed of
    CPPUNIT_ASSERT_MESSAGE("preceding/trailing whitespace", "no\"de 0" == testString);

    /// - Set testString.
    testString = "\"no\"de 0 \"";
    /// - Trim testString.
    testString = ParseTool::trim(testString, "\"");
    /// @test  The testString is appropriately trimmed of double-quotes.
    CPPUNIT_ASSERT_MESSAGE("preceding/trailing \" ", "node 0" == testString);

    /// - Set testString.
    testString = "\'nod'e 0 \'";
    /// - Trim testString.
    testString = ParseTool::trim(testString, "\'");
    /// @test  The testString is appropriately trimmed of single-quotes.
    CPPUNIT_ASSERT_MESSAGE("preceding/trailing \' ", "node 0" == testString);

    /// - Set testString.
    testString = "\'no\"d'e 0'\"'' \'";
    /// - Trim testString.
    testString = ParseTool::trim(testString, "\"\'");
    /// @test  The testString is appropriately trimmed of single-quotes.
    CPPUNIT_ASSERT_MESSAGE("preceding/trailing \' ", "node 0" == testString);

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests convertToDouble() method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtParseTool::testConvertToDouble()
{
    std::cout << "\n ParseTool 02: Testing convertToDouble() function ......................";

    /// - Declare a double.
    double testDouble = 0.0;

    /// - Try to convert a valid string into a double.
    testDouble = ParseTool::convertToDouble("159.12", "test", "test");

    /// @test  Nominal double conversion.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("convert \"159.12\" to double",
            159.12, testDouble, tTol);

    /// - Try to convert an invalid string into a double.
    testDouble = ParseTool::convertToDouble("notADouble", "test", "test");

    /// @test  Off-nominal double conversion.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("convert \"notADouble\" to double",
            0.0, testDouble, tTol);

    /// - Try to convert an out-of-range numeric string into a double.
    testDouble = ParseTool::convertToDouble("99e999", "test", "test");

    /// @test  Double conversion out of range.
    CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("double conversion out of range",
            DBL_MAX, testDouble, tTol);

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests that a line is split according to a delimiting character.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtParseTool::testSplitLineByCharacter()
{
    std::cout << "\n ParseTool 03: Testing splitLineByCharacter() function .................";

    /// - Set testLine.
    std::string testLine = "   Yankees, \"  New York  \"  , \'American League\', East, 27";

    /// - Split testLine into a vector of strings.
    ParseTool::splitLineByCharacter(tStringVector, testLine, ',');

    /// @test  The testString is appropriately segmented into a vector of strings.
    CPPUNIT_ASSERT_MESSAGE("Yankees", "Yankees" == tStringVector.at(0) );
    CPPUNIT_ASSERT_MESSAGE("\"  New York  \"", "\"  New York  \"" == tStringVector.at(1) );
    CPPUNIT_ASSERT_MESSAGE("\'American League\'", "\'American League\'" == tStringVector.at(2) );
    CPPUNIT_ASSERT_MESSAGE("East", "East" == tStringVector.at(3) );
    CPPUNIT_ASSERT_MESSAGE("27", "27" == tStringVector.at(4) );
    CPPUNIT_ASSERT_THROW_MESSAGE("index 5 should be out of range",
            tStringVector.at(5), std::out_of_range );

    /// - Set testLine.
    testLine = "  \"Cubs\" \t Chicago \t National League \t Central";

    /// - Split testLine into a vector of strings.
    ParseTool::splitLineByCharacter(tStringVector, testLine, '\t');

    /// @test  The testString is appropriately segmented into a vector of strings.
    CPPUNIT_ASSERT_MESSAGE("\"Cubs\"", "\"Cubs\"" == tStringVector.at(0) );
    CPPUNIT_ASSERT_MESSAGE("Chicago", "Chicago" == tStringVector.at(1) );
    CPPUNIT_ASSERT_MESSAGE("National League", "National League" == tStringVector.at(2) );
    CPPUNIT_ASSERT_MESSAGE("Central", "Central" == tStringVector.at(3) );
    CPPUNIT_ASSERT_THROW_MESSAGE("index 4 should be out of range",
            tStringVector.at(4), std::out_of_range );

    /// - Try to split by a character that is not found within the string.
    ParseTool::splitLineByCharacter(tStringVector, testLine, ',');

    /// @test  The string vector should only have one member, the entire string.
    CPPUNIT_ASSERT_MESSAGE("split by a non-occurring character", 1 == tStringVector.size());

    std::cout << " Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests parseLines() method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtParseTool::testParseLines()
{
    std::cout << "\n ParseTool 04: Testing parseLines() function ...........................";

    /// - Create null pointers.
    char* nullchar = 0;

    /// @test  Exception thrown on null file pointer
    CPPUNIT_ASSERT_THROW_MESSAGE( "null pointer to fileName",
            ParseTool::parseLines(tStringVector, nullchar), TsParseException);

    /// @test  Exception thrown on file not found.
    CPPUNIT_ASSERT_THROW_MESSAGE( "missing file",
            ParseTool::parseLines(tStringVector,"doesnt_exist.txt"), TsParseException);

    /// - Remove read access to a specific file and directory for testing.
    // TODO commenting out these tests since they cannot be performed correctly in GitLab CI.
    //      We can't get the chmod command to make these files non-readable by the test.  These
    //      inline commands don't work, nor does chmod a-r on these files in the .gitlab-ci.yml.
    //system("chmod -r file_no_permission.txt");
    //system("chmod -r nopermission");
    //
    /// @test  Exception thrown on file without read permission
    //CPPUNIT_ASSERT_THROW_MESSAGE( "file w/ no read access",
    //        ParseTool::parseLines(tStringVector,"file_no_permission.txt"), TsParseException);
    //
    /// @test  Exception thrown on directory without read permission
    //CPPUNIT_ASSERT_THROW_MESSAGE( "directory w/ no read access",
    //        ParseTool::parseLines(tStringVector,
    //                "nopermission/file_within_nonaccessible_directory.txt"), TsParseException);
    //
    /// - Exception should not be thrown on accessible directory even if its within
    ///   a non-accessible directory
    //ParseTool::parseLines(tStringVector,
    //        "nopermission/yespermission/file_within_accessible_directory.txt");
    // test nothing
    //
    /// - Return read-access to test file and directory.
    //system("chmod +r nopermission");
    //system("chmod +r file_no_permission.txt");

    /// - Parse a legitimate file with default arguments.
    ParseTool::parseLines(tStringVector, "file_generic.txt");
    /// @test  Lines are correctly read, ignoring blank lines and whitespace only
    CPPUNIT_ASSERT("# This might be a comment line" == tStringVector.at(0) );
    CPPUNIT_ASSERT("//This might be a comment line" == tStringVector.at(1) );
    CPPUNIT_ASSERT("This is line 1." == tStringVector.at(2) );
    CPPUNIT_ASSERT("\" This is line 2. \"" == tStringVector.at(3) );
    CPPUNIT_ASSERT("This is line 3." == tStringVector.at(4) );
    CPPUNIT_ASSERT("\"\"\" This is line 4. \"\"\"" == tStringVector.at(5) );
    CPPUNIT_ASSERT("This is more than 1/2 of a line." == tStringVector.at(6) );

    /// - Parse a legitimate file, specifying a comment character.
    ParseTool::parseLines(tStringVector, "file_generic.txt", 300, "\"", "#");
    /// @test  Lines are correctly read, ignoring blank lines and whitespace
    CPPUNIT_ASSERT("# This might be a comment line" != tStringVector.at(0) );
    CPPUNIT_ASSERT("//This might be a comment line" == tStringVector.at(0) );
    CPPUNIT_ASSERT("This is line 1." == tStringVector.at(1) );
    CPPUNIT_ASSERT("This is line 2." == tStringVector.at(2) );
    CPPUNIT_ASSERT("This is line 3." == tStringVector.at(3) );
    CPPUNIT_ASSERT("This is line 4." == tStringVector.at(4) );
    CPPUNIT_ASSERT("This is more than 1/2 of a line." == tStringVector.at(5) );

    /// - Parse a legitimate file, specifying a comment characters and remove character.
    ParseTool::parseLines(tStringVector, "file_generic.txt", 300, "\"", "#");
    /// @test  Lines are correctly read, ignoring blank lines and whitespace
    CPPUNIT_ASSERT("# This might be a comment line" != tStringVector.at(0) );
    CPPUNIT_ASSERT("//This might be a comment line" == tStringVector.at(0) );
    CPPUNIT_ASSERT("This is line 1." == tStringVector.at(1) );
    CPPUNIT_ASSERT("This is line 2." == tStringVector.at(2) );
    CPPUNIT_ASSERT("This is line 3." == tStringVector.at(3) );
    CPPUNIT_ASSERT("This is line 4." == tStringVector.at(4) );
    CPPUNIT_ASSERT("This is more than 1/2 of a line." == tStringVector.at(5) );

    /// - Parse a legitimate file, specifying a different comment character.
    ParseTool::parseLines(tStringVector, "file_generic.txt", 300, "\"", "//");
    /// @test  Lines are correctly read, ignoring blank lines and whitespace
    CPPUNIT_ASSERT("# This might be a comment line" == tStringVector.at(0) );
    CPPUNIT_ASSERT("//This might be a comment line" != tStringVector.at(1) );
    CPPUNIT_ASSERT("This is line 1." == tStringVector.at(1) );
    CPPUNIT_ASSERT("This is line 2." == tStringVector.at(2) );
    CPPUNIT_ASSERT("This is line 3." == tStringVector.at(3) );
    CPPUNIT_ASSERT("This is line 4." == tStringVector.at(4) );
    CPPUNIT_ASSERT("This is more than 1/2 of a line." == tStringVector.at(5) );

    /// - Parse a legitimate file, specifying remove character only
    ParseTool::parseLines(tStringVector, "file_generic.txt", 300, "\"");
    /// @test  Lines are correctly read, ignoring blank lines and whitespace
    CPPUNIT_ASSERT("# This might be a comment line" == tStringVector.at(0) );
    CPPUNIT_ASSERT("//This might be a comment line" == tStringVector.at(1) );
    CPPUNIT_ASSERT("This is line 1." == tStringVector.at(2) );
    CPPUNIT_ASSERT("This is line 2." == tStringVector.at(3) );
    CPPUNIT_ASSERT("This is line 3." == tStringVector.at(4) );
    CPPUNIT_ASSERT("This is line 4." == tStringVector.at(5) );
    CPPUNIT_ASSERT("This is more than 1/2 of a line." == tStringVector.at(6) );

    /// @test  Exception thrown on a lineWidth that is too small.
    CPPUNIT_ASSERT_THROW_MESSAGE( "directory w/ no read access",
            ParseTool::parseLines(tStringVector, "file_generic.txt", 10), TsParseException);

    // Print members of vector, for debugging.
//    ParseTool::parseLines(tStringVector, "/users/jvalerio/Desktop/ISS_desktop.png");
//    std::cout << "\nSize of tStringVector: " << tStringVector.size();
//    for (int i = 0; i < tStringVector.size(); ++i)
//    {
//        std::cout << "\n" << tStringVector.at(i);
//    }
//    std::cout << "\n";

    std::cout << " Pass";
}
