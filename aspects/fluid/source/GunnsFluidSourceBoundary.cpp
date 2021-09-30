/**
@file
@brief    GUNNS Fluid Source Boundary Link implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (core/GunnsFluidLink.o)
    (core/GunnsFluidNode.o)
    (aspects/fluid/fluid/PolyFluid.o)
    (simulation/hs/TsHsMsg.o)
    (software/exceptions/TsInitializationException.o)
   )
*/

#include "aspects/fluid/source/GunnsFluidSourceBoundary.hh"
#include "aspects/fluid/fluid/PolyFluid.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name               (--) Link name.
/// @param[in] nodes              (--) Network nodes array.
/// @param[in] flipFlowSign       (--) True makes positive flow rate go out of the node.
/// @param[in] traceCompoundsOnly (--) True only flows trace compounds, not bulk fluid.
///
/// @details  Default GUNNS Fluid Source Boundary Config Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceBoundaryConfigData::GunnsFluidSourceBoundaryConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const bool         flipFlowSign,
        const bool         traceCompoundsOnly)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mFlipFlowSign(flipFlowSign),
    mTraceCompoundsOnly(traceCompoundsOnly)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  GUNNS Fluid Source Boundary Config Data Copy Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceBoundaryConfigData::GunnsFluidSourceBoundaryConfigData(
        const GunnsFluidSourceBoundaryConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mFlipFlowSign(that.mFlipFlowSign),
    mTraceCompoundsOnly(that.mTraceCompoundsOnly)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Source Boundary Config Data Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceBoundaryConfigData::~GunnsFluidSourceBoundaryConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag   (--)   Blockage malfunction flag.
/// @param[in] malfBlockageValue  (--)   Blockage malfunction fractional value (0-1).
/// @param[in] flowDemand         (kg/s) Initial demanded mass flow rate of the link.
/// @param[in] internalFluid      (--)   Initial fluid properties of the link flow.
///
/// @details  Default constructs this Fluid Source Boundary input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceBoundaryInputData::GunnsFluidSourceBoundaryInputData(
        const bool                         malfBlockageFlag,
        const double                       malfBlockageValue,
        const double                       flowDemand,
        PolyFluidInputData*                internalFluid)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mFlowDemand(flowDemand),
    mInternalFluid(internalFluid)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Copy constructs this Fluid Source Boundary input data.  This is a shallow copy.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceBoundaryInputData::GunnsFluidSourceBoundaryInputData(
        const GunnsFluidSourceBoundaryInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mFlowDemand(that.mFlowDemand),
    mInternalFluid(that.mInternalFluid)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Source Boundary input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceBoundaryInputData::~GunnsFluidSourceBoundaryInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs the Link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceBoundary::GunnsFluidSourceBoundary()
    :
    GunnsFluidLink(NPORTS),
    mFlipFlowSign(false),
    mTraceCompoundsOnly(false),
    mFlowDemand(0.0),
    mTraceCompoundRates(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructs the Link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceBoundary::~GunnsFluidSourceBoundary()
{
    /// - Delete the trace compounds array.
    TS_DELETE_ARRAY(mTraceCompoundRates);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Fluid Source Boundary Config Data.
/// @param[in]     inputData    (--) GUNNS Fluid Source Boundary Input Data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Network port 0.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Source Boundary with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSourceBoundary::initialize(const GunnsFluidSourceBoundaryConfigData& configData,
                                          const GunnsFluidSourceBoundaryInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          const int                                 port0)
{
    /// - Initialize & validate parent.
    GunnsFluidLink::initialize(configData, inputData, networkLinks, &port0);

    /// - Reset init flag.
    mInitFlag           = false;

    /// - Validate initialization.
    validate(configData, inputData);

    /// - Initialize from config data.
    mFlipFlowSign       = configData.mFlipFlowSign;
    mTraceCompoundsOnly = configData.mTraceCompoundsOnly;

    /// - Initialize from input data.
    mFlowDemand         = inputData.mFlowDemand;
    createInternalFluid(*inputData.mInternalFluid);

    /// - Initialize the trace compounds array.
    if (GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds()) {
        const int tcNtypes = tc->getConfig()->mNTypes;
        TS_NEW_PRIM_ARRAY_EXT(mTraceCompoundRates, tcNtypes, double, mName + ".mTraceCompoundRates");

        for (int i = 0; i < tcNtypes; ++i) {
            if (inputData.mInternalFluid->mTraceCompounds) {
                mTraceCompoundRates[i] = inputData.mInternalFluid->mTraceCompounds->mState[i];
            } else {
                mTraceCompoundRates[i] = 0.0;
            }
        }
    }

    /// - Set init flag on successful validation.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  GUNNS Fluid Source Boundary Config Data.
/// @param[in]  inputData   (--)  GUNNS Fluid Source Boundary Input Data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Source Boundary initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSourceBoundary::validate(const GunnsFluidSourceBoundaryConfigData& configData,
                                        const GunnsFluidSourceBoundaryInputData&  inputData) const
{
    /// - Throw an exception if the internal fluid was not supplied.
    if (!inputData.mInternalFluid) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Internal Fluid not specified.");
    }

    /// - Throw an exception for Trace Compounds Only mode but the network has null trace compounds
    ///   config.
    if (configData.mTraceCompoundsOnly and
        !mNodes[0]->getContent()->getTraceCompounds()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Network has no trace compounds for trace compounds only mode.");
    }

    /// - Throw an exception for Trace Compounds Only mode but the input data has null trace
    ///   compounds rates.
    if (configData.mTraceCompoundsOnly and !inputData.mInternalFluid->mTraceCompounds) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Trace compound rates not provided for trace compounds only mode.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSourceBoundary::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details Updates the link during the time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSourceBoundary::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Call the virtual updateState method so that any derived class may modify the behavior.
    updateState(dt);

    /// - Call the virtual getFlowDemand method so that any derived class can control the flow rate.
    mFlowRate = getFlowDemand();

    /// - We flip the sign of the flow demand into the flow rate, and then all other terms are
    ///   always relative to the sign convention of positive flow goes into the node.
    if (mFlipFlowSign) {
        mFlowRate *= -1.0;
    }

    /// - Reduce the actual flow rate by the blockage malfunction if it is active.
    if (mMalfBlockageFlag) {
        mFlowRate *= (1.0 - mMalfBlockageValue);
    }

    /// - Convert the mass flow rate demand to molar flow rate based on the molecular weight of the
    ///   source fluid.  Molar flow rate is zeroed in TC-only mode since there will be no affect on
    ///   bulk fluid in the node.
    const double sourceMWeight = mInternalFluid->getMWeight();
    if (mTraceCompoundsOnly or sourceMWeight < DBL_EPSILON) {
        mFlux = 0.0;
    } else {
        mFlux = mFlowRate / sourceMWeight;
    }

    buildSource();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Computes the potential drop and port direction across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSourceBoundary::computeFlows(const double dt __attribute__((unused)))
{
    /// - Calculate potential drop.  Since the source of the node is assumed to be Ground with
    ///   potential = 0, flows into the node cause a potential rise and therefore negative potential
    ///   drop.
    if (mFlux < 0.0) {
        mPotentialDrop =  mPotentialVector[0];
    } else {
        mPotentialDrop = -mPotentialVector[0];
    }

    /// - Set port flow directions and schedule flow from source nodes.
    if (fabs(mFlux) > DBL_EPSILON) {
        mPortDirections[0] = SINK;
    } else {
        mPortDirections[0] = NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  transports the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSourceBoundary::transportFlows(const double dt)
{
    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   internal fluid.
    const double sourceDensity = mInternalFluid->getDensity();
    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power.
    computePower();

    /// - Call the virtual updateFluid method to allow derived classes to further modify the
    ///   internal fluid before it is transported.
    updateFluid(dt, mFlowRate);

    /// - Update the flow rates of the trace compounds that will be given to the node.
    GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        const GunnsFluidTraceCompoundsConfigData& tcConfig = *tc->getConfig();
        if (mTraceCompoundsOnly and fabs(mFlowRate) > DBL_EPSILON) {
            /// - In TC-only mode, TC's are given directly to/from the node's via its collectTc
            ///   function.
            for (int i = 0; i < tc->getConfig()->mNTypes; ++i) {
                static_cast<GunnsFluidNode*>(mNodes[0])->collectTc(i, mFlowRate * mTraceCompoundRates[i]);
            }
        } else {
            /// - When there are bulk fluid flows, TC's are added to the bulk fluid, and will be
            ///   transported within the bulk fluid to the node in transportFluid() below.
            for (int i = 0; i < tcConfig.mNTypes; ++i) {
                tc->setMass(i, mFlowRate * mTraceCompoundRates[i]);
            }
            tc->updateMoleFractions();
        }
    }

    /// - Transport the internal fluid to/from the attached node.
    transportFluid(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] forcedOutflow (--) Not used.
/// @param[in] fromPort      (--) Not used.
/// @param[in] toPort        (--) Not used.
///
/// @details  This function adds the link's flow to the attached node's collection terms.
///           Flow is defined positive going to the node, so a positive flow will add mass to it.
///
///           This implementation is simplified from the base class in several important ways:
///           - Since this is a 1-port link, the port arguments are ignored and all flows go to/from
///             the port 0 node.
///           - This method doesn't verify the port arguments since they're ignored.
///           - This link always has the internal fluid specified, so we don't check for it.
///           - This link always forces mixture change of the node for outflows, so we ignore the
///             forcedOutflow argument.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSourceBoundary::transportFluid(const bool forcedOutflow __attribute__((unused)),
                                              const int  fromPort __attribute__((unused)),
                                              const int  toPort __attribute__((unused)))
{
    /// - Minimum mass flow rate for transport.  Flow rates in links below this value are
    ///   not transported between nodes.  Math faults in the PolyFluid class can result when
    ///   mass or molar flow rates less than DBL_EPSILON are attempted, and this limit
    ///   value keeps both mass and molar flow rates above DBL_EPSILON.
    ///
    /// - Both positive and negative flows are added to the node's influx collection.  Since this
    ///   link always forces outflow of a specific mixture, we have to use the collectInflux method
    ///   with a negative flow rate.
    ///
    /// - In TC-only mode, TC flows to/from the node were transported in transportFlows..
    if (not mTraceCompoundsOnly and (fabs(mFlowRate) > m100EpsilonLimit)) {
        mNodes[0]->collectInflux (mFlowRate, mInternalFluid);
    }
}
