/**
@file
@brief    TS21 Fluid Controller Speed Valve Controller implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((TsPoweredValveController.o))
*/

#include "math/MsMath.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "TsSpeedValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Speed Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsSpeedValveController::TsSpeedValveController()
    :
    TsPoweredValveController(),
    mCommand(),
    mSensed()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Speed Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsSpeedValveController::~TsSpeedValveController()
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
/// @details    Initializes this Speed Valve Controller model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsSpeedValveController::initialize(const TsPoweredValveControllerConfigData& config,
                                        const TsPoweredValveControllerInputData&  input,
                                        const std::string&                        name)
{
    /// - First initialize & validate parent.
    TsPoweredValveController::initialize(config, input, name);

    /// - Reset initialization status flag.
    mInitialized      = false;

    /// - Initialize from the configuration data (none).

    /// - Initialize from the input data.
    mCommand.mEnable  = input.mEnabledFlag;
    mCommand.mSpeed   = input.mCmd;
    mSensed.mClose    = input.mCmdPosition < mMinCmdPosition + FLT_EPSILON;
    mSensed.mOpen     = input.mCmdPosition > mMaxCmdPosition - FLT_EPSILON;

    /// - Initialize the outputs (position and power) consistent with the inputs.
    update(0.0);

    /// - Set the initialization complete flag.
    mInitialized      = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates this Speed Valve Controller model position and power consumption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsSpeedValveController::update(const double dt)
{
    if (mEotInterrupt) {
        update(mSupplyVoltageFlag, mCommand, mSensed, dt);
    } else {
        update(mSupplyVoltageFlag, mCommand, dt);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   supplyVoltageFlag  (--)  Supply voltage flag.
/// @param[in]   command            (--)  Composite command.
/// @param[in]   dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates this Speed Valve Controller model position and power consumption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsSpeedValveController::update(const bool             supplyVoltageFlag,
                                    const TsSpeedValveCmd& command,
                                    const double           dt)
{
    TsPoweredValveController::update(supplyVoltageFlag, command.mEnable, command.mSpeed, dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   supplyVoltageFlag  (--)  Supply voltage flag.
/// @param[in]   command            (--)  Composite command.
/// @param[in]   sensed             (--)  Composite sensed position.
/// @param[in]   dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates this Speed Valve Controller model position and power consumption with
///           end of travel interruption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsSpeedValveController::update(const bool                    supplyVoltageFlag,
                                    const TsSpeedValveCmd&        command,
                                    const TsOpenCloseValveSensed& sensed,
                                    const double                  dt)
{
    double cmd = command.mSpeed;
    // mCmdScale is positive if a positive command opens the valve.
    // mCmdScale is negative if a negative command opens the valve.
    if (cmd > 0.0 && ((sensed.mOpen && mCmdScale > 0.0) || (sensed.mClose && mCmdScale < 0.0))) {
        cmd    = 0.0;
    } else if (cmd < 0.0 && ((sensed.mOpen && mCmdScale < 0.0) || (sensed.mClose && mCmdScale > 0.0))) {
        cmd    = 0.0;
    }

    TsPoweredValveController::update(supplyVoltageFlag, command.mEnable, cmd, dt);
}
