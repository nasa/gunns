/**
@file
@brief    TS21 Fluid Controller Toggle Valve Command implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ()
**************************************************************************************************/

#include <cfloat>

#include "TsToggleValveCmd.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  enable  (--)             Enable command flag.
/// @param[in]  toggle  (--)             Toggle command flag.
///
/// @details    Default constructs this Powered Toggle Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveCmd::TsToggleValveCmd(const bool  enable, const bool  toggle)
    :
    mEnable(enable),
    mToggle(toggle),
    mEnable_queue(0),
    mToggle_queue(0),
    mEnable_queue_size(0),
    mToggle_queue_size(0),
    mArmed(true),
    mToggleCount(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Toggle Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveCmd::~TsToggleValveCmd()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the state of this Powered Toggle Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsToggleValveCmd::update(const double dt)
{
    if (dt > DBL_EPSILON) {
        if (0 < mToggle_queue_size) {
            /// - Process queue if size > 0
            int toggleCount = 0;
            for (int i = 0; i < mToggle_queue_size; ++i) {
                // TODO this is dangerous, referencing into mEnable_queue without ever checking its
                // size.  we can't always assume it will be the same as mToggle_queue_size!
                if (mArmed && mToggle_queue[i] && mEnable_queue[i]) {
                    ++toggleCount;
                    mArmed  = false;
                } else if (!mArmed && !mToggle_queue[i] && mEnable_queue[i]) {
                    mArmed = true;
                }
            }
            /// - Toggle true for odd number of valid toggle commands, otherwise false
            mToggle = (1 == toggleCount % 2);
            /// - In the queue'd implementation, the mToggleCount represents the number of enabled
            ///   toggles detected in the queue, not how many times mToggle has resulted in true
            ///   (since an even number of new toggles results in mToggle = false).  This gives us
            ///   insight into how many pulses are present in the queue.
            mToggleCount += toggleCount;
            if (mToggle) {
                /// - Enable true if toggle true
                mEnable = true;
            } else {
                int enableCount = 0;
                for (int i = 0; i < mEnable_queue_size; ++i) {
                    if (mEnable_queue[i]) {
                        ++enableCount;
                    }
                }
                /// - Otherwise enable true if mostly enabled
                mEnable = (0.49 <= static_cast<double>(enableCount) / static_cast<double>(mEnable_queue_size));
            }
        } else {
            if (mArmed && mToggle && mEnable) {
                mArmed  = false;
            } else if (!mArmed && !mToggle && mEnable) {
                mArmed  = true;
            } else {
                mToggle = false;
            }
            /// - In the non-queue'd implementation, mToggleCount represents the number of times
            ///   mToggle has resulted in true.
            if (mToggle) {
                mToggleCount++;
            }
        }
    }
}
