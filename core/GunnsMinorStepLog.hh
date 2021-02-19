#ifndef GunnsMinorStepLog_EXISTS
#define GunnsMinorStepLog_EXISTS

/**
@file
@brief    GUNNS Solver Minor Step Log declarations

@defgroup  TSM_GUNNS_CORE_MINOR_STEP_LOG    GUNNS Solver Minor Step Log
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsMinorStepLog.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2014-09) (Initial Prototype)

@{
*/

#include <vector>
#include "core/GunnsBasicLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  GUNNS Solver Minor Step Log Link Data Class
///
/// @details  Stores a network link's contributions to the system of equations, and the result
///           of its confirmation check on the solution, for a network minor step.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMinorStepLinkData
{
    public:
        GunnsBasicLink::SolutionResult mSolutionResult;   /**< ** (1) trick_chkpnt_io(**) Link result of solution confirmation check. */
        std::vector<double>            mAdmittanceMatrix; /**< ** (1) trick_chkpnt_io(**) Link admittance matrix. */
        std::vector<double>            mSourceVector;     /**< ** (1) trick_chkpnt_io(**) Link source vector. */
        /// @brief  Default constructor for the GUNNS Solver Minor Step Log Link Data.
        GunnsMinorStepLinkData(const int linkNumPorts = 0);
        /// @brief  Default destructor for the GUNNS Solver Minor Step Log Link Data.
        virtual ~GunnsMinorStepLinkData();
        /// @brief  Assignment operator for the GUNNS Solver Minor Step Log Link Data.
        GunnsMinorStepLinkData& operator =(const GunnsMinorStepLinkData& that);
        /// @brief  Copy constructor for the GUNNS Solver Minor Step Log Link Data.
        GunnsMinorStepLinkData(const GunnsMinorStepLinkData& that);
        /// @brief  Returns the link solution result as a string.
        std::string renderSolutionResult() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  GUNNS Solver Minor Step Log Data Class
///
/// @details  Stores a data for a network minor step, including system of equations and results of
///           the solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMinorStepData
{
    public:
        /// @brief Enumeration of the non-linear network solution results.
        enum SolutionResults {
            SUCCESS      = 0, ///< The major step was successfully finished.
            CONFIRM      = 1, ///< Minor step solution result was CONFIRM.
            REJECT       = 2, ///< Minor step solution result was REJECT.
            DELAY        = 3, ///< Minor step solution result was DELAY.
            DECOMP_LIMIT = 4, ///< Decomposition limit was exceeded (some other data will be invalid).
            MINOR_LIMIT  = 5, ///< Minor step limit was exceeded (some other data will be invalid).
            MATH_FAIL    = 6  ///< Solution math failure (some other data will be invalid).
        };
        int                                 mMajorStep;        /**< ** (1) trick_chkpnt_io(**) Total elapsed major step count since init. */
        int                                 mMinorStep;        /**< ** (1) trick_chkpnt_io(**) Minor step count within the major step. */
        int                                 mDecomposition;    /**< ** (1) trick_chkpnt_io(**) Decomposition count within the major step. */
        SolutionResults                     mSolutionResult;   /**< ** (1) trick_chkpnt_io(**) Solution result of the minor step. */
        std::vector<double>                 mPotentialVector;  /**< ** (1) trick_chkpnt_io(**) Solution potential vector of the minor step. */
        std::vector<double>                 mNodesConvergence; /**< ** (1) trick_chkpnt_io(**) Non-linear network nodes convergence of the minor step. */
        std::vector<GunnsMinorStepLinkData> mLinksData;        /**< ** (1) trick_chkpnt_io(**) Links data for the minor step. */
        /// @brief  Default constructor for the GUNNS Solver Minor Step Log Data.
        GunnsMinorStepData(const unsigned int networkSize = 0, const unsigned int numLinks = 0, GunnsBasicLink** links = 0);
        /// @brief  Default destructor for the GUNNS Solver Minor Step Log Data.
        virtual ~GunnsMinorStepData();
        /// @brief  Copy constructor for the GUNNS Solver Minor Step Log Link Data.
        GunnsMinorStepData(const GunnsMinorStepData& that);
        /// @brief  Assignment operator for the GUNNS Solver Minor Step Log Link Data.
        GunnsMinorStepData& operator =(const GunnsMinorStepData& that);
        /// @brief  Returns the network solution result as a string.
        std::string renderSolutionResult() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Solver Minor Step Log Input Data
///
/// @details  This class provides a data structure for the GUNNS Solver Minor Step Log input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMinorStepLogInputData
{
    public:
        /// @brief  Enumeration of the minor step log dump mode commands.
        enum LogCommands {
            PAUSE       = 0, ///< Stop recording data from the solver.
            RECORD_AUTO = 1, ///< Record data from the solver and enable automatic file dump when solver isn't successful, or when user inputs SNAP.
            RECORD_SNAP = 2, ///< Record data from the solver and file will be dumped when user inputs SNAP.
            SNAP        = 3, ///< File is dumped immediately.
        };
        LogCommands  mModeCommand; /**< (1) trick_chkpnt_io(**) Commanded operating mode. */
        unsigned int mLogSteps;    /**< (1) trick_chkpnt_io(**) Number of minor steps to record in each log. */
        std::string  mPath;        /**< (1) trick_chkpnt_io(**) File system relative path for output files to go. */
        /// @brief  Default constructs this GUNNS Solver Minor Step Log input data.
        GunnsMinorStepLogInputData(const LogCommands  modeCommand = PAUSE,
                                   const unsigned int logSteps    = 0,
                                   const std::string  path        = "");
        /// @brief  Default destructs this GUNNS Solver Minor Step Log input data.
        virtual ~GunnsMinorStepLogInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsMinorStepLogInputData(const GunnsMinorStepLogInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsMinorStepLogInputData& operator =(const GunnsMinorStepLogInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  GUNNS Solver Minor Step Log class.  This implements a ring buffer for storing GUNNS
///         solver minor step data -- one minor step of data for each frame in the buffer.
///         Interfaces are provided for the solver to initialize this object and record data for the
///         current minor step, and start and close out a major step.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMinorStepLog
{
    TS_MAKE_SIM_COMPATIBLE(GunnsMinorStepLog);
    public:
        /// @brief  Enumeration of the log modes.
        enum LogModes {
            PAUSED         = 0, ///< Not recording data from the solver.
            RECORDING_AUTO = 1, ///< Recording data from the solver, file will be dumped when solver isn't successful or when user commands SNAP.
            RECORDING_SNAP = 2, ///< Recording data from the solver, file will be dumped when user commands SNAP.
            WRITING_FILE   = 3  ///< File write is currently in progress and recording from solver is paused.
        };
        /// @brief  Default constructs the GUNNS Solver Minor Step Log.
        GunnsMinorStepLog();
        /// @brief  Default destructs the GUNNS Solver Minor Step Log.
        virtual ~GunnsMinorStepLog();
        /// @brief  Initializes the GUNNS Solver Minor Step Log.
        void initialize(const std::string& name, const int networkSize, const int numLinks, GunnsBasicLink** links);
        /// @brief  Begins the major step, handles moding commands.
        void beginMajorStep();
        /// @brief  Begins a minor step, records step counts.
        void beginMinorStep(const int majorStep, const int minorStep);
        /// @brief  Records the given potential vector.
        void recordPotential(const double* potential);
        /// @brief  Records the given nodes convergence values array.
        void recordNodesConvergence(const double* nodesConvergence);
        /// @brief  Records the network link contributions to the system of equations.
        void recordLinkContributions();
        /// @brief  Records the link solution confirmation result for the given network link index.
        void recordLinkResult(const unsigned int link, const GunnsBasicLink::SolutionResult result);
        /// @brief  Records the given solution confirmation result for all network links.
        void recordAllLinksResult(const GunnsBasicLink::SolutionResult result);
        /// @brief  Records the given network solution results, closes out this step and goes to the next step in the buffer.
        void recordStepResult(const int decomposition, const GunnsMinorStepData::SolutionResults result);
        /// @brief  Closes out the major step, initiates automatic file output.
        void endMajorStep();
        /// @brief  Asynchronous job function to write output data files to the file system.
        void updateAsync();
        /// @brief  Freeze job function to resize the log buffer.
        void updateFreeze();
        GunnsMinorStepLogInputData      mInputData;     /**<    (1) trick_chkpnt_io(**) Input data for initialization and run inputs. */

    protected:
        std::string                     mName;          /**< *o (1) trick_chkpnt_io(**) Object name for identification in outputs. */
        LogModes                        mState;         /**< *o (1) trick_chkpnt_io(**) Operating mode of the log dump function. */
        bool                            mIsRecording;   /**< *o (1) trick_chkpnt_io(**) Recording data from the solver when set. */
        std::vector<GunnsMinorStepData> mBuffer;        /**< ** (1) trick_chkpnt_io(**) The ring buffer of minor step data. */
        unsigned int                    mSize;          /**< *o (1) trick_chkpnt_io(**) The current size, in number of minor steps, of the ring buffer. */
        unsigned int                    mHeadIndex;     /**< *o (1) trick_chkpnt_io(**) The index of the currently logging step in the ring buffer. */
        unsigned int                    mNumValidSteps; /**< *o (1) trick_chkpnt_io(**) The number of steps in the ring buffer containing valid data, up to mSize. */
        GunnsBasicLink**                mLinks;         /**< ** (1) trick_chkpnt_io(**) Array of pointers to the network links. */
        std::string                     mLogHeader;     /**< ** (1) trick_chkpnt_io(**) Output file header row (column labels). */
        std::string                     mFilename;      /**< *o (1) trick_chkpnt_io(**) Command to initiate file write from the async thread with this name. */
        /// @brief  Validates the initialization of this object.
        void validate(const std::string& name, const int networkSize, const int numLinks, GunnsBasicLink** links);
        /// @brief  Builds the output file header row string.
        void initLogHeader();
        /// @brief  Steps the log to the next frame in the ring buffer.
        void step();
        /// @brief  Stops recording data from the solver.
        void stopLogging();
        /// @brief  Starts recording data from the solver.
        void startLogging();
        /// @brief  Returns the formatted output file contents as a string.
        std::string renderOutput();
        /// @brief  Initiates output file write of the log data.
        void initiateDump();
        /// @brief  Returns a mutable reference to the data at the current ring buffer head.
        GunnsMinorStepData& head();
        /// @brief  Returns a const reference to the data at an offset from the current ring buffer head.
        const GunnsMinorStepData& headOffset(const int offset = 0) const;

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsMinorStepLog& operator =(const GunnsMinorStepLog&);
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsMinorStepLog(const GunnsMinorStepLog&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  If logging is enabled, then advances the ring buffer to the next minor step by
///           increasing the head index, with wrap-around, and increments the count of valid steps.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsMinorStepLog::step()
{
    if (mIsRecording) {
        mHeadIndex++;
        if (mHeadIndex >= mSize) {
            mHeadIndex = 0;
        }
        if (mNumValidSteps < mSize) {
            mNumValidSteps++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets mIsRecording to true, enabling data recording functions and ring buffer head
///           advancing.  Also resets mHeadIndex and mNumValidSteps, which effectively clears the
///           buffer so recording will start with an empty buffer.   The old buffer is not actually
///           cleared, but those old data frames will not be output until new data is recorded in
///           them.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsMinorStepLog::startLogging()
{
    mHeadIndex     = mSize - 1;
    mNumValidSteps = 0;
    mIsRecording   = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets mIsRecording to false.  This prevents the recording functions from storing data,
///           and prevents the step function from advancing the ring buffer head.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsMinorStepLog::stopLogging()
{
    mIsRecording = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsMinorStepData& (--) Reference to the data at the current ring buffer head.
///
/// @details  Returns a reference to the data at the current ring buffer head.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsMinorStepData& GunnsMinorStepLog::head()
{
    return mBuffer[mHeadIndex];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  offset  (--)  Frame offset relative to the current head to retrieve.
///
/// @returns  GunnsMinorStepData& (--) Const reference to the data.
///
/// @details  Returns a const reference to the data at the given offset from the current ring buffer
///           head.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsMinorStepData& GunnsMinorStepLog::headOffset(const int offset) const
{
    //TODO what if offset wraps around multiple times? would be an invalid argument...
    int index = mHeadIndex + offset;
    if (index < 0) {
        index += mSize;
    } else if (index >= static_cast<int>(mSize)) {
        index -= mSize;
    }
    return mBuffer[index];
}

#endif
