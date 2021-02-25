/**
@file
@brief     Example Space Vehicle Aspect implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((common/sensors/SensorAnalog.o)
    (common/controllers/generic/TsPidController.o)
    (common/controllers/fluid/TsPumpMotorController.o)
    (common/controllers/fluid/TsDualSolenoidValveController.o)
    (common/controllers/fluid/TsPositionValveController.o)
    (aspects/signal/effectors/thermostat/Thermostat.o))
*/

#include "ExampleSignal.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsMacros.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Example Space Vehicle Signal Aspect model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ExampleSignalConfigData::ExampleSignalConfigData()
    :
    sensorHxExitT(273.15, 373.15, 273.15, 0.0, 1.0),
    sensorCabinP(0.0, 200.0, 0.0, 0.0, 1.0),
    sensorCabinT(273.15, 373.15, 273.15, 0.0, 1.0),
    sensorCabinPPO2(0.0, 35.0, 0.0, 0.0, 1.0),
    sensorCabinPPCO2(0.0, 2.0, 0.0, 0.0, 1.0),
    sensorCabinRH(0.0, 1.0, 0.0, 0.0, 1.0),
    sensorCabinFanDp(-5.0, 5.0, -1.0, 0.0, 1.0),
    sensorCabinFanN(0.0, 10000.0, 0.0, 0.0, 1.0),
    sensorAtcsFlow(-1.0, 1.0, -1.0, 0.0, 1.0),
    sensorAtcsPumpDp(-200.0, 200.0, -200.0, 0.0, 1.0),
    sensorAtcsPumpN(0.0, 10000.0, 0.0, 0.0, 1.0),
    sensorAtcsPumpInT(200.0, 500.0, 200.0, 0.0, 1.0),
    sensorAtcsPumpInP(0.0, 500.0, 0.0, 0.0, 1.0),
    sensorAtcsRadInT(200.0, 500.0, 200.0, 0.0, 1.0),
    sensorTcvPos(0.0, 1.0, 1.0, 0.0, 1.0),
    sensorRadBypassPos(0.0, 1.0, 1.0, 0.0, 1.0),
    cabinTempCntlr(0.1, 0.0, 10.0, 10.0, 0.0, 1.0),
    atcsTempCntlr(-0.05, 0.0, -0.1, 10.0, 0.0, 1.0),
    cabinFan(),
    pump(),
    gasSelectValve(0.0, 1.0, 0.0, 1.0, 0.1, 1.0, 5.0, 20.0, 40.0, 32.0, false, TsPoweredValveController::LATCHING),
    tcv(0.0, 1.0, 0.0, 1.0, 10.0, 1.0, 5.0, 20.0, 40.0, 32.0, false, TsPoweredValveController::LATCHING, 0.01),
    radBypassValve(0.0, 1.0, 0.0, 1.0, 10.0, 1.0, 5.0, 20.0, 40.0, 32.0, false, TsPoweredValveController::LATCHING, 0.001),
    sensorsShellT(200.0, 500.0, 200.0, 0.0, 1.0),
    fwdShellThermostat(sensorsShellT, 294.261, 297.039),
    aftShellThermostat(sensorsShellT, 294.261, 297.039)
{
    cabinFan.mMotor.mWindingResistance = 0.91;
    cabinFan.mMotor.mTorqueConstant    = 2.0e-2;
    cabinFan.mMotor.mFrictionConstant  = 1.0e-6;
    cabinFan.mMotor.mFrictionMinSpeed  = 7200.0;
    cabinFan.mMotor.mInertia           = 6.0e-4;
    cabinFan.mForcingGain              = 0.2;
    cabinFan.mDampingGain              = 0.2;
    cabinFan.mDampingCutoff            = 0.01;
    cabinFan.mTemperatureTripLimit     = 400.0;
    cabinFan.mTemperatureTripReset     = 395.0;
    cabinFan.mMinVoltage               = 22.0;
    cabinFan.mMaxVoltage               = 40.0;
    cabinFan.mControllerPowerLoad      = 20.0;
    cabinFan.mNoiseAmplitude           = 0.0;
    cabinFan.mNoiseFrequency           = 0.0;
    cabinFan.mStartupCurrentLimit      = 20.0;

    pump.mMotor.mWindingResistance = 1.6;
    pump.mMotor.mTorqueConstant    = 2.7e-2;
    pump.mMotor.mFrictionConstant  = 1.2e-5;
    pump.mMotor.mFrictionMinSpeed  = 5000.0;
    pump.mMotor.mInertia           = 1.0e-3;
    pump.mForcingGain              = 0.2;
    pump.mDampingGain              = 0.2;
    pump.mDampingCutoff            = 0.01;
    pump.mTemperatureTripLimit     = 400.0;
    pump.mTemperatureTripReset     = 395.0;
    pump.mMinVoltage               = 22.0;
    pump.mMaxVoltage               = 40.0;
    pump.mControllerPowerLoad      = 5.0;
    pump.mNoiseAmplitude           = 0.0;
    pump.mNoiseFrequency           = 0.0;
    pump.mStartupCurrentLimit      = 20.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Example Space Vehicle Signal Aspect model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ExampleSignalConfigData::~ExampleSignalConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Example Space Vehicle Signal Aspect model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ExampleSignalInputData::ExampleSignalInputData()
    :
    sensorHxExitT(true),
    sensorCabinP(true, 101.325),
    sensorCabinT(true, 294.261),
    sensorCabinPPO2(true, 20.696),
    sensorCabinPPCO2(true, 0.266),
    sensorCabinRH(true, 0.48),
    sensorCabinFanDp(true),
    sensorCabinFanN(true),
    sensorAtcsFlow(true),
    sensorAtcsPumpDp(true),
    sensorAtcsPumpN(true),
    sensorAtcsPumpInT(true, 280.0),
    sensorAtcsPumpInP(true),
    sensorAtcsRadInT(true),
    sensorTcvPos(true),
    sensorRadBypassPos(true),
    cabinTempCntlr(0.0, 294.261),
    atcsTempCntlr(0.0, 284.817),
    cabinFan(),
    pump(),
    gasSelectValve(0.0, false, 0.0, true, true, 0.0),
    tcv(0.0, false, 0.0, true, true, 0.0),
    radBypassValve(0.0, false, 0.0, true, true, 0.0),
    sensorsShellT(true),
    fwdShellThermostat(sensorsShellT),
    aftShellThermostat(sensorsShellT)
{
    cabinFan.mCommandEnable = true;
    cabinFan.mCommandSpeed  = 8511.0;
    cabinFan.mVoltage       = 32.0;

    pump.mCommandEnable = true;
    pump.mCommandSpeed  = 6000.0;
    pump.mVoltage       = 32.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Example Space Vehicle Signal Aspect model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
ExampleSignalInputData::~ExampleSignalInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this Example Space Vehicle Signal Aspect model.
////////////////////////////////////////////////////////////////////////////////////////////////////
ExampleSignal::ExampleSignal()
    :
    mConfig(),
    mInput(),
    sensorHxExitT(),
    sensorCabinP(),
    sensorCabinT(),
    sensorCabinPPO2(),
    sensorCabinPPCO2(),
    sensorCabinRH(),
    sensorCabinFanDp(),
    sensorCabinFanN(),
    sensorAtcsFlow(),
    sensorAtcsPumpDp(),
    sensorAtcsPumpN(),
    sensorAtcsPumpInT(),
    sensorAtcsPumpInP(),
    sensorAtcsRadInT(),
    sensorTcvPos(),
    sensorRadBypassPos(),
    cabinTempCntlr(),
    atcsTempCntlr(),
    cabinFan(),
    pump(),
    gasSelectValve(),
    tcv(),
    radBypassValve(),
    fwdShellThermostat(),
    aftShellThermostat(),
    mName(""),
    mInitialized(false)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Example Space Vehicle Signal Aspect model.
///////////////////////////////////////////////////////////////////////////////////////////////////
ExampleSignal::~ExampleSignal()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name    (--)  Object name (non-empty).
///
/// @return     void
///
/// @note       All exceptions are caught and replaced with an H&S fatal message making this method
///             suitable for use as a Trick initialization task.
///
/// @details    Initializes this Example Space Vehicle Signal Aspect from config and input data.
///             - Resets the initialization complete flag.
///             - Initializes the object name or issues an H&S fatal message and returns if empty.
///             - Calls initialize to initialize the object from the configuration and input data.
///             - Calls update with zero time step to initialize the outputs.
///             - Calls isInitialized to set the initialization complete flag based on component initialization completion.
///             - Sends an H&S info message on successful initialization.
///             - Sends an H&S fatal message and returns on a TsInitializationException.
///             - Sends an H&S fatal message and returns on an unexpected exception derived from std::exception.
///             - Sends an H&S fatal message and returns on an unexpected exception not derived from std::exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ExampleSignal::init(const std::string& name)
{
    try {
        mInitialized = false;
        GUNNS_NAME_ERREX("TsPidController", name);
        initialize();
        update(0.0);
        mInitialized = isInitialized();
        GUNNS_INFO("initialization complete.");
    } catch (const TsInitializationException& e) {
        GUNNS_FATAL("caught a TsInitializationException.");
    } catch (const std::exception& e) {
        GUNNS_FATAL("caught an unexpected exception." << '\n' << std::string(e.what()));
    } catch (...) {
        GUNNS_FATAL("caught an unexpected exception.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws     TsInitializationException
///
/// @details    Initializes this Example Space Vehicle Signal Aspect from config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ExampleSignal::initialize()
{
    /// - Initialize objects.
    sensorHxExitT     .initialize(mConfig.sensorHxExitT,      mInput.sensorHxExitT,      mName + ".sensorHxExitT");
    sensorCabinP      .initialize(mConfig.sensorCabinP,       mInput.sensorCabinP,       mName + ".sensorCabinP");
    sensorCabinT      .initialize(mConfig.sensorCabinT,       mInput.sensorCabinT,       mName + ".sensorCabinT");
    sensorCabinPPO2   .initialize(mConfig.sensorCabinPPO2,    mInput.sensorCabinPPO2,    mName + ".sensorCabinPPO2");
    sensorCabinPPCO2  .initialize(mConfig.sensorCabinPPCO2,   mInput.sensorCabinPPCO2,   mName + ".sensorCabinPPCO2");
    sensorCabinRH     .initialize(mConfig.sensorCabinRH,      mInput.sensorCabinRH,      mName + ".sensorCabinRH");
    sensorCabinFanDp  .initialize(mConfig.sensorCabinFanDp,   mInput.sensorCabinFanDp,   mName + ".sensorCabinFanDp");
    sensorCabinFanN   .initialize(mConfig.sensorCabinFanN,    mInput.sensorCabinFanN,    mName + ".sensorCabinFanN");
    sensorAtcsFlow    .initialize(mConfig.sensorAtcsFlow,     mInput.sensorAtcsFlow,     mName + ".sensorAtcsFlow");
    sensorAtcsPumpDp  .initialize(mConfig.sensorAtcsPumpDp,   mInput.sensorAtcsPumpDp,   mName + ".sensorAtcsPumpDp");
    sensorAtcsPumpN   .initialize(mConfig.sensorAtcsPumpN,    mInput.sensorAtcsPumpN,    mName + ".sensorAtcsPumpN");
    sensorAtcsPumpInT .initialize(mConfig.sensorAtcsPumpInT,  mInput.sensorAtcsPumpInT,  mName + ".sensorAtcsPumpInT");
    sensorAtcsPumpInP .initialize(mConfig.sensorAtcsPumpInP,  mInput.sensorAtcsPumpInP,  mName + ".sensorAtcsPumpInP");
    sensorAtcsRadInT  .initialize(mConfig.sensorAtcsRadInT,   mInput.sensorAtcsRadInT,   mName + ".sensorAtcsRadInT");
    sensorTcvPos      .initialize(mConfig.sensorTcvPos,       mInput.sensorTcvPos,       mName + ".sensorTcvPos");
    sensorRadBypassPos.initialize(mConfig.sensorRadBypassPos, mInput.sensorRadBypassPos, mName + ".sensorRadBypassPos");
    cabinTempCntlr    .initialize(mConfig.cabinTempCntlr,     mInput.cabinTempCntlr,     mName + ".cabinTempCntlr");
    atcsTempCntlr     .initialize(mConfig.atcsTempCntlr,      mInput.atcsTempCntlr,      mName + ".atcsTempCntlr");
    cabinFan          .initialize(mConfig.cabinFan,           mInput.cabinFan,           mName + ".cabinFan");
    pump              .initialize(mConfig.pump,               mInput.pump,               mName + ".pump");
    gasSelectValve    .initialize(mConfig.gasSelectValve,     mInput.gasSelectValve,     mName + ".gasSelectValve");
    tcv               .initialize(mConfig.tcv,                mInput.tcv,                mName + ".tcv");
    radBypassValve    .initialize(mConfig.radBypassValve,     mInput.radBypassValve,     mName + ".radBypassValve");
    fwdShellThermostat.initialize(mConfig.fwdShellThermostat, mInput.fwdShellThermostat, mName + ".fwdShellThermostat");
    aftShellThermostat.initialize(mConfig.aftShellThermostat, mInput.aftShellThermostat, mName + ".aftShellThermostat");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  True if initialization completed successfully, false otherwise.
///
/// @details  Gets this Example Space Vehicle Signal Aspect initialization complete status.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool ExampleSignal::isInitialized() const
{
    return (sensorHxExitT     .isInitialized() and
            sensorCabinP      .isInitialized() and
            sensorCabinT      .isInitialized() and
            sensorCabinPPO2   .isInitialized() and
            sensorCabinPPCO2  .isInitialized() and
            sensorCabinRH     .isInitialized() and
            sensorCabinFanDp  .isInitialized() and
            sensorCabinFanN   .isInitialized() and
            sensorAtcsFlow    .isInitialized() and
            sensorAtcsPumpDp  .isInitialized() and
            sensorAtcsPumpN   .isInitialized() and
            sensorAtcsPumpInT .isInitialized() and
            sensorAtcsPumpInP .isInitialized() and
            sensorAtcsRadInT  .isInitialized() and
            sensorTcvPos      .isInitialized() and
            sensorRadBypassPos.isInitialized() and
            cabinTempCntlr    .isInitialized() and
            atcsTempCntlr     .isInitialized() and
            cabinFan          .isInitialized() and
            pump              .isInitialized() and
            gasSelectValve    .isInitialized() and
            tcv               .isInitialized() and
            radBypassValve    .isInitialized() and
            fwdShellThermostat.isInitialized() and
            aftShellThermostat.isInitialized());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Time step.
///
/// @note       All exceptions are caught and replaced with an H&S error message making this method
///             suitable for use as a Trick scheduled task.
///
/// @details    Updates this Example Space Vehicle Signal Aspect.
///             - Sends an H&S error message and returns on an uninitialized object.
///             - Calls update to update the models.
///             - Sends an H&S error message and returns on an unexpected exception derived from std::exception.
///             - Sends an H&S error message and returns on an unexpected exception not derived from std::exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ExampleSignal::step(const double dt)
{
    try {
        if (!mInitialized) {
            GUNNS_WARNING("is not initialized.");
            return;
        }
        update(dt);
    } catch (const std::exception& e) {
        GUNNS_WARNING("caught an unexpected exception." << '\n' << std::string(e.what()));
    } catch (...) {
        GUNNS_WARNING("caught an unexpected exception.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s) Execution time step.
///
/// @details    Updates this Example Space Vehicle Signal Aspect.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ExampleSignal::update(const double dt)
{
    /// - Update sensors.
    sensorHxExitT.update(dt);
    sensorCabinP.update(dt);
    sensorCabinT.update(dt);
    sensorCabinPPO2.update(dt);
    sensorCabinPPCO2.update(dt);
    sensorCabinRH.update(dt);
    sensorCabinFanDp.update(dt);
    sensorCabinFanN.sense(dt, true, cabinFan.getMotorSpeed());
    sensorAtcsFlow.update(dt);
    sensorAtcsPumpDp.update(dt);
    sensorAtcsPumpN.sense(dt, true, pump.getMotorSpeed());
    sensorAtcsPumpInT.update(dt);
    sensorAtcsPumpInP.update(dt);
    sensorAtcsRadInT.update(dt);
    sensorTcvPos.sense(dt, true, tcv.getPosition());
    sensorRadBypassPos.sense(dt, true, radBypassValve.getPosition());

    /// - Calculate commanded state of the gasSelectValve.  We model this as a bang-bang valve to
    ///   switch to O2 when cabin PPO2 is low and back to N2 when it is high.
    TsOpenCloseValveCmd gasSelectVlvCmd(true, false, false);
    if (sensorCabinPPO2.getSensedOutput() < 20.0) {
        gasSelectVlvCmd.mClose = true;
        gasSelectVlvCmd.mOpen  = false;
    } else if (sensorCabinPPO2.getSensedOutput() > 22.0) {
        gasSelectVlvCmd.mClose = false;
        gasSelectVlvCmd.mOpen  = true;
    }

    /// - Update effectors.
    cabinFan.setSensedSpeed(sensorCabinFanN.getSensedOutput());
    cabinFan.step(dt);
    pump.setSensedSpeed(sensorAtcsPumpN.getSensedOutput());
    pump.step(dt);
    gasSelectValve.setCommand(gasSelectVlvCmd);
    gasSelectValve.update(dt);
    tcv.setSensed(sensorTcvPos.getSensedOutput());
    tcv.setPositionCommand(cabinTempCntlr.update(dt, sensorCabinT.getSensedOutput()));
    tcv.update(dt);
    radBypassValve.setSensed(sensorRadBypassPos.getSensedOutput());
    radBypassValve.setPositionCommand(atcsTempCntlr.update(dt, sensorAtcsPumpInT.getSensedOutput()));
    radBypassValve.update(dt);
    fwdShellThermostat.update(dt);
    aftShellThermostat.update(dt);
}
