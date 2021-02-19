#ifndef TsHsOutputPlugin_EXISTS
#define TsHsOutputPlugin_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_OUTPUT_PLUGIN Output Plugin
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Component of the Health & Status message framework. An output plugin abstract base class designed to
   output messages to various destinations. The specific destinations (e.g. text file, database, etc.)
   are defined and implemented by subclasses.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
   (TsHsOutputPlugin.o)
  )

PROGRAMMERS:
- (
    ((Jeffrey Middleton) (L3) (January 2010) (Initial version))
    ((Wesley A. White) (Tietronix Software) (August 2011))
  )

@{
*/

#include <cstdarg>
#include <vector>
#include <string>

#include "simulation/timer/TS_timer.h"
#include "TsHsPluginConfig.hh"
#include "TS_hs_msg_types.h"

/// @brief Output plugin base class. All output plugins implement this interface.
class TsHsOutputPlugin
{
public:
    /// @brief Constructor.
    TsHsOutputPlugin(int id) :
        mId(id),
        mEnabled(true)
    {
        mId = id;
    }

    /// @brief Destructor.
    virtual ~TsHsOutputPlugin() { }

    /// @brief Return the plugin's ID
    int getId() const { return mId; }

    /// @brief Set the enable/disable state of the pluging
    void setEnabled(bool state) { mEnabled = state; }

    /// @brief Set the enable/disable state of the pluging
    bool isEnabled(void) const { return mEnabled; }

    /// @brief Should be called once to initialize a plugin.
    virtual void configure(const TsHsPluginConfig& configData __attribute__((unused))) { }

    /// @brief Should be called once to initialize a plugin.
    virtual bool init(void)     { return true; }

    /// @brief Restarts a plugin
    virtual bool restart(void)  { return true; }

    /// @brief Should be called periodically to update a plugin.
    virtual bool update(void)   { return true; }

    /// @brief Should be called once to shut down a plugin.
    virtual void shutdown(void) { }

    /// @brief Should be called as needed to log a message via an output plugin.
    virtual bool msg(const std::string&, const int, const std::string&, TS_HS_MSG_TYPE, const std::string&, const TS_TIMER_TYPE&, unsigned long, const std::string&) = 0;

protected:
    int  mId;        // ** (--)  Plugin ID
    bool mEnabled;   // ** (--)  True if this plugin is enabled, else false
};

/// @brief Manages a set of plugins by performing operations on them as a group. For example, calling the manager's
///        init method invokes all the managed plugins's init methods.
class TsHsOutputPluginMngr
{

public:

    /// @brief Default constructor
    TsHsOutputPluginMngr() : plugins() { }

    /// @brief Destructor
    ~TsHsOutputPluginMngr();

    /// @brief Initializes all managed plugins.
    void init(void);

    /// @brief Updates all managed plugins.
    void update(void);

    /// @brief Shuts down all managed plugins.
    void shutdown(void);

    /// @brief Forwards the specified message to all managed plugins.
    void msg(const std::string& file, const int line, const std::string& function, TS_HS_MSG_TYPE type, const std::string& subsys,
            const TS_TIMER_TYPE& met, unsigned long timestamp, const std::string& hsmsg);

    /// @brief Adds a plugin to the plugin managers list of output plugins.
    void add(TsHsOutputPlugin* plugin);

    /// @brief Returns a pointer to the specified plugin.
    TsHsOutputPlugin* getPlugin(int id);

    /// @brief Returns the number of plugins currently being managed by the plugin manager.
    unsigned numPlugins();

private:

    std::vector<TsHsOutputPlugin*> plugins;  // ** (--) Vector of plugins being managed
};

/// @brief Utility function used to generate timestamps which are appended to log file names to make the names unique.
std::string tsHsFileTimestamp();

/// @}

#endif /* TsHsOutputPlugin_EXISTS */
