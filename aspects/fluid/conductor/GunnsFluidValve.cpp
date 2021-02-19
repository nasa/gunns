/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Fluid Valve link model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))

PROGRAMMERS:
   ((Kenneth McMurtrie) (Tietronix Software) (Update) (2011-09))
**************************************************************************************************/

#include "core/GunnsFluidUtils.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                 (--)    Name of object.
/// @param[in]  nodes                (--)    Pointer to nodes.
/// @param[in]  maxConductivity      (m2)    Maximum conductivity.
/// @param[in]  expansionScaleFactor (--)    Scale factor for isentropic gas cooling.
/// @param[in]  thermalLength        (m)     Tube length for thermal convection
/// @param[in]  thermalDiameter      (m)     Tube inner diameter for thermal convection
/// @param[in]  surfaceRoughness     (m)     Tube wall surface roughness for thermal convection
///
/// @details  Default constructs this GUNNS Fluid Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidValveConfigData::GunnsFluidValveConfigData(const std::string& name,
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
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidValveConfigData::GunnsFluidValveConfigData(const GunnsFluidValveConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mThermalLength   (that.mThermalLength),
    mThermalDiameter (that.mThermalDiameter),
    mSurfaceRoughness(that.mSurfaceRoughness)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidValveConfigData::~GunnsFluidValveConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   (--)    Blockage malfunction flag.
/// @param[in]  malfBlockageValue  (--)    Blockage malfunction fractional value (0-1).
/// @param[in]  position           (--)    Fractional position.
/// @param[in]  malfLeakThruFlag   (--)    Leak through rate malfunction flag.
/// @param[in]  malfLeakThruValue  (kg/s)  Leak through rate malfunction value.
/// @param[in]  wallTemperature    (K)     Tube wall temperature for thermal convection
///
/// @details  Default constructs this GUNNS Fluid Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidValveInputData::GunnsFluidValveInputData(const bool   malfBlockageFlag,
                                                   const double malfBlockageValue,
                                                   const double position,
                                                   const bool   malfLeakThruFlag,
                                                   const double malfLeakThruValue,
                                                   const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mPosition(position),
    mMalfLeakThruFlag(malfLeakThruFlag),
    mMalfLeakThruValue(malfLeakThruValue),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidValveInputData::GunnsFluidValveInputData(const GunnsFluidValveInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mPosition(that.mPosition),
    mMalfLeakThruFlag(that.mMalfLeakThruFlag),
    mMalfLeakThruValue(that.mMalfLeakThruValue),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidValveInputData::~GunnsFluidValveInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidValve::GunnsFluidValve()
    :
    GunnsFluidConductor(),
    mMalfLeakThruFlag(false),
    mMalfLeakThruValue(0.0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mWallTemperature(0.0),
    mWallHeatFlux(0.0),
    mPosition(0.0),
    mPreviousLeakRate(0.0),
    mLeakConductivity(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Valve link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidValve::~GunnsFluidValve()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
/// @param[in,out]  links        --  Link vector.
/// @param[in]      port0        --  Nominal inlet port map index.
/// @param[in]      port1        --  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Valve link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidValve::initialize(const GunnsFluidValveConfigData& configData,
                                 const GunnsFluidValveInputData&  inputData,
                                 std::vector<GunnsBasicLink*>&    links,
                                 const int                        port0,
                                 const int                        port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag          = false;

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
    mWallTemperature    = inputData.mWallTemperature;
    mWallHeatFlux       = 0.0;
    mPosition          = inputData.mPosition;
    mMalfLeakThruFlag  = inputData.mMalfLeakThruFlag;
    mMalfLeakThruValue = inputData.mMalfLeakThruValue;
    mPreviousLeakRate  = 0.0;
    mLeakConductivity  = 0.0;

    /// - Validate configuration and input data.
    validate(inputData);

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag          = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inputData  (--) Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Valve link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidValve::validate(const GunnsFluidValveInputData&  inputData) const
{
    /// - Throw an exception if initial wall temperature is negative.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Wall temperature < 0.");
    }

    /// - Throw an exception if valve position < 0 or valve position > 1 .
    if (!MsMath::isInRange(0.0, mPosition, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Position outside valid range (0-1).");
    }

    /// - Throw an exception if malfunction leak through rate < 0.
    if (mMalfLeakThruValue < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Malfunction leak through rate < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidValve::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Time step (not used).
///
/// @return   void
///
/// @note     Derived classes should call this method at the end of their updateState method.
///
/// @details  Processes any malfunctions and updates this GUNNS Fluid Valve link model state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidValve::updateState(const double dt __attribute__((unused)))
{
    if (mMalfLeakThruFlag) {
        /// - For leak rate malfunction, compute leak conductivity once on malfunction activation
        ///   and set effective conductivity.
        if (mPreviousLeakRate != mMalfLeakThruValue) {
            mLeakConductivity  = GunnsFluidUtils::predictConductivity(mMalfLeakThruValue,
                                                                      mMinLinearizationPotential,
                                                                      mNodes[0]->getOutflow(),
                                                                      mNodes[1]->getOutflow());
            mPreviousLeakRate  = mMalfLeakThruValue;
        }
    } else {
        /// - For no active malfunctions, zero out leak conductivity.
        mPreviousLeakRate      = 0.0;
        mLeakConductivity      = 0.0;
    }

    /// - Effective conductivity is maximum conductivity scaled by position, bounded below by leak
    ///   conductivity.
    mEffectiveConductivity = mMaxConductivity * mPosition;
    if (mEffectiveConductivity < mLeakConductivity) {
        mEffectiveConductivity = mLeakConductivity;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt       (s)    Not used.
/// @param[in] flowRate (kg/s) Mass flow rate.
///
/// @returns  void
///
/// @details  Updates the internal fluid of this this GUNNS Fluid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidValve::updateFluid(const double dt __attribute__((unused)), const double flowRate)
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
/// @param[in] flag   (--)    Malfunction activation flag.
/// @param[in] value  (kg/s)  Desired initial leak mass flow rate.
///
/// @returns  void
///
/// @details  Arguments are defaulted to deactivate the malfunction, so you can conveniently turn
///           it off by calling setMalfLeakThru().
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidValve::setMalfLeakThru(const bool flag, const double value)
{
    mMalfLeakThruFlag  = flag;
    mMalfLeakThruValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @returns  void
///
/// @details  Sets the thermal surface area of this this GUNNS Fluid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidValve::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Fluid Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidValve::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}
