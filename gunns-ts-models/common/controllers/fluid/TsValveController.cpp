/**
@file
@brief    TS21 Fluid Controller Fluid (Valve, Fan, Pump) Controllers implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((simulation/hs/TsHsMsg.o)
  (software/exceptions/TsInitializationException.o))
*/

#include "GenericMacros.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "TsValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  minCmdPosition    (--)  Minimum valid valve position.
/// @param[in]  maxCmdPosition    (--)  Maximum valid valve position.
/// @param[in]  minFluidPosition  (--)  Minimum valid valve flow area fraction.
/// @param[in]  maxFluidPosition  (--)  Maximum valid valve flow area fraction.
///
/// @details    Default constructs this Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveControllerConfigData::TsValveControllerConfigData(const double minCmdPosition,
                                                         const double maxCmdPosition,
                                                         const double minFluidPosition,
                                                         const double maxFluidPosition)
    :
    mMinCmdPosition(minCmdPosition),
    mMaxCmdPosition(maxCmdPosition),
    mMinFluidPosition(minFluidPosition),
    mMaxFluidPosition(maxFluidPosition)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveControllerConfigData::TsValveControllerConfigData(const TsValveControllerConfigData& that)
    :
    mMinCmdPosition(that.mMinCmdPosition),
    mMaxCmdPosition(that.mMaxCmdPosition),
    mMinFluidPosition(that.mMinFluidPosition),
    mMaxFluidPosition(that.mMaxFluidPosition)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveControllerConfigData::~TsValveControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Object assigned from.
///
/// @return     TsValveControllerConfigData&  (--)  Object assigned to.
///
/// @details    Assignment operator for this Valve Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveControllerConfigData& TsValveControllerConfigData::operator=(const TsValveControllerConfigData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        mMinCmdPosition   = that.mMinCmdPosition;
        mMaxCmdPosition   = that.mMaxCmdPosition;
        mMinFluidPosition = that.mMinFluidPosition;
        mMaxFluidPosition = that.mMaxFluidPosition;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  cmdPosition             (--)  Valve position.
/// @param[in]  manualPositionFlag      (--)  Manual override valve position flag.
/// @param[in]  manualPositionValue     (--)  Manual override valve position value.
///
/// @details    Default constructs this Valve Controller model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveControllerInputData::TsValveControllerInputData(const double cmdPosition,
                                                       const bool   manualPositionFlag,
                                                       const double manualPositionValue)
    :
    mCmdPosition(cmdPosition),
    mManualPositionFlag(manualPositionFlag),
    mManualPositionValue(manualPositionValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Valve Controller model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveControllerInputData::TsValveControllerInputData(const TsValveControllerInputData& that)
    :
    mCmdPosition(that.mCmdPosition),
    mManualPositionFlag(that.mManualPositionFlag),
    mManualPositionValue(that.mManualPositionValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Valve Controller model Input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveControllerInputData::~TsValveControllerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Object assigned from.
///
/// @return     TsValveControllerInputData&  (--)  Object assigned to.
///
/// @details    Assignment operator for this Valve Controller model Input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveControllerInputData& TsValveControllerInputData::operator=(const TsValveControllerInputData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        mCmdPosition            = that.mCmdPosition;
        mManualPositionFlag     = that.mManualPositionFlag;
        mManualPositionValue    = that.mManualPositionValue;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default constructs this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveController::TsValveController()
    :
    mMalfValveStuckFlag(false),
    mMalfValveFailToFlag(false),
    mMalfValveFailToValue(0.0),
    mMalfManualFlag(false),
    mName(),
    mMinCmdPosition(0.0),
    mMaxCmdPosition(0.0),
    mFluidBias(0.0),
    mFluidScale(0.0),
    mCmdPosition(0.0),
    mManualPositionFlag(false),
    mManualPositionValue(0.0),
    mStuckFlag(false),
    mLowerLimitFlag(false),
    mUpperLimitFlag(false),
    mFluidPosition(0.0),
    mInitialized(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Default destructs this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsValveController::~TsValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config  (--)  Configuration data.
/// @param[in]  input   (--)  Input data.
/// @param[in]  name    (--)  Object name.
///
/// @return     voidunused
///
/// @throws     TsInitializationException
///
/// @details    Initializes this Valve Controller model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsValveController::initialize(const TsValveControllerConfigData& config,
                                   const TsValveControllerInputData&  input,
                                   const std::string&                 name)
{
    /// - Reset the initialization complete flag.
    mInitialized            = false;

    /// - Allocate and initialize the object name or throw a TsInitializationException exception if empty.
    TS_GENERIC_NAME_ERREX("TsValveController")

    /// - Validate the configuration and input data.
    validate(config, input);

    /// - Initialize from the configuration data.
    mMinCmdPosition         = config.mMinCmdPosition;
    mMaxCmdPosition         = config.mMaxCmdPosition;
    mFluidScale             = (config.mMaxFluidPosition - config.mMinFluidPosition) / (mMaxCmdPosition - mMinCmdPosition);
    mFluidBias              = config.mMaxFluidPosition - mFluidScale * config.mMaxCmdPosition;

    /// - Initialize from the input data.
    mCmdPosition            = input.mCmdPosition;
    mManualPositionFlag     = input.mManualPositionFlag;
    mManualPositionValue    = input.mManualPositionValue;

    /// - Initialize malfunctions off..
    mMalfValveStuckFlag     = false;
    mMalfValveFailToFlag    = false;
    mMalfValveFailToValue   = 0.0;
    mMalfManualFlag         = false;

    /// - Initialize the outputs (position) consistent with the inputs.
    TsValveController::update(0.0);

    /// - Set the initialization complete flag.
    mInitialized            = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  config  (--)  Configuration data.
/// @param[in]  input   (--)  Input data.
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Validates this Valve Controller model initialization data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsValveController::validate(const TsValveControllerConfigData& config,
                                 const TsValveControllerInputData&  input) const
{
    /// - Throw a TsInitializationException exception on valve maximum flow area fraction <= valve minimum flow area fraction.
    TS_GENERIC_IF_ERREX((config.mMaxFluidPosition <= config.mMinFluidPosition),
                        TsInitializationException, "Invalid Configuration Data",  "Valve maximum flow area fraction <= valve minimum flow area fraction.");

    /// - Throw a TsInitializationException exception on valve position out of range.
    TS_GENERIC_IF_ERREX((!MsMath::isInRange(config.mMinCmdPosition, input.mCmdPosition, config.mMaxCmdPosition)),
                        TsInitializationException, "Invalid Input Data",          "Valve position out of range.");

    /// - Throw a TsInitializationException exception on manual position out of range.
    TS_GENERIC_IF_ERREX(input.mManualPositionFlag && (!MsMath::isInRange(config.mMinCmdPosition, input.mManualPositionValue, config.mMaxCmdPosition)),
                        TsInitializationException, "Invalid Input Data",          "Manual position out of range.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Time step (not used).
///
/// @note       This method is intended to by called by a manager which has the responsibility
///             for ensuring that this instance has been initialized, hence the lack of an internal
///             initialization check.
///
/// @details    Updates this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsValveController::update(const double dt __attribute__((unused)))
{
    updatePosition(mCmdPosition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  position  (--)  Fractional valve position.
///
/// @details    Updates the fractional valve position of this Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsValveController::updatePosition(const double position)
{
    /// - Skip the position update on a stuck valve malfunction.
    if (!mMalfValveStuckFlag) {
        if (mMalfValveFailToFlag) {
            /// - Set the position to the fail-to value on a valve fail-to position malfunction.
            mCmdPosition = mMalfValveFailToValue;
        } else if (mManualPositionFlag && !mMalfManualFlag) {
            /// - Compute the desired position based on the manual command, if any, and subject to malfunction.
            mCmdPosition = mManualPositionValue;
        } else {
            /// - Otherwise, use the input desired position.
            mCmdPosition = position;
        }
    }

    /// - Update status flags (stuck, lower limit, upper limit).
    mStuckFlag                 = mMalfValveStuckFlag || mMalfValveFailToFlag;
    mLowerLimitFlag            = mCmdPosition <= mMinCmdPosition;
    mUpperLimitFlag            = mCmdPosition >= mMaxCmdPosition;

    /// - In all cases limit the position to the valid range.
    mCmdPosition               = MsMath::limitRange(mMinCmdPosition, mCmdPosition, mMaxCmdPosition);

    /// - Compute the valve flow area fraction from the position and limit range to 0 to 1.
    const double fluidPosition = mFluidBias + mFluidScale * mCmdPosition;
    mFluidPosition             = MsMath::limitRange(0.0, fluidPosition, 1.0);
}


