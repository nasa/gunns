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
#include <bitset>
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  GUNNS Solver Minor Step Log class.  This is a bare-naked data class for sharing minor
///         step log debug information between the solver and a file dump spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMinorStepLog
{
    TS_MAKE_SIM_COMPATIBLE(GunnsMinorStepLog);
    public:
        /// @brief Enumeration of the non-linear network solution results.
        enum SolutionResults {
            SUCCESS = 0, ///< Solver successfully finished the major step.
            MINOR   = 1, ///< Solver gave up when minor step limit was exceeded.
            DECOMP  = 2  ///< Solver gave up when decomposition limit was exceeded.
        };

        int                                                  mMajorStep;     /**< ** (--) trick_chkpnt_io(**) Total elapsed major step count since init */
        int                                                  mMinorStep;     /**< ** (--) trick_chkpnt_io(**) Last minor step count within the major step since restart */
        SolutionResults                                      mResult;        /**< ** (--) trick_chkpnt_io(**) Solution result of the major step */
        static const unsigned int                            mWordSize = 32; /**< ** (--) trick_chkpnt_io(**) Bit size of the debug words, should correspond to unsigned long int */
        std::vector< std::vector< std::bitset<mWordSize> > > mNodeBits;      /**< ** (--) trick_chkpnt_io(**) Node non-converging flags recorded for each minor step */
        std::vector< std::vector< std::bitset<mWordSize> > > mLinkBits;      /**< ** (--) trick_chkpnt_io(**) Link reject/delay flags recorded for each minor step */
        int                                                  mNumLinkWords;  /**< ** (--) trick_chkpnt_io(**) */
        int                                                  mNumNodeWords;  /**< ** (--) trick_chkpnt_io(**) */
        /// @brief Default constructs this GUNNS Solver Minor Step Log.
        GunnsMinorStepLog();
        /// @brief Default destructs this GUNNS Solver Minor Step Log.
        virtual ~GunnsMinorStepLog();
        /// @brief Initializes this GUNNS Solver Minor Step Log
        void initialize(const int networkSize, const int numLinks, const int stepLimit);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsMinorStepLog& operator =(const GunnsMinorStepLog&);
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsMinorStepLog(const GunnsMinorStepLog&);
};

/// @}

#endif
