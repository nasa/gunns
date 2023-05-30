/**
@file
@brief    GUNNS Fluid Distributed Interface Link implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (GunnsFluidCapacitor.o)
    (GunnsFluidDistributed2WayBus.o)
   )
*/

#include "GunnsFluidDistributedIf.hh"
#include "core/GunnsFluidUtils.hh"
#include "core/GunnsFluidCapacitor.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Distributed Interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfData::GunnsFluidDistributedIfData()
    :
    GunnsFluidDistributed2WayBusInterfaceData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Distributed Interface data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfData::~GunnsFluidDistributedIfData()
{
    if (mTcMoleFractions) {
        TS_DELETE_ARRAY(mTcMoleFractions);
    }
    if (mMoleFractions) {
        TS_DELETE_ARRAY(mMoleFractions);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] nIfFluids (--) Number of fluid constituents.
/// @param[in] nIfTc     (--) Number of trace compounds.
/// @param[in] name      (--) Name of the instance for dynamic memory names for Trick MM.
///
/// @details  Allocates dynamic arrays for mole fractions.  TrickHLA is unable to move data between
///           arrays that are new'd, so this method overrides the base class to allocate using TMM.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIfData::initialize(const unsigned int nIfFluids,
                                             const unsigned int nIfTc,
                                             const std::string& name)
{
    mNumFluid = nIfFluids;
    mNumTc    = nIfTc;

    /// - Delete & re-allocate fractions arrays in case of repeated calls to this function.
    if (mNumFluid > 0) {
        TS_DELETE_ARRAY(mMoleFractions);
        TS_NEW_PRIM_ARRAY_EXT(mMoleFractions, mNumFluid, double, name + ".mMoleFractions");
        for (unsigned int i=0; i<mNumFluid; ++i) {
            mMoleFractions[i] = 0.0;
        }
    }
    if (mNumTc > 0) {
        TS_DELETE_ARRAY(mTcMoleFractions);
        TS_NEW_PRIM_ARRAY_EXT(mTcMoleFractions, mNumTc, double, name + ".mTcMoleFractions");
        for (unsigned int i=0; i<mNumTc; ++i) {
            mTcMoleFractions[i] = 0.0;
        }
    }
}

//// @details  This value is chosen to get reliable network capacitance calculations from the solver
///           for liquid and gas nodes.
const double GunnsFluidDistributedIf::mNetworkCapacitanceFlux = 1.0E-6;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name          (--)  Link name.
/// @param[in] nodes         (--)  Network nodes array.
/// @param[in] isPairMaster  (--)  This is the master of the pair.
/// @param[in] useEnthalpy   (--)  Transport energy as specific enthalpy instead of temperature.
/// @param[in] demandOption  (--)  Demand mode option to trade stability for less restriction on flow rate.
/// @param[in] capacitorLink (--)  Pointer to the node capacitor link.
///
/// @details  Default GUNNS Fluid Distributed Interface Link Config Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfConfigData::GunnsFluidDistributedIfConfigData(
        const std::string&   name,
        GunnsNodeList*       nodes,
        const bool           isPairMaster,
        const bool           useEnthalpy,
        const bool           demandOption,
        GunnsFluidCapacitor* capacitorLink)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mIsPairMaster(isPairMaster),
    mUseEnthalpy(useEnthalpy),
    mDemandOption(demandOption),
    mCapacitorLink(capacitorLink),
    mModingCapacitanceRatio(1.25),
    mDemandFilterConstA(1.5),
    mDemandFilterConstB(0.75),
    mFluidSizesOverride(false),
    mNumFluidOverride(0),
    mNumTcOverride(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Distributed Interface Link Config Data Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfConfigData::~GunnsFluidDistributedIfConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] nFluids  (--)  Number of primary fluid compounds override value.
/// @param[in] nTc      (--)  Number of trace compounds override value.
///
/// @details  Sets the fluid mixture sizes override flag true, and sets the primary and trace fluid
///           compound mixture sizes to the given values.  This is used to force the interface
///           fluid mixture arrays to different sizes then the sizes in the GUNNS fluid network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIfConfigData::overrideInterfaceMixtureSizes(const unsigned int nFluids,
                                                                      const unsigned int nTc)
{
    mFluidSizesOverride = true;
    mNumFluidOverride   = nFluids;
    mNumTcOverride      = nTc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag   (--)  Blockage malfunction flag.
/// @param[in] malfBlockageValue  (--)  Blockage malfunction fractional value (0-1).
/// @param[in] forceDemandMode    (--)  Forces the link to always be in Demand mode.
/// @param[in] forceSupplyMode    (--)  Forces the link to always be in Supply mode.
///
/// @details  Default GUNNS Fluid Distributed Interface Link Input Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfInputData::GunnsFluidDistributedIfInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const bool   forceDemandMode,
        const bool   forceSupplyMode)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mForceDemandMode(forceDemandMode),
    mForceSupplyMode(forceSupplyMode)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Distributed Interface Link Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIfInputData::~GunnsFluidDistributedIfInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Distributed Interface Link Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIf::GunnsFluidDistributedIf()
    :
    GunnsFluidLink         (NPORTS),
    mInData                (),
    mOutData               (),
    mInterface             (),
    mUseEnthalpy           (false),
    mDemandOption          (false),
    mSupplyVolume          (0.0),
    mCapacitorLink         (0),
    mEffectiveConductivity (0.0),
    mSourcePressure        (0.0),
    mDemandFlux            (0.0),
    mDemandFluxGain        (0.0),
    mSuppliedCapacitance   (0.0),
    mTempMassFractions     (0),
    mTempMoleFractions     (0),
    mTempTcMoleFractions   (0),
    mWorkFluidState        (),
    mWorkFlowState         (),
    mOtherIfs              (),
    mFluidState            ()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Distributed Interface Link Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidDistributedIf::~GunnsFluidDistributedIf()
{
    delete [] mTempTcMoleFractions;
    mTempTcMoleFractions = 0;
    delete [] mTempMoleFractions;
    mTempMoleFractions = 0;
    delete [] mTempMassFractions;
    mTempMassFractions = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to link Config Data.
/// @param[in]     inputData    (--) Reference to link Input Data.
/// @param[in,out] networkLinks (--) Network links vector.
/// @param[in]     port0        (--) Network port 0.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Fluid Distributed Interface link with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::initialize(const GunnsFluidDistributedIfConfigData& configData,
                                         const GunnsFluidDistributedIfInputData&  inputData,
                                         std::vector<GunnsBasicLink*>&            networkLinks,
                                         const int                                port0)
{
    /// - Initialize & validate parent.
    int ports[1] = {port0};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag
    mInitFlag = false;

    /// - Initialize from config data.
    mUseEnthalpy            = configData.mUseEnthalpy;
    mDemandOption           = configData.mDemandOption;
    mCapacitorLink          = configData.mCapacitorLink;

    /// - Initialize from input data.
    if (inputData.mForceDemandMode) {
        mInterface.forceDemandRole();
    } else if (inputData.mForceSupplyMode) {
        mInterface.forceSupplyRole();
    } else {
        mInterface.resetForceRole();
    }

    /// - Create the internal link fluid, allocate and load the fluid config map to translate the
    ///   external network's fluid to match our local network's config.
    createInternalFluid();

    /// - Allocate memory and build the temporary mass and mole fractions arrays.  We allocate
    ///   persistent arrays now to save allocation time during run.
    const unsigned int nTypes = mNodes[0]->getFluidConfig()->mNTypes;
    delete [] mTempTcMoleFractions;
    delete [] mTempMoleFractions;
    delete [] mTempMassFractions;
    mTempMassFractions = new double[nTypes];
    mTempMoleFractions = new double[nTypes];
    for (unsigned int i = 0; i < nTypes; ++i) {
        mTempMassFractions[i] = 0.0;
        mTempMoleFractions[i] = 0.0;
    }
    const GunnsFluidTraceCompoundsConfigData* tcConfig = mNodes[0]->getFluidConfig()->mTraceCompounds;
    unsigned int nTc = 0;
    if (tcConfig) {
        nTc = tcConfig->mNTypes;
        if (nTc > 0) {
            mTempTcMoleFractions = new double[nTc];
            for (unsigned int i = 0; i < nTc; ++i) {
                mTempTcMoleFractions[i] = 0.0;
            }
        }
    }

    /// - Initialize the interface data objects so they can allocate memory.  The fluid sizes
    ///   overrides sizes the interface for different-sized mixture arrays than the fluid config
    ///   in this network.  This is for when the reusable HLA FOM arrays are larger than our model.
    ///   Otherwise, the interface is sized to match our fluid config.  The working fluid and flow
    ///   states are always sized to match our fluid config.
    if (configData.mFluidSizesOverride) {
        mInData   .initialize(configData.mNumFluidOverride, configData.mNumTcOverride, mName + ".mInData");
        mOutData  .initialize(configData.mNumFluidOverride, configData.mNumTcOverride, mName + ".mOutData");
        mInterface.initialize(configData.mIsPairMaster, configData.mNumFluidOverride, configData.mNumTcOverride);
    } else {
        mInData   .initialize(nTypes, nTc, mName + ".mInData");
        mOutData  .initialize(nTypes, nTc, mName + ".mOutData");
        mInterface.initialize(configData.mIsPairMaster, nTypes, nTc);
    }
    mWorkFluidState.initialize(nTypes, nTc);
    mWorkFlowState.initialize(nTypes, nTc);

    /// - Initialize remaining state variables.
    mSupplyVolume          = 0.0;
    mEffectiveConductivity = 0.0;
    mSourcePressure        = 0.0;
    mDemandFlux            = 0.0;
    mDemandFluxGain        = 1.0;
    mSuppliedCapacitance   = 0.0;

    try {
        mFluidState.initializeName(mName + ".mFluidState");
        double fractions[FluidProperties::NO_FLUID] = {1.0};
        // Use default values to satisfy fluid initialize, then set to node contents.
        PolyFluidInputData initFluidState(294.261,        // temperature
                                          101.325,        // pressure
                                          0.0,            // flowrate
                                          0.0,            // mass
                                          fractions);     // massFraction
        mFluidState.initialize(*mNodes[0]->getFluidConfig(), initFluidState);
        mFluidState.setState(mNodes[0]->getContent());
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Caught exception from mFluidState initialization.");
    }

    /// - Validate initialization.
    validate(inputData);

    /// - Pass notifications from the interface model to H&S.
    processIfNotifications(true);

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData (--) Reference to link Input Data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Distributed Interface initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::validate(const GunnsFluidDistributedIfInputData& inputData) const
{
    /// - Throw on null pointer to the node capacitor link.
    if (not mCapacitorLink) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Missing pointer to the node capacitor link.");
    }

    /// - Throw if conflicting mode force flags.
    if (inputData.mForceDemandMode and inputData.mForceSupplyMode) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "both mode force flags are set.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  otherIf  (--)  Address of another GunnsFluidDistributedIf link to store.
///
/// @details  Pushes the given GunnsFluidDistributedIf link pointer onto the mOtherIfs vector.
///           Duplicate object, including this, are quietly ignored.  This is a convenience to allow
///           the same list of links to be passed to all links in a network from the input file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::addOtherIf(GunnsFluidDistributedIf* otherIf)
{
    bool duplicate = (otherIf == this);
    for (unsigned int i=0; i<mOtherIfs.size(); ++i) {
        if (otherIf == mOtherIfs[i]) {
            duplicate = true;
        }
    }
    if (not duplicate) {
        mOtherIfs.push_back(otherIf);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mEffectiveConductivity = 0.0;
    mSourcePressure        = 0.0;
    mDemandFlux            = 0.0;
    mDemandFluxGain        = 1.0;
    mSuppliedCapacitance   = 0.0;
    for (int i = 0; i < mNodes[0]->getFluidConfig()->mNTypes; ++i) {
        mTempMassFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Pre-network step calculations.  Processes the incoming data from the external
///           interface, flips modes and updates frame counters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processInputs()
{
    /// - Copy data received from the data network (HLA, etc.) into the interface logic's input.
    mInterface.mInData = mInData;

    /// - Interface mode changes and node volume update in response.
    bool previousDemandMode = mInterface.isInDemandRole();
    mInterface.processInputs();
    bool demandMode = mInterface.isInDemandRole();
    if (demandMode and not previousDemandMode) {
        mSupplyVolume = mNodes[0]->getVolume();
        mCapacitorLink->editVolume(true, 0.0);
    } else if (previousDemandMode and not demandMode) {
        mCapacitorLink->editVolume(true, mSupplyVolume);
        mSupplyVolume = 0.0;
    }

    /// - More processing of incoming data for resulting pairing mode.
    processInputsDemand();
    processInputsSupply();

    /// - Pass notifications from the interface model to H&S.
    processIfNotifications(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      pressure  (kPa)  Pressure to set the fluid to.
/// @param[in,out]  fluid     (--)   Pointer to the PolyFluid object to be set.
///
/// @throws   TsOutOfBoundsException
///
/// @returns  double (--) Sum of input bulk compound mole fractions, <= 1.
///
/// @details  Copies the incoming fluid state from the other side of the interface (mInData) into
///           the given fluid object and sets it to the given pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidDistributedIf::inputFluid(const double pressure, PolyFluid* fluid)
{
    /// - Point to the working interface fluid state object based on interface role.
    GunnsFluidDistributedMixtureData* workingState = &mWorkFlowState;
    if (mInterface.isInDemandRole()) {
        workingState = &mWorkFluidState;
    }

    /// - Normalize the incoming bulk mole fractions to sum to 1.  Internally, GUNNS sums the bulk
    ///   mole fractions to 1, and this doesn't include the trace compounds.  But the interface
    ///   data includes the TC's in the sum to 1.  Adjustment to the TC's is handled below.
    double inBulkFractionSum = 0.0;
    const PolyFluidConfigData* fluidConfig = mNodes[0]->getFluidConfig();
    workingState->getMoleFractions(mTempMoleFractions, fluidConfig->mNTypes);
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        inBulkFractionSum += mTempMoleFractions[i];
    }
    if (inBulkFractionSum < DBL_EPSILON) {
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Interface Data",
                    "incoming bulk mole fractions sum to zero.");
    }
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        mTempMoleFractions[i] /= inBulkFractionSum;
    }

    /// - Convert incoming mole fractions to mass fractions.
    GunnsFluidUtils::convertMoleFractionToMassFraction(mTempMassFractions,
                                                       mTempMoleFractions,
                                                       mNodes[0]->getFluidConfig());

    fluid->setMassAndMassFractions(0.0, mTempMassFractions);
    fluid->setPressure(pressure);

    if (mUseEnthalpy) {
        fluid->setTemperature(fluid->computeTemperature(mWorkFluidState.mEnergy));
    } else {
        fluid->setTemperature(mWorkFluidState.mEnergy);
    }

    if (mWorkFluidState.mTcMoleFractions) {
        GunnsFluidTraceCompounds* tc = fluid->getTraceCompounds();
        if (tc) {
            /// - As above, adjust the TC mole fractions to be relative to the internal GUNNS bulk
            ///   fluid.
            const GunnsFluidTraceCompoundsConfigData* tcConfig = tc->getConfig();
            if (tcConfig) {
                workingState->getTcMoleFractions(mTempTcMoleFractions, tcConfig->mNTypes);
                for (int i = 0; i < tcConfig->mNTypes; ++i) {
                    mTempTcMoleFractions[i] /= inBulkFractionSum;
                }
                tc->setMoleFractions(mTempTcMoleFractions);
            }
        }
    }
    return inBulkFractionSum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes inputs from the other side of the interface when in supply mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processInputsSupply()
{
    /// - When we are Supply mode but have not yet received Demand data from the other side, we set
    ///   the demand flux to zero.
    /// - When in Demand mode, zero the demand flux.
    /// - When in Supply mode, zero the source pressure.
    mDemandFlux = 0.0;
    if (not mInterface.isInDemandRole()) {
        mSourcePressure = 0.0;
        if (mInterface.getFlowState(mWorkFlowState)) {
            /// - Convert (mol/s) to (kmol/s), and external mole rate to internal GUNNS rate.  The
            ///   internal GUNNS rate does not include the mole rate of the trace compounds.  The
            ///   inputFluid function returns the fraction of the bulk fluid compunds in the total,
            ///   which is our adjustment.
            mDemandFlux = -mInterface.mInData.mSource * UnitConversion::KILO_PER_UNIT
                        * inputFluid(1.0, mInternalFluid);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Processes inputs from the other side of the interface when in demand mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processInputsDemand()
{
    if (mInterface.isInDemandRole()) {
        if (mInterface.getFluidState(mWorkFluidState)) {
            /// - Convert (Pa) to (kPa).
            mSourcePressure = mWorkFluidState.mPressure * UnitConversion::KILO_PER_UNIT;
            inputFluid(mSourcePressure, mNodes[0]->getContent());
            mFluidState.setState(mNodes[0]->getContent());
        } else {
            /// - When we are in Demand mode but have not yet received Supply data from the other
            ///   side, we hold the node at its initial state.
            mSourcePressure = mNodes[0]->getPotential();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  End-of-network calculations.  Sets outputs data based on our current mode.  Calls to
///           check if its time to flip to Demand node from Supply mode based on relative
///           capacitance, and updates the count of frames since the last mode flip.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processOutputs()
{
    /// - Based on interface mode, update the working fluid state or flow state with pressure or
    ///   flow rate, respectively.
    if (mInterface.isInDemandRole()) {
        mWorkFlowState.mFlowRate = processOutputsDemand();
        mInterface.setFlowState(mWorkFlowState);
    } else {
        mWorkFluidState.mPressure = processOutputsSupply();
        mInterface.setFluidState(mWorkFluidState);
    }

    /// - Update the interface logic to compute its outputs based on our latest network capacitance
    ///   and handle any mode flip.
    const double capacitance = outputCapacitance();
    bool previousDemandMode = mInterface.isInDemandRole();
    mInterface.processOutputs(capacitance);
    bool newDemandMode = mInterface.isInDemandRole();
    if (newDemandMode and not previousDemandMode) {
        mSupplyVolume = mNodes[0]->getVolume();
        mCapacitorLink->editVolume(true, 0.0);
    }

    /// - Copy the interface logic's output to our data object for output on data network (HLA).
    ///   We use the base class assignment operator to assign base = base.  There is no data lost
    ///   to slicing since the derived class adds no attributes.
    mOutData.GunnsFluidDistributed2WayBusInterfaceData::operator=(mInterface.mOutData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fluid  (--)  Fluid state to be output to the other side of the interface.
///
/// @returns  double (--)  Sum of all bulk and trace compound mole fractions, >= 1.
///
/// @details  Copies the given fluid state for output to the other side of the interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidDistributedIf::outputFluid(PolyFluid* fluid, GunnsFluidDistributedMixtureData* work)
{
    /// - Output energy as either temperature or specific enthalpy as configured.
    if (mUseEnthalpy) {
        work->mEnergy = fluid->getSpecificEnthalpy();
    } else {
        work->mEnergy = fluid->getTemperature();
    }

    /// - Convert outgoing mass fractions to mole fractions.
    const PolyFluidConfigData* fluidConfig = mNodes[0]->getFluidConfig();
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        mTempMassFractions[i] = fluid->getMassFraction(i);
    }
    GunnsFluidUtils::convertMassFractionToMoleFraction(mTempMoleFractions,
                                                       mTempMassFractions,
                                                       fluidConfig);

    /// - Copy the trace compounds and sum their mole fractions for normalizing below.
    double moleFractionSum = 0.0;
    const GunnsFluidTraceCompounds* tc = fluid->getTraceCompounds();
    unsigned int nTc = 0;
    if (tc) {
        const GunnsFluidTraceCompoundsConfigData* tcConfig = tc->getConfig();
        if (tcConfig) {
            nTc = tcConfig->mNTypes;
        }
        for (unsigned int i = 0; i < nTc; ++i) {
            moleFractionSum += tc->getMoleFractions()[i];
        }
    }

    /// - Add bulk fluid mole fractions to the sum for normalizing.
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        moleFractionSum += mTempMoleFractions[i];
    }

    /// - Normalize the bulk and trace compounds mole fractions so they all sum to 1.  Unlike
    ///   GUNNS fluids, where only the bulk fractions sum to 1 and TC's are tracked elsewhere,
    ///   this interface requires to the total sum of bulk + TC's to 1.
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        mTempMoleFractions[i] /= moleFractionSum;
    }
    for (unsigned int i = 0; i < nTc; ++i) {
        mTempTcMoleFractions[i] = tc->getMoleFractions()[i] / moleFractionSum;
    }
    work->setMoleFractions(mTempMoleFractions, fluidConfig->mNTypes);
    work->setTcMoleFractions(mTempTcMoleFractions, nTc);
    return moleFractionSum;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  End-of-network calculation of outputs to the other side of the interface when this
///           side is in Supply mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidDistributedIf::processOutputsSupply()
{
    /// - Convert (kPa) to (Pa).
    const double pressure = mNodes[0]->getPotential() * UnitConversion::UNIT_PER_KILO;
    outputFluid(mNodes[0]->getContent(), &mWorkFluidState);
    mFluidState.setState(mNodes[0]->getContent());
    return pressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  End-of-network calculation of outputs to the other side of the interface when this
///           side is in Demand mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidDistributedIf::processOutputsDemand()
{
    /// - If there is no inflow to the node then its inflow fluid has a reset state so we can't use
    ///   it.  Instead, use the node's contents.  Convert (kmol/s) to (mol/s).  Adjust mole flow
    ///   rate (mFlux only includes bulk compounds) to also include the trace compounds for total
    ///   flow rate to/from the interface.  The outputFluid function returns this scale factor.  We
    ///   also fall back to the node's contents if the node inflow fluid has any negative fluid
    ///   mixture fractions.
    PolyFluid* useFluid = mNodes[0]->getInflow();
    if (useFluid->getTemperature() > 0.0) {
        if (checkNegativeFluidFractions(useFluid)) {
            GUNNS_WARNING("demand node inflow has negative mixture fractions.");
            useFluid = mNodes[0]->getContent();
        }
    } else {
        useFluid = mNodes[0]->getContent();
    }
    const double flow = mFlux * UnitConversion::UNIT_PER_KILO * outputFluid(useFluid, &mWorkFlowState);
    return flow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calculates and sets the outgoing capacitance value.  This is the network capacitance
///           of the node, minus the effective capacitance added by this link in Demand mode (the
///           mSuppliedCapacitance), and minus the effective capacitance at our node added by other
///           links in Demand mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidDistributedIf::outputCapacitance()
{
    /// - Subtract the capacitance we supply in Demand mode.
    double capacitance = mNodes[0]->getNetworkCapacitance() - mSuppliedCapacitance;

    /// - For each other interface link that we know about, calculate and subtract its effective
    ///   capacitance at our node.  This is the capacitance that it supplied to its own node, 
    ///   reduced at our node by the resistances and capacitances in the network between the nodes.
    ///   The reduction is the increase in the other node potential over the increase in our node
    ///   potential when a flow source added at our node; these are intermediate values output by
    ///   the solver to the nodes in its network capacitance calculation.
    for (unsigned int i=0; i<mOtherIfs.size(); ++i) {
        const double otherSuppliedCap = mOtherIfs[i]->getSuppliedCapacitance();
        if (otherSuppliedCap > DBL_EPSILON) {    // they are in Demand mode
            const double* ourNetCapDp = getNetCapDeltaPotential();
            const double  otherDp     = ourNetCapDp[mOtherIfs[i]->getNodeMap()[0]];
            if (otherDp > DBL_EPSILON) {         // they affect us thru the conductive network
                const double ourDp = ourNetCapDp[mNodeMap[0]];
                const double ratio = otherDp / std::max(ourDp, DBL_EPSILON);
                capacitance       -= (otherSuppliedCap * ratio);
            }
        }
    }

    /// - Limit the outgoing capacitance to positive values, just in case something goes wrong
    ///   in our calculation.
    return std::max(0.0, capacitance);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  Calculates this link's contributions to the network system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::step(const double dt)
{
    /// - In Demand mode, conductance mirrors the Supply network capacitance: G = C/dt.  In Supply
    ///   mode, zero conductance blocks the Demand mode potential source effect.
    if (mInterface.isInDemandRole() and dt > DBL_EPSILON) {
        /// - The interface computes the limited flow rate as a function of demand-side pressure,
        ///   which is the pressure across the conductor to our interface node.  We don't know this
        ///   pressure because this link doesn't know about those conductors, and we don't use the
        ///   limited flow rate anyway - rather we use the interface's limit gain, which doesn't
        ///   need that pressure.  So we send zero pressure to the interface function.  This means
        ///   that the generic interface's calculation of the demand flow rate limit, in our case,
        ///   is incorrect and can't be used.  You can manually calculate the correct flow limit,
        ///   limit_correct, from the interface's incorrect value, limit_wrong, the relative
        ///   capacitances of both sides, Cs & Cd, the interface gain, and the actual demand
        ///   pressure across the conductor to this demand node, Pd, as:
        ///     limit_correct = fabs(Pd*gain/(dt*(1/Cs + 1/Cd)) - limit_wrong)
        mInterface.computeDemandLimit(dt, 0.0);
        mDemandFluxGain = mInterface.getDemandLimitGain();
        if (mInterface.mOutData.mCapacitance > FLT_EPSILON and mInterface.mInData.mCapacitance) {
            const double conductance = mDemandFluxGain * mInterface.mInData.mCapacitance / dt;
            /// - The default for this option = false follows the interface design standard, but our
            ///   GUNNS implementation sometimes restricts the resulting flow rate too much.  Use
            ///   this option = true to relax the stability in favor of increased flow rate.  You
            ///   can safely use this when Cs/Cd >> 1 and for small lags <= 4.
            if (mDemandOption or conductance < FLT_EPSILON) {
                mEffectiveConductivity = conductance;
            } else {
                mEffectiveConductivity = 1.0 / std::max(1.0/conductance + dt/mInterface.mOutData.mCapacitance, DBL_EPSILON);
            }
        } else {
            mDemandFluxGain = 1.0;
            mEffectiveConductivity = mDemandFluxGain * mInterface.mInData.mCapacitance / dt;
        }
        /// - Reduce the effective conductance from the blockage malfunction.
        if (mMalfBlockageFlag) {
            mEffectiveConductivity *= (1.0 - mMalfBlockageValue);
        }
    } else {
        mEffectiveConductivity = 0.0;
    }

    /// - Build admittance matrix.
    const double systemConductance = MsMath::limitRange(0.0, mEffectiveConductivity, mConductanceLimit);
    if (fabs(mAdmittanceMatrix[0] - systemConductance) > 0.0) {
        mAdmittanceMatrix[0] = systemConductance;
        mAdmittanceUpdate    = true;
    }

    if (mInterface.isInDemandRole()) {
        mSuppliedCapacitance = mAdmittanceMatrix[0] * dt;
    } else {
        mSuppliedCapacitance = 0.0;
    }

    /// - Build source vector, including the potential source effect in Demand mode, and the flow
    ///   source effect to the demand side in Supply mode.
    mSourceVector[0] = mSourcePressure * mAdmittanceMatrix[0] + mDemandFlux;

    /// - Flag the node to have its network capacitance calculated by the solver.
    mNodes[0]->setNetworkCapacitanceRequest(mNetworkCapacitanceFlux);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step (unused).
///
/// @details  Computes the flux across the link, sets port flow directions and schedules outflux
///           from the source node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = -mPotentialVector[0];
    computeFlux();

    /// - Set port flow directions and schedule flow from source nodes.
    if (mFlux > DBL_EPSILON) {
        mPortDirections[0] = SINK;
    } else if (mFlux < -DBL_EPSILON) {
        if (mInterface.isInDemandRole()) {
            mPortDirections[0] = SOURCE;
            mNodes[0]->scheduleOutflux(-mFlux);
        } else {
            mPortDirections[0] = SINK;
        }
    } else {
        mPortDirections[0] = NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (unused).
///
/// @details  Updates mass flow rate and transports fluid from or to the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::transportFlows(const double dt __attribute__((unused)))
{
    /// - Calculate mass flow rate (mFlowRate) from molar rate (mFlux).
    if (mInterface.isInDemandRole()) {
        /// - In Demand mode, we use the node's MW because the node's fluid contents have already
        ///   taken the properties of the Supply fluid (from mInData).  This is true for both flow
        ///   directions for the fluid transport to/from the node.  However for negative flow (out
        ///   of the node to Ground), this is not the actual mass flow rate to the supply network -
        ///   that is handled in processOutputs.
        mFlowRate = mFlux * mNodes[0]->getContent()->getMWeight();
    } else {
        /// - In Supply mode, we use the MW of the Demand network fluid (from mInternalFluid, which
        ///   came from mInData).
        mFlowRate = mFlux * mInternalFluid->getMWeight();
    }

    /// - Transport fluid:
    if (mInterface.isInDemandRole()) {
        if (mFlowRate > m100EpsilonLimit) {
            mNodes[0]->collectInflux(mFlowRate, mNodes[0]->getContent());
        } else if (mFlowRate < -m100EpsilonLimit) {
            mNodes[0]->collectOutflux(-mFlowRate);
        }
    } else if (fabs(mFlowRate) > m100EpsilonLimit) {
        mNodes[0]->collectInflux(mFlowRate, mInternalFluid);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned.
/// @param[in] node (--) The desired node to assign the port to.
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidDistributedIf must not map port 0 to the network vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidDistributedIf::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is the vacuum node.
    if ((0 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to the boundary node.");
        result = false;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fluid (--) Pointer to the fluid object to check.
///
/// @returns  bool (--)  True if any fractions are negative.
///
/// @details  Checks all the bulk and trace compound mixture fractions in the given fluid for any
///           negative values.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidDistributedIf::checkNegativeFluidFractions(const PolyFluid* fluid) const
{
    const PolyFluidConfigData* fluidConfig = mNodes[0]->getFluidConfig();
    for (int i = 0; i < fluidConfig->mNTypes; ++i) {
        if (fluid->getMoleFraction(i) < 0.0) {
            return true;
        }
    }

    const GunnsFluidTraceCompounds* tc = fluid->getTraceCompounds();
    if (tc) {
        const GunnsFluidTraceCompoundsConfigData* tcConfig = tc->getConfig();
        for (int i = 0; i < tcConfig->mNTypes; ++i) {
            if (tc->getMoleFractions()[i] < 0.0) {
                return true;
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] isInit (--) If this should throw initialization errors, currently not used.
///
/// @details  Pops all notifications from the interface utility's queue and translates them to H&S
///           messages.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidDistributedIf::processIfNotifications(const bool isInit __attribute__((unused)))
{
    GunnsDistributed2WayBusNotification notification;
    unsigned int numNotifs = 0;
    do {
        numNotifs = mInterface.popNotification(notification);
        if (GunnsDistributed2WayBusNotification::NONE != notification.mLevel) {
            std::ostringstream msg;
            msg << "from mInterface: " << notification.mMessage;
            switch (notification.mLevel) {
                case GunnsDistributed2WayBusNotification::INFO:
                    GUNNS_INFO(msg.str());
                    break;
                case GunnsDistributed2WayBusNotification::WARN:
                    GUNNS_WARNING(msg.str());
                    break;
// The interface currently has no ERR outputs, so these are untestable:
//                case GunnsDistributed2WayBusNotification::ERR:
//                    if (isInit) {
//                        GUNNS_ERROR(TsInitializationException, "Catch and re-throw", msg.str());
//                    } else {
//                        GUNNS_ERROR(TsOutOfBoundsException, "Catch and re-throw", msg.str());
//                    }
//                    break;
                default: // this won't happen, checked by the if statement
                    break;
            }
        }
    } while (numNotifs > 0);
}
