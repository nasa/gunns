/**
@file
@brief    GUNNS Solver Minor Step Log File Dump Spotter implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((GunnsNetworkSpotter.o)
    (simulation/hs/TsHsMsg.o)
    (software/exceptions/TsInitializationException.o))
*/

#include <iostream>
#include <iomanip>
#include <fstream>

#include "GunnsMinorStepLogDumper.hh"
#include "GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Solver Minor Step Log File Dump Spotter configuration
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLogDumperConfigData::GunnsMinorStepLogDumperConfigData(const std::string& name)
    :
    GunnsNetworkSpotterConfigData(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Solver Minor Step Log File Dump Spotter configuration
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLogDumperConfigData::~GunnsMinorStepLogDumperConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  logMode   (--)  Initial operating mode of the log dump function.
/// @param[in]  logSteps  (--)  Initial & default number of major steps to record in each log.
/// @param[in]  path      (--)  File system relative path for output files to go.
///
/// @details  Default constructs this GUNNS Solver Minor Step Log File Dump Spotter input data.
///
/// @note     The mPath variable should be initialized via Trick input file.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLogDumperInputData::GunnsMinorStepLogDumperInputData(
        const GunnsMinorStepLogDumper::LogModes logMode,
        const int                               LogSteps,
        const std::string                       path)
    :
    mLogMode(logMode),
    mLogSteps(LogSteps),
    mPath(path)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Solver Minor Step Log File Dump Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLogDumperInputData::~GunnsMinorStepLogDumperInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  solver  (--)  Reference to the network solver.
/// @param[in]  links   (--)  Reference to the network links vector.
///
/// @details  Constructs the GUNNS Solver Minor Step Log File Dump Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLogDumper::GunnsMinorStepLogDumper(Gunns&                              solver,
                                                 const std::vector<GunnsBasicLink*>& links)
    :
    mSolverLog(solver.getMinorStepLog()),
    mLinks(links),
    mLogMode(STANDBY),
    mDefaultLogSteps(0),
    mPath(""),
    mLogSteps(0),
    mQueueSize(0),
    mLocked(false),
    mFileMessages(),
    mLogTableHeader(""),
    mLinkNamesTable("")
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GUNNS Solver Minor Step Log File Dump Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLogDumper::~GunnsMinorStepLogDumper()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Solver Minor Step Log File Dump Spotter with its configuration
///           and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLogDumper::initialize(const GunnsNetworkSpotterConfigData* configData,
                                         const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & type-cast config & input data.
    validateConfig(configData);
    const GunnsMinorStepLogDumperInputData*  input  = validateInput(inputData);

    /// - Initialize from input data.
    mDefaultLogSteps = input->mLogSteps;
    mLogMode         = input->mLogMode;
    mPath            = input->mPath;

    /// - Initialize state attributes.
    mLogSteps        = mDefaultLogSteps;
    mQueueSize       = 0;
    mLocked          = false;
    mFileMessages.clear();
    initLinkNamesTable();

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsMinorStepLogDumperConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsMinorStepLogDumperConfigData* GunnsMinorStepLogDumper::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsMinorStepLogDumperConfigData* result = dynamic_cast<const GunnsMinorStepLogDumperConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsMinorStepLogDumperInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsMinorStepLogDumperInputData* GunnsMinorStepLogDumper::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsMinorStepLogDumperInputData* result = dynamic_cast<const GunnsMinorStepLogDumperInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }

    /// - Throw exception on initial log mode = LOGGING.
    if (GunnsMinorStepLogDumper::LOGGING == result->mLogMode) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Initial log mode of LOGGING is not allowed.");
    }

    /// - Throw exception on default steps < 0.
    if (result->mLogSteps < 0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Initial log steps < 0.");
    }

    /// - Throw exception on empty path.
    if (result->mPath.empty()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Empty output file path.");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (unused).
///
/// @details  Updates the log mode, and if a log is in progress, builds the log data for eventual
///           output to a file.  This handshakes with the updateAsync() method via the mLocked flag
///           so that the updaetAsync() method doesn't interfere with the message vector when we are
///           writing to it here.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLogDumper::stepPostSolver(const double dt __attribute__((unused)))
{
    /// - Initialize the log table header based on number of link & node words.  This can't be
    ///   called until the first frame in run because the solver's initialize, which sets the number
    ///   of node words, doesn't happen until after this spotter's initialize.
    if (mLogTableHeader.empty()) {
        initLogTableHeader();
    }

    /// - Force the mode to STANDBY so that nothing will happen if the number of log steps somehow
    ///   gets to zero, or if the user tries to set LOGGING mode directly.
    if ( (1 > mLogSteps) or (LOGGING == mLogMode and not mLocked) ) {
        mLogMode  = STANDBY;
        mLogSteps = mDefaultLogSteps;
    }

    /// - There is a log in progress, or just started by user, or just started automatically.
    if ( (LOGGING == mLogMode) or (SNAP == mLogMode) or
        ((AUTO    == mLogMode) and not (GunnsMinorStepLog::SUCCESS == mSolverLog.mResult)) ) {

        /// - If a new log just started, create a new empty message and pop it onto the message
        ///   vector.
        if (LOGGING != mLogMode) {

            mLocked = true;
            GunnsMinorStepLogMessage message;
            mFileMessages.push_back(message);

            std::ostringstream file;
            file << mPath << "GUNNS_minor_step_log_" << mName << "_" << mSolverLog.mMajorStep << ".txt";
            mFileMessages.back().mFileName = file.str();
            mFileMessages.back().mData     = "";
        }

        /// - Add this major step's log & results to the currently logging message.
        mFileMessages.back().mData += formatLogData();

        /// - Decrement the step counter until we've logged the requested number of major steps.
        mLogMode = LOGGING;
        mLogSteps--;

        /// - Finish up the log message and flag it for file write.
        if (0 == mLogSteps) {
            std::ostringstream eof;
            eof << std::endl << "End of log: " << mFileMessages.back().mFileName << std::endl;
            mFileMessages.back().mData += eof.str();
            mFileMessages.back().mData += mLinkNamesTable;

            mLogMode  = STANDBY;
            mLogSteps = mDefaultLogSteps;
            mLocked   = false;
        }
    }
    mQueueSize = mFileMessages.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Writes one log message file per pass.  If there are multiple messages in the queue,
///           they are written in order of most recent.  This method should be called in an
///           asynchronous thread that does not interfere with the real-time GUNNS network thread.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLogDumper::updateAsync()
{
    if (not mLocked) {
        if (0 < mFileMessages.size()) {

            /// - Make a local copy of the message and pop it off the vector before we start the
            ///   file write to minimize interference with the real-time thread.
            GunnsMinorStepLogMessage message = mFileMessages.back();
            mFileMessages.pop_back();

            /// - Overwrite an old file if it has the same filename.
            std::string pathFile = message.mFileName;
            std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
            if (file.fail()) {
                GUNNS_WARNING("error opening file: " << pathFile);
            } else {
                GUNNS_INFO("writing file: " << pathFile);
                file << message.mData;
            }
            file.close();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   std::string  (--)  Minor step log of the current network solver major step.
///
/// @details  Writes minor step history of the current network solver major step into a formatted
///           string for output to a file.  This includes the node convergence bits and link
///           rejection bits for each minor step, which solver major step this is for, and what the
///           outcome of the solution was.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string GunnsMinorStepLogDumper::formatLogData()
{
    /// - Add the dump header.
    std::ostringstream data;
    data << std::endl << "GUNNS Minor Step Log for Solver: " << mName << " on Major Step: "
            << mSolverLog.mMajorStep;
    switch (mLogMode) {
        case (AUTO) :
            data << ", initiated automatically by solver.";
            break;
        case (SNAP) :
            data << ", initiated by snap command.";
            break;
        default :
            data << ", continuing log...";
            break;
    }

    /// - Add the minor step log header.
    data << mLogTableHeader;

    /// - Add the minor step log node & link bits for each minor step:
    ///    1 | 0000000f | 00000000
    ///    2 | 0000000e | 00000000
    for (int step = 0; step < mSolverLog.mMinorStep-1; ++step) {
        data << std::setw(5) << (step+1) << " |";
        for (int word = mSolverLog.mNumNodeWords-1; word > -1; --word) {
            data << " " << std::setw(8) << std::setfill('0') << std::hex << mSolverLog.mNodeBits[step][word].to_ulong();
        }
        data << " |";
        for (int word = mSolverLog.mNumLinkWords-1; word > -1; --word) {
            data << " " << std::setw(8) << std::setfill('0') << std::hex << mSolverLog.mLinkBits[step][word].to_ulong();
        }
        data << std::setfill(' ') << std::dec << std::endl;
    }

    /// - Add the result of the major step.
    data << std::setw(5) << mSolverLog.mMinorStep << " | ";
    switch (mSolverLog.mResult) {
        case (GunnsMinorStepLog::MINOR) :
            data << "***Solver failed to converge*** and gave up when minor step limit was exceeded." << std::endl;
            break;
        case (GunnsMinorStepLog::DECOMP) :
            data << "***Solver failed to converge*** and gave up when decomposition limit was exceeded." << std::endl;
            break;
        default :
            data << "Solver successfully finished." << std::endl;
            break;
    }
    return data.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the header rows for the minor step log table for output to debug file dumps:
///           Minor | Nodes:            | Links:
///           Step: |  63---32  31----0 |  63---32  31----0
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLogDumper::initLogTableHeader()
{
    std::ostringstream header;
    header << std::endl << "Minor | Nodes:   ";
    for (int word = mSolverLog.mNumNodeWords-2; word > -1; --word) {
        header << "         ";
    }
    header << "| Links:" << std::endl << "Step: |";
    for (int word = mSolverLog.mNumNodeWords-1; word > -1; --word) {
        header << std::setw(4) << ((word+1)*mSolverLog.mWordSize - 1) << std::setw(5) << std::setfill('-') << (word*mSolverLog.mWordSize) << std::setfill(' ');
    }
    header << " |";
    for (int word = mSolverLog.mNumLinkWords-1; word > -1; --word) {
        header << std::setw(4) << ((word+1)*mSolverLog.mWordSize - 1) << "-" << std::setw(4) << std::setfill('-') << (word*mSolverLog.mWordSize) << std::setfill(' ');
    }
    header << std::endl;
    mLogTableHeader = header.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the link name table for output to debug file dumps:
///           Link # | Name
///                0 | eps.pri.RPCM_1X
///                1 | eps.pri.RPCM_2X
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLogDumper::initLinkNamesTable()
{
    std::ostringstream table;
    table << std::endl << "Link # | Name" << std::endl;
    for (unsigned int link = 0; link < mLinks.size(); ++link) {
        table << std::setw(6) << link << " | " << mLinks[link]->getName() << std::endl;
    }
    mLinkNamesTable = table.str();
}
