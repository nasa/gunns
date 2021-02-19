/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Provides the classes for modeling human metabolics within a GUNNS fluid network.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidSource.o))

PROGRAMMERS:
   ((Kenneth McMurtrie) (Tietronix Software) (Prototype) (2011-06))
**************************************************************************************************/

#include "math/MsMath.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "GunnsFluidMetabolic.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                --      Name of object
/// @param[in,out] nodes               --      Pointer to nodes.
/// @param[in]     consumptionRateO2   (kg/s)  Nominal O2 consumption rate for a crew member.
/// @param[in]     productionRateCO2   (kg/s)  Nominal CO2 production rate for a crew member.
/// @param[in]     productionRateH2O   (kg/s)  Nominal CO2 production rate for a crew member.
/// @param[in]     productionRateHeat  (W)     Nominal heat production rate for a crew member.
/// @param[in]     sleepScaleFactor     --     Sleep production/consumption scale factor.
/// @param[in]     recoveryScaleFactor  --     Recovery production/consumption scale factor.
/// @param[in]     exerciseScaleFactor  --     Exercise production/consumption scale factor.
///
/// @details  Default constructs this GUNNS Fluid Metabolic link model configuration data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolicConfigData::GunnsFluidMetabolicConfigData(const std::string& name,
                                                             GunnsNodeList*     nodes,
                                                             const double       consumptionRateO2,
                                                             const double       productionRateCO2,
                                                             const double       productionRateH2O,
                                                             const double       productionRateHeat,
                                                             const double       sleepScaleFactor,
                                                             const double       recoveryScaleFactor,
                                                             const double       exerciseScaleFactor)
    :
    GunnsFluidSourceConfigData(name, nodes),
    mConsumptionRateO2(consumptionRateO2),
    mProductionRateCO2(productionRateCO2),
    mProductionRateH2O(productionRateH2O),
    mProductionRateHeat(productionRateHeat),
    mSleepScaleFactor(sleepScaleFactor),
    mRecoveryScaleFactor(recoveryScaleFactor),
    mExerciseScaleFactor(exerciseScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Metabolic link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolicConfigData::GunnsFluidMetabolicConfigData(
                                                          const GunnsFluidMetabolicConfigData& that)
    :
    GunnsFluidSourceConfigData(that),
    mConsumptionRateO2(that.mConsumptionRateO2),
    mProductionRateCO2(that.mProductionRateCO2),
    mProductionRateH2O(that.mProductionRateH2O),
    mProductionRateHeat(that.mProductionRateHeat),
    mSleepScaleFactor(that.mSleepScaleFactor),
    mRecoveryScaleFactor(that.mRecoveryScaleFactor),
    mExerciseScaleFactor(that.mExerciseScaleFactor)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Metabolic link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolicConfigData::~GunnsFluidMetabolicConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag   --      Blockage malfunction flag.
/// @param[in]    malfBlockageValue  --      Blockage malfunction fractional value (0-1).
/// @param[in]    flowDemand         (kg/s)  Initial demanded mass flow rate of the link.
/// @param[in]    nNominal           --      Number of crew members in nominal state.
/// @param[in]    nSleep             --      Number of crew members in sleep state.
/// @param[in]    nRecovery          --      Number of crew members in recovery state.
/// @param[in]    nExercise          --      Number of crew members in exercise state.
///
/// @details  Default constructs this GUNNS Fluid Metabolic link model input data with arguments
///           and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolicInputData::GunnsFluidMetabolicInputData(const bool   malfBlockageFlag,
                                                           const double malfBlockageValue,
                                                           const double flowDemand,
                                                           const double nNominal,
                                                           const double nSleep,
                                                           const double nRecovery,
                                                           const double nExercise)
    :
    GunnsFluidSourceInputData(malfBlockageFlag, malfBlockageValue, flowDemand),
    mNNominal(nNominal),
    mNSleep(nSleep),
    mNRecovery(nRecovery),
    mNExercise(nExercise)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  --   Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Metabolic link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolicInputData::GunnsFluidMetabolicInputData(const GunnsFluidMetabolicInputData& that)
    :
    GunnsFluidSourceInputData(that),
    mNNominal(that.mNNominal),
    mNSleep(that.mNSleep),
    mNRecovery(that.mNRecovery),
    mNExercise(that.mNExercise)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Metabolic link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolicInputData::~GunnsFluidMetabolicInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Metabolic link model with obviously invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic::GunnsFluidMetabolic()
    :
    GunnsFluidSource(),
    mNCrew(),
    mO2ConsumptionRate(),
    mCO2ProductionRate(),
    mH2OProductionRate(),
    mHeatProductionRate(),
    mConsumedO2(0.0),
    mProducedCO2(0.0),
    mProducedH2O(0.0),
    mProducedHeat(0.0),
    mO2(-1),
    mCO2(-1),
    mH2O(-1)
{
    /// - Initialize the arrays.
    for (int i = 0; i < GunnsFluidMetabolic::NO_METABOLIC; ++i) {
        mNCrew[i]              = 0.0;
        mO2ConsumptionRate[i]  = 0.0;
        mCO2ProductionRate[i]  = 0.0;
        mH2OProductionRate[i]  = 0.0;
        mHeatProductionRate[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidMetabolic::~GunnsFluidMetabolic()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
/// @param[in,out]  links        --  Link vector.
/// @param[in]      port0        --  Nominal inlet port map index.
/// @param[in]      port1        --  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Metabolic link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic::initialize(const GunnsFluidMetabolicConfigData& configData,
                                     const GunnsFluidMetabolicInputData&  inputData,
                                     std::vector<GunnsBasicLink*>&        links,
                                     const int                            port0,
                                     const int                            port1)
{
    /// - First initialize & validate parent.
    GunnsFluidSource::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize derived attributes from configuration and input data.
    derive(configData, inputData);

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic::validate(const GunnsFluidMetabolicConfigData& configData,
                                   const GunnsFluidMetabolicInputData&  inputData) const
{
    /// - Throw an exception on negative O2 consumption rate.
    if (configData.mConsumptionRateO2 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "O2 consumption rate < 0.0.");
    }

    /// - Throw an exception on negative CO2 production rate.
    if (configData.mProductionRateCO2 < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "CO2 production rate < 0.0.");
    }

    /// - Throw an exception on negative H2O production rate.
    if (configData.mProductionRateH2O < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "H2O production rate < 0.0.");
    }

    /// - Throw an exception on negative heat production rate.
    if (configData.mProductionRateHeat < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Heat production rate < 0.0.");
    }

    /// - Throw an exception on sleep scale factor negative or greater then nominal.
    if (!MsMath::isInRange(0.0, configData.mSleepScaleFactor, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Sleep scale factor < 0 or > 1.0.");
    }

    /// - Throw an exception on recovery scale factor less than nominal.
    if (configData.mRecoveryScaleFactor < 1.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Recovery scale factor < 1.0.");
    }

    /// - Throw an exception on exercise scale factor less than recovery.
    if (configData.mExerciseScaleFactor < configData.mRecoveryScaleFactor) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Exercise scale factor less than recovery scale factor.");
    }

    /// - Throw an exception on negative number of crew members in nominal state.
    if (inputData.mNNominal < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in nominal state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in sleep state.
    if (inputData.mNSleep < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in sleep state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in recovery state.
    if (inputData.mNRecovery < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in recovery state < 0.0.");
    }

    /// - Throw an exception on negative number of crew members in exercise state.
    if (inputData.mNExercise < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Number of crew members in exercise state < 0.0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic::restartModel()
{
    /// - Reset the base class.
    GunnsFluidSource::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mConsumedO2   = 0.0;
    mProducedCO2  = 0.0;
    mProducedH2O  = 0.0;
    mProducedHeat = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
///
/// @return   void
///
/// @details  Initializes the derived attributes of this GUNNS Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic::derive(const GunnsFluidMetabolicConfigData& configData,
                                 const GunnsFluidMetabolicInputData&  inputData)
{
    mNCrew[GunnsFluidMetabolic::NOMINAL]               = inputData.mNNominal;
    mNCrew[GunnsFluidMetabolic::SLEEP]                 = inputData.mNSleep;
    mNCrew[GunnsFluidMetabolic::RECOVERY]              = inputData.mNRecovery;
    mNCrew[GunnsFluidMetabolic::EXERCISE]              = inputData.mNExercise;

    mO2ConsumptionRate[GunnsFluidMetabolic::NOMINAL]   = configData.mConsumptionRateO2;
    mO2ConsumptionRate[GunnsFluidMetabolic::SLEEP]     = configData.mConsumptionRateO2 *
                                                    configData.mSleepScaleFactor;
    mO2ConsumptionRate[GunnsFluidMetabolic::RECOVERY]  = configData.mConsumptionRateO2 *
                                                    configData.mRecoveryScaleFactor;
    mO2ConsumptionRate[GunnsFluidMetabolic::EXERCISE]  = configData.mConsumptionRateO2 *
                                                    configData.mExerciseScaleFactor;

    mCO2ProductionRate[GunnsFluidMetabolic::NOMINAL]   = configData.mProductionRateCO2;
    mCO2ProductionRate[GunnsFluidMetabolic::SLEEP]     = configData.mProductionRateCO2 *
                                                    configData.mSleepScaleFactor;
    mCO2ProductionRate[GunnsFluidMetabolic::RECOVERY]  = configData.mProductionRateCO2 *
                                                    configData.mRecoveryScaleFactor;
    mCO2ProductionRate[GunnsFluidMetabolic::EXERCISE]  = configData.mProductionRateCO2 *
                                                    configData.mExerciseScaleFactor;

    mH2OProductionRate[GunnsFluidMetabolic::NOMINAL]   = configData.mProductionRateH2O;
    mH2OProductionRate[GunnsFluidMetabolic::SLEEP]     = configData.mProductionRateH2O *
                                                    configData.mSleepScaleFactor;
    mH2OProductionRate[GunnsFluidMetabolic::RECOVERY]  = configData.mProductionRateH2O *
                                                    configData.mRecoveryScaleFactor;
    mH2OProductionRate[GunnsFluidMetabolic::EXERCISE]  = configData.mProductionRateH2O *
                                                    configData.mExerciseScaleFactor;

    mHeatProductionRate[GunnsFluidMetabolic::NOMINAL]  = configData.mProductionRateHeat;
    mHeatProductionRate[GunnsFluidMetabolic::SLEEP]    = configData.mProductionRateHeat *
                                                    configData.mSleepScaleFactor;
    mHeatProductionRate[GunnsFluidMetabolic::RECOVERY] = configData.mProductionRateHeat *
                                                    configData.mRecoveryScaleFactor;
    mHeatProductionRate[GunnsFluidMetabolic::EXERCISE] = configData.mProductionRateHeat *
                                                    configData.mExerciseScaleFactor;

    mConsumedO2   = 0.0;
    mProducedCO2  = 0.0;
    mProducedH2O  = 0.0;
    mProducedHeat = 0.0;
    mO2           = mNodes[1]->getContent()->find(FluidProperties::GUNNS_O2);
    mCO2          = mNodes[1]->getContent()->find(FluidProperties::GUNNS_CO2);
    mH2O          = mNodes[1]->getContent()->find(FluidProperties::GUNNS_H2O);

    /// - Create the internal fluid.
    createInternalFluid();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt    (s)    Time step.
///
/// @return   void
///
/// @details  Updates the flow demand for this GUNNS Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic::updateState(const double dt)
{
    if (dt < DBL_EPSILON) {
        /// - Zero out the flow demand if time step is negligible.
       mFlowDemand = 0.0;
    } else {

        /// - Compute produced/consumed metabolic rates.
        mConsumedO2    = 0.0;
        mProducedCO2   = 0.0;
        mProducedH2O   = 0.0;
        mProducedHeat  = 0.0;
        for (int i = 0; i < GunnsFluidMetabolic::NO_METABOLIC; ++i) {
            /// - O2 consumption rate is limited by the O2 mass in the node.
            mConsumedO2   += fmin((mNodes[1]->getContent()->getMassFraction(mO2) *
                                   mNodes[1]->getContent()->getMass()) / dt,
                                  mNCrew[i] * mO2ConsumptionRate[i]);
            mProducedCO2  += mNCrew[i] * mCO2ProductionRate[i];
            mProducedH2O  += mNCrew[i] * mH2OProductionRate[i];
            mProducedHeat += mNCrew[i] * mHeatProductionRate[i];
        }
        const double flowDemand = mProducedH2O + mProducedCO2 - mConsumedO2;

        if (fabs(flowDemand) <= DBL_EPSILON) {
            /// - Zero out the flow demand if computed consumption rate is negligible.
            mFlowDemand = 0.0;
        } else {
            /// - Set the flow demand to the computed consumption rate.
            mFlowDemand = flowDemand;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt         (s)    Time step.
/// @param[in]    flowrate   (kg/s) Mass flow rate (not used).
///
/// @return   void
///
/// @details  Updates the atmosphere for the consumed O2 and produced CO2, H20 and heat based on
///           the number of crew members in each metabolic state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic::updateFluid(const double dt       __attribute__((unused)),
                                      const double flowrate __attribute__((unused)))
{
    /// - Skip jf flow demand is negligible.
    if (mFlowDemand > DBL_EPSILON) {

        /// - Reset the internal fluid state.
        mInternalFluid->resetState();

        /// - Update output atmosphere constituents using mass rate instead of mass since it is
        ///   only the mass fractions that are of interest.
        mInternalFluid->setMass(mO2,  -mConsumedO2);
        mInternalFluid->setMass(mCO2,  mProducedCO2);
        mInternalFluid->setMass(mH2O,  mProducedH2O);

        /// - Update output atmosphere mass, moles & fractions from constituent mass.
        mInternalFluid->updateMass();

        /// - Update output atmosphere temperature.
        mNodes[1]->collectHeatFlux(mProducedHeat);
        mInternalFluid->setTemperature(mNodes[1]->getContent()->getTemperature());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  number     (--)  Number of crew members (> 0) to transition.
/// @param[in]  fromState  (--)  Initial metabolic state.
/// @param[in]  toState    (--)  Final metabolic state.
///
/// @return   void
///
/// @details  Transitions the specified crew members from one metabolic state to another, subject
///           to the constraint that the number of crew members in any state must be non-negative.
///           NO_METABOLIC in the fromState or toState arguments can be used to increase or decrease
///           the total # of crew.  If NO_METABOLIC is given as the fromState, then the total # of
///           crew is increased by number, all in the toState.  Likewise, if NO_METABOLIC is given
///           as the toState, then the total # of crew are decreased by number, all from the
///           fromState.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidMetabolic::transition(const double        number,
                                     const MetabolicType fromState,
                                     const MetabolicType toState)
{
    if (number < 0) {
        /// - Do nothing on negative number of crew members.
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " rejecting request to transition a negative number of crew members.";
        hsSendMsg(msg);
    } else {
        /// - Transition as many as requested or are available.
        double n = number;
        if (NO_METABOLIC != fromState) {
            n = fmin(number, mNCrew[fromState]);
            mNCrew[fromState] = fmax(0.0, mNCrew[fromState] - n);
        }
        if (NO_METABOLIC != toState) {
            mNCrew[toState]   = fmax(0.0, mNCrew[toState]   + n);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    port  --  The port to be assigned.
/// @param[in]    node  --  The desired node to assign the port to.
///
/// @returns  -- True if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           a Gunns Fluid Metabolic link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidMetabolic::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 1 is the vacuum boundary node.
    if ((1 == port) && (node == mNodeList->mNumNodes - 1)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: cannot assign port 1 to the boundary node.";
        hsSendMsg(msg);
        result = false;
    }

    /// - Fail if port 0 is not the vacuum boundary node.
    else if ((0 == port) && (node != mNodeList->mNumNodes - 1)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: must assign port 0 to the boundary node.";
        hsSendMsg(msg);
        result = false;
    }

    return result;
}
