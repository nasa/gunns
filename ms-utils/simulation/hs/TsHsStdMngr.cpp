/*
@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Health and status message manager - code)

LIBRARY DEPENDENCY:
   (
     (simulation/hs/TsHsPluginConfig.o)
     (simulation/hs/TsHsTextPlugin.o)
     (simulation/hs/TsHsConsolePlugin.o)
     (simulation/hs/TsHsTermination.o)
     (simulation/hs/TsHsOutputPlugin.o)
     (simulation/hs/TsHsMsgFilter.o)
     (simulation/hs/TsHsMsgStdFilter.o)
     (simulation/hs/TsHsMngr.o)
     (simulation/timer/TS_timer.o)
     (simulation/hs/TsHsMsgQueue.o)
     (simulation/hs/TsHsConfig.o)
   )

PROGRAMMERS:
   (
     ((Jeffrey Middleton) (L3))
     ((Wesley A. White) (Tietronix Software) (August 2011))
     ((Wesley A. White) (Tietronix Software) (June 2013))
   )
*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include "sim_services/Message/include/message_proto.h"
#include "simulation/timer/TS_timer.h"
//#include "TsHsSqlitePlugin.hh"
//     (simulation/hs/TsHsSqlitePlugin.o)
#include "TsHsTextPlugin.hh"
#include "TsHsConsolePlugin.hh"
#include "TsHsTermination.hh"
#include "TsHsOutputPlugin.hh"
#include "TsHsStdMngr.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsStdMngr::TsHsStdMngr() :
    mConfig(),
    mMsgFilter(),
    mMsgQueue(4000, false),
    mTermination(),
    mRequestCount(0),
    mImmediateMode(true),
    mUseHelperThread(true),
    mPluginMngr()
{
    // Get configuration information
    mConfig.getConfigInfo();

    configure();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructor for unit testing. This ctor allows the config data to be set
//           programmatically by the unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsStdMngr::TsHsStdMngr(const TsHsConfig& config) :
    mConfig(config),
    mMsgFilter(),
    mMsgQueue(4000, false),
    mTermination(),
    mRequestCount(0),
    mImmediateMode(true),
    mUseHelperThread(true),
    mPluginMngr()
{
    configure();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Use configuration data to configure the logger.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::configure()
{
    // For thread-safe message testing purposes. Used by randomMessage function.
    srand(1234);

    // Configure the termination capability
    mTermination.setFatalToMultipleSubsystems(mConfig.mTerminationSubsystems);
    mTermination.setEnabled(mConfig.mTerminationEnabled);

    // Set up text plugin config
    mPluginConfig[TS_HS_TEXT].mEnabled     = mConfig.mTextPluginEnabled;
    mPluginConfig[TS_HS_TEXT].mOverwrite   = mConfig.mTextPluginOverwriteEnabled;
    mPluginConfig[TS_HS_TEXT].mBlocking    = mConfig.mTextPluginBlockingEnabled;
    mPluginConfig[TS_HS_TEXT].mPath        = mConfig.mTextPluginPath;

    // Set up sqlite plugin config
//    mPluginConfig[TS_HS_SQLITE].mEnabled   = mConfig.mSqlitePluginEnabled;
//    mPluginConfig[TS_HS_SQLITE].mOverwrite = mConfig.mSqlitePluginOverwriteEnabled;
//    mPluginConfig[TS_HS_SQLITE].mBlocking  = mConfig.mSqlitePluginBlockingEnabled;
//    mPluginConfig[TS_HS_SQLITE].mPath      = mConfig.mSqlitePluginPath;

    // Set up console plugin config
    mPluginConfig[TS_HS_CONSOLE].mEnabled   = mConfig.mConsolePluginEnabled;
    mPluginConfig[TS_HS_CONSOLE].mBlocking  = mConfig.mConsolePluginBlockingEnabled;

    // Create instances of the plugins and add them to the plugin manager.
    TsHsOutputPlugin* new_plugin;

    // Add the text plugin
    new_plugin = new TsHsTextPlugin(TS_HS_TEXT);
    new_plugin->configure(mPluginConfig[TS_HS_TEXT]);
    mPluginMngr.add(new_plugin);

    // Add the SQLite plugin
//    new_plugin = new TsHsSqlitePlugin(TS_HS_SQLITE);
//    new_plugin->configure(mPluginConfig[TS_HS_SQLITE]);
//    mPluginMngr.add(new_plugin);

    // Add the console plugin
    new_plugin = new TsHsConsolePlugin(TS_HS_CONSOLE);
    new_plugin->configure(mPluginConfig[TS_HS_CONSOLE]);
    mPluginMngr.add(new_plugin);

    // The manager will init all the plugins
    mPluginMngr.init();

    // Configure filtering in general
    mMsgFilter.setEnabled(mConfig.mFilterEnabled);
    mMsgFilter.setBlocking(mConfig.mFilterBlockingEnabled);

    // Configure the redundancy filter
    mMsgFilter.setInterval(mConfig.mRedundancyFilterInterval);
    mMsgFilter.setCount(mConfig.mRedundancyFilterCount);
    mMsgFilter.setHistory(mConfig.mRedundancyFilterHistory);

    // Configure the category filters
    mMsgFilter.setTypeFilter(TS_HS_DEBUG,   mConfig.mCategoryFilterDebugEnabled);
    mMsgFilter.setTypeFilter(TS_HS_INFO,    mConfig.mCategoryFilterInfoEnabled);
    mMsgFilter.setTypeFilter(TS_HS_WARNING, mConfig.mCategoryFilterWarningsEnabled);
    mMsgFilter.setTypeFilter(TS_HS_ERROR,   mConfig.mCategoryFilterErrorsEnabled);
    mMsgFilter.setTypeFilter(TS_HS_FATAL,   mConfig.mCategoryFilterFatalsEnabled);

    // Initialize the message filter
    mMsgFilter.init();

    // Configure the message queue
    mMsgQueue.initialize(mConfig.mQueueCapacity, mConfig.mQueueBlockingEnabled);

    // Sets the global tsGlobalHsMngr pointer to point to the manager. If more than one manager is
    // created (which should never happen), the pointer will point to the most recent one created.

    // In general it's not good to access the 'this' pointer in a ctor, but here it should be
    // okay. We aren't invoking any virtual methods. We're just capturing the address for later use.
    tsGlobalHsMngr = this;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsStdMngr::~TsHsStdMngr() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Creates the plugins using config data. Adds plugins to plugin manager and causes it
///          to init each plugin. Causes the message filter object to be initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::init()
{
    // Call the base class's init method. It sets the global pointer used by TsHsMsg.
    //TsHsMngr::init();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Should be called once after all other models have initialized. Used to transition
///          some settings before going to run mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::post_init()
{
    // About to start helper thread, so start queuing requests
    mImmediateMode = false;

    // Debug
    //    std::cout << "queue size at post init = " << mMsgQueue.getSize() << std::endl;
    //
    //    int lock_failures = mMsgFilter.getLockFailures();
    //    if (lock_failures > 0)
    //        message_publish(MSG_WARNING, "TsHsMsgStdFilter skipped %d messages due to mutex conflicts\n", lock_failures);
    //
    //    lock_failures = mMsgQueue.getLockFailures();
    //    if (lock_failures > 0)
    //        message_publish(MSG_WARNING, "TsHsMsgQueue skipped %d messages due to mutex conflicts\n", lock_failures);
    //
    //    int queue_overwrites = mMsgQueue.getOverflows();
    //    if (queue_overwrites > 0)
    //        message_publish(MSG_WARNING, "TsHsMsgQueue overwrote %d messages due to queue overflow\n", queue_overwrites);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Should be called once before a transition to freeze.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::freeze_init()
{
    // Tell the helper thread's job to terminate on it's next iteration. It won't be restarted
    // while in freeze mode. The freeze job will assume control of the message queue.
    mUseHelperThread = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Should be called periodically while in freeze.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::freeze()
{
    // Some models run even when in freeze, so queue processing must continue in freeze. It's
    // tempting to invoke inputOutputThread() here. This would work, but would be kludgy. The helper
    // thread job would compete with the freeze job until the former ran out of nodes to process.
    // There might still be a cycle of overlap, but the queue is thread safe.

    TsHsMsgQueueNode node;
    while (!mMsgQueue.isEmpty())
    {
        processQueueNode(node);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Should be called once before a transition from freeze to run.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::unfreeze()
{
    // Reenable the helper thread's job.
    mUseHelperThread = true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Should be called during a checkpoint restart
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::restart()
{
    // If we are restarting, the plugins must already exist, but their current configuration
    // might not match what was checkpointed. The plugins themselves are not checkpointed, but
    // their config data is checkpointed. We just need to reconfigure the existing plugings
    // and restart them using the checkpointed configuration data.

    TsHsOutputPlugin* plugin;

    plugin = mPluginMngr.getPlugin(TS_HS_TEXT);
    plugin->configure(mPluginConfig[TS_HS_TEXT]);
    plugin->restart();

//    plugin = mPluginMngr.getPlugin(TS_HS_SQLITE);
//    plugin->configure(mPluginConfig[TS_HS_SQLITE]);
//    plugin->restart();

    mMsgFilter.restart();

    // Make the restart conspicuous in the log file.
    msg(__FILE__, __LINE__, __func__, TS_HS_INFO, TS_HS_HS, " --------------------- H&S Restart ---------------------");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Sets the enabled flag on all plugins and updates them via the plugin manager.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::update()
{
    mPluginMngr.update();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Logs a message via the plugin manager. Determines the MET time of the message.
///          Truncates the 'subsystem' field, if necessary. Forwards the message to the plugin
///          manager, if the message is not screened out by the message filter.
///
/// @param[in] file     (--) name of file which initiated logging the message.
/// @param[in] line     (--) line of file which initiated logging the message.
/// @param[in] function (--) The name of the function logging the message; typically __func__.
/// @param[in] type     (--) the type of message (e.g. info, warning, etc.).
/// @param[in] subsys   (--) the subsystem from which the message originated.
/// @param[in] mtext    (--) the message text.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::msg(
   const std::string& file,
   const int          line,
   const std::string& function,
   TS_HS_MSG_TYPE     type,
   const std::string& subsys,
   const std::string& mtext)
{
    mRequestCount++;

    // Initialize MET to an obviously invalid value
    TS_TIMER_TYPE met = {-1, 99, 23, 59, 59, -9999.0, 0};

    // Set met to global pointer of MET data
    if (ts_global_met_ptr)
    {
        met = *ts_global_met_ptr;
    }

    unsigned long timestamp = time(NULL);

    // Limit the length of the subsystem field. Truncate if necessary.
    const int SUBSYS_MAX_SIZE = 13;
    char truncatedSubsys[SUBSYS_MAX_SIZE];
    strncpy(truncatedSubsys, subsys.c_str(), SUBSYS_MAX_SIZE-1);
    truncatedSubsys[SUBSYS_MAX_SIZE-1] = '\0';

    if (mImmediateMode)
    {
        // Output the message immediately without queuing  it
        if (mMsgFilter.shouldSendMessage(met.seconds, file, line, type, truncatedSubsys, mtext))
        {
            mPluginMngr.msg(file, line, function, type, truncatedSubsys, met, timestamp, mtext);
        }
    }
    else
    {
        // Queue the message
        mMsgQueue.insertMsg(file, line, function, type, truncatedSubsys, mtext, met, timestamp);
    }

    mTermination.msg(file, line, function,type, subsys, mtext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Processes messages on the message request queue.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::inputOutputThread()
{
    TsHsMsgQueueNode node;

    // The loop variable is kept true until shutdown. Then it's used to terminate the thread. Trick will
    // not restart an asynchronous thread that terminates during shutdown.
    
    // We let the loop terminate if there is no work to do. Trick will restart it. We do this in hopes that
    // the free up some cpu cycles for other jobs, otherwise we'd spin and burn-up everything.
    while (mUseHelperThread && !mMsgQueue.isEmpty())
    {
        // Process a queue node, if any. It's okay if the queue is empty.
        processQueueNode(node);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Processes an individual message request queue node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::processQueueNode(TsHsMsgQueueNode& node)
{
    if (mMsgQueue.removeMsg(node) != false)
    {
        // Present the message to the filter, and pass it through to mPluginMngr if okay.
        if (mMsgFilter.shouldSendMessage(node.mMet.seconds, node.mFile, node.mLine, node.mType, node.mSubsys, node.mText))
        {
            mPluginMngr.msg(node.mFile, node.mLine, node.mFunction, node.mType, node.mSubsys, node.mMet, node.mTimestamp, node.mText);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Shuts down the message manager.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::shutdown()
{
    // todo: should I block new request when shutting down -- a misbehaving model could delay
    //       shutdown indefinitely if it flooded the queue with messages?

    // There may be a backlog of messages in the queue that need to be processed before shutdown.
    // If our shutdown job finishes before the backlog is cleared the messages will be lost. To
    // prevent losing messages, we are going to take control of queue processing away from the
    // helper thread and give it to the shutdown job.

    // Tell the helper thread's job to terminate on it's next iteration. It won't be restarted
    // when in shutdown mode. The async thread itself will be terminated by trick just before trick
    // terminates.
    mUseHelperThread = false;

    // It's okay if there is some overlap in which both the helper and shutdown are trying to
    // process the queue. The queue is thread safe.

    TsHsMsgQueueNode node;
    while (!mMsgQueue.isEmpty())
    {
        processQueueNode(node);
    }

    int lock_failures = mMsgFilter.getLockFailures();

    if (lock_failures > 0)
    {
        message_publish(MSG_WARNING, "TsHsMsgStdFilter skipped %d messages due to mutex conflicts\n", lock_failures);
    }

    lock_failures = mMsgQueue.getLockFailures();
    if (lock_failures > 0)
    {
        message_publish(MSG_WARNING, "TsHsMsgQueue skipped %d messages due to mutex conflicts\n", lock_failures);
    }

    int queue_overwrites = mMsgQueue.getOverflows();
    if (queue_overwrites > 0)
    {
        message_publish(MSG_WARNING, "TsHsMsgQueue overwrote %d messages due to queue overflow\n", queue_overwrites);
    }

    mPluginMngr.shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Used by test threads to log messages at random intervals.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsStdMngr::randomMessages()
{
    // 1 ms = 1,000,000 ns
    // delay_ns range = [0.0 ..0.999] second
    int delay_ns = (rand() % 1000) * static_cast<int>(1e06);

    // The client's rate is slightly slower the rate set.
    struct timespec s;
    s.tv_sec  = 0;
    s.tv_nsec = static_cast<long>(delay_ns);

    // Sleep a second so we are running about at rate
    nanosleep(&s, NULL);

    for (int i=0 ; i<100 ; i++)
    {
        // Log a test message

        msg("File", delay_ns, "Function", TS_HS_ERROR, "Subsys", "Message text.\n");

        // Delay some, then repeat
        delay_ns = (rand() % 1000) * static_cast<int>(1e06);
        s.tv_nsec = static_cast<long>(delay_ns);
        nanosleep(&s, NULL);
    }

}
