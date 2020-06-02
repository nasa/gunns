/**
@file
@brief    GUNNS Thermoelectric Device Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/GunnsBasicConductor.o)
     (aspects/thermal/GunnsThermoelectricEffect.o)
    )
*/

#include "GunnsThermoelectricDevice.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                        (--)      Instance name for output messages.
/// @param[in] nodes                       (--)      Network nodes array.
/// @param[in] numThermocouples            (--)      Number of thermocouples in series.
/// @param[in] geometryFactor              (m)       Cross-sectional area over length of each thermocouple.
/// @param[in] electricalResistivityCoeff0 (ohm*m)   0th-order coefficient of electrical resistivity of material line function of temperature.
/// @param[in] electricalResistivityCoeff1 (ohm*m/K) 1st-order coefficient of electrical resistivity of material line function of temperature.
/// @param[in] seebeckCoeff0               (v/K)     0th-order coefficient of Seebeck coefficient of material polynomial of temperature.
/// @param[in] seebeckCoeff1               (v/K2)    1st-order coefficient of Seebeck coefficient of material polynomial of temperature.
/// @param[in] seebeckCoeff2               (v/K3)    2nd-order coefficient of Seebeck coefficient of material polynomial of temperature.
/// @param[in] thermalConductivityCoeff0   (W/m/K)   0th-order coefficient of thermal conductivity of material polynomial of temperature.
/// @param[in] thermalConductivityCoeff1   (W/m/K2)  1st-order coefficient of thermal conductivity of material polynomial of temperature.
/// @param[in] thermalConductivityCoeff2   (W/m/K3)  2nd-order coefficient of thermal conductivity of material polynomial of temperature.
/// @param[in] endPlateThermalConductance  (W/K)     Thermal conductance of one end plate.
/// @param[in] minTemperature              (K)       Minimum temperature for evaluating material properties functions.
/// @param[in] maxTemperature              (K)       Maximum temperature for evaluating material properties functions.
///
/// @details  Default constructs this Thermoelectric Device link Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricDeviceConfigData::GunnsThermoelectricDeviceConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       numThermocouples,
        const double       geometryFactor,
        const double       electricalResistivityCoeff0,
        const double       electricalResistivityCoeff1,
        const double       seebeckCoeff0,
        const double       seebeckCoeff1,
        const double       seebeckCoeff2,
        const double       thermalConductivityCoeff0,
        const double       thermalConductivityCoeff1,
        const double       thermalConductivityCoeff2,
        const double       endPlateThermalConductance,
        const double       minTemperature,
        const double       maxTemperature)
    :
    GunnsBasicConductorConfigData(name, nodes, 0.0),
    mThermoelectricEffect(numThermocouples,
                          geometryFactor,
                          electricalResistivityCoeff0,
                          electricalResistivityCoeff1,
                          seebeckCoeff0,
                          seebeckCoeff1,
                          seebeckCoeff2,
                          thermalConductivityCoeff0,
                          thermalConductivityCoeff1,
                          thermalConductivityCoeff2,
                          endPlateThermalConductance,
                          minTemperature,
                          maxTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this Thermoelectric Device link Config Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricDeviceConfigData::~GunnsThermoelectricDeviceConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag               (--)  Blockage malfunction flag.
/// @param[in] malfBlockageValue              (--)  Blockage malfunction fractional value (0-1).
/// @param[in] malfThermoelectricEffectsFlag  (--)  Initial thermoelectric effects scalar malfunction flag.
/// @param[in] malfThermoelectricEffectsValue (--)  Initial thermoelectric effects scalar malfunction value.
///
/// @details  Default constructs this Thermoelectric Device link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricDeviceInputData::GunnsThermoelectricDeviceInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const bool   malfThermoelectricEffectsFlag,
        const double malfThermoelectricEffectsScalar)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
    mThermoelectricEffect(malfThermoelectricEffectsFlag, malfThermoelectricEffectsScalar)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Thermoelectric Device link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricDeviceInputData::~GunnsThermoelectricDeviceInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Thermoelectric Device link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricDevice::GunnsThermoelectricDevice()
    :
    GunnsBasicConductor(),
    mThermoelectricEffect()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this Thermoelectric Device link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermoelectricDevice::~GunnsThermoelectricDevice()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data.
/// @param[in,out] inputData    (--) Reference to Link Input Data.
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector.
/// @param[in]     port0        (--) Port 0 Mapping.
/// @param[in]     port1        (--) Port 1 Mapping.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Thermoelectric Device link with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricDevice::initialize(const GunnsThermoelectricDeviceConfigData& configData,
                                                 GunnsThermoelectricDeviceInputData&  inputData,
                                           std::vector<GunnsBasicLink*>&              networkLinks,
                                           const int                                  port0,
                                           const int                                  port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    inputData.mThermoelectricEffect.mTemperatureHot  = mNodes[0]->getPotential();
    inputData.mThermoelectricEffect.mTemperatureCold = mNodes[1]->getPotential();
    mThermoelectricEffect.initialize(configData.mThermoelectricEffect,
                                     inputData.mThermoelectricEffect,
                                     mName + ".mThermoelectricEffect");

    /// - Set init flag on successful validation.
    mInitFlag = mThermoelectricEffect.isInitialized();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricDevice::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();

    /// - Reset class attributes.
    mThermoelectricEffect.restart();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates this Thermoelectric Device link during the time step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricDevice::step(const double dt)
{
    mThermoelectricEffect.setTemperatureHot (mPotentialVector[0]);
    mThermoelectricEffect.setTemperatureCold(mPotentialVector[1]);
    mThermoelectricEffect.update();
    mDefaultConductivity = mThermoelectricEffect.getThermalConductance();

    GunnsBasicConductor::step(dt);
    buildPotential();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Applies the heat flux outputs of the embedded thermoeletric effect as heat sources on
///           the nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricDevice::buildPotential()
{
    mSourceVector[0] = mThermoelectricEffect.getHeatFluxHot();
    mSourceVector[1] = mThermoelectricEffect.getHeatFluxCold();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The mPower term is the total heat flux moved from "hot" to "cold" ports including
///           thermal conduction and the Peltier effects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricDevice::computePower()
{
    mPower = mFlux + mSourceVector[1] - mSourceVector[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fromPort (--) Not used.
/// @param[in] toPort   (--) Not used.
///
/// @details This updates the flux values in the attached nodes, so the nodes can check that KCL is
///          satisfied.  There are 3 separate fluxes to be transported:
///          - the normal thermal conduction mFlux, positive from port 0 to 1.  We call the base
///            class implementation for this.
///          - heat sources from thermoelectric and Joule heating effects added to the port 0 node.
///          - heat sources from thermoelectric and Joule heating effects added to the port 1 node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermoelectricDevice::transportFlux(const int fromPort __attribute__((unused)),
                                              const int toPort   __attribute__((unused)))
{
    GunnsBasicLink::transportFlux(0, 1);
    if (mSourceVector[0] > 0.0) {
        mNodes[0]->collectInflux(mSourceVector[0]);
    } else {
        mNodes[0]->collectOutflux(-mSourceVector[0]);
    }
    if (mSourceVector[1] > 0.0) {
        mNodes[1]->collectInflux(mSourceVector[1]);
    } else {
        mNodes[1]->collectOutflux(-mSourceVector[1]);
    }
}
