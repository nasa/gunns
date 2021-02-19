/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Liquid Centrifugal Pump Model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((aspects/fluid/potential/GunnsPumpCavitation.o)
    (aspects/fluid/potential/GunnsGasFan.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Install) (2012-07))
***************************************************************************************************/

#include "GunnsLiquidCentrifugalPump.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                    (--)      Name of object.
/// @param[in,out] nodes                   (--)      Pointer to nodes.
/// @param[in]     maxConductivity         (m2)      Max conductivity.
/// @param[in]     expansionScaleFactor    (--)      Scale factor for isentropic gas cooling.
/// @param[in]     referenceDensity        (kg/m3)   Reference fluid density for power curve.
/// @param[in]     referenceSpeed          (rev/min) Reference impeller speed for power curve.
/// @param[in]     referenceCoeff0         (kPa)     Reference performance curve 0th-order coefficient.
/// @param[in]     referenceCoeff1         (kPa)     Reference performance curve 1th-order coefficient.
/// @param[in]     referenceCoeff2         (kPa)     Reference performance curve 2th-order coefficient.
/// @param[in]     referenceCoeff3         (kPa)     Reference performance curve 3th-order coefficient.
/// @param[in]     referenceCoeff4         (kPa)     Reference performance curve 4th-order coefficient.
/// @param[in]     referenceCoeff5         (kPa)     Reference performance curve 5th-order coefficient.
/// @param[in]     bestEfficiency          (--)      (0-1) Efficiency at best efficiency point at reference.
/// @param[in]     referenceQBep           (m3/s)    Volume flow rate at best efficiency point at reference.
/// @param[in]     filterGain              (--)      (0-1) Flow filter gain for system curve estimate.
/// @param[in]     driveRatio              (--)      Gear ratio of motor to impeller speed.
/// @param[in]     thermalLength           (m)       Impeller length for thermal convection.
/// @param[in]     thermalDiameter         (m)       Impeller inner diameter for thermal convection.
/// @param[in]     surfaceRoughness        (m)       Impeller wall surface roughness for convection.
/// @param[in]     liquidType              (--)      Type of the operating liquid.
/// @param[in]     autoCavitationDuration  (s)       Duration of auto-cavitation effect.
/// @param[in]     autoCavitationAmplitude (--)      Amplitude (0-1) of auto-cavitation pressure noise.
///
/// @details  Default constructs this GUNNS Liquid Centrifugal Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidCentrifugalPumpConfigData::GunnsLiquidCentrifugalPumpConfigData(
                                             const std::string& name,
                                             GunnsNodeList*     nodes,
                                             const double       maxConductivity,
                                             const double       expansionScaleFactor,
                                             const double       referenceDensity,
                                             const double       referenceSpeed,
                                             const double       referenceCoeff0,
                                             const double       referenceCoeff1,
                                             const double       referenceCoeff2,
                                             const double       referenceCoeff3,
                                             const double       referenceCoeff4,
                                             const double       referenceCoeff5,
                                             const double       bestEfficiency,
                                             const double       referenceQBep,
                                             const double       filterGain,
                                             const double       driveRatio,
                                             const double       thermalLength,
                                             const double       thermalDiameter,
                                             const double       surfaceRoughness,
                                             const bool         checkValveActive,
                                             const FluidProperties::FluidType liquidType,
                                             const double       autoCavitationDuration,
                                             const double       autoCavitationAmplitude)
    :
    GunnsGasFanConfigData(name, nodes, maxConductivity, expansionScaleFactor, referenceDensity,
                          referenceSpeed, referenceCoeff0, referenceCoeff1, referenceCoeff2,
                          referenceCoeff3, referenceCoeff4, referenceCoeff5, bestEfficiency,
                          referenceQBep, filterGain, driveRatio, thermalLength,
                          thermalDiameter, surfaceRoughness, checkValveActive),
    mLiquidType             (liquidType),
    mAutoCavitationDuration (autoCavitationDuration),
    mAutoCavitationAmplitude(autoCavitationAmplitude)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Liquid Centrifugal Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidCentrifugalPumpConfigData::GunnsLiquidCentrifugalPumpConfigData(
                                             const GunnsLiquidCentrifugalPumpConfigData& that)
    :
    GunnsGasFanConfigData   (that),
    mLiquidType             (that.mLiquidType),
    mAutoCavitationDuration (that.mAutoCavitationDuration),
    mAutoCavitationAmplitude(that.mAutoCavitationAmplitude)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Liquid Centrifugal Pump link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidCentrifugalPumpConfigData::~GunnsLiquidCentrifugalPumpConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag     (--)      Blockage malfunction flag.
/// @param[in] malfBlockageValue    (--)      Blockage malfunction fractional value (0-1).
/// @param[in] sourcePressure       (kPa)     Initial pressure rise of the link.
/// @param[in] motorSpeed           (rev/min) Initial speed of the motor.
/// @param[in] wallTemperature      (K)       Initial impeller wall temperature.
/// @param[in] enableAutoCavitation (--)      Enables the automatic cavitation model.
///
/// @details  Default constructs this GUNNS Liquid Centrifugal Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidCentrifugalPumpInputData::GunnsLiquidCentrifugalPumpInputData(
                                             const bool   malfBlockageFlag,
                                             const double malfBlockageValue,
                                             const double sourcePressure,
                                             const double motorSpeed,
                                             const double wallTemperature,
                                             const bool   enableAutoCavitation)
    :
    GunnsGasFanInputData(malfBlockageFlag, malfBlockageValue, sourcePressure, motorSpeed, wallTemperature),
    mEnableAutoCavitation(enableAutoCavitation)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Liquid Centrifugal Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidCentrifugalPumpInputData::GunnsLiquidCentrifugalPumpInputData(
                                             const GunnsLiquidCentrifugalPumpInputData& that)
    :
    GunnsGasFanInputData (that),
    mEnableAutoCavitation(that.mEnableAutoCavitation)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Liquid Centrifugal Pump link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidCentrifugalPumpInputData::~GunnsLiquidCentrifugalPumpInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Liquid Centrifugal Pump link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidCentrifugalPump::GunnsLiquidCentrifugalPump()
    :
    mCavitation(),
    mLiquidType(FluidProperties::NO_FLUID)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Liquid Centrifugal Pump link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsLiquidCentrifugalPump::~GunnsLiquidCentrifugalPump()
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
/// @details  Initializes this GUNNS Liquid Centrifugal Pump link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLiquidCentrifugalPump::initialize(const GunnsLiquidCentrifugalPumpConfigData& configData,
                                            const GunnsLiquidCentrifugalPumpInputData&  inputData,
                                            std::vector<GunnsBasicLink*>&               links,
                                            const int                                   port0,
                                            const int                                   port1)
{
    /// - First initialize & validate parent.
    GunnsGasFan::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag   = false;

    /// - Validates the link config data.
    validate(configData);

    /// - Initialize from configuration data.
    mLiquidType = configData.mLiquidType;
    mCavitation.initialize(configData.mAutoCavitationDuration,
                           configData.mAutoCavitationAmplitude,
                           inputData.mEnableAutoCavitation);

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag   = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Liquid Centrifugal Pump Model link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLiquidCentrifugalPump::validate(const GunnsLiquidCentrifugalPumpConfigData& configData) const
{
    /// - Throw an exception if the operating liquid type is not a liquid.
    if (FluidProperties::LIQUID !=
            mNodes[0]->getFluidConfig()->mProperties->getProperties(configData.mLiquidType)->getPhase()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Liquid type is not a liquid.");
    }

    /// - Throw an exception if the operating liquid is not found in the network.
    try {
        mNodes[0]->getOutflow()->find(configData.mLiquidType);
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
void GunnsLiquidCentrifugalPump::restartModel()
{
    /// - Reset the base class.
    GunnsGasFan::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Time step.
///
/// @returns  void
///
/// @details  Updates this GUNNS Liquid Centrifugal Pump link model source pressure.  Identical to a
///           GunnsGasFan, but then applies cavitation effects to the source pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsLiquidCentrifugalPump::updateState(const double dt)
{
    GunnsGasFan::updateState(dt);
    mCavitation.update(mSourcePressure, dt, mNodes[0], mLiquidType);
}
