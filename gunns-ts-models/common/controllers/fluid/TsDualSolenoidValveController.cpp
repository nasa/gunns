/**
@file
@brief     TS21 Fluid Controller Dual Solenoid Valve Controller Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 ((common/controllers/fluid/TsValveController.o))
*/

#include "TsDualSolenoidValveController.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  minCmdPosition    (--)  Minimum valid valve position.
/// @param[in]  maxCmdPosition    (--)  Maximum valid valve position.
/// @param[in]  minFluidPosition  (--)  Minimum valid valve flow area fraction.
/// @param[in]  maxFluidPosition  (--)  Maximum valid valve flow area fraction.
/// @param[in]  latch             (--)  Latching direction when power is removed.
///
/// @details  Default constructs this Dual Solenoid Valve Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveControllerConfigData::TsDualSolenoidValveControllerConfigData(
        const double                                   minCmdPosition,
        const double                                   maxCmdPosition,
        const double                                   minFluidPosition,
        const double                                   maxFluidPosition,
        const TsDualSolenoidValveController::LatchType latch)
    :
    TsValveControllerConfigData(minCmdPosition, maxCmdPosition, minFluidPosition, maxFluidPosition),
    mLatch(latch)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this ImvValve configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveControllerConfigData::~TsDualSolenoidValveControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that  (--)  Object to copy.
///
/// @details  Copy constructs this Dual Solenoid Valve Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveControllerConfigData::TsDualSolenoidValveControllerConfigData(
        const TsDualSolenoidValveControllerConfigData& that)
    :
    TsValveControllerConfigData(that),
    mLatch(that.mLatch)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that  (--)  Object to copy.
///
/// @details  Assignment operator for this Dual Solenoid Valve Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveControllerConfigData& TsDualSolenoidValveControllerConfigData::operator=(
        const TsDualSolenoidValveControllerConfigData& that)
{
    if (this != &that) {
        TsValveControllerConfigData::operator=(that);
        mLatch = that.mLatch;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  cmdPosition          (--)  Initial valve position.
/// @param[in]  manualPositionFlag   (--)  Manual override valve position flag.
/// @param[in]  manualPositionValue  (--)  Manual override valve position value.
/// @param[in]  openSolenoidCmd      (--)  Initial open channel powered flag.
/// @param[in]  closeSolenoidCmd     (--)  Initial close channel powered flag.
///
/// @details  Default constructs this Dual Solenoid Valve Controller input data.  Malfunction
///           argument terms in the base class constructor are defaulted to inactive.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveControllerInputData::TsDualSolenoidValveControllerInputData(
        const double cmdPosition,
        const bool   manualPositionFlag,
        const double manualPositionValue,
        const bool   openSolenoidCmd,
        const bool   closeSolenoidCmd)
    :
    TsValveControllerInputData(cmdPosition, manualPositionFlag, manualPositionValue),
    mOpenSolenoidCmd(openSolenoidCmd),
    mCloseSolenoidCmd(closeSolenoidCmd)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Dual Solenoid Valve Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveControllerInputData::~TsDualSolenoidValveControllerInputData()
{
    // nothing to do
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that  (--)  Object to copy.
///
/// @details  Copy constructs this Dual Solenoid Valve Controller input data.
//////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveControllerInputData::TsDualSolenoidValveControllerInputData(
        const TsDualSolenoidValveControllerInputData& that)
    :
    TsValveControllerInputData(that),
    mOpenSolenoidCmd(that.mOpenSolenoidCmd),
    mCloseSolenoidCmd(that.mCloseSolenoidCmd)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that  (--)  Object to copy.
///
/// @details  Assignment operator for this Dual Solenoid Valve Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveControllerInputData& TsDualSolenoidValveControllerInputData::operator=(
        const TsDualSolenoidValveControllerInputData& that)
{
    if (this != &that) {
        TsValveControllerInputData::operator=(that);
        mOpenSolenoidCmd  = that.mOpenSolenoidCmd;
        mCloseSolenoidCmd = that.mCloseSolenoidCmd;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Dual Solenoid Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveController::TsDualSolenoidValveController()
    :
    TsValveController(),
    mMalfOpenSolenoidFail(false),
    mMalfCloseSolenoidFail(false),
    mLatch(LATCHING),
    mOpenSolenoidCmd(false),
    mCloseSolenoidCmd(false),
    mOpenSolenoidLoad(LoadOFF),
    mCloseSolenoidLoad(LoadOFF),
    mMidCmdPosition(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Dual Solenoid Valve Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDualSolenoidValveController::~TsDualSolenoidValveController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData  (--)  Configuration data.
/// @param[in] inputData   (--)  Input data.
/// @param[in] name        (--)  Object name.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Dual Solenoid Valve Controller model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDualSolenoidValveController::initialize(
        const TsDualSolenoidValveControllerConfigData &configData,
        const TsDualSolenoidValveControllerInputData  &inputData,
        const std::string                             &name)
{
    /// - Initialize the base class
    TsValveController::initialize(configData, inputData, name);
    mInitialized           = false;

    /// - Validate config/input data.
    validate(inputData);

    /// - Initialize from config/input data.
    mLatch                 = configData.mLatch;
    mOpenSolenoidCmd       = inputData.mOpenSolenoidCmd;
    mCloseSolenoidCmd      = inputData.mCloseSolenoidCmd;
    mMidCmdPosition        = 0.5 * (mMaxCmdPosition + mMinCmdPosition);

    /// - Initialize remaining state data.
    mOpenSolenoidLoad      = LoadOFF;
    mCloseSolenoidLoad     = LoadOFF;
    mMalfOpenSolenoidFail  = false;
    mMalfCloseSolenoidFail = false;

    mInitialized           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inputData   (--)  Input data.
///
/// @throws   TsInitializationException
///
/// @details  Throws exceptions and H&S errors on bad input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDualSolenoidValveController::validate(const TsDualSolenoidValveControllerInputData &inputData)
{
    /// - Throw exception if input data has both open & close commands set.
    if (inputData.mOpenSolenoidCmd and inputData.mCloseSolenoidCmd) {
        TS_HS_EXCEPTION(TS_HS_ERROR, TS_HS_GENERIC, "Both solenoid commands set.",
                TsInitializationException, "Invalid Input Data", mName);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  Updates this Dual Solenoid Valve Controller during run-time.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDualSolenoidValveController::update(const double dt __attribute__((unused)))
{
    /// - Determine solenoid energized states based on power supply and malfunction.
    mOpenSolenoidLoad  = calculateSolenoidLoad(mOpenSolenoidCmd,  mMalfOpenSolenoidFail);
    mCloseSolenoidLoad = calculateSolenoidLoad(mCloseSolenoidCmd, mMalfCloseSolenoidFail);

    /// - Determine valve position based on solenoid energized states and latching.  If both
    ///   solenoids are energized, their motive force cancels out and latching takes over.
    double position = mCmdPosition;
    if (LoadOFF == mCloseSolenoidLoad and LoadON == mOpenSolenoidLoad) {
        position = mMaxCmdPosition;
    } else if (LoadOFF == mOpenSolenoidLoad and LoadON == mCloseSolenoidLoad) {
        position = mMinCmdPosition;
    } else if (NORMALLY_OPEN == mLatch) {
        position = mMaxCmdPosition;
    } else if (NORMALLY_CLOSED == mLatch) {
        position = mMinCmdPosition;
    } else if (mCmdPosition >= mMidCmdPosition) {
        position = mMaxCmdPosition;
    } else {
        position = mMinCmdPosition;
    }

    /// - Update the actual valve position.  The valve position malfunctions and manual override
    ///   implemented in the base class take precedence over our solenoid & latching-driven value.
    updatePosition(position);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  cmd   (--)  Solenoid power input flag from user load.
/// @param[in]  malf  (--)  Solenoid power malfunction flag.
///
/// @returns  UserLoadMode  (--)  LoadON if the solenoid is drawing power, otherwise LoadOFF
///
/// @details  Updates this Dual Solenoid Valve Controller during run-time.
////////////////////////////////////////////////////////////////////////////////////////////////////
UserLoadMode TsDualSolenoidValveController::calculateSolenoidLoad(const bool cmd,
                                                                  const bool malf) const
{
    if (cmd and not malf) {
        return LoadON;
    } else {
        return LoadOFF;
    }
}
