/**
@file
@brief     GUNNS Drive Shaft Network Spotter implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsNetworkSpotter.o)
   (core/GunnsBasicConductor.o)
   (aspects/fluid/potential/GunnsGasFan.o)
   (aspects/fluid/conductor/GunnsGasTurbine.o)
   (math/UnitConversion.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o))
**************************************************************************************************/

#include "GunnsDriveShaftSpotter.hh"
#include "aspects/fluid/potential/GunnsGasFan.hh"
#include "aspects/fluid/conductor/GunnsGasTurbine.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name            (--)  Instance name for self-identification in messages.
/// @param[in]  preStepCounter  (--)  Initial counter to support unit tests.
///
/// @details  Default constructs this GUNNS Drive Shaft Network Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDriveShaftSpotterConfigData::GunnsDriveShaftSpotterConfigData(const std::string& name,
                                                                   const double       frictionConstant,
                                                                   const double       frictionMinSpeed,
                                                                   const double       inertia)
    :
    GunnsNetworkSpotterConfigData(name),
    mFrictionConstant            (frictionConstant),
    mFrictionMinSpeed            (frictionMinSpeed),
    mInertia                     (inertia)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Drive Shaft Network Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDriveShaftSpotterConfigData::~GunnsDriveShaftSpotterConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  postStepCounter  (--)  Initial counter to support unit tests.
///
/// @details  Default constructs this GUNNS Drive Shaft Network Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDriveShaftSpotterInputData::GunnsDriveShaftSpotterInputData(const double motorSpeed)
    :
    mMotorSpeed            (motorSpeed),
    mMalfJamFlag           (false),
    mMalfJamValue          (0.0),
    mMalfSpeedOverrideFlag (false),
    mMalfSpeedOverrideValue(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Drive Shaft Network Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDriveShaftSpotterInputData::~GunnsDriveShaftSpotterInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default constructs this GUNNS Drive Shaft Network Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDriveShaftSpotter::GunnsDriveShaftSpotter()
    :
    GunnsNetworkSpotter(),
    mMalfJamFlag(false),
    mMalfJamValue(0.0),
    mMalfSpeedOverrideFlag(false),
    mMalfSpeedOverrideValue(0.0),
    mFrictionConstant(0.0),
    mFrictionMinSpeed(0.0),
    mInertia(0.0),
    mMotorSpeed(0.0),
    mInitFlag(false),
    mTurbRef(),
    mFanRef(),
    mFrictionTorque(0.0),
    mTotalExternalLoad(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Drive Shaft Network Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDriveShaftSpotter::~GunnsDriveShaftSpotter()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Drive Shaft Network Spotter with its configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDriveShaftSpotter::initialize(const GunnsNetworkSpotterConfigData* configData,
                                       const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & type-cast config & input data.
    const GunnsDriveShaftSpotterConfigData* config = validateConfig(configData);
    const GunnsDriveShaftSpotterInputData*  input  = validateInput(inputData);

    /// - Initialize with validated config & input data.
    mFrictionConstant = config->mFrictionConstant;
    mFrictionMinSpeed = config->mFrictionMinSpeed;
    mInertia          = config->mInertia;
    mMotorSpeed       = input->mMotorSpeed;

    mFrictionTorque    = 0.0;

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsDriveShaftSpotterConfigData  (--)  Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsDriveShaftSpotterConfigData* GunnsDriveShaftSpotter::validateConfig(
                                                        const GunnsNetworkSpotterConfigData* config)
{
    const GunnsDriveShaftSpotterConfigData* result =
                                      dynamic_cast<const GunnsDriveShaftSpotterConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }

    if (result->mFrictionConstant < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Friction constant less than 0.");
    }

    if (result->mFrictionMinSpeed < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Friction min speed less than 0.");
    }

    if (result->mInertia <= DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Inertia less than 0.");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsDriveShaftSpotterInputData  (--)  Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsDriveShaftSpotterInputData* GunnsDriveShaftSpotter::validateInput(
                                                          const GunnsNetworkSpotterInputData* input)
{
    const GunnsDriveShaftSpotterInputData* result =
                                        dynamic_cast<const GunnsDriveShaftSpotterInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }

    if (result->mMalfJamValue < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Jam malfunction value less than 0.");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step.
///
/// @details Calculates the change in motor speed based on last past total external torque. Torque
///          due to dynamic friction is also accounted for. A specific motor speed can be forced
///          using the override malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDriveShaftSpotter::stepPreSolver(const double dt) {
    /// - Dynamic friction uses a minimum speed, causing it to become constant at actual motor
    ///   speeds below that minimum, to avoid the motor taking forever to stop.
    mFrictionTorque = -mFrictionConstant * std::max(mMotorSpeed, mFrictionMinSpeed);

    /// - Torque and inertia are related to angular velocity in r/s, must be converted to rev/min.
    mMotorSpeed += (mTotalExternalLoad + mFrictionTorque) * dt
            * UnitConversion::SEC_PER_MIN_PER_2PI / mInertia;
    mMotorSpeed = std::max(mMotorSpeed, DBL_EPSILON);

    /// - The speed override malfunction completely overrides all motor dynamics and forces a
    ///   desired speed.
    if (mMalfSpeedOverrideFlag) {
        mMotorSpeed = mMalfSpeedOverrideValue;
    }

    /// - Set motor speed in all models
    for (unsigned int i = 0; i < mTurbRef.size(); i++) {
        mTurbRef[i]->setMotorSpeed(mMotorSpeed);
    }
    for (unsigned int i = 0; i < mFanRef.size(); i++) {
        mFanRef[i]->setMotorSpeed(mMotorSpeed);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (unused).
///
/// @details  Sums the external loads of all fans and turbines. The jam malfunction applies an
///           additional torque opposing the net torque. If the drive shaft is 100% jammed, the net
///           torque will be zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDriveShaftSpotter::stepPostSolver(const double dt __attribute__((unused))) {
    mTotalExternalLoad = 0.0;
    for (unsigned int i = 0; i < mTurbRef.size(); i++) {
        mTotalExternalLoad += mTurbRef[i]->getImpellerTorque();
    }
    for (unsigned int i = 0; i < mFanRef.size(); i++) {
        mTotalExternalLoad += mFanRef[i]->getImpellerTorque();
    }

    if (mMalfJamFlag) {
        mTotalExternalLoad -= mMalfJamValue * mTotalExternalLoad;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  object  (--)  Object to be referenced by drive shaft.
///
/// @throws   TsInitializationException
///
/// @details  Tells the drive shaft what fans and turbines it is attached to.
///
/// @note     This method must be called from the Trick input file, for every object attached to the
///           drive shaft.
////////////////////////////////////////////////////////////////////////////////////////////////////

void GunnsDriveShaftSpotter::addImpeller(GunnsFluidConductor* object){
    /// - Dynamically check whether the given object is a GunnsGasFan or GunnsGasTurbine.
    if (GunnsGasTurbine* turbinePointer = dynamic_cast<GunnsGasTurbine*>(object)) {
        mTurbRef.push_back(turbinePointer);
    } else if (GunnsGasFan* fanPointer = dynamic_cast<GunnsGasFan*>(object)) {
        mFanRef.push_back(fanPointer);
    } else {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Drive shaft must reference GunnsGasFan or GunnsGasTurbine objects only.")
    }
}
