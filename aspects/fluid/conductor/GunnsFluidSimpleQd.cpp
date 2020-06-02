/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Simple Quick-Disconnect link model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Initial) (2012-08))
**************************************************************************************************/

#include "GunnsFluidSimpleQd.hh"
#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Name of object.
/// @param[in] nodes                (--) Pointer to nodes.
/// @param[in] maxConductivity      (m2) Max conductivity.
/// @param[in] expansionScaleFactor (--) Scale factor for isentropic gas cooling.
/// @param[in] thermalLength        (m)  Tube length for thermal convection.
/// @param[in] thermalDiameter      (m)  Tube inner diameter for thermal convection.
/// @param[in] surfaceRoughness     (m)  Tube wall surface roughness for thermal convection.
///
/// @details   Default constructs this GUNNS Simple Quick-Disconnect link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleQdConfigData::GunnsFluidSimpleQdConfigData(const std::string& name,
                                                           GunnsNodeList*     nodes,
                                                           const double       maxConductivity,
                                                           const double       expansionScaleFactor,
                                                           const double       thermalLength,
                                                           const double       thermalDiameter,
                                                           const double       surfaceRoughness)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mThermalLength   (thermalLength),
    mThermalDiameter (thermalDiameter),
    mSurfaceRoughness(surfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source to copy.
///
/// @details  Copy constructs this GUNNS Simple Quick-Disconnect link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleQdConfigData::GunnsFluidSimpleQdConfigData(const GunnsFluidSimpleQdConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mThermalLength   (that.mThermalLength),
    mThermalDiameter (that.mThermalDiameter),
    mSurfaceRoughness(that.mSurfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Simple Quick-Disconnect link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleQdConfigData::~GunnsFluidSimpleQdConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1).
/// @param[in] state             (--) Initial connection state of the QD.
/// @param[in] wallTemperature   (K)  Tube wall temperature for thermal convection.
///
/// @details  Default constructs this GUNNS Simple Quick-Disconnect link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleQdInputData::GunnsFluidSimpleQdInputData(const bool   malfBlockageFlag,
                                                         const double malfBlockageValue,
                                                         const GunnsFluidSimpleQd::State state,
                                                         const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mState          (state),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source to copy.
///
/// @details  Copy constructs this GUNNS Simple Quick-Disconnect link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleQdInputData::GunnsFluidSimpleQdInputData(const GunnsFluidSimpleQdInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mState          (that.mState),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Simple Quick-Disconnect link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleQdInputData::~GunnsFluidSimpleQdInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Simple Quick-Disconnect link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleQd::GunnsFluidSimpleQd()
    :
    GunnsFluidConductor(),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mState(0),
    mWallTemperature(0.0),
    mWallHeatFlux(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Simple Quick-Disconnect link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleQd::~GunnsFluidSimpleQd()
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
/// @details  Initializes this this GUNNS Simple Quick-Disconnect link model with configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleQd::initialize(const GunnsFluidSimpleQdConfigData& configData,
                                    const GunnsFluidSimpleQdInputData&  inputData,
                                    std::vector<GunnsBasicLink*>&       links,
                                    const int                           port0,
                                    const int                           port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag           = false;

    /// - Validate input data.
    validate(inputData);

    /// - Initialize with configuration data.  Since surface area is a product of diameter, checking
    ///   it avoids divide-by-zero on diameter, and it also determines whether thermal convection is
    ///   to be used, which includes instantiating the internal fluid.
    mThermalDiameter    = configData.mThermalDiameter;
    mThermalSurfaceArea = configData.mThermalLength * UnitConversion::PI_UTIL * mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD  = configData.mSurfaceRoughness / mThermalDiameter;
        createInternalFluid();
    } else {
        mThermalROverD = 0.0;
    }

    /// - Initialize with input data.
    mState              = static_cast<int>(inputData.mState);
    mWallTemperature    = inputData.mWallTemperature;
    mWallHeatFlux       = 0.0;

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inputData  (--) Input data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates this this GUNNS Simple Quick-Disconnect link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleQd::validate(const GunnsFluidSimpleQdInputData&  inputData) const
{
    /// - Throw an exception if initial wall temperature is negative.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Wall temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleQd::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt       (s)    Not used.
///
/// @returns  void
///
/// @details  Updates the conductivity of the Simple QD based on the connection state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleQd::updateState(const double dt __attribute__((unused)))
{
    if (GunnsFluidSimpleQd::CONNECTED == static_cast<GunnsFluidSimpleQd::State>(mState)) {
        mEffectiveConductivity = mMaxConductivity;
    } else {
        mEffectiveConductivity = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt       (s)    Not used.
/// @param[in] flowRate (kg/s) Mass flow rate.
///
/// @returns  void
///
/// @details  Updates the internal fluid of this this GUNNS Simple Quick-Disconnect link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleQd::updateFluid(const double dt __attribute__((unused)), const double flowRate)
{
    /// - Perform heat convection between the internal fluid and tube wall.
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
/// @details  Sets the thermal surface area of this GUNNS Simple Quick-Disconnect link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleQd::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this GUNNS Simple Quick-Disconnect link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleQd::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}
