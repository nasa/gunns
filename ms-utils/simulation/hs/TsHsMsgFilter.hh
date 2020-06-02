#ifndef TsHsMsgFilter_EXISTS
#define TsHsMsgFilter_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_MESSAGE_FILTER Message Filter
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
- (Provides a base class for limiting the output of duplicate health & status messages.
   The idea is limit the number of times a duplicate message is output over an
   interval of time. The maximum number of messages which can be tracked at any one time
   can be set.)

LIBRARY DEPENDENCY:
- (
     (simulation/hs/TsHsMsgFilter.o)
  )

PROGRAMMERS:
- (((Wesley A. White) (Tietronix Software) (August 2011)))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "TS_hs_msg_types.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Message filter base class. This class implements getters and setters for
///          filter settings and defines interfaces for the actual filtering methods.
///          <b>Developer note:</b> the set methods may be called from the HS input file in
///          in order to control message filter behavior. Filter settings can also be changed
///          while a sim is running using trick view or similar application.
///
/// @details The TsHsMsgStdFilter class provides an interface for message filtering. Two type of
///          filters are supported. The redundancy filter eliminates or reduces the logging of
///          redundant messages. The category filter removes messages base on their message types.
///          Users of the redundancy filter set filter parameters (interval, count, history) which
///          specify the number of duplicate messages per interval of time are allowable.
///          Users also set the maximum number of messages for which the filter will maintain
///          a history. Users then present messages to the filter and the filter returns a
///          value indicating whether or not the message should be displayed. Interval is an
///          interval of time. Count is maximum number of identical message to allow within
///          the interval of time. History is the maximum number of messages for which to
///          maintain interval counts.
///
/// @details Examples of how to use filters in a trick 10 input file:
/// @code
///           // Set redundancy filter to track the last 20 unique message and suppress attempts
///           // to output more than 2 of any particular message in any 3 second interval of time.
///           hs.hsMngr.msgFilter.setEnabled(True)
///           hs.hsMngr.msgFilter.setInterval(3)
///           hs.hsMngr.msgFilter.setCount(2)
///           hs.hsMngr.msgFilter.setHistory(20)
///
///           // Filter out all TS_HS_DEBUG, and TS_HS_INFO messages
///           hs.hsMngr.msgFilter.setTypeFilter(0, True)
///           hs.hsMngr.msgFilter.setTypeFilter(1, True)
/// @endcode
/// @details  Note that when setting the type filter the first arguments are integers which
///           correspond to values in "TS_hs_msg_types.h". For example, "0" corresponds to
///           TS_HS_DEBUG.
/// @details  Filter settings can be modified while a simulation is running via trick view.
///           The variable mFilteredTypes may be used to set category filters. This field
///           is a bitmap with bits corresponding to the TS_HS_MSG_TYPE enumeration type. To
///           filter out TS_HS_DEBUG and TS_HS_INFO messages bits 0 and 1 respectively should
///           be set. Therefore the value of mFilteredTypes should be set to 3 (i.e. ...000011).
//////////////////////////////////////////////////////////////////////////////////////////////////

class TsHsMsgFilter
{
    TS_MAKE_SIM_COMPATIBLE(TsHsMsgFilter);
public:

    /// @brief Default constructor.
    TsHsMsgFilter();

    /// @brief Destructor.
    virtual ~TsHsMsgFilter();

    // Getters

    /// @brief Returns true if message filtering is enabled.
    bool isEnabled()             const { return mEnabled;  }

    /// @brief Test if the specified message type has been suppressed.
    bool isTypeFiltered(TS_HS_MSG_TYPE type) const;

    /// @brief Returns the number of messages examined by the filter.
    int  getMessagesProcessed()  const { return mMessagesProcessed; }

    /// @brief Returns the number of messages rejected by the filter.
    int  getMessagesSuppressed() const { return mMessagesSuppressed; }

    /// @brief Returns the interval that the filter will consider when looking for duplicate messages
    int  getInterval()           const { return mInterval; }

    /// @brief Returns the number of duplicate messages allowed during the interval
    int  getCount()              const { return mCount;    }

    /// @brief Returns the maximum number of unique messages the filter will track at a given time
    int  getHistory()            const { return mHistory;  }

    /// @brief Enables or disables the filter.
    void setEnabled(bool enabled);

    /// @brief Suppress or enable messages by message type
    void setTypeFilter(TS_HS_MSG_TYPE type, bool filterOn);

    /// @brief Set the time interval that the filter will consider when looking for duplicate messages.
    void setInterval(int sec);

    /// @brief Set the number of duplicate messages allowed during an interval of time.
    void setCount(int count);

    /// @brief Set the maximum number of messages the filter will track at a given time.
    void setHistory(int size);

    /// @brief Set the blocking policy to use when multiple threads try to access the filter simultaneously.
    void setBlocking(bool blocking);

    /// @brief Initializes the filter
     virtual void init();

    /// @brief Compares this message with previous ones to see if the message should be suppressed.
    virtual bool shouldSendMessage(
            const double       timestamp,
            const std::string& file,
            const int          line,
            TS_HS_MSG_TYPE     type,
            const std::string& subsys,
            const std::string& mtext
            ) = 0;

protected:

    bool   mEnabled;            //    (--)  True if the filter is turned "on", otherwise this is false
    int    mFilteredTypes;      //    (--)  Bitmap of message types to suppress

    int    mMessagesProcessed;  //    (--)  The number of messages examined by the filter
    int    mMessagesSuppressed; //    (--)  The number of messages suppressed by the filter

    int    mInterval;           //    (s)  Interval that the filter will consider when looking for duplicate messages
    int    mCount;              //    (--)  Number of duplicate messages allowed during the interval
    int    mHistory;            //    (--)  Maximum number of unique messages the filter will track at a given time
    bool   mBlocking;           //    (--)  Wait for resource or suppress message if resource locked and non-blocking.

private:

    // Disable copy constructor and assignment operator
    TsHsMsgFilter(const TsHsMsgFilter&);
    const TsHsMsgFilter& operator=(const TsHsMsgFilter&);
};

/// @}

#endif /* TsHsMsgFilter_EXISTS */
