/**
@file
@brief    GUNNS Electrical Resistive User Load Spotter implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsElectUserLoad.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsElectResistiveUserLoad.hh"
#include "core/GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--)  Instance name for self-identification in messages.
/// @param[in] underVoltageLimit (v)   Low voltage limit for operation.
/// @param[in] resistanceNormal  (ohm) Resistance for normal mode.
/// @param[in] resistanceStandby (ohm) Resistance for standby mode.
/// @param[in] fuseCurrentLimit  (amp) Current above which the fuse blows.
/// @param[in] dutyCycleFraction (--)  Fraction of time the load spends in the on state in its duty cycle.
/// @param[in] dutyCyclePeriod   (s)   Duration of each duty cycle.
///
/// @details  Default constructs this GUNNS Electrical Resistive User Load Spotter configuration
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectResistiveUserLoadConfigData::GunnsElectResistiveUserLoadConfigData(
        const std::string& name,
        const double       underVoltageLimit,
        const double       resistanceNormal,
        const double       resistanceStandby,
        const double       fuseCurrentLimit,
        const double       dutyCycleFraction,
        const double       dutyCyclePeriod)
    :
    GunnsElectUserLoadConfigData(name, underVoltageLimit, fuseCurrentLimit, dutyCycleFraction, dutyCyclePeriod),
    mResistanceNormal(resistanceNormal),
    mResistanceStandby(resistanceStandby)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical Resistive User Load Spotter configuration
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectResistiveUserLoadConfigData::~GunnsElectResistiveUserLoadConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  initialMode     (--)  Initial mode.
/// @param[in]  initialVoltage  (v)   Initial input voltage.
/// @param[in]  dutyCycleTimer  (s)   Initial duty cycle elapsed time.
///
/// @details  Default constructs this GUNNS Electrical Resistive User Load Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectResistiveUserLoadInputData::GunnsElectResistiveUserLoadInputData(
        const int    initialMode,
        const double initialVoltage,
        const double dutyCycleTimer)
    :
    GunnsElectUserLoadInputData(initialMode, initialVoltage, dutyCycleTimer)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical Resistive User Load Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectResistiveUserLoadInputData::~GunnsElectResistiveUserLoadInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @paaram[in]  configData  (--)  Optional pointer to the config data.
/// @paaram[in]  inputData   (--)  Optional pointer to the input data.
///
/// @details  Default constructs this GUNNS Electrical Resistive User Load Spotter.  The config &
///           input data arguments are optional.  They can either be set here or left defaulted to
///           NULL here and set by a later call to the initialize method.  A GunnShow network, for
///           example, will leave these NULL here and set them with an initialize call later.  Use
///           this constructor to set them from a simple container class for which you desire no
///           init jobs, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectResistiveUserLoad::GunnsElectResistiveUserLoad(
        GunnsElectResistiveUserLoadConfigData* configData,
        GunnsElectResistiveUserLoadInputData*  inputData)
    :
    GunnsElectUserLoad(),
    mConfig(configData),
    mInput(inputData),
    mLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Electrical Resistive User Load Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectResistiveUserLoad::~GunnsElectResistiveUserLoad()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  This validates the supplied config & input data objects.  If they fail validation, an
///           exception is thrown.  If they pass, these pointers are stored in the class for later
///           use by the initLoad method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectResistiveUserLoad::initialize(GunnsNetworkSpotterConfigData* configData,
                                             GunnsNetworkSpotterInputData*  inputData)
{
    /// - Validate & store config & input data pointers.
    mConfig = validateConfig(configData);
    mInput  = validateInput(inputData);

    /// - Initialize the base class.
    GunnsElectUserLoad::initialize(configData, inputData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Initializes this spotter and the contained resistive User Load with config & input
///           data.  The config & input data pointers should have been set by now, either in the
///           constructor or the initialize method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectResistiveUserLoad::initLoad()
{
    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate config & input data.
    initialize(mConfig, mInput);

    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(mConfig, mInput);
    mSupplyVoltage = mInput->mInitialVoltage;

    /// - Reset the init flag again, since base class will have set it.
    mInitFlag = false;

    /// - Initialize the User Load.
    ResistiveLoadConfigData loadConfig(mConfig->mName + ".mLoad",
                                       RESISTIVE_LOAD,
                                       mConfig->mUnderVoltageLimit,
                                       mConfig->mResistanceNormal,
                                       mConfig->mResistanceStandby,
                                       mConfig->mFuseCurrentLimit);
    ResistiveLoadInputData loadInput(false, 0.0, mInput->mInitialMode, mInput->mInitialVoltage);
    std::vector<UserLoadBase*> dummy;
    mLoad.initialize(loadConfig, loadInput, dummy, 0, 0);

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsElectResistiveUserLoadConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectResistiveUserLoadConfigData* GunnsElectResistiveUserLoad::validateConfig(GunnsNetworkSpotterConfigData* config)
{
    GunnsElectResistiveUserLoadConfigData* result = dynamic_cast<GunnsElectResistiveUserLoadConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsElectResistiveUserLoadInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectResistiveUserLoadInputData* GunnsElectResistiveUserLoad::validateInput(GunnsNetworkSpotterInputData* input)
{
    GunnsElectResistiveUserLoadInputData* result = dynamic_cast<GunnsElectResistiveUserLoadInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}
