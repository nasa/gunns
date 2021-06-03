#ifndef TsHsConfig_EXISTS
#define TsHsConfig_EXISTS

/**
@defgroup  TSM_UTILITIES_SIMULATION_HS_CONFIGURATION Configuration
@ingroup   TSM_UTILITIES_SIMULATION_HS

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Health & Status message configuration class. Locates relevant configuration files
   and environment variables and captures the configuration information there in
   for use configuring the H&S logging system.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((TsHsConfig.o))

PROGRAMMERS:
- (
    ((Wesley A. White) (Tietronix Software) (June 2013))
  )

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


/// @brief This class captures H&S configuration information from a variety of places. This
///        include environment variables and configuration files.
class TsHsConfig
{
public:
   /// @brief Default constructor.
   TsHsConfig();

   /// @brief Copy constructor.
   TsHsConfig(const TsHsConfig& config);

   /// @brief Destructor.
  ~TsHsConfig() {}

   /// @brief Attempts to acquire H&S configuration info from files and environment variables.
   bool getConfigInfo();

   /// @brief Loads an H&S configuration file from the specified stream.
   bool loadConfigFile(ifstream& file);

   /// @brief Replaces the text plugin path and sqlite plugin path with values of environment variables
   void applyPathOverrides();

   /// @brief Debug method that dumps data members to the console.
   void dumpConfig() const;

   // General config
   int    mOverallBlocking;                 //    (--) xxx
   double mTimerSeconds;                    //    (--) Initialize timer used to time-stamp message - not applicable if time comes from environment.
   int    mTerminationEnabled;              //    (--) Allow H&S to terminate simulation after for TS_HS_FATAL messages
   string mTerminationSubsystems;           //    (--) Restricts termination to listed subsystems - only applies if terminations enabled.

   // Queue config
   int    mQueueBlockingEnabled;            //    (--) If non-zero simultaneous attempt to insert into queue will result in threads waiting.
   int    mQueueCapacity;                   //    (--) The maximum number of logging requests that will fit in the queue without overflow.

   // Text plugin
   int    mTextPluginEnabled;               //    (--) Enables the text plugin.
   int    mTextPluginBlockingEnabled;       //    (--) Enables text plugin blocking.
   int    mTextPluginOverwriteEnabled;      //    (--) Enables overwriting of existing log file verses using time-stamped log file names.
   string mTextPluginPath;                  //    (--) Path and file name to text log file.

   // Sqlite plugin
   int    mSqlitePluginEnabled;             //    (--) Enables the SQLite plugin.
   int    mSqlitePluginBlockingEnabled;     //    (--) Enables SQLite plugin blocking.
   int    mSqlitePluginOverwriteEnabled;    //    (--) Enabled overwriting of existing SQLite database files verses using time-stamped DB file names.
   string mSqlitePluginPath;                //    (--) Path and file name to SQLite database file.

   // Console plugin
   int    mConsolePluginEnabled;            //    (--) Enables the console plugin.
   int    mConsolePluginBlockingEnabled;    //    (--) Enables console plugin blocking.

   // General filter config
   int    mFilterEnabled;                   //    (--) Enabled suppression of messages.
   int    mFilterBlockingEnabled;           //    (--) Enabled blocking to access the filter resource.

   // Redundancy filter config
   int    mRedundancyFilterInterval;        //    (s) A sliding window of time in which to look for duplicates.
   int    mRedundancyFilterCount;           //    (--) Number of duplicates to allow before suppressing subsequent copies.
   int    mRedundancyFilterHistory;         //    (--) Upper bound on the number of messages that are remembered within the filter interval.

   // Category filter config
   int    mCategoryFilterDebugEnabled;      //    (--) If enabled debug messages are suppressed.
   int    mCategoryFilterInfoEnabled;       //    (--) If enabled info messages are suppressed.
   int    mCategoryFilterWarningsEnabled;   //    (--) If enabled warning messages are suppressed.
   int    mCategoryFilterErrorsEnabled;     //    (--) If enabled error messages are suppressed.
   int    mCategoryFilterFatalsEnabled;     //    (--) If enabled fatal messages are suppressed.

   // Environment variables
   char*  mEnvConfigFilePath;               //    (--) Value of optional environment variable that specifies location of H&S config file.
   char*  mEnvTextPluginPath;               //    (--) Value of optional environment variable that specifies path to text log file.
   char*  mEnvSqlitePluginPath;             //    (--) Value of optional environment variable that specifies path to SQLite DB log file.

private:
   const TsHsConfig& operator=(const TsHsConfig&); // not implemented
};
/// @}

#endif /* TsHsConfig_EXISTS */
