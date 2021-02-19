#ifndef GunnsMinorStepLogDumper_EXISTS
#define GunnsMinorStepLogDumper_EXISTS

/**
@file
@brief     GUNNS Solver Minor Step Log File Dump Spotter declarations

@defgroup  TSM_GUNNS_CORE_MINOR_STEP_LOG_FILE_DUMP    GUNNS Solver Minor Step Log File Dump Spotter
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
- ((GunnsMinorStepLogDumper.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2014-09) (Initial Prototype)

@{
*/

#include <string>
#include <cstring>
#include <vector>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsHsException.hh"
#include "GunnsNetworkSpotter.hh"
#include "Gunns.hh"

/// - Forward declare classes used for pointer attributes and method arguments.
class GunnsMinorStepLogDumperInputData;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Solver Minor Step Log Message
///
/// @details  This provides a data structure for the GUNNS Solver Minor Step Log message data and
///           filename.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsMinorStepLogMessage {
        std::string mFileName; /**< ** (--) trick_chkpnt_io(**) File name for output */
        std::string mData;     /**< ** (--) trick_chkpnt_io(**) Data to output */

        /// @brief  Default constructs this GunnsMinorStepLogMessage.
        GunnsMinorStepLogMessage() : mFileName(""), mData("") {;}
        /// @brief  Default destructs this GunnsMinorStepLogMessage.
        virtual ~GunnsMinorStepLogMessage() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Solver Minor Step Log File Dump Spotter Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Solver
///           Minor Step Log File Dump Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMinorStepLogDumperConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        /// @brief  Default constructs this GUNNS Solver Minor Step Log File Dump Spotter
        ///         configuration data.
        GunnsMinorStepLogDumperConfigData(const std::string& name);
        /// @brief  Default destructs this GUNNS Solver Minor Step Log File Dump Spotter
        ///         configuration data.
        virtual ~GunnsMinorStepLogDumperConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsMinorStepLogDumperConfigData(const GunnsMinorStepLogDumperConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsMinorStepLogDumperConfigData& operator =(const GunnsMinorStepLogDumperConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  GUNNS Solver Minor Step Log File Dump Spotter class.  This class performs debug file
///         dump writes for the solver.  This class gets log data from the solver via the solver's
///         GunnsMinorStepLog object, and writes the data into a user-readable output file.  Files
///         are written either by user command or automatically when the solver fails to converge,
///         if enabled.  The step method called by the network manager does the data formatting, and
///         the updateAsync method is intended to be called from an asynchronous job on a separate
///         thread from the network's scheduled job update.  This prevents file writes from
///         hindering the real-time performance of the network update.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMinorStepLogDumper : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsMinorStepLogDumper);
    public:
        /// @brief  Enumeration of the minor step log dump modes.
        enum LogModes {
            STANDBY = 0, ///< Log is not dumped, user must manually set to AUTO or SNAP.
            AUTO    = 1, ///< Log is automatically dumped by solver on first non-converging major step.
            SNAP    = 2, ///< Log is dumped immediately.
            LOGGING = 3  ///< Logging is currently in progress.
        };

        /// @brief  Default Constructor
        GunnsMinorStepLogDumper(Gunns&                              solver,
                                const std::vector<GunnsBasicLink*>& links);
        /// @brief  Default Destructor
        virtual ~GunnsMinorStepLogDumper();
        /// @brief  Initializes the GUNNS Solver Minor Step Log File Dump Spotter class.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief  GUNNS Network Spotter processing performed before the GUNNS solver step.
        virtual void stepPreSolver(const double dt __attribute__((unused))) {;}
        /// @brief  GUNNS Network Spotter processing performed after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief  Writes stored data messages to files on the file system.
        void updateAsync();

    protected:
        GunnsMinorStepLog&                    mSolverLog;       /**< ** (--) trick_chkpnt_io(**) Reference to the network solver */
        const std::vector<GunnsBasicLink*>&   mLinks;           /**< ** (--) trick_chkpnt_io(**) Reference to the network links vector */
        LogModes                              mLogMode;         /**<    (--)                     Operating mode of the log dump function */
        int                                   mDefaultLogSteps; /**<    (--) trick_chkpnt_io(**) Default number of major steps to record in each log */
        std::string                           mPath;            /**<    (--) trick_chkpnt_io(**) File system relative path for output files to go */
        int                                   mLogSteps;        /**<    (--)                     How many major steps of data to record in the next log */
        int                                   mQueueSize;       /**<    (--) trick_chkpnt_io(**) Number of messages in the queue to be written */
        bool                                  mLocked;          /**< ** (--) trick_chkpnt_io(**) Queue is being updated in real-time thread */
        std::vector<GunnsMinorStepLogMessage> mFileMessages;    /**< ** (--) trick_chkpnt_io(**) Queued file dump messages to be written */
        std::string                           mLogTableHeader;  /**< ** (--) trick_chkpnt_io(**) Debug dump minor step log header */
        std::string                           mLinkNamesTable;  /**< ** (--) trick_chkpnt_io(**) Debug dump link names table */
        /// @brief   Validates the supplied configuration data.
        const GunnsMinorStepLogDumperConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsMinorStepLogDumperInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
        /// @brief   Returns a string with formatted log data for the current solver major step.
        std::string formatLogData();
        /// @brief   Builds the minor step log table header string.
        void initLogTableHeader();
        /// @brief   Builds the network link names table string.
        void initLinkNamesTable();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsMinorStepLogDumper(const GunnsMinorStepLogDumper& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsMinorStepLogDumper& operator =(const GunnsMinorStepLogDumper& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Solver Minor Step Log File Dump Spotter Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Solver
///           Minor Step Log File Dump Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMinorStepLogDumperInputData : public GunnsNetworkSpotterInputData
{
    public:
        GunnsMinorStepLogDumper::LogModes mLogMode;  /**< (--) trick_chkpnt_io(**) Initial operating mode of the log dump function */
        int                               mLogSteps; /**< (--) trick_chkpnt_io(**) Initial & default number of major steps to record in each log */
        std::string                       mPath;     /**< (--) trick_chkpnt_io(**) File system relative path for output files to go */
        /// @brief  Default constructs this GUNNS Solver Minor Step Log File Dump Spotter input data.
        GunnsMinorStepLogDumperInputData(const GunnsMinorStepLogDumper::LogModes logMode         = GunnsMinorStepLogDumper::STANDBY,
                                         const int                               defaultLogSteps = 0,
                                         const std::string                       path            = "./");
        /// @brief  Default destructs this GUNNS Solver Minor Step Log File Dump Spotter input data.
        virtual ~GunnsMinorStepLogDumperInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsMinorStepLogDumperInputData(const GunnsMinorStepLogDumperInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsMinorStepLogDumperInputData& operator =(const GunnsMinorStepLogDumperInputData& that);
};

/// @}

#endif
