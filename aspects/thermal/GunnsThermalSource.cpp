/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 (
  (core/GunnsBasicLink.o)
 )
***************************************************************************************************/
#include "GunnsThermalSource.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "math/MsMath.hh" //needed for MsMath::isInRange() in validate()
#include "aspects/thermal/PtcsMacros.hh"
/**************************************************************************************************/
/* ConfigData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name             (--)   name of given ThermalSource object
/// @param[in]  nodes            (--)   pointer to Gunns nodes to which object connects
/// @param[in]  tuningScalar     (--)   source's electrical-to-thermal tuning scalar
/// @param[in]  fluxDistributionFractions (--)   pointer to vector of fractions that describe the
///                                              flux distribution to each port
///
/// @details  Constructs this GunnsThermalSource ConfigData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalSourceConfigData::GunnsThermalSourceConfigData(
        const std::string& name,
        GunnsNodeList* nodes,
        const double tuningScalar,
        std::vector<double>* fluxDistributionFractions)
    :
    GunnsBasicLinkConfigData(name, nodes),
    cTuningScalar(tuningScalar),
    cFluxDistributionFractions(),
    cNumPortsOverride(0)
{
    /// - Clear the flux-distribution-fraction vector.
    cFluxDistributionFractions.clear();

    /// - Check for null pointer.
    if (0 != fluxDistributionFractions)
    {
        /// - Set the flux-distribution-fraction vector.
        cFluxDistributionFractions = *fluxDistributionFractions;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)   input source to copy
///
/// @details  Copy constructs this GunnsThermalSource ConfigData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalSourceConfigData::GunnsThermalSourceConfigData(const GunnsThermalSourceConfigData& that)
    :
    GunnsBasicLinkConfigData(that),
    cTuningScalar(that.cTuningScalar),
    cFluxDistributionFractions(that.cFluxDistributionFractions),
    cNumPortsOverride(that.cNumPortsOverride)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fractions     (--)   new flux distribution fractions
/// @param[in]  numFractions  (--)   number of new flux distribution fractions
///
/// @details  Overrides the flux distribution fractions for this GunnsThermalSource ConfigData.
///           Intended to be called from input file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSourceConfigData::overrideFluxDistributionFractions(double* fractions, const int numFractions) {
    if (fractions != 0 && numFractions > 0) {
        cFluxDistributionFractions.clear();
        cFluxDistributionFractions.resize(numFractions);
        cFluxDistributionFractions.assign(fractions, fractions + numFractions);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this GunnsThermalSource ConfigData Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalSourceConfigData::~GunnsThermalSourceConfigData()
{
    // nothing to do
}

/**************************************************************************************************/
/* InputData class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    malfBlockageFlag       (--)   Blockage malfunction flag
/// @param[in]    malfBlockageValue      (--)   Blockage malfunction fractional value (0-1)
/// @param[in]    demandedFlux           (W)    Initial demanded flux of the link
/// @param[in]    malfFluxOverrideFlag   (--)   Override flux malfunction flag
/// @param[in]    malfFluxOverrideValue  (W)    Override flux malfunction value
///
/// @details  Default constructs this GunnsThermalSource InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalSourceInputData::GunnsThermalSourceInputData(const bool   malfBlockageFlag,
                                                         const double malfBlockageValue,
                                                         const double demandedFlux,
                                                         const bool   malfFluxOverrideFlag,
                                                         const double malfFluxOverrideValue)
    :
    GunnsBasicLinkInputData(malfBlockageFlag, malfBlockageValue),
    iDemandedFlux(demandedFlux),
    iMalfFluxOverrideFlag(malfFluxOverrideFlag),
    iMalfFluxOverrideValue(malfFluxOverrideValue)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  (--)  Object to copy
///
/// @details  Copy constructs this GunnsThermalSource InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalSourceInputData::GunnsThermalSourceInputData(const GunnsThermalSourceInputData& that)
    :
    GunnsBasicLinkInputData(that),
    iDemandedFlux(that.iDemandedFlux),
    iMalfFluxOverrideFlag(that.iMalfFluxOverrideFlag),
    iMalfFluxOverrideValue(that.iMalfFluxOverrideValue)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GunnsThermalSource InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalSourceInputData::~GunnsThermalSourceInputData()
{
    // nothing to do
}

/**************************************************************************************************/
/* GunnsThermalSource class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tolerance on sum of fractions being within one.
////////////////////////////////////////////////////////////////////////////////////////////////////
const double GunnsThermalSource::FRACTION_TOLERANCE  = 1.1e-02;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs the GunnsThermalSource with default values
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalSource::GunnsThermalSource()
    :
    GunnsBasicLink(1),            //  give one port as default; more will be added in initialize()
    mMalfFluxOverrideFlag(false), // (--) Malfunction flag for heat flux override
    mMalfFluxOverrideValue(0.0),  // (W)  Malfunction value for heat flux override
    mTemperatureOverrideFlag(false), // (--) Activates the all-ports temperature override
    mTemperatureOverrideValue(0.0),  // (K)  Value used for the all-ports temperature override
    mDemandedFlux(0.0),           // (W)  demanded heat flux through the link
    mTuningScalar(1.0),           // (--) default source tuning scalar value assumed equal to 1
    mFluxDistributionFractions(0),// (--) array of fractions of the distribution of power b/t nodes
    mAvgPortTemperature(0.0)      // (K)  Average temperature of ports
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructs the GunnsThermalSource
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsThermalSource::~GunnsThermalSource()
{
    TS_DELETE_ARRAY(mFluxDistributionFractions);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]        configData   (--)  Source configuration data
/// @param[in]        inputData    (--)  Source input data
/// @param[in,out]    networkLinks (--)  Network link vector
/// @param[in]        portsVector  (--)  Vector of port numbers this Source is linked to
///
/// @throw    TsInitializationException by base class initialize() or validate()
///
/// @details  This initializes the GunnsThermalSource and sets up its connectivity to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::initialize(const GunnsThermalSourceConfigData& configData,
                                    const GunnsThermalSourceInputData&  inputData,
                                    std::vector<GunnsBasicLink*>&       networkLinks,
                                    std::vector<int>*                   portsVector)
{
    /// - Set number of ports based on portsVector parameter.
    ///   This will coincide with the network drawing.
    mNumPorts = static_cast<int>(portsVector->size());
    /// - Declare temporary ports array.
    int ports[mNumPorts];
    /// - Set members of ports array.
    for(int i = 0; i < mNumPorts; ++i)
    {
        ports[i] = portsVector->at(i);
    }

    /// - Override the number of ports.
    ///   In the case where a super network contains more ports for a
    ///   GunnsThermalSource than are in the subnetwork, we override the
    ///   number of ports. The "ports" array (set above) will contain the node
    ///   mapping from the original network drawing.
    ///
    ///   the super network modification is three-fold:
    ///   ConfigData.cNumPortsOverride, ConfigData.cFluxDistributionFractions,
    ///   and InputData.mInitialNodeMap. All of these can be modified in the
    ///   input file. cFluxDistributionFractions and mInitialNodeMap may be
    ///   modified independently as long as the number of elements does not change.
    ///   If the number of elements changes in either one, the other must
    ///   be modified to match, and the cNumPortsOverride must be modified.
    if (configData.cNumPortsOverride > 0) {
        mNumPorts = configData.cNumPortsOverride;
    }

    /// - Call BasicLink::initialize to set up ports.
    GunnsBasicLink::initialize(configData, inputData, networkLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Validate config and input data specific to a GunnsThermalSource.
    validate(configData, inputData);

    /// - Dynamically allocate the mFluxDistributionFractions array.
    TS_NEW_PRIM_ARRAY_EXT(mFluxDistributionFractions, mNumPorts, double, std::string(mName) + ".mFluxDistributionFractions");

    /// - Set members of mFluxDistributionFractions.
    for(int i = 0; i < mNumPorts; ++i)
    {
        mFluxDistributionFractions[i] = configData.cFluxDistributionFractions.at(i);
    }
    /// - Initialize the source with its input demanded flux.
    mDemandedFlux = inputData.iDemandedFlux;

    /// - Initialize the source with its input flux override malfunction values.
    mMalfFluxOverrideFlag = inputData.iMalfFluxOverrideFlag;
    mMalfFluxOverrideValue = inputData.iMalfFluxOverrideValue;

    /// - Initialize the temperature override to off.
    mTemperatureOverrideFlag = false;
    mTemperatureOverrideValue = 0.0;

    /// - Configure the source with its demand-to-actual tuning scalar.
    mTuningScalar = configData.cTuningScalar;

    /// - Update mAvgPortTemperature member.
    determineAvgPortTemperature();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
///
/// @throw   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS ThermalSource link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::validate(const GunnsThermalSourceConfigData& configData,
                                  const GunnsThermalSourceInputData&  inputData) const
{
    /// - Throw exception if flux-distribution-fraction is empty
    TS_PTCS_IF_ERREX(configData.cFluxDistributionFractions.empty(), TsInitializationException,
            "invalid config data", "FluxDistributionFractions vector is empty.");

    /// - Throw exception if size of the flux-distribution-fraction vector != the link's number of ports
    TS_PTCS_IF_ERREX(mNumPorts != static_cast<int>(configData.cFluxDistributionFractions.size()),
            TsInitializationException,
            "invalid config data", "Size of flux-distribution-fraction vector != numPorts.");

    /// - Throw an exception if flux demand < 0.
    TS_PTCS_IF_ERREX(inputData.iDemandedFlux < 0.0, TsInitializationException,
            "invalid input data", "Input flux demand less than zero.");

    /// - Calculate the sum of the flux-distribution fractions. Check for any that are out of range.
    double one = 0.0;
    for (unsigned int i = 0; i < configData.cFluxDistributionFractions.size(); ++i)
    {
        /// - Each flux-distribution fraction must be between zero and one.
        TS_PTCS_IF_ERREX(!MsMath::isInRange(0.0, configData.cFluxDistributionFractions.at(i), 1.0),
                TsInitializationException,
                "invalid config data", "Flux-distribution fraction out of range (0-1).");

        /// - Add toward the sum.
        one += configData.cFluxDistributionFractions.at(i);
    }
    /// - Calculate error.
    const double error = fabs(1.0 - one);

    /// - Throw exception if flux-distribution fractions do not add up to one.
    TS_PTCS_IF_ERREX(error > FRACTION_TOLERANCE, TsInitializationException,
            "invalid config data", "Flux-distribution fractions do not add up to 1.");
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::restartModel()
{
    /// - Reset the base class.
    GunnsBasicLink::restartModel();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details This updates the link during a calculation frame.
///          This class creates a fixed flux between two nodes.  One of the nodes may be
///          GROUND, or both may be normal network nodes.  Flow direction is defined positive
///          from Port0 to Port1, so it is best to use Port0 as the GROUND node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Updates the source's mDemandedFlux.
    updateState(dt);

    /// - The actual flux through the link will be the desired flux (mDemandedFlux), scaled by
    ///   mTuningScalar and the blockage malfunction, if it is active.
    mFlux = mTuningScalar * mDemandedFlux;

    /// - The override malfunction takes precedence over other malfs. It sets the flux directly.
    if (mMalfFluxOverrideFlag)
    {
        mFlux = mMalfFluxOverrideValue;
    }
    /// - If blockage malfunction is active, scale the flux by one minus the blockage value.
    else if (mMalfBlockageFlag)
    {
        mFlux *= (1.0 - mMalfBlockageValue);
    }

    /// - If the temperature override flag is set, activate the basic link's port potential override
    ///   capability for every port.
    if (mTemperatureOverrideFlag) {
        for (int i = 0; i < mNumPorts; ++i) {
            mOverrideVector[i]  = true;
            mPotentialVector[i] = mTemperatureOverrideValue;
        }
    } else {
        for (int i = 0; i < mNumPorts; ++i) {
            mOverrideVector[i]  = false;
        }
    }

    /// - Populate the link's source vector with the flux that goes toward each port.
    buildSourceVector();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt  (s)  Integration time step [not used]
///
/// @details  Updates the state of the GunnsThermalSource. This method does nothing but is a
///           placeholder for derived classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::updateState(const double dt __attribute__((unused)))
{
    /// - Determine and set the AvgPortTemperature.
    determineAvgPortTemperature();
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the source vector terms of the link's contribution to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::buildSourceVector()
{
    /// - Loop through each port.
    for (int i = 0; i < mNumPorts; ++i)
    {
        /// - Set each slot in the SourceVector to be the overall flux scaled by the appropriate
        ///   flux-distribution fraction.
        mSourceVector[i] = mFluxDistributionFractions[i] * mFlux;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt  (s)  Integration time step [not used]
///
/// @details  Makes calls to accumulate flux in the input/output terms of the receiving nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::computeFlows(const double dt __attribute__((unused)))
{
    /// - Loop through each port.
    for (int i = 0; i < mNumPorts; ++i)
    {
        /// - Tell each node to add to its influxRate.
        mNodes[i]->collectInflux(mFluxDistributionFractions[i] * mFlux);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Loops through ports, finding the temperature at each node. These are averaged to
///           determine the AvgPortTemperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::determineAvgPortTemperature()
{
    double sum = 0.0;

    /// - Loop through each port.
    for (int i = 0; i < mNumPorts; ++i)
    {
        /// - Increase the sum.
        sum += mNodes[i]->getPotential();
    }

    /// - Find average and set.
    mAvgPortTemperature = sum / mNumPorts;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    port  (--)  The port to be assigned
/// @param[in]    node  (--)  The desired node to assign the port to
///
/// @return   boolean (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsThermalSource must not map any port to the network vacuum boundary node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsThermalSource::checkSpecificPortRules(const int port __attribute__((unused)), const int node) const
{
    bool result = true;

    /// - Fail if the port is the vacuum boundary node.
    if (node == getGroundNodeIndex() )
    {
        /// - Send warning report.
        TS_PTCS_WARNING("Cannot assign a port to ground node.");
        result = false;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] index (W)  Malfunction value for heat flux override.
///
/// @details  Sets the malf parameters to the given values.  Calling this method with default
///           arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsThermalSource::setMalfFluxOverride(const bool flag, const double value)
{
    mMalfFluxOverrideFlag  = flag;
    mMalfFluxOverrideValue = value;
}
