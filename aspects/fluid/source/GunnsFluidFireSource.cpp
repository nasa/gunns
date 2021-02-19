/**
@file
@brief    GUNNS Fluid Fire Source Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (core/GunnsFluidLink.o)
    (core/GunnsFluidNode.o)
    (aspects/fluid/fluid/PolyFluid.o)
    (simulation/hs/TsHsMsg.o)
    (software/exceptions/TsInitializationException.o)
   )
*/

#include "aspects/fluid/source/GunnsFluidFireSource.hh"
#include "aspects/fluid/fluid/PolyFluid.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name               (--)   Link name.
/// @param[in] nodes              (--)   Network nodes array.
/// @param[in] fireO2ConsumpRate  (kg/J) O2 mass consumption per Joule of fire energy output.
/// @param[in] fireCO2ProductRate (kg/J) CO2 mass production per Joule of fire energy output.
/// @param[in] fireH2OProductRate (kg/J) H2O mass production per Joule of fire energy output.
/// @param[in] fireMinReqO2       (kPa)  Minimum partial pressure of O2 to sustain fire
/// @param[in] traceCompoundRates (kg/J) Array of trace compound mass production per Joule of fire energy output.
///
/// @details  Default GUNNS Fluid Fire Source Config Data Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFireSourceConfigData::GunnsFluidFireSourceConfigData(
        const std::string& name,
        GunnsNodeList*     nodes,
        const double       fireO2ConsumpRate,
        const double       fireCO2ProductRate,
        const double       fireH2OProductRate,
        const double       fireMinReqO2,
        double*            fireTraceCompoundRates)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mFireO2ConsumpRate(fireO2ConsumpRate),
    mFireCO2ProductRate(fireCO2ProductRate),
    mFireH2OProductRate(fireH2OProductRate),
    mFireMinReqO2(fireMinReqO2),
    mFireTraceCompoundRates(fireTraceCompoundRates)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  GUNNS Fluid Fire Source Config Data Copy Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFireSourceConfigData::GunnsFluidFireSourceConfigData(
        const GunnsFluidFireSourceConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mFireO2ConsumpRate(that.mFireO2ConsumpRate),
    mFireCO2ProductRate(that.mFireCO2ProductRate),
    mFireH2OProductRate(that.mFireH2OProductRate),
    mFireMinReqO2(that.mFireMinReqO2),
    mFireTraceCompoundRates(that.mFireTraceCompoundRates)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default GUNNS Fluid Fire Source Config Data Destructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFireSourceConfigData::~GunnsFluidFireSourceConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag   (--)   Blockage malfunction flag.
/// @param[in] malfBlockageValue  (--)   Blockage malfunction fractional value (0-1).
/// @param[in] malfFireFlag       (--)   Initial fire malfunction activation flag
/// @param[in] malfFireHeat       (W)    Initial fire malfunction generated heat value.
///
/// @details  Default constructs this Fluid Fire Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFireSourceInputData::GunnsFluidFireSourceInputData(
        const bool                         malfBlockageFlag,
        const double                       malfBlockageValue,
        const bool                         malfFireFlag,
        const double                       malfFireHeat)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mMalfFireFlag(malfFireFlag),
    mMalfFireHeat(malfFireHeat)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy.
///
/// @details  Copy constructs this Fluid Fire Source input data.  This is a shallow copy.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFireSourceInputData::GunnsFluidFireSourceInputData(
        const GunnsFluidFireSourceInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mMalfFireFlag(that.mMalfFireFlag),
    mMalfFireHeat(that.mMalfFireHeat)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Fire Source input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFireSourceInputData::~GunnsFluidFireSourceInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Constructs the Link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFireSource::GunnsFluidFireSource()
    :
    GunnsFluidLink(NPORTS),
    mMalfFireFlag(false),
    mMalfFireHeat(0.0),
    mFireO2ConsumpRate(0.0),
    mFireCO2ProductRate(0.0),
    mFireH2OProductRate(0.0),
    miO2(0),
    miCO2(0),
    miH2O(0),
    mFlowO2(0.0),
    mFlowCO2(0.0),
    mFlowH2O(0.0),
    mTraceCompoundRates(0),
    mTraceCompoundMassRates(0),
    mFireMinReqO2(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Destructs the Link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFireSource::~GunnsFluidFireSource()
{
    /// - Delete the trace compounds array.
    TS_DELETE_ARRAY(mTraceCompoundMassRates);
    TS_DELETE_ARRAY(mTraceCompoundRates);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData   (--) GUNNS Fluid Fire Source Config Data.
/// @param[in]     inputData    (--) GUNNS Fluid Fire Source Input Data.
/// @param[in,out] networkLinks (--) Network links.
/// @param[in]     port0        (--) Network port 0.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Fire Source with configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::initialize(const GunnsFluidFireSourceConfigData& configData,
                                      const GunnsFluidFireSourceInputData&  inputData,
                                      std::vector<GunnsBasicLink*>&         networkLinks,
                                      const int                             port0)
{
    /// - Initialize & validate parent.
    GunnsFluidLink::initialize(configData, inputData, networkLinks, &port0);

    /// - Reset init flag.
    mInitFlag           = false;

    /// - Validate initialization.
    validate(configData);

    /// - Initialize from input data.
    mMalfFireFlag         = inputData.mMalfFireFlag;
    mMalfFireHeat         = inputData.mMalfFireHeat;

    /// - Initialize from config data.
    mFireO2ConsumpRate  = configData.mFireO2ConsumpRate;
    mFireCO2ProductRate = configData.mFireCO2ProductRate;
    mFireH2OProductRate = configData.mFireH2OProductRate;
    mFireMinReqO2       = configData.mFireMinReqO2;

    /// - Create internal fluid from node state
    //createInternalFluidFromNode();
    createInternalFluid();

    miO2  = mInternalFluid->find(FluidProperties::GUNNS_O2);
    miCO2 = mInternalFluid->find(FluidProperties::GUNNS_CO2);
    miH2O = mInternalFluid->find(FluidProperties::GUNNS_H2O);

    /// - Initialize the trace compounds array.
    if (GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds()) {
        const int tcNtypes = tc->getConfig()->mNTypes;
        TS_NEW_PRIM_ARRAY_EXT(mTraceCompoundRates,     tcNtypes, double, mName + ".mTraceCompoundRates");
        TS_NEW_PRIM_ARRAY_EXT(mTraceCompoundMassRates, tcNtypes, double, mName + ".mTraceCompoundMassRates");

        for (int i = 0; i < tcNtypes; ++i) {
            if (configData.mFireTraceCompoundRates) {
                mTraceCompoundRates[i] = configData.mFireTraceCompoundRates[i];
            } else {
                mTraceCompoundRates[i] = 0.0;
            }
            mTraceCompoundMassRates[i] = 0.0;
        }
    }

    /// - Set init flag on successful validation.
    mInitFlag           = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  GUNNS Fluid Fire Source Config Data.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Fire Source initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::validate(const GunnsFluidFireSourceConfigData& configData) const
{
    /// - Throw an exception for sum of mass flow rates is equal to zero
    if (DBL_EPSILON > fabs(configData.mFireCO2ProductRate + configData.mFireH2OProductRate
                         + configData.mFireO2ConsumpRate)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Sum of bulk fluid mass flow rates is equal to zero.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed class attributes.
    mFlowO2  = 0.0;
    mFlowCO2 = 0.0;
    mFlowH2O = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details Updates the link during the time step
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    if ( mNodes[0]->getContent()->getPartialPressure(FluidProperties::GUNNS_O2) < mFireMinReqO2){
        mMalfFireFlag = false;
    }

    /// - Call the virtual updateState method so that any derived class may modify the behavior.
    updateState(dt);

    /// - Call calculateFlowRate to set mFlowRate relative to input data.
    calculateFlowRate();

    /// - Reduce the actual flow rate by the blockage malfunction if it is active.
    if (mMalfBlockageFlag) {
        mFlowRate *= (1.0 - mMalfBlockageValue);
        mFlowO2   *= (1.0 - mMalfBlockageValue);
        mFlowCO2  *= (1.0 - mMalfBlockageValue);
        mFlowH2O  *= (1.0 - mMalfBlockageValue);
    }

    /// - Convert the mass flow rate demand to molar flow rate based on the molecular weight of the
    ///   source fluid.  Molar flow rate is zeroed in TC-only mode since there will be no affect on
    ///   bulk fluid in the node.
    const double sourceMWeight = mInternalFluid->getMWeight();
    if (sourceMWeight > DBL_EPSILON) {
        mFlux = mFlowRate / sourceMWeight;
    } else {
        mFlux = 0.0;
    }

    buildSource();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  Computes the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::computeFlows(const double dt)
{
    /// - Set port flow directions and schedule flow from source nodes.
    if (mFlux > DBL_EPSILON) {
        mPortDirections[0] = SINK;
    } else {
        mPortDirections[0] = NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s) Integration time step
///
/// @details  transports and updates the flows across the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::transportFlows(const double dt)
{
    /// - Calculate fire power (heat/time).
    computePower();

    /// - Call the virtual updateFluid method to allow derived classes to further modify the
    ///   internal fluid before it is transported.
    updateFluid(dt, mFlowRate);

    /// - Transport the internal fluid to/from the attached node.
    transportFluid(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] forcedOutflow (--) Not used.
/// @param[in] fromPort      (--) Not used.
/// @param[in] toPort        (--) Not used.
///
/// @details  This function adds the link's flow to the attached node's collection terms.
///           Flow is defined positive going to the node, so a positive flow will add mass to it.
///
///           This implementation is simplified from the base class in several important ways:
///           - Since this is a 1-port link, the port arguments are ignored and all flows go to/from
///             the port 0 node.
///           - This method doesn't verify the port arguments since they're ignored.
///           - This link always has the internal fluid specified, so we don't check for it.
///           - This link always forces mixture change of the node for outflows, so we ignore the
///             forcedOutflow argument.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::transportFluid(const bool forcedOutflow __attribute__((unused)),
                                          const int  fromPort      __attribute__((unused)),
                                          const int  toPort        __attribute__((unused)))
{
    /// - Minimum mass flow rate for transport.  Flow rates in links below this value are
    ///   not transported between nodes.  Math faults in the PolyFluid class can result when
    ///   mass or molar flow rates less than DBL_EPSILON are attempted, and this limit
    ///   value keeps both mass and molar flow rates above DBL_EPSILON.
    ///
    /// - Both positive and negative flows are added to the node's influx collection, since this
    ///   link can force outflow of a specific mixture.
    if ((mFlowRate > m100EpsilonLimit) or (mFlowRate < -m100EpsilonLimit)) {
        mNodes[0]->collectInflux (mFlowRate, mInternalFluid);
    }

    /// - Insert the fire heat directly into the node instead of via the bulk fluid transport, since
    ///   the bulk fluid is at the node temperature.
    mNodes[0]->collectHeatFlux(mPower);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt         (s)    Not used.
/// @param[in] flowRate   (--)   Not used.
///
/// @details Updates the fluids in the network if there is a fire.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::updateFluid(const double dt       __attribute__((unused)),
                                       const double flowRate __attribute__((unused)))
{
    if (mMalfFireFlag and
        mNodes[0]->getContent()->getPartialPressure(FluidProperties::GUNNS_O2) > mFireMinReqO2) {
        mInternalFluid->resetState();
        setBulkMasses();

        // Update output atmosphere mass, moles & fractions from constituent moles
        mInternalFluid->updateMass();
        setTCMasses();

        // setting the temperature value of the poly fluid to equal the temperature of the Node
        mInternalFluid->setTemperature(mNodes[0]->getContent()->getTemperature());
    } else {
        /// - Malf inactive, or extinguished by low O2.
        mMalfFireFlag = false;
        zeroGenValues();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calculates all constituent contributions to flow rate if a fire is present.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::calculateFlowRate()
{
    /// - Only set rates if malf not active.  Will be zeroed by computeFlows when inactive.
    if (mMalfFireFlag) {
        /// - Add constituent flow rates that are functions of the heat to be produced.
        ///   Setting each bulk fluid constituent mass flow rate
        mFlowCO2   =  mFireCO2ProductRate * mMalfFireHeat;
        mFlowO2    = -mFireO2ConsumpRate  * mMalfFireHeat;
        mFlowH2O   =  mFireH2OProductRate * mMalfFireHeat;
        mFlowRate = mFlowCO2 + mFlowO2 + mFlowH2O;

        /// - Compute trace compound mass flow rates.
        if ( mInternalFluid->getTraceCompounds()){
            for (int i = 0; i < mInternalFluid->getTraceCompounds()->getConfig()->mNTypes; ++i)
            {
                /// - Calculate individual Trace Compound mass flow rates from user rate and
                ///   heat output
                mTraceCompoundMassRates[i] = mTraceCompoundRates[i] * mMalfFireHeat;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Sets all of the flow rates for the bulk fluid constituents flowing into node
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::setBulkMasses()
{
    /// - Set mass flow rates of bulk fluid constituents
    mInternalFluid->setMass(miO2,  mFlowO2);
    mInternalFluid->setMass(miCO2, mFlowCO2);
    mInternalFluid->setMass(miH2O, mFlowH2O);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Sets all of the flow rates for the trace compounds flowing into node
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::setTCMasses()
{
    /// - Set mass flow rates of trace compounds
    GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        const GunnsFluidTraceCompoundsConfigData& tcConfig = *tc->getConfig();
        for (int i = 0; i < tcConfig.mNTypes; ++i) {
            tc->setMass(i, mTraceCompoundMassRates[i]);
        }
        /// - Updating trace compounds mole fractions after setting mass flows
        tc->updateMoleFractions();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Sets all mass rates to zero
///////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFireSource::zeroGenValues()
{
    /// - Zero out all bulk fluid mass flows
    mFlowCO2   = 0.0;
    mFlowO2    = 0.0;
    mFlowH2O   = 0.0;
    mFlowRate  = 0.0;

    /// - Zero out all trace compound mass flows
    GunnsFluidTraceCompounds* tc = mInternalFluid->getTraceCompounds();
    if (tc) {
        const GunnsFluidTraceCompoundsConfigData& tcConfig = *tc->getConfig();
        for (int i = 0; i < tcConfig.mNTypes; ++i)
        {
            mTraceCompoundMassRates[i] = 0.0;
        }
    }

    /// - Zero out fire heat contribution
    mPower = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    port  (--)  The port to be assigned.
/// @param[in]    node  (--)  The desired node to assign the port to.
///
/// @returns  bool (--) True if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           a Gunns Fluid Fire Source.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidFireSource::checkSpecificPortRules(const int port __attribute__((unused)), const int node) const
{
    bool result = true;
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);

    /// - Throw an exception if the operating fluid type is not a gas.
    if (FluidProperties::GAS != nodePtr[node].getContent()->getPhase()) {
        GUNNS_WARNING("aborted setting a port: cannot assign port 0 to a liquid node.");
        result = false;
    }

    return result;
}
