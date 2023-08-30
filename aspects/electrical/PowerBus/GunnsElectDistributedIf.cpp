/**
@file      GunnsElectDistributedIf.cpp
@brief     GUNNS Electrical Distributed Bi-Directional Interface Link implementation.

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((core/GunnsBasicLink.o)
     (software/exceptions/TsInitializationException.o)
     (software/exceptions/TsOutOfBoundsException.o))
*/

#include "GunnsElectDistributedIf.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name              (--)    Link name.
/// @param[in] nodes             (--)    Network nodes array.
/// @param[in] isPairPrimary     (--)    This is the primary side of the interface, defaults to Supply role.
/// @param[in] conductance       (1/ohm) Output channel conductance of the internal voltage source.
/// @param[in] netCapDvThreshold (V)     Network capacitance delta-voltage threshold.
///
/// @details  Default Distributed Bi-Directional Interface link configuration data constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributedIfConfigData::GunnsElectDistributedIfConfigData(const std::string& name,
                                                                     GunnsNodeList*     nodes,
                                                                     const bool         isPairPrimary,
                                                                     const double       conductance,
                                                                     const double       netCapDvThreshold)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mIsPairPrimary(isPairPrimary),
    mConductance(conductance),
    mNetCapDvThreshold(netCapDvThreshold),
    mSupplies()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Distributed Bi-Directional Interface link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributedIfConfigData::GunnsElectDistributedIfConfigData(
        const GunnsElectDistributedIfConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mIsPairPrimary(that.mIsPairPrimary),
    mConductance(that.mConductance),
    mNetCapDvThreshold(that.mNetCapDvThreshold),
    mSupplies(that.mSupplies)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Distributed Bi-Directional Interface link config data destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributedIfConfigData::~GunnsElectDistributedIfConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] forceDemandMode  (--)  Forces the interface to always be in Demand mode.
/// @param[in] forceSupplyMode  (--)  Forces the interface to always be in Supply mode.
///
/// @details  Default Distributed Bi-Directional Interface link input data constructor.  This link
///           does not implement the base class blockage malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributedIfInputData::GunnsElectDistributedIfInputData(const bool forceDemandMode,
                                                                   const bool forceSupplyMode)
    :
    GunnsBasicLinkInputData(false, 0.0),
    mForceDemandMode(forceDemandMode),
    mForceSupplyMode(forceSupplyMode)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy.
///
/// @details  Copy constructs this Distributed Bi-Directional Interface link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributedIfInputData::GunnsElectDistributedIfInputData(const GunnsElectDistributedIfInputData& that)
    :
    GunnsBasicLinkInputData(that),
    mForceDemandMode(that.mForceDemandMode),
    mForceSupplyMode(that.mForceSupplyMode)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Distributed Bi-Directional Interface link Input Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributedIfInputData::~GunnsElectDistributedIfInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default Distributed Bi-Directional Interface link constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributedIf::GunnsElectDistributedIf()
    :
    GunnsBasicLink(NPORTS),
    mInterface(),
    mPowerLoad(),
    mVoltageSource(),
    mMalfPowerLoad(false),
    mMalfVoltageSource(false),
    mNetCapDvThreshold(0.0),
    mSupplies(),
    mNumSupplies(0),
    mSupplyMonitorIndex(0),
    mSupplyMonitor(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Distributed Bi-Directional Interface link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectDistributedIf::~GunnsElectDistributedIf()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Distributed Bi-Directional Interface link config data.
/// @param[in]     inputData    (--) Distributed Bi-Directional Interface link input data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Network port 0.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Distributed Bi-Directional Interface link with configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributedIf::initialize(const GunnsElectDistributedIfConfigData& configData,
                                         const GunnsElectDistributedIfInputData&  inputData,
                                         std::vector<GunnsBasicLink*>&            networkLinks,
                                         const int                                port0)
{
    /// - Initialize the parent class.
    int ports[1] = {port0};
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize the interface utility.
    mNetCapDvThreshold = configData.mNetCapDvThreshold;
    mNumSupplies       = configData.mSupplies.size();
    mSupplies.clear();
    for (unsigned int i=0; i<mNumSupplies; ++i) {
        GunnsElectDistributedIfSupplyData supply;
        supply.mSupplyData = mInterface.createSupplyData();
        supply.mLink       = configData.mSupplies.at(i);
        supply.mNetCapDV   = 0.0;
        mSupplies.push_back(supply);
    }
    mInterface.initialize(configData.mIsPairPrimary, mNodes[0]->getPotential());
    if (inputData.mForceSupplyMode) {
        mInterface.forceSupplyRole();
    } else if (inputData.mForceDemandMode) {
        mInterface.forceDemandRole();
    }
    processIfNotifications(true);
    updateSupplyMonitor();

    /// - Initialize the local voltage source and constant power load objects.
    GunnsElectConverterInputConfigData cplConfig(configData.mName + ".mPowerLoad", configData.mNodeList);
    GunnsElectConverterInputInputData  cplInput;
    mPowerLoad.initialize(cplConfig, cplInput, networkLinks, port0);

    GunnsElectConverterOutputConfigData vsConfig(configData.mName + ".mVoltageSource", configData.mNodeList,
                                                 GunnsElectConverterOutput::TRANSFORMER,
                                                 configData.mConductance, 1.0);
    GunnsElectConverterOutputInputData  vsInput(false, 0.0, false, 0.0, 0.0, 1.0);
    mVoltageSource.initialize(vsConfig, vsInput, networkLinks, port0);

    /// - Set init flag on successful initialization.
    validate();
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Throws exceptions if configuration and input data are outside of valid range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributedIf::validate() const
{
    if (mNetCapDvThreshold < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "mNetCapDvTreshold < DBL_EPSILON.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributedIf::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-checkpointed and non-configuration data attributes.
    mPowerLoad.restart();
    mVoltageSource.restart();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used).
///
/// @details  Calls the interface utility to update its frame counters, then performs a minor step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributedIf::step(const double dt)
{
    updateSupplyMonitor();

    mInterface.processInputs();
    updateInterface();

    /// - The voltage supplied to this side from the remote side only needs to be updated once, here
    ///   at the beginning of the major step, since it only changes from the remote side between our
    ///   steps.
    mVoltageSource.setInputVoltage(mInterface.getRemoteSupply());

    minorStep(dt, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step (not used).
///
/// @details  Updates the child link enable flags and input power demand based on interface mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributedIf::minorStep(const double dt        __attribute__((unused)),
                                        const int    minorStep __attribute__((unused)))
{
    if (mInterface.isInDemandRole()) {
        mVoltageSource.setEnabled(not mMalfVoltageSource);
        mPowerLoad.setEnabled(false);
        mPowerLoad.setInputPower(0.0);
        mPowerLoad.getInputUnderVoltageTrip()->setLimit(0.0);
    } else {
        mVoltageSource.setEnabled(false);
        mPowerLoad.setEnabled(not mMalfPowerLoad);
        mPowerLoad.setInputPower(mInterface.getRemoteLoad());
        /// - Set the power load's IUV trip limit so that it won't overload the input.  This helps
        ///   the network converge when the upstream supply can't meet the power demand.  Note that
        ///   the IUV trip isn't enabled because trip priority is always 0 - but the
        ///   GunnsElectConverterInput still uses the limit value to avoid overloading the input
        ///   even when the IUV trip isn't enabled.
        mPowerLoad.getInputUnderVoltageTrip()->setLimit(0.9 * mPotentialVector[0]);
    }

    /// - This link has no contributions to the network system of equations; the child converter
    ///   links do.
    mAdmittanceMatrix[0] = 0.0;
    mSourceVector[0]     = 0.0;
    mAdmittanceUpdate    = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  For each registered voltage supply, updates the interface utility with its controlled
///           voltage and whether it is available to supply power to this location.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributedIf::updateSupplyData()
{
    /// - Send each voltage supply's control voltage to the interface utility.  Determine if any
    ///   supplies are enabled and can output power at greater than zero volts.  If so, then set the
    ///   node's network capacitance request to determine if there is a conductive path from each
    ///   supply to here.
    bool anySuppliesEnabled = false;
    for (unsigned int i=0; i<mSupplies.size(); ++i) {
        mSupplies.at(i).mSupplyData->mMaximumVoltage = mSupplies.at(i).mLink->getControlVoltage();
        if (mSupplies.at(i).mLink->getEnabled() and mSupplies.at(i).mSupplyData->mMaximumVoltage > FLT_EPSILON) {
            anySuppliesEnabled = true;
        }
    }
    if (anySuppliesEnabled) {
        mNodes[0]->setNetworkCapacitanceRequest();
    }

    /// - Set each voltage supply's availability flag to the interface utility if it is enabled and
    ///   has a conductive path to this location.
    for (unsigned int i=0; i<mSupplies.size(); ++i) {
        const double dV = mNodes[0]->getNetCapDeltaPotential()[mSupplies[i].mLink->getNodeMap()[0]];
        mSupplies.at(i).mNetCapDV = dV;
        mSupplies.at(i).mSupplyData->mAvailable = mSupplies.at(i).mLink->getEnabled() and (dV > mNetCapDvThreshold);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Confirms or rejects the network solution after each minor step.  We update the
///           interface utility to check if it switches from Supply to Demand roles, which may
///           occur if local voltage supplies have dropped out in the most recent solution.  If it
///           has switched to Demand mode, then we reject the solution and force the network to
///           re-solve with our link in Demand mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult GunnsElectDistributedIf::confirmSolutionAcceptable(
        const int convergedStep,
        const int absoluteStep __attribute__((unused)))
{
    GunnsBasicLink::SolutionResult result = CONFIRM;

    const bool previousDemandRole = updateInterface();
    if (mInterface.isInDemandRole() and not previousDemandRole) {
        result = REJECT;
        /// - In Demand mode, drive the voltage source child link to apply the remote voltage
        ///   supply to our node on the next minor step, and disable the power load link.
        mVoltageSource.setEnabled(not mMalfVoltageSource);
        mPowerLoad.setEnabled(false);
        mPowerLoad.setInputPower(0.0);
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step.
///
/// @details  Calls the child links to compute and transport their flows to the node.  This link has
///           no direct contributions to the system of equations and has no flows of its own.
///           Updates the interface utility with the final node voltage and power demand for this
///           major step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributedIf::computeFlows(const double dt)
{
    mPowerLoad.computeFlows(dt);
    mVoltageSource.computeFlows(dt);
    updateInterface();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if the interface was in Demand role prior to this update.
///
/// @details  Updates the local voltage supply data, updates the interface with the latest local
///           node voltage and power demand, processes notifications from the interface, and returns
///           whether the interface was in Demand role prior to this update.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectDistributedIf::updateInterface()
{
    const bool previousRole = mInterface.isInDemandRole();
    updateSupplyData();
    if (mVoltageSource.getInputPowerValid()) {
        mInterface.update(mPotentialVector[0], mVoltageSource.getInputPower());
    } else {
        mInterface.update(mPotentialVector[0], 0.0);
    }
    processIfNotifications(false);
    return previousRole;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsElectDistributedIf cannot map port 0 to the network's Ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsElectDistributedIf::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is the Ground node.
    if ((0 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to the Ground node.");
        result = false;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] isInit (--) If this should throw initialization errors, currently not used.
///
/// @details  Pops all notifications from the interface utility's queue and translates them to H&S
///           messages.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectDistributedIf::processIfNotifications(const bool isInit __attribute__((unused)))
{
    GunnsDistributed2WayBusNotification notification;
    unsigned int numNotifs = 0;
    do {
        numNotifs = mInterface.popNotification(notification);
        if (GunnsDistributed2WayBusNotification::NONE != notification.mLevel) {
            std::ostringstream msg;
            msg << "from mInterface: " << notification.mMessage;
            switch (notification.mLevel) {
                case GunnsDistributed2WayBusNotification::INFO:
                    GUNNS_INFO(msg.str());
                    break;
// The interface currently has no WARN or ERR outputs, so these are untestable:
//                case GunnsDistributed2WayBusNotification::WARN:
//                    GUNNS_WARNING(msg.str());
//                    break;
//                case GunnsDistributed2WayBusNotification::ERR:
//                    if (isInit) {
//                        GUNNS_ERROR(TsInitializationException, "Catch and re-throw", msg.str());
//                    } else {
//                        GUNNS_ERROR(TsOutOfBoundsException, "Catch and re-throw", msg.str());
//                    }
//                    break;
                default: // this won't happen, checked by the if statement
                    break;
            }
        }
    } while (numNotifs > 0);
}
