/**
@file
@brief    TS21 Fluid Controller Position Valve Controller implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((TsPoweredValveController.o))
*/

#include "math/MsMath.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "TsPositionValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  minCmdPosition    (--)  Minimum valid valve position.
/// @param[in]  maxCmdPosition    (--)  Maximum valid valve position.
/// @param[in]  minFluidPosition  (--)  Minimum valid valve flow area fraction.
/// @param[in]  maxFluidPosition  (--)  Maximum valid valve flow area fraction.
/// @param[in]  transitTime       (s)   Reference transit time.
/// @param[in]  refCmd            (--)  Reference speed command (always 1).
/// @param[in]  holdPower         (W)   Power consumption when valve is holding.
/// @param[in]  movePower         (W)   Power consumption when valve is moving.
/// @param[in]  stuckPower        (W)   Power consumption when valve is stuck.
/// @param[in]  refVoltage        (v)   Reference voltage for power consumption.
/// @param[in]  eotInterrupt      (--)  End of travel interrupting flag.
/// @param[in]  latch             (--)  Latching type.
/// @param[in]  tolerance         (--)  Position command tolerance.
///
/// @details    Default constructs this Position Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPositionValveControllerConfigData::TsPositionValveControllerConfigData(const double                              minCmdPosition,
                                                                         const double                              maxCmdPosition,
                                                                         const double                              minFluidPosition,
                                                                         const double                              maxFluidPosition,
                                                                         const double                              transitTime,
                                                                         const double                              refCmd __attribute__((unused)),
                                                                         const double                              holdPower,
                                                                         const double                              movePower,
                                                                         const double                              stuckPower,
                                                                         const double                              refVoltage,
                                                                         const bool                                eotInterrupt,
                                                                         const TsPoweredValveController::LatchType latch,
                                                                         const double                              tolerance)
    :
    TsPoweredValveControllerConfigData(minCmdPosition,
                                       maxCmdPosition,
                                       minFluidPosition,
                                       maxFluidPosition,
                                       transitTime,
                                       1.0,
                                       holdPower,
                                       movePower,
                                       stuckPower,
                                       refVoltage,
                                       eotInterrupt,
                                       latch),
    mTolerance(tolerance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Powered Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPositionValveControllerConfigData::TsPositionValveControllerConfigData(const TsPositionValveControllerConfigData& that)
    :
    TsPoweredValveControllerConfigData(that),
    mTolerance(that.mTolerance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Powered Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPositionValveControllerConfigData::~TsPositionValveControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Object assigned from.
///
/// @return     TsPositionValveControllerConfigData&  (--)  Object assigned to.
///
/// @details    Assignment operator for this Powered Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPositionValveControllerConfigData& TsPositionValveControllerConfigData::operator=(const TsPositionValveControllerConfigData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        TsPoweredValveControllerConfigData::operator=(that);
        mTolerance  = that.mTolerance;
    }

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Position Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPositionValveController::TsPositionValveController()
    :
    TsPoweredValveController(),
    mTolerance(0.0),
    mCommand(),
    mSensed(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Position Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPositionValveController::~TsPositionValveController()
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
/// @details    Initializes this Position Valve Controller model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPositionValveController::initialize(const TsPositionValveControllerConfigData& config,
                                           const TsPoweredValveControllerInputData&   input,
                                           const std::string&                         name)
{
    /// - First initialize & validate parent.
    TsPoweredValveController::initialize(config, input, name);

    /// - Reset initialization status flag.
    mInitialized       = false;

    /// - Initialize from the configuration data (none).
    mTolerance         = config.mTolerance;

    /// - Initialize from the input data.
    mCommand.mEnable   = input.mEnabledFlag;
    mCommand.mPosition = input.mCmdPosition;
    mSensed            = input.mCmdPosition;

    /// - Initialize the outputs (position and power) consistent with the inputs.
    update(0.0);

    /// - Set the initialization complete flag.
    mInitialized       = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates this Position Valve Controller model position and power consumption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPositionValveController::update(const double dt)
{
    update(mSupplyVoltageFlag, mCommand, mSensed, dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   supplyVoltageFlag  (--)  Supply voltage flag.
/// @param[in]   command            (--)  Composite command.
/// @param[in]   sensed             (--)  Sensed position.
/// @param[in]   dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates this Position Valve Controller model position and power consumption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPositionValveController::update(const bool                supplyVoltageFlag,
                                       const TsPositionValveCmd& command,
                                       const double              sensed,
                                       const double              dt)
{
    double cmd = 0.0;
    if (fabs(command.mPosition - sensed) < mTolerance) {
        cmd                = 0.0;
    } else if (dt > DBL_EPSILON){
        const double limit = dt * mCmdScale;
        cmd                = MsMath::limitRange(-limit, command.mPosition - sensed, +limit) / limit;
    }

    TsPoweredValveController::update(supplyVoltageFlag, command.mEnable, cmd, dt);
}
