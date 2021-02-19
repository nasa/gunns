/**
@file
@brief    GUNNS Electrical User Load Switch Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  (
     (core/GunnsBasicConductor.o)
     (GunnsElectSwitchUtil.o)
     (aspects/electrical/UserLoad/GunnsElectUserLoad.o)
     (aspects/electrical/resistive/GunnsShortUtil.o)
     (software/exceptions/TsInitializationException.o)
  )
*/
#include "GunnsElectUserLoadSwitch.hh"
#include "math/Math.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "aspects/electrical/UserLoad/GunnsElectUserLoad.hh"

/// @details  This value is a trade between high ideal potential effect conductance and low rounding
///           error in the network solution.
const double GunnsElectUserLoadSwitch::mLoadsOverrideG = 1.0E8;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name                (--)   Reference to the link name.
/// @param[in]  nodes               (--)   Pointer to the network node list.
/// @param[in]  switchResistance    (ohm)  Default resistance of the switch.
/// @param[in]  switchTripPriority  (--)   The switch's trip priority.
///
/// @details  Constructs the Electrical User Load Switch Config data.  The base conductor is
///           configured with zero default conductance, since during run-time its effective
///           conductance is determined by the switch and attached loads.  Several of the switch
///           values are forced to work as used by this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitchConfigData::GunnsElectUserLoadSwitchConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       switchResistance,
        const int          switchTripPriority)
    :
    GunnsBasicConductorConfigData(name, nodes, 0.0),
    mSwitch(switchResistance, (switchTripPriority > 0), false, 0.0, true, 1, switchTripPriority, false),
    mLoadsPowerRefV(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical User Load Switch Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitchConfigData::~GunnsElectUserLoadSwitchConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag      (--)   Blockage malfunction flag.
/// @param[in]  malfBlockageValue     (--)   Blockage malfunction fractional value (0-1).
/// @param[in]  switchIsClosed        (--)   True if the switch is initially closed.
/// @param[in]  switchPosTripLimit    (amp)  Switch's positive over-current trip limit.
/// @param[in]  switchNegTripLimit    (amp)  Switch's negative over-current trip limit.
/// @param[in]  loadsOverrideActive   (--)   Initial state of the loads voltage override mode.
/// @param[in]  loadsOverrideVoltage  (--)   Initial loads voltage override voltage.
///
/// @details  Default constructs this Electrical User Load Switch input data.  No arguments are
///           provided to the electrical short input data, but it can be set via the input file.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitchInputData::GunnsElectUserLoadSwitchInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const bool   switchIsClosed,
        const double switchPosTripLimit,
        const double switchNegTripLimit,
        const bool   loadsOverrideActive,
        const double loadsOverrideVoltage)
    :
    GunnsBasicConductorInputData(malfBlockageFlag, malfBlockageValue),
    mSwitch(false, 0, switchIsClosed, switchPosTripLimit, switchNegTripLimit),
    mShort(),
    mLoadsOverrideActive(loadsOverrideActive),
    mLoadsOverrideVoltage(loadsOverrideVoltage)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Electrical User Load Switch input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitchInputData::~GunnsElectUserLoadSwitchInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Electrical User Load Switch Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectUserLoadSwitch::GunnsElectUserLoadSwitch():
    GunnsBasicConductor(),
    mSwitch(),
    mShort(),
    mLoadsPowerRefV(0.0),
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
GunnsElectUserLoadSwitch::~GunnsElectUserLoadSwitch()
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
void GunnsElectUserLoadSwitch::initialize(const GunnsElectUserLoadSwitchConfigData& configData,
                                          const GunnsElectUserLoadSwitchInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - Initialize the parent class.
    GunnsBasicConductor::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize class attributes.  The user loads must be initialized by their owner, not here.
    mLoadsVoltage = 0.0;

    /// - Initialize config & input data.
    mSwitch.initialize(configData.mSwitch, inputData.mSwitch, configData.mName + ".mSwitch", 0);
    mShort.initialize(inputData.mShort);
    mLoadsPowerRefV       = configData.mLoadsPowerRefV;
    mLoadsOverrideActive  = inputData.mLoadsOverrideActive;
    mLoadsOverrideVoltage = inputData.mLoadsOverrideVoltage;

    /// - Initialize the user loads.
    for (unsigned int i=0; i<mUserLoads.size(); ++i) {
        mUserLoads[i]->initLoad();
    }

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch::restartModel()
{
    /// - Reset the base class.
    GunnsBasicConductor::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mLoadsVoltage = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step, not used.
///
/// @throws   TsInitializationException
///
/// @details  Updates the combined effective conductance of the switch and all user loads for
///           inclusion in the network solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch::updateState(const double dt __attribute__((unused)))
{
    /// - When the loads voltage override is active, we force the switch open to disconnect it from
    ///   the upstream network.
    if (mLoadsOverrideActive) {
        mSwitch.setSwitchCommandedClosed(false);
    }

    /// - Update the switch state.
    mSwitch.updateSwitchState(true);

    /// - Compute the switch output & loads input voltage as the switch input volts minus the last-
    ///   pass switch current times the new switch resistance.  Switch can't return zero conductance
    ///   when it is closed.
    if (mLoadsOverrideActive) {
        mLoadsVoltage = mLoadsOverrideVoltage;
    } else if (mUserLoads.size() > 0 and mSwitch.isClosed() and mFlux >= 0.0) {
        mLoadsVoltage = std::max(0.0, mPotentialVector[0] - mFlux / mSwitch.getConductance());
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
    if (mSwitch.isClosed() and not mLoadsOverrideActive) {
        loadsConductance += mShort.computeShort(mLoadsVoltage);
    }

    /// - Combine the total loads conductance in series with the switch conductance.  Switch can't
    ///   return zero conductance when it is closed, and loads conductance is limited to > 0 above.
    if (mSwitch.isClosed()) {
        double totalResistance = 1.0 / mSwitch.getConductance();
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
void GunnsElectUserLoadSwitch::buildConductance()
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
void GunnsElectUserLoadSwitch::buildSource()
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
GunnsBasicLink::SolutionResult GunnsElectUserLoadSwitch::confirmSolutionAcceptable(
        const int convergedStep,
        const int absoluteStep __attribute__((unused)))
{
   GunnsBasicLink::SolutionResult result = DELAY;

   if (convergedStep > 0) {

      mPotentialDrop = getDeltaPotential();
      computeFlux();
      mSwitch.updateSwitchFlow(mFlux, mFlux, mPotentialVector[0], convergedStep, false);

      if (mSwitch.isWaitingToTrip() ) {
         result = GunnsBasicLink::DELAY;
      } else if (mSwitch.isJustTripped()) {
         result = GunnsBasicLink::REJECT;
      } else {
         result = GunnsBasicLink::CONFIRM;
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
void GunnsElectUserLoadSwitch::addUserLoad(GunnsElectUserLoad* load)
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
///           - A GunnsElectUserLoadSwitch with user loads must map port 1 to the network's Ground
///             node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectUserLoadSwitch::checkSpecificPortRules(const int port, const int node) const
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
void GunnsElectUserLoadSwitch::computeFlux()
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
///           The mLoadsPower is adjusted to an optional reference voltage if it is provided:
///             P = I * V, Pref = Iref * Vref, I = Iref
///             Pref = P*Vref/V
///
///           In the voltage override mode, the mLoadsPower is the sum of the user loads and the
///           power used by the downstream circuit, if any.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectUserLoadSwitch::computePower()
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
        mLoadsPower = -mPower - mSwitch.getPowerDissipation();
        if (0.0 != mLoadsPowerRefV and 0.0 < mLoadsVoltage) {
            mLoadsPower *= mLoadsPowerRefV / mLoadsVoltage;
        }
    }
}
