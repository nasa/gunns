/**
@file
@brief    GUNNS Selective Membrane Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidLink.o)
    (core/GunnsFluidUtils.o))
*/

#include "GunnsFluidSelectiveMembrane.hh"
#include "core/GunnsFluidUtils.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name            (--)       Name of this instance for messages.
/// @param[in,out] nodes           (--)       Pointer to the network nodes.
/// @param[in]     maxConductance  (m2)       Max conductance of the flow-thru path.
/// @param[in]     absorptionCoeff (kg/s/kPa) Absorption coefficient of the membrane.
/// @param[in]     externalType    (--)       Membrane absorbed fluid type in the external vent.
/// @param[in]     internalType    (--)       Membrane absorbed fluid type in the internal bulk flow.
///
/// @details  Default constructs this GUNNS Fluid Selective Membrane link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSelectiveMembraneConfigData::GunnsFluidSelectiveMembraneConfigData(
        const std::string&               name,
        GunnsNodeList*                   nodes,
        const double                     maxConductance,
        const double                     absorptionCoeff,
        const FluidProperties::FluidType externalType,
        const FluidProperties::FluidType internalType)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mMaxConductance (maxConductance),
    mAbsorptionCoeff(absorptionCoeff),
    mExternalType   (externalType),
    mInternalType   (internalType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     that (--) Source object to copy.
///
/// @details  Copy constructs this GUNNS Fluid Selective Membrane link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSelectiveMembraneConfigData::GunnsFluidSelectiveMembraneConfigData(
        const GunnsFluidSelectiveMembraneConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mMaxConductance (that.mMaxConductance),
    mAbsorptionCoeff(that.mAbsorptionCoeff),
    mExternalType   (that.mExternalType),
    mInternalType   (that.mInternalType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Selective Membrane link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSelectiveMembraneConfigData::~GunnsFluidSelectiveMembraneConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Fluid Selective Membrane link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSelectiveMembraneInputData::GunnsFluidSelectiveMembraneInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)   Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Selective Membrane link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSelectiveMembraneInputData::GunnsFluidSelectiveMembraneInputData(
        const GunnsFluidSelectiveMembraneInputData& that)
    :
    GunnsFluidLinkInputData(that)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Selective Membrane link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSelectiveMembraneInputData::~GunnsFluidSelectiveMembraneInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Selective Membrane link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSelectiveMembrane::GunnsFluidSelectiveMembrane()
    :
    GunnsFluidLink           (NPORTS),
    mMalfMembraneDegradeFlag (false),
    mMalfMembraneDegradeValue(0.0),
    mMaxConductance          (0.0),
    mAbsorptionCoeff         (0.0),
    mExternalType            (FluidProperties::NO_FLUID),
    mInternalType            (FluidProperties::NO_FLUID),
    mEffectiveConductance    (0.0),
    mSystemConductance       (0.0),
    mExternalPartialP        (0.0),
    mInternalPartialP        (),
    mExternalSaturationP     (0.0),
    mInternalSaturationP     (0.0),
    mExternalSaturated       (false),
    mInternalSaturated       (false),
    mPhaseChange             (false),
    mMembraneDeltaP          (),
    mMembraneFlowRate        (0.0),
    mPhaseChangeHeat         (0.0),
    mExternalMembraneFluid   (),
    mInternalMembraneFluid   ()
{
    mInternalPartialP[0] = 0.0;
    mInternalPartialP[1] = 0.0;
    mMembraneDeltaP[0]   = 0.0;
    mMembraneDeltaP[1]   = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Selective Membrane link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSelectiveMembrane::~GunnsFluidSelectiveMembrane()
{
    delete mInternalMembraneFluid;
    delete mExternalMembraneFluid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  Flow-thru path inlet port map index.
/// @param[in]      port1        (--)  Flow-thru path outlet port map index.
/// @param[in]      port2        (--)  Absorbed vent gas port map index.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Selective Membrane link model with configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSelectiveMembrane::initialize(const GunnsFluidSelectiveMembraneConfigData& configData,
                                             const GunnsFluidSelectiveMembraneInputData&  inputData,
                                             std::vector<GunnsBasicLink*>&                links,
                                             const int                                    port0,
                                             const int                                    port1,
                                             const int                                    port2)
{
    /// - Validate configuration data.
    validate(configData);

    /// - Initialize from config data.
    mMaxConductance  = configData.mMaxConductance;
    mAbsorptionCoeff = configData.mAbsorptionCoeff;
    mExternalType    = configData.mExternalType;
    mInternalType    = configData.mInternalType;

    /// - Initialize & validate parent.
    int ports[3] = {port0, port1, port2};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Initialize remaining state data.
    mEffectiveConductance  = 0.0;
    mSystemConductance     = 0.0;
    mInternalPartialP[0]   = 0.0;
    mInternalPartialP[1]   = 0.0;
    mExternalPartialP      = 0.0;
    mInternalSaturationP   = 0.0;
    mExternalSaturationP   = 0.0;
    mExternalSaturated     = false;
    mInternalSaturated     = false;
    mPhaseChange           = false;
    mMembraneDeltaP[0]     = 0.0;
    mMembraneDeltaP[1]     = 0.0;
    mMembraneFlowRate      = 0.0;
    mPhaseChangeHeat       = 0.0;

    /// - Initialize a PolyFluid for 100% external membrane absorbed fluid.
    TS_DELETE_OBJECT(mExternalMembraneFluid);
    TS_NEW_PRIM_OBJECT_EXT(mExternalMembraneFluid, PolyFluid,
                           (*(mNodes[0]->getContent()), std::string(mName) + ".mExternalMembraneFluid", false),
                           std::string(mName) + ".mExternalMembraneFluid");

    double* massFractions = new double[mNodes[0]->getContent()->getNConstituents()];
    for (int i=0; i<mNodes[0]->getContent()->getNConstituents(); ++i) {
        massFractions[i] = 0.0;
    }
    massFractions[mExternalMembraneFluid->find(mExternalType)] = 1.0;

    mExternalMembraneFluid->resetState();
    mExternalMembraneFluid->setFlowRate(0.0);
    mExternalMembraneFluid->setMassAndMassFractions(0.0, massFractions);
    mExternalMembraneFluid->setTemperature(mNodes[2]->getContent()->getTemperature());
    delete [] massFractions;

    /// - Initialize a PolyFluid for 100% internal membrane absorbed fluid.
    TS_DELETE_OBJECT(mInternalMembraneFluid);
    TS_NEW_PRIM_OBJECT_EXT(mInternalMembraneFluid, PolyFluid,
                           (*(mNodes[0]->getContent()), std::string(mName) + ".mInternalMembraneFluid", false),
                           std::string(mName) + ".mInternalMembraneFluid");

    massFractions = new double[mNodes[0]->getContent()->getNConstituents()];
    for (int i=0; i<mNodes[0]->getContent()->getNConstituents(); ++i) {
        massFractions[i] = 0.0;
    }
    massFractions[mInternalMembraneFluid->find(mInternalType)] = 1.0;

    mInternalMembraneFluid->resetState();
    mInternalMembraneFluid->setFlowRate(0.0);
    mInternalMembraneFluid->setMassAndMassFractions(0.0, massFractions);
    mInternalMembraneFluid->setTemperature(mNodes[0]->getContent()->getTemperature());
    delete [] massFractions;

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Selective Membrane link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSelectiveMembrane::validate(const GunnsFluidSelectiveMembraneConfigData& configData) const
{
    /// - Throw an exception on max conductance < 0.
    if (configData.mMaxConductance < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Max conductance < 0.");
    }

    /// - Throw an exception on absorption coefficient < 0.
    if (configData.mAbsorptionCoeff < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Absorption coefficient < 0.");
    }

    /// - Throw an exception on external type not found in the network.
    GunnsFluidNode* nodePtr = static_cast<GunnsFluidNode*> (configData.mNodeList->mNodes);
    try {
        nodePtr[0].getContent()->find(configData.mExternalType);
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "External fluid type not found in network.");
    }

    /// - Throw an exception on internal type not found in the network.
    try {
        nodePtr[0].getContent()->find(configData.mInternalType);
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Internal fluid type not found in network.");
    }

    /// - Throw an exception on internal and external type molecular weights not the same.
    if (nodePtr[0].getContent()->getProperties(configData.mInternalType)->getMWeight() !=
        nodePtr[0].getContent()->getProperties(configData.mExternalType)->getMWeight()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                "External and internal fluid types have different molecular weight.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSelectiveMembrane::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mEffectiveConductance  = 0.0;
    mSystemConductance     = 0.0;
    mExternalPartialP      = 0.0;
    mInternalPartialP[0]   = 0.0;
    mInternalPartialP[1]   = 0.0;
    mExternalSaturationP   = 0.0;
    mInternalSaturationP   = 0.0;
    mMembraneDeltaP[0]     = 0.0;
    mMembraneDeltaP[1]     = 0.0;
    mMembraneFlowRate      = 0.0;
    mPhaseChangeHeat       = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Computes this link's contributions to the network system of equations, models a normal
///           conductive flow path between Ports 0 & 1 similar to a GunnsFluidConductor, and models
///           selective fluid absorption through the membrane proportional to the fluid partial
///           pressures across the membrane.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSelectiveMembrane::step(const double dt __attribute__((unused)))
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
    buildAdmittance();

    /// - Compute partial pressure of the absorbed fluid in the internal and external streams.
    const int upstreamPort   = determineSourcePort(mFlux, 0, 1);
    const int downstreamPort = 1 - upstreamPort;
    mInternalPartialP[upstreamPort]   = computePortPartialP(upstreamPort,   mInternalType);
    mInternalPartialP[downstreamPort] = computePortPartialP(downstreamPort, mInternalType);
    mExternalPartialP                 = computePortPartialP(2,              mExternalType);

    /// - Compute saturation state of the absorbed fluid in the external stream.
    const FluidProperties* intProps = mNodes[0]->getContent()->getProperties(mInternalType);
    const FluidProperties* extProps = mNodes[0]->getContent()->getProperties(mExternalType);
    mExternalSaturationP = extProps->getSaturationPressure(mNodes[2]->getContent()->getTemperature());
    mExternalSaturated   = (mExternalPartialP > mExternalSaturationP);

    /// - Compute delta of the absorbed fluid partial-pressure across the membrane.
    mMembraneDeltaP[upstreamPort]   = mInternalPartialP[upstreamPort]   - mExternalPartialP;
    mMembraneDeltaP[downstreamPort] = mInternalPartialP[downstreamPort] - mExternalPartialP;

    /// - Set membrane flow to zero if dP of upstream and downstream ports are different signs.
    if (mMembraneDeltaP[downstreamPort] * mMembraneDeltaP[upstreamPort] < 0.0) {
        mMembraneFlowRate = 0.0;
        mPhaseChange      = false;
    } else {
        /// - Compute absorption mass flow rate across the membrane.  Limited to maximum of 99% of
        ///   available mass in the flow-thru internal stream to avoid pulling negative mass
        ///   fraction in the downstream node.  Not limited when flowing into the flow-thru stream
        ///   from the vent.
        ///
        /// - Compute saturation state of the absorbed fluid in the internal stream, and determine
        ///   whether phase change is occurring.
        if (mMembraneDeltaP[upstreamPort] > 0.0) {
            const double availableSource = 0.99 * fabs(mFlowRate)
                                         * mNodes[upstreamPort]->getContent()->getMassFraction(mInternalType);
            mMembraneFlowRate    = std::min(mMembraneDeltaP[upstreamPort] * mAbsorptionCoeff, availableSource);
            mInternalSaturationP = intProps->getSaturationPressure(mNodes[upstreamPort]->getContent()->getTemperature());
            mInternalSaturated   = (mInternalPartialP[upstreamPort] > mInternalSaturationP);
        } else {
            const double availableSource = 1.0e+6;
            mMembraneFlowRate    = std::min(mMembraneDeltaP[downstreamPort] * mAbsorptionCoeff, availableSource);
            mInternalSaturationP = intProps->getSaturationPressure(mNodes[downstreamPort]->getContent()->getTemperature());
            mInternalSaturated   = (mInternalPartialP[downstreamPort] > mInternalSaturationP);
        }
        mPhaseChange = (mInternalSaturated != mExternalSaturated);
        if (mMalfMembraneDegradeFlag) {
            mMembraneFlowRate *= (1.0 - MsMath::limitRange(0.0, mMalfMembraneDegradeValue, 1.0));
        }
    }

    /// - Build the link source vector.  Positive absorption flow across the membrane is added to
    ///   the vent port and removed from the upstream port.
    const double membraneFlux = mMembraneFlowRate / extProps->getMWeight();
    if (membraneFlux > 0.0) {
        mSourceVector[upstreamPort]   = -membraneFlux;
        mSourceVector[downstreamPort] =  0.0;
        mSourceVector[2]              =  membraneFlux;
    } else {
        mSourceVector[upstreamPort]   =  0.0;
        mSourceVector[downstreamPort] = -membraneFlux;
        mSourceVector[2]              =  membraneFlux;
    }

    /// - When phase change is occurring, compute the associated heat of the phase change at the
    ///   temperature of the supplying node.  The sign of the heat is positive when the source is
    ///   unsaturated, and negative when it is saturated.  The (kJ) units in the fluid's heat of
    ///   vaporization must be converted to (J).
    if (mPhaseChange) {
        if (mMembraneFlowRate < 0.0) {
            mPhaseChangeHeat = intProps->getHeatOfVaporization(mNodes[2]->getContent()->getTemperature())
                             * mMembraneFlowRate * UnitConversion::UNIT_PER_KILO;
            if (mExternalSaturated) {
                mPhaseChangeHeat *= -1.0;
            }
        } else {
            mPhaseChangeHeat = intProps->getHeatOfVaporization(mNodes[upstreamPort]->getContent()->getTemperature())
                             * mMembraneFlowRate * UnitConversion::UNIT_PER_KILO;
            if (mInternalSaturated) {
                mPhaseChangeHeat *= -1.0;
            }
        }
    } else {
        mPhaseChangeHeat = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Applies a linearization of the fluid momentum equation to create the non-linear
///           relationship between flow and pressure over multiple network steps.
///
/// @returns  double  (kg*mol/kPa/s)  Linearized molar conductance.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidSelectiveMembrane::linearizeConductance()
{
    return GunnsFluidUtils::computeAdmittance(mEffectiveConductance,
                                              mMinLinearizationPotential,
                                              mNodes[0]->getContent(),
                                              mNodes[1]->getContent());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  unused.
///
/// @details  Computes Delta Potential and Flux. Sets port Directions for ports zero, one, and two.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSelectiveMembrane::computeFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    /// - Update bulk molar flow rate, and molar flow rate across the membrane.
    computeFlux();

    const FluidProperties* extProps = mNodes[2]->getContent()->getProperties(mExternalType);
    const double membraneFlux       = mMembraneFlowRate / extProps->getMWeight();

    /// - Port and pure fluid determinations.
    const int  upstreamPort      = determineSourcePort(mFlux, 0, 1);
    const int  downstreamPort    = 1 - upstreamPort;
    const bool pureUpstreamFluid = (1.0 == mNodes[upstreamPort]->getContent()->getMassFraction(mInternalType));
    const bool pureVentFluid     = (1.0 == mNodes[2]->getContent()->getMassFraction(mExternalType));

    /// - Set port flow directions and schedule flow from source nodes.
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;
    mPortDirections[2] = NONE;

    if (fabs(mFlux) > DBL_EPSILON) {
        mPortDirections[upstreamPort]   = SOURCE;
        mPortDirections[downstreamPort] = SINK;
    }

    if (membraneFlux > m100EpsilonLimit) {
        mPortDirections[2] = SINK;
        if (pureUpstreamFluid) {
            mPortDirections[upstreamPort] = SOURCE;
            mNodes[upstreamPort]->scheduleOutflux(membraneFlux);
        } else {
            mPortDirections[upstreamPort] = SINK;
        }
    } else if (membraneFlux < -m100EpsilonLimit) {
        mPortDirections[downstreamPort] = SINK;
        if (pureVentFluid) {
            mPortDirections[2] = SOURCE;
            mNodes[2]->scheduleOutflux(-membraneFlux);
        } else {
            mPortDirections[2] = SINK;
        }
    }

    if (mFlux > DBL_EPSILON) {
        mNodes[upstreamPort]->scheduleOutflux(mFlux);
    } else if (mFlux < -DBL_EPSILON) {
        mNodes[upstreamPort]->scheduleOutflux(-mFlux);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  unused.
///
/// @details  transports flow-thru path and membrane flows between nodes and adds heat of phase
///           change when present.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSelectiveMembrane::transportFlows(const double dt __attribute__((unused)))
{
    /// - Convert the molar flow rate to mass rate using the molecular weight of the source node.
    const int    upstreamPort    = determineSourcePort(mFlux, 0, 1);
    const int    downstreamPort  = 1 - upstreamPort;
    const double upstreamDensity = mNodes[upstreamPort]->getOutflow()->getDensity();
    mFlowRate                    = mNodes[upstreamPort]->getOutflow()->getMWeight() * mFlux;

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   source node.
    if (upstreamDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / upstreamDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    /// - Calculate hydraulic power.
    computePower();

    /// - Call the base class to transport the flow-thru path fluid flow between the nodes.
    transportFluid(false);

    /// - Transport membrane absorption flow to/from the vent node.  The membrane flow is always at
    ///   the source node temperature.
    if (mMembraneFlowRate > m100EpsilonLimit) {
        mExternalMembraneFluid->setTemperature(mNodes[upstreamPort]->getOutflow()->getTemperature());
        mNodes[2]->collectInflux(mMembraneFlowRate, mExternalMembraneFluid);
    } else if (mMembraneFlowRate < -m100EpsilonLimit) {
        ///   When flowing out of a node, if the node contents are not 100% the membrane fluid type,
        ///   then we have to pull only that type out of the source node by using the node's
        ///   collectInflux method with a negative rate.  Otherwise we use the regular
        ///   collectOutflux method.
        if (mNodes[2]->getOutflow()->getMassFraction(mExternalType) < 1.0) {
            mExternalMembraneFluid->setTemperature(mNodes[2]->getOutflow()->getTemperature());
            mNodes[2]->collectInflux(mMembraneFlowRate, mExternalMembraneFluid);
        } else {
            mNodes[2]->collectOutflux(-mMembraneFlowRate);
        }
    }

    /// - Transport positive membrane absorption flow from the upstream node, or negative membrane
    ///   flow to the downstream node.
    if (mMembraneFlowRate > m100EpsilonLimit) {
        mInternalMembraneFluid->setTemperature(mNodes[upstreamPort]->getOutflow()->getTemperature());
        if (mNodes[upstreamPort]->getOutflow()->getMassFraction(mInternalType) < 1.0) {
            mNodes[upstreamPort]->collectInflux(-mMembraneFlowRate, mInternalMembraneFluid);
        } else {
            mNodes[upstreamPort]->collectOutflux(mMembraneFlowRate);
        }
    } else if (mMembraneFlowRate < -m100EpsilonLimit) {
        mInternalMembraneFluid->setTemperature(mNodes[2]->getOutflow()->getTemperature());
        mNodes[downstreamPort]->collectInflux(-mMembraneFlowRate, mInternalMembraneFluid);
    }

    /// - Collect the heat of phase change in the source side of the membrane flow.
    if (mPhaseChange) {
        if (mMembraneFlowRate > m100EpsilonLimit) {
            mNodes[downstreamPort]->collectHeatFlux(mPhaseChangeHeat);
        } else {
            mNodes[2]->collectHeatFlux(mPhaseChangeHeat);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the molar flux rate through the link.  Units:
///           Flux (kgmol/s) = Admittance (kgmol/kPa/s) * dP (kPa)
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSelectiveMembrane::computeFlux()
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
/// @param[in]    port  (--)  The port to be assigned.
/// @param[in]    node  (--)  The desired node to assign the port to.
///
/// @returns  bool (--) True if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           a Gunns Fluid Selective Membrane link model.
///           - Ports 0 and 1 must match the phase of the Internal fluid type or be the Ground node.
///           - Port 2 must match the phase of the External fluid type or be the Ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidSelectiveMembrane::checkSpecificPortRules(const int port, const int node) const
{
    bool            result  = true;
    GunnsFluidNode* nodePtr = static_cast<GunnsFluidNode*> (mNodeList->mNodes);

    /// - Fail if port 0 is not the Ground node and doesn't match the Internal fluid type phase.
    if ((0 == port) && (node != getGroundNodeIndex())
            && (nodePtr[node].getContent()->getPhase() !=
                nodePtr[node].getContent()->getProperties(mInternalType)->getPhase())) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to a node in same phase as the Internal fluid type.");
        result = false;
    }

    /// - Fail if port 1 is not the Ground node and doesn't match the Internal fluid type phase.
    else if ((1 == port) && (node != getGroundNodeIndex())
            && (nodePtr[node].getContent()->getPhase() !=
                nodePtr[node].getContent()->getProperties(mInternalType)->getPhase())) {
        GUNNS_WARNING("aborted setting a port: must assign port 1 to a node in same phase as the Internal fluid type.");
        result = false;
    }

    /// - Fail if port 2 is not the Ground node and doesn't match the External fluid type phase.
    else if ((2 == port) && (node != getGroundNodeIndex())
            && (nodePtr[node].getContent()->getPhase() !=
                nodePtr[node].getContent()->getProperties(mExternalType)->getPhase())) {
        GUNNS_WARNING("aborted setting a port: must assign port 2 to a node in same phase as the External fluid type.");
        result = false;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) (0-1) Malfunction value to degrade the membrane.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSelectiveMembrane::setMalfMembraneDegrade(const bool flag, const double value)
{
    mMalfMembraneDegradeFlag  = flag;
    mMalfMembraneDegradeValue = value;
}
