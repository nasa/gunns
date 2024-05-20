/*
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (core/GunnsFluidLink.o)
    (software/exceptions/TsInitializationException.o)
 )

 PROGRAMMERS:
 ((Tim Caldwell) (GHG) (Initial) (2012-03))
*/

#include "GunnsFluidAccum.hh"
#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsInitializationException.hh"

const int GunnsFluidAccum::LIQUID_PORT = 1;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @verbatim
///    <----->                           minChamberVolPercent                                <--->
///    <----->                           Minimum chamber volume for stability                <--->
///    <----------------->               minChamberVolDeadBandPercent              <------------->
///           <---------->               Dead band range                           <-------->
///    <-------------------------------- Total volume ------------------------------------------->
///    <-------------------------------- Accum volume(real world) -------------------------->
///    <-------------------------------- Maximum chamber volume ---------------------------->
///           <------------------------- Bellows movement range (0 - 100%) ----------------->
///     __________________________________________________________________________________________
///    |     |            |                                                       |          |    |
///    |     |            |                                                       |          |    |
///    |_____|____ _______|_______________________________________________________|__________|____|
///
///
///    minChamberVolPercent is multiplied to the accum volume and then added to it to create
///    a slightly larger accumulator total volume. This is for stability at the bellows limits.
///    minChamberVolPercent does not reduce the usable volume of the accumulator.
///
///    minChamberVolDeadBandPercent is multiplied to the accum volume. IT IS GREATER THAN
///    mMinChamberVolPercent and is part of the accum volume. It is used to compute the dead band
///    range. The dead band range is used to ramp down the liquid effective conductivity when the
///    bellows is nearing one of the limits.
///
/// @endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                             (--)     Link name
/// @param[in]  nodeList                         (--)     Network nodes array
/// @param[in]  maxConductivity                  (m2)     Maximum possible eff cond of the link
/// @param[in]  minConductivityScale             (--)     Min scale to crack open liquid side at bellows limit
/// @param[in]  accumVolume                      (m3)     Documented accumulator volume
/// @param[in]  minChamberVolPercent             (--)     Percent accum volume added for min chamber vol
/// @param[in]  minChamberVolDeadBandPercent     (--)     Percent accum volume for dead band
/// @param[in]  forceBellowsMaxRate              (--)     Fraction/sec. Max rate to force bellows
/// @param[in]  editHoldTime                     (s)      Hold time after edit for system stability
/// @param[in]  minTemperature                   (K)      Min temperature - all applicable chambers
/// @param[in]  maxTemperature                   (K)      Max temperature - all applicable chambers
/// @param[in]  maxPressure                      (kPa)    Max Pressure computed by accumulator.
/// @param[in]  springCoeff0;                    (kPa)    Spring pressure at bellows zero position, spring coefficient 0
/// @param[in]  springCoeff1;                    (--)     Spring coefficient 1, or spring constant, k (kPa/bellows position), in a linear spring system
/// @param[in]  springCoeff2;                    (--)     Spring coefficient 2 (kPa/bellows position squared)
/// @param[in]  fillModePressureThreshold;       (kPa)    Threshold for computing Fill Mode. Non-zero activates "one way" mEffConductivityScale ramping
/// @param[in]  effCondScaleRate;                (--)     Rate for "one way" mEffConductivityScale ramping (0-1)/sec
///
/// @details Default constructs this accumulator model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumConfigData::GunnsFluidAccumConfigData(const std::string& name,
                                                    GunnsNodeList* nodeList,
                                                    const double maxConductivity,               // suggest 1.0
                                                    const double minConductivityScale,          // suggest 0.1
                                                    const double accumVolume,                   // documented vol
                                                    const double minChamberVolPercent,          // suggest 5 - 10%
                                                    const double minChamberVolDeadBandPercent,  // suggest 10 - 15%
                                                    const double forceBellowsMaxRate,           // suggest 0.016
                                                    const double editHoldTime,                  // suggest 5.0
                                                    const double minTemperature,                // suggest liq freezing point
                                                    const double maxTemperature,                // suggest < liq boiling point
                                                    const double maxPressure,                   // suggest a little > max operating pressure
                                                    const double springCoeff0,                  // 0.0 and
                                                    const double springCoeff1,                  // 0.0 and
                                                    const double springCoeff2,                  // 0.0 to turn off spring.
                                                    const double fillModePressureThreshold,     // 0.0 use original mEffConductivityScale ramp. > 0.0 activates "one way" ramp.
                                                    const double effCondScaleOneWayRate)        // 0.0 Rate for "one way" ramp. (0-1)/sec
        :
        GunnsFluidLinkConfigData(name, nodeList),
        mMaxConductivity(maxConductivity),
        mMinConductivityScale(minConductivityScale),
        mAccumVolume(accumVolume),
        mMinChamberVolPercent(minChamberVolPercent),
        mMinChamberVolDeadBandPercent(minChamberVolDeadBandPercent),
        mForceBellowsMaxRate(forceBellowsMaxRate),
        mEditHoldTime(editHoldTime),
        mMinTemperature(minTemperature),
        mMaxTemperature(maxTemperature),
        mMaxPressure(maxPressure),
        mSpringCoeff0(springCoeff0),
        mSpringCoeff1(springCoeff1),
        mSpringCoeff2(springCoeff2),
        mFillModePressureThreshold(fillModePressureThreshold),
        mEffCondScaleOneWayRate(effCondScaleOneWayRate)
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this accumulator model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumConfigData::GunnsFluidAccumConfigData(const GunnsFluidAccumConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mMaxConductivity(that.mMaxConductivity),
    mMinConductivityScale(that.mMinConductivityScale),
    mAccumVolume(that.mAccumVolume),
    mMinChamberVolPercent(that.mMinChamberVolPercent),
    mMinChamberVolDeadBandPercent(that.mMinChamberVolDeadBandPercent),
    mForceBellowsMaxRate(that.mForceBellowsMaxRate),
    mEditHoldTime(that.mEditHoldTime),
    mMinTemperature(that.mMinTemperature),
    mMaxTemperature(that.mMaxTemperature),
    mMaxPressure(that.mMaxPressure),
    mSpringCoeff0(that.mSpringCoeff0),
    mSpringCoeff1(that.mSpringCoeff1),
    mSpringCoeff2(that.mSpringCoeff2),
    mFillModePressureThreshold(that.mFillModePressureThreshold),
    mEffCondScaleOneWayRate(that.mEffCondScaleOneWayRate)
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this accumulator model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumConfigData::~GunnsFluidAccumConfigData()
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag          (--)     Blockage malfunction flag
/// @param[in]  malfBlockageValue         (--)     Blockage malfunction fractional value (0-1)
/// @param[in]  initialBellowsPosition    (--)     Fraction of liquid chamber that is filled (0-1)
/// @param[in]  liquidFluidInputData      (--)     Input data for liquid fluid
///
/// @details Default constructs this accumulator model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumInputData::GunnsFluidAccumInputData(const bool   malfBlockageFlag,
                                                   const double malfBlockageValue,
                                                   const double initialBellowsPosition,
                                                   PolyFluidInputData* liquidFluidInputData)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mInitialBellowsPosition(initialBellowsPosition),
    mLiquidFluidInputData(liquidFluidInputData)
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this accumulator model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumInputData::GunnsFluidAccumInputData(const GunnsFluidAccumInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mInitialBellowsPosition(that.mInitialBellowsPosition),
    mLiquidFluidInputData(that.mLiquidFluidInputData)
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this accumulator model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccumInputData::~GunnsFluidAccumInputData()
{
    // Nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling the step
///           and computeFlows methods.
///
/// @details  Default constructs this accumulator model with purposely invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccum::GunnsFluidAccum()
    :
    GunnsFluidLink(NPORTS),
    mMalfBellowsStickFlag(false),
    mMalfBellowsStickToPosFlag(false),
    mMalfBellowsStickToPosValue(0.0),
    mMalfBellowsStickToPosRate(0.0),
    mMaxConductivity(0.0),
    mMinConductivityScale(0.0),
    mEffectiveConductivity(0.0),
    mEffConductivityScale(0.0),
    mLiqCapacitance(0.0),
    mBellowsPosition(0.0),
    mLiquidPressureReading(0.0),
    mLiquidHousingQ(0.0),
    mTotalVolume(0.0),
    mMinChamberVol(0.0),
    mMaxChamberVol(0.0),
    mMinDeadBandVol(0.0),
    mActiveVolRange(0.0),
    mLiquidVolume(0.0),
    mPressurizerVolume(0.0),
    mSpringCapacitance(0.0),
    mSpringCoeff0(0.0),
    mSpringCoeff1(0.0),
    mSpringCoeff2(0.0),
    mSpringPressure(0.0),
    mEditHoldTime(0.0),
    mEditBellowsFlag(false),
    mEditBellowsPosition(0.0),
    mEditBellowsRate(0.0),
    mEditBellowsTimer(0.0),
    mEditTemperatureFlag(false),
    mEditTemperatureValue(0.0),
    mHoldAccumFlag(false),
    mHoldTemperatureFlag(false),
    mPressurizerOrideBellowsFlag(false),
    mBellowsStuck(false),
    mForceBellowsMaxRate(0.0),
    mMinTemperature(0.0),
    mMaxTemperature(0.0),
    mMaxPressure(0.0),
    mBellowsMalfSet(false),
    mEditsAreReactive(true),
    mSimultaneousEditsInProgress(false),
    mFillMode(GunnsFluidAccum::EQUALIZED),
    mBellowsZone(GunnsFluidAccum::MIDDLE),
    mFillModePressureThreshold(0.0),
    mEffCondScaleOneWayRate(0.0),
    mAccelPressureHead(0.0)

{
    // Nothing to do
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this accumulator model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidAccum::~GunnsFluidAccum()
{
    TS_DELETE_OBJECT(mInternalFluid);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     void
///
/// @details    Updates the admittance matrix for this Gunns object. The normal admittance indices
///             (1,2) are zeroed out, because there is never flow between the two nodes. Instead,
///             flow to and from the accumulator is computed via the "virtual" paths to ground
///             that are inherant to the link, but normally ignored. For the node at port 1,
///             mAdmittanceMatrix[3] represents the path to ground. The accumulator model
///             acts as a GunnsFluidPotential, controlling the flow through this path.
///             mAdmittanceMatrix[0] is for the gas side (if used). Here, the admittance is zeroed.
///             If the derived class is a spring pressurizer, nothing is to be done except to
///             connect port 0 to vacuum. If a gas pressurizer is used, then port 0 must connect
///             to a gas node, and mAdmittanceMatrix[0] must be computed in the derived class
///             after this function has executed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::buildConductance()
{
    /// - Compute system conductance using effective conductivity.
    double liquidSystemConductance = MsMath::limitRange(0.0, mEffectiveConductivity, mConductanceLimit);
    /// - Set admittance matrix.
    if (fabs(mAdmittanceMatrix[3] - liquidSystemConductance) > 0.0) {
        /// - Zero out mAdmittanceMatrix[1] and [2]. No flow ever allowed between nodes.  Note we
        ///   don't zero [0] because that is reserved for the pressurizer to control.  It will be
        ///   updated by GunnsFluidAccumGas::buildGasConductance when applicable.  Because we don't
        ///   clear it here, if you manually set it via Trick View and there's no AccumGas to reset
        ///   it, you'll get a false leak out of the system.  So don't do that.
        mAdmittanceMatrix[1]   = 0.0;
        mAdmittanceMatrix[2]   = 0.0;
        /// - Set mAdmittanceMatrix[3] to flow between port 1 node and accumulator liquid chamber.
        mAdmittanceMatrix[3]   = liquidSystemConductance;
        mAdmittanceUpdate = true;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    port  (--)  The port to be assigned.
/// @param[in]    node  (--)  The desired node to assign the port to.
///
/// @returns  (--) True if all rules checks pass.
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           Gunns Fluid Accumulator link model.  These are:
///           - The node at port 0 must contain gas phase OR be the network's vacuum boundary node.
///           - The node at port 1 cannot be the network's vacuum boundary node.
///           - The node at port 1 must contain liquid phase.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidAccum::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is not either the vacuum boundary node or in gas phase.
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);
    if ((0 == port) && (node != getGroundNodeIndex()) &&
            (FluidProperties::GAS != nodePtr[node].getContent()->getPhase())) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: must assign port 1 to a gas node or the boundary"
            << " node.";
        hsSendMsg(msg);
        result = false;
    }
    /// - Fail if port 1 is the vacuum boundary node.
    if ((1 == port) && (node == getGroundNodeIndex())) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: cannot assign port 1 (liquid port) to the boundary node.";
        hsSendMsg(msg);
        result = false;
    }
    /// - Fail if port 1 is not in liquid phase.
    if ((1 == port) && (FluidProperties::LIQUID != nodePtr[node].getContent()->getPhase())) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: must assign port 1 to a liquid node.";
        hsSendMsg(msg);
        result = false;
    }
    return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  capacitance           (kg*mol/kPa) Fluid capacitance of the chamber.
/// @param[in]  maxConductivity       (m2)         Max conductivity limit.
/// @param[in]  currentConductivity   (m2)         Current conductivity.
/// @param[in]  minConductivity       (m2)         Min conductivity limit.
/// @param[in]  dt                    (s)          Delta time.
///
/// @return     (m2) Conductivity
///
/// @details    Computes conductivity from capacitance and delta time. Limits the returned value
///             based on min and max limits. Will eventually be moved to Gunns Utils.
///             Can also be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccum::computeConductivity(const double capacitance,
                                            const double maxConductivity,
                                            const double currentConductivity,
                                            const double minConductivity,
                                            const double dt) const
{
    double conductivity = currentConductivity;

    if (dt > DBL_EPSILON) {
        if (maxConductivity >= minConductivity) {
            /// - Divide capacitance by dt to get conductivity.
            conductivity = MsMath::limitRange(minConductivity, capacitance / dt, maxConductivity);

        } else {
            // If Max has been forced to be less than min for debugging, set to conductivity max.
            // This should not be done in nominal ops. It is for troubleshooting purposes.
            // Typically, the max could be set to 0.0 to close off one of the accumulator chambers.
            conductivity = MsMath::limitRange(0.0, capacitance / dt, maxConductivity);
        }
    }
    return conductivity;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flux            (kg*mol/s) Molar flow rate into the chamber.
/// @param[in]  associatedNode  (--)       Pointer to the node to compute flow for.
/// @param[in]  accumFluid      (m2)       Pointer to the internal fluid in the chamber.
///
/// @return     (kg/s) Flow rate
///
/// @details    Compute mass flow rate from molar flow rate and fluid properties.
///             Positive flow is into the accumulator. Negative flow is out of the accumulator.
///             Can also be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccum::computeFlowRate(const double     flux,
                                        GunnsBasicNode*  associatedNode,
                                        const PolyFluid* accumFluid) const
{
    /// - Convert the molar flow rate to mass rate using the molecular weight of the associated
    ///   node or accumulator chamber.
    double flowRate = 0.0;
    if (flux > 0.0) {
        flowRate = flux * associatedNode->getOutflow()->getMWeight();
    } else if (flux < 0.0) {
        flowRate = flux * accumFluid->getMWeight();
    }
    return flowRate;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  admittance      (kg*mol/kPa) Fluid admittance of the chamber.
/// @param[in]  associatedNode  (--)         Pointer to the node to compute flow for.
/// @param[in]  accumFluid      (m2)         Pointer to the internal fluid in the chamber.
///
/// @return     (kg*mol/s) Molar flow rate
///
/// @details    Compute molar flow rate from admittance and delta pressure.
///             Positive flow is into the accumulator. Negative flow is out of the accumulator.
///             Can also be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccum::computeFlux(const double     admittance,
                                    GunnsBasicNode*  associatedNode,
                                    const PolyFluid* accumFluid) const
{
    return (associatedNode->getPotential() - accumFluid->getPressure()) * admittance;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  flowRate       (kg/s)  Mass flow rate.
/// @param[in]  sourceNode     (--)    Pointer to node at inlet port.
/// @param[in]  internalFluid  (--)    Pointer to internal fluid.
///
/// @return     (kg/s) Flow rate
///
/// @details    Compute the volumetric flow rate from the mass flow rate and density.
///             Can also be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccum::computeVolFlowRate(const double     flowRate,
                                           GunnsBasicNode*  sourceNode,
                                           const PolyFluid* internalFluid) const
{
    double volFlowRate = 0.0;

    if (flowRate > 0.0) {
        /// - Positive flow is into the accumulator, so get the density from the node at the source port.
        volFlowRate = flowRate / sourceNode->getOutflow()->getDensity();
    } else if (flowRate < 0.0) {
        /// - Negative flow is out of the accumulator, so get the density from the internal fluid.
        volFlowRate = flowRate / internalFluid->getDensity();
    }

    return (volFlowRate);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Time step (not used).
///
/// @details    Compute the molar flow rate to/from the nodes as a result of the network solution,
///             schedules outflows from the nodes, and sets port direction flags for the flow
///             orchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::computeFlows(const double dt __attribute__((unused)))
{
    mFlux = computeFlux(mAdmittanceMatrix[3], mNodes[LIQUID_PORT], mInternalFluid);
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;
    if (mFlux > DBL_EPSILON) {
        mNodes[LIQUID_PORT]->scheduleOutflux(mFlux);
        mPortDirections[LIQUID_PORT] = SOURCE;
    } else if (mFlux < -DBL_EPSILON) {
        mPortDirections[LIQUID_PORT] = SINK;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  volume              (m3)      Chamber Volume.
/// @param[in]  density             (kg/m3)   Density of fluid.
///
/// @return   (kg)    New mass.
///
/// @details  Computes new mass from volume and density.
///           Can also be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccum::computeMass(const double volume, const double density) const
{
    return (std::max(volume * density, DBL_EPSILON));
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt           (s)      Delta time.
/// @param[in]  flowRate     (kg/s)   Flow rate.
/// @param[in]  currentMass  (kg)     Current mass.
///
/// @return   (kg)    New mass.
///
/// @details  Computes new mass from flow rate, delta time, and current mass.
///           Can also be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccum::computeMass(const double dt, const double flowRate, const double currentMass) const
{
    double newMass = currentMass;
    if (fabs(flowRate) > m100EpsilonLimit) {
        newMass = currentMass + flowRate * dt;
    }
    return (std::max(newMass, DBL_EPSILON));
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt                  (s)    Delta time.
/// @param[in]  flowRate            (kg/s) Flow rate.
/// @param[in]  inSpecificEnthalpy  (J/kg) Specific enthalpy of incoming flow.
/// @param[in]  accumFluid          (--)   Applicable fluid.
/// @param[in]  accumHousingQ       (J)    Heat input from accumulator housing.
///
/// @return     (K) Temperature of fluid in chamber.
///
/// @details    Compute temperature for the applicable accumulator chamber.
///             Can also be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccum::computeTemperature(const double dt,
                                           const double flowRate,
                                           const double inSpecificEnthalpy,
                                           const PolyFluid* accumFluid,
                                           const double accumHousingQ) const
{
    double newTemperature        = accumFluid->getTemperature();
    const double currentMass     = accumFluid->getMass();
    const double newMass         = computeMass(dt, flowRate, currentMass);

    if (!mHoldTemperatureFlag) {
        /// - Compute current heat within chamber.
        if (newMass > 0.0) {
            double flowHeatFlux = 0.0;
            if (flowRate < -m100EpsilonLimit) {
                flowHeatFlux = flowRate * accumFluid->getSpecificEnthalpy();
            } else if (flowRate > m100EpsilonLimit) {
                flowHeatFlux = flowRate * inSpecificEnthalpy;
            }
            const double currentHeat = currentMass * accumFluid->getSpecificEnthalpy();
            /// - Compute new heat based on current heat, heat from incoming fluid and heat from the chamber housing.
            const double newEnthalpy = (currentHeat + (accumHousingQ + flowHeatFlux) * dt) / newMass;
            /// - Compute new temperature based on new heat content.
            newTemperature = accumFluid->computeTemperature(newEnthalpy);
        } else {
            GUNNS_WARNING("newMass <= 0.0. compute temperature aborted.");
        }
    }
    /// - Limit temperature to be between within min and max limits.
    newTemperature = MsMath::limitRange(mMinTemperature, newTemperature, mMaxTemperature);

    return(newTemperature);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Computes and updates chamber volumes after initialize or when accumulator state is
///           forced.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::deriveChamberVolumes()
{
    mLiquidVolume =  mBellowsPosition * mActiveVolRange + mMinChamberVol;
    mLiquidVolume =  MsMath::limitRange(mMinChamberVol, mLiquidVolume, mMaxChamberVol);
    updatePressurizerVolume();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt           (s)      Delta time.
///
/// @return   void
///
/// @details  Calls forceLiqChamber with quantity edit terms. Releases edit when forced movement
///           is complete and timer is expired.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::editLiqChamber(const double dt)
{
    /// - Only update the temperature normally if edits are not reactive and there is not
    ///   a pressurizer edit also taking place. The pressurizer will handle the time frame
    ///   after the bellows edit is complete and the pressurizer edit is still active, using
    ///   the pressurizerOrideBellows method.
    ///   The pressurizer derived class must set mSimultaneousEditsInProgress.
    bool updateTemperatureNormally = (mEditsAreReactive && !mSimultaneousEditsInProgress);
    bool forceComplete = forceLiqChamber(dt, mEditBellowsPosition, mEditBellowsRate, updateTemperatureNormally);
    if (forceComplete and processTimer(dt, mEditBellowsTimer, mEditHoldTime)) {
        mEditBellowsFlag = false;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     Void
///
/// @details    Forces pressurizer chamber temperature (if present), based on base class temperature
///             edit terms.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::editPressurizerTemperature()
{
    // Place holder for derived gas pressurizer class
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      volume       (m3) Chamber volume.
/// @param[in,out]  accumFluid   (--) Chamber fluid.
///
/// @return         Void
///
/// @details        Force accumulator temperature, based on temperature edit terms.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::editTemperature(const double volume, PolyFluid* accumFluid)
{
    /// - Limit mEditTemperatureValue to be within min and max limits.
    mEditTemperatureValue = MsMath::limitRange(mMinTemperature, mEditTemperatureValue, mMaxTemperature);
    /// - Set fluid temperature to desired value.
    accumFluid->setTemperature(mEditTemperatureValue);
    /// - back calculate mass, so that density and bellows postiion are unaffected.
    double newMass = computeMass(volume, accumFluid->getDensity());
    accumFluid->setMass(newMass);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt       (s) Delta time.
///
/// @return     Void
///
/// @details    Calls forceLiqChamber with Malfunction terms. Sets mBellowsStuck flag where
///             applicable.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::failLiqChamber(const double dt)
{
    /// - Only 1 malfunction is possible at a time.
    /// - 1st priority is Bellows Stick malfunction.
    if (mMalfBellowsStickFlag) {
        mBellowsStuck = true;
    } else {  // mMalfBellowsStickToPosFlag
        /// - 2nd priority is Bellows Stick To Position malfunction.
        ///   Update temperature normally.
        mBellowsStuck = forceLiqChamber(dt, mMalfBellowsStickToPosValue, mMalfBellowsStickToPosRate, true);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt                    (s)  Delta time.
/// @param[in,out]  forcedBellowsPos      (--) Desired bellows position.
/// @param[in,out]  forceBellowsRate      (--)  Desired bellows travel rate (percent/sec).
/// @param[in]      updateTemperature     (--)  Update temperature flag.
///
/// @return         (--)  Force liquid chamber complete bool.
///
/// @details        Forces bellows position based on either edit or malfunction.
///                 Desired value and rate are limited within "rampValue".
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidAccum::forceLiqChamber(const double dt,
                                      double& forcedBellowsPos,
                                      double& forceBellowsRate,
                                      bool    updateTemperature)
{
    /// -  Ramp bellows position to desired value.
    bool forceBellowsComplete = rampValue(dt,
                                          mBellowsPosition,
                                          forcedBellowsPos,
                                          forceBellowsRate,
                                          0.0,                   // Lower bellows limit
                                          1.0,                   // upper bellows limit
                                          0.0,                   // force bellows rate low limit
                                          mForceBellowsMaxRate);
    /// - back calculate volumes and mass to match new bellows position, given the old pressure.
    deriveChamberVolumes();
    /// - Update temperature if allowed. Otherwise, set to existing value.
    if (updateTemperature) {
        mInternalFluid->setMass(computeMass(mLiquidVolume, mInternalFluid->getDensity()));
        mInternalFluid->setTemperature(computeTemperature(dt,
                                                          mFlowRate,
                                                          mNodes[LIQUID_PORT]->getOutflow()->getSpecificEnthalpy(),
                                                          mInternalFluid,
                                                          mLiquidHousingQ));

    } else {
        /// - Force temperature to remain as is.
        mInternalFluid->setTemperature(mInternalFluid->getTemperature());
        /// - Back calculate mass to match new pressure with old volume as well as forced temperature.
        /// - Do this after forcing temperature.
        mInternalFluid->setMass(computeMass(mLiquidVolume, mInternalFluid->getDensity()));
    }

    /// - If the bellows rupture malfunction is active, wait for the bellows position to be reached
    ///   before setting an indicator (used in GunnsFluidAccumGas to start updating again).
    if (mPressurizerOrideBellowsFlag and forceBellowsComplete) {
        mBellowsMalfSet = true;
    }
    return (forceBellowsComplete);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         double
///
/// @details        Returns the pressure from the pressurizer. Place holder for derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsFluidAccum::getPressurizerPressure()
{
    // Place holder for derived class.
    return(0.0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Config Data
/// @param[in]     inputData    (--) Input Data
/// @param[in,out] networkLinks (--) Gunns network links
/// @param[in]     port0        (--) Gunns network port 0
/// @param[in]     port1        (--) Gunns network port 1
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Initializes this accumulator model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::initialize(const GunnsFluidAccumConfigData& configData,
                             const GunnsFluidAccumInputData& inputData,
                             std::vector<GunnsBasicLink*>& networkLinks,
                             const int port0,      // Gas Node or ground
                             const int port1)      // Liquid Node
{
    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset initialization flag.
    mInitFlag = false;

    /// - Validate config and input data.
    validate(configData, inputData);

    /// - Initialize Gunns related attributes.
    mMaxConductivity = configData.mMaxConductivity;
    mMinConductivityScale = configData.mMinConductivityScale;
    mEffectiveConductivity = mMaxConductivity;
    mEffConductivityScale = 1.0;

    /// - Initialize accumulator chamber attributes.
    mBellowsPosition = inputData.mInitialBellowsPosition;
    mLiquidPressureReading = inputData.mLiquidFluidInputData->mPressure;
    mLiquidHousingQ = 0.0;

    mMinChamberVol = configData.mAccumVolume * configData.mMinChamberVolPercent / UnitConversion::PERCENTAGE;
    mMaxChamberVol = configData.mAccumVolume;
    mMinDeadBandVol = configData.mAccumVolume * configData.mMinChamberVolDeadBandPercent / UnitConversion::PERCENTAGE;
    mActiveVolRange = mMaxChamberVol - mMinChamberVol;
    mTotalVolume = configData.mAccumVolume + mMinChamberVol;
    deriveChamberVolumes();

    /// - Initialize fluid attributes.
    createInternalFluid(*inputData.mLiquidFluidInputData);
    mInternalFluid->setMass(computeMass(mLiquidVolume, mInternalFluid->getDensity()));
    mInternalFluid->setFlowRate(0.0);
    mFlowRate = 0.0;

    /// - Initialize spring attributes
    mSpringCoeff0 = configData.mSpringCoeff0;
    mSpringCoeff1 = configData.mSpringCoeff1;
    mSpringCoeff2 = configData.mSpringCoeff2;
    mSpringPressure = mBellowsPosition * mBellowsPosition * mSpringCoeff2 + mBellowsPosition * mSpringCoeff1 + mSpringCoeff0;
    double pressureSlope = 2 * mBellowsPosition * mSpringCoeff2 + mSpringCoeff1;
    if (pressureSlope > DBL_EPSILON) {
        mSpringCapacitance = 1.0/pressureSlope * mActiveVolRange * mInternalFluid->getDensity() / mInternalFluid->getMWeight();
    } else {       // If this case exists, then the derived class must compute the capacitance with a derived function.
        mSpringCapacitance = 0.0;
    }
    mLiqCapacitance = mSpringCapacitance;

    /// - Initialize Edit attributes
    mEditHoldTime = configData.mEditHoldTime;
    mEditBellowsFlag = false;
    mEditBellowsPosition = 0.0;
    mEditBellowsRate = configData.mForceBellowsMaxRate;
    mEditBellowsTimer = 0.0;
    mEditTemperatureFlag = false;
    mEditTemperatureValue = 0.0;
    mHoldAccumFlag = false;
    mHoldTemperatureFlag = false;
    mPressurizerOrideBellowsFlag = false;
    mBellowsMalfSet = false;

    /// - Initialize malfunction attributes.
    mMalfBellowsStickToPosRate = configData.mForceBellowsMaxRate;
    mBellowsStuck = false;

    /// - Initialize other maximums.
    mForceBellowsMaxRate = configData.mForceBellowsMaxRate;
    mMinTemperature = configData.mMinTemperature;
    mMaxTemperature = configData.mMaxTemperature;
    mMaxPressure = configData.mMaxPressure;

    mBellowsMalfSet = false;

    mEditsAreReactive = true;
    mSimultaneousEditsInProgress = false;

    mBellowsZone = GunnsFluidAccum::MIDDLE;
    mFillMode = GunnsFluidAccum::EQUALIZED;
    mFillModePressureThreshold = configData.mFillModePressureThreshold;
    mEffCondScaleOneWayRate = configData.mEffCondScaleOneWayRate;

    /// - Set Initialization flag.
    mInitFlag = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt           (s)      Delta time.
///
/// @return   (bool)  Timer expired.
///
/// @details  Place holder for pressurizer to override liquid chamber.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::pressurizerOrideBellows(const double dt __attribute__((unused)))
{
    // Place holder for derived class.
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt           (s)      Delta time.
/// @param[in]  timer        (s)      Current timer value.
/// @param[in]  timerLimit  (kg)      Timer limit.
///
/// @return   (bool)  Timer expired.
///
/// @details  Processes a timer (passed in by reference) for an accumulator edit.
///           Can also be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidAccum::processTimer(const double dt, double& timer, const double timerLimit)
{
    bool timerExpired = true;
    if (timer < timerLimit) {
        timer += dt;
        timerExpired = false;
    }
    return timerExpired;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      dt                (s)  Delta time.
/// @param[in,out]  currentValue      (--) Current value to be ramped.
/// @param[in,out]  targetValue       (--) Target value to ramp to.
/// @param[in,out]  rate              (--) Ramping rate.
/// @param[in]      targetLowerLimit  (--) Lower limit for target.
/// @param[in]      targetUpperLimit  (--) Upper limit for target.
/// @param[in]      rateLowerLimit    (--) Lower limit for rate.
/// @param[in]      rateUpperLimit    (--) Upper limit for rate.
///
/// @return         (--) Ramp complete bool.
///
/// @details        Ramps the current value towards the target value. Target value and rate are
///                 limited to the upper and lower limits. Parameters are modified to alert the
///                 user that these values have been limited. Works with any units. Can also
///                 be used by derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidAccum::rampValue(const double dt,
                                double& currentValue,
                                double& targetValue,
                                double& rate,
                                const double targetLowerLimit,
                                const double targetUpperLimit,
                                const double rateLowerLimit,
                                const double rateUpperLimit)
{
    double rampComplete = false;
    /// - Limit target value.
    targetValue = MsMath::limitRange(targetLowerLimit, targetValue, targetUpperLimit);
    /// - Limit ramp rate.
    rate = MsMath::limitRange(rateLowerLimit, rate, rateUpperLimit);
    double step = rate * dt;

    /// - Approach target without overshooting.
    if (targetValue > currentValue) {
        currentValue = std::min(currentValue + step, targetValue);
    } else if (targetValue < currentValue) {
        currentValue = std::max(currentValue - step, targetValue);
    }
    // Check for currentValue == target this iteration
    if (targetValue == currentValue) {
        /// - Return true if finished.
        rampComplete = true;
    }
    return (rampComplete);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         Void
///
/// @details        Sets all edit flags to false and all timers to 0.0. Call reset for possible
///                 derived class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::resetEditFlagsAndTimers()
{
    mEditBellowsFlag = false;
    mEditBellowsTimer = 0.0;
    mEditTemperatureFlag = false;
    resetPressurizerEditFlagsAndTimers();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return         Void
///
/// @details        Sets all pressurizer edit flags to false and all timers to 0.0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::resetPressurizerEditFlagsAndTimers()
{
    // Place holder for derived pressurizer class
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config and non-checkpointed terms.
    mBellowsZone = MIDDLE;
    mFillMode    = EQUALIZED;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) Malfunction activation flag, true activates
///
/// @details  Sets the stick the bellows in its current position malf flag to given state.
///           Calling this method with default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::setMalfBellowsStick(const bool flag)
{
    mMalfBellowsStickFlag = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (--) Malfunction position value to move to and stick there
/// @param[in] rate  (--) Fraction/sec, Rate for bellows movement
///
/// @details  Sets the stick the bellows to the given position malf flag to given state.
///           Calling this method with default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::setMalfBellowsStickToPos(const bool flag, const double value, const double rate)
{
    mMalfBellowsStickToPosFlag  = flag;
    mMalfBellowsStickToPosValue = value;
    mMalfBellowsStickToPosRate  = rate;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s)  Delta time.
///
/// @return     Void
///
/// @details    Step through a frame. Update capacitance, effective conductivity, and call update
///             state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::step(const double dt)
{
    /// - update capacitance and conductivity
    updateCapacitance();
    updateEffectiveConductivity(dt);
    /// - Call update state to update admittance, conductance, and potential
    updateState(dt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      flowRate       (kd/s)  Flow rate.
/// @param[in,out]  associatedNode (--)    Node associated with given chamber.
/// @param[in]      AccumFluid     (s)     Accumulator chamber fluid.
///
/// @return     Void
///
/// @details  This function adds the accumulators flow to the node's collection terms.
///           Positive flow is into the accumulator. Negative flow is out of the accumulator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::transportAccumFluid(const double flowRate,
                                          GunnsBasicNode* associatedNode,
                                          PolyFluid* AccumFluid)
{
    if (flowRate > m100EpsilonLimit) {
        /// - If flow is into the accumulator, remove flow from the node.
        associatedNode->collectOutflux(flowRate);
    } else if (flowRate < -m100EpsilonLimit) {
        /// - If flow is out of the accumulator, insert flow and fluid data
        ///   into the node.
        associatedNode->collectInflux(-flowRate, AccumFluid);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt           (s) Delta time.
///
/// @return     Void
///
/// @details    Calls update fluid for liquid chamber.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::transportFlows(const double dt)
{
    updateFluid(dt, 0.0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     Void
///
/// @details    Update capacitance of the liquid chamber. Should be overridden in derived class.
///             If no derived class, capacitance = spring capacitance.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updateCapacitance()
{
    double pressureSlope = 2 * mBellowsPosition * mSpringCoeff2 + mSpringCoeff1
                         + mAccelPressureHead / std::max(mBellowsPosition, DBL_EPSILON);
    if (pressureSlope > DBL_EPSILON) {
        mSpringCapacitance = 1.0/pressureSlope * mActiveVolRange * mInternalFluid->getDensity() / mInternalFluid->getMWeight();
    } else {       // If this case exists, then the derived class must compute the capacitance with a derived function.
        mSpringCapacitance = 0.0;
    }
    mLiqCapacitance = mSpringCapacitance;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @return     Void
///
/// @details    Update effective conductivity scale for liquid side. This is used to close off the
///             link when the bellows position is approaching a limit. When the bellows is at 0%
///             or 100% and pressure is such that the bellows will not move, the pressurizer
///             pressure can no longer affect the liquid side. This is represented by a zero
///             effective conductivity. In this state, the link is closed off, and liquid pressure
///             diverges from pressurizer pressure. In order to provide a smooth transition,
///             the conductivity is closed off gradually once the liquid or pressurizer volume
///             is between the mMinDeadBandVol and the mMinChamberVol. This also provides the
///             additional benefit of simulating the stiffening of the bellows mechanism as it
///             approaches a limit.
///
///             Liquid chamber pressure is always equal to pressurizer pressure(spring and gas if
///             present), but the mLiquidPressureReading (sensor truth value) is equal to the
///             port 1 node pressure. When the conductivity is closed off, liquid chamber pressure
///             will still equal pressurizer pressure, but the liquid pressure reading will show
///             the proper effect.
///
///
///             Note about mFillModePressureThreshold and mEffCondScaleOneWayRate:
///
///             In some applications it is desirable to have little to no resistance when filling
///             an empty accumulator or emptying a full one. In these cases, scaling the conductivity
///             while trying to move the bellows away from a hard stop gives a sluggish signature.
///
///             If mFillModePressureThreshold > 0, then the accumulator will compute the effective
///             conductivity scale differently depending on whether the bellows position is
///             approaching a hard stop or leaving a hard stop.
///
///             When approaching, it will decrease the effective conductivity scale based on how close
///             the bellows position is to the hard stop (original logic). But when moving away from a
///             hard stop, the effective conductivity scale will be ramped up to 1.0 using a time-based rate.
///             The intent is that the scale be increased to 1.0 much quicker than it would be using bellows
///             position, while at the same time not being slammed to 1.0, which can cause instabilities.
///             the first application for this feature was in ISS EVAS for the water bags. The
///             mEffCondScaleOneWayRate used for that case was 2.0 (fraction/sec). At 10hz, this meant that
///             the conductivity scale increased from 0.0 to 1.0 is 0.5 seconds. This was fast enough to avoid
///             a sluggish signature but also slow enough to avoid instability. This case involved 3 accumulators
///             open to each other along with several pressure sensitive valves and a pump - across 3 GUNNS
///             networks through supply/demand interfaces.
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updateEffConductivityScale(const double dt)
{

    /// - Verify dead band range is > 0.0. If not, report warning and return without updating
    ///   scale value.
    double minDeadBandRange = mMinDeadBandVol - mMinChamberVol;
    if (minDeadBandRange <= 0.0) {
        GUNNS_WARNING("has Min Dead Band Range <= 0.0. Invalid volume config. Conductivity scale update aborted.");
        return;
    }

    /// - Capture inside and outside pressures.
    double insideliquidPressure = mInternalFluid->getPressure();
    double outsideliquidPressure = mNodes[LIQUID_PORT]->getPotential();

    /// - Set up target scale and rate for "one way" scale calculations.
    double oneWayTargetScale = 1.0;
    double oneWayTargetScaleHighRateLimit = 0.0;
    if (dt > 0.0) {
        oneWayTargetScaleHighRateLimit = oneWayTargetScale / dt;
    }

    /// - Determine fill mode
    if (outsideliquidPressure - insideliquidPressure > mFillModePressureThreshold) {
        mFillMode = GunnsFluidAccum::FILLING;
    } else if (insideliquidPressure - outsideliquidPressure > mFillModePressureThreshold) {
        mFillMode = GunnsFluidAccum::DRAINING;
    } else {
        mFillMode = GunnsFluidAccum::EQUALIZED;
    }

    double fullDeadbandVol = mMaxChamberVol - minDeadBandRange;
    /// - Determine bellows zone
    if (mLiquidVolume <= mMinChamberVol) {
        mBellowsZone = GunnsFluidAccum::EMPTY;
    } else if (mLiquidVolume >= mMaxChamberVol) {
        mBellowsZone = GunnsFluidAccum::FULL;
    } else if (mLiquidVolume < mMinDeadBandVol and mLiquidVolume > mMinChamberVol) {
        mBellowsZone = GunnsFluidAccum::EMPTY_DEADBAND;
    } else if (mLiquidVolume > fullDeadbandVol and mLiquidVolume < mMaxChamberVol) {
        mBellowsZone = GunnsFluidAccum::FULL_DEADBAND;
    } else {
        mBellowsZone = GunnsFluidAccum::MIDDLE;
    }

    /// - Determine effective conductivity scale.
    if (mBellowsStuck) {
        /// - Close off link if bellows is stuck due to a malfunction. Allows pressures to diverge.
        mEffConductivityScale = 0.0;
    } else if (mBellowsZone == GunnsFluidAccum::MIDDLE or mPressurizerOrideBellowsFlag or mEditBellowsFlag) {
        /// - Set scale to 1.0. Fill mode and mFillModePressureThreshold don't matter.
        mEffConductivityScale = 1.0;

    } else if (mFillModePressureThreshold > DBL_EPSILON and mFillMode != GunnsFluidAccum::EQUALIZED) {
        /// - Bellows is inside a dead band or at min/max.
        /// - mFillModePressureThreshold is being used. calculate "one way" effective conductivity
        ///   scale. Only ramp down if approaching a hard stop. If leaving a hard stop, set to 1.0.
        ///   if fill mode is EQUALIZED, use original logic (next else block). One way ramping not necessary.
        ///   Atypical usage. Use if zero resistance is required in the beginning of filling and empty
        ///   accumulator or draining a full one.
        if (mFillMode == GunnsFluidAccum::FILLING) {
            if (mBellowsZone == GunnsFluidAccum::FULL_DEADBAND) {
                /// - buffer approach to filled hard stop.
                mEffConductivityScale = MsMath::limitRange(0.0, (mPressurizerVolume - mMinChamberVol) / minDeadBandRange, 1.0);

            } else if (mBellowsZone == GunnsFluidAccum::FULL) {
                /// - Close link if at full hard stop.
                mEffConductivityScale = 0.0;
            } else {
                /// - Either in the middle or near empty and moving away from hard stop.
                ///   Ramp scale for stability (not tied to bellows position).
                ///   This is especially needed when multiple accumulators
                ///   are exposed to each other and are at the limits. Multiple accumulators are more
                ///   likely to get into a feedback loop and jump on and off the hard stops, making the
                ///   conductivity scale jump back and forth between 0.0 and 1.0.
                rampValue(dt,
                          mEffConductivityScale,            // Value to be ramped
                          oneWayTargetScale,                // Target
                          mEffCondScaleOneWayRate,                // Rate
                          0.0,                              // Low target limit
                          1.0,                              // High target limit
                          0.0,                              // Rate low limit
                          oneWayTargetScaleHighRateLimit);  // Rate high limit
            }
        } else if (mFillMode == GunnsFluidAccum::DRAINING) {
            if (mBellowsZone == GunnsFluidAccum::EMPTY_DEADBAND) {
                /// - buffer approach to empty hard stop.
                mEffConductivityScale = MsMath::limitRange(0.0, (mLiquidVolume - mMinChamberVol) / minDeadBandRange, 1.0);

            } else if (mBellowsZone == GunnsFluidAccum::EMPTY) {
                /// - Close link if at empty hard stop.
                mEffConductivityScale = 0.0;
            }else {
                /// - Either in the middle or near empty and moving away from hard stop.
                ///   Ramp scale for stability (not tied to bellows position).
                ///   This is especially needed when multiple accumulators
                ///   are exposed to each other and are at the limits. Multiple accumulators are more
                ///   likely to get into a feedback loop and jump on and off the hard stops, making the
                ///   conductivity scale jump back and forth between 0.0 and 1.0.
                rampValue(dt,
                          mEffConductivityScale,            // Value to be ramped
                          oneWayTargetScale,                // Target
                          mEffCondScaleOneWayRate,                // Rate
                          0.0,                              // Low target limit
                          1.0,                              // High target limit
                          0.0,                              // Rate low limit
                          oneWayTargetScaleHighRateLimit);  // Rate high limit
            }
        }
    } else {
        /// - Bellows is inside a dead band or at min/max.
        /// - mFillModePressureThreshold is not being used. Calculate effective conductivity
        ///   scale based on bellows zone only. Fill mode is irrelevant.
        ///   EQUALIZED mode uses this logic regardless of mFillModePressureThreshold.
        ///   original, simpler logic used by most applications.
       if (mBellowsZone == GunnsFluidAccum::EMPTY_DEADBAND) {
            /// - If liquid volume is between dead band and minimum volume, scale the conductivity based on delta to
            ///   min volume.
            mEffConductivityScale = MsMath::limitRange(0.0, (mLiquidVolume - mMinChamberVol) / minDeadBandRange, 1.0);

       } else if (mBellowsZone == GunnsFluidAccum::FULL_DEADBAND) {
            /// - If pressurizer volume is between dead band and minimum volume, scale the conductivity based on delta to
            ///   min volume.
            mEffConductivityScale = MsMath::limitRange(0.0, (mPressurizerVolume - mMinChamberVol) / minDeadBandRange, 1.0);

       } else if ((mBellowsZone == GunnsFluidAccum::EMPTY and mFillMode == GunnsFluidAccum::FILLING) or
                   (mBellowsZone == GunnsFluidAccum::FULL and mFillMode == GunnsFluidAccum::DRAINING)) {
            /// - If bellows is at limit and pressure delta will push bellows off limit, crack open the conductivity.
            mEffConductivityScale = mMinConductivityScale;

       } else {
            /// - If bellows is at limit and pressure delta will not push bellows off limit, close link.
            mEffConductivityScale = 0.0;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt               (s)           Delta time.
///
/// @return     Void
///
/// @details    Update effective conductivity of liquid side, based on liquid capacitance and
///             conductivity scale. By dynamically adjusting the conductivity, stability is
///             provided given changing volumes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updateEffectiveConductivity(const double dt)
{
    /// - Compute conductivity based on liquid capacitance and conductivity scale.
    updateEffConductivityScale(dt);
    mEffectiveConductivity = mEffConductivityScale *
                                  computeConductivity(mLiqCapacitance,
                                                      mMaxConductivity,
                                                      mEffectiveConductivity,
                                                      0.0,
                                                      dt);
    /// - Limit conductivity to be between 0.0 and mMaxConductivity.
    mEffectiveConductivity = MsMath::limitRange(0.0, mEffectiveConductivity, mMaxConductivity);
    /// - call update effective conductivity for pressurizer
    updatePressurizerEffCond(dt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt           (s) Delta time.
///
/// @return     Void
///
/// @details    Update accumulator fluid state. Solve volumes, bellows position, mass, temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updateFluid(const double dt, const double)
{
    // Set bellows stuck to false by default. malfunction logic will set it true if applicable.
    mBellowsStuck = false;
    /// - Compute mass flow rate
    mFlowRate = computeFlowRate(mFlux, mNodes[LIQUID_PORT], mInternalFluid);
    /// - Compute volumetric flow rate
    mVolFlowRate = computeVolFlowRate(mFlowRate, mNodes[LIQUID_PORT], mInternalFluid);
    /// - Check for temperature edit first. This can happen at any time, regardless of malfs
    ///   or other overrides. Pressurizer temperature edit is also called here. It is not
    ///   called within updatePressurizerFluid, because of the edit flag reset. This is the only
    ///   edit shared directly between the liquid and pressurizer chambers.
    if (mEditTemperatureFlag) {
        /// - Process temperature edit if active.
        editTemperature(mLiquidVolume, mInternalFluid);
        editPressurizerTemperature();
        mEditTemperatureFlag = false;
    }
    if (mPressurizerOrideBellowsFlag) {
        pressurizerOrideBellows(dt);
    } else if (mMalfBellowsStickFlag or mMalfBellowsStickToPosFlag) {
        /// - Process malfunctions if active.
        resetEditFlagsAndTimers();
        failLiqChamber(dt);
    } else {
        if (mEditBellowsFlag) {
            /// - Process quantity edit if active.
            editLiqChamber(dt);
        } else {
            mEditBellowsTimer = 0.0;
            /// - Update mass, volume, temperature and bellows position.
            updateLiqChamber(dt);
        }
    }
    /// - Transport fluid to and from associated Gunns nodes.
    transportAccumFluid(mFlowRate, mNodes[LIQUID_PORT], mInternalFluid);
    /// - Update derived pressurizer fluid if present.
    updatePressurizerFluid(dt);
    /// - Update pressure of liquid side.
    updatePressure(dt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt    (s)  Delta time.
///
/// @return     Void
///
/// @details    Update liquid chamber during nominal conditions. Update temperature, mass, chamber
///             volumes, and bellows position.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updateLiqChamber(const double dt)
{
    /// - If hold accum flag set, do not update.
    if (mHoldAccumFlag) {
        return;
    }

    /// - Update temperature.
    mInternalFluid->setTemperature(computeTemperature(dt,
                                                      mFlowRate,
                                                      mNodes[LIQUID_PORT]->getOutflow()->getSpecificEnthalpy(),
                                                      mInternalFluid,
                                                      mLiquidHousingQ));
    /// - Update liquid mass.
    double newMass = computeMass(dt, mFlowRate, mInternalFluid->getMass());
    mInternalFluid->setMass(newMass);
    double density = mInternalFluid->getDensity();

    /// - Update liquid and pressurizer volumes.
    if (density > 0.0) {
        mLiquidVolume = MsMath::limitRange(mMinChamberVol, newMass / density, mMaxChamberVol);
    } else {
        GUNNS_WARNING("liquid density too small. Liquid volume update aborted.");
    }
    updatePressurizerVolume();
    /// - Update bellows position.
    if (mActiveVolRange > 0.0) {
        mBellowsPosition = (mLiquidVolume - mMinChamberVol) / mActiveVolRange;
        mBellowsPosition = MsMath::limitRange(0.0, mBellowsPosition, 1.0);
    } else {
        GUNNS_WARNING("has Active Volume Range <= 0.0. Invalid volume config. bellows position update aborted.");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s)  Delta time.
///
/// @return     Void
///
/// @details    Update pressurizer effective conductivity.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updatePressurizerEffCond(const double dt __attribute__((unused)))
{
    // Place holder for derived gas pressurizer class
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s)  Delta time.
///
/// @return     Void
///
/// @details    Update pressurizer fluid - Flow rate, mass, temperature, pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updatePressurizerFluid(const double dt __attribute__((unused)))
{
    // Place holder for derived pressurizer class
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s)  Delta time.
///
/// @return     Void
///
/// @details    Update pressurizer state - admittance, conductance, source pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updatePressurizerState(const double dt __attribute__((unused)))
{
    // Place holder for derived pressurizer class
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s)  Delta time.
///
/// @return     Void
///
/// @details    Update pressure of liquid chamber. Combine spring force and gas chamber
///             pressure if present.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updatePressure(const double dt __attribute__((unused)))
{
    /// - Update spring pressure
    mSpringPressure = mBellowsPosition * mBellowsPosition * mSpringCoeff2 + mBellowsPosition * mSpringCoeff1 + mSpringCoeff0;
    /// - Update the liquid pressure to be the combination of spring, acceleration pressure head,
    ///   and gas chamber pressures.  When acceleration pressure head is used, the resulting total
    ///   pressure is for the 'bottom' of the liquid column, which should be the liquid entrance/exit.
    mInternalFluid->setPressure(MsMath::limitRange(DBL_EPSILON,
                                                   mSpringPressure + mAccelPressureHead + getPressurizerPressure(),
                                                   mMaxPressure));
    /// - Update the liquid pressure reading to be the same as the liquid node pressure.
    ///   When conditions arise that would make the liquid and gas pressures to diverge,
    ///   it is seen through the pressure "readings". This is done by closing off
    ///   the conductivity to the liquid side of the network.
    mLiquidPressureReading = mNodes[LIQUID_PORT]->getPotential();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return     Void
///
/// @details    Update pressurizer volume. This exists to eliminate a round off issue that occurs
///             when the liquid volume is at max and min. The pressurizer volume is just under
///             the corresponding volume, which causes erroneous behavior.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updatePressurizerVolume()
{
    if (mLiquidVolume >= mMaxChamberVol) {
        mPressurizerVolume = mMinChamberVol;
    } else if (mLiquidVolume <= mMinChamberVol) {
        mPressurizerVolume = mMaxChamberVol;
    } else {
        mPressurizerVolume = MsMath::limitRange(mMinChamberVol, mTotalVolume - mLiquidVolume, mMaxChamberVol);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt (s)  Delta time.
///
/// @return     Void
///
/// @details    Update accumulator state. Solve link admittance, conductance, and potential.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::updateState(const double dt)
{
    /// - Build conductance and potential.
    buildConductance();
    buildPotential();
    /// - Call update state for the pressurizer
    updatePressurizerState(dt);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
/// @param[in]  inputData   (--)  Input data.
///
/// @return     void
///
/// @throws     TsInitializationException
///
/// @details    Validates accumulator configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidAccum::validate(const GunnsFluidAccumConfigData& configData,
                               const GunnsFluidAccumInputData& inputData) const
{
    const double bellowsMidPoint = UnitConversion::PERCENTAGE / 2.0;
    /// - Throw an exception on max conductivity < 0.0.
    if (configData.mMaxConductivity < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has max conductivity < 0.");
    }
    /// - Throw an exception on minConductivityScale < DBL_EPSILON or > 1.0.
    if (!MsMath::isInRange(DBL_EPSILON, configData.mMinConductivityScale, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Min Conductivity Scale must be between DBL_EPSILON and 1.0.");
    }
    /// - Throw an exception on accumulator volume <= FLT_EPSILON.
    if (configData.mAccumVolume <= static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Accumulator Volume cannot be <= FLT_EPSILON.");
    }
    /// - Throw an exception on Minimum chamber volume percentage <= 0.0.
    if (configData.mMinChamberVolPercent <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Minimum chamber volume percentage must be > 0.0.");
    }
    /// - Throw an exception on Minimum chamber volume percentage >= 50.0.
    if (configData.mMinChamberVolPercent >= bellowsMidPoint) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Minimum chamber volume percentage must be < 50 percent.");
    }
    /// - Throw an exception on minimum Chamber Volume Deadband <= minimum chamber volume percentage.
    if (configData.mMinChamberVolDeadBandPercent <= configData.mMinChamberVolPercent) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Minimum Chamber Volume Deadband must be > Minimum Chamber Volume Percentage.");
    }
    /// - Throw an exception on minimum Chamber Volume Deadband >= 50%.
    if (configData.mMinChamberVolDeadBandPercent >= bellowsMidPoint) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Minimum Chamber Volume Deadband must be < 50 percent.");
    }
    /// - Throw an exception on Force Bellows Max Rate <= 0.0.
    if (configData.mForceBellowsMaxRate <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Force Bellows Max Rate must be > 0.0.");
    }
    /// - Throw an exception on Edit hold time < 0.0.
    if (configData.mEditHoldTime < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Edit hold time cannot be < 0.0.");
    }
    /// - Throw an exception on Min Temperature <= 0.0.
    if (configData.mMinTemperature <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Min Temperature must be > 0.0 degrees K.");
    }
    /// - Throw an exception on Max Temperature <= 0.0.
    if (configData.mMaxTemperature <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Max Temperature must be > 0.0 degrees K.");
    }
    /// - Throw an exception on Max Temperature <= Min Temperature.
    if (configData.mMaxTemperature <= configData.mMinTemperature) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Max Temperature must be > Min Temperature.");
    }
    /// - Throw an exception on Max pressure < DBL_EPSILON.
    if (configData.mMaxPressure < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Max Pressure must be > DBL_EPSILON.");
    }
    /// - Throw an exception on one-way effective conductivity scale rate <= 0 if fill mode pressure threshold > 0.
    if (configData.mFillModePressureThreshold > DBL_EPSILON and configData.mEffCondScaleOneWayRate < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "One-way effective conductivity scale rate must be > DBL_EPSILON if fill mode pressure threshold > DBL_EPSILON.");
    }
    /// - Throw an exception on Initial bellows position < 0.0 or > 1.0.
    if (!MsMath::isInRange(0.0, inputData.mInitialBellowsPosition, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Initial bellows position must be between 0.0 and 1.0.");
    }
    /// - Throw an exception on fluid input data pointer being NULL.
    if (0 == inputData.mLiquidFluidInputData) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Liquid fluid input data is NULL.");
    }
}
