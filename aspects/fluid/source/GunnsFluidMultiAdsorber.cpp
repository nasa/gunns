/**
@file
@brief    GUNNS Multiple Constituent Adsorber Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o)
    (GunnsFluidAdsorptionCompound.o))
*/

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "aspects/fluid/fluid/GunnsFluidTraceCompounds.hh"
#include "GunnsFluidMultiAdsorber.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                 (--) Name of object.
/// @param[in,out] nodes                (--) Pointer to nodes.
/// @param[in]     maxConductivity      (m2) Maximum conductivity.
/// @param[in]     expansionScaleFactor (--) Scale factor for isentropic gas cooling.
/// @param[in]     thermalLength        (m)  Tube length for thermal convection.
/// @param[in]     thermalDiameter      (m)  Tube inner diameter for thermal convection.
/// @param[in]     surfaceRoughness     (m)  Tube wall surface roughness for thermal convection.
///
/// @details  Default constructs this GUNNS Fluid Multiple Constituent Adsorber link config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiAdsorberConfigData::GunnsFluidMultiAdsorberConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       maxConductivity,
        const double       expansionScaleFactor,
        const double       thermalLength,
        const double       thermalDiameter,
        const double       surfaceRoughness)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mThermalLength(thermalLength),
    mThermalDiameter(thermalDiameter),
    mSurfaceRoughness(surfaceRoughness),
    mCompounds()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Multiple Constituent Adsorber link config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiAdsorberConfigData::~GunnsFluidMultiAdsorberConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] type                  (--)       The compound that is sorbed.
/// @param[in] maxAdsorbedMass       (kg)       Maximum mass that can be adsorbed.
/// @param[in] efficiencyCoeff0      (--)       Sorption efficiency.
/// @param[in] efficiencyCoeff1      (1/K)      Sorption efficiency temperature coefficient.
/// @param[in] desorbPartialPressure (kPa)      Partial pressure at switch between adsorption and desorption.
/// @param[in] desorbRateFactor      (kg/s/kPa) Desorption rate dependence on pressure.
/// @param[in] heatOfAdsorptio;      (kJ/kg)    Enthalpy change of reaction in adsorption direction.
/// @param[in] taperOffFlag          (--)       Whether sorption rate diminishes as adsorbed mass approaches limits.
/// @param[in] dependentType         (--)       Optional other compound on which this compound's sorption depends.
/// @param[in] malfEfficiencyFlag    (--)       Initial efficiency malfunction flag.
/// @param[in] malfEfficiencyValue   (--)       Initial efficiency malfunction value.
/// @param[in] adsorbedMass          (kg)       Initial adsorbed mass of this compound in the adsorber.
/// @param[in] breakthroughExp       (--)       Exponent value for breakthrough adsorption.
///
/// @details  Adds a new compound with the given properties to the compounds container in this
///           GUNNS Fluid Multiple Constituent Adsorber link config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiAdsorberConfigData::addCompound(
        const ChemicalCompound::Type type,
        const double                 maxAdsorbedMass,
        const double                 efficiencyCoeff0,
        const double                 efficiencyCoeff1,
        const double                 desorbPartialPressure,
        const double                 desorbRateFactor,
        const double                 heatOfAdsorption,
        const bool                   taperOffFlag,
        const ChemicalCompound::Type dependentType,
        const bool                   malfEfficiencyFlag,
        const double                 malfEfficiencyValue,
        const double                 adsorbedMass,
        const double                 breakthroughExp)
{
    mCompounds.push_back(GunnsFluidAdsorptionCompound());
    mCompounds.back().mType                  = type;
    mCompounds.back().mEfficiencyCoeff0      = efficiencyCoeff0;
    mCompounds.back().mEfficiencyCoeff1      = efficiencyCoeff1;
    mCompounds.back().mMaxAdsorbedMass       = maxAdsorbedMass;
    mCompounds.back().mDesorbPartialPressure = desorbPartialPressure;
    mCompounds.back().mDesorbRateFactor      = desorbRateFactor;
    mCompounds.back().mHeatOfAdsorption      = heatOfAdsorption;
    mCompounds.back().mTaperOffFlag          = taperOffFlag;
    mCompounds.back().mDependentType         = dependentType;
    mCompounds.back().mMalfEfficiencyFlag    = malfEfficiencyFlag;
    mCompounds.back().mMalfEfficiencyValue   = malfEfficiencyValue;
    mCompounds.back().mAdsorbedMass          = adsorbedMass;
    mCompounds.back().mBreakthroughExp       = breakthroughExp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag     (--)  Blockage malfunction flag.
/// @param[in]  malfBlockageValue    (--)  Blockage malfunction fractional value (0-1).
/// @param[in]  wallTemperature      (K)   Tube wall temperature for thermal convection.
///
/// @details  Default constructs this GUNNS Fluid Multiple Constituent Adsorber link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiAdsorberInputData::GunnsFluidMultiAdsorberInputData(const bool   malfBlockageFlag,
                                                                   const double malfBlockageValue,
                                                                   const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Multiple Constituent Adsorber link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiAdsorberInputData::~GunnsFluidMultiAdsorberInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Multiple Constituent Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiAdsorber::GunnsFluidMultiAdsorber()
    :
    GunnsFluidConductor(),
    mCompounds(),
    mNCompounds(0),
    mThermalDiameter(0.0),
    mThermalSurfaceArea(0.0),
    mThermalROverD(0.0),
    mWallTemperature(0.0),
    mWallHeatFlux(0.0),
    mSorptionHeat(0.0),
    mSorptionFluid(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Multiple Constituent Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiAdsorber::~GunnsFluidMultiAdsorber()
{
    TS_DELETE_ARRAY(mCompounds);
    TS_DELETE_OBJECT(mSorptionFluid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @return      void
///
/// @throws      TsInitializationException
///
/// @details     Initializes this GUNNS Fluid Multiple Constituent Adsorber link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiAdsorber::initialize(const GunnsFluidMultiAdsorberConfigData& configData,
                                         const GunnsFluidMultiAdsorberInputData&  inputData,
                                         std::vector<GunnsBasicLink*>&            links,
                                         const int                                port0,
                                         const int                                port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset the initialization complete flag.
    mInitFlag           = false;

    /// - Create the internal and sorption fluids.
    createInternalFluid();
    TS_DELETE_OBJECT(mSorptionFluid);
    TS_NEW_PRIM_OBJECT_EXT(mSorptionFluid, PolyFluid,
                           (*mInternalFluid, std::string(mName) + ".mSorptionFluid"),
                           std::string(mName) + ".mSorptionFluid");
    mSorptionFluid->setFlowRate(0.0);

    /// - Validate the configuration and input data.
    validate(configData, inputData);

    /// - Create the compounds array.
    TS_DELETE_ARRAY(mCompounds);

    /// - Allocate and initialize the compounds array.
    mNCompounds = static_cast<unsigned int>(configData.mCompounds.size());
    TS_NEW_CLASS_ARRAY_EXT(mCompounds, static_cast<int>(mNCompounds), GunnsFluidAdsorptionCompound,
                           (), mName + ".mCompounds");
    for (unsigned int i=0; i<mNCompounds; ++i) {

        /// - For a compound with a dependent type, find the address of that type's own compound.
        GunnsFluidAdsorptionCompound* dependentCompound = 0;
        if (ChemicalCompound::NO_COMPOUND != configData.mCompounds[i].mDependentType) {
            for (unsigned int j=0; j<mNCompounds; ++j) {
                if (configData.mCompounds[j].mType == configData.mCompounds[i].mDependentType) {
                    dependentCompound = &mCompounds[j];
                }
            }
        }

        std::ostringstream stream;
        stream << ".mCompounds_" << i;
        mCompounds[i].initialize(mName + stream.str(), configData.mCompounds[i], mInternalFluid,
                                 dependentCompound);
    }

    /// - Initialize attributes from the validated configuration data.
    mThermalDiameter    = configData.mThermalDiameter;
    mThermalSurfaceArea = UnitConversion::PI_UTIL * configData.mThermalLength * configData.mThermalDiameter;
    if (mThermalSurfaceArea > DBL_EPSILON) {
        mThermalROverD  = configData.mSurfaceRoughness / mThermalDiameter;
    } else {
        mThermalROverD  = 0.0;
    }

    /// - Initialize attributes from the validated input data.
    mWallTemperature     = inputData.mWallTemperature;

    /// - Set the initialization complete flag.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
/// @param[in] inputData  (--) Input data.
///
/// @throws    TsInitializationException
///
/// @details   Validates the initialization of this GUNNS Fluid Multiple Constituent Adsorber link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiAdsorber::validate(const GunnsFluidMultiAdsorberConfigData& configData,
                                       const GunnsFluidMultiAdsorberInputData&  inputData) const
{
    /// - Throw an exception on empty compounds vector.
    if (configData.mCompounds.empty()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Adsorption compounds vector is empty.");
    }

    /// - Throw an exception if a compound has duplicate entries in the compound vector.
    for (unsigned int i=0; i<configData.mCompounds.size(); ++i) {
        for (unsigned int j=i+1; j<configData.mCompounds.size(); ++j) {
            if (configData.mCompounds[i].mType == configData.mCompounds[j].mType) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "Multiple entries for the same compound.");
            }
        }
    }

    /// - Throw an exception if adsorber wall temperature < 0.0.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Adsorber wall temperature < 0.0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiAdsorber::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mSorptionHeat        = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   dt         (s)     Time step.
/// @param[in]   flowRate   (kg/s)  Mass flow rate (not used).
///
/// @return      void
///
/// @details     Update the internal fluids for constituent mass removed by adsorbtion or added by desorbtion.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiAdsorber::updateFluid(const double dt, const double flowRate __attribute__((unused)))
{
    /// - Zero the sorption flow rates and reset the sorption fluids.
    mSorptionFluid->resetState();
    mSorptionHeat = 0.0;

    /// - Skip sorption when the time step is negligible.
    if (dt > DBL_EPSILON) {

        /// - Compute the heat transfer from the fluid to the adsorber, and update the internal fluid
        ///   temperature.
        mWallHeatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mInternalFluid,
                                                                   mFlowRate,
                                                                   mThermalROverD,
                                                                   mThermalDiameter,
                                                                   mThermalSurfaceArea,
                                                                   mWallTemperature);

        unsigned int inletPort = 0;
        unsigned int exitPort  = 1;
        if (mFlowRate < 0.0) {
            inletPort = 1;
            exitPort  = 0;
        }
        const double tAvg = 0.5 * (mNodes[inletPort]->getOutflow()->getTemperature()
                                 + mInternalFluid->getTemperature());
        const double pAvg = 0.5 * (mPotentialVector[0] + mPotentialVector[1]);

        /// - Update & sum sorption rates of all compounds.
        double fluidAdsorptionRate = 0.0;
        for (unsigned int i=0; i<mNCompounds; ++i) {
            mCompounds[i].sorb(dt, tAvg, pAvg, mFlowRate);
            if (not mCompounds[i].isTraceCompound()) {
                fluidAdsorptionRate += mCompounds[i].mAdsorptionRate;
            }
            mSorptionHeat += mCompounds[i].mSorptionHeat;
        }

        /// - Add heat of sorption to the link wall (thermal aspect), which doubles as our sorbant
        ///   material.  Typically, adsorption reactions are exothermic, meaning they heat up the
        ///   sorbant material, and desorptions are endothermic, pulling heat from the material.
        mWallHeatFlux += mSorptionHeat;

        if (fabs(fluidAdsorptionRate) > m100EpsilonLimit) {

            /// - Reset the internal fluid state.
            mSorptionFluid->resetState();

            /// - Update output atmosphere constituents using mass rate instead of mass since it is
            ///   only the mass fractions that are of interest.
            for (unsigned int i=0; i<mNCompounds; ++i) {
                if (not mCompounds[i].isTraceCompound()) {
                    mSorptionFluid->setMass(mCompounds[i].getIndex(), -mCompounds[i].mAdsorptionRate);
                }
            }

            /// - Update output atmosphere mass, moles & fractions from constituent mass.
            mSorptionFluid->updateMass();

            /// - Update trace compounds.
            if (GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds()) {
                for (unsigned int i=0; i<mNCompounds; ++i) {
                    if (mCompounds[i].isTraceCompound()) {
                        const int    index          = mCompounds[i].getIndex();
                        const double inletRate      = tc->getMasses()[index];
                        const double adsorptionRate = mCompounds[i].mAdsorptionRate;
                        tc->setMass(index, inletRate - adsorptionRate);
                    }
                }
                tc->updateMoleFractions();
            }

            /// - Update output atmosphere temperature.
            mSorptionFluid->setTemperature(mInternalFluid->getTemperature());

            /// - Shove sorption fluid into outlet node.
            /// - Add sorption fluid to outlet node and update source vector for flow between
            ///   downstream node and ground.  The source vector will be used next cycle so there
            ///   will be a small error in pressure, but mass will be conserved and GUNNS washes out
            ///   pressure errors. The alternative is to compute reaction in update state using
            ///   previous cycle flowrate, which could result in mass errors, which GUNNS can't fix.
            mNodes[exitPort]->collectInflux(-fluidAdsorptionRate, mSorptionFluid);
            mSourceVector[inletPort] = 0.0;
            mSourceVector[exitPort]  = -fluidAdsorptionRate / mSorptionFluid->getMWeight();
        } else {
            mSourceVector[0] = 0.0;
            mSourceVector[1] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidMultiAdsorber must not map either port to a liquid node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidMultiAdsorber::checkSpecificPortRules(const int port __attribute__((unused)),
                                                     const int node) const
{
    bool result = true;
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);

    /// - Fail if the node is a liquid node.
    if ( (node != getGroundNodeIndex()) and
         (FluidProperties::LIQUID == nodePtr[node].getContent()->getPhase()) ) {
        GUNNS_WARNING("aborted setting a port: cannot assign any port to a liquid node.");
        result = false;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (m2)    New Thermal Surface Area.
///
/// @returns  void
///
/// @details  Sets the thermal surface area of this this GUNNS Fluid Multiple Constituent Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiAdsorber::setThermalSurfaceArea(const double value)
{
    mThermalSurfaceArea = std::max(0.0, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value   (K)    New Wall Temperature.
///
/// @returns  void
///
/// @details  Sets the wall temperature of this this GUNNS Fluid Multiple Constituent Adsorber link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiAdsorber::setWallTemperature(const double value)
{
    mWallTemperature = std::max(0.0, value);
}
