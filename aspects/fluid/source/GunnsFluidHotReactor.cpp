/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((aspects/fluid/source/GunnsFluidReactor.o))
**************************************************************************************************/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "GunnsFluidHotReactor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                  (--)   Name of object.
/// @param[in,out]  nodes                 (--)   Pointer to nodes.
/// @param[in]      maxConductivity       (m2)   Maximum conductivity.
/// @param[in]      expansionScaleFactor  (--)   Scale factor for isentropic gas cooling.
/// @param[in]      reactions             (--)   Pointer to Defined Chemical Reactions.
/// @param[in]      reactionTypes         (--)   Array of Chemical Reaction Types.
/// @param[in]      nReactions            (--)   Number of Chemical Reactions.
/// @param[in]      compounds             (--)   Pointer to Defined Chemical Compounds.
/// @param[in]      compoundTypes         (--)   Array of Chemical Compound types.
/// @param[in]      nCompounds            (--)   Number of Chemical Compounds.
/// @param[in]      thermalLength         (m)    Tube length for thermal convection.
/// @param[in]      thermalDiameter       (m)    Tube inner diameter for thermal convection.
/// @param[in]      surfaceRoughness      (m)    Tube wall surface roughness for thermal convection.
/// @param[in]      efficiencyBias        (1/K)  Bias for the dependency of efficiency on temperature.
/// @param[in]      efficiencyScaleFactor (--)   Scale factor for the dependency of efficiency on temperature.
///
/// @details  Default constructs this GUNNS Fluid Hot Reactor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotReactorConfigData::GunnsFluidHotReactorConfigData(const std::string&              name,
                                                               GunnsNodeList*                  nodes,
                                                               const double                    maxConductivity,
                                                               const double                    expansionScaleFactor,
                                                               const DefinedChemicalReactions* reactions,
                                                               ChemicalReaction::Type*         reactionTypes,
                                                               const int                       nReactions,
                                                               const DefinedChemicalCompounds* compounds,
                                                               ChemicalCompound::Type*         compoundTypes,
                                                               const int                       nCompounds,
                                                               const double                    thermalLength,
                                                               const double                    thermalDiameter,
                                                               const double                    surfaceRoughness,
                                                               const double                    efficiencyBias,
                                                               const double                    efficiencyScaleFactor)
    :
    GunnsFluidReactorConfigData(name,
                                nodes,
                                maxConductivity,
                                expansionScaleFactor,
                                reactions,
                                reactionTypes,
                                nReactions,
                                compounds,
                                compoundTypes,
                                nCompounds,
                                thermalLength,
                                thermalDiameter,
                                surfaceRoughness),
    mEfficiencyBias(efficiencyBias),
    mEfficiencyScaleFactor(efficiencyScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hot Reactor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotReactorConfigData::~GunnsFluidHotReactorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      malfBlockageFlag    (--)    Blockage malfunction flag.
/// @param[in]      malfBlockageValue   (--)    Blockage malfunction fractional value (0-1).
/// @param[in]      totalMasses         (--)    Array of Chemical Compound total masses.
/// @param[in]      wallTemperature     (K)     Tube wall temperature for thermal convection.
///
/// @details  Default constructs this GUNNS Fluid Hot Reactor link model input data with arguments
///           and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotReactorInputData::GunnsFluidHotReactorInputData(const bool   malfBlockageFlag,
                                                             const double malfBlockageValue,
                                                             double*      totalMasses,
                                                             const double wallTemperature)
    :
    GunnsFluidReactorInputData(malfBlockageFlag, malfBlockageValue, totalMasses, wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      that  (--)  Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Hot Reactor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotReactorInputData::GunnsFluidHotReactorInputData(const GunnsFluidHotReactorInputData& that)
    :
    GunnsFluidReactorInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hot Reactor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotReactorInputData::~GunnsFluidHotReactorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Hot Reactor link model with obviously invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotReactor::GunnsFluidHotReactor()
    :
    GunnsFluidReactor(),
    mEfficiencyBias(0.0),
    mEfficiencyScaleFactor(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Hot Reactor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidHotReactor::~GunnsFluidHotReactor()
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
/// @details  Initializes this GUNNS Fluid Hot Reactor link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidHotReactor::initialize(      GunnsFluidHotReactorConfigData& configData,
                                      const GunnsFluidHotReactorInputData&  inputData,
                                      std::vector<GunnsBasicLink*>&         links,
                                      const int                             port0,
                                      const int                             port1)
{
    /// - First initialize and validate parent.
    GunnsFluidReactor::initialize(configData, inputData, links, port0, port1);

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
void GunnsFluidHotReactor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidReactor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      i  (--)  Reaction index.
///
/// @return   (--)  Efficiency of the reaction (0-1).
///
/// @details  Computes and returns the efficiency of the specified reaction linearly dependent on
///           fluid temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidHotReactor::computeEfficiency(const int i __attribute__((unused)))
{
    // using fluid temperature from previous cycle
    double result = MsMath::limitRange(0.0, mEfficiencyBias + mEfficiencyScaleFactor * mFluidTemperature, 1.0);

    /// - Scale by the optional reagent efficiency times the reagent mass.
    if (mReactions[i].mReagentEfficiency > DBL_EPSILON) {
        result *= mReactions[i].mReagentEfficiency * mCompounds[mReactions[i].mReagentIndex].mTotalMass;
    }

    return result;
}
