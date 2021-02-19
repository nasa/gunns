/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((core/GunnsFluidConductor.o)
  (core/GunnsFluidUtils.o))
***************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidPipe.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                 (--)    Name of object.
/// @param[in]      nodes                (--)    Pointer to nodes.
/// @param[in]      maxConductivity      (m2)    Maximum conductivity.
/// @param[in]      expansionScaleFactor (--)    Scale factor for isentropic gas cooling.
/// @param[in]      thermalLength        (m)     Tube length for thermal convection
/// @param[in]      thermalDiameter      (m)     Tube inner diameter for thermal convection
/// @param[in]      surfaceRoughness     (m)     Tube wall surface roughness for thermal convection
///
/// @details        Default constructs this GUNNS Fluid Pipe Link Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPipeConfigData::GunnsFluidPipeConfigData(const std::string& name,
                                                   GunnsNodeList*     nodes,
                                                   const double       maxConductivity,
                                                   const double       expansionScaleFactor,
                                                   const double       thermalLength,
                                                   const double       thermalDiameter,
                                                   const double       surfaceRoughness)
    :
    GunnsFluidConductorConfigData(name,
                                  nodes,
                                  maxConductivity,
                                  expansionScaleFactor),
    mThermalLength(thermalLength),
    mThermalDiameter(thermalDiameter),
    mSurfaceRoughness(surfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that   (--)  Source to copy.
///
/// @details        Copy constructs this GUNNS Fluid Pipe Link Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPipeConfigData::GunnsFluidPipeConfigData(const GunnsFluidPipeConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mThermalLength   (that.mThermalLength),
    mThermalDiameter (that.mThermalDiameter),
    mSurfaceRoughness(that.mSurfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pipe Link Model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPipeConfigData::~GunnsFluidPipeConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      malfBlockageFlag   (--)   Blockage malfunction flag.
/// @param[in]      malfBlockageValue  (--)   Blockage malfunction fractional value (0-1).
/// @param[in]      wallTemperature    (K)    Tube wall temperature for thermal convection
///
/// @details        Default constructs this GUNNS Fluid Pipe Link Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPipeInputData::GunnsFluidPipeInputData(const bool   malfBlockageFlag,
                                                 const double malfBlockageValue,
                                                 const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  (--)  Source to copy.
///
/// @details        Copy constructs this GUNNS Fluid Pipe Link Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPipeInputData::GunnsFluidPipeInputData(const GunnsFluidPipeInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pipe Link Model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPipeInputData::~GunnsFluidPipeInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note           This should be followed by a call to the initialize method before calling an
///                 update method.
///
/// @details        Default constructs this GUNNS Fluid Pipe Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPipe::GunnsFluidPipe()
    :
    GunnsFluidConductor(),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mWallTemperature(0.0),
    mWallHeatFlux(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details        Default destructs this GUNNS Fluid Pipe Link Model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPipe::~GunnsFluidPipe()
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
/// @return         void
///
/// @throws         TsInitializationException
///
/// @details        Initializes this this GUNNS Fluid Pipe Link Model with configuration and input
///                 data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPipe::initialize(const GunnsFluidPipeConfigData& configData,
                                const GunnsFluidPipeInputData&  inputData,
                                std::vector<GunnsBasicLink*>&   links,
                                const int                       port0,
                                const int                       port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag           = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize with configuration data.
    mThermalDiameter    = configData.mThermalDiameter;
    mThermalSurfaceArea = configData.mThermalLength * UnitConversion::PI_UTIL * mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD  = configData.mSurfaceRoughness / mThermalDiameter;
    } else {
        mThermalROverD  = 0.0;
    }

    /// - Initialize with input data.
    mWallTemperature    = inputData.mWallTemperature;
    mWallHeatFlux       = 0.0;

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data (unused).
/// @param[in]      inputData    (--)  Input data.
///
/// @return         void
///
/// @throws         TsInitializationException
///
/// @details        Validates this this GUNNS Fluid Pipe Link Model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPipe::validate(const GunnsFluidPipeConfigData& configData __attribute__((unused)),
                              const GunnsFluidPipeInputData&  inputData) const
{
    /// - Throw an exception if pipe temperature is negative.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Pipe temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPipe::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt        (s)     Time step.
/// @param[in]      flowRate  (kg/s)  Mass flow rate.
///
/// @return         void
///
/// @details        Updates the internal fluid of this this GUNNS Fluid Pipe Link Model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPipe::updateFluid(const double dt __attribute__((unused)), const double flowRate)
{
    /// - Perform heat convection between the internal fluid and pipe wall.
    mWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                               flowRate,
                                                               mThermalROverD,
                                                               mThermalDiameter,
                                                               mThermalSurfaceArea,
                                                               mWallTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @returns  void
///
/// @details  Sets the thermal surface area of this this GUNNS Fluid Pipe link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPipe::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Fluid Pipe link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPipe::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}
