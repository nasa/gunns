#ifndef UtTsHsMsgWrapper_EXISTS
#define UtTsHsMsgWrapper_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class TsHsStringMngr;

/// @brief  Unit test for TsHsMsg class and related functions.
class UtTsHsMsgWrapper: public CppUnit::TestFixture
{
public:

    // Ctor not really needed here, but gets rid of some compiler warnings
    UtTsHsMsgWrapper() : CppUnit::TestFixture(), mStringMngr(0) {}

    void setUp();
    void tearDown();

    void testMacroFunctions();

private:

    CPPUNIT_TEST_SUITE(UtTsHsMsgWrapper);
    CPPUNIT_TEST(testMacroFunctions);
    CPPUNIT_TEST_SUITE_END();

    // Data Members
    TsHsStringMngr* mStringMngr;

    // Disable these to prevent compiler warnings about them being not implemented
    UtTsHsMsgWrapper(const UtTsHsMsgWrapper&);
    const UtTsHsMsgWrapper& operator=(const UtTsHsMsgWrapper&);
};

#endif /* UtTsHsMsgWrapper_EXISTS */
