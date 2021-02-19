#ifndef TsHsMsgStdFilter_EXISTS
#define TsHsMsgStdFilter_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_MESSAGE_FILTER Message Filter
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (Extends the TsHsMsgFilter class providing "standard" message filters which
   include a redundancy filter and a category filter.)

LIBRARY DEPENDENCY:
(
  (simulation/hs/TsHsMsgStdFilter.o)
)

PROGRAMMERS:
- (((Wesley A. White) (Tietronix Software) (August 2011)))
@{
*/

#include <deque>
#include <map>
#include <pthread.h>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "TsHsMsgFilter.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Provides "standard" message filters which include a redundancy filter and a
///          category filter.
///
/// @details The TsHsMsgStdFilter class implements message filtering. Two type of filters are
///          supported. The redundancy filter eliminates or reduces the logging of
///          redundant messages. The category filter removes messages base on their message types.
///          See TsHsMsgFilter documentation for an overview. Detailed implementation information
///          follows.
///
/// @details <b>Design:</b> Recently displayed messages are stored in a map. The
///          message itself if the map key. The map value is a deque of display times for that
///          message. Message counts are not added for suppressed messages.
///
/// @details <b>Algorithm:</b> When the filter is presented a message, the message is located in the
///          map (inserted if necessary). Its display times deque is obtained and the display
///          time is added as follows. The deque is purged of any display times outside of the
///          current interval. If the deque contains more than 'count' items, the filter
///          returns a value indicating that the message should be suppressed. If the deque
///          contains fewer than 'count' items the time is added to the back of the deque and
///          the filter returns a value indicating that the message should be displayed.
///
/// @details <b>Memory Usage:</b> The filter is designed to limit the amount of memory used in a
///          'worst-case' scenario in which the filter is bombarded with a large number of
///          unique messages. This could happen, for example, if a programmer includes a
///          time-field or other rapidly changing value in the code which creates his/her
///          message strings. At most H unique messages will be tracked at any one time. For
///          each message at most C display times will be stored. Therefore the maximum size
///          of the filter is roughly given by \n
///
///          deque_overhead_size = sizeof(TsHsTimesDeque) = 80 bytes \n
///          deque_size          = deque_overhead_size + (C * sizeof(double) = 80+(C*8) bytes \n
///          map_node_size       = sizeof(std::_Rb_tree_node_base) = 32 bytes \n
///          map_overhead_size   = sizeof(TsHsMessageMap) = 48 bytes \n
///          map_size           ~= map_overhead_size + H*[max_message_size + deque_size + map_node_size] \n
///
///          Example (C=2, H=20, max_message_size=256): \n
///
///          deque_size = 80+(2*8) = 96 bytes \n
///          map_size = 48+20[256+96+32] = 7728 bytes \n
///
/// @details Currently the filter does not truncate strings to any max length. However, the
///          average length of messages tends to be running around 129 bytes.
///
/// @details <b>Processor Usage:</b> Deque insertion/deletion operations are performed in amortized
///          constant time.Map search, insertion, and deletion operations are performed in
///          O(log n) time. Finding and removing the oldest message in the map requires
///          (worst-case) iterating through all the maps elements. This is performed in O(n) time.
///          Therefore worst-case performance is O(n), which is not bad considering the filter
///          will work effectively with fairly small values of n=H. For example, it is very
///          likely that for n=H=20, the 20th message will probably be outside of the interval
///          of interest well before it is deleted, therefore its deletion will have no impact
///          on the performance of the filter.
///
/// @see TsHsMsgFilter for more information.
//////////////////////////////////////////////////////////////////////////////////////////////////
class TsHsMsgStdFilter: public TsHsMsgFilter
{
    TS_MAKE_SIM_COMPATIBLE(TsHsMsgStdFilter);
public:

    /// @brief Default constructor
    TsHsMsgStdFilter();

    int getLockFailures() const { return mTryLockFailures; }

    /// @brief Destructor
    virtual ~TsHsMsgStdFilter();

    /// @brief Presents a message to the filter to see if the message should be suppressed.
    virtual bool shouldSendMessage(
            const double        timestamp,
            const std::string&  file,
            const int           line,
            TS_HS_MSG_TYPE      type,
            const std::string&  subsys,
            const std::string&  mtext
            );

    /// @brief Reinitialize the message filter after a restart.
    virtual void restart();

protected:

    // Make reading the STL stuff more comprehensible with some typedefs

    typedef std::deque< double > TsHsTimesDeque;
    typedef std::map< std::string, TsHsTimesDeque > TsHsMessageMap;

    /// @brief Removes times from a times deque if the times precede the interval start time
    void purgeTimes(TsHsTimesDeque& times, double intervalStart);

    /// @brief Find and remove the oldest message in the message map
    void purgeMessages();

    TsHsMessageMap  mMessageMap;       // ** (--)  Tracks which messages have been displayed and when
    int             mTryLockFailures;  // ** (--)  Number of times trylock failed to get the lock (== number of dropped messages)
    pthread_mutex_t mResourceLock;     // ** (--)  Mutex which controls access to database files

private:
    // Disable copy constructor and assignment operator
    TsHsMsgStdFilter(const TsHsMsgStdFilter&);
    const TsHsMsgStdFilter& operator=(const TsHsMsgStdFilter&);
};

/// @}

#endif /* TsHsMsgStdFilter_EXISTS */
