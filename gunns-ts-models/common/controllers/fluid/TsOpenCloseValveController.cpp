/**
@file
@brief    TS21 Fluid Controller Open/Close Valve Controller implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((TsPoweredValveController.o))
*/

#include "math/MsMath.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "TsOpenCloseValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Open/Close Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveController::TsOpenCloseValveController()
    :
    TsPoweredValveController(),
    mCommand(),
    mSensed(),
    mHandleStowed(STOWED),
    mHandlePosition(0){
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Open/Close Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpenCloseValveController::~TsOpenCloseValveController()
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
/// @details    Initializes this Open/Close Valve Controller model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpenCloseValveController::initialize(const TsPoweredValveControllerConfigData& config,
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
    mCommand.mOpen    = input.mCmd > 0.0;
    mCommand.mClose   = input.mCmd < 0.0;
    mSensed.mOpen     = input.mCmdPosition > mMaxCmdPosition - FLT_EPSILON;
    mSensed.mClose    = input.mCmdPosition < mMinCmdPosition + FLT_EPSILON;

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
/// @details  Updates this Open/Close Valve Controller model position and power consumption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpenCloseValveController::update(const double dt)
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
/// @details  Updates this Open/Close Valve Controller model position and power consumption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpenCloseValveController::update(const bool                 supplyVoltageFlag,
                                        const TsOpenCloseValveCmd& command,
                                        const double               dt)
{
    double cmd = 0.0;
    if (command.mOpen && !command.mClose) {
        cmd    = +1.0;
    } else if (!command.mOpen && command.mClose) {
        cmd    = -1.0;
    }

    if (UNSTOWED == mHandleStowed) {
        if (mHandlePosition  > FLT_EPSILON) {
            cmd = 1.0;
        } else {
            cmd = -1.0;
        }
    }

    TsPoweredValveController::update(supplyVoltageFlag, command.mEnable, cmd, dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   supplyVoltageFlag  (--)  Supply voltage flag.
/// @param[in]   command            (--)  Composite command.
/// @param[in]   sensed             (--)  Composite sensed position.
/// @param[in]   dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates this Open/Close Valve Controller model position and power consumption with
///           end of travel interruption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpenCloseValveController::update(const bool                    supplyVoltageFlag,
                                        const TsOpenCloseValveCmd&    command,
                                        const TsOpenCloseValveSensed& sensed,
                                        const double                  dt)
{
    double cmd = 0.0;
    if (        command.mOpen && !command.mClose && !sensed.mOpen) {
        cmd    = +1.0;
    } else if (!command.mOpen &&  command.mClose && !sensed.mClose) {
        cmd    = -1.0;
    }

    TsPoweredValveController::update(supplyVoltageFlag, command.mEnable, cmd, dt);
}
