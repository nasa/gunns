/**
@file
@brief    GUNNS Fluid Evaporation Link implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ()
*/

#include "GunnsFluidEvaporation.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "aspects/fluid/capacitor/GunnsFluidAccum.hh"

/// @details  This value is large enough to allow the accumulator's bellows to pressurize the liquid
///           but small enough to avoid significantly biasing the desired pressure.
const double GunnsFluidEvaporation::mSpringCoeff1 = 1.0e-6;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name             (--)      Name of object.
/// @param[in] nodes            (--)      Pointer to nodes.
/// @param[in] gasType          (--)      Gas type that the liquid phase changes into.
/// @param[in] evaporationCoeff (1/s/kPa) Evaporation rate coefficient.
/// @param[in] poolMassExponent (--)      Exponent on pool mass contribution to evaporation rate.
/// @param[in] liquidPoolAccum  (--)      Pointer to the liquid pool accumulator.
///
/// @details  Default constructs this GUNNS Evaporation link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEvaporationConfigData::GunnsFluidEvaporationConfigData(
        const std::string&               name,
        GunnsNodeList*                   nodes,
        const FluidProperties::FluidType gasType,
        const double                     evaporationCoeff,
        const double                     poolMassExponent,
        GunnsFluidAccum*                 liquidPoolAccum)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mGasType(gasType),
    mEvaporationCoeff(evaporationCoeff),
    mPoolMassExponent(poolMassExponent),
    mLiquidPoolAccum(liquidPoolAccum)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Evaporation link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEvaporationConfigData::GunnsFluidEvaporationConfigData(const GunnsFluidEvaporationConfigData& that)
    :
    GunnsFluidLinkConfigData(that),
    mGasType(that.mGasType),
    mEvaporationCoeff(that.mEvaporationCoeff),
    mPoolMassExponent(that.mPoolMassExponent),
    mLiquidPoolAccum(that.mLiquidPoolAccum)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Evaporation link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEvaporationConfigData::~GunnsFluidEvaporationConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  malfBlockageFlag   (--)    Blockage malfunction flag.
/// @param[in]  malfBlockageValue  (--)    Blockage malfunction fractional value (0-1).
/// @param[in]  gasTotalPressure   (kPa)   Initial gas-side total pressure.
/// @param[in]  vaporPotential     (kPa)   Initial gas-side vapor potential.
/// @param[in]  evaporationRate    (kg/s)  Initial evaporation mass rate.
///
/// @details  Default constructs this GUNNS Evaporation link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEvaporationInputData::GunnsFluidEvaporationInputData(const bool   malfBlockageFlag,
                                                               const double malfBlockageValue,
                                                               const double gasTotalPressure,
                                                               const double vaporPotential,
                                                               const double evaporationRate)
    :
    GunnsFluidLinkInputData(malfBlockageFlag,
                            malfBlockageValue),
    mGasTotalPressure(gasTotalPressure),
    mVaporPotential(vaporPotential),
    mEvaporationRate(evaporationRate)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Evaporation link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEvaporationInputData::GunnsFluidEvaporationInputData(const GunnsFluidEvaporationInputData& that)
    :
    GunnsFluidLinkInputData(that),
    mGasTotalPressure(that.mGasTotalPressure),
    mVaporPotential(that.mVaporPotential),
    mEvaporationRate(that.mEvaporationRate)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Evaporation link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEvaporationInputData::~GunnsFluidEvaporationInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Evaporation link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEvaporation::GunnsFluidEvaporation()
    :
    GunnsFluidLink(NPORTS),
    mGasType(FluidProperties::NO_FLUID),
    mEvaporationCoeff(0.0),
    mPoolMassExponent(0.0),
    mLiquidPoolAccum(0),
    mGasTotalPressure(0.0),
    mGasMWeight(0.0),
    mEvaporationFluid(0),
    mLiquidPoolMass(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Evaporation link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidEvaporation::~GunnsFluidEvaporation()
{
    delete mEvaporationFluid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   (--)  Configuration data.
/// @param[in]      inputData    (--)  Input data.
/// @param[in,out]  links        (--)  Link vector.
/// @param[in]      port0        (--)  Nominal inlet port map index.
/// @param[in]      port1        (--)  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Evaporation link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::initialize(const GunnsFluidEvaporationConfigData& configData,
                                       const GunnsFluidEvaporationInputData&  inputData,
                                       std::vector<GunnsBasicLink*>&          links,
                                       const int                              port0,
                                       const int                              port1)
{
    /// - Initialize with config data needed for initial node mapping.
    mGasType          = configData.mGasType;
    mLiquidPoolAccum  = configData.mLiquidPoolAccum;

    /// - Initialize the parent class.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Reset initialization status flag.
    mInitFlag         = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize with remaining config data.
    DefinedFluidProperties definedFluidProps;
    mGasMWeight   = definedFluidProps.getProperties(mGasType)->getMWeight();
    mEvaporationCoeff = configData.mEvaporationCoeff;
    mPoolMassExponent = configData.mPoolMassExponent;

    /// - Initialize with input data.
    mGasTotalPressure = inputData.mGasTotalPressure;
    mPotentialDrop    = inputData.mVaporPotential;
    mFlowRate         = inputData.mEvaporationRate;
    // Divide-by-zero protected against by validation of mGasType.
    mFlux             = mFlowRate / mGasMWeight;

    if (mNodeMap[1] != getGroundNodeIndex()) {
        /// - Initialize a PolyFluid for 100% evaporation gas.
        TS_DELETE_OBJECT(mEvaporationFluid);
        TS_NEW_PRIM_OBJECT_EXT(mEvaporationFluid, PolyFluid,
                (*(mNodes[1]->getContent()), std::string(mName) + ".mEvaporationFluid", false),
                std::string(mName) + ".mEvaporationFluid");

        double* massFractions = new double[mNodes[1]->getContent()->getNConstituents()];
        for (int i=0; i<mNodes[1]->getContent()->getNConstituents(); ++i) {
            massFractions[i] = 0.0;
        }
        massFractions[mEvaporationFluid->find(mGasType)] = 1.0;

        mEvaporationFluid->resetState();
        mEvaporationFluid->setFlowRate(0.0);
        mEvaporationFluid->setMassAndMassFractions(0.0, massFractions);
        mEvaporationFluid->setTemperature(mNodes[0]->getContent()->getTemperature());
        delete [] massFractions;
    }

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag         = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) The configuration data to be validated.
/// @param[in] inputData (--) The input data to be validated.
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Evaporation link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::validate(const GunnsFluidEvaporationConfigData& configData,
                                     const GunnsFluidEvaporationInputData&  inputData) const
{
    /// - Throw an exception if pool mass exponent isn't in range.
    if (not MsMath::isInRange(-10.0, configData.mPoolMassExponent, 10.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Pool mass exponent outside of (-10, 10).");
    }

    /// - Throw an exception if evaporation coefficient < 0.
    if (configData.mEvaporationCoeff < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Evaporation coefficient < 0.");
    }

    /// - Throw an exception if the gas type is NO_FLUID.
    if (FluidProperties::NO_FLUID == configData.mGasType) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Gas type is NO_FLUID.");
    }

    /// - Throw an exception if the gas type isn't a gas.
    DefinedFluidProperties definedFluidProps;
    if (FluidProperties::GAS != definedFluidProps.getProperties(mGasType)->getPhase()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Gas type isn't a gas.");
    }

    /// - Throw an exception if the gas type isn't in the network when connected to a gas node.
    if (mNodeMap[1] != getGroundNodeIndex()) {
        try {
            mNodes[1]->getContent()->find(mGasType);
        } catch (...) {
            GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                        "Gas type isn't in the gas network.");
        }
    }

    /// - Throw an exception if initial gas total pressure < 0.
    if (inputData.mGasTotalPressure < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Gas total pressure < 0.");
    }

    /// - Throw an exception if initial vapor potential < 0.
    if (inputData.mVaporPotential < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Vapor potential < 0.");
    }

    /// - Throw an exception if initial evaporation rate < 0.
    if (inputData.mEvaporationRate < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Evaporation rate < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-configuration and non-checkpointed data.
    mLiquidPoolMass = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Execution time step.
///
/// @details  Updates the evaporation model and builds this link's contributions to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::step(const double dt)
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Update evaporation model.
    updateVaporPotential();
    updateVaporRate(dt);
    updatePower();

    /// - Build the link source vector.
    mFlux = mFlowRate / std::max(mGasMWeight, DBL_EPSILON);
    mSourceVector[0] = -mFlux;
    mSourceVector[1] =  mFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  When attached to a gas node, compute the "vapor potential", or delta of the
///           evaporation gas saturation and partial pressures in the gas node.  Otherwise, this
///           will be provided by init value and/or the gas-side link via sim bus.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::updateVaporPotential()
{
    if (mNodeMap[1] != getGroundNodeIndex()) {
        const double gasPartialP = mPotentialVector[1]
                                 * mNodes[1]->getContent()->getMoleFraction(mGasType);

        const FluidProperties* gasProps = mNodes[1]->getContent()->getProperties(mGasType);
        const double gasSaturationP =
                gasProps->getSaturationPressure(mNodes[1]->getContent()->getTemperature());

        mPotentialDrop = gasSaturationP - gasPartialP;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt (s)  Execution time step.
///
/// @details  When attached to a liquid node, compute the evaporation flow rate.  Otherwise, this
///           will be provided by init value and/or the liquid-side link via sim bus.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::updateVaporRate(const double dt)
{
    if (mLiquidPoolAccum and mNodeMap[0] != getGroundNodeIndex()) {

        /// - Interface with the liquid pool accumulator.
        mLiquidPoolMass = mLiquidPoolAccum->getUsableMass();
        mLiquidPoolAccum->setSpringCoeffs(mGasTotalPressure, mSpringCoeff1, 0.0);

        double evaporationRate = 0.0;
        const double liqMWeight = mNodes[0]->getContent()->getMWeight();

        if ( (DBL_EPSILON < mGasMWeight) and (DBL_EPSILON < liqMWeight) and (DBL_EPSILON < dt) ) {
            if (mLiquidPoolMass > FLT_EPSILON) {
                /// - Evaporation gas rate as a function of pool mass, evaporation coefficient and
                ///   vapor potential, then reduced by the link's blockage malfunction.  Limited to
                ///   positive values so this link can't condense.
                double mdot_gas = std::max(mPotentialDrop, 0.0) * mEvaporationCoeff
                                * powf(mLiquidPoolMass, mPoolMassExponent);
                if (mMalfBlockageFlag) {
                    mdot_gas *= (1.0 - MsMath::limitRange(0.0, mMalfBlockageValue, 1.0));
                }

                /// - Limit the evaporation rate to the available liquid mass.  The ratio of mol
                ///   weights accounts for the case where the evaporating species is in a liquid
                ///   solution, so the available mass to evaporate is a fraction of the total pool.
                const double mdot_liq = std::min(mdot_gas * liqMWeight / mGasMWeight,
                                                 mLiquidPoolMass / dt);
                evaporationRate = mdot_liq * mGasMWeight / liqMWeight;
            }
        }
        mFlowRate = evaporationRate;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  When attached to a gas node, compute the heat of evaporation and store it in mPower.
///
/// @note     This redefines the meaning of mPower for this link, different from the base class.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::updatePower()
{
    if (mNodeMap[1] != getGroundNodeIndex()) {
        const FluidProperties* gasProps = mNodes[1]->getContent()->getProperties(mGasType);
        mPower = gasProps->getHeatOfVaporization(mNodes[1]->getContent()->getTemperature())
               * UnitConversion::UNIT_PER_KILO * mFlowRate;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  unused.
///
/// @details  Computes Source/sink directions values for portDirections.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::computeFlows(const double dt __attribute__((unused)))
{
    /// - Set port flow directions and schedule flow from source nodes.
    if (mFlux > DBL_EPSILON) {
        mPortDirections[0] = SOURCE;
        mPortDirections[1] = SINK;
        mNodes[0]->scheduleOutflux(mFlux);
    } else if (mFlux < -DBL_EPSILON) {
        mPortDirections[0] = SINK;
        mPortDirections[1] = SOURCE;
        mNodes[1]->scheduleOutflux(-mFlux);
    } else {
        mPortDirections[0] = NONE;
        mPortDirections[1] = NONE;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  unused.
///
/// @details  Updates final flow properties and transports fluid from source to sink nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidEvaporation::transportFlows(const double dt __attribute__((unused)))
{
    if (mNodeMap[1] != getGroundNodeIndex()) {
        /// - Calculate true volumetric flow rate of the evaporating gas.
        const double evapDensity = mNodes[1]->getOutflow()->getDensity()
                                 * mNodes[1]->getOutflow()->getMassFraction(mGasType);
        if (evapDensity > DBL_EPSILON) {
            mVolFlowRate = mFlowRate / evapDensity;
        } else {
            mVolFlowRate = 0.0;
        }

        mGasTotalPressure = mPotentialVector[1];
    }

    if (mFlowRate > m100EpsilonLimit) {
        if (mEvaporationFluid and mNodeMap[1] != getGroundNodeIndex()) {
            /// - Transport evaporation flow to the gas node.  The evaporation flow is at the gas
            ///   node temperature.
            mEvaporationFluid->setTemperature(mNodes[1]->getOutflow()->getTemperature());
            mNodes[1]->collectInflux(mFlowRate, mEvaporationFluid);
        }

        if (mNodeMap[0] != getGroundNodeIndex()) {
            /// - Transport evaporation flow out of the liquid node.
            const double liqFlowRate = mFlux * mNodes[0]->getOutflow()->getMWeight();
            mNodes[0]->collectOutflux(liqFlowRate);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned.
/// @param[in] node (--) The desired node to assign the port to.
///
/// @return   bool  (--) Returns true if all rules checks pass.
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidEvaporation must map port 0 to a liquid or the Ground node.
///           - A GunnsFluidEvaporation must map port 1 to a gas or the Ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidEvaporation::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);

    /// - Fail if port 0 is not Ground or in liquid phase.
    if ( (0 == port) and (node != getGroundNodeIndex()) ) {
        if (FluidProperties::LIQUID != nodePtr[node].getContent()->getPhase()) {
            GUNNS_WARNING("aborted setting a port: cannot assign port 0 to a gas node.");
            result = false;
        }

        /// - Fail if port 0 is a liquid node when the liquid pool accumulator link pointer is null.
        if (not mLiquidPoolAccum) {
            GUNNS_WARNING("aborted setting a port: cannot assign port 0 to a liquid node with null liquid pool accumulator pointer.");
            result = false;
        }
    }

    else if ( (1 == port) and (node != getGroundNodeIndex()) ) {
        /// - Fail if port 1 is not Ground or in gas phase.
        if (FluidProperties::GAS != nodePtr[node].getContent()->getPhase()) {
            GUNNS_WARNING("aborted setting a port: cannot assign port 1 to a liquid node.");
            result = false;
        }
    }

    return result;
}
