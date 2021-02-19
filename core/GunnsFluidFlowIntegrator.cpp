/**
@file
@brief     GUNNS Fluid Flow Integrator Spotter implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsNetworkSpotter.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsFluidFlowIntegrator.hh"
#include "GunnsFluidNode.hh"
#include "GunnsFluidLink.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Fluid Flow Integrator Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowIntegratorConfigData::GunnsFluidFlowIntegratorConfigData(const std::string& name)
    :
    GunnsNetworkSpotterConfigData(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Flow Integrator Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowIntegratorConfigData::~GunnsFluidFlowIntegratorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  postStepCounter  (--)  Initial counter to support unit tests.
///
/// @details  Default constructs this GUNNS Fluid Flow Integrator Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowIntegratorInputData::GunnsFluidFlowIntegratorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Flow Integrator Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowIntegratorInputData::~GunnsFluidFlowIntegratorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeList  (--)  Reference to the network node list.
///
/// @details Default constructs this GUNNS Fluid Flow Integrator Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowIntegrator::GunnsFluidFlowIntegrator(const GunnsFluidLink& link)
    :
    GunnsNetworkSpotter      (),
    mLink                    (link),
    mNumFluidConstituents    (0),
    mMassFractionsPort0      (0),
    mMassFractionsPort1      (0),
    mIntegratedMasses        (0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Flow Integrator Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowIntegrator::~GunnsFluidFlowIntegrator()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory objects
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowIntegrator::cleanup()
{
    TS_DELETE_ARRAY(mIntegratedMasses);
    TS_DELETE_ARRAY(mMassFractionsPort1);
    TS_DELETE_ARRAY(mMassFractionsPort0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Flow Integrator Spotter with its configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowIntegrator::initialize(const GunnsNetworkSpotterConfigData* configData,
                                          const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate config & input data.
    validateConfig(configData);
    validateInput(inputData);

    mNumFluidConstituents = mLink.getNodeContent(0)->getNConstituents();
    TS_NEW_PRIM_ARRAY_EXT(mMassFractionsPort0,      mNumFluidConstituents, double, configData->mName + ".mMassFractionsPort0");
    TS_NEW_PRIM_ARRAY_EXT(mMassFractionsPort1,      mNumFluidConstituents, double, configData->mName + ".mMassFractionsPort1");
    TS_NEW_PRIM_ARRAY_EXT(mIntegratedMasses,        mNumFluidConstituents, double, configData->mName + ".mIntegratedMasses");

    // Initialize the arrays out to keep Valgrind happy.
    for (int fluidIndex = 0; fluidIndex < mNumFluidConstituents; ++fluidIndex) {
        mMassFractionsPort0[fluidIndex] = 0.0;
        mMassFractionsPort1[fluidIndex] = 0.0;
        mIntegratedMasses[fluidIndex] = 0.0;
    }

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsFluidFlowIntegratorConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsFluidFlowIntegratorConfigData* GunnsFluidFlowIntegrator::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsFluidFlowIntegratorConfigData* result = dynamic_cast<const GunnsFluidFlowIntegratorConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsFluidFlowIntegratorInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsFluidFlowIntegratorInputData* GunnsFluidFlowIntegrator::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsFluidFlowIntegratorInputData* result = dynamic_cast<const GunnsFluidFlowIntegratorInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step.
///
/// @details  Stores the link's node's content's mass fractions for integrating post-solution.  We
///           integrate based on the supplying node's contents, not the link's internal fluid, so
///           that we can use this on links that don't instantiate an internal fluid.  We must store
///           both node's contents because we don't yet know which direction the link will flow.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowIntegrator::stepPreSolver(const double dt __attribute__((unused)))
{
    collectMassFractions();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step.
///
/// @details  Integrate the constituent masses flowed through the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowIntegrator::stepPostSolver(const double dt)
{
    /// - Determine the correct mass fractions to use in the integration. This is based on the
    ///   sign of the flow rate.
    double flowRate = mLink.getFlowRate();
    if(flowRate > 0.0){
        performIntegration(mMassFractionsPort0, flowRate, dt);
    }else{
        performIntegration(mMassFractionsPort1, flowRate, dt);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Helper method to collect the fluid mass fractions out of the link's port 0 and port 1
///          nodes. Called by the stepPreSolver method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowIntegrator::collectMassFractions()
{
    for(int fluidIndex = 0; fluidIndex < mNumFluidConstituents; fluidIndex++){
        mMassFractionsPort0[fluidIndex] = mLink.getNodeContent(0)->getMassFraction(fluidIndex);
        mMassFractionsPort1[fluidIndex] = mLink.getNodeContent(1)->getMassFraction(fluidIndex);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Helper method to assign the mass fractions that are used in the integration of the
///          flow rate. Called in the stepPostSolver method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowIntegrator::performIntegration(double* fractionsToUse, double flowRate, double dt)
{
    for(int fluidIndex = 0; fluidIndex < mNumFluidConstituents; fluidIndex++){
        mIntegratedMasses[fluidIndex] += fractionsToUse[fluidIndex] * flowRate * dt;
    }
}
