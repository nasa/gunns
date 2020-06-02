#ifndef UT_TS_hs_EXISTS
#define UT_TS_hs_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class TsHsStringMngr;

/// @brief      Unit test for TS_hs_msg macro and related functions.
/// @deprecated This "C-style" method is being replaced by the C++ class TsHsMsg. Please change all references to TsHsMsg.

class UT_TS_hs: public CppUnit::TestFixture
{
public:

    // Ctor not really needed here, but gets rid of some compiler warnings
    UT_TS_hs() : CppUnit::TestFixture(), mngr(0) {}

    void setUp();
    void tearDown();

    void send();
    void sendMacro();

private:

    TsHsStringMngr* mngr;

    CPPUNIT_TEST_SUITE(UT_TS_hs);

    CPPUNIT_TEST(send);
    CPPUNIT_TEST(sendMacro);

    CPPUNIT_TEST_SUITE_END();

    // Disable these to prevent compiler warnings about them being not implemented
    UT_TS_hs(const UT_TS_hs&);
    const UT_TS_hs& operator=(const UT_TS_hs&);
};

#endif /* UT_TS_hs_EXISTS */
