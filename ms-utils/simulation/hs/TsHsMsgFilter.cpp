/****************************************** TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Provides a base class for limiting the output of duplicate health & status messages.
     The idea is limit the number of times a duplicate message is output over an
     interval of time. The maximum number of messages which can be tracked at any one time
     can be set.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (simulation/hs/TsHsMsgFilter.o)
   )

PROGRAMMERS:
   (((Wesley A. White) (Tietronix Software) (August 2011)))
**************************************************************************************************/

#include "TsHsMsgFilter.hh"

#include "iostream" // debug temp

// Some rather arbitrary default values, these are also used as substitutes for values
// which are set out-of-range.
#define DEFAULT_INTERVAL 60
#define DEFAULT_COUNT     2
#define DEFAULT_HISTORY  25

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructor. Initialize filter settings to some sane values.
/// @note    Ideal filter setting are subjective and should really be set in the input file. But
///          we set some sane values to make the code work "out of the box". Setting values to
///          all zeros, doesn't make sense in this case, so just set to some arbitrary yet
///          reasonable values.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsgFilter::TsHsMsgFilter() :
    mEnabled(true),
    mFilteredTypes(0),
    mMessagesProcessed(0),
    mMessagesSuppressed(0),
    mInterval(DEFAULT_INTERVAL),
    mCount(DEFAULT_COUNT),
    mHistory(DEFAULT_HISTORY),
    mBlocking(false)
{
    // Empty
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsMsgFilter::~TsHsMsgFilter()
{
    // Empty
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Initializes the filter.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgFilter::init()
{
    // Do not set class members here that can be set from the input file. Otherwise these values will
    // overwrite the input file settings. This gets called during trick's initialization phase
    // (which comes after the input file is processed).
    mMessagesProcessed  = 0;
    mMessagesSuppressed = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Suppress or enable messages by message type
/// @param[in] type     (--) Message category to suppress or allow.
/// @param[in] filterOn (--) Flag specifying whether to suppress or allow messages of this category.
/// @note This depends on TS_HS_MSG_TYPE's enumeration constants to have sequential 0-based values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgFilter::setTypeFilter(TS_HS_MSG_TYPE type, bool filterOn)
{
   int bit_number = static_cast<int>(type);

   if (filterOn)
   {
       mFilteredTypes |= (1 << bit_number);
   }
   else
   {
       mFilteredTypes &= ~(1 << bit_number);
   }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Test if the specified message type has been suppressed.
/// @param[in] type     (--) Message category of interest
/// @return true if specified message category has been suppressed.
/// @note This depends on TS_HS_MSG_TYPE's enumeration constants to have sequential 0-based values.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsMsgFilter::isTypeFiltered(TS_HS_MSG_TYPE type) const
{
   int bit_number = static_cast<int>(type);
   return mFilteredTypes & (1 << bit_number);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Set the time interval to the specified value. Does nothing if argument not
///            greater than 0.
///
/// @param[in] sec ( s) The interval of time in which we are concerned about duplicate messages.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgFilter::setInterval(int sec)
{
    if (sec > 0)
    {
        mInterval = sec;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Set the maximum message display count to the specified value. Does nothing if
///            argument not greater than 0.
///
/// @param[in] count (--) The max number of messages allowed per interval of time.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgFilter::setCount(int count)
{
    if (count > 0)
    {
        mCount = count;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Set the maximum amount of message history to keep. Does nothing if
///            argument not greater than 0.
///
/// @param[in] size (--) The max number of messages that will be tracked at a time.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgFilter::setHistory(int size)
{
    if (size > 0)
    {
        mHistory = size;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Set the blocking policy to use when multiple threads try to access the filter simultaneously.
///
/// @param[in] blocking (--) If true filter will block, otherwise message will be dropped.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgFilter::setBlocking(bool blocking)
{
    mBlocking = blocking;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Set the enabled/disabled policy on the filter
///
/// @param[in] enabled (--) If true filter is active
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsMsgFilter::setEnabled(bool enabled)
{
   mEnabled = enabled;
}
