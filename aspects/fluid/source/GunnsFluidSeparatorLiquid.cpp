/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Classes for the GUNNS Fluid Separator Liquid link model.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((core/GunnsFluidPotential.o))

PROGRAMMERS:
   ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-11))
**************************************************************************************************/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

#include "GunnsFluidSeparatorLiquid.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     name                 (--) Name of object.
/// @param[in,out] nodes                (--) Pointer to nodes.
/// @param[in]     maxConductivity      (m2) Maximum possible effective conductivity of the link.
/// @param[in]     expansionScaleFactor (--) Scale factor for isentropic cooling across the link.
/// @param[in]     liquidType           (--) Liquid phase fluid type of constituent being separated.
///
/// @details  Default constructs this GUNNS Fluid Separator Liquid link model configuration data
///           with arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorLiquidConfigData::GunnsFluidSeparatorLiquidConfigData(
                                              const std::string&               name,
                                              GunnsNodeList*                   nodes,
                                              const double                     maxConductivity,
                                              const double                     expansionScaleFactor,
                                              const FluidProperties::FluidType liquidType)
    :
    GunnsFluidPotentialConfigData(name, nodes, maxConductivity, expansionScaleFactor),
    mLiquidType(liquidType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that   --  Source to copy.
///
/// @details  Copy constructs this GUNNS Fluid Separator Liquid link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorLiquidConfigData::GunnsFluidSeparatorLiquidConfigData(
                                                    const GunnsFluidSeparatorLiquidConfigData& that)
    :
    GunnsFluidPotentialConfigData(that),
    mLiquidType(that.mLiquidType)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Separator Liquid link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorLiquidConfigData::~GunnsFluidSeparatorLiquidConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag    (--)  Blockage malfunction flag.
/// @param[in] malfBlockageValue   (--)  Blockage malfunction fractional value (0-1).
/// @param[in] sourcePressure      (kPa) Initial pressure rise of the link.
/// @param[in] transferTemperature (K)   Temperature of incoming liquid.
///
/// @details  Default constructs this GUNNS Fluid Separator Liquid link model input data with
///           arguments and obviously invalid default values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorLiquidInputData::GunnsFluidSeparatorLiquidInputData(
                                                                   const bool   malfBlockageFlag,
                                                                   const double malfBlockageValue,
                                                                   const double sourcePressure,
                                                                   const double transferTemperature)
    :
    GunnsFluidPotentialInputData(malfBlockageFlag, malfBlockageValue, sourcePressure),
    mTransferTemperature(transferTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    that  --   Source to copy from.
///
/// @details  Copy constructs this GUNNS Fluid Separator Liquid link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorLiquidInputData::GunnsFluidSeparatorLiquidInputData(
                                                     const GunnsFluidSeparatorLiquidInputData& that)
    :
    GunnsFluidPotentialInputData(that),
    mTransferTemperature(that.mTransferTemperature)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Separator Liquid link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorLiquidInputData::~GunnsFluidSeparatorLiquidInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @note     This should be followed by a call to the initialize method before calling an update
///           method.
///
/// @details  Default constructs this GUNNS Fluid Separator Liquid link model with obviously invalid
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorLiquid::GunnsFluidSeparatorLiquid()
    :
    GunnsFluidPotential(),
    mLiquidType(FluidProperties::NO_FLUID),
    mTransferTemperature(0.0),
    mLiquidIndex(-1)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Separator Liquid link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidSeparatorLiquid::~GunnsFluidSeparatorLiquid()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
/// @param[in,out]  links        --  Link vector.
/// @param[in]      port0        --  Nominal inlet port map index (vacuum node).
/// @param[in]      port1        --  Nominal outlet port map index.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Separator Liquid link model with configuration and input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorLiquid::initialize(const GunnsFluidSeparatorLiquidConfigData& configData,
                                           const GunnsFluidSeparatorLiquidInputData&  inputData,
                                           std::vector<GunnsBasicLink*>&              links,
                                           const int                                  port0,
                                           const int                                  port1)
{
    /// - First initialize & validate parent.
    GunnsFluidPotential::initialize(configData, inputData, links, port0, port1);

    /// - Reset initialization status flag.
    mInitFlag = false;

    /// - Validate configuration and input data.
    validate(configData, inputData);

    /// - Initialize derived attributes from configuration and input data.
    derive(configData, inputData);

    /// - Set initialization status flag to indicate successful initialization.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
///
/// @return   void
///
/// @throws   TsInitializationException
///
/// @details  Validates the initialization of this GUNNS Fluid Separator Liquid link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorLiquid::validate(const GunnsFluidSeparatorLiquidConfigData& configData,
                                         const GunnsFluidSeparatorLiquidInputData&  inputData) const
{
    /// - Throw an exception on incompatible gas/liquid types.
    /// @todo    Instead check for gas & liquid with same molecular weight.
    if (!(FluidProperties::GUNNS_WATER   == configData.mLiquidType ||
          FluidProperties::GUNNS_AMMONIA == configData.mLiquidType)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Incompatible gas/liquid types.");
    }

    /// - Throw an exception on liquid type not in PolyFluid.
    try {
        mNodes[1]->getContent()->find(configData.mLiquidType);
    } catch (const TsOutOfBoundsException& e){
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Liquid type not in fluid.");
    }

    /// - Throw an exception on transfer temperature < 0.
    if (inputData.mTransferTemperature < 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Transfer temperature < 0.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorLiquid::restartModel()
{
    /// - Reset the base class.
    GunnsFluidPotential::restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      configData   --  Configuration data.
/// @param[in]      inputData    --  Input data.
///
/// @return   void
///
/// @details  Initializes the attributes of this GUNNS Fluid Separator Liquid link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorLiquid::derive(const GunnsFluidSeparatorLiquidConfigData& configData,
                                       const GunnsFluidSeparatorLiquidInputData&  inputData)
{
    /// - Initialize from configuration data.
    mLiquidType          = configData.mLiquidType;
    mLiquidIndex         = mNodes[1]->getContent()->find(configData.mLiquidType);

    /// - Initialize from input data.
    mTransferTemperature = inputData.mTransferTemperature;

    /// - Create the internal fluid as 100% separated liquid.
    double fractions[FluidProperties::NO_FLUID] = {0.0};
    fractions[mLiquidIndex] = 1.0;

    PolyFluidInputData fluidInit(mTransferTemperature, // temperature
                                 101.32501,            // pressure
                                 0.0,                  // flowrate
                                 0.0,                  // mass
                                 fractions);           // massFraction
    createInternalFluid(fluidInit);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    dt         (s)    Time step.
/// @param[in]    flowrate   (kg/s) Mass flow rate (not used).
///
/// @return   void
///
/// @details  Updates the incoming fluid temperature based on inputs from the gas aspect.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorLiquid::updateFluid(const double dt       __attribute__((unused)),
                                            const double flowrate __attribute__((unused)))
{
    /// - Update the internal fluid using the (simbus input) liquid transfer temperature.
    mInternalFluid->setTemperature(mTransferTemperature);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Calculates the final link mass flow rate for output to the gas aspect via simbus.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidSeparatorLiquid::processOutputs()
{
    mFlowRate = mFlux * mInternalFluid->getMWeight();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    port  --  The port to be assigned.
/// @param[in]    node  --  The desired node to assign the port to.
///
/// @returns  -- True if all rules checks pass.
///
/// @details  Checks the requested port & node arguments for validity against rules that apply to
///           Gunns Fluid Separator Liquid link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidSeparatorLiquid::checkSpecificPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Fail if port 1 is the vacuum boundary node.
    if ((1 == port) && (node == mNodeList->mNumNodes - 1)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: cannot assign port 1 to the boundary node.";
        hsSendMsg(msg);
        result = false;
    }

    /// - Fail if port 0 is not the vacuum boundary node.
    else if ((0 == port) && (node != mNodeList->mNumNodes - 1)) {
        TsHsMsg msg(TS_HS_WARNING, "GUNNS");
        msg << mName << " aborted setting a port: must assign port 0 to the boundary node.";
        hsSendMsg(msg);
        result = false;
    }

    return result;
}
