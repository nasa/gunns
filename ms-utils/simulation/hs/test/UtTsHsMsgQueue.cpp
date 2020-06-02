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

#include <string>
#include "UtTsHsMsgQueue.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Required by cppunit. Called at the beginning of each testXXX method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgQueue::setUp()
{
    // Initially start with 5 nodes and no blocking.
    msgQueue = new TsHsMsgQueue(5, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Required by cppunit. Called at the end of each testXXX method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgQueue::tearDown()
{
    delete msgQueue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test construction of the message filter object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgQueue::testConstructors()
{
    CPPUNIT_ASSERT_EQUAL(false,    msgQueue->getBlocking());
    CPPUNIT_ASSERT_EQUAL(0,        msgQueue->getLockFailures());
    CPPUNIT_ASSERT_EQUAL(0,        msgQueue->getOverflows());
    CPPUNIT_ASSERT_EQUAL(true,     msgQueue->isEmpty());
    CPPUNIT_ASSERT_EQUAL(false,    msgQueue->isFull());
    CPPUNIT_ASSERT_EQUAL(0,        msgQueue->getSize());
    CPPUNIT_ASSERT_EQUAL(5,        msgQueue->getCapacity());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test message filter set functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgQueue::testSetters()
{
    // Set blocking
    msgQueue->setBlocking(true);
    CPPUNIT_ASSERT_EQUAL(true,  msgQueue->getBlocking());

    // Set blocking
    msgQueue->setBlocking(false);
    CPPUNIT_ASSERT_EQUAL(false,  msgQueue->getBlocking());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details See what happens when the number of nodes added to the queue exceeds the queue's
///          capacity. What should happen is that the older nodes are dropped to make room for the
///          new nodes to be inserted.
///
/// @notes   xxx
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgQueue::testOrdinaryOverflow()
{
    TS_TIMER_TYPE met;
    unsigned long timestamp;

    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 1", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 2", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 3", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 4", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 5", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 6", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 7", met, timestamp);

    CPPUNIT_ASSERT_EQUAL(2, msgQueue->getOverflows());

    TsHsMsgQueueNode node;
    msgQueue->removeMsg(node);

    CPPUNIT_ASSERT_EQUAL(true,  std::string("this is message 3") == node.mText );
    CPPUNIT_ASSERT_EQUAL(true,  msgQueue->removeMsg(node) ); // 4
    CPPUNIT_ASSERT_EQUAL(true,  msgQueue->removeMsg(node) ); // 5
    CPPUNIT_ASSERT_EQUAL(true,  msgQueue->removeMsg(node) ); // 6
    CPPUNIT_ASSERT_EQUAL(true,  msgQueue->removeMsg(node) ); // 7
    CPPUNIT_ASSERT_EQUAL(false, msgQueue->removeMsg(node) ); // empty remove fails
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details xxx
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgQueue::testQueueReinitializationDownsize()
{
    TS_TIMER_TYPE met;
    unsigned long timestamp;

    CPPUNIT_ASSERT_EQUAL(5, msgQueue->getCapacity());
    CPPUNIT_ASSERT_EQUAL(0, msgQueue->getSize());
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 1", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 2", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 3", met, timestamp);

    // No overflows, still have 2 nodes available
    CPPUNIT_ASSERT_EQUAL(0, msgQueue->getOverflows());

    // Downsizing from capacity 5 to capacity 2 his will drop message 1
    msgQueue->initialize(2, false);
    CPPUNIT_ASSERT_EQUAL(1, msgQueue->getOverflows());
    CPPUNIT_ASSERT_EQUAL(2, msgQueue->getSize());

    // This will drop message 2
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 4", met, timestamp);
    CPPUNIT_ASSERT_EQUAL(2, msgQueue->getOverflows());

    TsHsMsgQueueNode node;

    msgQueue->removeMsg(node);
    CPPUNIT_ASSERT_EQUAL(true,  std::string("this is message 3") == node.mText );
    msgQueue->removeMsg(node);
    CPPUNIT_ASSERT_EQUAL(true,  std::string("this is message 4") == node.mText );
    CPPUNIT_ASSERT_EQUAL(false, msgQueue->removeMsg(node) ); // empty remove fails
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details xxx
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsHsMsgQueue::testQueueReinitializationUpsize()
{
    TS_TIMER_TYPE met;
    unsigned long timestamp;

    // Downsize, then upsize
    CPPUNIT_ASSERT_EQUAL(5, msgQueue->getCapacity());
    msgQueue->initialize(2, false);
    CPPUNIT_ASSERT_EQUAL(2, msgQueue->getCapacity());

    // Overfill the queue to see that the downsize really worked. Message 1 will be dropped.
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 1", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 2", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 3", met, timestamp);
    CPPUNIT_ASSERT_EQUAL(1, msgQueue->getOverflows());

    // Overfill the new bigger queue, to see that the upsize really worked. Message 2 will be dropped.
    msgQueue->initialize(4, false);
    CPPUNIT_ASSERT_EQUAL(4, msgQueue->getCapacity());
    CPPUNIT_ASSERT_EQUAL(2, msgQueue->getSize());
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 4", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 5", met, timestamp);
    msgQueue->insertMsg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_GENERIC, "this is message 6", met, timestamp);
    CPPUNIT_ASSERT_EQUAL(2, msgQueue->getOverflows());

    TsHsMsgQueueNode node;

    msgQueue->removeMsg(node);
    CPPUNIT_ASSERT_EQUAL(true,  std::string("this is message 3") == node.mText );
    msgQueue->removeMsg(node);
    CPPUNIT_ASSERT_EQUAL(true,  std::string("this is message 4") == node.mText );
    msgQueue->removeMsg(node);
    CPPUNIT_ASSERT_EQUAL(true,  std::string("this is message 5") == node.mText );
    msgQueue->removeMsg(node);
    CPPUNIT_ASSERT_EQUAL(true,  std::string("this is message 6") == node.mText );
    CPPUNIT_ASSERT_EQUAL(false, msgQueue->removeMsg(node) ); // empty remove fails
}



