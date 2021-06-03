#ifndef TsHsMsgQueue_EXISTS
#define TsHsMsgQueue_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_MESSAGE QUEUE Message Queue
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Health and Status message queue class. Allows log messages to be queued by the
   main thread and later output by a helper thread.)

REQUIREMENTS:
- (TBD)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((TsHsMsgQueue.o))

PROGRAMMERS:
- (
    ((Wesley A. White) (Tietronix Software) (June 2013))
  )

@{
*/

#include <cstdlib>
#include <string>
#include <pthread.h>

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "TS_hs_msg_types.h"
#include "simulation/timer/TS_timer.h"
//#include "TS_timer.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Queue node type used by TsHsMsgQueue. Each node represents a request to
///          log a message.
//////////////////////////////////////////////////////////////////////////////////////////////////
class TsHsMsgQueueNode
{
    TS_MAKE_SIM_COMPATIBLE(TsHsMsgQueueNode);
public:
    TsHsMsgQueueNode();
    std::string       mFile;       // ** (--) Name of file logging the message
    int               mLine;       // ** (--) Number of line logging the message
    std::string       mFunction;   // ** (--) Name of function logging the message
    TS_HS_MSG_TYPE    mType;       // ** (--) Severity of message, e.g. warning, error, info, etc.
    std::string       mSubsys;     // ** (--) Subsystem issuing message
    std::string       mText;       // ** (--) The text to log, e.g. description of the error
    TS_TIMER_TYPE     mMet;        // ** (--) MET time related to logging event
    unsigned long     mTimestamp;  // ** (--) Unix time stamp related to logging event
    TsHsMsgQueueNode* mNext;       // ** (--) Pointer to next node
private:
    TsHsMsgQueueNode(const TsHsMsgQueueNode&);
    const TsHsMsgQueueNode& operator=(const TsHsMsgQueueNode&);
};

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   A queue which allows logging input/output to be decoupled from the main thread.
///
/// @details This is a fixed size circular queue. On overflow the queue will overwrite the oldest
///          message. The queue is thread-safe, i.e. protected by a mutex. Inserts can be
///          blocking or non-blocking. If non-blocking, messages will be skipped (not output) if
///          a thread fails to get a lock during the operation. Removals are always
///          blocking, since the main thread is not doing the removing.
//////////////////////////////////////////////////////////////////////////////////////////////////
class TsHsMsgQueue
{
public:
    TS_MAKE_SIM_COMPATIBLE(TsHsMsgQueue);

    /// @brief Default constructor.
    TsHsMsgQueue(void);

    /// @brief Constructor that takes queue size and blocking flag.
    TsHsMsgQueue(int size, bool blocking);

    /// @brief Destructor.
    virtual ~TsHsMsgQueue();

    /// @brief Initializes (or re-initializes) queue size and blocking flag.
    void initialize(int size, bool blocking);

    /// @brief Inserts a message request at the tail of the queue.
    void insertMsg(
            const std::string& file,
            const int          line,
            const std::string& function,
            TS_HS_MSG_TYPE     type,
            const std::string& subsys,
            const std::string& mtext,
            TS_TIMER_TYPE      met,
            unsigned long      timestamp);

    /// @brief Removes a message request from the head of the queue.
    bool removeMsg(TsHsMsgQueueNode& node);

    /// @brief Returns the number of messages skipped due to lock failures.
    int getLockFailures() const { return mTryLockFailures; }

    /// @brief Returns the number of messages overwritten due to overflow.
    int getOverflows() const { return mOverflowCount; }

    /// @brief Returns true if the queue is empty.
    bool isEmpty() const { return mHead == mTail; }

    /// @brief Returns true if the queue is full.
    bool isFull() const { return mTail->mNext == mHead; }

    /// @brief Return the capacity of the queue.
    int getCapacity() const;

    /// @brief Return the current size of the queue.
    int getSize() const;

    /// @brief Allows blocking to be changed after queue is created.
    void setBlocking(bool blocking) { mBlocking = blocking; }

    /// @brief Returns the value of the blocking flag.
    bool getBlocking() const { return mBlocking; }

protected:

    /// @brief Allocates memory for the queue.
    void allocQueue(int size);

    /// @brief Change the queue size while preserving as many outstanding request as possible.
    void reallocQueue(int size);

    /// @brief Make a shallow copy of a queue
    void clone(const TsHsMsgQueue& original);

    /// @brief Inserts a message request at the tail of the queue.
    void insertMsg(TsHsMsgQueueNode& node);

    TsHsMsgQueueNode* mAllocation;        // ** (--) Points to the top of the block of memory allocation.
    int               mAllocatedSize;     // *o (--) trick_chkpnt_io(*io) Number of nodes allocated

    TsHsMsgQueueNode* mTail;              // ** (--) Points to next node to insert
    TsHsMsgQueueNode* mHead;              // ** (--) Points to next node to remove

    pthread_mutex_t   mMutex;             // ** (--) Place holder for mutex

    bool              mBlocking;          //    (--) If the queue is busy, we can choose to block, or we can choose to discard the message.
    int               mTryLockFailures;   // *o (--) trick_chkpnt_io(*io) Number of messages skipped due to mutex conflicts.
    int               mOverflowCount;     // *o (--) trick_chkpnt_io(*io) Number of messages dropped due to queue overflow.

private:
    TsHsMsgQueue(const TsHsMsgQueue&);
    const TsHsMsgQueue& operator=(const TsHsMsgQueue&);
};
/// @}

#endif /* TsHsMsgQueue_EXISTS */
