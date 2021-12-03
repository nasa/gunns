/**
@file
@brief Simple H2 Redox link implementation.

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (core/GunnsFluidLink.o)
   )
*/

#include "GunnsFluidSimpleH2Redox.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details  Heat is 48.6 kJ/mol, converted to J/kg*mol.  Source: NREL/CP-550-47302,
///           "Hydrogen Production: Fundamentals and Case Study Summaries", Harrison, et.al.,
///           National Renewable Energy Laboratory, Jan. 2010
const double GunnsFluidSimpleH2Redox::mReactionHeat = 4.86e7;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      name                (--)        Name of object.
/// @param[in,out]  nodes               (--)        Pointer to nodes.
/// @param[in]      numCells            (--)        Number of reaction cells in the stack.
/// @param[in]      cellVoltageLoaded   (V)         Nominal cell voltage under load.
/// @param[in]      cellH2ReactRate     (kg/s/amp)  Cell H2 reaction mass rate per amp.
/// @param[in]      maxEfficiency       (--)        Maximum efficiency (0-1) of the reaction.
///
/// @details  Default constructs this Simple H2 Redox link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleH2RedoxConfigData::GunnsFluidSimpleH2RedoxConfigData(const std::string& name,
                                                                     GunnsNodeList*     nodes,
                                                                     const int          numCells,
                                                                     const double       cellVoltageLoaded,
                                                                     const double       cellH2ReactRate,
                                                                     const double       maxEfficiency)
    :
    GunnsFluidLinkConfigData(name, nodes),
    mNumCells(numCells),
    mCellVoltageLoaded(cellVoltageLoaded),
    mCellH2ReactRate(cellH2ReactRate),
    mMaxEfficiency(maxEfficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Simple H2 Redox link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleH2RedoxConfigData::~GunnsFluidSimpleH2RedoxConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      malfBlockageFlag   (--)    Blockage malfunction flag.
/// @param[in]      malfBlockageValue  (--)    Blockage malfunction fractional value (0-1).
/// @param[in]      current            (amp)   Initial electrical current.
/// @param[in]      trippedOff         (--)    Initial tripped off flag.
///
/// @details  Default constructs this Simple H2 Redox link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleH2RedoxInputData::GunnsFluidSimpleH2RedoxInputData(const bool   malfBlockageFlag,
                                                                   const double malfBlockageValue,
                                                                   const double current,
                                                                   const bool   trippedOff)
    :
    GunnsFluidLinkInputData(malfBlockageFlag, malfBlockageValue),
    mCurrent(current),
    mTrippedOff(trippedOff)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Simple H2 Redox link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleH2RedoxInputData::~GunnsFluidSimpleH2RedoxInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this Simple H2 Redox link model with obviously invalid data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleH2Redox::GunnsFluidSimpleH2Redox()
    :
    GunnsFluidLink(NPORTS),
    mNumCells(0),
    mCellVoltageLoaded(0.0),
    mCellH2ReactRate(0.0),
    mMaxEfficiency(0.0),
    mCurrent(0.0),
    mTrippedOff(false),
    mOutputStackVoltage(0.0),
    mOutputHeat(0.0),
    mEfficiency(0.0),
    mH2MassRate(0.0),
    mO2MassRate(0.0),
    mH2OMassRate(0.0),
    mH2MoleRate(0.0),
    mO2MoleRate(0.0),
    mH2OMoleRate(0.0),
    mH2Fluid(0),
    mO2Fluid(0),
    mH2OFluid(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Simple H2 Redox link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSimpleH2Redox::~GunnsFluidSimpleH2Redox()
{
//    TS_DELETE_OBJECT(mHeFluid);
    TS_DELETE_OBJECT(mH2OFluid);
    TS_DELETE_OBJECT(mO2Fluid);
    TS_DELETE_OBJECT(mH2Fluid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Fuel (H2/CH4) node port map index.
/// @param[in]      port1       (--)  Air (O2/He) node port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Simple H2 Redox link model with configuration and input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleH2Redox::initialize(      GunnsFluidSimpleH2RedoxConfigData& configData,
                                         const GunnsFluidSimpleH2RedoxInputData&  inputData,
                                         std::vector<GunnsBasicLink*>&            links,
                                         const int                                port0,
                                         const int                                port1)
{
    /// - First initialize and validate parent.
    int ports[2] = {port0, port1};
    GunnsFluidLink::initialize(configData, inputData, links, ports);

    /// - Reset the initialization status flag.
    mInitFlag   = false;

    /// - Validate the configuration data.
    validate(configData, inputData);

    /// - Create and initialize the internal fluids.  We ignore the mInternalFluid base class
    ///   pointer and use our own fluid objects for clarity.
    if (mH2Fluid) {
        mH2Fluid->cleanup();
        TS_DELETE_OBJECT(mH2Fluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mH2Fluid, PolyFluid,
                           (*(mNodes[0]->getContent()), mName + ".mH2Fluid", false),
                           mName + ".mH2Fluid");
    mH2Fluid->resetState();
    mH2Fluid->setMass(mNodes[0]->getContent()->find(FluidProperties::GUNNS_H2), 1.0);
    mH2Fluid->updateMass();

    if (mO2Fluid) {
        mO2Fluid->cleanup();
        TS_DELETE_OBJECT(mO2Fluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mO2Fluid, PolyFluid,
                           (*(mNodes[0]->getContent()), mName + ".mO2Fluid", false),
                           mName + ".mO2Fluid");
    mO2Fluid->resetState();
    mO2Fluid->setMass(mNodes[0]->getContent()->find(FluidProperties::GUNNS_O2), 1.0);
    mO2Fluid->updateMass();

    if (mH2OFluid) {
        mH2OFluid->cleanup();
        TS_DELETE_OBJECT(mH2OFluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mH2OFluid, PolyFluid,
                           (*(mNodes[0]->getContent()), mName + ".mH2OFluid", false),
                           mName + ".mH2OFluid");
    mH2OFluid->resetState();
    mH2OFluid->setMass(mNodes[0]->getContent()->find(FluidProperties::GUNNS_H2O), 1.0);
    mH2OFluid->updateMass();

    /// - Initialize from the validated configuration and input data.
    mNumCells           = configData.mNumCells;
    mCellVoltageLoaded  = configData.mCellVoltageLoaded;
    mCellH2ReactRate    = configData.mCellH2ReactRate;
    mMaxEfficiency      = configData.mMaxEfficiency;
    mCurrent            = inputData.mCurrent;
    mTrippedOff         = inputData.mTrippedOff;

    /// - Initialize remaining state attributes.
    zeroReactionRates();

    /// - Set the initialization status flag to indicate successful initialization.
    mInitFlag   = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this Simple H2 Redox link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleH2Redox::validate(const GunnsFluidSimpleH2RedoxConfigData& configData,
                                       const GunnsFluidSimpleH2RedoxInputData&  inputData)
{
    /// - Throw an exception on reaction compounds (H2, O2, H2O) unavailable in the network.
    try {
        mNodes[0]->getContent()->find(FluidProperties::GUNNS_H2);
    } catch (TsOutOfBoundsException& e) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "fluid type GUNNS_H2 not in network.");
    }
    try {
        mNodes[0]->getContent()->find(FluidProperties::GUNNS_O2);
    } catch (TsOutOfBoundsException& e) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "fluid type GUNNS_O2 not in network.");
    }
    try {
        mNodes[0]->getContent()->find(FluidProperties::GUNNS_H2O);
    } catch (TsOutOfBoundsException& e) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "fluid type GUNNS_H2O not in network.");
    }

    /// - Throw an exception if number of cells < 1.
    if (configData.mNumCells < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "number of cells < 1.");
    }

    /// - Throw an exception if number of loaded cell voltage < DBL_EPSILON.
    if (configData.mCellVoltageLoaded < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "cell voltage under load < DBL_EPSILON.");
    }

    /// - Throw an exception if H2 reaction rate < DBL_EPSILON.
    if (configData.mCellH2ReactRate < DBL_EPSILON) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "H2 reaction rate < DBL_EPSILON.");
    }

    /// - Throw an exception if max efficiency not [0-1).
    if (!MsMath::isInRange(DBL_EPSILON, configData.mMaxEfficiency, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "max efficiency not in [0-1).");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleH2Redox::restartModel()
{
    /// - Reset the base class.
    GunnsFluidLink::restartModel();

    /// - Reset non-config & non-checkpointed attributes.
    mEfficiency = 0.0;
    zeroReactionRates();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step (not used).
///
/// @details  Updates the model state: computes reaction flow rates and updates the link's
///           contributions to the network system of equations.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleH2Redox::step(const double dt __attribute__((unused)))
{
    /// - Process user commands to dynamically re-map ports.
    processUserPortCommand();

    /// - Check node reactant partial pressures and trip off the model if they are near zero.  Once
    ///   set, the flag must be manually reset by the user.  This prevents pulling negative pressure
    ///   or constituent masses on the nodes.  We use an arbitrary limit pressure of 1 Pa.
    if (mCurrent > 0.0) {
        if (mNodes[0]->getContent()->getMoleFraction(FluidProperties::GUNNS_H2) * mPotentialVector[0] < 0.001 or
            mNodes[1]->getContent()->getMoleFraction(FluidProperties::GUNNS_O2) * mPotentialVector[1] < 0.001) {
            mTrippedOff = true;
        }
    } else {
        if (mNodes[0]->getContent()->getMoleFraction(FluidProperties::GUNNS_H2O) * mPotentialVector[0] < 0.001) {
            mTrippedOff = true;
        }
    }

    /// - Reaction efficiency, limited to [0-1).
    if (mMalfBlockageFlag) {
        mEfficiency = MsMath::limitRange(DBL_EPSILON, mMaxEfficiency * (1.0 - mMalfBlockageValue), 1.0);
    } else {
        mEfficiency = MsMath::limitRange(DBL_EPSILON, mMaxEfficiency, 1.0);
    }

    /// - H2 rate as function of electrical current and efficiency.  Negative sign because positive
    ///   current removes H2.  Reduced efficiency increases fuel cell H2 consumption and reduces
    ///   electrolysis H2 production,
    mH2MassRate = -mCurrent * mCellH2ReactRate * mNumCells;
    if (mCurrent > 0.0) {
        mH2MassRate /= mEfficiency;
    } else {
        mH2MassRate *= mEfficiency;
    }

    if (mTrippedOff or fabs(mH2MassRate) < m100EpsilonLimit) {
        /// - Zero all reaction rates if the reactor is tripped off or insufficient H2 rate.
        zeroReactionRates();

    } else {
        /// - When reacting, update all molar rates from the H2 mass rates, molecular masses and
        ///   reaction stoichiometry.  The 0.5 factor on O2 mole rate is because there is 1/2 O2
        ///   molecule for each H2 molecule.
        mH2MoleRate  =  mH2MassRate / mH2Fluid->getMWeight();
        mO2MoleRate  =  0.5 * mH2MoleRate;
        mH2OMoleRate = -mH2MoleRate;

        /// - Update O2 & H2O mass rates.
        mO2MassRate  =  mO2MoleRate * mO2Fluid->getMWeight();
        mH2OMassRate =  mH2OMoleRate * mH2OFluid->getMWeight();
    }

    /// - Update stack volts output to electrical aspect.
    if (mTrippedOff) {
        mOutputStackVoltage = 0.0;
    } else {
        mOutputStackVoltage = mCellVoltageLoaded * mNumCells;
    }

    /// - Output heat to thermal aspect.  Output heat is positive (exothermic) for fuel-cell
    ///   reaction, which has negative H2 rate, so flip the sign here.
    mOutputHeat = mReactionHeat * -mH2MoleRate;

    /// - Update the link source vector.
    buildSource();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Builds the source vector terms of the link's contribution to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSimpleH2Redox::buildSource()
{
    mSourceVector[0] = mH2MoleRate + mH2OMoleRate;
    mSourceVector[1] = mO2MoleRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  Compute Fluid properties for flow transport.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleH2Redox::computeFlows(const double dt __attribute__((unused)))
{
    /// - Set port directions based on source vectors.  In the case of negative source vector, we
    ///   are pulling some mass out of the node.  Since these are gas nodes it's unlikely that the
    ///   node is ever 100% the source fluid type.  So we don't bother with using the SOURCE port
    ///   direction.  This link avoids overflows by tripping off when the source fluid type gets
    ///   too rare in the source node.
    mPortDirections[0] = NONE;
    mPortDirections[1] = NONE;

    /// - mH2MoleRate always equals the negative of mH2OMoleRate so mSourceVector should always
    ///   equal zero.  This check is left here in case some future upgrade allows the mSourceVector
    ///   to be non-zero (addition of absorption/desoprtion of contaminants, etc.)
    if (mSourceVector[0] > m100EpsilonLimit) {
        mPortDirections[0] = SINK;
    }
    if (mSourceVector[1] > m100EpsilonLimit) {
        mPortDirections[1] = SINK;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @details  Transports the reaction constituent flows to/from the nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSimpleH2Redox::transportFlows(const double dt __attribute__((unused)))
{
    /// - Set the pressure & temperature of the internal fluids equal to the port nodes to reduce
    ///   pressure correction in the nodes.
    mH2Fluid->setPressure(mPotentialVector[0]);
    mH2OFluid->setPressure(mPotentialVector[0]);
    mO2Fluid->setPressure(mPotentialVector[1]);

    mH2Fluid->setTemperature(mNodes[0]->getOutflow()->getTemperature());
    mH2OFluid->setTemperature(mNodes[0]->getOutflow()->getTemperature());
    mO2Fluid->setTemperature(mNodes[1]->getOutflow()->getTemperature());

    if (fabs(mH2MassRate) >= m100EpsilonLimit) {
        /// - Transport node flows.
        mNodes[0]->collectInflux(mH2MassRate,  mH2Fluid);
        mNodes[0]->collectInflux(mH2OMassRate, mH2OFluid);
        mNodes[1]->collectInflux(mO2MassRate,  mO2Fluid);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    port  (--)  The port to be assigned.
/// @param[in]    node  (--)  The desired node to assign the port to.
///
/// @returns  (--) True if all rules checks pass.
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           Simple H2 Redox link.  These are:
///           - No port can connect to the network's vacuum boundary node.
///           - The node at both ports must contain gas phase.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidSimpleH2Redox::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if node is the vacuum boundary node.
    if (node == getGroundNodeIndex()) {
        GUNNS_WARNING("aborted setting a port: cannot assign any port to the boundary node.");
        result = false;
    }

    /// - Fail if port 0 is not in gas phase.
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);
    if ((0 == port) && (FluidProperties::GAS != nodePtr[node].getContent()->getPhase())) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to a gas node.");
        result = false;
    }

    /// - Fail if port 1 is not in gas phase.
    if ((1 == port) && (FluidProperties::GAS != nodePtr[node].getContent()->getPhase())) {
        GUNNS_WARNING("aborted setting a port: must assign port 1 to a gas node.");
        result = false;
    }

    return result;
}
