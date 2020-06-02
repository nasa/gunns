/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Fluid Impeller Model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidPotential.o))

PROGRAMMERS:
   ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-09))
**************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidImpeller.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                 (--)             Name of object.
/// @param[in,out] nodes                (--)             Pointer to nodes.
/// @param[in]     maxConductivity      (m2)             Max conductivity.
/// @param[in]     expansionScaleFactor (--)             Scale factor for isentropic gas cooling.
/// @param[in]     referenceDensity     (kg/m3)          Reference fluid density for power curve.
/// @param[in]     referenceSpeed       (revolution/min) Reference impeller speed for power curve.
/// @param[in]     thermalLength        (m)              Impeller length for thermal convection.
/// @param[in]     thermalDiameter      (m)              Impeller inner diameter for thermal convection.
/// @param[in]     surfaceRoughness     (m)              Impeller wall surface roughness for convection.
///
/// @details  Default constructs this GUNNS Fluid Impeller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidImpellerConfigData::GunnsFluidImpellerConfigData(const std::string& name,
                                                           GunnsNodeList*     nodes,
                                                           const double       maxConductivity,
                                                           const double       expansionScaleFactor,
                                                           const double       referenceDensity,
                                                           const double       referenceSpeed,
                                                           const double       thermalLength,
                                                           const double       thermalDiameter,
                                                           const double       surfaceRoughness)
    :
    GunnsFluidPotentialConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mReferenceDensity(referenceDensity),
    mReferenceSpeed(referenceSpeed),
    mThermalLength(thermalLength),
    mThermalDiameter(thermalDiameter),
    mSurfaceRoughness(surfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Impeller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidImpellerConfigData::GunnsFluidImpellerConfigData(const GunnsFluidImpellerConfigData& that)
    :
    GunnsFluidPotentialConfigData(that),
    mReferenceDensity(that.mReferenceDensity),
    mReferenceSpeed(that.mReferenceSpeed),
    mThermalLength(that.mThermalLength),
    mThermalDiameter(that.mThermalDiameter),
    mSurfaceRoughness(that.mSurfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Impeller link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidImpellerConfigData::~GunnsFluidImpellerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)             Blockage malfunction flag.
/// @param[in] malfBlockageValue (--)             Blockage malfunction fractional value (0-1).
/// @param[in] sourcePressure    (kPa)            Initial pressure rise of the link.
/// @param[in] motorSpeed        (revolution/min) Initial speed of the motor.
/// @param[in] wallTemperature   (K)              Initial impeller wall temperature.
///
/// @details  Default constructs this GUNNS Fluid Impeller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidImpellerInputData::GunnsFluidImpellerInputData(const bool   malfBlockageFlag,
                                                         const double malfBlockageValue,
                                                         const double sourcePressure,
                                                         const double motorSpeed,
                                                         const double wallTemperature)
    :
    GunnsFluidPotentialInputData(malfBlockageFlag, malfBlockageValue, sourcePressure),
    mMotorSpeed     (motorSpeed),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Impeller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidImpellerInputData::GunnsFluidImpellerInputData(const GunnsFluidImpellerInputData& that)
    :
    GunnsFluidPotentialInputData(that),
    mMotorSpeed     (that.mMotorSpeed),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Impeller link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidImpellerInputData::~GunnsFluidImpellerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Impeller link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidImpeller::GunnsFluidImpeller()
    :
    GunnsFluidPotential(),
    mPowerCurveCoefficient(0.0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mMotorSpeed(0.0),
    mWallTemperature(0.0),
    mWallHeatFlux(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Impeller link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidImpeller::~GunnsFluidImpeller()
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
/// @details  Initializes this GUNNS Fluid Impeller link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidImpeller::initialize(const GunnsFluidImpellerConfigData& configData,
                                    const GunnsFluidImpellerInputData&  inputData,
                                    std::vector<GunnsBasicLink*>&       links,
                                    const int                           port0,
                                    const int                           port1)
{
    /// - First initialize & validate parent.
    GunnsFluidPotential::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag               = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize from input data.
    mMotorSpeed            = inputData.mMotorSpeed;
    mWallTemperature       = inputData.mWallTemperature;
    mWallHeatFlux          = 0.0;

    /// - Initialize from configuration data.
    mPowerCurveCoefficient = inputData.mSourcePressure / (configData.mReferenceDensity * configData.mReferenceSpeed * configData.mReferenceSpeed);
    mThermalDiameter       = configData.mThermalDiameter;
    mThermalSurfaceArea    = UnitConversion::PI_UTIL * configData.mThermalLength * configData.mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD     = configData.mSurfaceRoughness / configData.mThermalDiameter;
    } else {
        mThermalROverD     = 0.0;
    }

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Warn of deprecation due to obsolescence by GunnsGasFan.
    GUNNS_WARNING("this link is deprecated!  It is obsoleted by GunnsGasFan.");

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag              = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Impeller Model link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidImpeller::validate(const GunnsFluidImpellerConfigData& configData,
                                  const GunnsFluidImpellerInputData&  inputData) const
{
    /// - Throw an exception if reference density is non-positive,
    if (configData.mReferenceDensity < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Reference density < DBL_EPSILON.");
    }
    /// - Throw an exception if reference speed is non-positive,
    if (configData.mReferenceSpeed < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "Reference speed < DBL_EPSILON.");
    }
    /// - Throw an exception if source (reference) pressure is non-positive,
    if (inputData.mSourcePressure < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Source (reference) pressure value < DBL_EPSILON.");
    }
    /// - Throw an exception if impeller speed is negative,
    if (inputData.mMotorSpeed < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Impeller speed < 0.");
    }
    /// - Throw an exception if impeller temperature is negative,
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Impeller temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidImpeller::restartModel()
{
    /// - Reset the base class.
    GunnsFluidPotential::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Time step (not used).
///
/// @return   void
///
/// @details  Updates this GUNNS Fluid Impeller link model source pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidImpeller::updateState(const double dt __attribute__((unused)))
{
    /// - Compute delta pressure proportional to fluid density and square of impeller speed.
    mSourcePressure = mPowerCurveCoefficient * mMotorSpeed * mMotorSpeed * mInternalFluid->getDensity();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt        (s)     Time step.
/// @param[in]    flowRate  (kg/s)  Mass flow rate.
///
/// @return   void
///
/// @details  Updates this GUNNS Fluid Impeller link model internal fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidImpeller::updateFluid(const double dt __attribute__((unused)), const double flowRate)
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
/// @details  Sets the thermal surface area of this this GUNNS Fluid Impeller link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidImpeller::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Fluid Impeller link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidImpeller::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}
