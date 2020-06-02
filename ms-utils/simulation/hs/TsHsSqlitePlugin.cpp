/****************************************** TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Sqlite output plugin for health & status message framework - code)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((TsHsOutputPlugin.o))

PROGRAMMERS:
   (
     ((Jeffrey Middleton) (L3) (January 2010) (Initial version))
     ((Wesley A. White) (Tietronix Software) (August 2011))
   )
**************************************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <sqlite3.h>
#include "sim_services/Message/include/message_proto.h"
#include "simulation/timer/TS_timer.h"
#include "TS_hs_msg_types.h"
#include "TsHsSqlitePlugin.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs a SQLite plugin object given the name of a database file to generate.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsSqlitePlugin::TsHsSqlitePlugin(int id) :
    TsHsOutputPlugin(id),
    mFilename(""),
    mFirstpass(true),
    mOverwrite(true),
    mTransactionOpen(false),
    mDatabaseHandle(0),
    mTryLockFailures(0),
    mResourceLock(),
    mBlocking(false)
{
    pthread_mutex_init(&mResourceLock, NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsSqlitePlugin::~TsHsSqlitePlugin()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Configure the plugin.
///
/// @return  configData - contains the plugin configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsSqlitePlugin::configure(const TsHsPluginConfig& configData)
{
    mEnabled   = configData.mEnabled;
    mFilename  = configData.mPath;
    mOverwrite = configData.mOverwrite;
    mBlocking  = configData.mBlocking;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Display database errors message to the console (standard error device) and close the
///          database if the error is considered fatal.
///
/// @param[in] file      (--) The file where the error occurred (unused).
/// @param[in] line      (--) The line number within the file mentioned above (unused).
/// @param[in] sql_error (--) The SQLite error message.
/// @param[in] fatal     (--) A flag used to determine if the database should be closed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsSqlitePlugin::handleSqlError(const std::string& file __attribute__((unused)), int line __attribute__((unused)), char** sql_error, bool fatal)
{
    if (*sql_error != 0)
    {
        message_publish(MSG_ERROR, "TsHsSqlitePlugin sqlite error: %s\n", *sql_error);
        sqlite3_free(*sql_error);

        if (fatal)
        {
            sqlite3_close(mDatabaseHandle);
            mTransactionOpen = false;
            mDatabaseHandle = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Opens the SQLite database used for logging health and status messages. If the
///          database does not already exists, it will be created.
///
/// @return  True if the open succeeds, otherwise false.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsSqlitePlugin::openDatabase()
{
    const int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX;

    if (sqlite3_open_v2(mFilename.c_str(), &mDatabaseHandle, open_flags, NULL) != SQLITE_OK)
    {
        message_publish(MSG_ERROR, "TsHsSqlitePlugin error opening database '%s': %s\n", mFilename.c_str(), sqlite3_errmsg(mDatabaseHandle));
        sqlite3_close(mDatabaseHandle);
        mDatabaseHandle = 0;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Opens the SQLite database and creates the database tables used for logging
///          Health and Status messages.
///
/// @return  True if the initialization succeeds, else false.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsSqlitePlugin::init(void)
{
    // If not enabled don't create the log file.
    if (!mEnabled)
    {
        return true;
    }

    // On the first pass, remove database from previous runs
    if (mFirstpass)
    {
        // If the system has been configured to use file_timestamped file names then create a new
        // file, otherwise overwrite the existing one.
        if (mOverwrite)
        {
            remove(mFilename.c_str());
        }
        else
        {
            mFilename += tsHsFileTimestamp();
        }

        mFirstpass = false;
    }

    // Initialize database file and create tables
    if (!openDatabase())
    {
        return false;
    }

    // Create the tables and indices
    // Check for NULL in case the mFirstpass block failed
    if (mDatabaseHandle)
    {
        char* sql_error;

        const char* creation_command =
                "CREATE TABLE IF NOT EXISTS timestamps(id INTEGER, met INTEGER, timestamp INTEGER);"
                "CREATE TABLE IF NOT EXISTS messages("
                    "id INTEGER PRIMARY KEY, file TEXT, line INTEGER, "
                    "type INTEGER, subsys TEXT, message TEXT, count INTEGER, last_time INTEGER);"
                "CREATE INDEX IF NOT EXISTS time_ids ON timestamps(id);";

        sqlite3_exec(mDatabaseHandle, creation_command, NULL, NULL, &sql_error);

        handleSqlError(__FILE__, __LINE__, &sql_error, true);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Restarts a plugin.
///
/// @return  True if successful, or false if initialization failed.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsSqlitePlugin::restart(void)
{
    // OBCS has requested that H&S not clear the log during restarts. They put emulator health info
    // in the log and they need continuity over restarts. They also continue to run during freeze.

    //mFirstpass       = true;
    //mTryLockFailures = 0;
    //mTransactionOpen = false;
    //shutdown();
    //return init();

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Updates the plugin
///
/// @return  True if the update succeeds, else false.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsSqlitePlugin::update(void)
{
    if (!mEnabled)
    {
        return true;
    }

    if (!mDatabaseHandle)
    {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Commits any pending transactions and closes the database.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsSqlitePlugin::shutdown(void)
{
    if (!mEnabled)
    {
        return;
    }

    if (!mDatabaseHandle)
    {
        return;
    }

    // Wait on any threads that may still be logging messages

    if (pthread_mutex_lock(&mResourceLock) == 0) // 0 means lock granted
    {
        sqlite3_close(mDatabaseHandle);
        pthread_mutex_unlock(&mResourceLock);
    }

    mDatabaseHandle = 0;

    if (mTryLockFailures > 0)
    {
        message_publish(MSG_WARNING, "TsHsSqlitePlugin skipped %d messages due to mutex conflicts\n", mTryLockFailures);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Hash function used to generate database key values.
///
/// @param[in] salt  (--) (unused).
/// @param[in] str   (--) The characters of this string are used to calculate the hash value.
///
/// @return  The hash value.
////////////////////////////////////////////////////////////////////////////////////////////////////
long long int TsHsSqlitePlugin::hashString(long long int salt __attribute__((unused)), const std::string& str)
{
    long long int hh = 0;
    const char* str_ptr = str.c_str();

    while (*str_ptr)
    {
        hh = hh * 101 + static_cast<long long int>(static_cast<unsigned char>(*str_ptr));
        str_ptr++;
    }

    return hh;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Log a health and status message to a SQLite database file.
///
/// @param[in] file      (--) name of file which initiated logging the message.
/// @param[in] line      (--) line of file which initiated logging the message.
/// @param[in] function  (--) The name of the function logging the message (unused).
/// @param[in] type      (--) the type of message (e.g. info, warning, etc.).
/// @param[in] subsys    (--) the subsystem from which the message originated.
/// @param[in] met       (--) the mission-elapsed time that the message was sent.
/// @param[in] timestamp (--) the unix timestamp that the message was sent.
/// @param[in] mtext     (--) the message text.
///
/// @return  True if successful, or false on failure.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsSqlitePlugin::msg(
   const std::string&   file,
   int                  line,
   const std::string&   function __attribute__((unused)),
   TS_HS_MSG_TYPE       type,
   const std::string&   subsys,
   const TS_TIMER_TYPE& met,
   unsigned long        timestamp,
   const std::string&   mtext)
{
    if (!mEnabled)
    {
        return true;
    }

    if (!mDatabaseHandle)
    {
        return false;
    }

    // Use a hash function to generate a unique database key value.
    long long int msg_hash;
    msg_hash = hashString(0, file);
    msg_hash = 101 * msg_hash + line;
    msg_hash = hashString(msg_hash, subsys);
    msg_hash = hashString(msg_hash, mtext);

    // This is not optimal - for new entries, we should just insert (skip the update)
    const char* insert_command_template =
            // insert the hash-message entry (if it's already there, carry on)
            "INSERT OR IGNORE INTO messages VALUES (%lld, '%q', %d, %d, '%q', '%q', 0, %d);"
            // increment the count
            "UPDATE messages SET count = count + 1, last_time = %d WHERE id = %lld;"
            // insert the timestamp entry
            "INSERT INTO timestamps VALUES (%lld, %d, %d);";

    // Fill-in the command template to produce a complete SQL command

    int met_seconds = static_cast<int> (floor(met.seconds));

    char* insert_command = sqlite3_mprintf(insert_command_template,
            msg_hash, file.c_str(), line, type, subsys.c_str(), mtext.c_str(), met_seconds, // INSERT
            met_seconds, msg_hash,                                  // UPDATE
            msg_hash, met_seconds, timestamp);                      // INSERT

    if (mBlocking)
    {
        // We will wait if necessary, no messages will be lost.
        if (pthread_mutex_lock(&mResourceLock) == 0) // 0 means lock granted
        {
            insertMessage(insert_command);
            pthread_mutex_unlock(&mResourceLock);
        }
    }
    else
    {
        // Don't wait. Discard message if resource conflict.
        if (pthread_mutex_trylock(&mResourceLock) == 0) // 0 means lock granted
        {
            insertMessage(insert_command);
            pthread_mutex_unlock(&mResourceLock);
            sqlite3_free(insert_command);
        }
        else
        {
            mTryLockFailures++;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Log a health and status message to a SQLite database file.
///
/// @param[in] insertCommand (--) sql command to insert message into database.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsSqlitePlugin::insertMessage(char* insertCommand)
{
    // todo: try catch here to ensure resource unlocks if exception
    char* sql_error;
    // Execute the SQL command to insert the new message into the database
    sqlite3_exec(mDatabaseHandle, "BEGIN TRANSACTION;", NULL, NULL, &sql_error);
    sqlite3_exec(mDatabaseHandle, insertCommand, NULL, NULL, &sql_error);
    sqlite3_exec(mDatabaseHandle, "COMMIT;", NULL, NULL, &sql_error);
    handleSqlError(__FILE__, __LINE__, &sql_error, false);

    return true;
}

