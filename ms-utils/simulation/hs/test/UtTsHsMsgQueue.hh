/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Unit test class for TsHsMsgQueue class)

LIBRARY DEPENDENCY:
(
    (simulation/hs/TsHsMsgQueue.o)
)

PROGRAMMERS:
(
    ((Wesley A. White)   (TSI) (June 2013))
)
*******************************************************************************/
#ifndef UtTsHsMsgQueue_EXISTS
#define UtTsHsMsgQueue_EXISTS

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "TsHsMsgQueue.hh"

//class TsHsMsgQueue;

/// @brief Unit test for TsHsMsgQueue, i.e. the health and status message filter
class UtTsHsMsgQueue: public CppUnit::TestFixture
{
public:

    // Ctor not really needed here, but gets rid of some compiler warnings
    UtTsHsMsgQueue() : CppUnit::TestFixture(), msgQueue(0) {}

    void setUp();
    void tearDown();

    void testConstructors();
    void testSetters();
    void testOrdinaryOverflow();
    void testQueueReinitializationDownsize();
    void testQueueReinitializationUpsize();

private:

    CPPUNIT_TEST_SUITE(UtTsHsMsgQueue);

    CPPUNIT_TEST(testConstructors);
    CPPUNIT_TEST(testSetters);
    CPPUNIT_TEST(testOrdinaryOverflow);
    CPPUNIT_TEST(testQueueReinitializationDownsize);
    CPPUNIT_TEST(testQueueReinitializationUpsize);

    CPPUNIT_TEST_SUITE_END();

    // Data members
    TsHsMsgQueue* msgQueue;

    // Disable these to prevent compiler warnings about them being not implemented
    UtTsHsMsgQueue(const UtTsHsMsgQueue&);
    const UtTsHsMsgQueue& operator=(const UtTsHsMsgQueue&);
};

#endif /* UtTsHsMsgQueue_EXISTS */
