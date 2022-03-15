/**
@file     GunnsFluidSorptionBed.cpp
@brief    GUNNS Sorption Bed Link implementation

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o)
    (core/GunnsFluidUtils.o))
*/

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsFluidUtils.hh"
#include "GunnsFluidSorptionBed.hh"
#include "math/UnitConversion.hh"
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Sorption Bed Sorbate.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSorbate::GunnsFluidSorptionBedSorbate()
    :
    mLoading(0.0),
    mLoadingEquil(0.0),
    mLoadingRate(0.0),
    mLoadingFraction(0.0),
    mAdsorptionRate(0.0),
    mLoadedMass(0.0),
    mProperties(0),
    mFluidIndex(0),
    mBlockingStates()
{
    mBlockingStates.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sorption Bed Sorbate.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSorbate::~GunnsFluidSorptionBedSorbate()
{
    // nothing to do.
}

//TODO
void GunnsFluidSorptionBedSorbate::init(const SorbateProperties*   properties,
                                        const PolyFluidConfigData* fluidConfig,
                                        const double               loading)
{
    //TODO validate & throw
    mProperties = properties;
    mLoading    = loading;

    /// - Store the network fluid index of this sorbate compound.
    mFluidIndex = -1;
    for (int i=0; i<fluidConfig->mNTypes; ++i) {
        if (mProperties->getCompound()->mFluidType == fluidConfig->mTypes[i]) {
            mFluidIndex = i;
            break;
        }
    }
    if (mFluidIndex < 0) {
        //TODO throw: compound isn't in the network fluid
    }
}

//TODO
void GunnsFluidSorptionBedSorbate::registerInteractions(GunnsFluidSorptionBedSorbate* sorbates,
                                                        const unsigned int            nSorbates)
{
    //TODO store pointers to the sorbate states of the blocking compounds...
    for (unsigned int i=0; i<mProperties->getBlockingCompounds()->size(); ++i) {
        const ChemicalCompound::Type compound = mProperties->getBlockingCompounds()->at(i).mCompound;
        for (unsigned int j=0; i<nSorbates; ++j) {
            if (compound == sorbates[j].getProperties()->getCompound()->mType) {
                mBlockingStates.push_back(&sorbates[j]);
                break;
            }
        }
    }
    //TODO throw if no blocking state found - blocking compound isn't in the sorbant
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  pp          (kPa)  Partial pressure of the sorbate in the freestream.
/// @param[in]  temperature (K)    Temperature of the freestream.
///
/// @details  Computes & stores the equilibrium loading of the sorbate under current conditions,
///           using the Toth isotherm equation for this sorbate-sorbant pair.
///
/// @note  The caller must ensure temperature > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO inline or refactor with updateLoading?
void GunnsFluidSorptionBedSorbate::updateLoadingEquil(const double pp, const double temperature)
{
    mLoadingEquil = mProperties->computeLoadingEquil(pp, temperature);
    /// - Include effect of blocking compounds.
    const std::vector<SorbateInteractingCompounds>* blockingCompounds = mProperties->getBlockingCompounds();
    for (unsigned int i=0; i<blockingCompounds->size(); ++i) {
        const double blockingLoadingFraction = 0.0;
        if (mBlockingStates.at(i)->mLoadingEquil > mLoadingEquil) {
            mLoadingEquil *= 1.0 - std::min(blockingLoadingFraction, 1.0) * blockingCompounds->at(i).mInteraction;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timestep  (s)            Integration time step.
/// @param[in]  inFlux    (kg*mol/m3/s)  Flow rate of sorbate in the freestream prior to sorption.
/// @param[in] desorbLimit (kg*mol/m3/s) Limit on desorption rate.
///
/// @details  Computes & stores the current loading rate, limited to the incoming freestream mass
///           (for adsorption) or the loaded mass (for desorption), and integrates it into the
///           updated loading and loading fraction terms.
///
/// @note  The caller must ensure timestep > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSorbate::updateLoading(const double timestep, const double inFlux, const double desorbLimit)
{
    mLoadingRate = mProperties->computeLoadingRate(mLoadingEquil, mLoading);
    if (mLoadingEquil >= mLoading) {
        mLoadingRate = std::min(mLoadingRate, 0.99 * inFlux);  //TODO magic #
    } else {
        mLoadingRate = std::max(std::max(-mLoading / timestep, -desorbLimit), mLoadingRate);
    }
    mLoading += mLoadingRate * timestep;
    mLoadingFraction = mLoading / std::max(mLoadingEquil, DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  volume  (m3)  Volume of the sorbant in the segment.
///
/// @details  Updates the loaded mass from the loading, sorbant volume, and sorbate molecular weight.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSorbate::updateLoadedMass(const double volume)
{
    mLoadedMass = mLoading * volume * mProperties->getCompound()->mMWeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Sorption Bed Segment.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegment::GunnsFluidSorptionBedSegment()
    :
    mSorbates(0),
    mHtc(0.0),
    mVolume(0.0),
    mVolSorbant(0.0),
    mTemperature(0.0),
    mHeatFlux(0.0),
    mThermCap(0.0),
    mFluid(0),
    mNSorbates(0),
    mProperties(0)
{
    // nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sorption Bed Segment.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegment::~GunnsFluidSorptionBedSegment()
{
    TS_DELETE_ARRAY(mSorbates);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO
///
/// @details  Initializes this sorbant segment with given values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSegment::init(const GunnsFluidSorptionBedSegmentConfigData& configData,
                                        const PolyFluidConfigData*                    fluidConfig)
{
    //TODO validate & throw
    mName       = configData.mName;
    mProperties = configData.mProperties;
    mHtc        = configData.mHtc;
    mVolume     = configData.mVolume;
    mVolSorbant = mProperties->computeVolume(configData.mVolume);
    mNSorbates  = mProperties->getSorbates()->size();
    TS_NEW_CLASS_ARRAY_EXT(mSorbates, static_cast<int>(mNSorbates), GunnsFluidSorptionBedSorbate,
                           (), mName + ".mSorbates");
    for (unsigned int i=0; i<mNSorbates; ++i) {
        mSorbates[i].init(&mProperties->getSorbates()->at(i), fluidConfig, 0.0);
    }
    for (unsigned int i=0; i<mNSorbates; ++i) {
        mSorbates[i].registerInteractions(mSorbates, mNSorbates);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out]  flowIn    (kg/s)  Total gas mass flow rate through this segment.
/// @param[in]       pIn       (kPa)   Gas pressure at the inlet to this segment.
/// @param[in]       pOut      (kPa)   Gas pressure at the exit of this segment.
/// @param[in]       timestep  (s)     Integration time step.
///
/// @details  Updates the state of this sorbate segment, computes exit fluid properties and heat
///           transfer with the wall.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSegment::update(double& flow, const double pIn, const double pOut, const double timestep)
{
    /// - Compute heat convection from fluid to sorbant.
    const double Tin      = mFluid->getTemperature();
    const double flowIn   = flow;
    double heatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mFluid,
                                                                 flowIn,
                                                                 mHtc,
                                                                 mTemperature);
    const double Tout     = mFluid->getTemperature();
    const double Tavg     = 0.5 * (Tin + Tout);
    const double ndot     = flowIn / mFluid->getMWeight();

    //TODO this is constant so why are we updating every pass?
    // maybe store as segment state during init...
    double thermCap = mProperties->computeThermalCapacity(mVolSorbant);

    //TODO implement offgassing...

    for (unsigned int i=0; i<mNSorbates; ++i) {
        /// - Update equilibrium loading of sorbates.
        const double pp = pIn * mSorbates[i].getMoleFraction(mFluid);
        mSorbates[i].updateLoadingEquil(pp, Tavg);

        /// - Find mole rates available to absorb from the inlet stream, including both the bulk
        ///   fluid and trace compound.
        const int fluidIndex = mSorbates[i].getFluidIndex();
        const double mdotIn  = flowIn * mFluid->getMassFraction(fluidIndex);
        const double ndotIn  = ndot   * mFluid->getMoleFraction(fluidIndex);

        /// - Find mole rate of desorption that would saturate the exit stream.  This is an
        ///   approximation since the total pressure doesn't include the addition of the desorbed
        ///   fluid and this doesn't account for other sorbates desorbing at the same time.
        const FluidProperties::FluidType fluidType = mSorbates[i].getProperties()->getCompound()->mFluidType;
        const double pSat    = mFluid->getProperties(fluidType)->getSaturationPressure(Tout);
        const double ndotSat = ndot * pSat / std::max(pOut, DBL_EPSILON);
        const double desorbLimit = ndotSat - ndotIn;
        const double adsorbLimit = ndotIn / mVolSorbant;

        /// - Update loading rates (kg*mol/m3/s), bounded by adsorption and desorption rate limits.
        mSorbates[i].updateLoading(timestep, adsorbLimit, desorbLimit / mVolSorbant);
        mSorbates[i].updateLoadedMass(mVolSorbant);
        mSorbates[i].mAdsorptionRate = mSorbates[i].mLoadingRate * mVolSorbant;//TODO move to BedSorbate class function

        /// - Update the bulk fluid flow with the adsorption/desorption flow rate for input to the next
        ///   segment.
        const double dmdotIn = mSorbates[i].mAdsorptionRate
                             * mSorbates[i].getProperties()->getCompound()->mMWeight;
        flow -= dmdotIn;
        mFluid->setMass(fluidIndex, mdotIn - dmdotIn);

        /// - Add heats of sorption to the wall heat flux for output to the thermal aspect.
        ///   Units: (kg*mol/m3/s) * (m3) * (kJ/g*mol) * (1000 g/kg) * (1000 J/kJ) = (J/s)
        ///   For consistency with other thermal aspects, sign on mHeatFlux is positive for flux to the
        ///   thermal aspect (exothermic), so we must flip sign on the mDh terms.
        //TODO sign flip moved to SorbateProperties
        heatFlux += mSorbates[i].getProperties()->computeHeatFlux(mSorbates[i].mAdsorptionRate);

        /// - Update total thermal capacity of sorbant + sorbates for output to the thermal aspect.
        ///   Sorbate loading can significantly change the thermal capacity of the combined sorbant +
        ///   sorbates mass, for absorbing future heat flux.
        const double cp = mFluid->getProperties(fluidType)->getSpecificHeat(mTemperature);
        thermCap += cp * mSorbates[i].mLoadedMass;
    }

    mFluid->updateMass();
    mHeatFlux = heatFlux;
    mThermCap = thermCap;
}

//TODO inline
unsigned int GunnsFluidSorptionBedSegment::getNSorbates() const
{
    return mNSorbates;
}

//TODO
GunnsFluidSorptionBedSegmentConfigData::GunnsFluidSorptionBedSegmentConfigData(
        const std::string&       name,
        const SorbantProperties* sorbant,
        const double             volume,
        const double             htc)
    :
    mName(name),
    mProperties(sorbant),
    mVolume(volume),
    mHtc(htc)
{
    // nothing to do
}

//TODO
GunnsFluidSorptionBedSegmentConfigData::~GunnsFluidSorptionBedSegmentConfigData()
{
    // nothing to do
}

//TODO
GunnsFluidSorptionBedSegmentConfigData::GunnsFluidSorptionBedSegmentConfigData(const GunnsFluidSorptionBedSegmentConfigData& that)
    :
    mName(that.mName),
    mProperties(that.mProperties),
    mVolume(that.mVolume),
    mHtc(that.mHtc)
{
    // nothing to do
}

//TODO doesn't assignin anything as all members are const
GunnsFluidSorptionBedSegmentConfigData& GunnsFluidSorptionBedSegmentConfigData::operator =(const GunnsFluidSorptionBedSegmentConfigData& that)
{
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                 (--) Name of object.
/// @param[in,out] nodes                (--) Pointer to nodes.
/// @param[in]     maxConductivity      (m2) Maximum conductivity.
///
/// @details  Default constructs this GUNNS Fluid Sorption Bed link config data.  The mSegments
///           vector is empty and must be populated with segment configurations after this
///           constructor is called and before the GunnsFluidSorptionBed's iniitalize is called.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedConfigData::GunnsFluidSorptionBedConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       maxConductivity)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, 0.0),
    mSegments(),
    mDefinedSorbants(0)
{
    mSegments.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sorption Bed link config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedConfigData::~GunnsFluidSorptionBedConfigData()
{
    delete mDefinedSorbants;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] density  (kg/m3)  Density of the sorbant material.
/// @param[in] porosity (--)     Fraction of the packed sorbant enclosure volume that is voids.
/// @param[in] cp       (J/kg/K) Specific heat of the sorbant material.
///
/// @returns  SorbantProperties* (--) Pointer to the new sorbant properties.
///
/// @details  Adds a new custom sorbant type to this bed with given density, porosity and specific
///           head, and returns its address.  This sorbant has no sorbates yet, and the user must
///           add them.
////////////////////////////////////////////////////////////////////////////////////////////////////
SorbantProperties* GunnsFluidSorptionBedConfigData::addCustomSorbant(const double density,
                                                                     const double porosity,
                                                                     const double cp)
{
    SorbantProperties newSorbant(SorbantProperties::CUSTOM, density, porosity, cp);
    mCustomSorbants.push_back(newSorbant);
    return &mCustomSorbants.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] definedType (--)  Defined type of sorbant in the segment.
/// @param[in] volume      (m3)  Total volume of the segment including sorbant and voids.
/// @param[in] htc         (W/K) Convective heat transfer coefficient of the segment.
///
/// @throws   TsInitializationException
///
/// @details  This is called from the input file to add a segment and configure its sorbant
///           properties, volume, and heat transfer coefficient.  This overloaded function accepts a
///           defined sorbant type.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedConfigData::addSegment(const SorbantProperties::Type definedType,
                                                 const double volume, const double htc)
{
    if (SorbantProperties::CUSTOM == definedType) {
        //TODO note that H&S doesn't work because this is pre-init
        //TODO throw, use the other overloaded method
    }
    if (not mDefinedSorbants) {
        mDefinedSorbants = new DefinedSorbantProperties();
    }
    std::ostringstream name;
    name << mName << ".mSegments_" << mSegments.size();
    const SorbantProperties* properties = mDefinedSorbants->getSorbant(definedType);
    GunnsFluidSorptionBedSegmentConfigData newSegment(name.str(), properties, volume, htc);
    mSegments.push_back(newSegment);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] customType (--)  Pointer to the custom sorbant properties for this segment.
/// @param[in] volume     (m3)  Total volume of the segment including sorbant and voids.
/// @param[in] htc        (W/K) Convective heat transfer coefficient of the segment.
///
/// @throws   TsInitializationException
///
/// @details  This is called from the input file to add a segment and configure its sorbant
///           properties, volume, and heat transfer coefficient.  This overloaded function accepts a
///           pointer to one of this bed's custom sorbant properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedConfigData::addSegment(const SorbantProperties* customType,
                                                 const double volume, const double htc)
{
    const SorbantProperties* foundProperties = 0;
    for (unsigned int i=0; i<mCustomSorbants.size(); ++i) {
        if (customType == &mCustomSorbants.at(i)) {
            foundProperties = customType;
        }
    }
    if (customType != foundProperties) {
        //TODO note that H&S doesn't work because this is pre-init
        //TODO throw
    }
    std::ostringstream name;
    name << mName << ".mSegments_" << mSegments.size();
    GunnsFluidSorptionBedSegmentConfigData newSegment(name.str(), foundProperties, volume, htc);
    mSegments.push_back(newSegment);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag     (--)  Blockage malfunction flag.
/// @param[in]  malfBlockageValue    (--)  Blockage malfunction fractional value (0-1).
/// @param[in]  wallTemperature      (K)   Tube wall temperature for thermal convection.
///
/// @details  Default constructs this GUNNS Fluid Sorption Bed link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedInputData::GunnsFluidSorptionBedInputData(const bool   malfBlockageFlag,
                                                                   const double malfBlockageValue,
                                                                   const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sorption Bed link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedInputData::~GunnsFluidSorptionBedInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Sorption Bed link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBed::GunnsFluidSorptionBed()
    :
    GunnsFluidConductor(),
    mSegments(0),
    mNSegments(0),
    mVolume(0.0),
    mAdsorptionRates(0),
    mAdsorbedMasses(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sorption Bed link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBed::~GunnsFluidSorptionBed()
{
    TS_DELETE_ARRAY(mAdsorbedMasses);
    TS_DELETE_ARRAY(mAdsorptionRates);
    TS_DELETE_ARRAY(mSegments);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @throws      TsInitializationException
///
/// @details     Initializes this GUNNS Fluid Sorption Bed link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBed::initialize(const GunnsFluidSorptionBedConfigData& configData,
                                       const GunnsFluidSorptionBedInputData&  inputData,
                                       std::vector<GunnsBasicLink*>&          links,
                                       const int                              port0,
                                       const int                              port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset the initialization complete flag.
    mInitFlag           = false;

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Validate the configuration and input data.
    validate(configData, inputData);

    /// - Initialize attributes from the validated configuration data and finish initialization of
    ///   the segments.
    mNSegments = configData.mSegments.size();
    TS_NEW_CLASS_ARRAY_EXT(mSegments, static_cast<int>(mNSegments), GunnsFluidSorptionBedSegment,
                           (), mName + ".mSegments");
    for (unsigned int i=0; i<mNSegments; ++i) {
        mSegments[i].init(configData.mSegments.at(i), mNodes[0]->getFluidConfig());
        mVolume += mSegments[i].mVolume;
        mSegments[i].mTemperature = inputData.mWallTemperature;
        mSegments[i].mFluid = mInternalFluid;
    }

    const int nFluids = mNodes[0]->getFluidConfig()->mNTypes;
    TS_NEW_PRIM_ARRAY_EXT(mAdsorptionRates, nFluids, double, mName + ".mAdsorptionRates");
    TS_NEW_PRIM_ARRAY_EXT(mAdsorbedMasses,  nFluids, double, mName + ".mAdsorbedMasses");
    for (int i=0; i<nFluids; ++i) {
        mAdsorptionRates[i] = 0.0;
        mAdsorbedMasses[i]  = 0.0;
    }

    /// - Set the initialization complete flag.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Configuration data.
/// @param[in] inputData  (--) Input data.
///
/// @throws    TsInitializationException
///
/// @details   Validates the initialization of this GUNNS Fluid Sorption Bed link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBed::validate(const GunnsFluidSorptionBedConfigData& configData,
                                      const GunnsFluidSorptionBedInputData&  inputData) const
{
    /// - Throw an exception if GUNNS_H2O is not in the network.
    try {
        mInternalFluid->find(FluidProperties::GUNNS_H2O);
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "GUNNS_H2O is not present in the network.");
    }

    /// - Throw an exception if GUNNS_CO2 is not in the network.
    try {
        mInternalFluid->find(FluidProperties::GUNNS_CO2);
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "GUNNS_CO2 is not present in the network.");
    }

    /// - Throw an exception if adsorber wall temperature < 0.0.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Adsorber wall temperature < 0.0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBed::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
//    mSorptionHeat        = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details    Update the internal fluids for constituent mass removed by adsorbtion or added by desorbtion.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBed::computeFlows(const double dt)
{
    mPotentialDrop = getDeltaPotential();

    /// - Compute molar flow rate.
    computeFlux();

    /// - Convert the molar flow rate to mass rate using the molecular weight of the source node.
    int    sourcePort    = determineSourcePort(mFlux, 0, 1);
    double sourceDensity =         mNodes[sourcePort]->getContent()->getDensity();
    mFlowRate            = mFlux * mNodes[sourcePort]->getContent()->getMWeight();

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   source node.
    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power.
    computePower();

    /// - Skip sorption when the time step is negligible.
    const int nFluids = mNodes[0]->getFluidConfig()->mNTypes;
    for (int i=0; i<nFluids; ++i) {
        mAdsorptionRates[i] = 0.0;
        mAdsorbedMasses[i]  = 0.0;
    }
    double totalAdsorptionRate = 0.0;

    if (dt > DBL_EPSILON) {

        const double dPoverV = fabs(mPotentialDrop) / mVolume;
        double segFlow = fabs(mFlowRate);

        if (mFlowRate > 0.0) {
            mInternalFluid->setState(mNodes[0]->getContent());
            for (int i=0; i<nFluids; ++i) {
                mInternalFluid->setMass(i, mNodes[0]->getContent()->getMassFraction(i) * segFlow);
            }
            double segP = mPotentialVector[0];
            for (unsigned int i=0; i<mNSegments; ++i) {
                const double nextSegP = segP - dPoverV * mSegments[i].mVolume;
                /// - Update segments in order of flow direction.
                mSegments[i].update(segFlow, segP, nextSegP, dt);

                /// - Estimate total pressure in between segments, for inlet pressure to each
                ///   segment, as interpolated between the node pressures by relative segment length
                ///   (volume, assuming constant area).  We assume affect of adsorb/desorbed sorbates
                ///   on these intermediate pressures is negligible.
                segP = nextSegP;
                for (unsigned int j=0; j<mSegments[i].getNSorbates(); ++j) {
                    const GunnsFluidSorptionBedSorbate& sorbate = mSegments[i].mSorbates[j];
                    mAdsorptionRates[sorbate.getFluidIndex()] += sorbate.mAdsorptionRate;
                    totalAdsorptionRate += sorbate.mAdsorptionRate;
                }
            }

        } else if (mFlowRate < 0.0) {
            mInternalFluid->setState(mNodes[1]->getContent());
            for (int i=0; i<nFluids; ++i) {
                mInternalFluid->setMass(i, mNodes[1]->getContent()->getMassFraction(i) * segFlow);
            }
            double segP = mPotentialVector[1];
            for (unsigned int i=mNSegments-1; i>=0; --i) {
                const double nextSegP = segP - dPoverV * mSegments[i].mVolume;
                mSegments[i].update(segFlow, segP, nextSegP, dt);
                segP = nextSegP;
                //TODO refactor with above, DRY
                for (unsigned int j=0; j<mSegments[i].getNSorbates(); ++j) {
                    const GunnsFluidSorptionBedSorbate& sorbate = mSegments[i].mSorbates[j];
                    mAdsorptionRates[sorbate.getFluidIndex()] += sorbate.mAdsorptionRate;
                    totalAdsorptionRate += sorbate.mAdsorptionRate;
                }
            }
        }

        /// - Transport flow between nodes and correct the source vector for adsorption.
        if (mFlowRate > 0.0) {
            mNodes[0]->collectOutflux(mFlowRate);
            mNodes[1]->collectInflux(segFlow, mInternalFluid);
            mSourceVector[0] = 0.0;
            mSourceVector[1] = totalAdsorptionRate;
;
        } else if (mFlowRate < 0.0) {
            mNodes[1]->collectOutflux(-mFlowRate);
            mNodes[0]->collectInflux(segFlow, mInternalFluid);
            mSourceVector[0] = totalAdsorptionRate;
            mSourceVector[1] = 0.0;
        }

        /// - Convert absorbed sorbate mole rate into mass rate.
        for (int i=0; i<nFluids; ++i) {
            mAdsorptionRates[i] *= mInternalFluid->getProperties(mInternalFluid->getType(i))->getMWeight();
        }

        /// - Update total adsorbed masses for display.
        for (unsigned int i=0; i<mNSegments; ++i) {
            for (unsigned int j=0; j<mSegments[i].getNSorbates(); ++j) {
                const GunnsFluidSorptionBedSorbate& sorbate = mSegments[i].mSorbates[j];
                mAdsorbedMasses[sorbate.getFluidIndex()] += sorbate.mLoadedMass;
            }
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
///           - A GunnsFluidSorptionBed must not map either port to a liquid node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidSorptionBed::checkSpecificPortRules(const int port, const int node) const
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
