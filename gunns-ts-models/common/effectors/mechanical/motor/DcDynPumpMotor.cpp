/**
@file
@brief     DC Dynamic Pump Motor Model implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
   ((math/UnitConversion.o))
*/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "GenericMacros.hh"
#include "math/MsMath.hh"
#include "math/UnitConversion.hh"
#include "DcDynPumpMotor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] windingResistance (ohm)                Electrical resistance of motor winding.
/// @param[in] torqueConstant    (N*m/amp)            Torque and back-EMF (v*s/r) constant.
/// @param[in] frictionConstant  (N*m*min/revolution) Dynamic friction torque constant.
/// @param[in] frictionMinSpeed  (revolution/min)     Minimum speed for dynamic friction.
/// @param[in] inertia           (kg*m2)              Inertia of the motor+load mass.
///
/// @details  Default constructs this Motor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DcDynPumpMotorConfigData::DcDynPumpMotorConfigData(const double windingResistance,
                                                   const double torqueConstant,
                                                   const double frictionConstant,
                                                   const double frictionMinSpeed,
                                                   const double inertia)
    :
    mWindingResistance(windingResistance),
    mTorqueConstant   (torqueConstant),
    mFrictionConstant (frictionConstant),
    mFrictionMinSpeed (frictionMinSpeed),
    mInertia          (inertia)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this Motor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DcDynPumpMotorConfigData::DcDynPumpMotorConfigData(const DcDynPumpMotorConfigData& that)
    :
    mWindingResistance(that.mWindingResistance),
    mTorqueConstant   (that.mTorqueConstant),
    mFrictionConstant (that.mFrictionConstant),
    mFrictionMinSpeed (that.mFrictionMinSpeed),
    mInertia          (that.mInertia)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to assign to
///
/// @details  Assignment operator for the Motor config data class.
////////////////////////////////////////////////////////////////////////////////////////////////////
DcDynPumpMotorConfigData& DcDynPumpMotorConfigData::operator =(const DcDynPumpMotorConfigData& that)
{
    if (this != &that)
    {
        mWindingResistance = that.mWindingResistance,
        mTorqueConstant    = that.mTorqueConstant,
        mFrictionConstant  = that.mFrictionConstant,
        mFrictionMinSpeed  = that.mFrictionMinSpeed,
        mInertia           = that.mInertia;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Motor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
DcDynPumpMotorConfigData::~DcDynPumpMotorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] voltage      (V)              Initial control voltage.
/// @param[in] currentLimit (amp)            Initial current limit.
/// @param[in] motorSpeed   (revolution/min) Initial motor speed.
/// @param[in] loadTorque1  (N*m)            1st initial external load torque.
/// @param[in] loadTorque2  (N*m)            2nd initial external load torque.
/// @param[in] loadTorque3  (N*m)            3rd initial external load torque.
/// @param[in] loadTorque4  (N*m)            4th initial external load torque.
///
/// @details  Default constructs this Motor input data.  Malfunction parameters are provided as
///           class attributes to support unit testing, but are not needed to be specified in the
///           constructor, thus no arguments are provided.
////////////////////////////////////////////////////////////////////////////////////////////////////
DcDynPumpMotorInputData::DcDynPumpMotorInputData(const double voltage,
                                                 const double currentLimit,
                                                 const double motorSpeed,
                                                 const double loadTorque1,
                                                 const double loadTorque2,
                                                 const double loadTorque3,
                                                 const double loadTorque4)
    :
    mVoltage               (voltage),
    mCurrentLimit          (currentLimit),
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
DcDynPumpMotorInputData::DcDynPumpMotorInputData(const DcDynPumpMotorInputData& that)
    :
    mVoltage               (that.mVoltage),
    mCurrentLimit          (that.mCurrentLimit),
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
DcDynPumpMotorInputData& DcDynPumpMotorInputData::operator =(const DcDynPumpMotorInputData& that)
{
    if (this != &that)
    {
        mVoltage                = that.mVoltage;
        mCurrentLimit           = that.mCurrentLimit;
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
DcDynPumpMotorInputData::~DcDynPumpMotorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling the step
///           method.
///
/// @details  Default constructs this Motor.
////////////////////////////////////////////////////////////////////////////////////////////////////
DcDynPumpMotor::DcDynPumpMotor()
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
    mWindingResistance(0.0),
    mTorqueConstant(0.0),
    mFrictionConstant(0.0),
    mFrictionMinSpeed(0.0),
    mInertia(0.0),
    mVoltage(0.0),
    mCurrentLimit(0.0),
    mMotorSpeed(0.0),
    mPower(0.0),
    mCurrent(0.0),
    mResistance(0.0),
    mWasteHeat(0.0),
    mEfficiency(0.0),
    mStallTorque(0.0),
    mDriveTorque(0.0),
    mFrictionTorque(0.0),
    mTotalExternalLoad(0.0),
    mDegradedResistance(0.0),
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
DcDynPumpMotor::~DcDynPumpMotor()
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
void DcDynPumpMotor::initialize(const DcDynPumpMotorConfigData& configData,
                                const DcDynPumpMotorInputData&  inputData,
                                const std::string&              name)
{
    /// - Reset the initialized flag.
    mInitFlag               = false;

    /// - Allocate and initialize the object name or throw a TsInitializationException exception if empty.
    TS_GENERIC_NAME_ERREX("DcDynPumpMotor")

    /// - Validate initialization data.
    validate(configData, inputData);

    /// - Initialize with config data.
    mWindingResistance      = configData.mWindingResistance;
    mTorqueConstant         = configData.mTorqueConstant;
    mFrictionConstant       = configData.mFrictionConstant;
    mFrictionMinSpeed       = configData.mFrictionMinSpeed;
    mInertia                = configData.mInertia;

    /// - Initialize with input data.
    mVoltage                = inputData.mVoltage;
    mCurrentLimit           = inputData.mCurrentLimit;
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
    mResistance             = 0.0;
    mWasteHeat              = 0.0;
    mEfficiency             = 0.0;
    mStallTorque            = 0.0;
    mDriveTorque            = 0.0;
    mFrictionTorque         = 0.0;
    mTotalExternalLoad      = 0.0;
    mDegradedResistance     = 0.0;
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
void DcDynPumpMotor::validate(const DcDynPumpMotorConfigData& configData,
                              const DcDynPumpMotorInputData&  inputData) const
{
    /// - Throw an exception if winding resistance < DBL_EPSILON.
    if (configData.mWindingResistance < DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Winding resistance cannot be < DBL_EPSILON.", mName);
    }

    /// - Throw an exception if torque constant < DBL_EPSILON.
    if (configData.mTorqueConstant < DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Torque constant cannot be < DBL_EPSILON.", mName);
    }

    /// - Throw an exception if friction constant is < 0.0.
    if (configData.mFrictionConstant < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Friction constant cannot be < 0.0", mName);
    }

    /// - Throw an exception if friction minimum speed is < 0.0.
    if (configData.mFrictionMinSpeed < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Friction minimum speed cannot be < 0.0.", mName);
    }

    /// - Throw an exception if inertia < DBL_EPSILON.
    if (configData.mInertia < DBL_EPSILON) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Configuration Data", TsInitializationException,
                "Inertia cannot be < DBL_EPSILON.", mName);
    }

    /// - Throw an exception if initial voltage < 0.
    if (inputData.mVoltage < 0.0) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "TSM", "Invalid Input Data", TsInitializationException,
                "Initial voltage cannot be < 0.", mName);
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
void DcDynPumpMotor::step(const double dt)
{
    computeElectricalOutputs();
    generateMotorTorque();
    gatherExternalLoads();
    computeMotorSpeed(dt);
    computeWasteHeat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Maximum current & power occurs at stall, and is reduced as speed increases by the
///           back-EMF effect.  The degradation malfunction effectively increases the electrical
///           resistance, reducing current, power and drive torque, caused by brush contactor
///           erosion or broken rotor windings, etc.
///
///           Current and resistive load on the power supply are relative to the input control
///           voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DcDynPumpMotor::computeElectricalOutputs()
{
    mDegradedResistance = std::max(DBL_EPSILON, mWindingResistance);
    if (mMalfDegradeFlag) {
        mDegradedResistance /= std::max(DBL_EPSILON, (1.0 - mMalfDegradeValue));
    }

    /// - Torque constant in (N*m/amp) is equivalent to back-emf constant in (v*s/r), and we convert
    ///   the motor speed to r/s units for the back-emf contribution.
    /// - Current is limited to the controller-supplied value, which allows the controller to limit
    ///   in-rush current of the motor during startup, etc.  This also limits the torque the motor
    ///   produces.
    mCurrent = (mVoltage - mTorqueConstant * mMotorSpeed / UnitConversion::SEC_PER_MIN_PER_2PI)
             / mDegradedResistance;
    mCurrent = MsMath::limitRange(0.0, mCurrent, mCurrentLimit);

    mPower       = mCurrent * mVoltage;
    mResistance  = mVoltage / std::max(mCurrent, DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Stall torque is the maximum torque the motor can produce with the given input voltage,
///           which occurs at speed zero (stall).  The drive torque actually produced by the motor
///           is directly proportional to current, which decreases from stall current linearly with
///           speed due to the back-EMF effect, and is further reduced by the degrade malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DcDynPumpMotor::generateMotorTorque()
{
    mStallTorque = mTorqueConstant * mVoltage / mDegradedResistance;
    mDriveTorque = mTorqueConstant * mCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  The motor supports up to 4 external loads, which can come from simbus or setter
///           method.  The jam malfunction applies an extra external torque, calculated as a
///           fraction of stall torque, such that at 100% jam the motor cannot accelerate even if
///           all other external loads are removed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DcDynPumpMotor::gatherExternalLoads()
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
///           motor.  Our motor cannot spin backwards.  All torques are signed relative to the
///           forward rotation of the motor, so torque produced by the motor is positive, while
///           loads and friction are negative.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DcDynPumpMotor::computeMotorSpeed(const double dt)
{
    /// - Dynamic friction uses a minimum speed, causing it to become constant at actual motor
    ///   speeds below that minimum, to avoid the motor taking forever to stop.
    mFrictionTorque = -mFrictionConstant * std::max(mMotorSpeed, mFrictionMinSpeed);

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
/// @details  All power produced by the motor that is not absorbed by the external load is assumed
///           to become waste heat.  Further losses in the external load model are not included in
///           our waste heat or efficiency.
////////////////////////////////////////////////////////////////////////////////////////////////////
void DcDynPumpMotor::computeWasteHeat()
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
