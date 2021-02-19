/**
@file
@brief    GUNNS Fluid Phase Change Conductor Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((core/GunnsFluidConductor.o))
*/

#include "GunnsFluidPhaseChangeConductor.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name             (--)  Name of object.
/// @param[in]  nodes            (--)  Pointer to nodes.
/// @param[in]  maxConductivity  (m2)  Max conductivity.
/// @param[in]  liquidType       (--)  Fluid type of constituent liquid being changed.
/// @param[in]  gasType          (--)  Fluid type of constituent gas being changed.
/// @param[in]  efficiency       (--)  Optional efficiency of phase change heat.
///
/// @details  Default constructs this GUNNS Fluid Phase Change Conductor link model configuration
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeConductorConfigData::GunnsFluidPhaseChangeConductorConfigData(
        const std::string&               name,
        GunnsNodeList*                   nodes,
        const double                     maxConductivity,
        const FluidProperties::FluidType liquidType,
        const FluidProperties::FluidType gasType,
        const double                     efficiency)
    :
    GunnsFluidConductorConfigData(name, nodes, maxConductivity, 0.0),
    mLiquidType(liquidType),
    mGasType(gasType),
    mEfficiency(efficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Phase Change Conductor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeConductorConfigData::GunnsFluidPhaseChangeConductorConfigData(
        const GunnsFluidPhaseChangeConductorConfigData& that)
    :
    GunnsFluidConductorConfigData(that),
    mLiquidType(that.mLiquidType),
    mGasType(that.mGasType),
    mEfficiency(that.mEfficiency)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Phase Change Conductor link model configuration
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeConductorConfigData::~GunnsFluidPhaseChangeConductorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
/// @param[in] wallTemperature   (K)  Initial wall temperature
///
/// @details  Default constructs this GUNNS Fluid Phase Change Conductor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeConductorInputData::GunnsFluidPhaseChangeConductorInputData(
        const bool   malfBlockageFlag,
        const double malfBlockageValue,
        const double wallTemperature)
    :
    GunnsFluidConductorInputData(malfBlockageFlag, malfBlockageValue),
    mWallTemperature(wallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that  (--)  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Phase Change Conductor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeConductorInputData::GunnsFluidPhaseChangeConductorInputData(
        const GunnsFluidPhaseChangeConductorInputData& that)
    :
    GunnsFluidConductorInputData(that),
    mWallTemperature(that.mWallTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Phase Change Conductor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeConductorInputData::~GunnsFluidPhaseChangeConductorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Phase Change Conductor link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeConductor::GunnsFluidPhaseChangeConductor()
    :
    GunnsFluidConductor(),
    mLiquidType(FluidProperties::NO_FLUID),
    mGasType(FluidProperties::NO_FLUID),
    mLiquidFluid(0),
    mGasFluid(0),
    mEfficiency(0.0),
    mWallHeatFlux(0.0),
    mWallTemperature(0.0)
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Phase Change Conductor link model.
///////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidPhaseChangeConductor::~GunnsFluidPhaseChangeConductor()
{
    /// - Delete the internal fluids.
    TS_DELETE_OBJECT(mGasFluid);
    TS_DELETE_OBJECT(mLiquidFluid);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData  (--)  Configuration data.
/// @param[in]      inputData   (--)  Input data.
/// @param[in,out]  links       (--)  Link vector.
/// @param[in]      port0       (--)  Nominal inlet port map index.
/// @param[in]      port1       (--)  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Phase Change Conductor link model with configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeConductor::initialize(
        const GunnsFluidPhaseChangeConductorConfigData& configData,
        const GunnsFluidPhaseChangeConductorInputData&  inputData,
        std::vector<GunnsBasicLink*>&                   links,
        const int                                       port0,
        const int                                       port1)
{
    /// - First initialize & validate parent.
    GunnsFluidConductor::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize attributes from the validated configuration and input data.
    mLiquidType      = configData.mLiquidType;
    mGasType         = configData.mGasType;
    mEfficiency      = configData.mEfficiency;
    mWallTemperature = inputData.mWallTemperature;

    /// - Create the internal fluids.  We ignore the mInternalFluid base class pointer and use our
    ///   own fluid objects for clarity.
    if (mLiquidFluid) {
        mLiquidFluid->cleanup();
        TS_DELETE_OBJECT(mLiquidFluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mLiquidFluid, PolyFluid,
                           (*(mNodes[0]->getContent()), mName + ".mLiquidFluid"),
                           mName + ".mLiquidFluid");
    mLiquidFluid->resetState();
    mLiquidFluid->setMass(mNodes[0]->getContent()->find(configData.mLiquidType), 1.0);
    mLiquidFluid->updateMass();

    if (mGasFluid) {
        mGasFluid->cleanup();
        TS_DELETE_OBJECT(mGasFluid);
    }
    TS_NEW_PRIM_OBJECT_EXT(mGasFluid, PolyFluid,
                           (*(mNodes[0]->getContent()), mName + ".mGasFluid"),
                           mName + ".mGasFluid");
    mGasFluid->resetState();
    mGasFluid->setMass(mNodes[0]->getContent()->find(configData.mGasType), 1.0);
    mGasFluid->updateMass();

    /// - Initialize remaining state.
    mWallHeatFlux = 0.0;

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag          = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Configuration data.
/// @param[in]  inputData   (--)  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates this GUNNS Fluid Phase Change Conductor link model initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeConductor::validate(
        const GunnsFluidPhaseChangeConductorConfigData& configData,
        const GunnsFluidPhaseChangeConductorInputData&  inputData) const
{
    /// - Throw an exception on liquid type not in network.
    try {
        mNodes[0]->getOutflow()->find(configData.mLiquidType);
    } catch (const TsOutOfBoundsException&) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "liquid type is not in network.");
    }

    /// - Throw an exception on gas type not in network.
    try {
        mNodes[0]->getOutflow()->find(configData.mGasType);
    } catch (const TsOutOfBoundsException&) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "gas type is not in network.");
    }

    /// - Throw an exception if gas & liquid are not the same compound, i.e. don't have identical
    ///   molecular weights.
    const FluidProperties* gasProperties = mNodes[1]->getOutflow()->getProperties(configData.mGasType);
    const FluidProperties* liqProperties = mNodes[0]->getOutflow()->getProperties(configData.mLiquidType);
    if (gasProperties->getMWeight() != liqProperties->getMWeight()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "gas and liquid types have different molecular weights.");
    }

    /// - Throw an exception if the given gas & liquid types are not of the correct phase.
    if (FluidProperties::GAS != gasProperties->getPhase()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "gas fluid type is not gas phase.");
    }
    if (FluidProperties::LIQUID != liqProperties->getPhase()) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data", "liquid fluid type is not liquid phase.");
    }

    /// - Throw an exception if initial wall temperature < 0.
    if (inputData.mWallTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data", "initial wall temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @return   bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           this specific class.  These are:
///           - A GunnsFluidPhaseChangeConductor must map port 0 to a liquid or Ground node.
///           - A GunnsFluidPhaseChangeConductor must map port 1 to a gas or Ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidPhaseChangeConductor::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;
    GunnsFluidNode* nodePtr = dynamic_cast<GunnsFluidNode*> (mNodeList->mNodes);

    /// - Fail if port 0 is not the Ground node or in liquid phase.
    if ( (0 == port) and (node != getGroundNodeIndex()) and
         (FluidProperties::LIQUID != nodePtr[node].getContent()->getPhase()) ) {
        GUNNS_WARNING("aborted setting a port: must assign port 0 to Ground or a liquid node.");
        result = false;
    }

    /// - Fail if port 1 is not the Ground node or in gas phase.
    if ( (1 == port) and (node != getGroundNodeIndex()) and
         (FluidProperties::GAS != nodePtr[node].getContent()->getPhase()) ) {
        GUNNS_WARNING("aborted setting a port: must assign port 1 to Ground or a gas node.");
        result = false;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeConductor::restartModel()
{
    /// - Reset the base class.
    GunnsFluidConductor::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] forcedOutflow (--) not used.
/// @param[in] fromPort      (--) not used.
/// @param[in] toPort        (--) not used.
///
/// @details  This function adds the phase-change flow to the node's collection terms.
///           Flow is defined positive going from the Port 0 to Port 1, so a positive flow
///           will add mass to port 1 and remove from port 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidPhaseChangeConductor::transportFluid(const bool forcedOutflow __attribute__((unused)),
                                                    const int  fromPort __attribute__((unused)),
                                                    const int  toPort __attribute__((unused)))
{
    /// - Wall heat flux logic.
    double exitT = 0.0;
    double dH    = 0.0;
    if (mEfficiency != 0.0) {

        const FluidProperties* liqProperties = mNodes[0]->getContent()->getProperties(mLiquidType);
        const FluidProperties* gasProperties = mNodes[1]->getContent()->getProperties(mGasType);

        if (mFlowRate >= 0.0) {
            /// - Assuming the exit gas is saturated, find the saturation temperature at its current
            ///   pressure.
            exitT = gasProperties->getSaturationTemperature(mPotentialVector[1]);
            /// - Calculate change in enthalpy from inlet to saturated exit.  We can't just compare
            ///   specific enthalpies of the gas & liquid because our enthalpy of the gas is way
            ///   off, because of the h = Cp * T simplification in the fluid properties.  So we
            ///   must first calculate sensible heat of liquid change from inlet to saturated exit
            ///   T, subtracted from the fluid & added to to wall.
            dH    = liqProperties->getSpecificEnthalpy(mNodes[0]->getOutflow()->getTemperature())
                  - liqProperties->getSpecificEnthalpy(exitT);
            /// - Then add latent heat of vaporization at the saturated exit T, converted from
            ///   (kJ/kg) to (J/kg), subtracted from the fluid & added to the wall.
            dH   -= liqProperties->getHeatOfVaporization(exitT) * UnitConversion::UNIT_PER_KILO;
        } else {
            exitT = liqProperties->getSaturationTemperature(mPotentialVector[0]);
            dH    = gasProperties->getSpecificEnthalpy(mNodes[1]->getOutflow()->getTemperature())
                  - gasProperties->getSpecificEnthalpy(exitT);
            dH   += gasProperties->getHeatOfVaporization(exitT) * UnitConversion::UNIT_PER_KILO;
        }
    } else {
        if (mFlowRate >= 0.0) {
            exitT = mNodes[0]->getOutflow()->getTemperature();
        } else {
            exitT = mNodes[1]->getOutflow()->getTemperature();
        }
    }

    /// - Wall heat flux is scaled by efficiency.  Shut off wall heat flux if it is trending to
    ///   make the wall temperature overshoot the exit saturation temperature.
    double wallHeatFlux = mEfficiency * dH * fabs(mFlowRate);
    if ( ((wallHeatFlux < 0.0) and (mWallTemperature < exitT)) or
         ((wallHeatFlux > 0.0) and (mWallTemperature > exitT)) ) {
        wallHeatFlux = 0.0;
    }
    mWallHeatFlux = wallHeatFlux;

    /// - Transport phase change fluids to/from the nodes.  Only the specified gas & liquid types
    ///   are transported, so the internal mLiquidFluid & mGasFluid only have their one fluid type
    ///   at 100% mass fraction.  If the source node contents are not 100% this fluid type, then the
    ///   other constituents have their mass removed from the network and replaced with the phase
    ///   change type.
    if (mFlowRate > m100EpsilonLimit) {
        mNodes[0]->collectOutflux(mFlowRate);
        mGasFluid->setTemperature(exitT);
        mNodes[1]->collectInflux(mFlowRate, mGasFluid);

    } else if (mFlowRate < -m100EpsilonLimit) {
        mNodes[1]->collectOutflux(-mFlowRate);
        mLiquidFluid->setTemperature(exitT);
        mNodes[0]->collectInflux(-mFlowRate, mLiquidFluid);
    }
}
