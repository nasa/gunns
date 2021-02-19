/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (See GunnsFluidSublimator.hh)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidLink.o))

PROGRAMMERS:
   ((Jason Harvey) (L3) (Initial) (2012-02))
**************************************************************************************************/

#include "software/exceptions/TsInitializationException.hh"

#include "GunnsFluidSublimator.hh"
#include "core/GunnsFluidUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                    (--)    Name of object
/// @param[in,out] nodes                   (--)    Pointer to nodes
/// @param[in]     gasType                 (--)    Gas phase of the operating fluid
/// @param[in]     heatOfVaporization      (kJ/kg) Heat of vaporization of the operating fluid
/// @param[in]     heatOfFusion            (kJ/kg) Heat of fusion (melting) of the operating fluid
/// @param[in]     triplePointTemperature  (K)     Temperature of the operating fluid triple point
/// @param[in]     maxIceMass              (kg)    Maximum mass of ice the sublimator can form
/// @param[in]     iceCoverageFraction     (--)    (0-1) fraction of maximum ice mass for complete plate coverage
/// @param[in]     maxThermalConductivity  (J/s/K) Maximum conductivity of the GunnsThermalPotential
/// @param[in]     plateConductivity       (m2)    Effective conductivity of plate with no ice coverage
/// @param[in]     thermalConductivityGain (--)    (0-1) Recommend 0.01, thermal conductivity filter gain
///
/// @details  Default constructs this GUNNS Fluid Sublimator link model configuration data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSublimatorConfigData::GunnsFluidSublimatorConfigData(
                                              const std::string&               name,
                                              GunnsNodeList*                   nodes,
                                              const FluidProperties::FluidType gasType,
                                              const double                     heatOfVaporization,
                                              const double                     heatOfFusion,
                                              const double                     triplePointTemperature,
                                              const double                     maxIceMass,
                                              const double                     iceCoverageFraction,
                                              const double                     maxThermalConductivity,
                                              const double                     plateConductivity,
                                              const double                     thermalConductivityGain)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mGasType                (gasType),
    mHeatOfVaporization     (heatOfVaporization),
    mHeatOfFusion           (heatOfFusion),
    mTriplePointTemperature (triplePointTemperature),
    mMaxIceMass             (maxIceMass),
    mIceCoverageFraction    (iceCoverageFraction),
    mMaxThermalConductivity (maxThermalConductivity),
    mPlateConductivity      (plateConductivity),
    mThermalConductivityGain(thermalConductivityGain)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Sublimator link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSublimatorConfigData::GunnsFluidSublimatorConfigData(
                                                       const GunnsFluidSublimatorConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mGasType                (that.mGasType),
    mHeatOfVaporization     (that.mHeatOfVaporization),
    mHeatOfFusion           (that.mHeatOfFusion),
    mTriplePointTemperature (that.mTriplePointTemperature),
    mMaxIceMass             (that.mMaxIceMass),
    mIceCoverageFraction    (that.mIceCoverageFraction),
    mMaxThermalConductivity (that.mMaxThermalConductivity),
    mPlateConductivity      (that.mPlateConductivity),
    mThermalConductivityGain(that.mThermalConductivityGain)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sublimator link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSublimatorConfigData::~GunnsFluidSublimatorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    iceMass              (kg)   Initial mass of ice in the sublimator
/// @param[in]    structureTemperature (K)    Initial temperature of the sublimator structure
///
/// @details  Default constructs this GUNNS Fluid Sublimator link model input data with
///           arguments and obviously invalid default values.
///
/// @note     The malfunction terms are not provided as arguments to the constructor, and they are
///           only present in the input data class to support unit testing.  The constructor
///           defaults all malfunctions to off.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSublimatorInputData::GunnsFluidSublimatorInputData(const double iceMass,
                                                             const double structureTemperature)
    :
    GunnsFluidLinkInputData(false, 0.0),
    mIceMass               (iceMass),
    mStructureTemperature  (structureTemperature),
    mMalfFeedRateBiasFlag  (false),
    mMalfFeedRateBiasValue (0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)   Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Sublimator link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSublimatorInputData::GunnsFluidSublimatorInputData(
                                                        const GunnsFluidSublimatorInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mIceMass               (that.mIceMass),
    mStructureTemperature  (that.mStructureTemperature),
    mMalfFeedRateBiasFlag  (that.mMalfFeedRateBiasFlag),
    mMalfFeedRateBiasValue (that.mMalfFeedRateBiasValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sublimator link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSublimatorInputData::~GunnsFluidSublimatorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Sublimator link model with obviously invalid
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSublimator::GunnsFluidSublimator()
    :
    GunnsFluidLink          (NPORTS),
    mMalfFeedRateBiasFlag   (false),
    mMalfFeedRateBiasValue  (0.0),
    mGasType                (FluidProperties::NO_FLUID),
    mHeatOfVaporization     (0.0),
    mHeatOfFusion           (0.0),
    mTriplePointTemperature (0.0),
    mMaxIceMass             (0.0),
    mIceCoverageFraction    (0.0),
    mMaxThermalConductivity (0.0),
    mPlateConductivity      (0.0),
    mThermalConductivityGain(0.0),
    mIceMass                (0.0),
    mStructureTemperature   (0.0),
    mHeatBalance            (0.0),
    mHeatOfSublimation      (0.0),
    mThermalConductivity    (0.0),
    mVentRelativeHumidity   (0.0),
    mSystemConductance      (0.0),
    mLiquidBreakthrough     (false),
    mVentFluid              (0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Sublimator link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSublimator::~GunnsFluidSublimator()
{
    TS_DELETE_OBJECT(mVentFluid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  feedwater supply port map index.
/// @param[in]      port1        (--)  vapor vent port map index.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Sublimator link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSublimator::initialize(const GunnsFluidSublimatorConfigData& configData,
                                      const GunnsFluidSublimatorInputData&  inputData,
                                      std::vector<GunnsBasicLink*>&         links,
                                      const int                             port0,
                                      const int                             port1)
{
    /// - First initialize & validate parent.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize model terms from configuration and input data.
    mGasType                 = configData.mGasType;
    mHeatOfVaporization      = configData.mHeatOfVaporization;
    mHeatOfFusion            = configData.mHeatOfFusion;
    mTriplePointTemperature  = configData.mTriplePointTemperature;
    mMaxIceMass              = configData.mMaxIceMass;
    mIceCoverageFraction     = configData.mIceCoverageFraction;
    mMaxThermalConductivity  = configData.mMaxThermalConductivity;
    mPlateConductivity       = configData.mPlateConductivity;
    mThermalConductivityGain = configData.mThermalConductivityGain;
    mIceMass                 = inputData.mIceMass;
    mStructureTemperature    = inputData.mStructureTemperature;
    mMalfFeedRateBiasFlag    = inputData.mMalfFeedRateBiasFlag;
    mMalfFeedRateBiasValue   = inputData.mMalfFeedRateBiasValue;

    /// - Initialize derived attributes from configuration and input data.
    derive();

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
///
/// @returns  void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Sublimator link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSublimator::validate(const GunnsFluidSublimatorConfigData& configData,
                                    const GunnsFluidSublimatorInputData&  inputData) const
{
    /// - Throw an exception if the operating fluid gas is not found in the network.
    try {
        mNodes[0]->getContent()->find(configData.mGasType);
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Operating fluid gas not found in network.");
    }

    /// - Throw an exception if heat of fusion <= 0.
    if (configData.mHeatOfFusion <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Heat of fusion <= 0.");
    }

    /// - Throw an exception if heat of vaporization <= 0.
    if (configData.mHeatOfVaporization <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Heat of vaporization <= 0.");
    }

    /// - Throw an exception if heat of fusion >= heat of vaporization.
    if (configData.mHeatOfVaporization <= configData.mHeatOfFusion) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Heat of vaporization <= heat of fusion.");
    }

    /// - Throw an exception if triple point T <= 0.
    if (configData.mTriplePointTemperature <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Triple point temperature <= 0.");
    }

    /// - Throw an exception if max ice mass <= 0.
    if (configData.mMaxIceMass <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Max ice mass <= 0.");
    }

    /// - Throw an exception if max thermal conductivity <= 0.
    if (configData.mMaxThermalConductivity <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Max thermal conductivity <= 0.");
    }

    /// - Throw an exception if ice coverage fraction !0-1.
    if (!MsMath::isInRange(0.0, configData.mIceCoverageFraction, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Ice coverage fraction not (0-1).");
    }

    /// - Throw an exception if plate conductivity <= 0.
    if (configData.mPlateConductivity <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Plate conductivity <= 0.");
    }

    /// - Throw an exception if thermal conductivity filter gain !0-1.
    if (!MsMath::isInRange(0.0, configData.mThermalConductivityGain, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Thermal conductivity filter gain not (0-1).");
    }

    /// - Throw an exception if initial ice mass < 0.
    if (inputData.mIceMass < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Ice mass < 0.");
    }

    /// - Throw an exception if initial temperature < 0.
    if (inputData.mStructureTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Structure temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSublimator::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mVentRelativeHumidity  = 0.0;
    mSystemConductance     = 0.0;
    mLiquidBreakthrough    = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @details  Initializes the attributes of this GUNNS Fluid Sublimator link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSublimator::derive()
{
    /// - Initialize heat of sublimation.
    mHeatOfSublimation = mHeatOfVaporization + mHeatOfFusion;

    /// - Build a PolyFluid for vent gas at freezing point, 100% operating fluid
    TS_DELETE_OBJECT(mVentFluid);
    TS_NEW_PRIM_OBJECT_EXT(mVentFluid, PolyFluid,
                           (*(mNodes[0]->getContent()), std::string(mName) + ".mVentFluid", false),
                           std::string(mName) + ".mVentFluid");

    double* massFractions = new double[mNodes[0]->getContent()->getNConstituents()];
    for (int i=0; i<mNodes[0]->getContent()->getNConstituents(); ++i) {
        massFractions[i] = 0.0;
    }
    massFractions[mVentFluid->find(mGasType)] = 1.0;

    mVentFluid->resetState();
    mVentFluid->setFlowRate(0.0);
    mVentFluid->setMassAndMassFractions(0.0, massFractions);
    mVentFluid->setTemperature(mTriplePointTemperature);
    delete [] massFractions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @returns  void
///
/// @details  Method to update the link during a time step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSublimator::step(const double dt)
{
    /// - Relative humidity. Ability to sublimate will approach zero as this approaches 1. This will
    ///   be modeled by scaling effective conductivity of the thermal potential link towards zero.
    mVentRelativeHumidity = MsMath::limitRange(0.0,
            GunnsFluidUtils::computeRelativeHumidityH2O(mNodes[1]->getContent()), 1.0);
    const bool sublimate = mVentRelativeHumidity < 1.0;

    /// - Calculate sublimation & melting rates using the heat balance from the thermal aspect.
    ///   Note the heats of sublimation & melting constants are converted from kJ to J.
    double sublimationRate = 0.0;
    if (sublimate && mIceMass > 0.0) {
        sublimationRate = std::max(0.0, -mHeatBalance / mHeatOfSublimation * UnitConversion::KILO_PER_UNIT);
    }
    const double meltingRate = std::max(0.0, -mHeatBalance / mHeatOfFusion * UnitConversion::KILO_PER_UNIT);

    /// - Default the ice formation rate to the melting rate.  If sublimation & boiling cannot occur
    ///   or if no feed liquid is available to form more ice, then the heat balance melts the ice.
    double iceRate  = -meltingRate;

    /// - Determine if there is sufficient feed pressure to flow.
    const double dP = mPotentialVector[0] - mPotentialVector[1];
    const bool feed = dP > 0.1;
    /// - Add instructor bias to the feed rate.  This has no effect on thermal performance.  The
    ///   bias cannot force flow if feed is not available.  The bias cannot force backflow into the
    ///   feed line.
    double feedRateBias = 0.0;
    if (feed && mMalfFeedRateBiasFlag) {
        feedRateBias = std::max(mMalfFeedRateBiasValue, -sublimationRate);
    }

    /// - Determine effective maximum ice pack mass based on plate size and blockage malfunction.
    ///   The blockage plugs some of the plate, reducing the volume that can contain ice.
    double effectiveMaxIceMass = mMaxIceMass;
    if (mMalfBlockageFlag) {
        effectiveMaxIceMass *= (1.0 - mMalfBlockageValue);
    }

    /// - Base class's system conductance is used to flow from the feed water port 0 to ground, and
    ///   includes feed water flows due to plate breakthrough, sublimation and feed rate bias.
    mSystemConductance = 1.0E-12;
    mLiquidBreakthrough = false;
    const double feedMWeight = mNodes[0]->getContent()->getMWeight();

    /// - When plate is not fully covered with ice, feed and icing rates depend on the breakthrough
    ///   liquid flow as well as sublimation.
    if (feed && mIceMass < effectiveMaxIceMass * mIceCoverageFraction) {

        /// - Feed demand is the sublimation rate plus the liquid breakthrough rate (plate vent)
        ///   based on delta-pressure, plate conductivity, and plate ice coverage.  Link blockage
        //    malfunction plugs some of the plate and reduces breakthrough rate.
        double plateConductivity = mPlateConductivity
                * (1.0 - mIceMass / (effectiveMaxIceMass * mIceCoverageFraction));
        if (mMalfBlockageFlag) {
            plateConductivity *= (1.0 - mMalfBlockageValue);
        }

        /// - Convert liquid breakthrough conductivity to GUNNS admittance.  Breakthrough flow rate
        ///   is a function of delta-pressure across the plate, even though we will not add this
        ///   flow to port 1 node.  Use only the port 0 density, not average, as breakthrough water
        ///   is not boiling and would keep its liquid density across the plate.
        mSystemConductance += plateConductivity
                * sqrt(UnitConversion::PA_PER_KPA * mNodes[0]->getContent()->getDensity() / dP)
                / feedMWeight;
        /// - Sublimation and feed bias rates are created by adding their converted rates to the
        ///   link admittance.
        mSystemConductance += (sublimationRate + feedRateBias) / mPotentialVector[0] / feedMWeight;

        /// - If boiling & sublimation is occurring, we assume all of the vented liquid is boiling.
        ///   The summed heat loss due to liquid boiling and ice sublimation forms more ice, based
        ///   on the ratio of the heats of vaporization to melting.  We scale the liquid boil-off by
        ///   the relative humidity - less water can boil as the vent gets more saturated with
        ///   vapor.  The sublimation rate already takes this into account.
        const FluidProperties* props = mNodes[0]->getContent()->getProperties(FluidProperties::GUNNS_H2O);
        if (sublimate && props->getSaturationPressure(mStructureTemperature) > mNodes[1]->getPotential()) {
            const double plateVentRate = mSystemConductance * dP * feedMWeight;
            iceRate = (sublimationRate + plateVentRate * (1.0 - mVentRelativeHumidity))
                               * mHeatOfVaporization / mHeatOfFusion;
        } else {
            mLiquidBreakthrough = true;
        }
    }

    /// - When the plate has full ice coverage, ice is formed by the heat of sublimation, and the
    ///   feed rate equals the sublimation rate + user bias rate.
    else if (feed && sublimate) {
        iceRate  = sublimationRate * mHeatOfVaporization / mHeatOfFusion;
        /// - Sublimation and feed bias rates are created by adding their converted rates to the
        ///   link admittance.
        mSystemConductance += (sublimationRate + feedRateBias) / mPotentialVector[0] / feedMWeight;
    }

    /// - When there is sublimation but no feed, heat balance goes to sublimating the ice, but there
    ///   is no more liquid to grow ice from, so ice mass drops at the sublimation rate.
    else if (sublimate) {
        iceRate = -sublimationRate;
    }

    /// - Calculate the new ice mass, limited to the valid range.
    mIceMass += iceRate * dt;
    mIceMass = MsMath::limitRange(0.0, mIceMass, effectiveMaxIceMass);

    /// - Calculate the thermal conductivity of the thermal aspect, to modulate how much heat the
    ///   sublimator can sink, based on the amount of ice in the pack (more ice absorbs more heat)
    ///   and the ability of the ice to sublimate to the vent.
    const double conductivity = mMaxThermalConductivity * (1.0 - mVentRelativeHumidity) *
                                mIceMass / mMaxIceMass;

    /// - Lag filter the actual thermal conductivity for stability.
    mThermalConductivity = (1.0 - mThermalConductivityGain) * mThermalConductivity +
            mThermalConductivityGain * conductivity;
    if (mThermalConductivity < DBL_EPSILON) {
        mThermalConductivity = 0.0;
    }

    /// - Build the link admittance matrix to create feed water flow from port 0 to ground.
    if (fabs(mAdmittanceMatrix[0] - mSystemConductance) > 0.0) {
        mAdmittanceMatrix[0]   = mSystemConductance;
        mAdmittanceMatrix[1]   = 0.0;
        mAdmittanceMatrix[2]   = 0.0;
        mAdmittanceMatrix[3]   = 0.0;
        mAdmittanceUpdate = true;
    }

    /// - Build the source vector for the network.  This creates a potential source from ground to
    ///   port 0 with port 1's pressure.  This causes the feed flow from port 0 to ground to
    ///   equalize to port 1's pressure.  We only add sublimated vapor to the vent node because it
    ///   is a gas-only node.  Liquid breakthrough is pulled from the feed side but not added to the
    ///   vent so it is effectively destroyed.
    mSourceVector[0] = mPotentialVector[1] * mSystemConductance;
    mSourceVector[1] = sublimationRate / mVentFluid->getMWeight();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used)
///
/// @returns  void
///
/// @details  Method to update the potential drop and flux and set port directions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSublimator::computeFlows(const double dt __attribute__((unused)))
{
    /// - Info only, delta-pressure across sublimator, not used for anything.
    mPotentialDrop = getDeltaPotential();

    /// - Base class mFlux, mFlowRate and mVolFlowRate are for the feed water flow into the
    ///   sublimator (ground) and do not account for vent flows.  Compute molar flow rate.
    mFlux = mPotentialVector[0] * mAdmittanceMatrix[0] - mSourceVector[0];
    const double ventFlow = mSourceVector[1] * mVentFluid->getMWeight();

    /// - Set port flow directions based on flow and vent rates
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;

    if (mFlux > m100EpsilonLimit){
        mPortDirections[0] = SOURCE;
        mNodes[0]->scheduleOutflux(mFlux);
    }

    if (ventFlow > m100EpsilonLimit){
        mPortDirections[1] = SINK;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used)
///
/// @returns  void
///
/// @details  Method to update flow quantities and transport fluid masses during the step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSublimator::transportFlows(const double dt __attribute__((unused)))
{
    /// - Convert the molar flow rate to mass rate using the molecular weight of the source node.
    mFlowRate = mFlux * mNodes[0]->getOutflow()->getMWeight();

    /// - Transport flows between the ports.  The trickle conductivity we create between ports 0 & 1
    ///   is only for pressure - we don't actually flow mass between them because they are of
    ///   different phase.
    if (mFlowRate > m100EpsilonLimit) {
        mNodes[0]->collectOutflux(mFlowRate);
    }
    const double ventFlow = mSourceVector[1] * mVentFluid->getMWeight();
    if (ventFlow > m100EpsilonLimit) {
        mNodes[1]->collectInflux(ventFlow, mVentFluid);
    }

    /// - Calculate true volumetric flow rate from the mass flow rate, using the density of the
    ///   source node.
    const double feedDensity = mNodes[0]->getOutflow()->getDensity();
    if (feedDensity > DBL_EPSILON) {
        mVolFlowRate = mFlowRate / feedDensity;
    } else {
        mVolFlowRate = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    port  (--)  The port to be assigned.
/// @param[in]    node  (--)  The desired node to assign the port to.
///
/// @returns  (--) True if all rules checks pass.
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           Gunns Fluid Sublimator link model.  These are:
///           - A GunnsFluidSublimator must not map port 0 to the network's vacuum boundary node.
///           - The node at port 0 must contain liquid phase.
///           - The node at port 1 must contain gas phase OR be the network's vacuum boundary node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidSublimator::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is the vacuum boundary node.
    if ((0 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to the boundary node.");
        result = false;
    }

    /// - Fail if port 0 is not in liquid phase.
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);
    if ((0 == port) && (FluidProperties::LIQUID != nodePtr[node].getContent()->getPhase())) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to a liquid node.");
        result = false;
    }

    /// - Fail if port 1 is not either the vacuum boundary node or in gas phase.
    if ((1 == port) && (node != getGroundNodeIndex()) &&
            (FluidProperties::GAS != nodePtr[node].getContent()->getPhase())) {
        GUNNS_WARNING(" aborted setting a port: must assign port 1 to a gas node or the boundary"
            << " node.");
        result = false;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--)   Malfunction activation flag, true activates
/// @param[in] value (kg/s) Malfunction value to bias the feed rate
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSublimator::setMalfFeedRateBias(const bool flag, const double value)
{
    mMalfFeedRateBiasFlag  = flag;
    mMalfFeedRateBiasValue = value;
}
