#ifndef UtXmlFileSearch_EXISTS
#define UtXmlFileSearch_EXISTS

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "../XmlFileSearch.hh"
#include <vector>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_FILE_SEARCH_XmlFileSearch    File Search Unit Tests
/// @ingroup   UT_TSM_UTILITIES_FILE_SEARCH
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the XmlFileSearch class. Verify XmlFileSearch's functionality.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from XmlFileSearch and befriend UtUtXmlFileSearch.
///
/// @details  Class derived from the unit under test. Has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members. Reason of doing this is
///           to have access to the private FSW and malfunction parameters of XmlFileSearch
///           class.
///////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyXmlFileSearch: public XmlFileSearch
{
public:
FriendlyXmlFileSearch();
virtual ~FriendlyXmlFileSearch();
friend class UtXmlFileSearch;
};

inline FriendlyXmlFileSearch::FriendlyXmlFileSearch()
    :
    XmlFileSearch()
{
}

inline FriendlyXmlFileSearch::~FriendlyXmlFileSearch()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    XmlFileSearch unit tests.
///
/// @details  This class provides the unit tests for the XmlFileSearch class with
///           CPPUnit framework.
///////////////////////////////////////////////////////////////////////////////////////////////////
class UtXmlFileSearch : public CppUnit::TestFixture {

public:
    UtXmlFileSearch();
    virtual ~UtXmlFileSearch();

    void setUp();
    void tearDown();

    void DefaultConstructorTest();
    void TestExceptionThrownWhenMaxFilesIsZero();
    void TestExceptionThrownFileFailedToLoad();
    void TestExceptionThrownWhenNoFilesFound();
    void TestNumberOfFilesFound();
    void TestMaxNumberOfFilesExceptionThrown();

protected:
    //No protected data.

private:
    FriendlyXmlFileSearch testArticle;
    std::vector<std::string> myFiles;

    CPPUNIT_TEST_SUITE(UtXmlFileSearch);

    CPPUNIT_TEST(DefaultConstructorTest);
    CPPUNIT_TEST(TestExceptionThrownWhenMaxFilesIsZero);
    CPPUNIT_TEST(TestExceptionThrownFileFailedToLoad);
    CPPUNIT_TEST(TestExceptionThrownWhenNoFilesFound);
    CPPUNIT_TEST(TestNumberOfFilesFound);
    CPPUNIT_TEST(TestMaxNumberOfFilesExceptionThrown);

    CPPUNIT_TEST_SUITE_END();

    /// @brief keep private, never used
    UtXmlFileSearch(const UtXmlFileSearch &rhs);

    /// @brief keep private, never used
    UtXmlFileSearch& operator= (const UtXmlFileSearch &rhs);
};
/// @}


#endif //UtXmlFileSearch_EXISTS
