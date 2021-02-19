/**
@file
@brief    TS21 Fluid Controller Powered (Motor or Solenoid) Valve Controller implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((TsValveController.o))
*/

#include "GenericMacros.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "TsPoweredValveController.hh"

const double TsPoweredValveController::MAX_RESISTANCE = 1.0e+06;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  minCmdPosition    (--)  Minimum valid valve position.
/// @param[in]  maxCmdPosition    (--)  Maximum valid valve position.
/// @param[in]  minFluidPosition  (--)  Minimum valid valve flow area fraction.
/// @param[in]  maxFluidPosition  (--)  Maximum valid valve flow area fraction.
/// @param[in]  transitTime       (s)   Reference transit time.
/// @param[in]  refCmd            (--)  Reference speed command.
/// @param[in]  holdPower         (W)   Power consumption when valve is holding.
/// @param[in]  movePower         (W)   Power consumption when valve is moving.
/// @param[in]  stuckPower        (W)   Power consumption when valve is stuck.
/// @param[in]  refVoltage        (V)   Reference voltage for power consumption.
/// @param[in]  eotInterrupt      (--)  End of travel interrupting flag.
/// @param[in]  latch             (--)  Latching type.
///
/// @details    Default constructs this Powered Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveControllerConfigData::TsPoweredValveControllerConfigData(const double                              minCmdPosition,
                                                                       const double                              maxCmdPosition,
                                                                       const double                              minFluidPosition,
                                                                       const double                              maxFluidPosition,
                                                                       const double                              transitTime,
                                                                       const double                              refCmd,
                                                                       const double                              holdPower,
                                                                       const double                              movePower,
                                                                       const double                              stuckPower,
                                                                       const double                              refVoltage,
                                                                       const bool                                eotInterrupt,
                                                                       const TsPoweredValveController::LatchType latch)
    :
    TsValveControllerConfigData(minCmdPosition, maxCmdPosition, minFluidPosition, maxFluidPosition),
    mTransitTime(transitTime),
    mRefCmd(refCmd),
    mHoldPower(holdPower),
    mMovePower(movePower),
    mStuckPower(stuckPower),
    mRefVoltage(refVoltage),
    mEotInterrupt(eotInterrupt),
    mLatch(latch)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Powered Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveControllerConfigData::TsPoweredValveControllerConfigData(const TsPoweredValveControllerConfigData& that)
    :
    TsValveControllerConfigData(that),
    mTransitTime(that.mTransitTime),
    mRefCmd(that.mRefCmd),
    mHoldPower(that.mHoldPower),
    mMovePower(that.mMovePower),
    mStuckPower(that.mStuckPower),
    mRefVoltage(that.mRefVoltage),
    mEotInterrupt(that.mEotInterrupt),
    mLatch(that.mLatch)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Powered Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveControllerConfigData::~TsPoweredValveControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Object assigned from.
///
/// @return     TsPoweredValveControllerConfigData&  (--)  Object assigned to.
///
/// @details    Assignment operator for this Powered Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveControllerConfigData& TsPoweredValveControllerConfigData::operator=(const TsPoweredValveControllerConfigData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        TsValveControllerConfigData::operator=(that);
        mTransitTime  = that.mTransitTime;
        mRefCmd       = that.mRefCmd;
        mHoldPower    = that.mHoldPower;
        mMovePower    = that.mMovePower;
        mStuckPower   = that.mStuckPower;
        mRefVoltage   = that.mRefVoltage;
        mEotInterrupt = that.mEotInterrupt;
        mLatch        = that.mLatch;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  cmdPosition             (--)  Valve position.
/// @param[in]  manualPositionFlag      (--)  Manual override position flag.
/// @param[in]  manualPositionValue     (--)  Manual override position value.
/// @param[in]  supplyVoltageFlag       (--)  Supply voltage good flag.
/// @param[in]  enabledFlag             (--)  Controller enabled flag.
/// @param[in]  cmd                     (--)  Speed command.
///
/// @details    Default constructs this Powered Valve Controller model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveControllerInputData::TsPoweredValveControllerInputData(const double cmdPosition,
                                                                     const bool   manualPositionFlag,
                                                                     const double manualPositionValue,
                                                                     const bool   supplyVoltageFlag,
                                                                     const bool   enabledFlag,
                                                                     const double cmd)
    :
    TsValveControllerInputData(cmdPosition,
                               manualPositionFlag,
                               manualPositionValue),
    mSupplyVoltageFlag(supplyVoltageFlag),
    mEnabledFlag(enabledFlag),
    mCmd(cmd)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Powered Valve Controller model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveControllerInputData::TsPoweredValveControllerInputData(const TsPoweredValveControllerInputData& that)
    :
    TsValveControllerInputData(that),
    mSupplyVoltageFlag(that.mSupplyVoltageFlag),
    mEnabledFlag(that.mEnabledFlag),
    mCmd(that.mCmd)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Powered Valve Controller model Input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveControllerInputData::~TsPoweredValveControllerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Object assigned from.
///
/// @return     TsPoweredValveControllerInputData&  (--)  Object assigned to.
///
/// @details    Assignment operator for this Powered Valve Controller model Input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveControllerInputData& TsPoweredValveControllerInputData::operator=(const TsPoweredValveControllerInputData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        TsValveControllerInputData::operator=(that);
        mSupplyVoltageFlag      = that.mSupplyVoltageFlag;
        mEnabledFlag            = that.mEnabledFlag;
        mCmd                    = that.mCmd;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveController::TsPoweredValveController()
    :
    TsValveController(),
    mMalfPowerGoodFlag(false),
    mMalfPowerFailFlag(false),
    mMalfDriveStuckFlag(false),
    mMalfDriveFailToFlag(false),
    mMalfDriveFailToValue(0.0),
    mCmdScale(0.0),
    mHoldPower(0.0),
    mMovePower(0.0),
    mStuckPower(0.0),
    mHoldResistance(0.0),
    mMoveResistance(0.0),
    mStuckResistance(0.0),
    mEotInterrupt(false),
    mLatch(LATCHING),
    mSupplyVoltageFlag(false),
    mEnabledFlag(false),
    mCmd(0.0),
    mPower(0.0),
    mResistance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPoweredValveController::~TsPoweredValveController()
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
/// @details    Initializes this Powered Valve Controller model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPoweredValveController::initialize(const TsPoweredValveControllerConfigData& config,
                                          const TsPoweredValveControllerInputData&  input,
                                          const std::string&                        name)
{
    /// - First initialize & validate parent.
    TsValveController::initialize(config, input, name);

    /// - Reset initialization status flag.
    mInitialized            = false;

    /// - Validate the configuration and input data.
    validate(config, input);

    /// - Initialize from the configuration data.
    const double vSq        = config.mRefVoltage * config.mRefVoltage;
    mCmdScale               = (mMaxCmdPosition - mMinCmdPosition) / (config.mRefCmd * config.mTransitTime);
    mHoldPower              = config.mHoldPower;
    mMovePower              = config.mMovePower;
    mStuckPower             = config.mStuckPower;
    mHoldResistance         = vSq / config.mHoldPower;
    mMoveResistance         = vSq / config.mMovePower;
    mStuckResistance        = vSq / config.mStuckPower;
    mEotInterrupt           = config.mEotInterrupt;
    mLatch                  = config.mLatch;

    /// - Initialize from the input data.
    mSupplyVoltageFlag      = input.mSupplyVoltageFlag;
    mEnabledFlag            = input.mEnabledFlag;
    mCmd                    = input.mCmd;

    /// - Initialize malfunctions off.
    mMalfPowerGoodFlag      = false;
    mMalfPowerFailFlag      = false;
    mMalfDriveStuckFlag     = false;
    mMalfDriveFailToFlag    = false;
    mMalfDriveFailToValue   = 0.0;

    /// - Initialize the outputs (position and power) consistent with the inputs.
    TsPoweredValveController::update(0.0);

    /// - Set the initialization complete flag.
    mInitialized            = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config  (--)  Configuration data.
/// @param[in]  input   (--)  Input data (unused).
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Validates this Powered Valve Controller model initialization data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPoweredValveController::validate(const TsPoweredValveControllerConfigData& config,
                                        const TsPoweredValveControllerInputData&  input __attribute__((unused))) const
{
    /// - Throw a TsInitializationException exception on transit time < DBL_EPSILON.
    TS_GENERIC_IF_ERREX((config.mTransitTime < DBL_EPSILON),                           TsInitializationException,
                        "Invalid Configuration Data",  "transit time < DBL_EPSILON.");

    /// - Throw a TsInitializationException exception on reference command too small.
    TS_GENERIC_IF_ERREX((MsMath::isInRange(-DBL_EPSILON, config.mRefCmd, +DBL_EPSILON)), TsInitializationException,
                        "Invalid Configuration Data",  "reference command too small.");

    /// - Throw a TsInitializationException exception on hold power < DBL_EPSILON.
    TS_GENERIC_IF_ERREX((config.mHoldPower < DBL_EPSILON),                             TsInitializationException,
                        "Invalid Configuration Data",  "hold power < DBL_EPSILON.");

    /// - Throw a TsInitializationException exception on stuck power < DBL_EPSILON.
    TS_GENERIC_IF_ERREX((config.mStuckPower < DBL_EPSILON),                            TsInitializationException,
                        "Invalid Configuration Data",  "stuck power < DBL_EPSILON.");

    /// - Throw a TsInitializationException exception on move power < DBL_EPSILON.
    TS_GENERIC_IF_ERREX((config.mMovePower < DBL_EPSILON),                             TsInitializationException,
                        "Invalid Configuration Data",  "move power < DBL_EPSILON.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   supplyVoltageFlag  (--)  Supply voltage flag.
/// @param[in]   enabledFlag        (--)  Enabled flag.
/// @param[in]   cmd                (--)  Speed command.
/// @param[in]   dt                 (s)   Time step.
///
/// @return   void
///
/// @details  Updates this Powered Valve Controller model position and power consumption.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPoweredValveController::update(const bool supplyVoltageFlag, const bool enabledFlag, const double cmd, const double dt)
{
    setSupplyVoltageFlag(supplyVoltageFlag);
    TsPoweredValveController::setEnabledFlag(enabledFlag);
    setCmd(cmd);
    TsPoweredValveController::update(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Time step.
///
/// @return     void
///
/// @details    Updates this Powered Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPoweredValveController::update(const double dt)
{
    /// - Default the desired position to the previous position.
    double position = mCmdPosition;

    /// - Skip the position update on a stuck motor/solenoid malfunction.
    if (!mMalfDriveStuckFlag) {
         if (mMalfDriveFailToFlag) {
            /// - Set the position to the fail-to value on a motor/solenoid fail-to malfunction.
            position  = mMalfDriveFailToValue;
        } else if (!(mSupplyVoltageFlag || mMalfPowerGoodFlag) || mMalfPowerFailFlag || !mEnabledFlag) {
            /// - Set the position according to latch type when no power.
            switch (mLatch) {
                case NORMALLY_CLOSED:
                    position = mMinCmdPosition;
                    break;
                case NORMALLY_OPEN:
                    position = mMaxCmdPosition;
                    break;
                case LATCHING:
                default:
                    position = mCmdPosition;
                    break;
            }
        } else if (mEnabledFlag) {
            /// - Otherwise use the speed command if powered and enabled.
            position += mCmd * mCmdScale * dt;
        }
    }

    /// - Update the power consumption and resistance.
    updatePower();

    /// - Update the actual position.
    updatePosition(position);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     void
///
/// @details    Updates this Powered Valve Controller model power consumption and resistance
///             distinguishing four cases: stuck, moving, holding and unpowered.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPoweredValveController::updatePower()
{
    if ((mSupplyVoltageFlag || mMalfPowerGoodFlag) && !mMalfPowerFailFlag && mEnabledFlag) {
        if (fabs(mCmd) > DBL_EPSILON && (!mManualPositionFlag || mMalfManualFlag)) {

            // mCmdScale is positive if a positive command opens the valve.
            // mCmdScale is negative if a negative command opens the valve.
            const bool commandingBelowMin = (mCmdPosition <= mMinCmdPosition &&
                                            ((mCmd < 0.0 && mCmdScale > 0.0) || (mCmd > 0.0 && mCmdScale < 0.0)));

            const bool commandingAboveMax = (mCmdPosition >= mMaxCmdPosition &&
                                            ((mCmd > 0.0 && mCmdScale > 0.0) || (mCmd < 0.0 && mCmdScale < 0.0)));

            if (mMalfValveStuckFlag || mMalfDriveStuckFlag || commandingBelowMin || commandingAboveMax) {
                mPower      = mStuckPower;
                mResistance = mStuckResistance;
            } else {
                mPower      = mMovePower;
                mResistance = mMoveResistance;
            }
        } else {
            mPower          = mHoldPower;
            mResistance     = mHoldResistance;
        }
    } else {
        mPower              = 0.0;
        mResistance         = MAX_RESISTANCE;
    }
}

