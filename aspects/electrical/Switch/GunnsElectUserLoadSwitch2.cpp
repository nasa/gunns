/**
@file     GunnsElectUserLoadSwitch2.cpp
@brief    GUNNS Electrical User Load Switch Link Variant 2 implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  (
     (core/GunnsBasicConductor.o)
     (GunnsElectSwitchUtil2.o)
     (aspects/electrical/UserLoad/GunnsElectUserLoad.o)
     (aspects/electrical/UserLoad/UserLoadBase.o)
     (aspects/electrical/resistive/GunnsShortUtil.o)
     (software/exceptions/TsInitializationException.o)
     (common/sensors/TsNoise.o)
  )
*/

#include "GunnsElectUserLoadSwitch2.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "aspects/electrical/UserLoad/GunnsElectUserLoad.hh"
#include "common/sensors/TsNoise.hh"

/// @details  This value is a trade between high ideal potential effect conductance and low rounding
///           error in the network solution.
const double GunnsElectUserLoadSwitch2::mLoadsOverrideG = 1.0E8;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                         (--)   Reference to the link name.
/// @param[in]  nodes                        (--)   Pointer to the network node list.
/// @param[in]  switchResistance             (ohm)  Default resistance of the switch.
/// @param[in]  switchTripPriority           (--)   The switch's trip priority.
/// @param[in]  curentSensorMinRange         (amp)  The through-current sensor minimum range.
/// @param[in]  curentSensorMaxRange         (amp)  The through-current sensor maximum range.
/// @param[in]  inputVoltageSensorMinRange   (V)    The input voltage sensor minimum range.
/// @param[in]  inputVoltageSensorMaxRange   (V)    The input voltage sensor maximum range.
/// @param[in]  outputVoltageSensorMinRange  (V)    The output voltage sensor minimum range.
/// @param[in]  outputVoltageSensorMaxRange  (V)    The output voltage sensor maximum range.
///
/// @details  Constructs the Electrical User Load Switch Config data.  The base conductor is
///           configured with zero default conductance, since during run-time its effective
///           conductance is determined by the switch and attached loads.  Several of the switch
///           values are forced to work as used by this link.  The sensors are configured with
///           mostly default values, and only arguments are provided for the min/max range values
///           since those are the mostly commonly used, to reduce the arguments to this function.
///           The user can change the other values via the input file when needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitch2ConfigData::GunnsElectUserLoadSwitch2ConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       switchResistance,
        const int          switchTripPriority,
        const float        curentSensorMinRange,
        const float        curentSensorMaxRange,
        const float        inputVoltageSensorMinRange,
        const float        inputVoltageSensorMaxRange,
        const float        outputVoltageSensorMinRange,
        const float        outputVoltageSensorMaxRange)
    :
    GunnsBasicConductorConfigData(name, nodes, 0.0),
    mSwitch(switchResistance, switchTripPriority),
    mCurrentSensor(curentSensorMinRange, curentSensorMaxRange),
    mInputVoltageSensor(inputVoltageSensorMinRange, inputVoltageSensorMaxRange),
    mOutputVoltageSensor(outputVoltageSensorMinRange, outputVoltageSensorMaxRange)
{
    mCurrentSensor.mNoiseFunction       = TsNoise::getNoiseFunction();
    mInputVoltageSensor.mNoiseFunction  = TsNoise::getNoiseFunction();
    mOutputVoltageSensor.mNoiseFunction = TsNoise::getNoiseFunction();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical User Load Switch Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitch2ConfigData::~GunnsElectUserLoadSwitch2ConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag           (--)  Blockage malfunction flag.
/// @param[in] malfBlockageValue          (--)  Blockage malfunction fractional value (0-1).
/// @param[in] switchIsClosed             (--)  True if the switch is initially closed.
/// @param[in] inputUnderVoltageTripLimit (V)   Initial input under-voltage trip limit.
/// @param[in] inputUnderVoltageTripReset (V)   Initial input under-voltage trip reset.
/// @param[in] inputOverVoltageTripLimit  (V)   Initial input under-voltage trip limit.
/// @param[in] inputOverVoltageTripReset  (V)   Initial input under-voltage trip reset.
/// @param[in] switchPosTripLimit         (amp) Initial positive over-current trip limit.
/// @param[in] switchNegTripLimit         (amp) Initial negative over-current trip limit.
/// @param[in] loadsOverrideActive        (--)  Initial state of the loads voltage override mode.
/// @param[in] loadsOverrideVoltage       (--)  Initial loads voltage override voltage.
///
/// @details  Default constructs this Electrical User Load Switch input data.  No arguments are
///           provided to the electrical short input data, but it can be set via the input file.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitch2InputData::GunnsElectUserLoadSwitch2InputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const bool   switchIsClosed,
        const double inputUnderVoltageTripLimit,
        const double inputUnderVoltageTripReset,
        const double inputOverVoltageTripLimit,
        const double inputOverVoltageTripReset,
        const double switchPosTripLimit,
        const double switchNegTripLimit,
        const bool   loadsOverrideActive,
        const double loadsOverrideVoltage)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
    mSwitch(switchIsClosed, switchIsClosed, false, inputUnderVoltageTripLimit, inputUnderVoltageTripReset, inputOverVoltageTripLimit, inputOverVoltageTripReset, switchPosTripLimit, switchNegTripLimit),
    mShort(),
    mLoadsOverrideActive(loadsOverrideActive),
    mLoadsOverrideVoltage(loadsOverrideVoltage)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Electrical User Load Switch input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitch2InputData::~GunnsElectUserLoadSwitch2InputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Electrical User Load Switch Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitch2::GunnsElectUserLoadSwitch2():
    GunnsBasicConductor(),
    mSwitch(),
    mCurrentSensor(),
    mInputVoltageSensor(),
    mOutputVoltageSensor(),
    mShort(),
    mLoadsVoltage(0.0),
    mLoadsPower(0.0),
    mLoadsOverrideActive(false),
    mLoadsOverrideVoltage(0.0),
    mUserLoads()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical User Load Switch Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitch2::~GunnsElectUserLoadSwitch2()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data.
/// @param[in]     inputData    (--) Reference to Link Input Data.
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector.
/// @param[in]     port0        (--) Port 0 Mapping.
/// @param[in]     port1        (--) Port 1 Mapping.
///
/// @throws   TsInitializationException
///
/// @details  Initializes the Electrical User Load Switch with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::initialize(const GunnsElectUserLoadSwitch2ConfigData& configData,
                                          const GunnsElectUserLoadSwitch2InputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize config & input data.
    mSwitch             .initialize(configData.mSwitch,              inputData.mSwitch,              configData.mName + ".mSwitch");
    mCurrentSensor      .initialize(configData.mCurrentSensor,       inputData.mCurrentSensor,       configData.mName + ".mCurrentSensor");
    mInputVoltageSensor .initialize(configData.mInputVoltageSensor,  inputData.mInputVoltageSensor,  configData.mName + ".mInputVoltageSensor");
    mOutputVoltageSensor.initialize(configData.mOutputVoltageSensor, inputData.mOutputVoltageSensor, configData.mName + ".mOutputVoltageSensor");
    mShort              .initialize(inputData.mShort);
    mLoadsOverrideActive  = inputData.mLoadsOverrideActive;
    mLoadsOverrideVoltage = inputData.mLoadsOverrideVoltage;

    /// - Initialize the user loads.
    for (unsigned int i=0; i<mUserLoads.size(); ++i) {
        mUserLoads[i]->initLoad();
    }

    /// - Initialize class attributes.
    mLoadsVoltage = 0.0;
    mLoadsPower   = 0.0;

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mLoadsVoltage = 0.0;
    mLoadsPower   = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  This updates all of the user load objects' duty cycle function, then calls the base
///           class step function to complete the major step updates.
///
/// @notes  To ensure that the user load's duty cycle only gets called once per network major step,
///         this function should only be called on the first network major step, and subsequent
///         minor steps should skip this and call GunnsBasicConductor::step directly instead.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::step(const double dt)
{
    for (unsigned int i=0; i<mUserLoads.size(); ++i) {
        mUserLoads[i]->stepDutyCycle(dt);
    }
    GunnsBasicConductor::step(dt);

    /// - Update the sensors with dt for integrating their drift malfunction, using the last-pass
    ///   truth values for now.  They will sense the current step solution in the
    ///   confirmSolutionAcceptable method.
    mCurrentSensor      .sense(dt, true, mFlux);
    mInputVoltageSensor .sense(dt, true, mPotentialVector[0]);
    mOutputVoltageSensor.sense(dt, true, mLoadsVoltage);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step, not used.
///
/// @throws   TsInitializationException
///
/// @details  Updates the combined effective conductance of the switch and all user loads for
///           inclusion in the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::updateState(const double dt __attribute__((unused)))
{
    /// - When the loads voltage override is active, we force the switch open to disconnect it from
    ///   the upstream network.
    if (mLoadsOverrideActive) {
        mSwitch.setPositionCommand(false);
    }

    /// - Update the switch state.
    mSwitch.updateState();

    /// - Compute the switch output & loads input voltage as the switch input volts minus the last-
    ///   pass switch current times the new switch resistance.
    if (mLoadsOverrideActive) {
        mLoadsVoltage = mLoadsOverrideVoltage;
    } else if (mUserLoads.size() > 0 and mSwitch.getPosition() and mFlux >= 0.0) {
        mLoadsVoltage = std::max(0.0, mPotentialVector[0] - mFlux * mSwitch.getResistance());
    } else {
        mLoadsVoltage = 0.0;
    }

    /// - Loop over the user loads and add their conductances in parallel.
    double loadsConductance = m100EpsilonLimit;
    for (unsigned int i=0; i<mUserLoads.size(); ++i) {
        mUserLoads[i]->setSupplyVoltage(mLoadsVoltage);
        mUserLoads[i]->step();
        loadsConductance += 1.0 / std::max(DBL_EPSILON, mUserLoads[i]->getLoad()->getResistance());
    }

    /// - Add the short conductance in parallel with the loads between the switch output and Ground.
    ///   The short is only applied through a closed switch and when the load override is inactive.
    if (mSwitch.getPosition() and not mLoadsOverrideActive) {
        loadsConductance += mShort.computeShort(mLoadsVoltage);
    }

    /// - Combine the total loads conductance in series with the switch conductance.  Switch can't
    ///   return zero conductance when it is closed, and loads conductance is limited to > 0 above.
    if (mSwitch.getPosition()) {
        double totalResistance = mSwitch.getResistance();
        if (mUserLoads.size() > 0) {
            totalResistance += 1.0 / loadsConductance;
        } else if (mNodeMap[1] == getGroundNodeIndex()) {
            /// - We assume the switch output is not grounded, so that if there are no loads and the
            ///   switch is closed, we only apply its conductance to a normal node, not the Ground
            ///   node.  This prevents the switch over-current tripping due to high flux thru just
            ///   its own low resistance to Ground.
            totalResistance = mConductanceLimit;
        }
        mEffectiveConductivity = 1.0 / totalResistance;
    } else {
        mEffectiveConductivity = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  When the loads override mode is active, the normal switch conductance between ports is
///           omitted, and instead the ideal potential source conductance is added to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::buildConductance()
{
    if (mLoadsOverrideActive) {
        if (mAdmittanceMatrix[0] != 0.0 or mAdmittanceMatrix[3] != mLoadsOverrideG) {
            mAdmittanceMatrix[0] = 0.0;
            mAdmittanceMatrix[1] = 0.0;
            mAdmittanceMatrix[2] = 0.0;
            mAdmittanceMatrix[3] = mLoadsOverrideG;
            mAdmittanceUpdate = true;
        }
    } else {
        GunnsBasicConductor::buildConductance();
    }
    buildSource();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  When the loads override mode is active, the ideal potential source is added to port 1,
///           otherwise the whole source vector is zeroed like a normal conductor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::buildSource()
{
    mSourceVector[0] = 0.0;
    if (mLoadsOverrideActive) {
        mSourceVector[1] = mLoadsOverrideVoltage * mAdmittanceMatrix[3];
    } else {
        mSourceVector[1] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  convergedStep  (--)  The # of minor steps since the network last converged.
/// @param[in]  absoluteStep   (--)  The network absolute minor step number, not used.
///
/// @returns  SolutionResult  (--)  Returns whether the link confirms, rejects, or delays the
///                                 solution.
///
/// @details  Updates the switch trip logic.  Delays the network solution until the switch's turn to
///           trip, then either confirms (if no trip) or rejects (if just tripped) the solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectUserLoadSwitch2::confirmSolutionAcceptable(
        const int convergedStep,
        const int absoluteStep __attribute__((unused)))
{
   GunnsBasicLink::SolutionResult result = DELAY;

   if (convergedStep > 0) {

       /// - Check the user loads for their fuses blowing.  Always check fuses on trip priority 1
       ///   and before the switch is updated, so the fuses (downstream) always blow before the
       ///   switch (upstream) trips.
       bool fuseTrips = false;
       if (1 == convergedStep) {
           for (unsigned int i=0; i<mUserLoads.size(); ++i) {
               fuseTrips = fuseTrips or mUserLoads[i]->getLoad()->updateFuse(mPotentialVector[0]);
           }
       }
       if (fuseTrips) {
           result = GunnsBasicLink::REJECT;
       } else {
           mPotentialDrop = getDeltaPotential();
           computeFlux();

           /// - Update sensors with current & voltage from the network solution.  Note since we
           ///   don't have timestep in this function we pass zero timestep to the sensors here, so
           ///   we have to update them again in the step function with a timestep for their drift
           ///   malfunction.
           mCurrentSensor      .sense(0.0, true, mFlux);
           mInputVoltageSensor .sense(0.0, true, mPotentialVector[0]);
           mOutputVoltageSensor.sense(0.0, true, mLoadsVoltage);

           mSwitch.updateTrips(mCurrentSensor.getSensedOutput(),
                               mInputVoltageSensor.getSensedOutput(), convergedStep);
           if (mSwitch.isWaitingToTrip() ) {
               result = GunnsBasicLink::DELAY;
           } else if (mSwitch.hasJustTripped()) {
               result = GunnsBasicLink::REJECT;
           } else {
               result = GunnsBasicLink::CONFIRM;
           }
       }
   }

   return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  load  (--)  Pointer to the user load object to add.
///
/// @throws   TsInitializationException
///
/// @details  Adds the given user load pointer to the internal vector.  We only allow this prior to
///           link initialization -- otherwise user loads could get connected to a non-ground node.
///           Attempting to add a load after initialization causes an exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::addUserLoad(GunnsElectUserLoad* load)
{
    if (mInitFlag) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "User load can't be added after link is initialized.");
    } else {
        mUserLoads.push_back(load);
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
///           - A GunnsElectUserLoadSwitch2 with user loads must map port 1 to the network's Ground
///             node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectUserLoadSwitch2::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 1 is not the Ground node when there are user loads.
    if ((1 == port) && (node != getGroundNodeIndex()) && (mUserLoads.size() > 0)) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 1 to a non-Ground node when there are user loads.");
        result = false;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  When the loads voltage override mode is not active, flux is calculated as a normal
///           conductor.
///
///           When the loads voltage override mode is active:
///           - mPotentialDrop reflects the potential source effect from Ground to Port 1.
///           - If Port 1 is not connected to the Ground node, flux is calculated for the potential
///             source from Ground to Port 1.
///           - If Port 1 is connected to the Ground node, flux is not calculated here; instead it
///             is calculated in computePower() from the user load power.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::computeFlux()
{
    if (mLoadsOverrideActive) {
        mPotentialDrop = -mPotentialVector[1];
        if (mNodeMap[1] != getGroundNodeIndex()) {
            mFlux = mPotentialDrop * mAdmittanceMatrix[3] + mSourceVector[1];
        }
    } else {
        GunnsBasicConductor::computeFlux();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes mPower as the total power consumed thru the link including the switch and
///           user loads, and mLoadsPower as the total power consumed by just the user loads.
///           In the voltage override mode, the mLoadsPower is the sum of the user loads and the
///           power used by the downstream circuit, if any.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch2::computePower()
{
    if (mLoadsOverrideActive) {
        if (mNodeMap[1] == getGroundNodeIndex()) {
            mLoadsPower = 0.0;
            for (unsigned int i=0; i<mUserLoads.size(); ++i) {
                mLoadsPower += mUserLoads[i]->getLoad()->getPower();
            }
            mPower = mLoadsPower;
            mFlux  = mPower / std::max(mLoadsOverrideVoltage, DBL_EPSILON);
        } else {
            mPower      = mFlux * mPotentialVector[1];
            mLoadsPower = mPower;
        }
    } else {
        GunnsBasicLink::computePower();
        mLoadsPower = -mPower - mFlux * mFlux * mSwitch.getResistance();
    }
}
