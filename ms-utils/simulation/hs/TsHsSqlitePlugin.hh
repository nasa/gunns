#ifndef TsHsSqlitePlugin_EXISTS
#define TsHsSqlitePlugin_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_SQLITE_PLUGIN Sqlite Plugin
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Component of the Health & Status message framework. An output plugin that outputs to an sqlite database.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- (
   (TsHsSqlitePlugin.o)
  )

PROGRAMMERS:
- ((Jeffrey Middleton) (L3) (January 2010) (Initial version))

@{
*/

#include <string>
#include <sqlite3.h>
#include <pthread.h>
#include "TsHsOutputPlugin.hh"
#include "TsHsPluginConfig.hh"

/// @brief An output plugin used to log messages to an SQLite database.
class TsHsSqlitePlugin: public TsHsOutputPlugin
{
public:

    /// @brief Constructor
    TsHsSqlitePlugin(int id);

    /// @brief Constructor
    //TsHsSqlitePlugin(const std::string& db_file, bool overwrite);

    /// @brief Destructor
    virtual ~TsHsSqlitePlugin();

    /// @brief Should be called once to initialize a plugin.
    virtual void configure(const TsHsPluginConfig& configData);

    /// @brief Initializes the output plugin by creating required tables.
    virtual bool init(void);

    /// @brief Restarts a plugin
    virtual bool restart(void);

    /// @brief Updates the plugin by committing transactions to the database.
    virtual bool update(void);

    /// @brief Commits final transactions and close the database connection.
    virtual void shutdown(void);

    /// @brief Issues an sqlite health and status message.
    virtual bool msg(const std::string& file, int line, const std::string& function, TS_HS_MSG_TYPE type, const std::string& subsys,
                     const TS_TIMER_TYPE& met, unsigned long timestamp, const std::string& hsmsg);

protected:

    bool insertMessage(char* insertCommand);

    /// @brief Displays the error message and, if fatal, close the database.
    void handleSqlError(const std::string&, int, char**, bool);

    /// @brief Hash function used to generate database key values.
    long long int hashString(long long int, const std::string&);

    /// @brief Opens the database used to store H&S messages.
    bool openDatabase();

    // Data Members

    std::string     mFilename;         // ** (--) Filename of SQLite db
    bool            mFirstpass;        // ** (--) First init
    bool            mOverwrite;        // ** (--) Overwrite file or use multiple files with timestamps
    bool            mTransactionOpen;  // ** (--) True if a transaction is in progress
    sqlite3*        mDatabaseHandle;   // ** (--) SQLite db handle
    int             mTryLockFailures;  // ** (--) Number of times trylock failed to get the lock (== number of dropped messages)
    pthread_mutex_t mResourceLock;     // ** (--) Mutex which controls access to database files
    bool            mBlocking;         // ** (--) Wait on mutex if true, else skip message

private:

    // Don't define these; assignment or copy of a plugin is an error
    TsHsSqlitePlugin(const TsHsSqlitePlugin&);
    TsHsSqlitePlugin& operator=(const TsHsSqlitePlugin&);
};

/// @}

#endif /* TsHsSqlitePlugin_EXISTS */
