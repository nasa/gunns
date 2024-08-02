/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))
***************************************************************************************************/

#include "GunnsFluidSensor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                  (--)   Name of object.
/// @param[in]  nodes                 (--)   Pointer to nodes.
/// @param[in]  maxConductivity       (m2)   Max conductivity.
/// @param[in]  expansionScaleFactor  (--)   Scale factor for isentropic gas cooling.
///
/// @details  Default constructs this GUNNS Fluid Sensor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSensorConfigData::GunnsFluidSensorConfigData(const std::string& name,
                                                       GunnsNodeList*     nodes,
                                                       const double       maxConductivity,
                                                       const double       expansionScaleFactor)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Sensor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSensorConfigData::GunnsFluidSensorConfigData(const GunnsFluidSensorConfigData& that)
    :
    GunnsFluidConductorConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sensor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSensorConfigData::~GunnsFluidSensorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   (--)      Blockage malfunction flag.
/// @param[in]  malfBlockageValue  (--)      Blockage malfunction fractional value (0-1).
///
/// @details  Default constructs this GUNNS Fluid Sensor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSensorInputData::GunnsFluidSensorInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Sensor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSensorInputData::GunnsFluidSensorInputData(const GunnsFluidSensorInputData& that)
    :
    GunnsFluidConductorInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sensor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSensorInputData::~GunnsFluidSensorInputData()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Sensor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSensor::GunnsFluidSensor()
    :
    GunnsFluidConductor(),
    mTemperature(0.0),
    mPressure(0.0),
    mDeltaPressure(0.0),
    mMassFlowRate(0.0),
    mVolumetricFlowRate(0.0),
    mPartialPressure()
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sensor link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSensor::~GunnsFluidSensor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  Nominal inlet port map index.
/// @param[in]      port1        (--)  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Sensor link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSensor::initialize(const GunnsFluidSensorConfigData& configData,
                                  const GunnsFluidSensorInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     links,
                                  const int                         port0,
                                  const int                         port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Initialize all partial pressures to zero.
    for (int i = 0; i < FluidProperties::NO_FLUID; ++i) {
        mPartialPressure[i] = 0.0;
    }

    /// - Initialize sensor truth values.
    updateSensedValues();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSensor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  Calls the base class implementation for normal fluid conductor transport, then calls
///           the final sensed output values to be updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSensor::transportFlows(const double dt)
{
    GunnsFluidConductor::transportFlows(dt);
    updateSensedValues();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Updates this GUNNS Fluid Sensor link model temperature, pressure, delta pressure and
///            mass flow rate sensor truth values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSensor::updateSensedValues()
{
    /// - Temperature is from port 0 Node fluid.
    mTemperature         = mNodes[0]->getContent()->getTemperature();

    /// - Pressure is from port 0 Node fluid.
    mPressure            = mNodes[0]->getContent()->getPressure();

    /// - Delta pressure is port 0 Node fluid pressure - port 1 Node fluid pressure.
    mDeltaPressure       = mPressure - mNodes[1]->getContent()->getPressure();

    /// - Mass flow rate is link mass flow rate.
    mMassFlowRate        = mFlowRate;

    /// - Volumetric flow rate is link volumetric flow rate.
    mVolumetricFlowRate  = mVolFlowRate;

    /// - Partial pressures are port 0 Node fluid partial pressures. Partial pressures for absent
    ///   constituents are not updated from initial value of 0.0.
    for (int i = 0; i < mNodes[0]->getContent()->getNConstituents(); ++i) {
        const FluidProperties::FluidType type = mNodes[0]->getContent()->getType(i);
        mPartialPressure[type]                = mNodes[0]->getContent()->getPartialPressure(type);
    }
}
