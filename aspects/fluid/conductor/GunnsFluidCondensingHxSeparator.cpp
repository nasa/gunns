/**
@file
@brief    GUNNS Fluid Condensing Heat Exchanger & Phase Separator implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
(Classes for the Condensing Heat Exchanger & Phase Separator fluid aspect.)

LIBRARY DEPENDENCY:
((core/GunnsFluidConductor.o))

PROGRAMMERS:
((Jason Harvey) (L-3 Communications) (March 2013) (Install))
*/

#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "GunnsFluidCondensingHxSeparator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                (--)              Name of object.
/// @param[in]  nodes               (--)              Pointer to nodes.
/// @param[in]  maxConductivity     (m2)              Max conductivity.
/// @param[in]  hxNumSegments       (--)              Number of heat exchanger segments.
/// @param[in]  hxDryHtc            (W/K)             Heat exchanger dry heat transfer coefficient.
/// @param[in]  hxWetHtcFactor      (1/kg)            Contribution of condensate to HX heat coefficient.
/// @param[in]  hxWetFlowFactor     (1/kg)            Contribution of condensate to HX link conductivity.
/// @param[in]  hxEvaporationCoeff  (kg/s/kPa)        Heat exchanger evaporation coefficient.
/// @param[in]  slurperFlowMassDry  (kg)              HX condensate to begin slurper flow from PAUSED_DRY.
/// @param[in]  slurperFlowMassWet  (kg)              HX condensate to begin slurper flow from PAUSED_WET.
/// @param[in]  slurperFlowRateRef  (kg/s)            Slurper condensate flow rate at WS reference speed.
/// @param[in]  wsMassExponent      (--)              Water separator mass factor exponent.
/// @param[in]  wsMaxCondensate     (kg)              Water separator maximum liquid mass capacity.
/// @param[in]  wsReferenceSpeed    (revolution/min)  Water separator reference speed.
/// @param[in]  wsReferencePressure (kPa)             Water separator pressure heat at reference speed.
/// @param[in]  wsReferenceTorque   (N*m)             Water separator motor torque at reference speed.
/// @param[in]  wsDriveRatio        (--)              Water separator gear ratio of motor to drum speed.
/// @param[in]  wsEvaporationCoeff  (kg/s/kPa)        Water separator evaporation coefficient.
///
/// @details  Default constructs this GUNNS Fluid Condensing Heat Exchanger & Separator config data.
///
/// @note     To avoid impacts to legacy users, the new base class GunnsFluidCondensingHxConfigData
///           terms for the heat transfer coefficient are defaulted so that this original link's
///           performance is kept, i.e. only the base 0th order HTC coefficient is used, and the
///           higher-order terms are defaulted to have no effect.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxSeparatorConfigData::GunnsFluidCondensingHxSeparatorConfigData(
                                                             const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const double       maxConductivity,
                                                             const int          hxNumSegments,
                                                             const double       hxDryHtc,
                                                             const double       hxWetHtcFactor,
                                                             const double       hxWetFlowFactor,
                                                             const double       hxEvaporationCoeff,
                                                             const double       slurperFlowMassDry,
                                                             const double       slurperFlowMassWet,
                                                             const double       slurperFlowRateRef,
                                                             const double       wsMassExponent,
                                                             const double       wsMaxCondensate,
                                                             const double       wsReferenceSpeed,
                                                             const double       wsReferencePressure,
                                                             const double       wsReferenceTorque,
                                                             const double       wsDriveRatio,
                                                             const double       wsEvaporationCoeff)
    :
    GunnsFluidCondensingHxConfigData(name, nodes, maxConductivity, hxNumSegments, hxDryHtc, 0.0,
                                     1.0, 1.0E15),
    mHxWetHtcFactor     (hxWetHtcFactor),
    mHxWetFlowFactor    (hxWetFlowFactor),
    mHxEvaporationCoeff (hxEvaporationCoeff),
    mSlurperFlowMassDry (slurperFlowMassDry),
    mSlurperFlowMassWet (slurperFlowMassWet),
    mSlurperFlowRateRef (slurperFlowRateRef),
    mWsMassExponent     (wsMassExponent),
    mWsMaxCondensate    (wsMaxCondensate),
    mWsReferenceSpeed   (wsReferenceSpeed),
    mWsReferencePressure(wsReferencePressure),
    mWsReferenceTorque  (wsReferenceTorque),
    mWsDriveRatio       (wsDriveRatio),
    mWsEvaporationCoeff (wsEvaporationCoeff)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Condensing Heat Exchanger & Separator config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxSeparatorConfigData::GunnsFluidCondensingHxSeparatorConfigData(
                                              const GunnsFluidCondensingHxSeparatorConfigData& that)
    :
    GunnsFluidCondensingHxConfigData(that),
    mHxWetHtcFactor     (that.mHxWetHtcFactor),
    mHxWetFlowFactor    (that.mHxWetFlowFactor),
    mHxEvaporationCoeff (that.mHxEvaporationCoeff),
    mSlurperFlowMassDry (that.mSlurperFlowMassDry),
    mSlurperFlowMassWet (that.mSlurperFlowMassWet),
    mSlurperFlowRateRef (that.mSlurperFlowRateRef),
    mWsMassExponent     (that.mWsMassExponent),
    mWsMaxCondensate    (that.mWsMaxCondensate),
    mWsReferenceSpeed   (that.mWsReferenceSpeed),
    mWsReferencePressure(that.mWsReferencePressure),
    mWsReferenceTorque  (that.mWsReferenceTorque),
    mWsDriveRatio       (that.mWsDriveRatio),
    mWsEvaporationCoeff (that.mWsEvaporationCoeff)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Condensing Heat Exchanger & Separator condig data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxSeparatorConfigData::~GunnsFluidCondensingHxSeparatorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag    (--)              Blockage malfunction flag.
/// @param[in]  malfBlockageValue   (--)              Blockage malfunction fractional value (0-1).
/// @param[in]  wallTemperature     (K)               Initial default wall temperature all segments.
/// @param[in]  wsMotorSpeed        (revolution/min)  Initial speed of the water separator motor.
/// @param[in]  wsCondensateMass    (kg)              Initial mass of condensate in separator.
/// @param[in]  hxCondensateMass    (kg)              Initial mass of condensate in heat exchanger.
/// @param[in]  transferFlowRate    (kg/s)            Initial mass flow rate to liquid network.
/// @param[in]  slurperState        (--)              Initial Slurper/HX flow state.
///
/// @details  Default constructs this GUNNS Fluid Condensing Heat Exchanger & Separator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxSeparatorInputData::GunnsFluidCondensingHxSeparatorInputData(
                            const bool                                           malfBlockageFlag,
                            const double                                         malfBlockageValue,
                            const double                                         wallTemperature,
                            const double                                         wsMotorSpeed,
                            const double                                         wsCondensateMass,
                            const double                                         hxCondensateMass,
                            const double                                         transferFlowRate,
                            const GunnsFluidCondensingHxSeparator::SlurperStates slurperState)
    :
    GunnsFluidCondensingHxInputData(malfBlockageFlag, malfBlockageValue, wallTemperature),
    mWsMotorSpeed    (wsMotorSpeed),
    mWsCondensateMass(wsCondensateMass),
    mHxCondensateMass(hxCondensateMass),
    mTransferFlowRate(transferFlowRate),
    mSlurperState    (slurperState)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Condensing Heat Exchanger & Separator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxSeparatorInputData::GunnsFluidCondensingHxSeparatorInputData(
                                               const GunnsFluidCondensingHxSeparatorInputData& that)
    :
    GunnsFluidCondensingHxInputData(that),
    mWsMotorSpeed    (that.mWsMotorSpeed),
    mWsCondensateMass(that.mWsCondensateMass),
    mHxCondensateMass(that.mHxCondensateMass),
    mTransferFlowRate(that.mTransferFlowRate),
    mSlurperState    (that.mSlurperState)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Condensing Heat Exchanger & Separator input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxSeparatorInputData::~GunnsFluidCondensingHxSeparatorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Condensing Heat Exchanger & Separator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxSeparator::GunnsFluidCondensingHxSeparator()
    :
    GunnsFluidCondensingHx(),
    mMalfHxCondensateFlag (false),
    mMalfHxCondensateValue(0.0),
    mHxWetHtcFactor       (0.0),
    mHxWetFlowFactor      (0.0),
    mHxEvaporationCoeff   (0.0),
    mSlurperFlowMassDry   (0.0),
    mSlurperFlowMassWet   (0.0),
    mWsMassExponent       (0.0),
    mWsMaxCondensate      (0.0),
    mWsDriveRatio         (0.0),
    mWsEvaporationCoeff   (0.0),
    mSlurperFlowFactor    (0.0),
    mWsPowerCurveCoeff    (0.0),
    mWsTorqueFactor       (0.0),
    mWsMotorSpeed         (0.0),
    mWsCondensateMass     (0.0),
    mHxCondensateMass     (0.0),
    mTransferFlowRate     (0.0),
    mSlurperState         (PAUSED_DRY),
    mSlurperFlowRate      (0.0),
    mWsDrumSpeed          (0.0),
    mWsEvaporationRate    (0.0),
    mWsDeltaPressure      (0.0),
    mTransferTemperature  (0.0),
    mTransferPressure     (0.0),
    mWsMotorTorque        (0.0),
    mLiquidOverflow       (false)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Condensing Heat Exchanger & Separator.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidCondensingHxSeparator::~GunnsFluidCondensingHxSeparator()
{
    TS_DELETE_OBJECT(mCondensateFluid);
    TS_DELETE_ARRAY (mSegmentHeat);
    TS_DELETE_ARRAY (mSegmentTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  Nominal inlet port map index.
/// @param[in]      port1        (--)  Nominal outlet port map index.
///
/// @callgraph
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Condensing Heat Exchanger & Separator with config and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::initialize(
                                        const GunnsFluidCondensingHxSeparatorConfigData& configData,
                                        const GunnsFluidCondensingHxSeparatorInputData&  inputData,
                                        std::vector<GunnsBasicLink*>&                    links,
                                        const int                                        port0,
                                        const int                                        port1)
{
    /// - First initialize & validate parent.
    GunnsFluidCondensingHx::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag              = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize with configuration data.
    mHxWetHtcFactor        = configData.mHxWetHtcFactor;
    mHxWetFlowFactor       = configData.mHxWetFlowFactor;
    mHxEvaporationCoeff    = configData.mHxEvaporationCoeff;
    mSlurperFlowMassDry    = configData.mSlurperFlowMassDry;
    mSlurperFlowMassWet    = configData.mSlurperFlowMassWet;
    mWsMassExponent        = configData.mWsMassExponent;
    mWsMaxCondensate       = configData.mWsMaxCondensate;
    mWsDriveRatio          = configData.mWsDriveRatio;
    mWsEvaporationCoeff    = configData.mWsEvaporationCoeff;

    /// - Initialize attributes derived from configuration data.
    mSlurperFlowFactor     = configData.mSlurperFlowRateRef  / configData.mWsReferenceSpeed;
    mWsPowerCurveCoeff     = configData.mWsReferencePressure / configData.mWsReferenceSpeed
                           / pow(mWsMaxCondensate, mWsMassExponent);
    mWsTorqueFactor        = configData.mWsReferenceTorque   / configData.mWsReferenceSpeed;

    /// - Initialize input data.
    mWsMotorSpeed          = inputData.mWsMotorSpeed;
    mWsCondensateMass      = inputData.mWsCondensateMass;
    mHxCondensateMass      = inputData.mHxCondensateMass;
    mTransferFlowRate      = inputData.mTransferFlowRate;
    mSlurperState          = inputData.mSlurperState;

    /// - Initialize Malfunctions.
    mMalfHxCondensateFlag  = false;
    mMalfHxCondensateValue = 0.0;

    /// - Initialize state data.
    mSlurperFlowRate       = 0.0;
    mWsDrumSpeed           = 0.0;
    mWsEvaporationRate     = 0.0;
    mWsDeltaPressure       = 0.0;

    /// - Initialize outputs.
    mTransferTemperature   = 0.0;
    mTransferPressure      = 0.0;
    mWsMotorTorque         = 0.0;
    mLiquidOverflow        = false;

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag              = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
/// @param[in]  inputData   (--)  Input data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Condensing Heat Exchanger & Separator initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::validate(
                                   const GunnsFluidCondensingHxSeparatorConfigData& configData,
                                   const GunnsFluidCondensingHxSeparatorInputData&  inputData) const
{
    /// - Throw an exception on wet flow factor < 0.
    if (configData.mHxWetFlowFactor < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "HX wet flow factor < 0.");
    }

    /// - Throw an exception on heat exchanger evaporation coefficient < 0.
    if (configData.mHxEvaporationCoeff < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "HX evaporation coefficient < 0.0.");
    }

    /// - Throw an exception on slurper flow start dry mass < DBL_EPSILON.
    if (configData.mSlurperFlowMassDry < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Slurper flow start dry mass < DBL_EPSILON.");
    }

    /// - Throw an exception on slurper flow start wet mass < DBL_EPSILON.
    if (configData.mSlurperFlowMassWet < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Slurper flow start wet mass < DBL_EPSILON.");
    }

    /// - Throw an exception on slurper reference flow rate < DBL_EPSILON.
    if (configData.mSlurperFlowRateRef < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Slurper reference flow rate < DBL_EPSILON.");
    }

    /// - Throw an exception on WS mass exponent not (0.1 - 10)
    if (!MsMath::isInRange(0.1, configData.mWsMassExponent, 10.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "WS mass exponent not in (0.1 to 10.0).");
    }

    /// - Throw an exception on liquid mass capacity < FLT_EPSILON.
    if (configData.mWsMaxCondensate < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "WS maximum condensate capacity < FLT_EPSILON.");
    }

    /// - Throw an exception on reference speed < FLT_EPSILON.
    if (configData.mWsReferenceSpeed < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "WS reference speed < FLT_EPSILON.");
    }

    /// - Throw an exception on reference pressure < FLT_EPSILON.
    if (configData.mWsReferencePressure < static_cast<double>(FLT_EPSILON)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "WS reference pressure < FLT_EPSILON.");
    }

    /// - Throw an exception on reference torque > 0.
    if (configData.mWsReferenceTorque > 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "WS reference torque > 0.");
    }

    /// - Throw an exception on drive ratio < 0.
    if (configData.mWsDriveRatio < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "WS drive ratio < 0.");
    }

    /// - Throw an exception on separator evaporation coefficient < 0.
    if (configData.mWsEvaporationCoeff < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "WS evaporation coefficient < 0.0.");
    }

    /// - Throw an exception on separator speed < 0.
    if (inputData.mWsMotorSpeed < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "WS motor speed < 0.");
    }

    /// - Throw an exception on mass of liquid in separator < 0.
    if (inputData.mWsCondensateMass < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Mass of liquid in WS < 0.");
    }

    /// - Throw an exception on mass of liquid in HX < 0.
    if (inputData.mHxCondensateMass < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Mass of liquid in HX < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::restartModel()
{
    /// - Reset the base class.
    GunnsFluidCondensingHx::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mSlurperFlowRate   = 0.0;
    mWsDrumSpeed       = 0.0;
    mWsEvaporationRate = 0.0;
    mWsDeltaPressure   = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
///
/// @details  Updates the effective conductivity of the link and the water separator drum speed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::updateState(const double dt __attribute__((unused)))
{
    /// - Add the contribution of condensation on the HX fins to restrict airflow.  For example a
    ///   mHxWetFlowFactor of 0.01 reduces the link conductivity by 1% per kg of condensate mass.
    mEffectiveConductivity *= MsMath::limitRange(0.0, 1.0 - mHxWetFlowFactor * mHxCondensateMass, 1.0);

    /// - Protect for divide by zero on drive ratio; also setting drive ratio to zero disconnects
    ///   the drum from the motor, allowing manual control of the drum speed for tuning.
    if (mWsDriveRatio > DBL_EPSILON) {
        mWsDrumSpeed = mWsMotorSpeed / mWsDriveRatio;
    } else {
        GUNNS_WARNING("separator drum is disconnected from motor.");
    }
    mWsMotorTorque = mWsDrumSpeed * mWsTorqueFactor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
/// @param[in]  flowRate  (kg/s)  Mass flow rate.
///
/// @callgraph
///
/// @details  Calls the base class updateFluid moethid, then calls models for the slurper bar and
///           phase separator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::updateFluid(const double dt, const double flowRate)
{
    GunnsFluidCondensingHx::updateFluid(dt, flowRate);
    updateSlurper();
    updateWaterSeparator(dt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calls the base class to compute the nominal heat transfer coefficient, then multiplies
///           it by the effect of the pooled condensate mass on the fins.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::computeHeatTransferCoefficient()
{
    GunnsFluidCondensingHx::computeHeatTransferCoefficient();
    mSegmentHtc *= std::max(0.0, (1.0 + mHxWetHtcFactor * mHxCondensateMass));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
///
/// @details  Calls the base class method to model the bulk air heat transfer and condensation, then
///           keeps track of the pooled condensate mass on the fins.  Models evaporation from the
///           pool back to the bulk air flow when there is no condensation.  Condensation adds to
///           the pool, and evaporation and outflow to the slurper bar reduce the pool.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::updateHeatExchanger(const double dt)
{
    GunnsFluidCondensingHx::updateHeatExchanger(dt);

    /// - If the heat exchanger is not condensing, and there is significant pooled condensate mass
    ///   and air flow, calculate evaporation rate and store it as negative mCondensationRate, so it
    ///   will be moved from the condensate pool to the air stream.
    if (DBL_EPSILON < mFlowRate and DBL_EPSILON > mCondensationRate and DBL_EPSILON < mHxCondensateMass) {

        const FluidProperties* propertiesH2O = mNodes[0]->getOutflow()->getProperties(FluidProperties::GUNNS_H2O);
        double tOut = mInternalFluid->getTemperature();
        mCondensationRate = -mHxEvaporationCoeff * std::max( 0.0,
                            (propertiesH2O->getSaturationPressure(tOut)
                             - mInternalFluid->getPartialPressure(FluidProperties::GUNNS_H2O)) );

        /// - Add the evaporation back into the bulk air flow.
        mInternalFluid->addState(mCondensateFluid, -mCondensationRate);
        mInternalFluid->setPressure(mNodes[0]->getPotential());
        mInternalFluid->setTemperature(tOut);

        /// - Heat of vaporization, converted to J/kg.
        mLatentHeat = mCondensationRate * propertiesH2O->getHeatOfVaporization(tOut)
                    * UnitConversion::UNIT_PER_KILO;

        /// - Divide up the total latent heat evenly among the segments.
        for (int i = 0; i < mNumSegments; ++i) {
            mSegmentHeat[i] += mLatentHeat / mNumSegments;
        }
    }

    /// - Re-total heat flux from the fluid to the wall for the entire HX.
    mTotalHeat = mSensibleHeat + mLatentHeat;

    /// - Update the pooled condensate mass in the HX, allowing for override malfunction.  The malf
    ///   only affects the rate at which mass is pooled, and it doesn't affect the mass actually
    ///   removed from the air.
    double poolRate = mCondensationRate;
    if (mMalfHxCondensateFlag) {
        poolRate    = mMalfHxCondensateValue;
    }
    mHxCondensateMass = std::max(mHxCondensateMass + (poolRate - mSlurperFlowRate) * dt, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Determines whether the heat exchanger is flowing condensate through the slurper to the
///           water separator.  Before flow can begin, the surface area of the HX fins, slurper bars
///           & slurper drain pipe take time to become wetted with condensate.  Starting from a
///           completely dry state (mSlurperState = PAUSED_DRY), the wetted mass must reach a
///           certain amount (mSlurperFlowMassDry) before flow begins.  Once flow starts, liquid
///           surface tension and the HX surface's hydrophilic coating keeps all new condensation
///           flowing until the total condensate mass reaches zero.  At this point, if condensation
///           is still occurring (state = PAUSED_WET), flow will resume at a smaller pooled mass
///           (mSlurperFlowMassWet).
///
///           This is a simplified model of HX surface flow, but it suffices to give the general
///           signature of initial flow from a dry state taking a long time to begin, followed by
///           occasional but shorter pauses, depending on the relative condensation & WS flow rates.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::updateSlurper()
{
    const bool pooled     = mHxCondensateMass > static_cast<double>(FLT_EPSILON);
    const bool condensing = mCondensationRate > static_cast<double>(FLT_EPSILON);
    const bool separating = mWsDrumSpeed      > static_cast<double>(FLT_EPSILON);

    /// - Slurper state transition conditions, implemented in order below:
    ///     FLOWING    to PAUSED_WET if (condensate = 0 or WS speed = 0) & condensing
    ///     FLOWING    to PAUSED_DRY if (condensate = 0 or WS speed = 0) & not condensing
    ///     PAUSED_WET to PAUSED_DRY if (condensate = 0 or WS speed = 0) & not condensing
    ///     PAUSED_WET to FLOWING    if (condensate > start wet)
    ///     PAUSED_DRY to FLOWING    if (condensate > start dry)
    ///    (PAUSED_DRY to PAUSED_WET never happens)
    switch (mSlurperState) {
        case (FLOWING) :
            if (not separating or not pooled) {
                if (condensing) {
                    mSlurperState = PAUSED_WET;
                } else {
                    mSlurperState = PAUSED_DRY;
                }
            }
            break;

        case (PAUSED_WET) :
            if ((not separating or not pooled) and not condensing) {
                mSlurperState = PAUSED_DRY;
            } else if (mHxCondensateMass > mSlurperFlowMassWet) {
                mSlurperState = FLOWING;
            }
            break;

        default :   // mSlurperState = PAUSED_DRY
            if (mHxCondensateMass > mSlurperFlowMassDry) {
                mSlurperState = FLOWING;
            }
            break;
    }

    /// - WS pulls flow from the HX through the slurper proportional to WS speed.
    if (FLOWING == mSlurperState) {
        mSlurperFlowRate = mSlurperFlowFactor * mWsDrumSpeed;
    } else {
        mSlurperFlowRate = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt        (s)     Time step.
///
/// @details  Stores condensate and models pressure head created in the spinning WS drum and
///           handshakes with the external liquid system for pumping condensate out of the WS.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::updateWaterSeparator(const double dt)
{
    /// - Simplified model of evaporation for dryout of the separator when the air is not saturated.
    ///   Remove evaporated mass from the WS drum.  As a simplification since it is a negligible
    ///   amount of water, we neglect to add the evaporated mass back into the air.
    if (mFlowRate > DBL_EPSILON) {
        const FluidProperties* propertiesH2O = mNodes[0]->getOutflow()->getProperties(FluidProperties::GUNNS_H2O);
        mWsEvaporationRate = mWsEvaporationCoeff * std::max( 0.0,
                         (propertiesH2O->getSaturationPressure(mInternalFluid->getTemperature())
                          - mInternalFluid->getPartialPressure(FluidProperties::GUNNS_H2O)) );
    } else {
        mWsEvaporationRate = 0.0;
    }

    /// - Store condensate from the HX by way of the slurper in the WS drum.  Remove mass
    ///   transferred from the separator to the liquid network.  As a simplification, do not allow
    ///   backflow into the separator drum from the liquid network.
    mWsCondensateMass += (mSlurperFlowRate - mWsEvaporationRate - std::max(0.0, mTransferFlowRate))
                       * dt;

    /// - If the liquid side is depressurized and the separator drum is empty of liquid, then gas
    ///   would flow from the gas side to the liquid side; however we don't bother to simulate this.
    ///   Instead, we cancel the flow demand and liquid mass is created in the liquid side (mass not
    ///   conserved).
    mWsCondensateMass = std::max(mWsCondensateMass, 0.0);

    /// - Indicate if the WS drum has overflowed with too much condensate.  This can be used for
    ///   output to a liquid detection sensor signal aspect.
    mLiquidOverflow   = mWsCondensateMass > mWsMaxCondensate;

    /// - Compute separator delta pressure on liquid as function of speed and mass.
    mWsDeltaPressure  = mWsPowerCurveCoeff * mWsDrumSpeed * pow(mWsCondensateMass, mWsMassExponent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Prepares outputs to the liquid separator aspect.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::processOutputs()
{
    mTransferTemperature = mNodes[1]->getContent()->getTemperature();
    mTransferPressure    = mNodes[1]->getPotential() + mWsDeltaPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (--) HX condensation pool rate override malfunction value
///
/// @details  Sets the malf parameters to given the values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidCondensingHxSeparator::setMalfHxCondensate(const bool flag, const double value)
{
    mMalfHxCondensateFlag  = flag;
    mMalfHxCondensateValue = value;
}
