/**
@file
@brief    GUNNS Fluid External Supply Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Source File for GUNNS Fluid External Supply)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (GunnsFluidSource.o)
   )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
    )
*/
#include "GunnsFluidExternalSupply.hh"
#include "core/GunnsFluidUtils.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details  This value is chosen to get reliable network capacitance calculations from the solver
///           for liquid and gas nodes.
const double GunnsFluidExternalSupply::mNetworkCapacitanceFlux = 1.0E-6;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                  (--) Link name
/// @param[in] nodes                 (--) Network nodes array
/// @param[in] externalConfig        (--) Pointer to the external network's fluid config data
/// @param[in] convertToType         (--) Fluid type to convert extra external constituents to
/// @param[in] useNetworkCapacitance (--) Causes mSupplyCapacitance to be available for external demands
///
/// @details  Default GUNNS Fluid External Supply Config Data Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalSupplyConfigData::GunnsFluidExternalSupplyConfigData(
        const std::string&         name,
        GunnsNodeList*             nodes,
        PolyFluidConfigData*       externalConfig,
        FluidProperties::FluidType convertToType,
        const bool                 useNetworkCapacitance)
    :
    GunnsFluidSourceConfigData(name, nodes),
    mExternalConfig           (externalConfig),
    mConvertToType            (convertToType),
    mUseNetworkCapacitance    (useNetworkCapacitance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  GUNNS Fluid External Supply Config Data Copy Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalSupplyConfigData::GunnsFluidExternalSupplyConfigData
        (const GunnsFluidExternalSupplyConfigData& that)
    :
    GunnsFluidSourceConfigData(that),
    mExternalConfig           (that.mExternalConfig),
    mConvertToType            (that.mConvertToType),
    mUseNetworkCapacitance    (that.mUseNetworkCapacitance)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid External Supply Config Data Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalSupplyConfigData::~GunnsFluidExternalSupplyConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag      (--)   Blockage malfunction flag
/// @param[in] malfBlockageValue     (--)   Blockage malfunction fractional value (0-1)
/// @param[in] flowDemand            (kg/s) Initial demanded mass flow rate of the link
/// @param[in] demandTemperature     (K)    Initial demand temperature
/// @param[in] demandMassFractions   (--)   Initial demand mass fractions
/// @param[in] demandTcMoleFractions (--)   Initial demand trace compounds mole fractions
///
/// @details  Default constructs this Fluid External Supply input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalSupplyInputData::GunnsFluidExternalSupplyInputData(const bool    malfBlockageFlag,
                                                                     const double  malfBlockageValue,
                                                                     const double  flowDemand,
                                                                     const double  demandTemperature,
                                                                           double* demandMassFractions,
                                                                           double* demandTcMoleFractions)
    :
    GunnsFluidSourceInputData(malfBlockageFlag, malfBlockageValue, flowDemand),
    mDemandTemperature    (demandTemperature),
    mDemandMassFractions  (demandMassFractions),
    mDemandTcMoleFractions(demandTcMoleFractions)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Fluid External Supply input data.
///
/// @note  This is a shallow copy, mDemandMassFractions will point to the same address as the source
///        object's pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalSupplyInputData::GunnsFluidExternalSupplyInputData(
        const GunnsFluidExternalSupplyInputData& that)
    :
    GunnsFluidSourceInputData(that),
    mDemandTemperature    (that.mDemandTemperature),
    mDemandMassFractions  (that.mDemandMassFractions),
    mDemandTcMoleFractions(that.mDemandTcMoleFractions)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid External Supply input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalSupplyInputData::~GunnsFluidExternalSupplyInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalSupply::GunnsFluidExternalSupply()
    :
    GunnsFluidSource      (),
    mUseNetworkCapacitance(0.0),
    mTransformMap         (0),
    mSupplyCapacitance    (0.0),
    mSupplyPressure       (0.0),
    mSupplyTemperature    (0.0),
    mSupplyMassFractions  (0),
    mSupplyTcMoleFractions(0),
    mDemandFlux           (0.0),
    mDemandTemperature    (0.0),
    mDemandMassFractions  (0),
    mDemandTcMoleFractions(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructs the Link
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidExternalSupply::~GunnsFluidExternalSupply()
{
    TS_DELETE_ARRAY(mDemandTcMoleFractions);
    TS_DELETE_ARRAY(mDemandMassFractions);
    TS_DELETE_ARRAY(mSupplyMassFractions);
    {
        delete [] mTransformMap;
        mTransformMap = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Fluid External Supply Config Data
/// @param[in]     inputData    (--) GUNNS Fluid External Supply Input Data
/// @param[in,out] networkLinks (--) Network links
/// @param[in]     port0        (--) Network port 0
/// @param[in]     port1        (--) Network port 1
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid External Supply with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalSupply::initialize(const GunnsFluidExternalSupplyConfigData& configData,
                                          const GunnsFluidExternalSupplyInputData&  inputData,
                                          std::vector<GunnsBasicLink*>&             networkLinks,
                                          const int                                 port0,
                                          const int                                 port1)
{
    /// - Initialize & validate parent.
    GunnsFluidSource::initialize(configData, inputData, networkLinks, port0, port1);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Create the internal link fluid, allocate and load the fluid config map to translate the
    ///   external network's fluid to match our local network's config.
    createInternalFluid();

    delete [] mTransformMap;
    mTransformMap = new int[configData.mExternalConfig->mNTypes + 1];

    GunnsFluidUtils::buildTransformMap(mTransformMap, configData.mExternalConfig,
            mNodes[0]->getFluidConfig(), configData.mConvertToType);

    /// - Allocate memory for the read & write data mass fraction arrays.
    TS_DELETE_ARRAY(mSupplyMassFractions);
    TS_DELETE_ARRAY(mDemandMassFractions);
    TS_NEW_PRIM_ARRAY_EXT(mSupplyMassFractions, mNodes[0]->getFluidConfig()->mNTypes, double, std::string(mName) + ".mSupplyMassFractions");
    TS_NEW_PRIM_ARRAY_EXT(mDemandMassFractions, configData.mExternalConfig->mNTypes,  double, std::string(mName) + ".mDemandMassFractions");

    /// - Allocate memory for the demand trace compounds mole fraction array.  Note this remains
    ///   un-allocated if the network has no trace compounds config or zero compounds in the config.
    ///   The supply trace compounds mole fraction array points directly to the node's array.
    const GunnsFluidTraceCompoundsConfigData* tcConfig = mNodes[0]->getFluidConfig()->mTraceCompounds;
    if (tcConfig) {
        const int nTc = tcConfig->mNTypes;
        if (nTc > 0) {
            TS_DELETE_ARRAY(mDemandTcMoleFractions);
            TS_NEW_PRIM_ARRAY_EXT(mDemandTcMoleFractions, nTc, double, std::string(mName) + ".mDemandTcMoleFractions");
        }
        mSupplyTcMoleFractions = mNodes[0]->getContent()->getTraceCompounds()->getMoleFractions();
    }

    /// - Initialize the output supply terms.
    mSupplyCapacitance = 0.0;
    mSupplyPressure    = mNodes[0]->getContent()->getPressure();
    mSupplyTemperature = mNodes[0]->getContent()->getTemperature();
    for (int i=0; i<mNodes[0]->getFluidConfig()->mNTypes; ++i) {
        mSupplyMassFractions[i] = mNodes[0]->getContent()->getMassFraction(i);
    }

    /// - Initialize the input demand terms.  Normally these will be overwritten by a demand link
    ///   via simbus, but initializing here allows this link to be run standalone if desired.  We
    ///   use the base class mFlowDemand mass rate instead of mDemandFlux molar rate.  We use the
    ///   node's temperature if the input data temperature is not specified, {1.0, 0.0 ...} mass
    ///   fractions if the input data fractions are not specified, and zero trace compounds mole
    ///   fractions if their input data is not specified.
    mDemandFlux = 0.0;
    if (inputData.mDemandTemperature > DBL_EPSILON) {
        mDemandTemperature = inputData.mDemandTemperature;
    } else {
        mDemandTemperature = mNodes[0]->getContent()->getTemperature();
    }
    if (inputData.mDemandMassFractions) {
        for (int i=0; i<configData.mExternalConfig->mNTypes; ++i) {
            mDemandMassFractions[i] = inputData.mDemandMassFractions[i];
        }
    } else {
        mDemandMassFractions[0] = 1.0;
        for (int i=1; i<configData.mExternalConfig->mNTypes; ++i) {
            mDemandMassFractions[i] = 0.0;
        }
    }
    if (mDemandTcMoleFractions) {
        if (inputData.mDemandTcMoleFractions) {
            for (int i=0; i<tcConfig->mNTypes; ++i) {
                mDemandTcMoleFractions[i] = inputData.mDemandTcMoleFractions[i];
            }
        } else {
            for (int i=0; i<tcConfig->mNTypes; ++i) {
                mDemandTcMoleFractions[i] = 0.0;
            }
        }
    }

    /// - Initialize from config data.
    mUseNetworkCapacitance = configData.mUseNetworkCapacitance;

    /// - Validate initialization.
    validate();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates this GUNNS Fluid External Supply initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalSupply::validate() const
{
    /// - There is currently nothing to validate.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalSupply::restartModel()
{
    /// - Reset the base class.
    GunnsFluidSource::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data read from the external network's demand link, via the sim bus.  Data is
///          moved from the storage terms input from simbus into the link internal members.  The
///          demand flow rate is supported as either a mass flow (mFlowDemand) or molar flow
///          (mDemandFlux) rate.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalSupply::processInputs()
{
    try {
        GunnsFluidUtils::transformState(mInternalFluid, 1.0, mDemandTemperature,
                                        mDemandMassFractions, mTransformMap,
                                        mDemandTcMoleFractions);
    } catch (TsOutOfBoundsException &e) {
        GUNNS_WARNING("caught exception from GunnsFluidUtils::transformState.");
    }
    mFlux     = mDemandFlux;
    mFlowRate = mFlowDemand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Handles data written to the external network's demand link, via the simbus.  Data is
///          moved from the supply node's content into the storage terms for output to simbus.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalSupply::processOutputs()
{
    mSupplyCapacitance = mNodes[0]->getNetworkCapacitance();
    mSupplyPressure    = mNodes[0]->getPotential();
    mSupplyTemperature = mNodes[0]->getContent()->getTemperature();
    for (int i = 0; i < mInternalFluid->getNConstituents(); ++i) {
        mSupplyMassFractions[i] = mNodes[0]->getContent()->getMassFraction(i);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Not used.
///
/// @details  Updates the link during the time step.
///
/// @note     The blockage malfunction is deliberately not implemented, so that we are forced to
///           send the same flux as the external network demands in order to conserve mass.  To
///           block the flux across the interface, use the malfunction on the demand link side.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalSupply::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - The link supports demanded flow as either mass or molar flow, but output a warning if we
    ///   are getting competing inputs, as this indicates an problem in simbus set-up.  We will use
    ///   molar flux demand over mass flow demand if both are set.
    if (fabs(mFlowRate) > DBL_EPSILON and fabs(mFlux) > DBL_EPSILON) {
        GUNNS_WARNING("is receiving both molar and mass flow demands.");
    }

    /// - Calculate link mass flow rate from the demanded molar flux and molecular weight of the
    ///   source fluid.  If there is an input mass flow demand but no molar flux demand, calculate
    ///   the link molar flux rate from the demanded mass flow.  The demand flux takes priority if
    ///   both mass & molar flows are set.
    if (mFlux < -DBL_EPSILON) {
        mFlowRate = mFlux     * mInternalFluid->getMWeight();

    } else if (mFlux > DBL_EPSILON) {
        mFlowRate = mFlux     * mNodes[0]->getContent()->getMWeight();

    } else if (mFlowRate < -DBL_EPSILON) {
        mFlux     = mFlowRate / mInternalFluid->getMWeight();

    } else if (mFlowRate > DBL_EPSILON) {
        mFlux     = mFlowRate / mNodes[0]->getContent()->getMWeight();
    }

    /// - Lower-limit the mass flow rate and corresponding molar flux.
    if (fabs(mFlowRate) < m100EpsilonLimit) {
        mFlowRate = 0.0;
        mFlux     = 0.0;
    }

    /// - Build the system source vector.
    buildSource();

    /// - Flag the node to have its network capacitance calculated by the solver for output to
    ///   external demands.
    if (mUseNetworkCapacitance) {
        mNodes[0]->setNetworkCapacitanceRequest(mNetworkCapacitanceFlux);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Not used.
///
/// @details  Transports the flows across the link.  This overrides the base class implementation in
///           two important ways: 1) updateFluid is not called, as this class is not meant to modify
///           the fluid, 2) this version cannot force a mixture change in the port 1 node if the
///           internal fluid mixture is different from the port 1 node content.  This is an
///           important distinction: if the external network has a different fluid configuration
///           than ours, the constituents in our network that aren't present in the external network
///           will be missing from our internal fluid (as our internal fluid state comes from the
///           external network).  We want flow from our network to the external network to move all
///           of our constituents out of our network, even if it is converted to another fluid type
///           in the external network.
///
/// @note     Volumetric flow and hydraulic power are not calculated for this link, as they would
///           match the GunnsFluidExternalDemand that this link is paired to.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidExternalSupply::transportFlows(const double dt __attribute__((unused)))
{
    mPotentialDrop = getDeltaPotential();

    /// - When flow is positive, the internal fluid is not used for anything since it's just flowing
    ///   to ground in our network, and the fluid properties output to the demand link come from the
    ///   node instead.  However, we go ahead and load the internal fluid with the node's properties
    ///   so it is a convenient place for users to see the actual flow properties in either
    ///   direction.
    if (mFlowRate > m100EpsilonLimit) {
        mInternalFluid->setState(mNodes[0]->getOutflow());
    }

    /// - Call the base class to transport the fluid between the nodes.  The false bool argument
    ///   indicates that this class cannot force a mixture change in the supplying node.
    transportFluid(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidExternalSupply must not map port 0 to the network vacuum node.
///           - A GunnsFluidExternalSupply must map port 1 to the network vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidExternalSupply::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 0 is the vacuum node.
    if ((0 == port) && (node == getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to the boundary node.");
        result = false;
    }

    /// - Fail if port 1 is not the vacuum node.
    else if ((1 == port) && (node != getGroundNodeIndex())) {
        GUNNS_WARNING("aborted setting a port: must assign port 1 to the boundary node.");
        result = false;
    }

    return result;
}
