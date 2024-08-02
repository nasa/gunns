/**
@file
@brief    GUNNS Fluid Capacitor Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
    (Provides the classes for modeling the GUNNS fluid capacitor.)

 REQUIREMENTS:
    ()

 REFERENCE:
    ()

 ASSUMPTIONS AND LIMITATIONS:
    ()
LIBRARY DEPENDENCY:
    (
     (GunnsFluidLink.o)
    )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
     (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
    )
*/

#include "GunnsFluidCapacitor.hh"
#include "GunnsFluidUtils.hh"
#include <cmath>
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Link name
/// @param[in] nodes                (--) Network nodes array
/// @param[in] expansionScaleFactor (--) Scale factor for isentropic gas cooling (0-1)
///
/// @details  Default GUNNS Fluid Capacitor Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCapacitorConfigData::GunnsFluidCapacitorConfigData(const std::string& name,
                                                             GunnsNodeList*    nodes,
                                                             const double      expansionScaleFactor)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mExpansionScaleFactor(expansionScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Fluid Capacitor Config Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCapacitorConfigData::GunnsFluidCapacitorConfigData(
        const GunnsFluidCapacitorConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mExpansionScaleFactor(that.mExpansionScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Capacitor Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCapacitorConfigData::~GunnsFluidCapacitorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag   (--) Blockage malfunction flag
/// @param[in] malfBlockageValue  (--) Blockage malfunction fractional value (0-1)
/// @param[in] initialVolume      (m3) Maximum possible effective conductivity of the link
/// @param[in] initialFluidState  (--) Initial state of the port 0 node contents
///
/// @details  Default GUNNS Fluid Capacitor Input Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCapacitorInputData::GunnsFluidCapacitorInputData(const bool          malfBlockageFlag,
                                                           const double        malfBlockageValue,
                                                           const double        initialVolume,
                                                           PolyFluidInputData* initialFluidState)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mInitialVolume(initialVolume),
    mInitialFluidState(initialFluidState)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Fluid Capacitor Input Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCapacitorInputData::GunnsFluidCapacitorInputData(const GunnsFluidCapacitorInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mInitialVolume(that.mInitialVolume),
    mInitialFluidState(that.mInitialFluidState)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Capacitor Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCapacitorInputData::~GunnsFluidCapacitorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Capacitor Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCapacitor::GunnsFluidCapacitor():
    GunnsFluidLink(NPORTS),
    mCapacitance(0.0),
    mExpansionScaleFactor(0.0),
    mDisablePressureCorrection(false),
    mEditVolumeFlag(false),
    mEditVolume(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Capacitor Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCapacitor::~GunnsFluidCapacitor()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Fluid Capacitor Config Data
/// @param[in]     inputData    (--) GUNNS Fluid Capacitor Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Capacitor with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::initialize(const GunnsFluidCapacitorConfigData& configData,
                                     const GunnsFluidCapacitorInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        networkLinks,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - Initialize & validate parent.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate initialization data.
    validate(configData, inputData);

    /// - Initialize from config and input data.
    mExpansionScaleFactor = configData.mExpansionScaleFactor;
    mNodes[0]->setExpansionScaleFactor(mExpansionScaleFactor);
    mNodes[0]->setVolume(inputData.mInitialVolume);
    initializeFluid(*inputData.mInitialFluidState);
    mCapacitance    = 0.0;
    mDisablePressureCorrection = false;
    mEditVolumeFlag = false;
    mEditVolume     = 0.0;

    /// - Set init flag on successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] initialFluidState (--) Fluid Initial State
///
/// @details Protected Method for Initializing the Link Fluid State. All fluid capacitors must
///          initialize the fluid properties, so we force an implementation here.  The capacitor is
///          always defined to act upon the port 0 fluid, which is owned by the GunnsFluidNode.
///          The fluid object already exists, we just need to initialize its state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::initializeFluid(const PolyFluidInputData& initialFluidState)
{
    /// - Copy construct a local fluid from the original node 0 contents, then load that fluid with
    ///   the desired initial properties.
    PolyFluid initialFluid(*mNodes[0]->getContent(), std::string(mName) + ".TEMPORARY");
    initialFluid.resetState();
    initialFluid.setMassAndMassFractions(0.0, initialFluidState.mMassFraction);
    initialFluid.setTemperature(initialFluidState.mTemperature);
    GunnsFluidTraceCompounds* traceCompounds = initialFluid.getTraceCompounds();
    if (traceCompounds and initialFluidState.mTraceCompounds) {
        traceCompounds->setMoleFractions(initialFluidState.mTraceCompounds->mState);
    }

    /// - Clear out the original node 0 contents and replace them with the local fluid.
    mNodes[0]->getContent()->resetState();
    mNodes[0]->getContent()->addState(&initialFluid, DBL_EPSILON);

    /// - Initialize remaining node parameters.
    mNodes[0]->setPotential(initialFluidState.mPressure);
    mNodes[0]->getContent()->setTemperature(initialFluidState.mTemperature);
    mPotentialVector[0] = mNodes[0]->getPotential();
    mNodes[0]->updateMass();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) GUNNS Fluid Capacitor Config Data
/// @param[in] inputData  (--) GUNNS Fluid Capacitor Input Data
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Capacitor initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::validate(const GunnsFluidCapacitorConfigData& configData,
                                   const GunnsFluidCapacitorInputData&  inputData) const
{
    /// - Issue an error on volume < 0.
    if (inputData.mInitialVolume < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Link has volume < 0.");
    }

    /// - Issue an error on no input fluid properties.
    if (!inputData.mInitialFluidState) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Link is missing initial fluid properties.");
    }

    /// - Issue an error on expansion scale factor < 0 or > 1.
    if (!MsMath::isInRange(0.0, configData.mExpansionScaleFactor, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has expansion scale factor < 0 or > 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mCapacitance         = 0.0;
    mEditVolumeFlag      = false;
    mEditVolume          = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details Method to update the link during a time step. For a fluid capacitor, like a Tank or
///          any volume, we define the actual capacitive node to be Nodes[0], and Nodes[1] is set
///          as a zero potential node.  Make sure to always map Nodes[0] as the volume object node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.  Note that you should zero out the
    ///   volume before removing a capacitive link from a node.
    processUserPortCommand();

    /// - Process a volume edit.  The edit re-initializes the node volume, leaving density unchanged
    ///   (mass is re-calculated), and incurring no compression effects.
    if (mEditVolumeFlag) {
        mEditVolumeFlag = false;
        mNodes[0]->initVolume(mEditVolume);
    }

    /// - Call the virtual updateState method so a derived model can calculate a new volume.
    mNodes[0]->setExpansionScaleFactor(mExpansionScaleFactor);
    updateState(dt);

    /// - Set the new capacitance based on the volume.
    mNodes[0]->setVolume(getVolume());
    mCapacitance = GunnsFluidUtils::computeCapacitance(mNodes[0]->getContent(), getVolume());

    /// - Calculate and apply the pressure correction.
    if (!mDisablePressureCorrection) {
        mPotentialVector[0] += mNodes[0]->computePressureCorrection();
    }

    /// - Build this link's contributions to the system of equations, and add the compression and
    ///   thermal effects to the source vector.  We only add the thermal contribution if dt is not
    ///   zero.
    buildAdmittanceMatrix(dt);
    buildSourceVector(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Computes the flow rate across this link and schedules flow demand from source nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();
    mFlux = mPotentialDrop * mAdmittanceMatrix[0] - mSourceVector[0];

    /// - Set port flow directions and schedule flow from source nodes.  Since this link never
    ///   actually transports fluid to or from the nodes, we can leave the port directions as NONE
    ///   and skip scheduling outflux to nodes.  Our mFlux ins't its own flow, but reflects the net
    ///   moles content change of the node due to other flows in or out.
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Updates final flow properties and transports fluid from source to sink nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::transportFlows(const double dt)
{
    /// - Call virtual updateFluid class so the derived model can calculate heat flux to the fluid.
    updateFluid(dt, mFlowRate);
    mNodes[0]->collectHeatFlux(getHeatFlux());
    mNodes[0]->setUndampedHeatFlux(getUndampedHeatFlux());

    /// - Call the base class to transport the fluid between the nodes.  The true bool argument
    ///   indicates that this class can force a mixture change in the supplying node.  This class
    ///   forces flow rate to zero.  However, this method call is to support derived classes that
    ///   may create a flow rate through the link.
    transportFluid(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  This method builds the link's mass capacitance into the admittance matrix, applied to
///           the node at port 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::buildAdmittanceMatrix(const double dt)
{
    double admittance;

    /// - Protect for divide by zero in dt, which will occur when running in Freeze.  In Freeze,
    ///   dividing capacitance by DBL_EPSILON makes the node capacitance in the system of
    ///   equations essentially infinite, which will freeze node potential in place.
    if (dt < DBL_EPSILON) {
        admittance = mCapacitance / DBL_EPSILON;
    }
    else {
        admittance = mCapacitance / dt;
    }

    /// - Build the system admittance matrix contribution.
    if (fabs(mAdmittanceMatrix[0] - admittance) > 0.0) {
        mAdmittanceMatrix[0] = admittance;
        mAdmittanceMatrix[1] = 0.0;
        mAdmittanceMatrix[2] = 0.0;
        mAdmittanceMatrix[3] = 0.0;
        mAdmittanceUpdate = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  This method builds the link's mass capacitance, thermal capacitance, and compression
///           effect into the source vector, applied to the node at port 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCapacitor::buildSourceVector(const double dt)
{
    /// - Add the node's capacitance contribution to the source vector.
    mSourceVector[0] = (mPotentialVector[0] - mPotentialVector[1]) * mAdmittanceMatrix[0];
    mSourceVector[1] = 0.0;

    /// - Add the node's thermal capacitance and compression effect to the source vector.
    mSourceVector[0] += mNodes[0]->computeCompression();
    if (dt > DBL_EPSILON) {
        mSourceVector[0] += mNodes[0]->computeThermalCapacitance() / dt;
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
///           - A GunnsFluidCapacitor must not map port 0 to the network's vacuum boundary node.
///           - A GunnsFluidCapacitor must map port 1 to the network's vacuum boundary node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidCapacitor::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is the vacuum boundary node.
    if ((0 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to the boundary node.");
        result = false;
    }

    /// - Fail if port 1 is not the vacuum boundary node.
    else if ((1 == port) && (node != getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: must assign port 1 to the boundary node.");
        result = false;
    }

    return result;
}
