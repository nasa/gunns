/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))
**************************************************************************************************/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "GunnsFluidLiquidWaterSensor.hh"
#include "core/GunnsFluidUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Name of object.
/// @param[in] nodes                (--) Pointer to nodes.
/// @param[in] maxConductivity      (m2) Max conductivity.
/// @param[in] expansionScaleFactor (--) Scale factor for isentropic gas cooling.
///
/// @details  Default constructs this GUNNS Fluid Liquid Water Sensor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLiquidWaterSensorConfigData::GunnsFluidLiquidWaterSensorConfigData(
                                                       const std::string& name,
                                                       GunnsNodeList*     nodes,
                                                       const double       maxConductivity,
                                                       const double       expansionScaleFactor)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Liquid Water Sensor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLiquidWaterSensorConfigData::GunnsFluidLiquidWaterSensorConfigData(
        const GunnsFluidLiquidWaterSensorConfigData& that)
    :
    GunnsFluidConductorConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Liquid Water Sensor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLiquidWaterSensorConfigData::~GunnsFluidLiquidWaterSensorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1).
///
/// @details  Default constructs this GUNNS Fluid Liquid Water Sensor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLiquidWaterSensorInputData::GunnsFluidLiquidWaterSensorInputData(
                                                     const bool   malfBlockageFlag,
                                                     const double malfBlockageValue)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Liquid Water Sensor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLiquidWaterSensorInputData::GunnsFluidLiquidWaterSensorInputData(
        const GunnsFluidLiquidWaterSensorInputData& that)
    :
    GunnsFluidConductorInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Liquid Water Sensor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLiquidWaterSensorInputData::~GunnsFluidLiquidWaterSensorInputData()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Liquid Water Sensor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLiquidWaterSensor::GunnsFluidLiquidWaterSensor()
    :
    GunnsFluidConductor(),
    mLiquidPresent(false),
    mRelativeHumidity(0.0),
    mDewPoint(0.0),
    mSaturationPressure(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Liquid Water Sensor link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidLiquidWaterSensor::~GunnsFluidLiquidWaterSensor()
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
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Liquid Water Sensor link model with configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLiquidWaterSensor::initialize(
                                  const GunnsFluidLiquidWaterSensorConfigData& configData,
                                  const GunnsFluidLiquidWaterSensorInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&                links,
                                  const int                                    port0,
                                  const int                                    port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Initialize state data.
    mLiquidPresent      = false;
    mRelativeHumidity   = 0.0;
    mDewPoint           = 0.0;
    mSaturationPressure = 0.0;

    /// - Validate the link.
    validate();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Liquid Water Sensor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLiquidWaterSensor::validate() const
{
    /// - Catch and re-throw an exception on water vapor not present in this network.
    try {
        mNodes[0]->getContent()->find(FluidProperties::GUNNS_H2O);
    } catch (TsOutOfBoundsException &e) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                "GUNNS_H2O not found in network.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLiquidWaterSensor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Time step.
///
/// @returns  void
///
/// @details  Updates this GUNNS Fluid Liquid Water Sensor link model in run-time.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidLiquidWaterSensor::updateState(const double dt __attribute__((unused)))
{
    PolyFluid*             fluid = mNodes[0]->getContent();
    const double           ppH2o = fluid->getPartialPressure(FluidProperties::GUNNS_H2O);
    const FluidProperties* props = fluid->getProperties(FluidProperties::GUNNS_H2O);

    /// - Update the saturation pressure and dewpoint.
    mSaturationPressure = props->getSaturationPressure(fluid->getTemperature());
    mDewPoint           = props->getSaturationTemperature(ppH2o);

    /// - Update the relative humidity.  Liquid exists if relative humidity >= 1.0.
    if (mSaturationPressure > DBL_EPSILON) {
        mRelativeHumidity = ppH2o / mSaturationPressure;
    }
    mLiquidPresent = (mRelativeHumidity >= 1.0);
}
