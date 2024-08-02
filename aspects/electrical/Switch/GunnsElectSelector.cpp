/**
@file
@brief    GUNNS Electrical Selector Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/GunnsBasicLink.o)
    )
*/

#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh"
#include "GunnsElectSelector.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                (--)     Link name.
/// @param[in] nodes               (--)     Network nodes array.
/// @param[in] defaultConductance  (1/ohm)  Default conductance of the selector connection.
/// @param[in] portLabels          (--)     Optional vector of port labels for runtime display.
///
/// @details  Constructs the Electrical Selector Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelectorConfigData::GunnsElectSelectorConfigData(
        const std::string&        name,
        GunnsNodeList*            nodes,
        const double              defaultConductance,
        std::vector<std::string>* portLabels)
    :
    GunnsBasicLinkConfigData(name, nodes),
    mDefaultConductance(defaultConductance),
    mPortLabels()
{
    mPortLabels.clear();
    if (0 != portLabels)
    {
        mPortLabels = *portLabels;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy Constructs the Electrical Selector Config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelectorConfigData::GunnsElectSelectorConfigData(
        const GunnsElectSelectorConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    mDefaultConductance(that.mDefaultConductance),
    mPortLabels(that.mPortLabels)
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical Selector Config Data Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelectorConfigData::~GunnsElectSelectorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag   (--)  Blockage malfunction flag.
/// @param[in] malfBlockageValue  (--)  Blockage malfunction fractional value (0-1).
///
/// @details  Default constructs this Electrical Selector input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelectorInputData::GunnsElectSelectorInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const int    selector)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    mSelector(selector)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy from.
///
/// @details  Copy constructs this Electrical Selector input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelectorInputData::GunnsElectSelectorInputData(
        const GunnsElectSelectorInputData& that)
    :
    GunnsBasicLinkInputData(that),
    mSelector(that.mSelector)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Electrical Selector input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelectorInputData::~GunnsElectSelectorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs the Electrical Selector Port Label Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelectorPortLabel::GunnsElectSelectorPortLabel()
    :
    mName("")
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs the Electrical Selector Port Label Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelectorPortLabel::~GunnsElectSelectorPortLabel()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Electrical Selector Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelector::GunnsElectSelector()
    :
    GunnsBasicLink(0),
    mDefaultConductance(0.0),
    mPortLabels(0),
    mSelector(0),
    mEffectiveConductance(0.0),
    mLastConnectedPort(0),
    mSystemAdmittance(0.0),
    mSystemSource(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Electrical Selector Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsElectSelector::~GunnsElectSelector()
{
    TS_DELETE_ARRAY(mPortLabels);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) Reference to Link Config Data.
/// @param[in]     inputData    (--) Reference to Link Input Data.
/// @param[in,out] networkLinks (--) Reference to the Network Link Vector.
/// @param[in]     portsVector  (--) Vector of node numbers the link ports connect to.
///
/// @throws   TsInitializationException
///
/// @details  Initializes the basic conductor with config and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSelector::initialize(const GunnsElectSelectorConfigData& configData,
                                    const GunnsElectSelectorInputData&  inputData,
                                    std::vector<GunnsBasicLink*>&       networkLinks,
                                    std::vector<int>*                   portsVector)
{
    mInitFlag = false;
    validate(configData, inputData, portsVector);

    /// - Initialize the base class with initial node map from ports vector.
    mNumPorts = portsVector->size();
    int ports[mNumPorts];
    for(int i=0; i<mNumPorts; ++i)
    {
        ports[i] = portsVector->at(i);
    }
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Allocate dynamic port labels array and initialize them.
    const int numLabels = configData.mPortLabels.size();
    TS_DELETE_ARRAY(mPortLabels);
    if (numLabels > 0) {
        TS_NEW_CLASS_ARRAY_EXT(mPortLabels, numLabels, GunnsElectSelectorPortLabel, (), mName+".mPortLabels");

        for (int i=0; i<numLabels; ++i) {
            mPortLabels[i].mName = configData.mPortLabels.at(i);
        }
    }

    /// - Initialize class attributes.
    mDefaultConductance   = configData.mDefaultConductance;
    mSelector             = inputData.mSelector;
    mLastConnectedPort    = -1;
    mSystemAdmittance     = 0.0;
    mSystemSource         = 0.0;

    /// - Set init flag on successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData   (--) Reference to Link Config Data.
/// @param[in]  inputData    (--) Reference to Link Input Data.
/// @param[in]  portsVector  (--) Vector of node numbers the link ports connect to.
///
/// @throws   TsInitializationException
///
/// @details  Validates the link initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSelector::validate(const GunnsElectSelectorConfigData& configData,
                                  const GunnsElectSelectorInputData&  inputData,
                                  std::vector<int>*                   portsVector) const
{
    /// - Throw exception on # ports < 2, as there must be at least one output port.
    if (portsVector->size() < 2) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Number of link ports < 2.");
    }

    /// - Throw an exception on default conductance being less than zero.
    if (configData.mDefaultConductance < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "default conductance < 0.");
    }

    /// - Throw an exception on initial selection not in valid range.
    if (!MsMath::isInRange(0, inputData.mSelector, portsVector->size()-1)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "initial port selection not in valid range.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSelector::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mEffectiveConductance = 0.0;
    mLastConnectedPort    = -1;
    mSystemAdmittance     = 0.0;
    mSystemSource         = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Not used.
///
/// @details  Updates this link's contributions to the network system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSelector::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Default the effective conductance, then call the virtual updateState method so a derived
    ///   model can override it.
    mEffectiveConductance = mDefaultConductance;
    updateState(dt);

    /// - The blockage malfunction reduces effective conductance.
    if (mMalfBlockageFlag) {
        mEffectiveConductance *= MsMath::limitRange(0.0, (1.0 - mMalfBlockageValue), 1.0);
    }

    /// - Limit effective conductance to GUNNS range.
    if (mEffectiveConductance < m100EpsilonLimit) {
        mEffectiveConductance = 0.0;
    } else {
        mEffectiveConductance = std::min(mEffectiveConductance, mConductanceLimit);
    }

    clearAdmittance();
    buildAdmittance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes out the link admittance matrix whenever the connected port # changes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSelector::clearAdmittance()
{
    const int port = getConnectedPort();
    if (port != mLastConnectedPort) {
        for (int i=0; i<mNumPorts*mNumPorts; ++i) {
            mAdmittanceMatrix[i] = 0.0;
        }
    }
    mLastConnectedPort = port;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the link admittance matrix for inclusion into the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSelector::buildAdmittance()
{
    /// - System admittance is zeroed unless there is a connection between port 0 and another
    ///   valid port.
    const int port = getConnectedPort();
    if (port > 0) {
        mSystemAdmittance = mEffectiveConductance;
    } else {
        mSystemAdmittance = 0.0;
    }

    /// - Assemble the admittance matrix.
    if (std::fabs(mAdmittanceMatrix[0] - mSystemAdmittance) > 0.0) {
        mAdmittanceMatrix[0]                   =  mSystemAdmittance;
        mAdmittanceMatrix[port]                = -mSystemAdmittance;
        mAdmittanceMatrix[port*mNumPorts]      = -mSystemAdmittance;
        mAdmittanceMatrix[port*mNumPorts+port] =  mSystemAdmittance;
        mAdmittanceUpdate                      = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Method for computing the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSelector::computeFlows(const double dt)
{
    const int port = getConnectedPort();
    mPotentialDrop = getDeltaPotential(0, port);
    computeFlux();
    updateFlux(dt, mFlux);
    computePower();
    transportFlux(0, port);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Compute flux across the link, defined positive from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsElectSelector::computeFlux()
{
    const int    port = getConnectedPort();
    const double hiP  = std::max(mPotentialVector[0], mPotentialVector[port]);
    if (std::fabs(mPotentialDrop) < (hiP * m100EpsilonLimit)) {
        /// - Zero flux if dP is too low.  This eliminates most false quantity leak due to rounding
        ///   error in the solver.
        mFlux = 0.0;
    } else {
        mFlux = mPotentialDrop * mAdmittanceMatrix[0];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int  (--)  Actual connected port #.
///
/// @details  Returns the actual connected port #.  If the selected port # is < 1 or > the max port
///           # for the link, then this returns 0 so that there is effectively no connection.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsElectSelector::getConnectedPort() const
{
    int result = mSelector;
    if (result < 1 or result >= mNumPorts) {
        result = 0;
    }
    return result;
}
