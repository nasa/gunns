/****************************************** TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Extends the TsHsMsgFilter class and implements its shouldSendMessage filter method.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (simulation/hs/TsHsMsgStdFilter.o)
    (simulation/hs/TsHsMsgFilter.o)
   )

PROGRAMMERS:
   (((Wesley A. White) (Tietronix Software) (August 2011)))
**************************************************************************************************/
#include <iostream>
#include <sstream>

#include "sim_services/Message/include/message_proto.h"
#include "TsHsMsgStdFilter.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsgStdFilter::TsHsMsgStdFilter() :
    mMessageMap(),
    mTryLockFailures(0),
    mResourceLock()
{
    pthread_mutex_init(&mResourceLock, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Reinitialize the message filter after a restart. The data structure used to
///            store the message history is too complicated for trick to checkpoint. So we
///            reinitialize here by clearing the history. This means we might get a few more
///            redundant messages until the message history repopulates.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgStdFilter::restart()
{
    // Clear the message history
    if (mMessageMap.size() > 0)
    {
        mMessageMap.clear();
    }

    // Reset try-lock failure count. Maybe we should checkpoint and restore this value?
    mTryLockFailures = 0;

    // Reset the mutex. Not sure if this gains us anything. The checkpoint wouldn't have been cut until
    // all the model threads were in freeze, i.e. all HS clients had released their resource locks.
    pthread_mutex_destroy(&mResourceLock);
    pthread_mutex_init(&mResourceLock, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsgStdFilter::~TsHsMsgStdFilter()
{
    // Empty
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details       Removes times from the deque which precede the current interval of interest.
///
/// @param[in,out] times         (--) The deque containing times that a particular message occurred.
/// @param[in]     intervalStart (--) We are not interested in times earlier that this.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgStdFilter::purgeTimes(TsHsTimesDeque& times, double intervalStart)
{
    while (!times.empty() && times.front() < intervalStart)
    {
        times.pop_front();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Finds and removes the oldest message in the map.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgStdFilter::purgeMessages()
{
    if (mMessageMap.size() == 0)
    {
        return;
    }

    TsHsMessageMap::iterator oldest = mMessageMap.begin();

    for (TsHsMessageMap::iterator iter = ++oldest; iter != mMessageMap.end(); iter++)
    {
        if ((*iter).second.back() < (*oldest).second.back())
        {
            oldest = iter;
        }
    }

    mMessageMap.erase(oldest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Compares a message with previous ones to see the message should be suppressed or not.
///            The assumption is that the timestamp also represents the current time.
///
/// @param[in] timestamp (--) Time that message was received by filter. The MET "seconds" field
///                           of a TS_TIMER_TYPE structure which is created by the HS sim object.
/// @param[in] file      (--) Source file name invoking 'send'; typically __FILE__ macro is used.
/// @param[in] line      (--) Line in file invoking 'send'; typically __LINE__ macro is used.
/// @param[in] type      (--) Enumeration constant that represents the type of message.
/// @param[in] subsys    (--) The subsystem that is logging the message.
/// @param[in] mtext     (--) The message text.
///
/// @return    true if message should be displayed; false if message should be suppressed.
///
/// @note      The algorithm expects that in consecutive calls to this method, the value of timestamp
///            is increasing. The algorithm will not function correctly if time is moving backwards.
////////////////////////////////////////////////////////////////////////////////////////////////////

bool TsHsMsgStdFilter::shouldSendMessage(
        const double       timestamp,
        const std::string& file,
        const int          line,
        TS_HS_MSG_TYPE     type,
        const std::string& subsys,
        const std::string& mtext
        )
{
    typedef std::pair< std::string,TsHsTimesDeque > TsHsMessageMapPair;

    if (!mEnabled)
    {
        return true;
    }

    if (mBlocking)
    {
        // Wait for resource
        if (pthread_mutex_lock(&mResourceLock) != 0) // non-0 means lock failed
        {
            // This should never happen. If we wait, we should eventually get the lock.
            message_publish(MSG_ERROR, "TsHsMsgStdFilter::shouldSendMessage failed to get resource lock\n");
        }
    }
    else
    {
        // Skip logging the message if there's a mutex conflict
        if (pthread_mutex_trylock(&mResourceLock) != 0) // non-0 means lock failed
        {
            ++mTryLockFailures;
            return false;
        }
    }

    ++mMessagesProcessed;

    // If the message we are attempting to log is of a type that has been categorically filtering out, then
    // suppress the message.
    if (isTypeFiltered(type))
    {
        ++mMessagesSuppressed;
        pthread_mutex_unlock(&mResourceLock);
        return false;
    }

    // Construct a message string to present to the filter. This doesn't have to
    // be identical to the message string that actually gets output to the log.
    std::ostringstream oss;
    oss << file << "|" << line << "|" << subsys << "|" << mtext << std::ends;
    std::string message(oss.str());

    // Attempt to locate this message in the message map.
    TsHsMessageMap::iterator iter = mMessageMap.find(message);

    if (iter == mMessageMap.end())
    {
        // The message was not found in map. If the map is full, bump the oldest message.
        // Insert the new message/time. Return a value indicating the message should be displayed.

        while (static_cast<int>(mMessageMap.size()) >= mHistory)
        {
            purgeMessages();
        }

        TsHsTimesDeque times;
        times.push_back(timestamp);
        mMessageMap.insert(TsHsMessageMapPair(message, times));

        pthread_mutex_unlock(&mResourceLock);
        return true;
    }
    else
    {
        // The message was found in map. Compute the beginning of the interval
        // and get rid of any times that precede it. If the deque is not full,
        // add the new time to it. Otherwise, if the deque is full, then the count has
        // been reached or exceeded, so suppress the message.

        double interval_start = timestamp - mInterval;
        purgeTimes((*iter).second, interval_start);

        if (static_cast<int>((*iter).second.size()) < mCount)
        {
            (*iter).second.push_back(timestamp);
            pthread_mutex_unlock(&mResourceLock);
            return true;
        }
        else
        {
            ++mMessagesSuppressed;
            pthread_mutex_unlock(&mResourceLock);
            return false;
        }
    }
    pthread_mutex_unlock(&mResourceLock);
    return false; // compiler thinks it can get here so it wants a value returned
}
