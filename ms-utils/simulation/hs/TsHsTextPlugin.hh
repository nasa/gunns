#ifndef TsHsTextPlugin_EXISTS
#define TsHsTextPlugin_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_TEXT_PLUGIN Text Plugin
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Component of the Health & Status message framework. An output plugin that outputs to a text file.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
   (TsHsTextPlugin.o)
  )

PROGRAMMERS:
- (
     ((Jeffrey Middleton) (L3) (January 2010) (Initial version))
     ((Wesley A. White) (Tietronix Software) (August 2011))
  )

@{
*/

#include <string>
#include "TsHsOutputPlugin.hh"
#include "TsHsPluginConfig.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief An output plugin used to log messages to a text file.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsHsTextPlugin: public TsHsOutputPlugin
{
public:

    /// @brief Constructor
    TsHsTextPlugin(int id);

    // @brief Destructor
    virtual ~TsHsTextPlugin();

    /// @brief Should be called once to initialize a plugin.
    virtual void configure(const TsHsPluginConfig& configData);

    /// @brief Initializes the plugin using config data.
    virtual bool init(void);

    /// @brief Restarts a plugin
    virtual bool restart(void);

    /// @brief Logs a message to a text file
    virtual bool msg(const std::string& file, const int line, const std::string& function, TS_HS_MSG_TYPE type, const std::string& subsys,
                     const TS_TIMER_TYPE& met, unsigned long timestamp, const std::string& hsmsg);

    virtual void shutdown(void);

protected:

    bool insertMessage(const std::string& message);

    std::string     mFilename;        // ** (--) Output file name
    bool            mFirstpass;       // ** (--) Used to determine when to create a new file
    bool            mOverwrite;       // ** (--) Overwrite file or use multiple files with timestamps
    int             mTryLockFailures; // ** (--) Number of times trylock failed to get the lock (== number of dropped messages)
    pthread_mutex_t mResourceLock;    // ** (--) Mutex which controls access to database files
    bool            mBlocking;        // ** (--) Wait on mutex if true, else skip message

private:

    // Don't define these; assignment or copy of a plugin is an error
    TsHsTextPlugin(const TsHsTextPlugin&);
    TsHsTextPlugin& operator=(const TsHsTextPlugin&);
};

/// @}

#endif /* TsHsTextPlugin_EXISTS */
