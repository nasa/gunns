/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Unit test class for TsHsMsgStdFilter class)

LIBRARY DEPENDENCY:
(
    (TsHsStringMngr.o)
    (TsHsMsg.o)
    (simulation/hs/TsHsMsgFilter.o)
    (simulation/hs/TsHsMsgStdFilter.o)
)

PROGRAMMERS:
(
    ((Wesley A. White)   (TSI) (August 2011))
)
*******************************************************************************/

#include "UtTsHsMsgStdFilter.hh"
#include "TsHsMsgStdFilter.hh"

#define DEFAULT_INTERVAL 60
#define DEFAULT_COUNT     2
#define DEFAULT_HISTORY  25

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Required by cppunit. Called at the beginning of each testXXX method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgStdFilter::setUp()
{
    // Reset for each test group
    msgFilter = new TsHsMsgStdFilter();

    // Initialize filter
    msgFilter->init();

    // Restart
    msgFilter->restart();

    // Purge messages -- for coverage
    //msgFilter->purgeMessages(); // protected method would require friend class to test

    // Set blocking -- both ways for coverage
    msgFilter->setBlocking(true);
    msgFilter->setBlocking(false);

    // The ctor defaults these to off. Turn them on for unit testing.
    msgFilter->setTypeFilter(TS_HS_DEBUG, false);
    msgFilter->setTypeFilter(TS_HS_INFO, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Required by cppunit. Called at the end of each testXXX method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgStdFilter::tearDown()
{
    delete msgFilter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test construction of the message filter object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgStdFilter::testConstructors()
{
    CPPUNIT_ASSERT_EQUAL(true,             msgFilter->isEnabled());
    CPPUNIT_ASSERT_EQUAL(0,                msgFilter->getMessagesProcessed());
    CPPUNIT_ASSERT_EQUAL(0,                msgFilter->getMessagesSuppressed());

    CPPUNIT_ASSERT_EQUAL(DEFAULT_INTERVAL, msgFilter->getInterval());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_COUNT,    msgFilter->getCount());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_HISTORY,  msgFilter->getHistory());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test message filter set functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgStdFilter::testSetters()
{
    // Set values out of range
    msgFilter->setInterval(0);
    msgFilter->setCount(0);
    msgFilter->setHistory(0);

    CPPUNIT_ASSERT_EQUAL(DEFAULT_INTERVAL, msgFilter->getInterval());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_COUNT,    msgFilter->getCount());
    CPPUNIT_ASSERT_EQUAL(DEFAULT_HISTORY,  msgFilter->getHistory());

    // Set values out of range - these will be ignored
    msgFilter->setInterval(-999);
    msgFilter->setCount(-999);
    msgFilter->setHistory(-999);

    // Set values in range
    msgFilter->setInterval(3);
    msgFilter->setCount(2);
    msgFilter->setHistory(10);

    CPPUNIT_ASSERT_EQUAL( 3, msgFilter->getInterval());
    CPPUNIT_ASSERT_EQUAL( 2, msgFilter->getCount());
    CPPUNIT_ASSERT_EQUAL(10, msgFilter->getHistory());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test message filter set functions when they are used to change settings dynamically
///          during run-time (as opposed to during initialization). This test simulates how
///          settings could be modified while the simulation is running via tools like trick_tv.
///
/// @notes   This test is not even close to being exhaustive. The filter is somewhat of a
///          state machine with a large amount of state information and several current settings
///          all affecting the filtering outcome. This test amounts to a "spot check" verifying
///          correct outcome given typical usage.
///
/// @note    There is no "update" function for the message filter. Changes to settings take effect
///          each time a message is logged. Message filter data structures are pruned/grown
///          each time a message is logged. Pruning is opportunistic. The entire data structure
///          is not pruned with each message; only parts of the data structure related to the
///          processing of the message are pruned. In other words, the data structure will not
///          immediately shrink to its minimum size during the processing of a single message.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgStdFilter::testSettersRuntime()
{
    // Initial settings

    msgFilter->setInterval(10); // Large 10s interval
    msgFilter->setCount(1);     // No duplicates allowed
    msgFilter->setHistory(3);   // 3 queues

    msgFilter->shouldSendMessage(  1.00, "file", 123, TS_HS_DEBUG, "subsys", "C"); // added to C queue
    msgFilter->shouldSendMessage(  1.05, "file", 123, TS_HS_DEBUG, "subsys", "A"); // added to A queue
    msgFilter->shouldSendMessage(  1.10, "file", 123, TS_HS_DEBUG, "subsys", "B"); // added to B queue
    msgFilter->shouldSendMessage(  1.20, "file", 123, TS_HS_DEBUG, "subsys", "A"); // X
    msgFilter->shouldSendMessage(  1.30, "file", 123, TS_HS_DEBUG, "subsys", "B"); // X
    msgFilter->shouldSendMessage(  1.40, "file", 123, TS_HS_DEBUG, "subsys", "A"); // X

    CPPUNIT_ASSERT_EQUAL(6, msgFilter->getMessagesProcessed());
    CPPUNIT_ASSERT_EQUAL(3, msgFilter->getMessagesSuppressed());

    // New settings

    msgFilter->setInterval(1); // This will cause the B=1.30 and A=1.40 to be purged from the queue
    msgFilter->setCount(2);    // Now allowing one duplicate
    msgFilter->setHistory(2);  // This will cause the C queue to be bumped

    msgFilter->shouldSendMessage(  6.00, "file", 123, TS_HS_DEBUG, "subsys", "D"); // C queue replaced by D
    msgFilter->shouldSendMessage(  6.10, "file", 123, TS_HS_DEBUG, "subsys", "B"); // B=1.4 purged B=6.1 added
    msgFilter->shouldSendMessage(  6.20, "file", 123, TS_HS_DEBUG, "subsys", "A"); // A=1.4 purged A=6.2 added
    msgFilter->shouldSendMessage(  6.30, "file", 123, TS_HS_DEBUG, "subsys", "B"); // added to B queue
    msgFilter->shouldSendMessage(  6.40, "file", 123, TS_HS_DEBUG, "subsys", "A"); // added to A queue
    msgFilter->shouldSendMessage(  6.50, "file", 123, TS_HS_DEBUG, "subsys", "C"); // D queue replaced by C

    CPPUNIT_ASSERT_EQUAL(12, msgFilter->getMessagesProcessed());
    CPPUNIT_ASSERT_EQUAL(3, msgFilter->getMessagesSuppressed());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test message filter's redundancy filtering algorithm.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgStdFilter::testRedundancyFilter()
{
    // Set values in range
    msgFilter->setInterval(3);
    msgFilter->setCount(2);
    msgFilter->setHistory(10);

    // Show that when count equals or exceeds the limit (2) within the
    // interval (3), duplicates will be suppressed.
    msgFilter->shouldSendMessage(  1.00, "file", 123, TS_HS_DEBUG, "subsys", "A");
    msgFilter->shouldSendMessage(  1.10, "file", 123, TS_HS_DEBUG, "subsys", "B");
    msgFilter->shouldSendMessage(  1.20, "file", 123, TS_HS_DEBUG, "subsys", "A");
    msgFilter->shouldSendMessage(  1.30, "file", 123, TS_HS_DEBUG, "subsys", "C");
    msgFilter->shouldSendMessage(  1.40, "file", 123, TS_HS_DEBUG, "subsys", "A");

    CPPUNIT_ASSERT_EQUAL(5, msgFilter->getMessagesProcessed());
    CPPUNIT_ASSERT_EQUAL(1, msgFilter->getMessagesSuppressed());

    // Show that duplicates occurring in different intervals
    // are passed through.
    msgFilter->shouldSendMessage(  5.00, "file", 123, TS_HS_DEBUG, "subsys", "A");
    msgFilter->shouldSendMessage( 10.00, "file", 123, TS_HS_DEBUG, "subsys", "A");
    msgFilter->shouldSendMessage( 11.00, "file", 123, TS_HS_DEBUG, "subsys", "A");

    CPPUNIT_ASSERT_EQUAL(8, msgFilter->getMessagesProcessed());
    CPPUNIT_ASSERT_EQUAL(1, msgFilter->getMessagesSuppressed());

    // Show that the filter can be disabled
    msgFilter->setEnabled(false);

    msgFilter->shouldSendMessage( 11.00, "file", 123, TS_HS_DEBUG, "subsys", "A");
    msgFilter->shouldSendMessage( 11.01, "file", 123, TS_HS_DEBUG, "subsys", "A");
    msgFilter->shouldSendMessage( 11.02, "file", 123, TS_HS_DEBUG, "subsys", "A");

    CPPUNIT_ASSERT_EQUAL(8, msgFilter->getMessagesProcessed());
    CPPUNIT_ASSERT_EQUAL(1, msgFilter->getMessagesSuppressed());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test message filter's type filtering algorithm.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgStdFilter::testTypeFilter()
{
    // Ensure redundancy filter won't interfere
    msgFilter->setInterval(1);
    msgFilter->setCount(11);
    msgFilter->setHistory(1);

    // Filter out debug and info messages
    msgFilter->setTypeFilter(TS_HS_DEBUG, true);
    msgFilter->setTypeFilter(TS_HS_INFO, true);

    // Attempt to send some messages of each type
    msgFilter->shouldSendMessage(  1.00, "file", 123, TS_HS_DEBUG,   "subsys", "A"); // 1 x
    msgFilter->shouldSendMessage(  1.01, "file", 123, TS_HS_DEBUG,   "subsys", "A"); // 2 x
    msgFilter->shouldSendMessage(  1.02, "file", 123, TS_HS_DEBUG,   "subsys", "A"); // 3 x
    msgFilter->shouldSendMessage(  1.10, "file", 123, TS_HS_INFO,    "subsys", "A"); // 4 x
    msgFilter->shouldSendMessage(  1.11, "file", 123, TS_HS_INFO,    "subsys", "A"); // 5 x
    msgFilter->shouldSendMessage(  1.12, "file", 123, TS_HS_INFO,    "subsys", "A"); // 6 x
    msgFilter->shouldSendMessage(  1.13, "file", 123, TS_HS_INFO,    "subsys", "A"); // 7 x
    msgFilter->shouldSendMessage(  1.20, "file", 123, TS_HS_WARNING, "subsys", "A"); // 8
    msgFilter->shouldSendMessage(  1.30, "file", 123, TS_HS_ERROR,   "subsys", "A"); // 9
    msgFilter->shouldSendMessage(  1.40, "file", 123, TS_HS_FATAL,   "subsys", "A"); // 10

    // Seven of the 10 messages should have been suppressed
    CPPUNIT_ASSERT_EQUAL(10, msgFilter->getMessagesProcessed());
    CPPUNIT_ASSERT_EQUAL( 7, msgFilter->getMessagesSuppressed());

    // Make sure we can turn this thing back off!
    msgFilter->setTypeFilter(TS_HS_DEBUG, false);
    msgFilter->setTypeFilter(TS_HS_INFO, false);

    // Attempt to send some messages of each type
    msgFilter->shouldSendMessage(  2.00, "file", 123, TS_HS_DEBUG,   "subsys", "A"); // 1
    msgFilter->shouldSendMessage(  2.01, "file", 123, TS_HS_DEBUG,   "subsys", "A"); // 2
    msgFilter->shouldSendMessage(  2.02, "file", 123, TS_HS_DEBUG,   "subsys", "A"); // 3
    msgFilter->shouldSendMessage(  2.10, "file", 123, TS_HS_INFO,    "subsys", "A"); // 4
    msgFilter->shouldSendMessage(  2.11, "file", 123, TS_HS_INFO,    "subsys", "A"); // 5
    msgFilter->shouldSendMessage(  2.12, "file", 123, TS_HS_INFO,    "subsys", "A"); // 6
    msgFilter->shouldSendMessage(  2.13, "file", 123, TS_HS_INFO,    "subsys", "A"); // 7
    msgFilter->shouldSendMessage(  2.20, "file", 123, TS_HS_WARNING, "subsys", "A"); // 8
    msgFilter->shouldSendMessage(  2.30, "file", 123, TS_HS_ERROR,   "subsys", "A"); // 9
    msgFilter->shouldSendMessage(  2.40, "file", 123, TS_HS_FATAL,   "subsys", "A"); // 10

    // Seven of the 10 messages should have been suppressed
    CPPUNIT_ASSERT_EQUAL(20, msgFilter->getMessagesProcessed());
    CPPUNIT_ASSERT_EQUAL( 7, msgFilter->getMessagesSuppressed());

}

