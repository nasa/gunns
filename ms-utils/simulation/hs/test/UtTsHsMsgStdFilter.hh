/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Unit test class for TsHsMsgStdFilter class)

LIBRARY DEPENDENCY:
(
    (TsHsStringMngr.o)
    (TSHsMsg.o)
    (simulation/hs/TsHsMsgFilter.o)
    (simulation/hs/TsHsMsgStdFilter.o)
)

PROGRAMMERS:
(
    ((Wesley A. White)   (TSI) (August 2011))
)
*******************************************************************************/
#ifndef UtTsHsMsgStdFilter_EXISTS
#define UtTsHsMsgStdFilter_EXISTS

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class TsHsMsgStdFilter;

/// @brief Unit test for TsHsMsgStdFilter, i.e. the health and status message filter
class UtTsHsMsgStdFilter: public CppUnit::TestFixture
{
public:

    // Ctor not really needed here, but gets rid of some compiler warnings
    UtTsHsMsgStdFilter() : CppUnit::TestFixture(), msgFilter(0) {}

    void setUp();
    void tearDown();

    void testConstructors();
    void testSetters();
    void testSettersRuntime();
    void testRedundancyFilter();
    void testTypeFilter();

private:

    CPPUNIT_TEST_SUITE(UtTsHsMsgStdFilter);

    CPPUNIT_TEST(testConstructors);
    CPPUNIT_TEST(testSetters);
    CPPUNIT_TEST(testSettersRuntime);
    CPPUNIT_TEST(testRedundancyFilter);
    CPPUNIT_TEST(testTypeFilter);

    CPPUNIT_TEST_SUITE_END();

    // Data members
    TsHsMsgStdFilter* msgFilter;

    // Disable these to prevent compiler warnings about them being not implemented
    UtTsHsMsgStdFilter(const UtTsHsMsgStdFilter&);
    const UtTsHsMsgStdFilter& operator=(const UtTsHsMsgStdFilter&);
};

#endif /* UtTsHsMsgStdFilter_EXISTS */
