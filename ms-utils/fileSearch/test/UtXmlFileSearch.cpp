/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (XmlFileSearch.o)
 )
 PROGRAMMERS:
 (
  ((Jose A. Perez) (L3 Comm) (October 16, 2012) (TS21) (Initial))
 )
 **************************************************************************************************/
#include "UtXmlFileSearch.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsParseException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default class constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtXmlFileSearch::UtXmlFileSearch()
    :
    testArticle      (),
    myFiles          (0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
///////////////////////////////////////////////////////////////////////////////////////////////////
UtXmlFileSearch::~UtXmlFileSearch()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtXmlFileSearch::setUp()
{
    //Nothing to do here.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtXmlFileSearch::tearDown()
{
    //nothing to do here.
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor test.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtXmlFileSearch::DefaultConstructorTest()
{
    std::cout << "\n-------------------------------------------------------------------------------";
    std::cout << "\n Xml File Search: Default Constructor Test ";

    CPPUNIT_ASSERT(0 == testArticle.fileList.size());
    CPPUNIT_ASSERT(std::string("Xml File Search: There were no files found.") == std::string(XmlFileSearch::NO_FILE_MSG));

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test exeption thrown when max number of files is zero.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtXmlFileSearch::TestExceptionThrownWhenMaxFilesIsZero()
{
    std::cout << "\n Xml File Search: Test exception thrown when max number of files is zero.";

    int maxFiles = 0;
    try {
        testArticle.searchDirectory("./fileSearchFiles", maxFiles);
        CPPUNIT_ASSERT_THROW(testArticle.searchDirectory("./fileSearchFiles", maxFiles), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
                "Xml File Search: Max number of files must be greater than zero."), ne.getMessage());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test exeption thrown when file failed to load.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtXmlFileSearch::TestExceptionThrownFileFailedToLoad()
{
    std::cout << "\n Xml File Search: Test exception thrown when file failed to load.";

    try {
        testArticle.searchDirectory("./DoesNotExist");
        CPPUNIT_ASSERT_THROW(testArticle.searchDirectory("./DoesNotExist"), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
                "Xml File Search: Unable to load file, check file, file existence, ect."), ne.getMessage());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test exeption thrown when there were no files found in directory.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtXmlFileSearch::TestExceptionThrownWhenNoFilesFound()
{
    std::cout << "\n Xml File Search: Test exception thrown when no files were found.";

    try {
        testArticle.searchDirectory("./fileSearchFiles/noFiles");
        CPPUNIT_ASSERT_THROW(testArticle.searchDirectory("./fileSearchFiles/noFiles"), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string("Xml File Search: Unable to load file, check file, file existence, ect."),
                                         ne.getMessage());
    }

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test number of files found.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtXmlFileSearch::TestNumberOfFilesFound()
{
    std::cout << "\n Xml File Search: Test number of files found.";

    testArticle.searchDirectory("./fileSearchFiles");
    myFiles = testArticle.getFileList();

    CPPUNIT_ASSERT(2 == myFiles.size());

    std::cout << "\t... Pass";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test max number of files exception thrown.
///////////////////////////////////////////////////////////////////////////////////////////////////
void UtXmlFileSearch::TestMaxNumberOfFilesExceptionThrown()
{
    std::cout << "\n Xml File Search: Test max number of files exception thrown.";

    try {
        testArticle.searchDirectory("./fileSearchFiles/101Files");
        CPPUNIT_ASSERT_THROW(testArticle.searchDirectory("./fileSearchFiles/101Files"), TsInitializationException);
        }
    catch (TsInitializationException &ne)
    {
        //Verify is the correct configuration exception.
        CPPUNIT_ASSERT_EQUAL(std::string(
            "Notice: Have found more than the max number of files limit."), ne.getMessage());
    }

    myFiles = testArticle.getFileList();

    CPPUNIT_ASSERT(101 == myFiles.size());

    std::cout << "\t... Pass";
}
