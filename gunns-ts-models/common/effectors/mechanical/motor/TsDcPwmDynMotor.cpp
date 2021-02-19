/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (PWM-Controlled Dynamic DC Motor Model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((math/UnitConversion.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Install) (2012-07))
**************************************************************************************************/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"
#include "math/MsMath.hh"
#include "math/UnitConversion.hh"
#include "TsDcPwmDynMotor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] pulseWasteFraction (--)                 (0-1) Fraction of power wasted during PWM off-time.
/// @param[in] stallTorqueCoeff1  (N*m/V)              Stall torque curve 1st-order coefficient.
/// @param[in] stallTorqueCoeff2  (N*m/V)              Stall torque curve 2nd-order coefficient.
/// @param[in] frictionTorque     (N*m)                Friction torque (constant).
/// @param[in] bemfConstant       (V*min/revolution)   Back-EMF constant.
/// @param[in] armatureResistance (ohm)                Electrical resistance of motor armature.
/// @param[in] inertia            (kg*m2)              Inertia of the motor+load mass.
/// @param[in] speedLoadRatio     (revolution/min/N/m) Slope of motor speed/torque line.
///
/// @details  Default constructs this Motor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotorConfigData::TsDcPwmDynMotorConfigData(const double pulseWasteFraction,
                                                     const double stallTorqueCoeff1,
                                                     const double stallTorqueCoeff2,
                                                     const double frictionTorque,
                                                     const double bemfConstant,
                                                     const double armatureResistance,
                                                     const double inertia,
                                                     const double speedLoadRatio)
    :
    mPulseWasteFraction(pulseWasteFraction),
    mStallTorqueCoeff1 (stallTorqueCoeff1),
    mStallTorqueCoeff2 (stallTorqueCoeff2),
    mFrictionTorque    (frictionTorque),
    mBemfConstant      (bemfConstant),
    mArmatureResistance(armatureResistance),
    mInertia           (inertia),
    mSpeedLoadRatio    (speedLoadRatio)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this Motor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotorConfigData::TsDcPwmDynMotorConfigData(const TsDcPwmDynMotorConfigData& that)
    :
    mPulseWasteFraction(that.mPulseWasteFraction),
    mStallTorqueCoeff1 (that.mStallTorqueCoeff1),
    mStallTorqueCoeff2 (that.mStallTorqueCoeff2),
    mFrictionTorque    (that.mFrictionTorque),
    mBemfConstant      (that.mBemfConstant),
    mArmatureResistance(that.mArmatureResistance),
    mInertia           (that.mInertia),
    mSpeedLoadRatio    (that.mSpeedLoadRatio)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Motor config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotorConfigData& TsDcPwmDynMotorConfigData::operator =(const TsDcPwmDynMotorConfigData& that)
{
    if (this != &that)
    {
        mPulseWasteFraction = that.mPulseWasteFraction;
        mStallTorqueCoeff1  = that.mStallTorqueCoeff1;
        mStallTorqueCoeff2  = that.mStallTorqueCoeff2;
        mFrictionTorque     = that.mFrictionTorque;
        mBemfConstant       = that.mBemfConstant;
        mArmatureResistance = that.mArmatureResistance;
        mInertia            = that.mInertia;
        mSpeedLoadRatio     = that.mSpeedLoadRatio;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Motor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotorConfigData::~TsDcPwmDynMotorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage     (V)              Initial supply voltage.
/// @param[in] pulseWidth  (--)             (0-1) Initial pulse width.
/// @param[in] motorSpeed  (revolution/min) Initial motor speed.
/// @param[in] loadTorque1 (N*m)            1st initial external load torque.
/// @param[in] loadTorque2 (N*m)            2nd initial external load torque.
/// @param[in] loadTorque3 (N*m)            3rd initial external load torque.
/// @param[in] loadTorque4 (N*m)            4th initial external load torque.
///
/// @details  Default constructs this Motor input data.  Malfunction parameters are provided as
///           class attributes to support unit testing, but are not needed to be specified in the
///           constructor, thus no arguments are provided.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotorInputData::TsDcPwmDynMotorInputData(const double voltage,
                                                   const double pulseWidth,
                                                   const double motorSpeed,
                                                   const double loadTorque1,
                                                   const double loadTorque2,
                                                   const double loadTorque3,
                                                   const double loadTorque4)
    :
    mVoltage               (voltage),
    mPulseWidth            (pulseWidth),
    mMotorSpeed            (motorSpeed),
    mLoadTorque1           (loadTorque1),
    mLoadTorque2           (loadTorque2),
    mLoadTorque3           (loadTorque3),
    mLoadTorque4           (loadTorque4),
    mMalfDegradeFlag       (false),
    mMalfDegradeValue      (0.0),
    mMalfJamFlag           (false),
    mMalfJamValue          (0.0),
    mMalfSpeedOverrideFlag (false),
    mMalfSpeedOverrideValue(0.0),
    mMalfHeatRateBiasFlag  (false),
    mMalfHeatRateBiasValue (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this Motor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotorInputData::TsDcPwmDynMotorInputData(const TsDcPwmDynMotorInputData& that)
    :
    mVoltage               (that.mVoltage),
    mPulseWidth            (that.mPulseWidth),
    mMotorSpeed            (that.mMotorSpeed),
    mLoadTorque1           (that.mLoadTorque1),
    mLoadTorque2           (that.mLoadTorque2),
    mLoadTorque3           (that.mLoadTorque3),
    mLoadTorque4           (that.mLoadTorque4),
    mMalfDegradeFlag       (that.mMalfDegradeFlag),
    mMalfDegradeValue      (that.mMalfDegradeValue),
    mMalfJamFlag           (that.mMalfJamFlag),
    mMalfJamValue          (that.mMalfJamValue),
    mMalfSpeedOverrideFlag (that.mMalfSpeedOverrideFlag),
    mMalfSpeedOverrideValue(that.mMalfSpeedOverrideValue),
    mMalfHeatRateBiasFlag  (that.mMalfHeatRateBiasFlag),
    mMalfHeatRateBiasValue (that.mMalfHeatRateBiasValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Motor input data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotorInputData& TsDcPwmDynMotorInputData::operator =(const TsDcPwmDynMotorInputData& that)
{
    if (this != &that)
    {
        mVoltage                = that.mVoltage;
        mPulseWidth             = that.mPulseWidth;
        mMotorSpeed             = that.mMotorSpeed;
        mLoadTorque1            = that.mLoadTorque1;
        mLoadTorque2            = that.mLoadTorque2;
        mLoadTorque3            = that.mLoadTorque3;
        mLoadTorque4            = that.mLoadTorque4;
        mMalfDegradeFlag        = that.mMalfDegradeFlag;
        mMalfDegradeValue       = that.mMalfDegradeValue;
        mMalfJamFlag            = that.mMalfJamFlag;
        mMalfJamValue           = that.mMalfJamValue;
        mMalfSpeedOverrideFlag  = that.mMalfSpeedOverrideFlag;
        mMalfSpeedOverrideValue = that.mMalfSpeedOverrideValue;
        mMalfHeatRateBiasFlag   = that.mMalfHeatRateBiasFlag;
        mMalfHeatRateBiasValue  = that.mMalfHeatRateBiasValue;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Motor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotorInputData::~TsDcPwmDynMotorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling the step
///           method.
///
/// @details  Default constructs this Motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotor::TsDcPwmDynMotor()
    :
    mMalfDegradeFlag(false),
    mMalfDegradeValue(0.0),
    mMalfJamFlag(false),
    mMalfJamValue(0.0),
    mMalfSpeedOverrideFlag(false),
    mMalfSpeedOverrideValue(0.0),
    mMalfHeatRateBiasFlag(false),
    mMalfHeatRateBiasValue(0.0),
    mName(),
    mPulseWasteFraction(0.0),
    mStallTorqueCoeff1(0.0),
    mStallTorqueCoeff2(0.0),
    mFrictionTorque(0.0),
    mBemfConstant(0.0),
    mArmatureResistance(0.0),
    mInertia(0.0),
    mSpeedLoadRatio(0.0),
    mVoltage(0.0),
    mPulseWidth(0.0),
    mMotorSpeed(0.0),
    mPower(0.0),
    mCurrent(0.0),
    mGeneratedCurrent(0.0),
    mResistance(0.0),
    mWasteHeat(0.0),
    mEfficiency(0.0),
    mStallTorque(0.0),
    mDriveTorque(0.0),
    mTotalExternalLoad(0.0),
    mInitFlag(false)
{
    mLoadTorques[0] = 0.0;
    mLoadTorques[1] = 0.0;
    mLoadTorques[2] = 0.0;
    mLoadTorques[3] = 0.0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Motor.
///////////////////////////////////////////////////////////////////////////////////////////////////
TsDcPwmDynMotor::~TsDcPwmDynMotor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
/// @param[in] inputData  (--) Input data.
/// @param[in] name       (--) Name of the motor for output messages.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Motor with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDcPwmDynMotor::initialize(const TsDcPwmDynMotorConfigData& configData,
                                 const TsDcPwmDynMotorInputData&  inputData,
                                 const std::string&               name)
{
    /// - Reset the initialized flag.
    mInitFlag               = false;

    /// - Allocate and initialize the object name or throw a TsInitializationException exception if empty.
    TS_GENERIC_NAME_ERREX("TsDcPwmDynMotor")

    /// - Validate initialization data.
    validate(configData, inputData);

    /// - Initialize with config data.
    mPulseWasteFraction     = configData.mPulseWasteFraction;
    mStallTorqueCoeff1      = configData.mStallTorqueCoeff1;
    mStallTorqueCoeff2      = configData.mStallTorqueCoeff2;
    mFrictionTorque         = configData.mFrictionTorque;
    mBemfConstant           = configData.mBemfConstant;
    mArmatureResistance     = configData.mArmatureResistance;
    mInertia                = configData.mInertia;
    mSpeedLoadRatio         = configData.mSpeedLoadRatio;

    /// - Initialize with input data.
    mVoltage                = inputData.mVoltage;
    mPulseWidth             = inputData.mPulseWidth;
    mMotorSpeed             = inputData.mMotorSpeed;
    mLoadTorques[0]         = inputData.mLoadTorque1;
    mLoadTorques[1]         = inputData.mLoadTorque2;
    mLoadTorques[2]         = inputData.mLoadTorque3;
    mLoadTorques[3]         = inputData.mLoadTorque4;
    mMalfDegradeFlag        = inputData.mMalfDegradeFlag;
    mMalfDegradeValue       = inputData.mMalfDegradeValue;
    mMalfJamFlag            = inputData.mMalfJamFlag;
    mMalfJamValue           = inputData.mMalfJamValue;
    mMalfSpeedOverrideFlag  = inputData.mMalfSpeedOverrideFlag;
    mMalfSpeedOverrideValue = inputData.mMalfSpeedOverrideValue;
    mMalfHeatRateBiasFlag   = inputData.mMalfHeatRateBiasFlag;
    mMalfHeatRateBiasValue  = inputData.mMalfHeatRateBiasValue;

    /// - Initialize remaining state data.
    mPower                  = 0.0;
    mCurrent                = 0.0;
    mGeneratedCurrent       = 0.0;
    mResistance             = 0.0;
    mWasteHeat              = 0.0;
    mEfficiency             = 0.0;
    mStallTorque            = 0.0;
    mDriveTorque            = 0.0;
    mTotalExternalLoad      = 0.0;
    mInitFlag               = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
/// @param[in] inputData  (--) Input data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates this Motor's configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDcPwmDynMotor::validate(const TsDcPwmDynMotorConfigData& configData,
                               const TsDcPwmDynMotorInputData&  inputData) const
{
    /// - Throw an exception if pulse waste fraction not within 0-1.
    if (!MsMath::isInRange(0.0, configData.mPulseWasteFraction, 1.0)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Input Data", TsInitializationException,
                "Pulse waste fraction cannot be outside 0-1.", mName);
    }

    /// - Throw an exception if friction torque is positive.
    if (configData.mFrictionTorque > 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Friction torque cannot be > 0.", mName);
    }

    /// - Throw an exception if armature resistance < DBL_EPSILON.
    if (configData.mArmatureResistance < DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Armature resistance cannot be < DBL_EPSILON.", mName);
    }

    /// - Throw an exception if inertia < DBL_EPSILON.
    if (configData.mInertia < DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Inertia cannot be < DBL_EPSILON.", mName);
    }

    /// - Throw an exception if speed/load ratio < DBL_EPSILON.
    if (configData.mSpeedLoadRatio < DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Speed/Load ratio cannot be < DBL_EPSILON.", mName);
    }

    /// - Throw an exception if initial voltage < 0.
    if (inputData.mVoltage < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Input Data", TsInitializationException,
                "Initial voltage cannot be < 0.", mName);
    }

    /// - Throw an exception if initial pulse width not within 0-1.
    if (!MsMath::isInRange(0.0, inputData.mPulseWidth, 1.0)) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Input Data", TsInitializationException,
                "Initial pulse width cannot be outside 0-1.", mName);
    }

    /// - Throw an exception if initial speed < 0.
    if (inputData.mMotorSpeed < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Input Data", TsInitializationException,
                "Initial speed cannot be < 0.", mName);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Time step.
///
/// @returns  void
///
/// @details  Updates all motor parameters over the time step.  The called virtual methods are meant
///           to be overridden by derived classes to implement extra features as needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDcPwmDynMotor::step(const double dt)
{
    generateMotorTorque();
    gatherExternalLoads();
    computeMotorSpeed(dt);
    computeElectricalOutputs();
    computeWasteHeat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Stall torque is the maximum torque the motor can produce with the given input voltage,
///           which occurs at speed zero (stall) and full pulse width.  It generally increases
///           linearly with voltage, but ours is a 2nd-order polynomial, useful for tuning the motor
///           to match empirical data at design points, etc.
///
///           The degradation malfunction reduces the torque the motor can produce, caused by brush
///           contactor erosion or broken rotor windings, etc.
///
///           The torque actually produced by the motor decreases from stall torque linearly with
///           speed, and is scaled by the power-on time fraction represented by the pulse width.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDcPwmDynMotor::generateMotorTorque()
{
    mStallTorque = mStallTorqueCoeff1 * mVoltage
                 + mStallTorqueCoeff2 * mVoltage * mVoltage;

    if (mMalfDegradeFlag) {
        mStallTorque *= (1.0 - mMalfDegradeValue);
    }

    mDriveTorque = std::max(mStallTorque - mMotorSpeed / mSpeedLoadRatio, 0.0) * mPulseWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  The motor supports up to 4 external loads, which can come from simbus or setter
///           method.  The jam malfunction applies an extra external torque, calculated as a
///           fraction of stall torque, such that at 100% jam the motor cannot accelerate even if
///           all other external loads are removed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDcPwmDynMotor::gatherExternalLoads()
{
    mTotalExternalLoad = mLoadTorques[0] + mLoadTorques[1] + mLoadTorques[2] + mLoadTorques[3];

    if (mMalfJamFlag) {
        mTotalExternalLoad -= mMalfJamValue * mStallTorque;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  The balance of all torques is divided by motor inertia to accelerate decelerate the
///           motor.  Our motor cannot spin backwards.  Friction is assumed constant.  All torques
///           are signed relative to the forward rotation of the motor, so torque produced by the
///           motor is positive, while loads and friction are negative.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDcPwmDynMotor::computeMotorSpeed(const double dt)
{
    /// - Torque and inertia are related to angular velocity in r/s, must be converted to rev/min.
    mMotorSpeed += (mDriveTorque + mTotalExternalLoad + mFrictionTorque) * dt
                 * UnitConversion::SEC_PER_MIN_PER_2PI / mInertia;
    mMotorSpeed = std::max(mMotorSpeed, 0.0);

    /// - The speed override malfunction completely overrides all motor dynamics and forces a
    ///   desired speed.
    if (mMalfSpeedOverrideFlag) {
        mMotorSpeed = mMalfSpeedOverrideValue;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Maximum current & power occurs at stall, and is reduced as speed increases by the
///           back-EMF effect.  The degradation malfunction effectively increases the electrical
///           resistance, reducing current & power.  Pulse Width Modulation scales the current &
///           power in proportion to the on-time, but mPulseWasteFraction causes current during the
///           off-time as well, to simulate losses in the PWM chopper circuit.
///
///           The actual output to the electrical aspect Resistive User Load object is the total
///           motor resistance, although power can be used with a Constant Power User Load if
///           desired.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDcPwmDynMotor::computeElectricalOutputs()
{
    const double pwmFactor = mPulseWasteFraction + mPulseWidth * (1.0 - mPulseWasteFraction);
    const double bemf = mBemfConstant * mMotorSpeed / mArmatureResistance;
    mCurrent = pwmFactor * (mVoltage / mArmatureResistance - bemf);
    if (mMalfDegradeFlag) {
        mCurrent *= (1.0 - mMalfDegradeValue);
    }
    mCurrent          = std::max(mCurrent, 0.0);
    mGeneratedCurrent = std::max(bemf - mCurrent, 0.0);
    mPower            = mCurrent * mVoltage;

    if (mCurrent > DBL_EPSILON) {
        mResistance = mVoltage / mCurrent;
    } else {
        mResistance = 1.0 / DBL_EPSILON;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  All power produced by the motor that is not absorbed by the external load is assumed
///           to become waste heat.  Further losses in the external load model are not included in
///           our waste heat or efficiency.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsDcPwmDynMotor::computeWasteHeat()
{
    /// - Motor speed is converted to r/s to relate power in Watts.
    const double usefulPower = -mTotalExternalLoad * mMotorSpeed
                             / UnitConversion::SEC_PER_MIN_PER_2PI;

    /// - Update waste heat and include the heat rate bias.
    mWasteHeat = std::max(mPower - usefulPower, 0.0);
    if (mMalfHeatRateBiasFlag) {
        mWasteHeat += mMalfHeatRateBiasValue;
    }

    /// - Efficiency is ratio of useful power to total power demand, protected for divide by zero.
    if (mPower > 0.0) {
        mEfficiency = usefulPower / mPower;
    } else {
        mEfficiency = 0.0;
    }
}
