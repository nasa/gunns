/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (Message queue to support HS message thread. Offloads logging I/O from main thread.)


REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ()

PROGRAMMERS:
   (
    ((Wesley A. White) (Tietronix Software) (June 2013))
   )
*/

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <execinfo.h>
#include <pthread.h>

#include "TsHsMsgQueue.hh"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsgQueueNode::TsHsMsgQueueNode() :
    mFile(),
    mLine(0),
    mFunction(),
    mType(TS_HS_DEBUG),
    mSubsys(TS_HS_UNKNOWN),
    mText(),
    mMet(),
    mTimestamp(0L),
    mNext(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsgQueue::TsHsMsgQueue(void) :
    mAllocation(0),
    mAllocatedSize(0),
    mTail(0),
    mHead(0),
    mMutex(),
    mBlocking(false),
    mTryLockFailures(0),
    mOverflowCount(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Constructor that takes queue size and blocking flag.
///
/// @param[in] size     (--) The maximum number of messages the queue can hold before overflow occurs.
/// @param[in] blocking (--) True if user is willing to allow insertMsg to block.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsgQueue::TsHsMsgQueue(int size, bool blocking) :
    mAllocation(0),
    mAllocatedSize(0),
    mTail(0),
    mHead(0),
    mMutex(),
    mBlocking(blocking),
    mTryLockFailures(0),
    mOverflowCount(0)
{
    pthread_mutex_init(&mMutex, NULL);
    allocQueue(size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Destructor. Frees queue memory.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsgQueue::~TsHsMsgQueue()
{
    delete [] mAllocation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Return the capacity of the queue.
////////////////////////////////////////////////////////////////////////////////////////////////////
int TsHsMsgQueue::getCapacity() const
{
    if (mAllocatedSize == 0)
        return 0;

    // The tail node is never used to store data.
    return mAllocatedSize-1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Return the current size of the queue.
////////////////////////////////////////////////////////////////////////////////////////////////////
int TsHsMsgQueue::getSize() const
{
    if (mHead == 0 || mTail == 0 || mAllocatedSize == 0)
       return 0;

    if (mHead <= mTail)
       return mTail-mHead;
    else
       return mAllocatedSize - (mHead-mTail);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Initializes (or re-initializes) the queue.
///
/// @param[in] size     (--) The maximum number of messages the queue can hold before overflow occurs.
/// @param[in] blocking (--) True if user is willing to allow insertMsg to block.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgQueue::initialize(int size, bool blocking)
{
    mBlocking = blocking;
    reallocQueue(size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Inserts a message request at the tail of the queue. If queue overflows, the oldest
///            message will be silently overwritten. A counter is kept to record how many time
///            this happens. Inserts can be blocking or not. If not blocking then messages will
///            be dropped if a lock cannot be achieved.
///
/// @param[in] file       (--) Source file name invoking 'send'; typically __FILE__ macro is used.
/// @param[in] line       (--) Line in file invoking 'send'; typically __LINE__ macro is used.
/// @param[in] function   (--) The name of the function logging the message; typically __func__.
/// @param[in] type       (--) Enumeration constant that represents the type of message.
/// @param[in] subsys     (--) The subsystem that is logging the message.
/// @param[in] mtext      (--) The message text.
/// @param[in] met        (--) The mission elapsed time.
/// @param[in] timestamp  (--) The Unix timestamp.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgQueue::insertMsg(
    const std::string& file,
    const int          line,
    const std::string& function,
    TS_HS_MSG_TYPE     type,
    const std::string& subsys,
    const std::string& mtext,
    TS_TIMER_TYPE      met,
    unsigned long      timestamp)
{
    if (mBlocking)
    {
        pthread_mutex_lock(&mMutex);
    }
    else
    {
        if (pthread_mutex_trylock(&mMutex) != 0) // non-0 means lock failed
        {
            mTryLockFailures++;
            return;
        }
    }

    // Insert message at tail of queue
    mTail->mFile      = file;
    mTail->mLine      = line;
    mTail->mFunction  = function;
    mTail->mType      = type;
    mTail->mSubsys    = subsys;
    mTail->mText      = mtext;
    mTail->mMet       = met;
    mTail->mTimestamp = timestamp;

    // If we have filled the queue, overwrite the oldest node by moving the read pointer
    if (isFull())
    {
        //cout << "overwriting message '" << mtext << "'" << endl;
        mHead = mHead->mNext;
        mOverflowCount++;
    }
    //else
    //    cout << "inserting message '" << mtext << "'" << endl;

    // Update the tail pointer
    mTail = mTail->mNext;

    pthread_mutex_unlock(&mMutex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Inserts a message request at the tail of the queue. If queue overflows, the oldest
///            message will be silently overwritten. A counter is kept to record how many time
///            this happens. Inserts can be blocking or not. If not blocking then messages will
///            be dropped if a lock cannot be achieved.
///
/// @param[in] node     (--) node to add to queue.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgQueue::insertMsg(TsHsMsgQueueNode& node)
{
    insertMsg(node.mFile, node.mLine, node.mFunction, node.mType, node.mSubsys, node.mText, node.mMet, node.mTimestamp);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Removes a message request from the head of the queue and places it in node. If
///            the queue is empty, false is return and the contents of node are undefined.
///
/// @param[out] node   (--) The message request info from the head of the queue is returned in this.
///
/// @return Returns true if successful, false if queue was empty.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsMsgQueue::removeMsg(TsHsMsgQueueNode& node)
{
    // Return false if the the queue is empty
    if (isEmpty())
        return false;

    pthread_mutex_lock(&mMutex);

    // Copy message at head of queue to return node
    node.mFile      = mHead->mFile;
    node.mLine      = mHead->mLine;
    node.mFunction  = mHead->mFunction;
    node.mType      = mHead->mType;
    node.mSubsys    = mHead->mSubsys;
    node.mText      = mHead->mText;
    node.mMet       = mHead->mMet;
    node.mTimestamp = mHead->mTimestamp;

    // Update the head pointer
    mHead = mHead->mNext;

    pthread_mutex_unlock(&mMutex);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Allocates memory for the queue. If size is <1, a queue big enough to hold 1 message
///            will be allocated.
///
/// @param[in] size     (--) The maximum number of messages the queue can hold before overflow occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgQueue::allocQueue(int size)
{
    // Internal size is one-greater than user-requested size. Minimum internal size is 2.
    int new_allocated_size = max(size+1, 2);

    // Don't re-allocate a queue that's the same size as the original, just reuse the original.
    if (new_allocated_size == mAllocatedSize)
        return;

    mAllocatedSize = new_allocated_size;

    // If this is a re-allocation, then clean up original first
    delete [] mAllocation;

    //cout << "allocating queue physical size= " << mAllocatedSize << endl;

    // Allocate memory, hook up nodes, and set up head/tail pointers
    mTail = mAllocation = new TsHsMsgQueueNode[mAllocatedSize];

    for (int i = 0 ; i < mAllocatedSize-1 ; i++)
    {
        mTail[i].mNext = &mTail[i+1];
    }
    mTail[mAllocatedSize-1].mNext = mTail;
    mHead = mTail;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Reallocates memory for the queue. Copies outstanding request from original queue to
///            resized queue. If resized queue is smaller, older request will be discarded.
///
/// @param[in] size     (--) The maximum number of messages the queue can hold before overflow occurs.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgQueue::reallocQueue(int size)
{
    // If there is no previous queue, then this is not a re-alloc. Just alloc instead.
    if (mAllocatedSize == 0)
    {
        allocQueue(size);
        return;
    }

    // Internal size is one-greater than user-requested size. Minimum internal size is 2.
    int new_allocated_size = max(size+1, 2);

    // If the queue is already the requested size, use the existing queue.
    if (new_allocated_size == mAllocatedSize)
        return;

    // Make a clone of the original queue
    TsHsMsgQueue temp_queue;
    temp_queue.clone(*this);

    // Transform the original queue into a new queue, preserving most of the non-pointer fields.
    mAllocation      = 0;
    mAllocatedSize   = 0;
    mTail            = 0;
    mHead            = 0;
    allocQueue(size);

    //cout << "reallocating queue physical size= " << mAllocatedSize << endl;

    // Copy request from old queue to new queue. If new queue is smaller, then older request
    // will be discarded when queue wraps.

    TsHsMsgQueueNode node;
    while (temp_queue.removeMsg(node) == true)
    {
        insertMsg(node);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Make a shallow copy of a queue
/// @param[in] original     (--) The source queue to clone from.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgQueue::clone(const TsHsMsgQueue& original)
{
    mAllocation      = original.mAllocation;
    mAllocatedSize   = original.mAllocatedSize;
    mTail            = original.mTail;
    mHead            = original.mHead;
    mMutex           = original.mMutex;
    mBlocking        = original.mBlocking;
    mTryLockFailures = original.mTryLockFailures;
    mOverflowCount   = original.mOverflowCount;
}

