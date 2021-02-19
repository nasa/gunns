/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid Equivalent-Circuit Conductor.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
    (
     (core/GunnsFluidLink.o)
    )

PROGRAMMERS:
   ((Jason Harvey) (L3) (2013-01) (Initial Prototype))
**************************************************************************************************/
#include "GunnsFluidEqConductor.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details  This value is chosen to get reliable network capacitance calculations from the solver
///           for liquid and gas nodes.
const double GunnsFluidEqConductor::mNetworkCapacitanceFlux = 1.0E-6;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                  (--) Link name.
/// @param[in] nodes                 (--) Network nodes array.
/// @param[in] useNetworkCapacitance (--) Causes network capacitances to be available for output to external demands.
///
/// @details  Default GUNNS Fluid Equivalent-Circuit Conductor Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEqConductorConfigData::GunnsFluidEqConductorConfigData(const std::string& name,
                                                                 GunnsNodeList*     nodes,
                                                                 const bool         useNetworkCapacitance)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mUseNetworkCapacitance  (useNetworkCapacitance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Gunns Fluid Equivalent-Circuit Conductor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEqConductorConfigData::GunnsFluidEqConductorConfigData(
        const GunnsFluidEqConductorConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mUseNetworkCapacitance  (that.mUseNetworkCapacitance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Equivalent-Circuit Conductor Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEqConductorConfigData::~GunnsFluidEqConductorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag.
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1).
///
/// @details  Default GUNNS Fluid Equivalent-Circuit Conductor Input Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEqConductorInputData::GunnsFluidEqConductorInputData(const bool   malfBlockageFlag,
                                                               const double malfBlockageValue)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Fluid Equivalent-Circuit Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEqConductorInputData::GunnsFluidEqConductorInputData(const GunnsFluidEqConductorInputData& that)
    :
    GunnsFluidLinkInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Equivalent-Circuit Conductor Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEqConductorInputData::~GunnsFluidEqConductorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Gunns Fluid Equivalent-Circuit Conductor Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEqConductor::GunnsFluidEqConductor()
    :
    GunnsFluidLink(NPORTS),
    mUseNetworkCapacitance(false),
    mPort0DemandFlux(0.0),
    mPort0DemandTemperature(0.0),
    mPort1DemandFlux(0.0),
    mPort1DemandTemperature(0.0),
    mPort0SupplyCapacitance(0.0),
    mPort0SupplyPressure(0.0),
    mPort0SupplyTemperature(0.0),
    mPort0SupplyMassFractions(0),
    mPort1SupplyCapacitance(0.0),
    mPort1SupplyPressure(0.0),
    mPort1SupplyTemperature(0.0),
    mPort1SupplyMassFractions(0),
    mEquivalentConductance(0.0),
    mPort0LeakConductance(0.0),
    mPort1LeakConductance(0.0),
    mLastPotentialDrop(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Fluid Equivalent-Circuit Conductor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEqConductor::~GunnsFluidEqConductor()
{
    TS_DELETE_ARRAY(mPort1SupplyMassFractions);
    TS_DELETE_ARRAY(mPort0SupplyMassFractions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Fluid Equivalent-Circuit Conductor Config Data.
/// @param[in]     inputData    (--) GUNNS Fluid Equivalent-Circuit Conductor Input Data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Network port 0.
/// @param[in]     port1        (--) Network port 1.
///
/// @throws   TsInitializationException
///
/// @details  This method initializes this Fluid Equivalent-Circuit Conductor with configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEqConductor::initialize(const GunnsFluidEqConductorConfigData& configData,
                                       const GunnsFluidEqConductorInputData&  inputData,
                                       std::vector<GunnsBasicLink*>&          networkLinks,
                                       const int                              port0,
                                       const int                              port1)
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.
    mUseNetworkCapacitance    = configData.mUseNetworkCapacitance;
    mPort0DemandFlux          = 0.0;
    mPort1DemandFlux          = 0.0;
    mPort0DemandTemperature   = 0.0;
    mPort1DemandTemperature   = 0.0;
    mPort0SupplyCapacitance   = 0.0;
    mPort0SupplyPressure      = 0.0;
    mPort0SupplyTemperature   = 0.0;
    mPort1SupplyCapacitance   = 0.0;
    mPort1SupplyPressure      = 0.0;
    mPort1SupplyTemperature   = 0.0;
    mEquivalentConductance    = 0.0;
    mPort0LeakConductance     = 0.0;
    mPort1LeakConductance     = 0.0;
    mLastPotentialDrop        = 0.0;

    /// - Allocate memory for the outgoing data mass fraction arrays.
    TS_DELETE_ARRAY(mPort0SupplyMassFractions);
    TS_DELETE_ARRAY(mPort1SupplyMassFractions);
    TS_NEW_PRIM_ARRAY_EXT(mPort0SupplyMassFractions, mNodes[0]->getFluidConfig()->mNTypes, double, std::string(mName) + ".mPort0SupplyMassFractions");
    TS_NEW_PRIM_ARRAY_EXT(mPort1SupplyMassFractions, mNodes[0]->getFluidConfig()->mNTypes, double, std::string(mName) + ".mPort1SupplyMassFractions");
    for (int i = 0; i < mNodes[0]->getFluidConfig()->mNTypes; ++i) {
        mPort0SupplyMassFractions[i] = 0.0;
        mPort1SupplyMassFractions[i] = 0.0;
    }

    /// - Create the internal fluid.
    createInternalFluid();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEqConductor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mEquivalentConductance    = 0.0;
    mPort0LeakConductance     = 0.0;
    mPort1LeakConductance     = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data written to the external network's demand links, via the simbus.  Data is
///          moved from the supply node's content into the storage terms for output to simbus.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEqConductor::processOutputs()
{
    mPort0SupplyCapacitance = mNodes[0]->getNetworkCapacitance();
    mPort1SupplyCapacitance = mNodes[1]->getNetworkCapacitance();
    mPort0SupplyPressure    = mNodes[0]->getPotential();
    mPort1SupplyPressure    = mNodes[1]->getPotential();
    mPort0SupplyTemperature = mNodes[0]->getContent()->getTemperature();
    mPort1SupplyTemperature = mNodes[1]->getContent()->getTemperature();
    for (int i = 0; i < mInternalFluid->getNConstituents(); ++i) {
        mPort0SupplyMassFractions[i] = mNodes[0]->getContent()->getMassFraction(i);
        mPort1SupplyMassFractions[i] = mNodes[1]->getContent()->getMassFraction(i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Calls step on the Fluid Equivalent-Circuit Conductor to update the state and compute a
///           new conductivity.
///
/// @note     The blockage malfunction is deliberately not implemented, so that we are forced to
///           send the same flux as the external network demands in order to conserve mass.  To
///           block the flux across the interface, use malfunctions on the demand link side.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEqConductor::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    const double absSupplyFlux = fabs(mPort0DemandFlux);
    const double absReturnFlux = fabs(mPort1DemandFlux);

    double avgFlux        = 0.0;
    double supplyLeakFlux = 0.0;
    double returnLeakFlux = 0.0;

    /// - We assume that all difference in mass in & mass out is due to leaks in the external
    ///   network.  Therefore anything else that creates a difference in mass in/mass out should not
    ///   be used in the external network or this will break.  Throw an exception if mass source is
    ///   detected in the external network.
    if (mPort0DemandFlux < -DBL_EPSILON and mPort1DemandFlux < -DBL_EPSILON) {
    //TODO commented out until I can fix this being triggered by run transients after initialization
    //     and flow noise in no-flow condition.  For now, no flow will occur.
//        GUNNS_ERROR(TsOutOfBoundsException, "Invalid Model Inputs",
//                    "external network is introducing mass.");

    /// - If both fluxes are positive, then we assume the external loop is completely blocked but
    ///   there are leaks on both sides of the blockage.
    } else if (mPort0DemandFlux > DBL_EPSILON and mPort1DemandFlux > DBL_EPSILON) {
        supplyLeakFlux = mPort0DemandFlux;
        returnLeakFlux = mPort1DemandFlux;

    /// - If one flux is zero, then we assume the external loop is completely blocked and there is a
    ///   leak on the non-zero flux side (the flux is all leak).
    } else if (absSupplyFlux < DBL_EPSILON) {
        returnLeakFlux = mPort1DemandFlux;

    } else if (absReturnFlux < DBL_EPSILON) {
        supplyLeakFlux = mPort0DemandFlux;

    /// - Normal case, find average flux thru the external loop, and apply the leak to our
    ///   downstream port.
    } else {
        avgFlux        = (absSupplyFlux + absReturnFlux) / 2.0;
        if (mPort1DemandFlux < -DBL_EPSILON) {
            returnLeakFlux = mPort0DemandFlux + mPort1DemandFlux;
        } else {
            supplyLeakFlux = mPort0DemandFlux + mPort1DemandFlux;
        }
    }

    /// - AvgFlux is flux solved by the external network in response to our pressures 2 frames ago,
    ///   so when we estimate the equivalent resistance of the external network, we use the stored
    ///   delta-pressure that corresponds with the fluxes.
    if (fabs(mLastPotentialDrop) > DBL_EPSILON) {
        mEquivalentConductance = MsMath::limitRange(0.0, avgFlux / fabs(mLastPotentialDrop),
                                                  mConductanceLimit);
    } else {
        mEquivalentConductance = 0.0;
    }

    if (mPotentialVector[0] > DBL_EPSILON) {
        mPort0LeakConductance  = supplyLeakFlux / mPotentialVector[0];
    } else {
        mPort0LeakConductance  = 0.0;
    }

    if (mPotentialVector[1] > DBL_EPSILON) {
        mPort1LeakConductance  = returnLeakFlux / mPotentialVector[1];
    } else {
        mPort1LeakConductance  = 0.0;
    }

    /// - Build contributions to the network system of equations.
    buildConductance();

    /// - Flag the nodes to have their network capacitance calculated by the solver for output to
    ///   external demands.
    if (mUseNetworkCapacitance) {
        mNodes[0]->setNetworkCapacitanceRequest(mNetworkCapacitanceFlux);
        mNodes[1]->setNetworkCapacitanceRequest(mNetworkCapacitanceFlux);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the Admittance Matrix for the link.  System conductance creates flux between
///           ports 0 & 1.  Leak conductances create flux from ports 0 & 1 to ground.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEqConductor::buildConductance()
{
    const double A0  =  mEquivalentConductance + mPort0LeakConductance;
    const double A12 = -mEquivalentConductance;
    const double A3  =  mEquivalentConductance + mPort1LeakConductance;

    if (fabs(mAdmittanceMatrix[0] - A0)  > 0.0 or
        fabs(mAdmittanceMatrix[1] - A12) > 0.0 or
        fabs(mAdmittanceMatrix[2] - A12) > 0.0 or
        fabs(mAdmittanceMatrix[3] - A3)  > 0.0) {

        mAdmittanceUpdate    = true;
        mAdmittanceMatrix[0] = A0;
        mAdmittanceMatrix[1] = A12;
        mAdmittanceMatrix[2] = A12;
        mAdmittanceMatrix[3] = A3;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Computes the flow rate across this link and the updated fluid state
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEqConductor::computeFlows(const double dt __attribute__((unused)))
{
    mLastPotentialDrop = mPotentialDrop;
    mPotentialDrop = getDeltaPotential();

    /// - Compute molar flow rate.
    mFlux = mPotentialDrop * mEquivalentConductance;

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
/// @param[in] dt (s) Integration time step.
///
/// @details  Updates final flow properties and transports fluid from source to sink nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEqConductor::transportFlows(const double dt __attribute__((unused)))
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

    /// - Calculate hydraulic power rise from supply to return ports.
    mPower = -UnitConversion::PA_PER_KPA * mVolFlowRate * getDeltaPotential();

    /// - Update the internal fluid temperature with the external network temperature at the port
    ///   from which flow is returning.
    if (mFlowRate > DBL_EPSILON) {
        mInternalFluid->setTemperature(mPort1DemandTemperature);
    } else if (mFlowRate < -DBL_EPSILON) {
        mInternalFluid->setTemperature(mPort0DemandTemperature);
    }
    /// - Call the base class to transport the fluid between the nodes.  The false bool argument
    ///   indicates that this class cannot force a mixture change in the supplying node.
    transportFluid(false);

    /// - Transport external leak flow rates out of the nodes.
    const double port0LeakFlow = mPort0LeakConductance * mPotentialVector[0]
                               * mNodes[0]->getOutflow()->getMWeight();
    const double port1LeakFlow = mPort1LeakConductance * mPotentialVector[1]
                               * mNodes[1]->getOutflow()->getMWeight();
    mNodes[0]->collectOutflux(port0LeakFlow);
    mNodes[1]->collectOutflux(port1LeakFlow);
}
