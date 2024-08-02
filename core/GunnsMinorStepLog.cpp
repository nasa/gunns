/**
@file
@brief    GUNNS Solver Minor Step Log implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((simulation/hs/TsHsMsg.o)
    (software/exceptions/TsInitializationException.o))
*/

#include <iostream>
#include <fstream>
#include <cstddef> // size_t
#include "GunnsMinorStepLog.hh"
#include "GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] linkNumPorts (--) Number of ports in the link.
///
/// @details   Default constructs this minor step link data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLinkData::GunnsMinorStepLinkData(const int linkNumPorts)
    :
    mSolutionResult(GunnsBasicLink::CONFIRM),
    mAdmittanceMatrix(),
    mSourceVector()
{
    for (int i=0; i<linkNumPorts*linkNumPorts; ++i) {
        mAdmittanceMatrix.push_back(0.0);
    }
    for (int i=0; i<linkNumPorts; ++i) {
        mSourceVector.push_back(0.0);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this minor step link data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLinkData::~GunnsMinorStepLinkData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  The object to get assignment values from.
///
/// @details  Assignment operator for this minor step link data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLinkData& GunnsMinorStepLinkData::operator =(const GunnsMinorStepLinkData& that)
{
    if (this != &that) {
        mSolutionResult   = that.mSolutionResult;
        mAdmittanceMatrix = that.mAdmittanceMatrix;
        mSourceVector     = that.mSourceVector;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  The object to copy.
///
/// @details  Copy constructs this minor step link data.  This is a deep copy.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLinkData::GunnsMinorStepLinkData(const GunnsMinorStepLinkData& that)
{
    *this = that;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  std::string (--) Text representation of the link solution result enumeration.
///
/// @details  Returns a text string representation of the link's solution result enumeration value.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string GunnsMinorStepLinkData::renderSolutionResult() const
{
    switch (mSolutionResult) {
    case GunnsBasicLink::CONFIRM:
        return "CONFIRM";
    case GunnsBasicLink::REJECT:
        return "REJECT";
    case GunnsBasicLink::DELAY:
        return "DELAY";
    default:
        return "invalid";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] networkSize  (--) Number of rows in the network system of equations.
/// @param[in] numLinks     (--) Number of links in the network.
/// @param[in] links        (--) Array of pointers to the network links.
///
/// @details   Default constructs this minor step data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepData::GunnsMinorStepData(const unsigned int networkSize, const unsigned int numLinks,
                                       GunnsBasicLink** links)
    :
    mMajorStep(0),
    mMinorStep(0),
    mDecomposition(0),
    mSolutionResult(SUCCESS),
    mPotentialVector(),
    mNodesConvergence(),
    mLinksData()
{
    for (unsigned int i=0; i<networkSize; ++i) {
        mPotentialVector.push_back(0.0);
        mNodesConvergence.push_back(0.0);
    }
    for (unsigned int i=0; i<numLinks; ++i) {
        mLinksData.push_back(GunnsMinorStepLinkData(links[i]->getNumberPorts()));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this minor step data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepData::~GunnsMinorStepData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  The object to get assignment values from.
///
/// @details  Assignment operator for this minor step data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepData& GunnsMinorStepData::operator =(const GunnsMinorStepData& that)
{
    if (this != &that) {
        mMajorStep        = that.mMajorStep;
        mMinorStep        = that.mMinorStep;
        mSolutionResult   = that.mSolutionResult;
        mDecomposition    = that.mDecomposition;
        mPotentialVector  = that.mPotentialVector;
        mNodesConvergence = that.mNodesConvergence;
        mLinksData        = that.mLinksData;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Copy constructs this minor step data.  This is a deep copy.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepData::GunnsMinorStepData(const GunnsMinorStepData& that)
{
    *this = that;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::string  (--)  Network solution result as a string.
///
/// @details  Returns a text string representation of the network solution result enumeration
///           value.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string GunnsMinorStepData::renderSolutionResult() const
{
    switch (mSolutionResult) {
    case SUCCESS:
        return "SUCCESS";
    case CONFIRM:
        return "CONFIRM";
    case REJECT:
        return "REJECT";
    case DELAY:
        return "DELAY";
    case DECOMP_LIMIT:
        return "DECOMP_LIMIT";
    case MINOR_LIMIT:
        return "MINOR_LIMIT";
    case MATH_FAIL:
        return "MATH_FAIL";
    default:
        return "invalid";
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  modeCommand  (--)  Commanded operating mode.
/// @param[in]  logSteps     (--)  Number of minor steps to record in each log.
/// @param[in]  path         (--)  File system relative path for output files to go.
///
/// @details  Default constructs this minor step input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLogInputData::GunnsMinorStepLogInputData(const LogCommands  modeCommand,
                                                       const unsigned int logSteps,
                                                       const std::string  path)
    :
    mModeCommand(modeCommand),
    mLogSteps(logSteps),
    mPath(path)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this minor step input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLogInputData::~GunnsMinorStepLogInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the GUNNS Solver Minor Step Log.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLog::GunnsMinorStepLog()
    :
    mInputData(),
    mName(""),
    mState(PAUSED),
    mIsRecording(false),
    mBuffer(),
    mSize(0),
    mHeadIndex(0),
    mNumValidSteps(0),
    mLinks(0),
    mLogHeader(""),
    mFilename("")
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the GUNNS Solver Minor Step Log.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMinorStepLog::~GunnsMinorStepLog()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  networkSize  (--)  Number of nodes in the network, not counting the ground node.
/// @param[in]  numLinks     (--)  Number of links in the network.
/// @param[in]  links        (--)  Array of pointers to the network links.
///
/// @throws  TsInitializationException
///
/// @details  Initializes this GUNNS Solver Minor Step Log.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::initialize(const std::string& name, const int networkSize, const int numLinks, GunnsBasicLink** links)
{
    /// - Default the output file path if the input data is empty.
    if (mInputData.mPath.empty()) {
        mInputData.mPath = "./";
    }
    validate(name, networkSize, numLinks, links);

    /// - Create an initial empty frame of data with its links sized properly for the network;
    ///   This will be copied when the buffer size is initialized.
    mBuffer.push_back(GunnsMinorStepData(networkSize, numLinks, links));
    mHeadIndex     = mSize - 1;
    mNumValidSteps = 0;
    mLinks         = links;

    /// - Initialize the log header row string.
    initLogHeader();

    /// - Initialize the buffer size.
    updateFreeze();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Handles user mode commands and most state moding at the beginning of the solver major
///           step.  Pauses and resumes recording in response to commands, and initiates log file
///           write for the SNAP command.
///
/// @note  This should be the first function called by the solver in a major step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::beginMajorStep()
{
    if (0 == mSize) {
        mInputData.mModeCommand = GunnsMinorStepLogInputData::PAUSE;
        mState = PAUSED;
    } if (WRITING_FILE != mState) {
        switch (mInputData.mModeCommand) {
        case GunnsMinorStepLogInputData::PAUSE:
            stopLogging();
            mState = PAUSED;
            break;
        case GunnsMinorStepLogInputData::RECORD_AUTO:
            if (PAUSED == mState) {
                startLogging();
            }
            mState = RECORDING_AUTO;
            break;
        case GunnsMinorStepLogInputData::RECORD_SNAP:
            if (PAUSED == mState) {
                startLogging();
            }
            mState = RECORDING_SNAP;
            break;
        case GunnsMinorStepLogInputData::SNAP:
            /// - When snap command is processed, reset the command input to reflect the previous
            ///   state.  For example snapping from RECORDING_AUTO state returns the command to
            ///   RECORD_AUTO.
            if (PAUSED == mState) {
                mInputData.mModeCommand = GunnsMinorStepLogInputData::PAUSE;
            } else if (RECORDING_AUTO == mState) {
                mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_AUTO;
            } else {
                mInputData.mModeCommand = GunnsMinorStepLogInputData::RECORD_SNAP;
            }
            initiateDump();
            break;
        default:
            GUNNS_WARNING("invalid mModeCommand value.");
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  majorStep  (--)  Major step count.
/// @param[in]  minorStep  (--)  Minor step count in the current major step.
///
/// @details  If recording is enabled, calls the step function to advance the ring buffer to the
///           next step, and record the major and minor step values for the new step.  Also records
///           CONFIRM for initial link results; these may be overwritten later in the step by the
///           solver.
///
/// @note  This should be the first function called by the solver in a minor step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::beginMinorStep(const int majorStep, const int minorStep)
{
    if (mIsRecording) {
        step();
        head().mMajorStep = majorStep;
        head().mMinorStep = minorStep;
        recordAllLinksResult(GunnsBasicLink::CONFIRM);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  potential (--) Array of network solution potentials.
///
/// @details  If recording is enabled, records the given solver potential vector to the ring buffer
///           head.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::recordPotential(const double* potential)
{
    if (mIsRecording) {
        head().mPotentialVector.assign(potential, potential + head().mPotentialVector.size());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodesConvergence (--) Array of network solution node convergence values.
///
/// @details  If recording is enabled, records the given solver nodes convergence array to the ring
///           buffer head.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::recordNodesConvergence(const double* nodesConvergence)
{
    if (mIsRecording) {
        head().mNodesConvergence.assign(nodesConvergence, nodesConvergence + head().mNodesConvergence.size());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  If recording is enabled, records all of the link's current admittance matrix and
///           source vector values to the ring buffer head.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::recordLinkContributions()
{
    if (mIsRecording) {
        /// - Store the ring buffer head reference since we're going to reference it many times.
        GunnsMinorStepData& data = head();
        for (unsigned int link=0; link<data.mLinksData.size(); ++link) {
            double* source = mLinks[link]->getSourceVector();
            data.mLinksData[link].mSourceVector.assign(source, source + data.mLinksData[link].mSourceVector.size());

            double* admittance = mLinks[link]->getAdmittanceMatrix();
            data.mLinksData[link].mAdmittanceMatrix.assign(admittance, admittance + data.mLinksData[link].mAdmittanceMatrix.size());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  link    (--)  Index of the link to record.
/// @param[in]  result  (--)  The indexed link's solution confirmation result.
///
/// @details  If recording is enabled, records the given solution result for the given link index to
///           the ring buffer head.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::recordLinkResult(const unsigned int link, const GunnsBasicLink::SolutionResult result)
{
    if (mIsRecording) {
        head().mLinksData[link].mSolutionResult = result;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  result  (--)  The link solution confirmation result to record for all links.
///
/// @details  If recording is enabled, records the given solution result for all of the network
///           links to the ring buffer head.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::recordAllLinksResult(const GunnsBasicLink::SolutionResult result)
{
    for (unsigned int link=0; link<head().mLinksData.size(); ++link) {
        recordLinkResult(link, result);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  decomposition  (--)  Decomposition count in the current major step.
/// @param[in]  result         (--)  Solution result of the minor step.
///
/// @details  If recording is enabled, records the given network solution result and decomposition
///           count to the ring buffer head.
///
/// @note  This should be the last function called by the solver in a minor step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::recordStepResult(const int decomposition, const GunnsMinorStepData::SolutionResults result)
{
    if (mIsRecording) {
        head().mDecomposition  = decomposition;
        head().mSolutionResult = result;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  After the solver major step has completed, this initiates log file write from the
///           RECORD_AUTO state if the latest record has any non-success indication from the solver.
///
/// @note  This should be the last function called by the solver in a major step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::endMajorStep()
{
    if (RECORDING_AUTO == mState and GunnsMinorStepData::SUCCESS != head().mSolutionResult) {
        initiateDump();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Writes the output file if the output file name has been set.  This method should be
///           called in an asynchronous thread that does not interfere with the real-time GUNNS
///           network thread.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::updateAsync()
{
    if (not mFilename.empty()) {
        std::ofstream file (mFilename.c_str(), (std::ofstream::out | std::ofstream::trunc));
        if (file.fail()) {
            GUNNS_WARNING("error opening file: " << mFilename);
        } else {
            GUNNS_INFO("writing file: " << mFilename);
            file << renderOutput();
        }
        file.close();

        /// - Reset the file write command and the logger state to paused when done.
        mFilename = "";
        mState = PAUSED;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  If the commanded size is different from the current buffer size, then clear the buffer
///           vector and refill the vector with fill data to the new given size.  The startLogging
///           function is called to reset the buffer head and valid frames count, and enable
///           recording from the solver.
///
/// @note  This should only be called in simulation initialization or freeze, because it can take
///        a long time complete and could cause frame overruns if called during run.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::updateFreeze()
{
    if (mSize != mInputData.mLogSteps and mInputData.mLogSteps > 0 and mBuffer.size() > 0) {
        /// - Save a frame of data for sizing of frames after the vector has been cleared.
        GunnsMinorStepData saveFrame(mBuffer[0]);

        mBuffer.clear();
        mSize = mInputData.mLogSteps;
        for (unsigned int i=0; i<mSize; ++i) {
            mBuffer.push_back(GunnsMinorStepData(saveFrame));
        }
        startLogging();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name         (--)  Name of the object.
/// @param[in]  networkSize  (--)  Number of nodes in the network, not counting the ground node.
/// @param[in]  numLinks     (--)  Number of links in the network.
/// @param[in]  links        (--)  Array of pointers to the network links.
///
/// @throws  TsInitializationException
///
/// @details  Validates the initialization data for this GUNNS Solver Minor Step Log, and stores the
///           given object name.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::validate(const std::string& name, const int networkSize, const int numLinks, GunnsBasicLink** links)
{
    /// - Throw exception on empty name.
    if (name.empty()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "empty object name.");
    }
    mName = name;

    /// - Throw exception on network size < 1.
    if (networkSize < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "network size < 1.");
    }

    /// - Throw exception on number of links < 1.
    if (numLinks < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "number of network links < 1.");
    }

    /// - Throw exception on null links pointer
    if (not links) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "null links pointer");
    }

    /// - Throw exception on initial log mode command = SNAP.
    if (GunnsMinorStepLogInputData::SNAP == mInputData.mModeCommand) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "initial mode command of SNAP is not allowed.");
    }

    /// - Throw on initial recording command but zero log steps.
    if (0 == mInputData.mLogSteps and
            (GunnsMinorStepLogInputData::RECORD_AUTO == mInputData.mModeCommand or
             GunnsMinorStepLogInputData::RECORD_SNAP == mInputData.mModeCommand)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "initial log steps = 0 with a recording command set.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the header row for the output file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::initLogHeader()
{
    const std::size_t networkSize = mBuffer[0].mPotentialVector.size();
    std::ostringstream header;
    header << "Major Step, Minor Step, Decomposition, Solution Result";
    for (std::size_t node = 0; node < networkSize; ++node) {
        header << ", Potential Vector [" << node << "]";
    }
    for (std::size_t node = 0; node < networkSize; ++node) {
        header << ", Node Convergence [" << node << "]";
    }
    for (unsigned int link = 0; link < mBuffer[0].mLinksData.size(); ++link) {
        header << ", Link " << mLinks[link]->getName() << ": Solution Result";
        for (unsigned int i = 0; i < mBuffer[0].mLinksData[link].mAdmittanceMatrix.size(); ++i) {
            header << ", Admittance Matrix [" << i << "]";
        }
        for (unsigned int i = 0; i < mBuffer[0].mLinksData[link].mSourceVector.size(); ++i) {
            header << ", Source Vector [" << i << "]";
        }
    }
    header << std::endl;
    mLogHeader = header.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::string (--) Formatted log data output with comma separated values and newlines.
///
/// @details  Returns the recorded data log as a csv format, with comma separated values and
///           newlines.  One row of data for each valid minor step of data, starting at the tail
///           and ending at the head of the buffer.  The first row is column labels.
///
/// @note  If the mIsRecording flag is set, meaning recording from the solver is active, this will
///        not return the logged data, and instead return a failure message.  This helps ensure
///        that data frames will not be recorded while the output file is still being written.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string GunnsMinorStepLog::renderOutput()
{
    std::string output = "";
    if (not mIsRecording) {
        const std::size_t networkSize = mBuffer[0].mPotentialVector.size();
        std::ostringstream data;
        data << mLogHeader;
        /// - Loop over the number valid steps from the tail to the head, with wrap-around.
        for (int j = 1 - mNumValidSteps; j < 1; ++j) {
            const GunnsMinorStepData& frame = headOffset(j);
            data << frame.mMajorStep << ", " << frame.mMinorStep << ", " << frame.mDecomposition
                 << ", " << frame.renderSolutionResult();
            for (std::size_t node = 0; node < networkSize; ++node) {
                data << ", " << frame.mPotentialVector[node];
            }
            for (std::size_t node = 0; node < networkSize; ++node) {
                data << ", " << frame.mNodesConvergence[node];
            }
            for (unsigned int link = 0; link < mBuffer[0].mLinksData.size(); ++link) {
                data << ", " << frame.mLinksData[link].renderSolutionResult();
                for (unsigned int i = 0; i < mBuffer[0].mLinksData[link].mAdmittanceMatrix.size(); ++i) {
                    data << ", " << frame.mLinksData[link].mAdmittanceMatrix[i];
                }
                for (unsigned int i = 0; i < mBuffer[0].mLinksData[link].mSourceVector.size(); ++i) {
                    data << ", " << frame.mLinksData[link].mSourceVector[i];
                }
            }
            data << std::endl;
        }
        output = data.str();
    } else {
        output = "Log formatting failure, logging was not stopped when getFormattedOutput was called.";
    }
    return output;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initiates the file write by pausing recording, moding to the WRITING_FILE state and
///           sending the output filename to the asynchronous file output function (updateAsync),
///           which triggers the actual file write.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMinorStepLog::initiateDump()
{
    stopLogging();
    mState = WRITING_FILE;
    std::ostringstream file;
    file << mInputData.mPath << mName << "_" << head().mMajorStep << ".csv";
    mFilename = file.str();
}
