/**
@file
@brief    TS21 Fluid Controller Toggle Valve Controller implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((TsPoweredValveController.o)
  (TsToggleValveCmd.o))
*/

#include <cfloat>

#include "TsToggleValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Toggle Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveController::TsToggleValveController()
    :
    TsPoweredValveController(),
    mCommand(),
    mSensed(),
    mMidPosition(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Toggle Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsToggleValveController::~TsToggleValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config  (--)  Configuration data.
/// @param[in]  input   (--)  Input data.
/// @param[in]  name    (--)  Object name.
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Initializes this Toggle Valve Controller model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsToggleValveController::initialize(const TsPoweredValveControllerConfigData& config,
                                         const TsPoweredValveControllerInputData&  input,
                                         const std::string&                        name)
{
    /// - First initialize & validate parent.
    TsPoweredValveController::initialize(config, input, name);

    /// - Reset initialization status flag.
    mInitialized      = false;

    /// - Initialize from the configuration data.
    mMidPosition      = 0.5 * (mMinCmdPosition + mMaxCmdPosition);

    /// - Initialize from the input data.
    mCommand.mEnable  = input.mEnabledFlag;
    mCommand.mToggle  = false;
    mSensed.mOpen     = input.mCmdPosition > mMaxCmdPosition - FLT_EPSILON;
    mSensed.mClose    = input.mCmdPosition < mMinCmdPosition + FLT_EPSILON;

    /// - Initialize the outputs (position and power) consistent with the inputs.
    update(0.0);

    /// - Set the initialization complete flag.
    mInitialized      = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)   Time step.
///
/// @return      void
///
/// @details     Updates this Toggle Valve Controller model position and power consumption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsToggleValveController::update(const double dt)
{
    mCommand.update(dt);

    if (mEotInterrupt) {
        updateWithEotInterrupt(dt);
    } else {
        updateWithoutEotInterrupt(dt);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)   Time step.
///
/// @return      void
///
/// @details     Updates this Toggle Valve Controller model position and power consumption without
///              end of travel interruption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsToggleValveController::updateWithoutEotInterrupt(const double dt)
{
    double cmd = 0.0;
    if (mCommand.getToggle() && dt > DBL_EPSILON) {
        if (mCmdPosition < mMidPosition) {
            cmd    = (mMaxCmdPosition - mMinCmdPosition) / dt;
        } else  {
            cmd    = (mMinCmdPosition - mMaxCmdPosition) / dt;
        }
    }

    TsPoweredValveController::update(mSupplyVoltageFlag, mCommand.getEnable(), cmd, dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt  (s)   Time step.
///
/// @return      void
///
/// @details     Updates this Toggle Valve Controller model position and power consumption with
///              end of travel interruption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsToggleValveController::updateWithEotInterrupt(const double dt)
{
    double cmd = 0.0;
    if (mCommand.getToggle() && dt > DBL_EPSILON) {
        if (mCmdPosition < mMidPosition && !mSensed.mOpen) {
            cmd    = (mMaxCmdPosition - mMinCmdPosition) / dt;
        } else  if (mCmdPosition >= mMidPosition && !mSensed.mClose) {
            cmd    = (mMinCmdPosition - mMaxCmdPosition) / dt;
        }
    }

    TsPoweredValveController::update(mSupplyVoltageFlag, mCommand.getEnable(), cmd, dt);
}
