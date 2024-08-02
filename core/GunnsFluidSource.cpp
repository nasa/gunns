/**
@file
@brief    GUNNS Fluid Source Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid Source)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (GunnsFluidLink.o)
    (GunnsFluidNode.o)
    (aspects/fluid/fluid/PolyFluid.o)
   )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
     (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
    )
*/
#include "GunnsFluidSource.hh"
#include "aspects/fluid/fluid/PolyFluid.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name  (--) Link name
/// @param[in] nodes (--) Network nodes array
///
/// @details  Default GUNNS Fluid Source Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceConfigData::GunnsFluidSourceConfigData(const std::string& name,
                                                       GunnsNodeList*     nodes)
    :
    GunnsFluidLinkConfigData(name, nodes)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Fluid Source Config Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceConfigData::GunnsFluidSourceConfigData(const GunnsFluidSourceConfigData& that)
    :
    GunnsFluidLinkConfigData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Source Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceConfigData::~GunnsFluidSourceConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--)   Blockage malfunction flag
/// @param[in] malfBlockageValue (--)   Blockage malfunction fractional value (0-1)
/// @param[in] flowDemand        (kg/s) Initial demanded mass flow rate of the link
///
/// @details  Default constructs this Fluid Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceInputData::GunnsFluidSourceInputData(const bool   malfBlockageFlag,
                                                     const double malfBlockageValue,
                                                     const double flowDemand)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mFlowDemand(flowDemand)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Fluid Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceInputData::GunnsFluidSourceInputData(const GunnsFluidSourceInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mFlowDemand(that.mFlowDemand)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSourceInputData::~GunnsFluidSourceInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSource::GunnsFluidSource()
    :
    GunnsFluidLink(NPORTS),
    mFlowDemand(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructs the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSource::~GunnsFluidSource()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Fluid Source Config Data
/// @param[in]     inputData    (--) GUNNS Fluid Source Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Source with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSource::initialize(const GunnsFluidSourceConfigData& configData,
                                  const GunnsFluidSourceInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&     networkLinks,
                                  const int                         port0,
                                  const int                         port1)
{
    /// - Initialize & validate parent.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize from config data.
    mFlowDemand = inputData.mFlowDemand;

    /// - Validate initialization.
    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates this GUNNS Fluid Source initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSource::validate() const
{
    /// - There is currently nothing to validate.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSource::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates the link during the time step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSource::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - This class creates a flow rate between two nodes.  One of the nodes may be vacuum, or both
    ///   may be normal network nodes.  Flow direction is defined positive from Port0 to Port1, so
    ///   it is best to use Port0 as the VACUUM node.
    ///
    /// - The flow mixture can either be the natural mixture resulting from node-to-node flow, OR it
    ///   can be forced to have the mixture contained in the optional mInternalFluid object,
    ///   whether mInternalFluid is defined or NULL.  This way, we can do fancy things like only
    ///   move the O2 from Node0 to Node1, or move CO2 one way and N2 the opposite way, etc.
    updateState(dt);

    /// - Call the virtual getFlowDemand method so that any derived class can control the flow rate.
    ///   Reduce the actual flow rate by the blockage malfunction if it is active.
    ///
    /// - The demanded mass flow rate may differ from the actual mass flow rate delivered from the
    ///   network solution.  This will happen if the source node changes mixture due to overflow
    ///   during this step.
    double flowRate = getFlowDemand();
    if (mMalfBlockageFlag) {
        flowRate *= (1.0 - mMalfBlockageValue);
    }

    /// - Convert the mass flow rate demand to molar flow rate based on the molecular weight of the
    //    source fluid.
    double sourceMWeight = 0.0;

    if (mInternalFluid)  {
        sourceMWeight = mInternalFluid->getMWeight();
    } else {
        sourceMWeight = mNodes[determineSourcePort(flowRate, 0, 1)]->getOutflow()->getMWeight();
    }

    if (sourceMWeight > DBL_EPSILON) {
        mFlux = flowRate / sourceMWeight;
    } else {
        mFlux = 0.0;
    }

    buildSource();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Computes the flow rate across this link and schedules flow demand from source nodes.
///           Mole flow rate (mFlux) is already calculated in the step method as the input to the
///           system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSource::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    /// - Set port flow directions and schedule flow from source nodes.
    if (mFlux > DBL_EPSILON) {
        mPortDirections[0] = SOURCE;
        mPortDirections[1] = SINK;
        mNodes[0]->scheduleOutflux(mFlux);
    } else if (mFlux < -DBL_EPSILON) {
        mPortDirections[0] = SINK;
        mPortDirections[1] = SOURCE;
        mNodes[1]->scheduleOutflux(-mFlux);
    } else {
        mPortDirections[0] = NONE;
        mPortDirections[1] = NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates final flow properties and transports fluid from source to sink nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSource::transportFlows(const double dt)
{
    /// - Convert the molar flow rate to mass rate using the molecular weight of the source node.
    int    sourcePort    = determineSourcePort(mFlux, 0, 1);
    double sourceDensity =         mNodes[sourcePort]->getOutflow()->getDensity();
    mFlowRate            = mFlux * mNodes[sourcePort]->getOutflow()->getMWeight();

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   source node.
    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power.
    computePower();

    /// - If the derived model has declared that it modifies the fluid passing through it (by
    ///   instantiating mInternalFluid), then call the derived model to do the update.
    if (mInternalFluid) {
        updateFluid(dt, mFlowRate);
    }

    transportFluidMixtureOption();
}
