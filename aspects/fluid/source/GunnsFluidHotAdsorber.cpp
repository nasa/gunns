/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((aspects/fluid/source/GunnsFluidAdsorber.o))
**************************************************************************************************/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "GunnsFluidHotAdsorber.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                  (--)    Name of object.
/// @param[in,out]  nodes                 (--)    Pointer to nodes.
/// @param[in]      maxConductivity       (m2)    Maximum conductivity.
/// @param[in]      expansionScaleFactor  (--)    Scale factor for isentropic gas cooling.
/// @param[in]      gasType               (--)    Fluid type of constituent gas being sorbed.
/// @param[in]      efficiency            (--)    Efficiency of gas adsorbtion from atmosphere.
/// @param[in]      maxAdsorbtionRate     (kg/s)  Maximum mass rate at which gas can be adsorbed from atmosphere.
/// @param[in]      maxAdsorbedMass       (kg)    Maximum amount of adsorbed mass.
/// @param[in]      desorbtionRate        (kg/s)  Mass rate at which gas can be desorbed to atmosphere.
/// @param[in]      efficiencyBias        (1/K)   Bias for the dependency of efficiency on temperature.
/// @param[in]      efficiencyScaleFactor (--)    Scale factor for the dependency of efficiency on temperature.
/// @param[in]      thermalLength         (m)     Tube length for thermal convection.
/// @param[in]      thermalDiameter       (m)     Tube inner diameter for thermal convection.
/// @param[in]      surfaceRoughness      (m)     Tube wall surface roughness for thermal convection.
///
/// @details  Default constructs this GUNNS Fluid Hot Adsorber link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotAdsorberConfigData::GunnsFluidHotAdsorberConfigData(const std::string&               name,
                                                                 GunnsNodeList*                   nodes,
                                                                 const double                     maxConductivity,
                                                                 const double                     expansionScaleFactor,
                                                                 const FluidProperties::FluidType gasType,
                                                                 const double                     efficiency,
                                                                 const double                     maxAdsorbtionRate,
                                                                 const double                     maxAdsorbedMass,
                                                                 const double                     desorbtionRate,
                                                                 const double                     thermalLength,
                                                                 const double                     thermalDiameter,
                                                                 const double                     surfaceRoughness,
                                                                 const double                     efficiencyBias,
                                                                 const double                     efficiencyScaleFactor)
    :
    GunnsFluidAdsorberConfigData(name,
                                 nodes,
                                 maxConductivity,
                                 expansionScaleFactor,
                                 gasType,
                                 efficiency,
                                 maxAdsorbtionRate,
                                 maxAdsorbedMass,
                                 desorbtionRate,
                                 thermalLength,
                                 thermalDiameter,
                                 surfaceRoughness),
    mEfficiencyBias(efficiencyBias),
    mEfficiencyScaleFactor(efficiencyScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Hot Adsorber link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotAdsorberConfigData::GunnsFluidHotAdsorberConfigData(const GunnsFluidHotAdsorberConfigData& that)
    :
    GunnsFluidAdsorberConfigData(that),
    mEfficiencyBias(that.mEfficiencyBias),
    mEfficiencyScaleFactor(that.mEfficiencyScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hot Adsorber link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotAdsorberConfigData::~GunnsFluidHotAdsorberConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag     (--)  Blockage malfunction flag.
/// @param[in]  malfBlockageValue    (--)  Blockage malfunction fractional value (0-1).
/// @param[in]  desorbtionCycle      (--)  Desorbtion cycle flag.
/// @param[in]  adsorbedMass         (--)  Adsorbed mass in the adsorber.
/// @param[in]  wallTemperature      (K)   Tube wall temperature for thermal convection.
/// @param[in]  tcEfficiency         (--)  Adsorption/desorption efficiency for trace compounds.
/// @param[in]  tcMaxAdsorbedMass    (kg)  Maximum amount of adsorbed mass of trace compounds.
/// @param[in]  tcAdsorbedMass       (kg)  Initial amount of adsorbed mass of trace compounds.
/// @param[in]  malfEfficiencyFlag   (--)  Initial efficiency malfunction flag.
/// @param[in]  malfEfficiencyValue  (--)  Initial efficiency malfunction value.
///
/// @details  Default constructs this GUNNS Fluid Hot Adsorber link model input data with arguments
///           and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotAdsorberInputData::GunnsFluidHotAdsorberInputData(
        const bool                         malfBlockageFlag,
        const double                       malfBlockageValue,
        const bool                         desorbtionCycle,
        const double                       adsorbedMass,
        const double                       wallTemperature,
        GunnsFluidTraceCompoundsInputData* tcEfficiency,
        GunnsFluidTraceCompoundsInputData* tcMaxAdsorbedMass,
        GunnsFluidTraceCompoundsInputData* tcAdsorbedMass,
        const bool                         malfEfficiencyFlag,
        const double                       malfEfficiencyValue)
    :
    GunnsFluidAdsorberInputData(malfBlockageFlag,
                                malfBlockageValue,
                                desorbtionCycle,
                                adsorbedMass,
                                wallTemperature,
                                tcEfficiency,
                                tcMaxAdsorbedMass,
                                tcAdsorbedMass,
                                malfEfficiencyFlag,
                                malfEfficiencyValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  (--)  Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Hot Adsorber link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotAdsorberInputData::GunnsFluidHotAdsorberInputData(const GunnsFluidHotAdsorberInputData& that)
    :
    GunnsFluidAdsorberInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hot Adsorber link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotAdsorberInputData::~GunnsFluidHotAdsorberInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Hot Adsorber link model with obviously invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotAdsorber::GunnsFluidHotAdsorber()
    :
    GunnsFluidAdsorber(),
    mEfficiencyBias(0.0),
    mEfficiencyScaleFactor(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hot Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotAdsorber::~GunnsFluidHotAdsorber()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Hot Adsorber link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHotAdsorber::initialize(const GunnsFluidHotAdsorberConfigData& configData,
                                      const GunnsFluidHotAdsorberInputData&   inputData,
                                      std::vector<GunnsBasicLink*>&           links,
                                      const int                               port0,
                                      const int                               port1)
{
    /// - First initialize and validate parent.
    GunnsFluidAdsorber::initialize(configData, inputData, links, port0, port1);

    /// - Reset the initialization status flag.
    mInitFlag              = false;

    /// - Validate the configuration and input data.
    validate(configData, inputData);

    /// - Initialize from the validated configuration and input data.
    mEfficiencyBias        = configData.mEfficiencyBias;
    mEfficiencyScaleFactor = configData.mEfficiencyScaleFactor;

    /// - Initialize the outputs.
    updateFluid(0.0, 0.0);

    /// - Set the initialization status flag to indicate successful initialization.
    mInitFlag              = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHotAdsorber::restartModel()
{
    /// - Reset the base class.
    GunnsFluidAdsorber::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (--)  Efficiency of the sorbtion (-1 to 1).
///
/// @details  Computes and returns the efficiency of the specified sorbtion linearly dependent on
///           temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHotAdsorber::computeEfficiency()
{
    double efficiency = MsMath::limitRange(-1.0, mEfficiencyBias + mEfficiencyScaleFactor * mFluidTemperature, 1.0);

    /// - The efficiency malfunction overrides the normal value.  Desorption cycle flag responds to
    ///   the final efficiency.
    if (mMalfEfficiencyFlag) {
        efficiency = mMalfEfficiencyValue;
    }
    mDesorbtionCycle = efficiency < 0.0;
    return efficiency;
}
