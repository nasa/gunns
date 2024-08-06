/****************************************** TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Health and status message configuration - code)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ()

PROGRAMMERS:
   (
     ((Wesley A. White) (Tietronix Software) (June 2013))
   )
**************************************************************************************************/
#include <cstdlib>
#include "TsHsConfig.hh"

const char* HS_CONFIG_FILE_NAME = "hsconfig.txt";

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsConfig::TsHsConfig() :
    mOverallBlocking(1),
    mTimerSeconds(0.0),
    mTerminationEnabled(1),
    mTerminationSubsystems(""),
    mQueueBlockingEnabled(1),
    mQueueCapacity(4000),
    mTextPluginEnabled(1),
    mTextPluginBlockingEnabled(1),
    mTextPluginOverwriteEnabled(1),
    mTextPluginPath("../../../logs/TS_Health_and_Status.out"),
    mSqlitePluginEnabled(1),
    mSqlitePluginBlockingEnabled(1),
    mSqlitePluginOverwriteEnabled(1),
    mSqlitePluginPath("../../../logs/TS_Health_and_Status.sqlite"),
    mConsolePluginEnabled(0),
    mConsolePluginBlockingEnabled(0),
    mFilterEnabled(1),
    mFilterBlockingEnabled(1),
    mRedundancyFilterInterval(2),
    mRedundancyFilterCount(1),
    mRedundancyFilterHistory(20),
    mCategoryFilterDebugEnabled(1),
    mCategoryFilterInfoEnabled(1),
    mCategoryFilterWarningsEnabled(1),
    mCategoryFilterErrorsEnabled(1),
    mCategoryFilterFatalsEnabled(1),
    mEnvConfigFilePath(0),
    mEnvTextPluginPath(0),
    mEnvSqlitePluginPath(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Copy constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsHsConfig::TsHsConfig(const TsHsConfig& config) :
    mOverallBlocking(config.mOverallBlocking),
    mTimerSeconds(config.mTimerSeconds),
    mTerminationEnabled(config.mTerminationEnabled),
    mTerminationSubsystems(config.mTerminationSubsystems),
    mQueueBlockingEnabled(config.mQueueBlockingEnabled),
    mQueueCapacity(config.mQueueCapacity),
    mTextPluginEnabled(config.mTextPluginEnabled),
    mTextPluginBlockingEnabled(config.mTextPluginBlockingEnabled),
    mTextPluginOverwriteEnabled(config.mTextPluginOverwriteEnabled),
    mTextPluginPath(config.mTextPluginPath),
    mSqlitePluginEnabled(config.mSqlitePluginEnabled),
    mSqlitePluginBlockingEnabled(config.mSqlitePluginBlockingEnabled),
    mSqlitePluginOverwriteEnabled(config.mSqlitePluginOverwriteEnabled),
    mSqlitePluginPath(config.mSqlitePluginPath),
    mConsolePluginEnabled(config.mConsolePluginEnabled),
    mConsolePluginBlockingEnabled(config.mConsolePluginBlockingEnabled),
    mFilterEnabled(config.mFilterEnabled),
    mFilterBlockingEnabled(config.mFilterBlockingEnabled),
    mRedundancyFilterInterval(config.mRedundancyFilterInterval),
    mRedundancyFilterCount(config.mRedundancyFilterCount),
    mRedundancyFilterHistory(config.mRedundancyFilterHistory),
    mCategoryFilterDebugEnabled(config.mCategoryFilterDebugEnabled),
    mCategoryFilterInfoEnabled(config.mCategoryFilterInfoEnabled),
    mCategoryFilterWarningsEnabled(config.mCategoryFilterWarningsEnabled),
    mCategoryFilterErrorsEnabled(config.mCategoryFilterErrorsEnabled),
    mCategoryFilterFatalsEnabled(config.mCategoryFilterFatalsEnabled),
    // WARNING: these values come from stdlib's getenv(). They are not dynamically allocated, so we do a shallow copy here.
    mEnvConfigFilePath(config.mEnvConfigFilePath),
    mEnvTextPluginPath(config.mEnvTextPluginPath),
    mEnvSqlitePluginPath(config.mEnvSqlitePluginPath)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Attempts to acquire H&S configuration info from files and environment variables.
///          Initial configuration values are set by the ctor initialization list. This method
///          first attempts to load a file named HS_CONFIG_FILE_NAME from the current directory.
///          If that fails, the method attempts to load the file from location $HS_CONFIG_PATH. If
///          that fails, the default ctor values are used. In all cases values may be overridden
///          after being set by file or ctor by $USS_HS_TEXT_PATH and/or $USS_HS_SQLITE_PATH.
///
/// @return  Returns false if a configuration file exist but contains bad content, else returns
///          true.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsConfig::getConfigInfo()
{
   // Get relevant environment variables
   mEnvConfigFilePath    = getenv("HS_CONFIG_PATH");
   mEnvTextPluginPath    = getenv("USS_HS_TEXT_PATH");
   mEnvSqlitePluginPath  = getenv("USS_HS_SQLITE_PATH");

   // Debug
   //cout << "mEnvConfigFilePath   = " << (mEnvConfigFilePath   ? mEnvConfigFilePath   : "null") << endl;
   //cout << "mEnvTextPluginPath   = " << (mEnvTextPluginPath   ? mEnvTextPluginPath   : "null") << endl;
   //cout << "mEnvSqlitePluginPath = " << (mEnvSqlitePluginPath ? mEnvSqlitePluginPath : "null") << endl;

   ifstream config_file;
   bool     load_status;

   // Attempt to load config file from current directory
   config_file.open(HS_CONFIG_FILE_NAME, ios::in);
   if (config_file)
   {
      load_status = loadConfigFile(config_file);
      applyPathOverrides();
      return(load_status);
   }

   if (mEnvConfigFilePath == 0)
   {
      // There's no path environment variable set, so apply overrides, if any
      // to the default values provided by the constructor.
      applyPathOverrides();
      return true;
   }

   string full_path_file_name = string(mEnvConfigFilePath) + "/" + string(HS_CONFIG_FILE_NAME);

   //cout << "full_path_file_name = " << full_path_file_name << endl;

   // Attempt to load config file from ${HS_CONFIG_FILE}

   // todo: need to reset ios flags here?
   config_file.open(full_path_file_name.c_str(), ios::in);
   if (config_file)
   {
      load_status = loadConfigFile(config_file);
      applyPathOverrides();
      return(load_status);
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Replaces the text plugin path and sqlite plugin path with values of environment
///          variables $USS_HS_TEXT_PATH and $USS_HS_SQLITE_PATH, respectively, provided the
///          environment variables are defined.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsConfig::applyPathOverrides()
{
      // Override plugin path if environment variables set

      if (mEnvTextPluginPath != 0)
      {
         mTextPluginPath = mEnvTextPluginPath;
      }

      if (mEnvSqlitePluginPath != 0)
      {
         mSqlitePluginPath = mEnvSqlitePluginPath;
      }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Loads an H&S configuration file from the specified stream.
///
/// @param[inout] file  (--) The input file.
///
/// @return  Returns true if all fields read and file's ios flag are good else returns false.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool TsHsConfig::loadConfigFile(ifstream& file)
{
   if (!file)
   {
      cerr << "TsHsConfig::loadConfigFile file in invalid state\n";
      return false;
   }

   file >> mOverallBlocking
        >> mTimerSeconds
        >> mTerminationEnabled
        >> mTerminationSubsystems
        >> mQueueBlockingEnabled
        >> mQueueCapacity;

   if (file.good() == false)
   {
        cout << "TsHsConfig::loadConfigFile failed to read line 1 data fields" << endl;
        return false;
   }

   file >> mTextPluginEnabled
        >> mTextPluginBlockingEnabled
        >> mTextPluginOverwriteEnabled
        >> mTextPluginPath;

   if (file.good() == false)
   {
        cout << "TsHsConfig::loadConfigFile failed to read line 2 data fields" << endl;
        return false;
   }

   file >> mSqlitePluginEnabled
        >> mSqlitePluginBlockingEnabled
        >> mSqlitePluginOverwriteEnabled
        >> mSqlitePluginPath;

   if (file.good() == false)
   {
        cout << "TsHsConfig::loadConfigFile failed to read line 3 data fields" << endl;
        return false;
   }

   file >> mConsolePluginEnabled
        >> mConsolePluginBlockingEnabled;

   if (file.good() == false)
   {
        cout << "TsHsConfig::loadConfigFile failed to read line 4 data fields" << endl;
        return false;
   }

   file >> mFilterEnabled
        >> mFilterBlockingEnabled;

   if (file.good() == false)
   {
        cout << "TsHsConfig::loadConfigFile failed to read line 5 data fields" << endl;
        return false;
   }

   file >> mRedundancyFilterInterval
        >> mRedundancyFilterCount
        >> mRedundancyFilterHistory;

   if (file.good() == false)
   {
        cout << "TsHsConfig::loadConfigFile failed to read line 6 data fields" << endl;
        return false;
   }

   file >> mCategoryFilterDebugEnabled
        >> mCategoryFilterInfoEnabled
        >> mCategoryFilterWarningsEnabled
        >> mCategoryFilterErrorsEnabled
        >> mCategoryFilterFatalsEnabled;

   if (file.good() == false)
   {
        cout << "TsHsConfig::loadConfigFile failed to read line 7 data fields" << endl;
        return false;
   }

   return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Debug method that dumps data members to the console.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsHsConfig::dumpConfig() const
{
   cout << " " << mOverallBlocking
        << " " << mTimerSeconds
        << " " << mTerminationEnabled
        << " '"<< mTerminationSubsystems << "'"
        << " " << mQueueBlockingEnabled
        << " " << mQueueCapacity
        << endl;

   cout << " " << mTextPluginEnabled
        << " " << mTextPluginBlockingEnabled
        << " " << mTextPluginOverwriteEnabled
        << " '"<< mTextPluginPath << "'"
        << endl;

   cout << " " << mSqlitePluginEnabled
        << " " << mSqlitePluginBlockingEnabled
        << " " << mSqlitePluginOverwriteEnabled
        << " '"<< mSqlitePluginPath << "'"
        << endl;

   cout << " " << mConsolePluginEnabled
        << " " << mConsolePluginBlockingEnabled
        << endl;

   cout << " " << mFilterEnabled
        << " " << mFilterBlockingEnabled
        << endl;

   cout << " " << mRedundancyFilterInterval
        << " " << mRedundancyFilterCount
        << " " << mRedundancyFilterHistory
        << endl;

   cout << " " << mCategoryFilterDebugEnabled
        << " " << mCategoryFilterInfoEnabled
        << " " << mCategoryFilterWarningsEnabled
        << " " << mCategoryFilterErrorsEnabled
        << " " << mCategoryFilterFatalsEnabled
        << endl;
}
