/**
@file
@brief    GUNNS Electrical User Load Base Spotter implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsNetworkSpotter.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsElectUserLoad.hh"
#include "core/GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--)  Instance name for self-identification in messages.
/// @param[in] underVoltageLimit (V)   Low voltage limit for operation.
/// @param[in] fuseCurrentLimit  (amp) Current above which the fuse blows.
/// @param[in] dutyCycleFraction (--)  Fraction of time the load spends in the on state in its duty cycle.
/// @param[in] dutyCyclePeriod   (s)   Duration of each duty cycle.
///
/// @details  Default constructs this GUNNS Electrical User Load Base Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadConfigData::GunnsElectUserLoadConfigData(const std::string& name,
                                                           const double       underVoltageLimit,
                                                           const double       fuseCurrentLimit,
                                                           const double       dutyCycleFraction,
                                                           const double       dutyCyclePeriod)
    :
    GunnsNetworkSpotterConfigData(name),
    mUnderVoltageLimit(underVoltageLimit),
    mFuseCurrentLimit(fuseCurrentLimit),
    mDutyCycleFraction(dutyCycleFraction),
    mDutyCyclePeriod(dutyCyclePeriod)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical User Load Base Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadConfigData::~GunnsElectUserLoadConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  initialMode      (--)  Initial mode.
/// @param[in]  initialVoltage   (V)   Initial input voltage.
/// @param[in]  dutyCycleTimer   (s)   Initial duty cycle elapsed time.
///
/// @details  Default constructs this GUNNS Electrical User Load Base Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadInputData::GunnsElectUserLoadInputData(const int    initialMode,
                                                         const double initialVoltage,
                                                         const double dutyCycleTimer)
    :
    GunnsNetworkSpotterInputData(),
    mInitialMode(initialMode),
    mInitialVoltage(initialVoltage),
    mDutyCycleTimer(dutyCycleTimer)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical User Load Base Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadInputData::~GunnsElectUserLoadInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Default constructs this GUNNS Electrical User Load Base Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoad::GunnsElectUserLoad()
    :
    GunnsNetworkSpotter(),
    mDutyCycleFraction(0.0),
    mDutyCyclePeriod(0.0),
    mDutyCycleTimer(0.0),
    mSupplyVoltage(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical User Load Base Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoad::~GunnsElectUserLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  This validates the supplied config & input data objects.  If they fail validation, an
///           exception is thrown.  If they pass, the object state is initialized from the config
///           and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoad::initialize(const GunnsNetworkSpotterConfigData* configData,
                                    const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Validate & store config & input data pointers.
    const GunnsElectUserLoadConfigData* config = validateConfig(configData);
    const GunnsElectUserLoadInputData*  input  = validateInput(inputData);

    mDutyCycleFraction = config->mDutyCycleFraction;
    mDutyCyclePeriod   = config->mDutyCyclePeriod;
    mDutyCycleTimer    = input->mDutyCycleTimer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step.
///
/// @details  This updates the optional duty cycle function.  This function is enabled by setting
///           the period > 0.  This function should be called once per network update.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoad::stepDutyCycle(const double dt)
{
    if (mDutyCyclePeriod > 0.0) {
        mDutyCycleTimer += dt;
        if (mDutyCycleTimer > mDutyCyclePeriod) {
            mDutyCycleTimer = 0.0;
        }
        if (mDutyCycleTimer / mDutyCyclePeriod > mDutyCycleFraction) {
            getLoad()->setLoadOperMode(LoadOFF);
        } else {
            getLoad()->setLoadOperMode(LoadON);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step.
///
/// @details  Calls the stepDutyCycle function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoad::stepPreSolver(const double dt)
{
    stepDutyCycle(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (not used).
///
/// @details  This method does nothing because this spotter has no function after the network
///           solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoad::stepPostSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsElectUserLoadConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsElectUserLoadConfigData* GunnsElectUserLoad::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsElectUserLoadConfigData* result = dynamic_cast<const GunnsElectUserLoadConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }

    /// - Throw if the duty cycle fraction is not in 0-1.
    if (not MsMath::isInRange(0.0, result->mDutyCycleFraction, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "duty cycle fraction not in (0-1).");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsElectUserLoadInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsElectUserLoadInputData* GunnsElectUserLoad::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsElectUserLoadInputData* result = dynamic_cast<const GunnsElectUserLoadInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}
