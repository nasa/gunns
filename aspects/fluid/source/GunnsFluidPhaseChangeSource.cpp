/****************************** TRICK HEADER *******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))
***************************************************************************************************/

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "GunnsFluidPhaseChangeSource.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                  (--)    Name of object.
/// @param[in,out]  nodes                 (--)    Pointer to nodes.
/// @param[in]      liquidType            (--)    Fluid type of constituent liquid being changed.
/// @param[in]      gasType               (--)    Fluid type of constituent gas being changed.
/// @param[in]      efficiency            (--)    Power Efficiency of the modeled device.
///
/// @details  Default constructs this GUNNS Fluid Phase Change Source link model configuration data
///           with arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeSourceConfigData::GunnsFluidPhaseChangeSourceConfigData(
                                                 const std::string&               name,
                                                 GunnsNodeList*                   nodes,
                                                 const FluidProperties::FluidType liquidType,
                                                 const FluidProperties::FluidType gasType,
                                                 const double                     efficiency)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mLiquidType(liquidType),
    mGasType(gasType),
    mEfficiency(efficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Phase Change Source link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeSourceConfigData::GunnsFluidPhaseChangeSourceConfigData(const GunnsFluidPhaseChangeSourceConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mLiquidType(that.mLiquidType),
    mGasType(that.mGasType),
    mEfficiency(that.mEfficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Phase Change Source link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeSourceConfigData::~GunnsFluidPhaseChangeSourceConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag   (--)  Blockage malfunction flag.
/// @param[in]    malfBlockageValue  (--)  Blockage malfunction fractional value (0-1).
/// @param[in]    powerInput         (W)   Initial power input to the link.
///
/// @details  Default constructs this GUNNS Fluid Phase Change Source link model input data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeSourceInputData::GunnsFluidPhaseChangeSourceInputData(
                                                         const bool   malfBlockageFlag,
                                                         const double malfBlockageValue,
                                                         const double powerInput)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mPowerInput(powerInput)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Phase Change Source link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeSourceInputData::GunnsFluidPhaseChangeSourceInputData(const GunnsFluidPhaseChangeSourceInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mPowerInput(that.mPowerInput)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Phase Change Source link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeSourceInputData::~GunnsFluidPhaseChangeSourceInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Phase Change Source link model with obviously
///           invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeSource::GunnsFluidPhaseChangeSource()
    :
    GunnsFluidLink(NPORTS),
    mLiquidType(FluidProperties::NO_FLUID),
    mGasType(FluidProperties::NO_FLUID),
    mEfficiency(0.0),
    mLiquidIndex(0),
    mGasIndex(0),
    mPowerInput(0.0),
    mLiquidFluid(0),
    mGasFluid(0),
    mTsat(0.0),
    mDh(0.0),
    mL(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Phase Change Source link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeSource::~GunnsFluidPhaseChangeSource()
{
    /// - Delete the internal fluids.
    TS_DELETE_OBJECT(mGasFluid);
    TS_DELETE_OBJECT(mLiquidFluid);
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
/// @details     Initializes this GUNNS Fluid Phase Change Source link model with configuration and
///              input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeSource::initialize(const GunnsFluidPhaseChangeSourceConfigData& configData,
                                             const GunnsFluidPhaseChangeSourceInputData&  inputData,
                                             std::vector<GunnsBasicLink*>&                links,
                                             const int                                    port0,
                                             const int                                    port1)
{
    /// - First initialize & validate parent.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Reset the initialization complete flag.
    mInitFlag           = false;

    /// - Validate the configuration data.
    validate(configData);

    /// - Initialize attributes from the validated configuration data.
    mLiquidType         = configData.mLiquidType;
    mGasType            = configData.mGasType;
    mEfficiency         = configData.mEfficiency;

    /// - Initialize the configuration-derived attributes.
    mLiquidIndex        = mNodes[0]->getContent()->find(configData.mLiquidType);
    mGasIndex           = mNodes[0]->getContent()->find(configData.mGasType);

    /// - Initialize attributes from the validated input data.
    mPowerInput    = inputData.mPowerInput;

    /// - Create the internal fluids.  We ignore the mInternalFluid base class pointer and use our
    ///   own fluid objects for clarity.
    if (mLiquidFluid) {
        mLiquidFluid->cleanup();
        TS_DELETE_OBJECT(mLiquidFluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mLiquidFluid, PolyFluid,
                           (*(mNodes[0]->getContent()), mName + ".mLiquidFluid"),
                           mName + ".mLiquidFluid");
    mLiquidFluid->resetState();
    mLiquidFluid->setMass(mLiquidIndex, 1.0);
    mLiquidFluid->updateMass();

    if (mGasFluid) {
        mGasFluid->cleanup();
        TS_DELETE_OBJECT(mGasFluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mGasFluid, PolyFluid,
                           (*(mNodes[0]->getContent()), mName + ".mGasFluid"),
                           mName + ".mGasFluid");
    mGasFluid->resetState();
    mGasFluid->setMass(mGasIndex, 1.0);
    mGasFluid->updateMass();

    /// - Initialize the remaining state attributes.
    mTsat = 0.0;
    mDh   = 0.0;
    mL    = 0.0;

    /// - Set the initialization complete flag.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
///
/// @return      void
///
/// @throws      TsInitializationException
///
/// @details     Validates the initialization of this GUNNS Fluid Phase Change Source link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeSource::validate(const GunnsFluidPhaseChangeSourceConfigData& configData) const
{
    /// - Throw an exception on liquid type not in network.
    try {
        mNodes[0]->getContent()->find(configData.mLiquidType);
    } catch (const TsOutOfBoundsException&) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "liquid type is not in network.");
    }

    /// - Throw an exception on gas type not in network.
    try {
        mNodes[0]->getContent()->find(configData.mGasType);
    } catch (const TsOutOfBoundsException&) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "gas type is not in network.");
    }

    /// - Throw an exception if gas & liquid are not the same compound, i.e. don't have identical
    ///   molecular weights.
    const FluidProperties* gasProperties = mNodes[1]->getContent()->getProperties(configData.mGasType);
    const FluidProperties* liqProperties = mNodes[0]->getContent()->getProperties(configData.mLiquidType);
    if (gasProperties->getMWeight() != liqProperties->getMWeight()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "gas and liquid types have different molecular weights.");
    }

    /// - Throw an exception if the given gas & liquid types are not of the correct phase.
    if (FluidProperties::GAS != gasProperties->getPhase()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "gas fluid type is not gas phase.");
    }
    if (FluidProperties::LIQUID != liqProperties->getPhase()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "liquid fluid type is not liquid phase.");
    }

    /// - Throw an exception on efficiency outside valid range (-1.0 to +1.0)).
    if (!MsMath::isInRange(-1.0, configData.mEfficiency, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "efficiency outside valid range (-1.0 to +1.0).");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @return   bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - Neither port may be mapped to the network's vacuum/ground node.
///           - A GunnsFluidPhaseChangeSource must map port 0 to a liquid node.
///           - A GunnsFluidPhaseChangeSource must map port 1 to a gas node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidPhaseChangeSource::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);

    /// - Fail if either port is the vacuum boundary node.
    if (node == getGroundNodeIndex()) {
        GUNNS_WARNING("aborted setting a port: cannot assign either port to the boundary node.");
        result = false;
    }

    /// - Fail if port 0 is not in liquid phase.
    else if ((0 == port) && (FluidProperties::LIQUID != nodePtr[node].getContent()->getPhase())) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to a liquid node.");
        result = false;
    }

    /// - Fail if port 1 is not in gas phase.
    else if ((1 == port) && (FluidProperties::GAS != nodePtr[node].getContent()->getPhase())) {
        GUNNS_WARNING("aborted setting a port: must assign port 1 to a gas node.");
        result = false;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeSource::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    // (currently there are none)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step
///
/// @return   void
///
/// @details  Computes the mass flow of phase change based on input power, and contributions to the
///           system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeSource::step(const double dt __attribute__((unused)))
{
    /// - Process commands to change node map.
    processUserPortCommand();

    /// - Calculate actual heat power from efficiency.
    mPower = mPowerInput * mEfficiency;

    /// - Blockage malf degrades the phase change rate same as a reduction in efficiency.
    if (mMalfBlockageFlag) {
        mPower *= (1.0 - mMalfBlockageValue);
    }

    const FluidProperties* gasProperties = mNodes[1]->getContent()->getProperties(mGasType);
    const FluidProperties* liqProperties = mNodes[0]->getContent()->getProperties(mLiquidType);

    /// - For positive heat flow, calculate heat to move liquid inlet to saturation T + heat of
    ///   vaporization at the gas exit P.
    if (mPower > 0.0) {
        /// - Assuming the exit gas is saturated, find the saturation temperature at its current
        ///   pressure.
        mTsat = gasProperties->getSaturationTemperature(mPotentialVector[1]);
        /// - Change in enthalpy to heat the liquid to the saturation point.  Here we assume that
        ///   the liquid is 100% the phase change liquid type, since we don't use liquid mixtures in
        ///   GUNNS.
        const double tLiq = mNodes[0]->getContent()->getTemperature();
        mDh = liqProperties->getSpecificEnthalpy(mTsat)
            - liqProperties->getSpecificEnthalpy(tLiq);
        /// - Latent heat of vaporization of the phase change fluid type at the saturation T,
        ///   converted from kJ/kg to J/kg.
        mL = liqProperties->getHeatOfVaporization(mTsat) * UnitConversion::UNIT_PER_KILO;
        /// - Calculate resulting mass phase change rate.  If mDh is negative then the liquid is
        ///   already above the saturation temperature and is boiling, which reduces the input power
        ///   needed to produce mass flow.  However, we limit mDh + mL > 0 to avoid dividing by zero
        ///   or mass flow direction opposite the power sign.
        mFlowRate = mPower / fmax(mDh + mL, FLT_EPSILON);

    } else {
        /// - Assuming the exit liquid is saturated, find the saturation temperature at its current
        ///   pressure.
        mTsat = liqProperties->getSaturationTemperature(mPotentialVector[0]);
        /// - Change in enthalpy to cool the gas side mixture to the gas type's saturation point.
        const double tGas = mNodes[1]->getContent()->getTemperature();
        mDh = mNodes[1]->getContent()->computeSpecificEnthalpy(tGas,  mPotentialVector[1])
            - mNodes[1]->getContent()->computeSpecificEnthalpy(mTsat, mPotentialVector[1]);
        /// - Latent heat of vaporization of the phase change fluid type at the saturation T,
        ///   converted from kJ/kg to J/kg.
        mL = gasProperties->getHeatOfVaporization(mTsat) * UnitConversion::UNIT_PER_KILO;
        /// - Calculate resulting mass phase change rate.
        mFlowRate = mPower / fmax(mDh + mL, FLT_EPSILON);
    }

    /// - Update the source vector.  The gas & liquid molecular weights are the same, as enforced
    ///   during validate.
    mFlux = mFlowRate / gasProperties->getMWeight();
    if (fabs(mFlowRate) > m100EpsilonLimit) {
        mSourceVector[0] = -mFlux;
        mSourceVector[1] =  mFlux;
    } else {
        mSourceVector[0] = 0.0;
        mSourceVector[1] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step
///
/// @details  Computes the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeSource::computeFlows(const double dt __attribute__((unused)))
{
    /// - Potential drop doesn't really mean much for this link but we update it anyway.
    mPotentialDrop = getDeltaPotential();

    /// - Set port flow directions and schedule flow from source nodes.
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;
    if (mFlux > DBL_EPSILON) {
        mPortDirections[0] = SOURCE;
        mPortDirections[1] = SINK;
        mNodes[0]->scheduleOutflux(mFlux);
    } else if (mFlux < -DBL_EPSILON) {
        mPortDirections[0] = SINK;
        if (1.0 == mNodes[1]->getContent()->getMassFraction(mGasType)) {
            mPortDirections[1] = SOURCE;
            mNodes[1]->scheduleOutflux(-mFlux);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step
///
/// @details  transports the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeSource::transportFlows(const double dt __attribute__((unused)))
{

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   source node.
    double sourceDensity = mNodes[determineSourcePort(mFlowRate, 0, 1)]->getOutflow()->getDensity();

    if (sourceDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / sourceDensity;
    } else {
        mVolFlowRate = 0.0;
    }

    transportFluid(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] forcedOutflow (--) not used.
/// @param[in] fromPort      (--) not used.
/// @param[in] toPort        (--) not used.
///
/// @details  This function adds the phase-change flow to the node's collection terms.
///
///           Flow is defined positive going from the Port 0 to Port 1, so a positive flow
///           will add mass to port 1 and remove from port 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeSource::transportFluid(const bool forcedOutflow __attribute__((unused)),
                                                 const int  fromPort __attribute__((unused)),
                                                 const int  toPort __attribute__((unused)))
{
    /// - Transport phase change fluids to/from the nodes.  Only the specified gas & liquid types
    ///   must be transported, so the internal mLiquidFluid & mGasFluid only have their one fluid
    ///   type at 100% mass fraction.
    ///
    ///   When flowing into a node, set the internal fluid temperature to the saturation temperature
    ///   at the node's new pressure.  If flowing out of a node, set the internal fluid equal to the
    ///   node's temperature.
    if (mFlowRate > m100EpsilonLimit) {
        mNodes[0]->collectOutflux(mFlowRate);
    } else if (mFlowRate < -m100EpsilonLimit) {
        const FluidProperties* liqProperties = mNodes[0]->getContent()->getProperties(mLiquidType);
        mLiquidFluid->setTemperature(liqProperties->getSaturationTemperature(mPotentialVector[0]));
        mNodes[0]->collectInflux(-mFlowRate, mLiquidFluid);
    }

    if (mFlowRate > m100EpsilonLimit) {
        const FluidProperties* gasProperties = mNodes[1]->getContent()->getProperties(mGasType);
        mGasFluid->setTemperature(gasProperties->getSaturationTemperature(mPotentialVector[1]));
        mNodes[1]->collectInflux(mFlowRate, mGasFluid);
    } else if (mFlowRate < -m100EpsilonLimit) {
        /// If the gas source node contents are not 100% the gas fluid type, then we have to pull
        /// only that type out of the source node by using the node's collectInFlux method with a
        /// negative rate.
        if (mNodes[1]->getContent()->getMassFraction(mGasType) < 1.0) {
            mGasFluid->setTemperature(mNodes[1]->getContent()->getTemperature());
            mNodes[1]->collectInflux(mFlowRate, mGasFluid);
        } else {
            mNodes[1]->collectOutflux(-mFlowRate);
        }
    }
}
