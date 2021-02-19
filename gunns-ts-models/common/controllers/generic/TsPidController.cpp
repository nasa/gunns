/**
@file
@brief    PID Controller Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((simulation/hs/TsHsMsg.o)
  (software/exceptions/TsInitializationException.o))
*/

#include "TsPidController.hh"
#include "GenericMacros.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  gainP      (--)   Proportional gain.
/// @param[in]  gainI      (1/s)  Integral gain.
/// @param[in]  gainD      (s)    Derivative gain.
/// @param[in]  interval   (s)    Control update interval.
/// @param[in]  limitLow   (--)   Lower limit on output value.
/// @param[in]  limitHigh  (--)   Upper limit on output value.
///
/// @details  Default constructs this PID Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidControllerConfigData::TsPidControllerConfigData(const double gainP,
                                                     const double gainI,
                                                     const double gainD,
                                                     const double interval,
                                                     const double limitLow,
                                                     const double limitHigh)
    :
    mGainP(gainP),
    mGainI(gainI),
    mGainD(gainD),
    mInterval(interval),
    mLimitLow(limitLow),
    mLimitHigh(limitHigh)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this PID Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidControllerConfigData::~TsPidControllerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this PID Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidControllerConfigData::TsPidControllerConfigData(const TsPidControllerConfigData& that)
    :
    mGainP(that.mGainP),
    mGainI(that.mGainI),
    mGainD(that.mGainD),
    mInterval(that.mInterval),
    mLimitLow(that.mLimitLow),
    mLimitHigh(that.mLimitHigh)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object assigned from.
///
/// @return   TsPidControllerConfigData&  (--)  Object assigned to.
///
/// @details  Assignment operator for this PID Controller configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidControllerConfigData& TsPidControllerConfigData::operator=(const TsPidControllerConfigData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        mGainP     = that.mGainP;
        mGainI     = that.mGainI;
        mGainD     = that.mGainD;
        mInterval  = that.mInterval;
        mLimitLow  = that.mLimitLow;
        mLimitHigh = that.mLimitHigh;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  input       (--)   Initial input process variable.
/// @param[in]  setpoint    (1/s)  Initial setpoint value to control to.
/// @param[in]  timer       (s)    Initial timer for control update interval.
/// @param[in]  integral    (s)    Initial controller integral value.
/// @param[in]  output      (--)   Initial output control value.
///
/// @details  Default constructs this PID Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidControllerInputData::TsPidControllerInputData(const double input,
                                                   const double setpoint,
                                                   const double timer,
                                                   const double integral,
                                                   const double output)
    :
    mInput(input),
    mSetpoint(setpoint),
    mTimer(timer),
    mIntegral(integral),
    mOutput(output)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this PID Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidControllerInputData::~TsPidControllerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this PID Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidControllerInputData::TsPidControllerInputData(const TsPidControllerInputData& that)
    :
    mInput(that.mInput),
    mSetpoint(that.mSetpoint),
    mTimer(that.mTimer),
    mIntegral(that.mIntegral),
    mOutput(that.mOutput)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Object assigned from.
///
/// @return   TsPidControllerInputData&  (--)  Object assigned to.
///
/// @details  Assignment operator for this PID Controller input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidControllerInputData& TsPidControllerInputData::operator=(const TsPidControllerInputData& that)
{
    /// - Skip attribute assignment if objects are the same
    if (this != &that) {
        mInput     = that.mInput;
        mSetpoint     = that.mSetpoint;
        mTimer     = that.mTimer;
        mIntegral  = that.mIntegral;
        mOutput  = that.mOutput;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this PID Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidController::TsPidController()
    :
    mGainP(0.0),
    mGainI(0.0),
    mGainD(0.0),
    mInterval(0.0),
    mLimitLow(0.0),
    mLimitHigh(0.0),
    mInput(0.0),
    mSetpoint(0.0),
    mTimer(0.0),
    mIntegral(0.0),
    mOutput(0.0),
    mError(0.0),
    mDerivative(0.0),
    mName(""),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this PID Controller model.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsPidController::~TsPidController()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
/// @param[in]  inputData   (--)  Input data.
/// @param[in]  name        (--)  Instance name for messages.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this PID Controller model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPidController::initialize(const TsPidControllerConfigData& configData,
                                 const TsPidControllerInputData&  inputData,
                                 const std::string&               name)
{
    /// - Reset the initialization complete flag.
    mInitFlag   = false;

    /// - Initialize instance name.
    TS_GENERIC_NAME_ERREX("TsPidController");

    /// - Validate config data.
    validate(configData);

    /// - Initialize with config & input data.
    mGainP      = configData.mGainP;
    mGainI      = configData.mGainI;
    mGainD      = configData.mGainD;
    mInterval   = configData.mInterval;
    mLimitLow   = configData.mLimitLow;
    mLimitHigh  = configData.mLimitHigh;
    mInput      = inputData.mInput;
    mSetpoint   = inputData.mSetpoint;
    mTimer      = inputData.mTimer;
    mIntegral   = inputData.mIntegral;
    mOutput     = inputData.mOutput;

    /// - Initialize remaining state data.
    mError      = mInput - mSetpoint;
    mDerivative = 0.0;

    /// - Set the initialization complete flag.
    mInitFlag   = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this PID Controller model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsPidController::validate(const TsPidControllerConfigData &configData) const
{
    /// - Issue an error if update interval <= 0.
    if (not (configData.mInterval > 0.0)) {
        TS_GENERIC_ERREX(TsInitializationException, "Invalid Configuration Data",
                         "Update interval is not > 0.");
    }

    /// - Issue an error if update interval <= 0.
    if (not (configData.mLimitHigh > configData.mLimitLow)) {
        TS_GENERIC_ERREX(TsInitializationException, "Invalid Configuration Data",
                         "High limit not > low limit.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt     (s)   Execution time step.
///
/// @returns  double  (--)  Resulting output value.
///
/// @details  If the elapsed time since the last output update has passed the interval, then updates
///           the output value with the PID logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsPidController::update(const double dt)
{
    mTimer += dt;
    if (mInterval > 0.0 and mTimer >= mInterval) {
        const double prevError = mError;
        mError      = mInput - mSetpoint;
        mIntegral  += mError * mTimer;
        mDerivative = (mError - prevError) / mTimer;
        mOutput    += (mError * mGainP + mIntegral * mGainI + mDerivative * mGainD);
        mTimer      = 0.0;

        /// - Zero very small results to avoid underflows.
        if (fabs(mIntegral) < DBL_EPSILON) {
            mIntegral = 0.0;
        }
        if (fabs(mOutput) < DBL_EPSILON) {
            mOutput = 0.0;
        }

        /// - Limit the output to the allowable range.
        mOutput = MsMath::limitRange(mLimitLow, mOutput, mLimitHigh);
    }
    return mOutput;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt     (s)   Execution time step.
/// @param[in]  input  (--)  Input value to control.
///
/// @returns  double  (--)  Resulting output value.
///
/// @details  Calls setInput with input, then calls update(dt) to update the controller.
////////////////////////////////////////////////////////////////////////////////////////////////////
double TsPidController::update(const double dt, const double input)
{
    setInput(input);
    return update(dt);
}
