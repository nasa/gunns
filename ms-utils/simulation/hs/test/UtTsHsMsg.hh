#ifndef UtTsHsMsg_EXISTS
#define UtTsHsMsg_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class TsHsStringMngr;

/// @brief  Unit test for TsHsMsg class and related functions.
class UtTsHsMsg: public CppUnit::TestFixture
{
public:

    // Ctor not really needed here, but gets rid of some compiler warnings
    UtTsHsMsg() : CppUnit::TestFixture(), mStringMngr(0) {}

    void setUp();
    void tearDown();

    void testClear();
    void testSend();
    void testSetters();
    void testStreamInsertionOperator();
    void testDisable();
    void testMacroFunctions();
    void testHsRegisterSubsys();

private:

    CPPUNIT_TEST_SUITE(UtTsHsMsg);

    CPPUNIT_TEST(testSend);
    CPPUNIT_TEST(testSetters);
    CPPUNIT_TEST(testClear);
    CPPUNIT_TEST(testStreamInsertionOperator);
    CPPUNIT_TEST(testDisable);
    CPPUNIT_TEST(testMacroFunctions);
    CPPUNIT_TEST(testHsRegisterSubsys);

    CPPUNIT_TEST_SUITE_END();

    // Data Members
    TsHsStringMngr* mStringMngr;

    // Disable these to prevent compiler warnings about them being not implemented
    UtTsHsMsg(const UtTsHsMsg&);
    const UtTsHsMsg& operator=(const UtTsHsMsg&);
};

#endif /* UtTsHsMsg_EXISTS */
