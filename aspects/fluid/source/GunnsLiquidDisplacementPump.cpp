/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Liquid Displacement Pump link model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((aspects/fluid/potential/GunnsPumpCavitation.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Initial) (2012-07))
**************************************************************************************************/

#include "GunnsLiquidDisplacementPump.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                    (--)   Name of object.
/// @param[in,out] nodes                   (--)   Pointer to network nodes.
/// @param[in]     cycleVolume             (--)   Volume of fluid displaced per cycle.
/// @param[in]     driveRatio              (--)   Gear ratio of motor to impeller speed.
/// @param[in]     thermalLength           (m)    Impeller length for thermal convection.
/// @param[in]     thermalDiameter         (m)    Impeller inner diameter for thermal convection.
/// @param[in]     surfaceRoughness        (m)    Impeller wall surface roughness for convection.
/// @param[in]     liquidType              (--)   Type of the operating liquid.
/// @param[in]     autoCavitationDuration  (s)    Duration of auto-cavitation effect.
/// @param[in]     autoCavitationAmplitude (m3/s) Amplitude of auto-cavitation pressure noise.
///
/// @details  Default constructs this GUNNS Liquid Displacement Pump link model configuration data
///           with arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidDisplacementPumpConfigData::GunnsLiquidDisplacementPumpConfigData(
                                              const std::string& name,
                                              GunnsNodeList*     nodes,
                                              const double       cycleVolume,
                                              const double       driveRatio,
                                              const double       thermalLength,
                                              const double       thermalDiameter,
                                              const double       surfaceRoughness,
                                              const bool         checkValveActive,
                                              const FluidProperties::FluidType liquidType,
                                              const double       autoCavitationDuration,
                                              const double       autoCavitationAmplitude)
    :
    GunnsGasDisplacementPumpConfigData(name, nodes, cycleVolume, driveRatio, thermalLength,
                                       thermalDiameter, surfaceRoughness, checkValveActive),
    mLiquidType             (liquidType),
    mAutoCavitationDuration (autoCavitationDuration),
    mAutoCavitationAmplitude(autoCavitationAmplitude)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Copy constructs this GUNNS Liquid Displacement Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidDisplacementPumpConfigData::GunnsLiquidDisplacementPumpConfigData(
                                                  const GunnsLiquidDisplacementPumpConfigData& that)
    :
    GunnsGasDisplacementPumpConfigData(that),
    mLiquidType             (that.mLiquidType),
    mAutoCavitationDuration (that.mAutoCavitationDuration),
    mAutoCavitationAmplitude(that.mAutoCavitationAmplitude)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Liquid Displacement Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidDisplacementPumpConfigData::~GunnsLiquidDisplacementPumpConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag     (--)      Blockage malfunction flag.
/// @param[in] malfBlockageValue    (--)      Blockage malfunction fractional value (0-1).
/// @param[in] flowDemand           (kg/s)    Demanded mass flow rate of the link.
/// @param[in] motorSpeed           (rev/min) Initial speed of the motor.
/// @param[in] wallTemperature      (K)       Initial impeller wall temperature.
/// @param[in] enableAutoCavitation (--)      Enables the automatic cavitation model.
///
/// @details  Default constructs this GUNNS Liquid Displacement Pump link model input data with
///           arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidDisplacementPumpInputData::GunnsLiquidDisplacementPumpInputData(
                                                                  const bool   malfBlockageFlag,
                                                                  const double malfBlockageValue,
                                                                  const double flowDemand,
                                                                  const double motorSpeed,
                                                                  const double wallTemperature,
                                                                  const bool   enableAutoCavitation)
    :
    GunnsGasDisplacementPumpInputData(malfBlockageFlag, malfBlockageValue, flowDemand,
                                      motorSpeed, wallTemperature),
    mEnableAutoCavitation(enableAutoCavitation)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source to copy from.
///
/// @details  Copy constructs this GUNNS Liquid Displacement Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidDisplacementPumpInputData::GunnsLiquidDisplacementPumpInputData(
                                                   const GunnsLiquidDisplacementPumpInputData& that)
    :
    GunnsGasDisplacementPumpInputData(that),
    mEnableAutoCavitation(that.mEnableAutoCavitation)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Liquid Displacement Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidDisplacementPumpInputData::~GunnsLiquidDisplacementPumpInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Liquid Displacement Pump link model with obviously
///           invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidDisplacementPump::GunnsLiquidDisplacementPump()
    :
    mCavitation(),
    mLiquidType(FluidProperties::NO_FLUID)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Liquid Displacement Pump link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidDisplacementPump::~GunnsLiquidDisplacementPump()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData (--) Configuration data.
/// @param[in]     inputData  (--) Input data.
/// @param[in,out] links      (--) Link vector.
/// @param[in]     port0      (--) Nominal inlet port map index.
/// @param[in]     port1      (--) Nominal outlet port map index.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Liquid Displacement Pump link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLiquidDisplacementPump::initialize(const GunnsLiquidDisplacementPumpConfigData& configData,
                                             const GunnsLiquidDisplacementPumpInputData&  inputData,
                                             std::vector<GunnsBasicLink*>&                links,
                                             const int                                    port0,
                                             const int                                    port1)
{
    /// - First initialize & validate parent.
    GunnsGasDisplacementPump::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validates the link config data.
    validate(configData);

    /// - Initialize from configuration data.
    mLiquidType = configData.mLiquidType;
    mCavitation.initialize(configData.mAutoCavitationDuration,
                           configData.mAutoCavitationAmplitude,
                           inputData.mEnableAutoCavitation);

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Liquid Displacement Pump link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLiquidDisplacementPump::validate(const GunnsLiquidDisplacementPumpConfigData& configData) const
{
    /// - Throw an exception if the operating liquid type is not a liquid.
    if (FluidProperties::LIQUID !=
            mNodes[0]->getFluidConfig()->mProperties->getProperties(configData.mLiquidType)->getPhase()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Liquid type is not a liquid.");
    }

    /// - Throw an exception if the operating liquid is not found in the network.
    try {
        mNodes[0]->getContent()->find(configData.mLiquidType);
    } catch (TsOutOfBoundsException& e) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Operating liquid type not found in network.");
    }

    /// - Throw an exception if auto-cavitation duration is negative.
    if (configData.mAutoCavitationDuration < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Auto-cavitation duration < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLiquidDisplacementPump::restartModel()
{
    /// - Reset the base class.
    GunnsGasDisplacementPump::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Time step.
///
/// @returns  void
///
/// @details  Updates the cavitation object and applies its effects to the volumetric flow rate
///           generated by the displacement pump.  This model supports pump speed in either
///           direction, so we send the correct inlet node to the cavitation object based on flow
///           direction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLiquidDisplacementPump::applyCavitation(const double dt)
{
    if (mVolFlowRate >= 0.0) {
        mCavitation.update(mVolFlowRate, dt, mNodes[0], mLiquidType);
    } else {
        mCavitation.update(mVolFlowRate, dt, mNodes[1], mLiquidType);
    }
}
