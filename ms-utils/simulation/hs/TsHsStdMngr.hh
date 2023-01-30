#ifndef TsHsStdMngr_EXISTS
#define TsHsStdMngr_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_MSG_MANAGER Message Manager
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Health & Status message framework "standard" message manager.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
    (simulation/hs/TsHsMngr.o)
    (simulation/hs/TsHsStdMngr.o)
    (simulation/hs/TsHsConfig.o)
  )

PROGRAMMERS:
- (
    ((Jeffrey Middleton) (L3) (January 2010) (Initial version))
    ((Wesley A. White) (Tietronix Software) (August 2011))
  )

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "TsHsMngr.hh"
#include "TsHsMsgStdFilter.hh"
#include "TsHsMsgQueue.hh"
#include "TsHsOutputPlugin.hh"
#include "TsHsPluginConfig.hh"
#include "TsHsConfig.hh"
#include "TsHsTermination.hh"
#include "TS_hs_msg_types.h"

/// @brief Ideally this would be the last enum value, not a define.
///        But Trick doesn't get the enum into io_src, and everything dies.
#define HS_NUM_PLUGINS 3


/// @brief Implements the interface defined by TsHsMngr to produce a 'standard' health and status manager.
///        The standard manager is based around the concept of output plugins and includes a message filtering
///        capability.
class TsHsStdMngr : public TsHsMngr
{
TS_MAKE_SIM_COMPATIBLE(TsHsStdMngr);

public:

    /// @brief Enumerates the types of output plugins which are currently supported.
    enum HsPluginName
    {
        TS_HS_TEXT    = 0,
        TS_HS_SQLITE  = 1,
        TS_HS_CONSOLE = 2
    };

    /// @brief Default constructor.
    TsHsStdMngr();

    /// @brief  Constructor for unit testing.
    TsHsStdMngr(const TsHsConfig& config);

    /// @brief Destructor.
    virtual ~TsHsStdMngr();

    /// @brief Should be called once to initialize the manager.
    virtual void init();

    /// @brief Should be called once after all other models have initialized.
    virtual void post_init();

    /// @brief Should be called once before a transition to freeze.
    virtual void freeze_init();

    /// @brief Should be called periodically while in freeze.
    virtual void freeze();

    /// @brief Should be called once before a transition from freeze to run.
    virtual void unfreeze();

    /// @brief Should be called during a checkpoint restart
    virtual void restart();

    /// @brief Should be called periodically to update the manager.
    virtual void update();

    /// @brief Should be called as needed to log a message.
    virtual void msg(const std::string&, const int, const std::string&, TS_HS_MSG_TYPE, const std::string&, const std::string&);

    /// @brief Processes messages on the message request queue.
    void inputOutputThread();

    /// @brief Processes an individual message request queue node.
    void processQueueNode(TsHsMsgQueueNode& node);

    /// @brief Should be called once to shut down the manager.
    virtual void shutdown();

    /// @brief Called by test threads to log random messages.
    void randomMessages();

    // Data Members

    TsHsConfig       mConfig;                        //    (--) Collects configuration info
    TsHsPluginConfig mPluginConfig[HS_NUM_PLUGINS];  //    (--) Plugin status and path info
    TsHsMsgStdFilter mMsgFilter;                     //    (--) The message filter
    TsHsMsgQueue     mMsgQueue;                      //    (--) The message queue
    TsHsTermination  mTermination;                   //    (--) Termination handling
    int              mRequestCount;                  //    (--) The total number of logging request attempts
    bool             mImmediateMode;                 //    (--) Process messages immediately, i.e. do not queue.
    bool             mUseHelperThread;               //    (--) Flag used to stop helper thread during shutdown.

protected:

    /// @brief Used by constructors to configure logger.
    void configure();

    TsHsOutputPluginMngr mPluginMngr;                // ** (--) The plugin manager

private:

    // Not implemented
    TsHsStdMngr(const TsHsStdMngr&);
    const TsHsStdMngr& operator=(const TsHsStdMngr&);
};

#undef HS_NUM_PLUGINS

/// @}

#endif /* TsHsStdMngr_EXISTS */
