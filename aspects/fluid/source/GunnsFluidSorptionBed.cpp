/**
@file     GunnsFluidSorptionBed.cpp
@brief    GUNNS Sorption Bed Link implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o)
    (core/GunnsFluidUtils.o))
*/

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "core/GunnsFluidUtils.hh"
#include "GunnsFluidSorptionBed.hh"
#include "math/UnitConversion.hh"
#include <sstream>

/// @details  This value is chosen to avoid adsorbing all of the sorbate mass out of the flow
///           stream, for stability.
const float GunnsFluidSorptionBedSorbate::mLimitAdsorbFraction = 0.99;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Sorption Bed Sorbate.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSorbate::GunnsFluidSorptionBedSorbate()
    :
    mMalfLoadingEquilFlag(false),
    mMalfLoadingEquilValue(0.0),
    mLoading(0.0),
    mLoadingEquil(0.0),
    mLoadingRate(0.0),
    mLoadingFraction(0.0),
    mAdsorptionRate(0.0),
    mLoadedMass(0.0),
    mProperties(0),
    mFluidIndexes(),
    mBlockingStates(),
    mBlockingCompoundIndex(),
    mOffgasIndexes()
{
    mBlockingStates.clear();
    mBlockingCompoundIndex.clear();
    mOffgasIndexes.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Sorption Bed Sorbate.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSorbate::~GunnsFluidSorptionBedSorbate()
{
    // nothing to do.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] properties  (--)        Pointer to the sorbate properties object.
/// @param[in] fluidIndex  (--)        Index of this sorbate in the fluid constituents.
/// @param[in] tcIndex     (--)        Index of this sorbate in the trace compounds.
/// @param[in] loading     (kg*mol/m3) Initial sorbate loading.
/// @param[in] volume      (m3)        Effective volume of sorbant.
/// @param[in] fluid       (--)        Pointer to the link internal fluid.
///
/// @details  Initializes this sorption bed sorbate.  Caller must ensure that the initial loading
///           value is >= 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSorbate::init(const SorbateProperties* properties,
                                        const int                fluidIndex,
                                        const int                tcIndex,
                                        const double             loading,
                                        const double             volume,
                                        const PolyFluid*         fluid)
{
    mProperties          = properties;
    mFluidIndexes.mFluid = fluidIndex;
    mFluidIndexes.mTc    = tcIndex;
    mLoading             = loading;
    //TODO what about initial TC mass?
    updateLoadedMass(volume);

    /// - Store the fluid indexes for the offgas compounds.
    const std::vector<SorbateInteractingCompounds>* sorbateOffgasCompounds = mProperties->getOffgasCompounds();
    if (sorbateOffgasCompounds->size() > 0) {
        DefinedChemicalCompounds definedChems;
        for (unsigned int i=0; i<sorbateOffgasCompounds->size(); ++i) {
            const ChemicalCompound* chemProps = definedChems.getCompound(sorbateOffgasCompounds->at(i).mCompound);
            mOffgasIndexes.push_back(GunnsFluidSorptionBedFluidIndex());
            fluid->findCompound(mOffgasIndexes.back().mFluid,
                                mOffgasIndexes.back().mTc, chemProps);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] sorbates  (--) Array of all sorbate types in the sorption bed.
/// @param[in] nSorbates (--) Size of the sorbates array.
///
/// @details  Stores pointers to the other sorbates that interact with this sorbate in the bed as
///           blocking compounds, so that we can reference them and compute their effect in
///           updateLoadingEquil.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSorbate::registerInteractions(GunnsFluidSorptionBedSorbate* sorbates,
                                                        const unsigned int            nSorbates)
{
    for (unsigned int i=0; i<mProperties->getBlockingCompounds()->size(); ++i) {
        const ChemicalCompound::Type compound = mProperties->getBlockingCompounds()->at(i).mCompound;
        for (unsigned int j=0; j<nSorbates; ++j) {
            if (compound == sorbates[j].getProperties()->getCompound()->mType) {
                mBlockingStates.push_back(&sorbates[j]);
                mBlockingCompoundIndex.push_back(i);
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  pp          (kPa)  Partial pressure of the sorbate in the freestream.
/// @param[in]  temperature (K)    Temperature of the freestream.
///
/// @details  Computes & stores the equilibrium loading of the sorbate under current conditions,
///           using the Toth isotherm equation for this sorbate-sorbant pair, and including the
///           effect of blocking compounds.
///
/// @note  The caller must ensure temperature > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSorbate::updateLoadingEquil(const double pp, const double temperature)
{
    mLoadingEquil = mProperties->computeLoadingEquil(pp, temperature);
    const std::vector<SorbateInteractingCompounds>* blockingCompounds = mProperties->getBlockingCompounds();
    for (unsigned int i=0; i<mBlockingStates.size(); ++i) {
        /// - Each interacting compound scales the loading equilibrium of this compound.  Blocking
        ///   compounds block by how much they are present.  Co-absorption compounds block by how
        ///   much they are NOT present.
        double interaction = blockingCompounds->at(mBlockingCompoundIndex.at(i)).mInteraction;
        if (interaction >= 0.0) {
            interaction *= mBlockingStates.at(i)->mLoadingFraction;
        } else {
            interaction *= (mBlockingStates.at(i)->mLoadingFraction - 1.0);
        }
        mLoadingEquil *= (1.0 - MsMath::limitRange(0.0, interaction, 1.0));
    }
    if (mMalfLoadingEquilFlag) {
        mLoadingEquil *= fmax(0.0, mMalfLoadingEquilValue);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] timestep    (s)           Integration time step.
/// @param[in] inFlux      (kg*mol/m3/s) Flow rate of sorbate in the freestream prior to sorption.
/// @param[in] desorbLimit (kg*mol/m3/s) Limit on desorption rate, as positive value > 0.
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
    if (mLoadingRate >= 0.0) {
        mLoadingRate = fmin(mLoadingRate, mLimitAdsorbFraction * inFlux);
    } else {
        /// - Note that the desorbLimit is given as a posiive value, but mLoadingRate is negative
        ///   for desorption, so we flip the sign on desorbLimit inside here.
        mLoadingRate = fmax(fmax(-mLoading / timestep, -desorbLimit), mLoadingRate);
    }
    mLoading = fmax(0.0, mLoading + mLoadingRate * timestep);
    mLoadingFraction = mLoading / fmax(mLoadingEquil, DBL_EPSILON);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluid (--) Pointer to the fluid object.
///
/// @returns  double (--) Mole fraction of this sorbate in the given fluid.
///
/// @details  The sorbate can exist in the network as a bulk fluid and/or a trace compound.  This
///           sums both the bulk fluid's and the trace compound's mole fractions, whichever are
///           present.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidSorptionBedSorbate::getMoleFraction(const PolyFluid* fluid) const
{
    double result = 0.0;
    if (mFluidIndexes.mFluid >= 0) {
        result = fluid->getMoleFraction(mFluidIndexes.mFluid);
    }
    if (mFluidIndexes.mTc >= 0) {
        result += fluid->getTraceCompounds()->getMoleFractions()[mFluidIndexes.mTc];
    }

    /// - Note that it is possible for this result to be > 1 if the compound is both a bulk fluid
    ///   and trace compound.  This will cause many issues throughout the network that we can't
    ///   prevent by limiting the result here, so we don't bother with limiting.
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  volume  (m3)  Volume of sorbing sorbant in the segment.
///
/// @details  Updates the loaded mass from the loading, sorbant volume, and sorbate molecular weight
///           and updates the adsorption rate from the loading rate and sorbant volume.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSorbate::updateLoadedMass(const double volume)
{
    mLoadedMass = mLoading * volume * mProperties->getCompound()->mMWeight;
    mAdsorptionRate = mLoadingRate * volume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Resets non-checkpointed and non-configuration data values post-checkpoint load.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSorbate::restartModel()
{
    mLoadingEquil    = 0.0;
    mLoadingRate     = 0.0;
    mLoadingFraction = 0.0;
    mAdsorptionRate  = 0.0;
    mLoadedMass      = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name    (--)  Reference to the segment object name.
/// @param[in] sorbant (--)  Pointer to the Sorbant Properties in this segment.
/// @param[in] volume  (m3)  Total volume of this segment including sorbant material and voids.
/// @param[in] htc     (W/K) Convective heat transfer coefficient between the sorbant and the fluid stream.
///
/// @details  Constructs this Sorption Bed Segment Configuration Data with arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegmentConfigData::GunnsFluidSorptionBedSegmentConfigData(
        const std::string&       name,
        const SorbantProperties* sorbant,
        const double             volume,
        const double              htc)
    :
    mName(name),
    mProperties(sorbant),
    mVolume(volume),
    mHtc(htc)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this Sorption Bed Segment Configuration Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegmentConfigData::~GunnsFluidSorptionBedSegmentConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be copied.
///
/// @details  Copy constructs this Sorption Bed Segment Configuration Data from the given object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegmentConfigData::GunnsFluidSorptionBedSegmentConfigData(const GunnsFluidSorptionBedSegmentConfigData& that)
    :
    mName(that.mName),
    mProperties(that.mProperties),
    mVolume(that.mVolume),
    mHtc(that.mHtc)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be copied.
///
/// @details  This doesn't assign anything since all class member are const.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegmentConfigData& GunnsFluidSorptionBedSegmentConfigData::operator =(const GunnsFluidSorptionBedSegmentConfigData& that)
{
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] segment (--)        The segment number to load.
/// @param[in] sorbate (--)        The sorbate compound type to load.
/// @param[in] loading (kg*mol/m3) The initial loading amount.
///
/// @details  Constructs this Sorption Bed Segment Input Data with arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegmentInputData::GunnsFluidSorptionBedSegmentInputData(
        const unsigned int           segment,
        const ChemicalCompound::Type sorbate,
        const double                 loading)
    :
    mSegment(segment),
    mSorbate(sorbate),
    mLoading(loading)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this Sorption Bed Segment Input Data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegmentInputData::~GunnsFluidSorptionBedSegmentInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be copied.
///
/// @details  Copy constructs this Sorption Bed Segment Input Data from the given object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegmentInputData::GunnsFluidSorptionBedSegmentInputData(const GunnsFluidSorptionBedSegmentInputData& that)
    :
    mSegment(that.mSegment),
    mSorbate(that.mSorbate),
    mLoading(that.mLoading)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Reference to the object to be copied.
///
/// @details  This doesn't assign anything since all class member are const.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegmentInputData& GunnsFluidSorptionBedSegmentInputData::operator =(const GunnsFluidSorptionBedSegmentInputData& that)
{
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Sorption Bed Segment.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBedSegment::GunnsFluidSorptionBedSegment()
    :
    mMalfDegradeFlag(false),
    mMalfDegradeValue(0.0),
    mSorbates(0),
    mVolume(0.0),
    mTemperature(0.0),
    mHeatFlux(0.0),
    mThermCap(0.0),
    mFluid(0),
    mNSorbates(0),
    mProperties(0),
    mHtc(0.0),
    mVolSorbant(0.0)
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
/// @param[in] configData  (--) Reference to the configuration data for this bed segment.
/// @param[in] fluidConfig (--) Pointer to the network fluid configuration.
/// @param[in] loading     (--) Reference to vector of initial sorbate loadings.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this sorbant segment with given values.  Sorbates are optional in the
///           network.  They can either be present as bulk fluid types, trace compounds or both,
///           or they can be missing entirely if they're not desired to be modeled.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSegment::init(const GunnsFluidSorptionBedSegmentConfigData&             configData,
                                        const PolyFluidConfigData*                                fluidConfig,
                                        const std::vector<GunnsFluidSorptionBedSegmentInputData>& loading)
{
    /// - Initialize from configuration data.
    GUNNS_NAME_ERREX("GunnsFluidSorptionBedSegment", configData.mName);
    mProperties = configData.mProperties;
    mHtc        = configData.mHtc;
    mVolume     = configData.mVolume;
    mVolSorbant = mProperties->computeVolume(configData.mVolume);

    /// - The SorbantProperties for the sorbant in this bed define the list of possible sorbates,
    ///   but these are optional in the network.  Find the sorbates that are present in the network
    ///   as bulk fluids and/or trace compounds.  For those that exist in the network, store their
    ///   SorbateProperties, and their indexes in the network fluid and trace compounds.
    std::vector<int> sorbateIndexes;
    std::vector<int> fluidIndexes;
    std::vector<int> tcIndexes;
    for (unsigned int i=0; i<mProperties->getSorbates()->size(); ++i) {
        int fluidIndex = -1;
        int tcIndex    = -1;
        mFluid->findCompound(fluidIndex, tcIndex, mProperties->getSorbates()->at(i).getCompound());

        /// - Record the indexes of the sorbate in the fluid and trace compounds.
        if (fluidIndex >= 0 or tcIndex >= 0) {
            sorbateIndexes.push_back(i);
            fluidIndexes.push_back(fluidIndex);
            tcIndexes.push_back(tcIndex);
        }
    }

    /// - Allocate the sorbate states array and initialize the sorbate states.
    mNSorbates = sorbateIndexes.size();
    TS_NEW_CLASS_ARRAY_EXT(mSorbates, static_cast<int>(mNSorbates), GunnsFluidSorptionBedSorbate,
                           (), mName + ".mSorbates");

    double effectiveSorbantVol = computeEffectiveSorbantVolume();
    for (unsigned int i=0; i<mNSorbates; ++i) {
        /// - Find initial loading of this sorbate in the loading input data.
        double load = 0.0;
        for (unsigned int j=0; j<loading.size(); ++j) {
            if (loading.at(j).mSorbate == mProperties->getSorbates()->at(sorbateIndexes[i]).getCompound()->mType) {
                load = loading.at(j).mLoading;
            }
        }
        mSorbates[i].init(&mProperties->getSorbates()->at(sorbateIndexes[i]),
                          fluidIndexes[i], tcIndexes[i], load, effectiveSorbantVol, mFluid);
    }

    /// - Initialize sorbate interactions.
    for (unsigned int i=0; i<mNSorbates; ++i) {
        mSorbates[i].registerInteractions(mSorbates, mNSorbates);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in, out] flowIn   (kg/s) Total gas mass flow rate through this segment, always >= 0.
/// @param[in]      pIn      (kPa)  Gas pressure at the inlet to this segment.
/// @param[in]      pOut     (kPa)  Gas pressure at the exit of this segment.
/// @param[in]      timestep (s)    Integration time step.
///
/// @details  Updates the state of this sorbate segment, computes exit fluid properties and heat
///           transfer with the wall.
//TODO break this up, get the cyclo complex down...
//TODO note design limitation on desorption of trace compounds.  Because we can't create bulk flow
//     when only desorbing trace compounds, mass isn't conserved.  Workaround is to have the user
//     create a small leak flow from the pressurized cabin side through this link, by using a
//     selective membrane link allowing N2 or some other inert fluid relative to our sorbants, to
//     create a small bulk flow to allow TC's to desorb into.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBedSegment::update(double& flow, const double pIn, const double pOut, const double timestep)
{
    /// - Compute heat convection from fluid to sorbant.
    const double Tin      = mFluid->getTemperature();
    const double flowIn   = flow;
    double       heatFlux = GunnsFluidUtils::computeConvectiveHeatFlux(mFluid,
                                                                       flowIn,
                                                                       mHtc,
                                                                       mTemperature);
    const double Tout     = mFluid->getTemperature();
    const double Tavg     = 0.5 * (Tin + Tout);
    const double ndot     = flowIn / mFluid->getMWeight();
    double       thermCap = mProperties->computeThermalCapacity(mVolSorbant);

    for (unsigned int i=0; i<mNSorbates; ++i) {
        /// - Update equilibrium loading of sorbates.
        const double pp = pIn * mSorbates[i].getMoleFraction(mFluid);
        mSorbates[i].updateLoadingEquil(pp, Tavg);

        /// - Find mole rates available to absorb from the inlet stream, including both the bulk
        ///   fluid and trace compound.
        const double molWeight  = mSorbates[i].getProperties()->getCompound()->mMWeight;
        const int    fluidIndex = mSorbates[i].getFluidIndexes()->mFluid;
        const int    tcIndex    = mSorbates[i].getFluidIndexes()->mTc;
        double       ndotInTc   = 0.0;
        double       ndotIn     = 0.0;
        if (fluidIndex >= 0) {
            ndotIn += ndot * mFluid->getMoleFraction(fluidIndex);
        }
        const double mdotInBulk = ndotIn * molWeight;
        if (tcIndex >= 0) {
            ndotInTc = ndot * mFluid->getTraceCompounds()->getMoleFractions()[tcIndex];
            ndotIn += ndotInTc;
        }

        /// - Find mole rate of desorption that would saturate the exit stream.  This is an
        ///   approximation since the total pressure doesn't include the addition of the desorbed
        ///   fluid and this doesn't account for other sorbates desorbing at the same time.
        ///   We only do this for sorbates that are bulk fluids, since for trace compounds we do
        ///   not model fluid states such as phase or saturation.
        double desorbLimit = FLT_MAX; // use this number as an arbitrarily large limit
        const FluidProperties::FluidType fluidType = mSorbates[i].getProperties()->getCompound()->mFluidType;
        if (fluidIndex >= 0) {
            const double pSat    = mFluid->getProperties(fluidType)->getSaturationPressure(Tout);
            const double ndotSat = ndot * pSat / fmax(pOut, DBL_EPSILON);
            desorbLimit = fmax(0.0, ndotSat - ndotIn);
        }
        const double adsorbLimit = ndotIn / mVolSorbant;

        /// - Update loading rates (kg*mol/m3/s), bounded by adsorption and desorption rate limits.
        mSorbates[i].updateLoading(timestep, adsorbLimit, desorbLimit / mVolSorbant);
        mSorbates[i].updateLoadedMass(computeEffectiveSorbantVolume());

        /// - Update the bulk fluid flow with the adsorption/desorption flow rate for input to the next
        ///   segment.
        flow += exchangeFluid(fluidIndex, tcIndex, mdotInBulk, ndotInTc, -mSorbates[i].mAdsorptionRate, molWeight);

        /// - Compute offgassing produced by adsorption of sorbates, and add the offgas compounds to the
        ///   exit fluid stream.
        if (mSorbates[i].mAdsorptionRate > DBL_EPSILON) {
            const std::vector<SorbateInteractingCompounds>*     offgasCompounds = mSorbates[i].getProperties()->getOffgasCompounds();
            const std::vector<GunnsFluidSorptionBedFluidIndex>* offgasIndexes   = mSorbates[i].getOffgasIndexes();
            for (unsigned int j=0; j<offgasCompounds->size(); ++j) {
                const int offgasIndexBulk = offgasIndexes->at(j).mFluid;
                const int offgasIndexTc   = offgasIndexes->at(j).mTc;
                if (offgasIndexBulk >= 0 or offgasIndexTc >= 0) {
                    const double offgasRate = mSorbates[i].mAdsorptionRate * offgasCompounds->at(j).mInteraction;
                    if (offgasRate >= DBL_EPSILON) {
                        double offgasBulkMdotIn = 0.0;
                        double offgasTcNdotIn   = 0.0;
                        double offgasMolWeight  = 0.0;
                        if (offgasIndexBulk >= 0) {
                            offgasBulkMdotIn = flowIn * mFluid->getMassFraction(offgasIndexBulk);
                            offgasMolWeight  = mFluid->getProperties(mFluid->getType(offgasIndexBulk))->getMWeight();
                        }
                        if (offgasIndexTc >= 0) {
                            offgasTcNdotIn  = ndot * mFluid->getTraceCompounds()->getMoleFractions()[offgasIndexTc];
                            offgasMolWeight = mFluid->getTraceCompounds()->getConfig()->mCompounds.at(offgasIndexTc)->mMWeight;
                        }
                        flow += exchangeFluid(offgasIndexBulk, offgasIndexTc, offgasBulkMdotIn,
                                              offgasTcNdotIn, offgasRate, offgasMolWeight);
                    }
                }
            }
        }

        /// - Add heats of sorption to the wall heat flux for output to the thermal aspect.  The
        ///   sorbate properties returns the heat flux sign as positive for exothermic.
        heatFlux += mSorbates[i].computeHeatFlux();

        /// - Update total thermal capacity of sorbant + sorbates for output to the thermal aspect.
        ///   Sorbate loading can significantly change the thermal capacity of the combined sorbant +
        ///   sorbates mass, for absorbing future heat flux.  We only do this for sorbates that are
        ///   bulk fluids, since we don't model thermal capacity for trace compounds.
        if (fluidIndex >= 0) {
            const double cp = mFluid->getProperties(fluidType)->getSpecificHeat(mTemperature);
            thermCap += cp * mSorbates[i].mLoadedMass;
        }
    }

    /// - Update bulk fluid and trace compounds constituent mixture fractions in the exit stream.
    mFluid->updateMass();
    mHeatFlux = heatFlux;
    mThermCap = thermCap;
    GunnsFluidTraceCompounds* tc = mFluid->getTraceCompounds();
    if (tc) {
        tc->updateMoleFractions();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluidIndex (--)       Index of the compound in the network bulk fluids.
/// @param[in] tcIndex    (--)       Index of the compound in the trace compounds.
/// @param[in] mdotInBulk (kg/s)     Incoming sorbate bulk fluid mass flow rate before exchange.
/// @param[in] ndotInTc   (kg*mol/s) Incoming sorbate trace compound mole flow rate before exchange.
/// @param[in] dndot      (kg*mol/s) Mole rate of sorbate added to the fluid.
/// @param[in] molWeight  (1/mol)    Molecular weight of the compound.
///
/// @returns  double (m/s) mass rate added (desorbed) to the bulk fluid.
///
/// @details  Updates the compound mass in mFluid.  Only the masses of the bulk fluid constituents
///           and trace compounds are set here, and their mixture fractions must be updated later.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidSorptionBedSegment::exchangeFluid(const int fluidIndex, const int tcIndex,
                                                   const double mdotInBulk, const double ndotInTc,
                                                   const double dndot, const double molWeight)
{
    double mdotBulkDesorb = 0.0;
    double ndotOutTc      = ndotInTc;

    if (dndot < -DBL_EPSILON) {
        /// - Adsorption from fluid.  If there is incoming trace compound, adsorb from it first,
        ///   then any remainder from the bulk fluid.
        const double tcSorb = fmax(dndot, -ndotInTc); // tcSorb is <= 0.
        ndotOutTc += tcSorb;
        if (fluidIndex >= 0) {
            const double dnRemain = -dndot + tcSorb;      // tcSorb is <= 0, dnRemain is >= 0.
            mdotBulkDesorb = -dnRemain * molWeight;
            mFluid->setMass(fluidIndex, fmax(mdotInBulk + mdotBulkDesorb, 0.0));
        }
    } else if (dndot > DBL_EPSILON) {
        /// - Desorption to fluid.  Only desorb to the trace compound if there is no bulk fluid.
        if (fluidIndex >= 0) {
            mdotBulkDesorb = dndot * molWeight;
            mFluid->setMass(fluidIndex, mdotInBulk + mdotBulkDesorb);
        } else if (tcIndex >= 0) {
            ndotOutTc += dndot;
        }
    }

    /// - Set new exit mass rate of the trace compound sorbate.
    if (tcIndex >= 0) {
        mFluid->getTraceCompounds()->setMass(tcIndex, ndotOutTc * molWeight);
    }

    return mdotBulkDesorb;
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
        /// - Note that normally this is called before simulation init, so the H&S system is not
        ///   yet running to process the H&S message that goes along with this exception.
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                "this function can't be used for custom sorbant types.");
    }
    if (not mDefinedSorbants) {
        mDefinedSorbants = new DefinedSorbantProperties();
    }
    std::ostringstream name;
    name << mName << ".mSegments_" << mSegments.size();
    const SorbantProperties* properties = mDefinedSorbants->getSorbant(definedType);
    mSegments.push_back(GunnsFluidSorptionBedSegmentConfigData(name.str(), properties, volume, htc));
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
void GunnsFluidSorptionBedConfigData::addSegment(SorbantProperties* customType,
                                                 const double volume, const double htc)
{
    const SorbantProperties* foundProperties = 0;
    for (unsigned int i=0; i<mCustomSorbants.size(); ++i) {
        if (customType == &mCustomSorbants.at(i)) {
            foundProperties = customType;
        }
    }
    if (customType != foundProperties) {
        /// - Note that normally this is called before simulation init, so the H&S system is not
        ///   yet running to process the H&S message that goes along with this exception.
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                "custom sorbant properties for this segment not found in the bed config data.");
    }
    std::ostringstream name;
    name << mName << ".mSegments_" << mSegments.size();
    mSegments.push_back(GunnsFluidSorptionBedSegmentConfigData(name.str(), foundProperties, volume, htc));
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
    mAdsorptionFluidRates(0),
    mAdsorptionTcRates(0),
    mAdsorbedFluidMasses(0),
    mAdsorbedTcMasses(0),
    mWorkFluidRates(0),
    mWorkTcRates(0),
    mWorkFluidMasses(0),
    mWorkTcMasses(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sorption Bed link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSorptionBed::~GunnsFluidSorptionBed()
{
    delete [] mWorkTcMasses;
    delete [] mWorkTcRates;
    TS_DELETE_ARRAY(mAdsorbedTcMasses);
    TS_DELETE_ARRAY(mAdsorptionTcRates);
    delete [] mWorkFluidMasses;
    delete [] mWorkFluidRates;
    TS_DELETE_ARRAY(mAdsorbedFluidMasses);
    TS_DELETE_ARRAY(mAdsorptionFluidRates);
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
        /// - Build the initial sorbate loadings for this segment.
        std::vector<GunnsFluidSorptionBedSegmentInputData> loading;
        for (unsigned int j=0; j<inputData.mLoading.size(); ++j) {
            if (inputData.mLoading.at(j).mSegment == i) {
                loading.push_back(inputData.mLoading.at(j));
            }
        }
        mSegments[i].mTemperature = inputData.mWallTemperature;
        mSegments[i].mFluid = mInternalFluid;
        mSegments[i].init(configData.mSegments.at(i), mNodes[0]->getFluidConfig(), loading);
        mVolume += mSegments[i].mVolume;
    }

    /// - Allocate and intialize the adsorption totals arrays.
    const int nFluids = mNodes[0]->getFluidConfig()->mNTypes;
    TS_NEW_PRIM_ARRAY_EXT(mAdsorptionFluidRates, nFluids, double, mName + ".mAdsorptionFluidRates");
    TS_NEW_PRIM_ARRAY_EXT(mAdsorbedFluidMasses,  nFluids, double, mName + ".mAdsorbedFluidMasses");
    mWorkFluidRates  = new double[nFluids];
    mWorkFluidMasses = new double[nFluids];

    const GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        const int nTc = tc->getConfig()->mNTypes;
        if (nTc > 0) {
            TS_NEW_PRIM_ARRAY_EXT(mAdsorptionTcRates, nTc, double, mName + ".mAdsorptionTcRates");
            TS_NEW_PRIM_ARRAY_EXT(mAdsorbedTcMasses,  nTc, double, mName + ".mAdsorbedTcMasses");
            mWorkTcRates  = new double[nTc];
            mWorkTcMasses = new double [nTc];
        }
    }
    zeroAdsorbOutputs();
    computeAdsorbOutputs();

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
    /// - Throw an exception if adsorber wall temperature < 0.0.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "Adsorber wall temperature < 0.0.");
    }

    for (unsigned int i=0; i<inputData.mLoading.size(); ++i) {
        /// - Throw an exception if a segment number is out of range.
        if (inputData.mLoading.at(i).mSegment >= configData.mSegments.size()) {
            GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "an initial sorbate loading segment number is out of range.");
        }

        /// - Throw an exception if a segment initial sorbate loading is negative.
        if (inputData.mLoading.at(i).mLoading < 0.0) {
            GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "an initial sorbate loading value < 0.");
        }
    }

    for (unsigned int i=0; i<configData.mSegments.size(); ++i) {
        /// - Throw an exception if a segment configuration data is missing sorbant properties.
        if (not configData.mSegments.at(i).mProperties) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "a segment has null sorbant properties.");
        }

        /// - Throw an exception if a segment configuration data has zero volume.
        if (configData.mSegments.at(i).mVolume < DBL_EPSILON) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "a segment has zero volume.");
        }

        /// - Throw an exception if a segment configuration data has zero heat transfer coefficient.
        if (configData.mSegments.at(i).mHtc < DBL_EPSILON) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "a segment has zero heat transfer coefficient.");
        }
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
    zeroAdsorbOutputs();
    for (unsigned int i=0; i<mNSegments; ++i) {
        for (unsigned int j=0; j<mSegments[i].getNSorbates(); ++j) {
            mSegments[i].mSorbates[j].restartModel();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes the bulk fluid and trace compounds adsorbed mass and adsorption rate arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBed::zeroAdsorbOutputs()
{
    const int nFluids = mNodes[0]->getFluidConfig()->mNTypes;
    for (int i=0; i<nFluids; ++i) {
        mAdsorptionFluidRates[i] = 0.0;
        mAdsorbedFluidMasses[i]  = 0.0;
    }

    const GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        for (int i=0; i<tc->getConfig()->mNTypes; ++i) {
            mAdsorptionTcRates[i] = 0.0;
            mAdsorbedTcMasses[i]  = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes the bulk fluid and trace compounds adsorbed mass and adsorption rate temprary
///           working arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBed::zeroAdsorbWork()
{
    const int nFluids = mNodes[0]->getFluidConfig()->mNTypes;
    for (int i=0; i<nFluids; ++i) {
        mWorkFluidRates[i]  = 0.0;
        mWorkFluidMasses[i] = 0.0;
    }

    const GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        for (int i=0; i<tc->getConfig()->mNTypes; ++i) {
            mWorkTcRates[i]  = 0.0;
            mWorkTcMasses[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the bulk fluid and trace compounds adsorbed mass and adsorption rate values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBed::computeAdsorbOutputs()
{
    zeroAdsorbWork();
    const unsigned int nFluids = mNodes[0]->getFluidConfig()->mNTypes;
    unsigned int nTc = 0;
    const GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        nTc = tc->getConfig()->mNTypes;
    }

    for (unsigned int i=0; i<mNSegments; ++i) {
        for (unsigned int j=0; j<mSegments[i].getNSorbates(); ++j) {
            const GunnsFluidSorptionBedSorbate& sorbate = mSegments[i].mSorbates[j];
            const int fluidIndex = sorbate.getFluidIndexes()->mFluid;
            const int tcIndex    = sorbate.getFluidIndexes()->mTc;
            if (fluidIndex >= 0) {
                mWorkFluidRates[fluidIndex]  += sorbate.mAdsorptionRate;
                mWorkFluidMasses[fluidIndex] += sorbate.mLoadedMass;
            } else if (tcIndex >= 0) {
                /// - For compounds that are both a bulk fluid and a TC, we only report loading as
                ///   being in the bulk fluid, and report zero loading of the TC - this reflects
                ///   that this model desorbs these 'dual' compounds to the bulk fluid, not the TC.
                mWorkTcRates[tcIndex]  += sorbate.mAdsorptionRate;
                mWorkTcMasses[tcIndex] += sorbate.mLoadedMass;
            }
        }
    }

    /// - Convert absorbed mole rate into mass rate.
    for (unsigned int i=0; i<nFluids; ++i) {
        mAdsorptionFluidRates[i] = mWorkFluidRates[i] * mInternalFluid->getProperties(mInternalFluid->getType(i))->getMWeight();
        mAdsorbedFluidMasses[i]  = mWorkFluidMasses[i];
    }
    for (unsigned int i=0; i<nTc; ++i) {
        mAdsorptionTcRates[i] = mWorkTcRates[i] * tc->getConfig()->mCompounds.at(i)->mMWeight;
        mAdsorbedTcMasses[i]  = mWorkTcMasses[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  Calculates bulk through-flow properties, updates the bed segments, and transports the
///           through-fluid from the source node and, after sorption exchange, to the sink node.
///           Note that even with no pressure gradient across the link to cause fluid flow to
///           conduct through the link, segments can desorb into the fluid, creating flow source to
///           a node - this always is applied to the port 1 node.  This is a flow source effect,
///           applied to the source vector on the next pass.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSorptionBed::transportFlows(const double dt)
{
    /// - Convert the molar flow rate to mass rate using the molecular weight of the source node.
    const int    sourcePort    = determineSourcePort(mFlux, 0, 1);
    const int    sinkPort      = 1 - sourcePort;
    const double sourceDensity = mNodes[sourcePort]->getOutflow()->getDensity();
    mFlowRate                  = mFlux * mNodes[sourcePort]->getOutflow()->getMWeight();

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   source node.
    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power.
    computePower();

    /// - Update segments and get flow rate exiting the link to downstream after sorption.
    const double exitFlow = updateSegments(dt, sourceDensity, sourcePort);

    /// - Compute total bulk desorbed moles as difference between bulk moles in and out.
    const double bulkDesorbMoleRate = exitFlow / mInternalFluid->getMWeight()
                                    - fabs(mFlowRate) / mNodes[sourcePort]->getOutflow()->getMWeight();

    /// - Transport flow between nodes and correct the source vector for sorption.
    mNodes[sourcePort]->collectOutflux(fabs(mFlowRate));
    mNodes[sinkPort]->collectInflux(exitFlow, mInternalFluid);
    mSourceVector[sourcePort] = 0.0;
    mSourceVector[sinkPort]   = bulkDesorbMoleRate;

    computeAdsorbOutputs();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt            (s)     Integration time step.
/// @param[in]  sourceDensity (kg/m3) Inlet fluid density.
/// @param[in]  sourcePort    (--)    Port number supplying the inlet flow to this bed link.
///
/// @returns  double (kg/s) Flow rate exiting the bed link to the downstream node, after sorption.
///
/// @details  Iterates over the bed segments based on flow direction, updates them for sorption and
///           and exchange with the through-fluid, and returns the exiting through flowrate after
///           sorption.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidSorptionBed::updateSegments(const double dt, const double sourceDensity, const unsigned int sourcePort)
{
    /// - Initialize the exit flow as the inlet flow, before sorption.
    double exitFlow = fabs(mFlowRate);

    /// - Skip sorption when the time step is negligible or neither port node has fluid density
    ///   (both ports are on Ground).
    if (dt > DBL_EPSILON and sourceDensity > DBL_EPSILON) {

        /// - Using a minimum flow rate for setting the internal fluid from the source node outflow
        ///   fluid allows a valid fluid mixture to remain in the internal fluid when bulk flow
        ///   rate is zero.
        mInternalFluid->setState(mNodes[sourcePort]->getOutflow());
        mInternalFluid->setMass(std::max(exitFlow, DBL_EPSILON));

        const double dPoverV = fabs(mPotentialDrop) / mVolume;
        double segP          = mPotentialVector[sourcePort];

        if (mFlowRate >= 0.0) {
            for (unsigned int i=0; i<mNSegments; ++i) {
                const double nextSegP = segP - dPoverV * mSegments[i].mVolume;
                /// - Update segments in order of flow direction.
                mSegments[i].update(exitFlow, segP, nextSegP, dt);

                /// - Estimate total pressure in between segments, for inlet pressure to each
                ///   segment, as interpolated between the node pressures by relative segment length
                ///   (volume, assuming constant area).  We assume affect of adsorb/desorbed sorbates
                ///   on these intermediate pressures is negligible.
                segP = nextSegP;
            }
        } else if (mFlowRate < 0.0) {
            for (unsigned int i=mNSegments; i-- > 0;) { // this syntax safely loops the unsigned backwards to 0 inclusive.
                const double nextSegP = segP - dPoverV * mSegments[i].mVolume;
                mSegments[i].update(exitFlow, segP, nextSegP, dt);
                segP = nextSegP;
            }
        }
    }
    return exitFlow;
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

////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in] type (--) The trace compound type to return the adsorbed mass of.
///
/// @return  double (kg) The total adsorbed mass of the given trace compound type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the total adsorbed mass of the given trace compound type.
////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidSorptionBed::getAdsorbedTcMass(ChemicalCompound::Type type) const
{
    double mass = 0.0;
    const GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        mass = mAdsorbedTcMasses[tc->find(type, mName)];
    } else {
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Function Call", "the network has no trace compounds.")
    }
    return mass;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in] type (--) The trace compound type to return the adsorption rate of.
///
/// @return  double (kg) The total adsorption rate of the given trace compound type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the total adsorption rate of the given trace compound type.
////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidSorptionBed::getAdsorptionTcRate(ChemicalCompound::Type type) const
{
    double mass = 0.0;
    const GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        mass = mAdsorptionTcRates[tc->find(type, mName)];
    } else {
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Function Call", "the network has no trace compounds.")
    }
    return mass;
}
