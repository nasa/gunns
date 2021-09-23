/*
@file     GunnsFluidMultiSeparator.hh
@brief    GUNNS Fluid Multi-Separator implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o)
    (software/exceptions/TsInitializationException.o)
    (software/exceptions/TsOutOfBoundsException.o)
    (core/GunnsFluidUtils.o))

*/

#include "GunnsFluidMultiSeparator.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "core/GunnsFluidUtils.hh"
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name           (--) Name of object.
/// @param[in,out] nodes          (--) Pointer to network nodes list.
/// @param[in]     maxConductance (m2) Max conductance of the flow-thru path.
/// @param[in]     fluidTypes     (--) Fluid types to be separated.
/// @param[in]     fluidPorts     (--) Port assignments for each separated fluid.
/// @param[in]     tcTypes        (--) Trace compound types to be separated.
/// @param[in]     tcPorts        (--) Port assignments for each separated trace compound.
///
/// @details  Default constructs this GUNNS Fluid Multi-Separator link model configuration data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiSeparatorConfigData::GunnsFluidMultiSeparatorConfigData(
        const std::string&                       name,
        GunnsNodeList*                           nodes,
        const double                             maxConductance,
        std::vector<FluidProperties::FluidType>* fluidTypes,
        std::vector<int>*                        fluidPorts,
        std::vector<ChemicalCompound::Type>*     tcTypes,
        std::vector<int>*                        tcPorts)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mMaxConductance(maxConductance),
    mFluidTypes(),
    mFluidPorts(),
    mTcTypes(),
    mTcPorts()
{
    /// - Clear the vectors.
    mFluidTypes.clear();
    mFluidPorts.clear();
    mTcTypes.clear();
    mTcPorts.clear();

    /// - Check for null pointer and set the vectors.
    if (fluidTypes)
    {
        mFluidTypes = *fluidTypes;
    }
    if (fluidPorts)
    {
        mFluidPorts = *fluidPorts;
    }
    if (tcTypes)
    {
        mTcTypes = *tcTypes;
    }
    if (tcPorts)
    {
        mTcPorts = *tcPorts;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Multi-Separator link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiSeparatorConfigData::GunnsFluidMultiSeparatorConfigData(
        const GunnsFluidMultiSeparatorConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mMaxConductance(that.mMaxConductance),
    mFluidTypes(that.mFluidTypes),
    mFluidPorts(that.mFluidPorts),
    mTcTypes(that.mTcTypes),
    mTcPorts(that.mTcPorts)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Multi-Separator link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiSeparatorConfigData::~GunnsFluidMultiSeparatorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1).
/// @param[in] fluidFractions    (--) For each separated fluid type, initial thru-flow fraction that is removed.
/// @param[in] tcFractions       (--) For each separated trace compound, initial thru-flow fraction that is removed.
///
/// @details  Default constructs this GUNNS Fluid Multi-Separator link model input data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiSeparatorInputData::GunnsFluidMultiSeparatorInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        std::vector<double>* fluidFractions,
        std::vector<double>* tcFractions)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mFluidFractions(),
    mTcFractions()
{
    /// - Clear the vectors.
    mFluidFractions.clear();
    mTcFractions.clear();

    /// - Check for null pointers and set the vectors.
    if (fluidFractions)
    {
        mFluidFractions = *fluidFractions;
    }
    if (tcFractions)
    {
        mTcFractions = *tcFractions;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Multi-Separator link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiSeparatorInputData::GunnsFluidMultiSeparatorInputData(
        const GunnsFluidMultiSeparatorInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mFluidFractions(that.mFluidFractions),
    mTcFractions(that.mTcFractions)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Multi-Separator link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiSeparatorInputData::~GunnsFluidMultiSeparatorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Multi-Separator link model with obviously invalid
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiSeparator::GunnsFluidMultiSeparator()
    :
    GunnsFluidLink(0),
    mMaxConductance(0.0),
    mNumSepTypes(0),
    mNumTcTypes(0),
    mSepIndex(0),
    mTcIndex(0),
    mSepPort(0),
    mTcPort(0),
    mSepFraction(0),
    mTcFraction(0),
    mEffectiveConductance(0.0),
    mSystemConductance(0.0),
    mSepBufferThru(0),
    mSepBufferExit(0),
    mSepFluid(0),
    mWorkTcMassFlowRates(0),
    mWorkMoleFractions(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Multi-Separator link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMultiSeparator::~GunnsFluidMultiSeparator()
{
    cleanupMemory();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes dynamic memory allocations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::cleanupMemory()
{
    TS_DELETE_ARRAY(mWorkMoleFractions);
    TS_DELETE_ARRAY(mWorkTcMassFlowRates);
    TS_DELETE_ARRAY(mSepFluid);
    TS_DELETE_ARRAY(mSepBufferExit);
    TS_DELETE_ARRAY(mSepBufferThru);
    TS_DELETE_ARRAY(mTcFraction);
    TS_DELETE_ARRAY(mSepFraction);
    TS_DELETE_ARRAY(mTcPort);
    TS_DELETE_ARRAY(mSepPort);
    TS_DELETE_ARRAY(mTcIndex);
    TS_DELETE_ARRAY(mSepIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Configuration data.
/// @param[in]     inputData    (--) Input data.
/// @param[in,out] networkLinks (--) Network links vector.
/// @param[in]     portsVector  (--) Vector of node numbers the link ports connect to.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Multi-Separator link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::initialize(const GunnsFluidMultiSeparatorConfigData& configData,
                                          const GunnsFluidMultiSeparatorInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          std::vector<int>*                         portsVector)
{
    /// - Reset initialization flag.
    mInitFlag = false;

    /// - Initialize the base class with initial node map from ports vector.
    mNumPorts = portsVector->size();
    int ports[mNumPorts];
    for(int i=0; i<mNumPorts; ++i)
    {
        ports[i] = portsVector->at(i);
    }
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Validate the config & input data.  It's split into separate functions to keep the function
    ///   lengths reasonable.
    validate(configData, inputData);
    validateSep(configData, inputData);
    validateTc(configData, inputData);

    /// - Assign attributes from config & input data.
    mMaxConductance       = configData.mMaxConductance;
    mNumSepTypes          = configData.mFluidTypes.size();
    mNumTcTypes           = configData.mTcTypes.size();
    mEffectiveConductance = 0.0;
    mSystemConductance    = 0.0;

    cleanupMemory();
    allocateMemory(configData, inputData);
    initializeFluids();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData   (--) Configuration data.
/// @param[in]  inputData    (--) Input data.
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Multi-Separator link model for
///           paramters not associated with the separated fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::validate(const GunnsFluidMultiSeparatorConfigData& configData,
                                        const GunnsFluidMultiSeparatorInputData&  inputData) const
{
    /// - Throw an exception on insufficient number of ports.
    if (mNumPorts < 3) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "does not have enough ports (must be > 2).");
    }

    /// - Throw an exception on max conductance < 0.
    if (configData.mMaxConductance < FLT_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "max conductance < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData   (--) Configuration data.
/// @param[in]  inputData    (--) Input data.
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Multi-Separator link model for
///           paramters associated with the separated bulk fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::validateSep(const GunnsFluidMultiSeparatorConfigData& configData,
                                           const GunnsFluidMultiSeparatorInputData&  inputData) const
{
    /// - Throw an exception on empty separation and trace compound types vectors.
    const int numTypes   = configData.mFluidTypes.size();
    const int numTcTypes = configData.mTcTypes.size();
    if (numTypes + numTcTypes < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "number of separation + trace compounds types < 1.");
    }

    /// - Throw an exception on size mismatch between separation types and port assignments.
    if (numTypes != static_cast<int>(configData.mFluidPorts.size())) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mismatch in separation types and port assignments vector sizes.");
    }

    /// - Throw an exception on size mismatch between separation types and fractions.
    if (numTypes != static_cast<int>(inputData.mFluidFractions.size())) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "mismatch in separation types and fractions vector sizes.");
    }

    for (int i=0; i<numTypes; ++i) {
        /// - Throw an exception for duplicated separation types.
        for (int j=i+1; j<numTypes; ++j) {
            if (configData.mFluidTypes.at(i) == configData.mFluidTypes.at(j)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "duplicated separation types.");
            }
        }

        /// - Throw an exception on separation port out of bounds.
        if (not MsMath::isInRange(2, configData.mFluidPorts.at(i), mNumPorts-1)) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "a separation port assignment is out of range.");
        }

        /// - Throw an exception on fluid type not in the network.
        try {
            configData.mNodeList->mNodes[0].getContent()->find(configData.mFluidTypes.at(i));
        } catch (const TsOutOfBoundsException& e){
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "a separation fluid type is not in the network.");
        }

        /// - Throw an exception if the separation mass fraction is not in (0-1).
        if (!MsMath::isInRange(0.0, inputData.mFluidFractions.at(i), 1.0)) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "a separation mass fraction is not in (0 to 1).");
        }

        /// - Throw an exception if the fluid type doesn't match the phase of the exit node.
        const DefinedFluidProperties* netFluidProperties = mNodes[0]->getFluidConfig()->mProperties;
        const FluidProperties::FluidPhase sepPhase =
                netFluidProperties->getProperties(configData.mFluidTypes.at(i))->getPhase();
        if (sepPhase != mNodes[configData.mFluidPorts.at(i)]->getContent()->getPhase()) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "fluid phase mismatch between a separated type and its exit node.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData   (--) Configuration data.
/// @param[in]  inputData    (--) Input data.
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Multi-Separator link model for
///           paramters associated with the separated trace compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::validateTc(const GunnsFluidMultiSeparatorConfigData& configData,
                                          const GunnsFluidMultiSeparatorInputData&  inputData) const
{
    /// - Throw an exception on size mismatch between trace compound types and port assignments.
    const int numTcTypes = configData.mTcTypes.size();
    if (numTcTypes != static_cast<int>(configData.mTcPorts.size())) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mismatch in trace compound types and port assignments vector sizes.");
    }

    /// - Throw an exception on size mismatch between trace compound types and fractions.
    if (numTcTypes != static_cast<int>(inputData.mTcFractions.size())) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "mismatch in trace compound types and fractions vector sizes.");
    }

    GunnsFluidTraceCompounds* tc = mNodes[0]->getContent()->getTraceCompounds();
    if (tc) {
        for (int i=0; i<numTcTypes; ++i) {
            /// - Throw an exception for duplicated separation types.
            for (int j=i+1; j<numTcTypes; ++j) {
                if (configData.mTcTypes.at(i) == configData.mTcTypes.at(j)) {
                    GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                            "duplicated trace compound types.");
                }
            }

            /// - Throw an exception on trace compound port out of bounds.
            if (not MsMath::isInRange(2, configData.mTcPorts.at(i), mNumPorts-1)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "a trace compound port assignment is out of range.");
            }

            /// - Throw an exception on trace compound type not in the network.
            try {
                tc->find(configData.mTcTypes.at(i));
            } catch (const TsOutOfBoundsException& e){
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "a trace compound type is not in the network.");
            }

            /// - Throw an exception if the trace compound separation mass fraction is not in (0-1).
            if (!MsMath::isInRange(0.0, inputData.mTcFractions.at(i), 1.0)) {
                GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "a trace compound separation mass fraction is not in (0 to 1).");
            }
        }
    } else if (numTcTypes > 0) {
        /// - Throw an exception if trace compounds specified but there are none in the network.
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                "trace compounds are specified but there are none in the network.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData   (--) Configuration data.
/// @param[in]  inputData    (--) Input data.
///
/// @details  Allocates dynamic arrays and initializes them with config & input data.
///
/// @note  This assumes that config data has already been validated and all fluid & trace compounds
///        types are verified to exist in the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::allocateMemory(const GunnsFluidMultiSeparatorConfigData& configData,
                                              const GunnsFluidMultiSeparatorInputData&  inputData)
{
    /// - Allocate & initialize memory for separation fluids.
    if (mNumSepTypes > 0) {
        TS_NEW_PRIM_ARRAY_EXT(mSepIndex,      mNumSepTypes, int,           mName+".mSepIndex");
        TS_NEW_PRIM_ARRAY_EXT(mSepPort,       mNumSepTypes, int,           mName+".mSepPort");
        TS_NEW_PRIM_ARRAY_EXT(mSepFraction,   mNumSepTypes, double,        mName+".mSepFraction");
        TS_NEW_PRIM_ARRAY_EXT(mSepBufferThru, mNumSepTypes, double,        mName+".mSepBufferThru");
        TS_NEW_PRIM_ARRAY_EXT(mSepBufferExit, mNumSepTypes, double,        mName+".mSepBufferExit");
        TS_NEW_CLASS_ARRAY_EXT(mSepFluid,     mNumSepTypes, PolyFluid, (), mName+".mSepFluid");

        for (int i=0; i<mNumSepTypes; ++i) {
            mSepIndex[i]      = mNodes[0]->getContent()->find(configData.mFluidTypes.at(i));
            mSepPort[i]       = configData.mFluidPorts.at(i);
            mSepFraction[i]   = inputData.mFluidFractions.at(i);
            mSepBufferThru[i] = 0.0;
            mSepBufferExit[i] = 0.0;
        }
    }

    GunnsFluidTraceCompounds* tc = mNodes[0]->getContent()->getTraceCompounds();
    if (tc) {
        const GunnsFluidTraceCompoundsConfigData* tcConfig = tc->getConfig();

        /// - Allocate & initialize memory for separation trace compounds.
        if (mNumTcTypes > 0) {
            TS_NEW_PRIM_ARRAY_EXT(mTcIndex,    mNumTcTypes, int,    mName+".mTcIndex");
            TS_NEW_PRIM_ARRAY_EXT(mTcPort,     mNumTcTypes, int,    mName+".mTcPort");
            TS_NEW_PRIM_ARRAY_EXT(mTcFraction, mNumTcTypes, double, mName+".mTcFraction");

            for (int i=0; i<mNumTcTypes; ++i) {
                mTcIndex[i]    = tc->find(configData.mTcTypes[i]);
                mTcPort[i]     = configData.mTcPorts.at(i);
                mTcFraction[i] = inputData.mTcFractions.at(i);
            }
        }

        /// - Allocate & initialize memory for network trace compounds.
        TS_NEW_PRIM_ARRAY_EXT(mWorkTcMassFlowRates, tcConfig->mNTypes, double, mName+".mWorkTcMassFlowRates");

        for (int i=0; i<tcConfig->mNTypes; ++i) {
            mWorkTcMassFlowRates[i] = 0.0;
        }
    }

    /// - Allocate & initialize memory for network bulk fluid compounds.
    const int nConstituents = mNodes[0]->getContent()->getNConstituents();
    TS_NEW_PRIM_ARRAY_EXT(mWorkMoleFractions, nConstituents, double, mName+".mWorkMoleFractions");

    for (int i=0; i<nConstituents; ++i) {
        mWorkMoleFractions[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes each of the separation fluid objects as 100% mixture of that fluid type.
///           Initializes the base class internal fluid for the bulk through flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::initializeFluids()
{
    for (int i=0; i<mNumSepTypes; ++i) {
        /// - Mass fractions array as 100% this separation fluid.
        double* massFractions = new double[mNodes[0]->getContent()->getNConstituents()];
        for (int j=0; j<mNodes[0]->getContent()->getNConstituents(); ++j) {
            massFractions[j] = 0.0;
        }
        massFractions[mSepIndex[i]] = 1.0;

        /// - Since the separation fluid has already been default constructed, we must init it
        ///   with its name and config & input data objects.
        std::ostringstream fluidName;
        fluidName << mName << ".mSepFluid_" << i;
        mSepFluid[i].initializeName(fluidName.str());

        /// - Copy construct config data for this fluid from the node's fluid config.
        PolyFluidConfigData fluidConfig(*mNodes[0]->getFluidConfig());

        PolyFluidInputData fluidInput(mNodes[0]->getContent()->getTemperature(),
                                      mNodes[0]->getContent()->getPressure(),
                                      0.0,
                                      0.0,
                                      massFractions,
                                      0);
        mSepFluid[i].initialize(fluidConfig, fluidInput);
        delete [] massFractions;
    }

    /// - Create the internal fluid.
    createInternalFluid();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes (currently there are none.)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Computes this link's contributions to the network system of equations, models a normal
///           conductive flow path between Ports 0 & 1 similar to a GunnsFluidConductor, and source
///           flows to the exit ports for the separated fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Set the Link Effective Conductance based on the effective conductivity and the blockage
    ///   fraction.
    mEffectiveConductance = mMaxConductance;
    if (mMalfBlockageFlag) {
        mEffectiveConductance *= (1.0 - mMalfBlockageValue);
    }

    mSystemConductance = MsMath::limitRange(0.0, linearizeConductance(), mConductanceLimit);

    /// - Build the link admittance matrix.  This is the same as a regular fluid conductor with
    ///   the through-flow conductance between ports 0 and 1, except the matrix is larger because
    ///   of the separated fluid exit ports.
    if (fabs(mAdmittanceMatrix[0] - mSystemConductance) > 0.0) {
        mAdmittanceMatrix[0]           =  mSystemConductance;
        mAdmittanceMatrix[1]           = -mSystemConductance;
        mAdmittanceMatrix[mNumPorts]   = -mSystemConductance;
        mAdmittanceMatrix[mNumPorts+1] =  mSystemConductance;
        mAdmittanceUpdate = true;
    }

    /// - Build the link source vector.  Each separated fluid flow from last pass is added to its
    ///   respective exit port, and subtracted from the downstream port of the bulk through-flow.
    mSourceVector[0] = 0.0;
    mSourceVector[1] = 0.0;
    for (int i=0; i<mNumSepTypes; ++i) {
        if (mPotentialVector[0] >= mPotentialVector[1]) {
            mSourceVector[1] -= mSepBufferThru[i];
        } else {
            mSourceVector[0] -= mSepBufferThru[i];
        }
        mSepBufferExit[i] = mSepBufferThru[i];
        mSourceVector[mSepPort[i]] = mSepBufferExit[i];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (kg*mol/kPa/s)  Linearized molar conductance.
///
/// @details  Applies a linearization of the fluid momentum equation to create the non-linear
///           relationship between flow and pressure over multiple network steps.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidMultiSeparator::linearizeConductance()
{
    return GunnsFluidUtils::computeAdmittance(mEffectiveConductance,
                                              mMinLinearizationPotential,
                                              mNodes[0]->getContent(),
                                              mNodes[1]->getContent());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (--) Not used.
///
/// @details  Computes flow rates from the network solution, including bulk through-flow and the
///           separated fluid rates.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    /// - Update bulk molar flow rate, and molar flow rate across the membrane.
    ///   mFlowRate and mFlux are the flow into the link from the upstream through-flow port.
    ///   The actual flow rate into the downstream port is this less the separated compound flows.
    computeFlux();

    const int upstreamPort   = determineSourcePort(mFlux, 0, 1);
    const int downstreamPort = 1 - upstreamPort;
    mFlowRate    = mFlux * mNodes[upstreamPort]->getOutflow()->getMWeight();
    mVolFlowRate = mFlowRate / std::max(DBL_EPSILON,
                   mNodes[upstreamPort]->getOutflow()->getDensity());
    mPower       = -UnitConversion::PA_PER_KPA * mVolFlowRate * mPotentialDrop;

    /// - Set bulk flow port flow directions.
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;
    if (fabs(mFlux) > DBL_EPSILON) {
        mPortDirections[upstreamPort]   = SOURCE;
        mPortDirections[downstreamPort] = SINK;
    }

    /// - Separation flow rates for each separation fluid as a fraction of the incoming bulk flow,
    ///   and separation port flow directions.  To minimize state error in the nodes, we try to
    ///   flow the amount reflecting the network solution this pass as much as possible.  We buffer
    ///   the flow rates to sync up the transport with the network solution.  This scheme results
    ///   in a small amount of state error in the downstream node, but no errors in the upstream
    ///   or separation exit nodes.
    /// - mSepBufferThru is the flow that is removed from the bulk flow inlet and before
    ///   giving the bulk flow the downstream node this pass, and reflected in the admittance matrix
    ///   next pass.
    /// - mSepBufferExit is the flow that was removed from the bulk flow last pass, reflected
    ///   in the source vector this pass, and added to the exit ports this pass.
    for (int i=0; i<mNumSepTypes; ++i) {
        mSepBufferThru[i] = mSepFraction[i] * fabs(mFlux)
                * mNodes[upstreamPort]->getOutflow()->getMoleFraction(mSepIndex[i]);
        if (mSepBufferExit[i] > DBL_EPSILON) {
            mPortDirections[mSepPort[i]] = SOURCE;
        } else {
            mPortDirections[mSepPort[i]] = NONE;
        }
    }

    /// - Schedule flow from the source node.
    if (mFlux > DBL_EPSILON) {
        mNodes[upstreamPort]->scheduleOutflux(mFlux);
    } else if (mFlux < -DBL_EPSILON) {
        mNodes[upstreamPort]->scheduleOutflux(-mFlux);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the molar flux rate of the bulk flow into the link before any fluids are
///           separated.  This is the same as GunndFluidConductor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::computeFlux()
{
    const double hiP = std::max(mPotentialVector[0], mPotentialVector[1]);
    if (fabs(mPotentialDrop) < (hiP * m100EpsilonLimit)) {
        /// - Zero flux if dP is too low.  This eliminates most mass loss/creation due to rounding
        ///   error in the solver.
        mFlux = 0.0;
    } else {
        mFlux = mPotentialDrop * mAdmittanceMatrix[0];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (--) Not used.
///
/// @details  Transports flows to and from the nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMultiSeparator::transportFlows(const double dt __attribute__((unused)))
{
    const int upstreamPort   = determineSourcePort(mFlux, 0, 1);
    const int downstreamPort = 1 - upstreamPort;
    const double temperature = mNodes[upstreamPort]->getOutflow()->getTemperature();

    /// - The separation fluids added to the exit nodes use the buffered separation flows from last
    ///   pass, which were reflected in the link source vector this pass.
    for (int port=2, fluid=0; port<mNumPorts; ++port, ++fluid) {
        mSepFluid[fluid].setTemperature(temperature);
        const double mdot = mSepBufferExit[fluid] * mSepFluid[fluid].getMWeight();
        if (mdot > DBL_EPSILON) {
            mNodes[port]->collectInflux(mdot, &mSepFluid[fluid]);
        }
    }

    const double flux = fabs(mFlux);
    if (flux > DBL_EPSILON) {
        mInternalFluid->setState(mNodes[upstreamPort]->getOutflow());
        mNodes[upstreamPort]->collectOutflux(fabs(mFlowRate));

        /// - Move trace compounds from the bulk fluid to their exit nodes.
        GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
        if (tc and mNumTcTypes > 0) {
            const GunnsFluidTraceCompoundsConfigData* tcConfig = tc->getConfig();
            for (int i=0; i<tcConfig->mNTypes; i++) {
                mWorkTcMassFlowRates[i] = flux * tc->getMoleFractions()[i] * tcConfig->mCompounds[i]->mMWeight;
            }
            for (int i=0; i<mNumTcTypes; ++i) {
                const double tcSepRate = mTcFraction[i] * mWorkTcMassFlowRates[mTcIndex[i]];
                mWorkTcMassFlowRates[mTcIndex[i]] -= tcSepRate;
                static_cast<GunnsFluidNode*>(mNodes[mTcPort[i]])->collectTc(mTcIndex[i], tcSepRate);
            }
            tc->setMasses(mWorkTcMassFlowRates);
        }

        /// - The bulk through flow has the separated fluids removed from it before being given to
        ///   the downstream node, using the separation flow rates from this pass, which will be
        ///   reflected in the link admittance matrix next pass.  This creates state error in the
        ///   downstream node this pass, but this error is corrected later.
        int nConstituents = mNodes[0]->getContent()->getNConstituents();
        double exitFlux = flux;
        if (mNumSepTypes > 0) {
            for (int i=0; i<nConstituents; ++i) {
                /// - At this point moleFractions is molar flow rate, not fractions yet.
                mWorkMoleFractions[i] = flux * mInternalFluid->getMoleFraction(i);
            }
            for (int i=0; i<mNumSepTypes; ++i) {
                mWorkMoleFractions[mSepIndex[i]] -= mSepBufferThru[i];
                exitFlux                         -= mSepBufferThru[i];
            }
            if (exitFlux > DBL_EPSILON) {
                /// - Now moleFractions is normalized back into fractions.
                for (int i=0; i<nConstituents; ++i) {
                    mWorkMoleFractions[i] /= exitFlux;
                }
                mInternalFluid->setMoleAndMoleFractions(exitFlux, mWorkMoleFractions);
                mInternalFluid->setTemperature(temperature);
            }
        }

        /// - Update trace compound mole fractions relative to the moles remaining in the
        ///   internal fluid, before given to the downstream node.
        if (tc and mNumTcTypes > 0) {
            tc->updateMoleFractions();
        }
        mNodes[downstreamPort]->collectInflux(exitFlux * mInternalFluid->getMWeight(), mInternalFluid);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) Index of the fluid type in the separated fluid types array.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the index of the given fluid type in our separated types.  Note this is not
///           the same as the index in the network's fluid types.  Throws exception if the given
///           type is not in the network or not one of our separated types.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidMultiSeparator::findIndexOfType(const FluidProperties::FluidType type) const
{
    try {
        const int index = mInternalFluid->find(type);
        for (int i=0; i<mNumSepTypes; ++i) {
            if (index == mSepIndex[i]) {
                return i;
            }
        }
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Argument Range",
                    "type is not separated by this link.");
    } catch (TsOutOfBoundsException& e) {
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Argument Range",
                    "type is not in the network.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) Index of the type in the separated trace compound types array.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the index of the given trace compound type in our separated types.  Note this
///           is not the same as the index in the network's trace compound types.  Throws exception
///           if the given type is not in the network or not one of our separated types.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidMultiSeparator::findIndexOfTc(const ChemicalCompound::Type type) const
{
    GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        try {
            const int index = tc->find(type);
            for (int i=0; i<mNumTcTypes; ++i) {
                if (index == mTcIndex[i]) {
                    return i;
                }
            }
            GUNNS_ERROR(TsOutOfBoundsException, "Invalid Argument Range",
                    "type is not separated by this link.");
        } catch (TsOutOfBoundsException& e) {
            GUNNS_ERROR(TsOutOfBoundsException, "Invalid Argument Range",
                    "type is not in the network.");
        }
    } else {
        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Argument Range",
                "there are no trace compounds in the network.");
    }
}
