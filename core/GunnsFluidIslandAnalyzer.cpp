/**
@file
@brief     GUNNS Fluid Island Analyzer Spotter implementation

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsNetworkSpotter.o)
   (aspects/fluid/fluid/PolyFluid.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsFluidIslandAnalyzer.hh"
#include "GunnsFluidNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Fluid Island Analyzer Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidIslandAnalyzerConfigData::GunnsFluidIslandAnalyzerConfigData(const std::string& name)
    :
    GunnsNetworkSpotterConfigData(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Island Analyzer Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidIslandAnalyzerConfigData::~GunnsFluidIslandAnalyzerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  postStepCounter  (--)  Initial counter to support unit tests.
///
/// @details  Default constructs this GUNNS Fluid Island Analyzer Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidIslandAnalyzerInputData::GunnsFluidIslandAnalyzerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Island Analyzer Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidIslandAnalyzerInputData::~GunnsFluidIslandAnalyzerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeList  (--)  Reference to the network node list.
///
/// @details Default constructs this GUNNS Fluid Island Analyzer Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidIslandAnalyzer::GunnsFluidIslandAnalyzer(GunnsNodeList& nodeList)
    :
    GunnsNetworkSpotter   (),
    mNodeList             (nodeList),
    mAttachedNode         (0),
    mIslandSize           (0),
    mIslandNodes          (0),
    mIslandVolume         (0.0),
    mIslandMass           (0.0),
    mIslandConstituentMass(0),
    mIslandEnergy         (0.0),
    mHiPressure           (0.0),
    mHiPressureNode       (0),
    mLoPressure           (0.0),
    mLoPressureNode       (0),
    mHiTemperature        (0.0),
    mHiTemperatureNode    (0),
    mLoTemperature        (0.0),
    mLoTemperatureNode    (0),
    mHiMoleFraction       (0),
    mHiMoleFractionNode   (0),
    mLoMoleFraction       (0),
    mLoMoleFractionNode   (0),
    mIslandTcMass         (0),
    mHiTcMoleFraction     (0),
    mHiTcMoleFractionNode (0),
    mLoTcMoleFraction     (0),
    mLoTcMoleFractionNode (0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Island Analyzer Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidIslandAnalyzer::~GunnsFluidIslandAnalyzer()
{
    TS_DELETE_ARRAY(mLoTcMoleFractionNode);
    TS_DELETE_ARRAY(mLoTcMoleFraction);
    TS_DELETE_ARRAY(mHiTcMoleFractionNode);
    TS_DELETE_ARRAY(mHiTcMoleFraction);
    TS_DELETE_ARRAY(mIslandTcMass);
    TS_DELETE_ARRAY(mLoMoleFractionNode);
    TS_DELETE_ARRAY(mLoMoleFraction);
    TS_DELETE_ARRAY(mHiMoleFractionNode);
    TS_DELETE_ARRAY(mHiMoleFraction);
    TS_DELETE_ARRAY(mIslandConstituentMass);
    TS_DELETE_ARRAY(mIslandNodes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Fluid Island Analyzer Spotter with its configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidIslandAnalyzer::initialize(const GunnsNetworkSpotterConfigData* configData,
                                          const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & type-cast config & input data.
    const GunnsFluidIslandAnalyzerConfigData* config = validateConfig(configData);
    validateInput(inputData);

    // This hack is to avoid a compile warning about config being unused in the macros below in a
    // non-Trick environment.
    (void)(sizeof(config->mName));

    /// - Allocate array for the island nodes.
    TS_NEW_PRIM_ARRAY_EXT(mIslandNodes,           mNodeList.mNumNodes, bool,   config->mName + ".mIslandNodes");

    /// - Allocate arrays for parameters related to fluid constituents.
    GunnsFluidNode*    fluidNodes      = static_cast<GunnsFluidNode*> (mNodeList.mNodes);
    const unsigned int numConstituents = fluidNodes[0].getFluidConfig()->mNTypes;
    TS_NEW_PRIM_ARRAY_EXT(mIslandConstituentMass, numConstituents,     double, config->mName + ".mIslandConstituentMass");
    TS_NEW_PRIM_ARRAY_EXT(mHiMoleFraction,        numConstituents,     double, config->mName + ".mHiMoleFraction");
    TS_NEW_PRIM_ARRAY_EXT(mHiMoleFractionNode,    numConstituents,     int,    config->mName + ".mHiMoleFractionNode");
    TS_NEW_PRIM_ARRAY_EXT(mLoMoleFraction,        numConstituents,     double, config->mName + ".mLoMoleFraction");
    TS_NEW_PRIM_ARRAY_EXT(mLoMoleFractionNode,    numConstituents,     int,    config->mName + ".mLoMoleFractionNode");

    /// - Allocate arrays for parameters related to trace compounds.
    if (fluidNodes[0].getFluidConfig()->mTraceCompounds) {
        const unsigned int numTc = fluidNodes[0].getFluidConfig()->mTraceCompounds->mNTypes;
        TS_NEW_PRIM_ARRAY_EXT(mIslandTcMass,         numTc, double, config->mName + ".mIslandTcMass");
        TS_NEW_PRIM_ARRAY_EXT(mHiTcMoleFraction,     numTc, double, config->mName + ".mHiTcMoleFraction");
        TS_NEW_PRIM_ARRAY_EXT(mHiTcMoleFractionNode, numTc, int,    config->mName + ".mHiTcMoleFractionNode");
        TS_NEW_PRIM_ARRAY_EXT(mLoTcMoleFraction,     numTc, double, config->mName + ".mLoTcMoleFraction");
        TS_NEW_PRIM_ARRAY_EXT(mLoTcMoleFractionNode, numTc, int,    config->mName + ".mLoTcMoleFractionNode");
    }

    mAttachedNode = -1;
    resetStateData();

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsFluidIslandAnalyzerConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsFluidIslandAnalyzerConfigData* GunnsFluidIslandAnalyzer::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsFluidIslandAnalyzerConfigData* result = dynamic_cast<const GunnsFluidIslandAnalyzerConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsFluidIslandAnalyzerInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsFluidIslandAnalyzerInputData* GunnsFluidIslandAnalyzer::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsFluidIslandAnalyzerInputData* result = dynamic_cast<const GunnsFluidIslandAnalyzerInputData*>(input);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Bad input data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (unused).
///
/// @details This method is empty because no pre-solver functionality is needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidIslandAnalyzer::stepPreSolver(const double dt __attribute__((unused)))
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Execution time step (unused).
///
/// @details Calls the analyze method to to perform its island analysis after the network solution.
///          Resets the state data each pass, then only analyzes the island if the attached node is
///          a valid non-ground node number.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidIslandAnalyzer::stepPostSolver(const double dt __attribute__((unused)))
{
    resetStateData();
    if (-1 < mAttachedNode and (mNodeList.mNumNodes - 1) > mAttachedNode) {
        analyze();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Clear & initialize state parameters prior to analyzing the island.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidIslandAnalyzer::resetStateData()
{
    mIslandSize        =  0;
    mIslandVolume      =  0.0;
    mIslandMass        =  0.0;
    mIslandEnergy      =  0.0;
    mHiPressure        =  0.0;
    mHiPressureNode    = -1;
    mLoPressure        =  0.0;
    mLoPressureNode    = -1;
    mHiTemperature     =  0.0;
    mHiTemperatureNode = -1;
    mLoTemperature     =  0.0;
    mLoTemperatureNode = -1;

    for (int node = 0; node < mNodeList.mNumNodes; ++node) {
        mIslandNodes[node] = false;
    }

    GunnsFluidNode*    fluidNodes      = static_cast<GunnsFluidNode*> (mNodeList.mNodes);
    const unsigned int numConstituents = fluidNodes[0].getFluidConfig()->mNTypes;
    for (unsigned int constituent = 0; constituent < numConstituents; ++constituent) {
        mIslandConstituentMass[constituent] =  0.0;
        mHiMoleFraction[constituent]        =  0.0;
        mHiMoleFractionNode[constituent]    = -1;
        mLoMoleFraction[constituent]        =  0.0;
        mLoMoleFractionNode[constituent]    = -1;
    }

    if (fluidNodes[0].getFluidConfig()->mTraceCompounds) {
        const unsigned int numTc = fluidNodes[0].getFluidConfig()->mTraceCompounds->mNTypes;
        for (unsigned int tc = 0; tc < numTc; ++tc) {
            mIslandTcMass[tc]         =  0.0;
            mHiTcMoleFraction[tc]     =  0.0;
            mHiTcMoleFractionNode[tc] = -1;
            mLoTcMoleFraction[tc]     =  0.0;
            mLoTcMoleFractionNode[tc] = -1;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Determines details about the attached island.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidIslandAnalyzer::analyze()
{
    /// - Get the attached node's island vector.
    GunnsFluidNode*         fluidNodes = static_cast<GunnsFluidNode*> (mNodeList.mNodes);
    const std::vector<int>* island     = fluidNodes[mAttachedNode].getIslandVector();

    if (island) {

        /// - Store the number of nodes in the island.
        mIslandSize = island->size();

        /// - Set search parameters to initial limits.
        mHiPressure        = -1.0E15;
        mLoPressure        =  1.0E15;
        mHiTemperature     = -1.0E15;
        mLoTemperature     =  1.0E15;

        const unsigned int numConstituents = fluidNodes[0].getFluidConfig()->mNTypes;

        for (unsigned int constituent = 0; constituent < numConstituents; ++constituent) {
            mHiMoleFraction[constituent]     = -1.0E15;
            mLoMoleFraction[constituent]     =  1.0E15;
        }

        if (fluidNodes[0].getFluidConfig()->mTraceCompounds) {
            const unsigned int numTc = fluidNodes[0].getFluidConfig()->mTraceCompounds->mNTypes;
            for (unsigned int tc = 0; tc < numTc; ++tc) {
                mHiTcMoleFraction[tc] = -1.0E15;
                mLoTcMoleFraction[tc] =  1.0E15;
            }
        }

        /// - Loop over the nodes in the island.
        for (int i = 0; i < mIslandSize; ++i) {
            const int        node  = (*island)[i];
            const PolyFluid* fluid = fluidNodes[node].getContent();

            /// - Indicate which nodes are in the island.
            mIslandNodes[node] = true;

            /// - Find high and low pressures.
            const double potential = fluid->getPressure();
            if (potential >= mHiPressure) {
                mHiPressure     = potential;
                mHiPressureNode = node;
            }
            if (potential <= mLoPressure) {
                mLoPressure     = potential;
                mLoPressureNode = node;
            }

            /// - Find high and low temperatures.
            const double temperature = fluid->getTemperature();
            if (temperature >= mHiTemperature) {
                mHiTemperature     = temperature;
                mHiTemperatureNode = node;
            }
            if (temperature <= mLoTemperature) {
                mLoTemperature     = temperature;
                mLoTemperatureNode = node;
            }

            /// - Find high and low concentrations of each fluid constituent.
            for (unsigned int constituent = 0; constituent < numConstituents; ++constituent) {
                mIslandConstituentMass[constituent] += fluid->getMass()
                                                     * fluid->getMassFraction(constituent);
                const double fraction = fluid->getMoleFraction(fluid->getType(constituent));
                if (fraction >= mHiMoleFraction[constituent]) {
                    mHiMoleFraction[constituent]     = fraction;
                    mHiMoleFractionNode[constituent] = node;
                }
                if (fraction <= mLoMoleFraction[constituent]) {
                    mLoMoleFraction[constituent]     = fraction;
                    mLoMoleFractionNode[constituent] = node;
                }
            }

            /// - Find trace compounds.
            if (fluidNodes[0].getFluidConfig()->mTraceCompounds) {
                const unsigned int numTc    = fluidNodes[0].getFluidConfig()->mTraceCompounds->mNTypes;
                const double*      tcMasses = fluid->getTraceCompounds()->getMasses();
                const double*      tcMoles  = fluid->getTraceCompounds()->getMoleFractions();
                for (unsigned int tc = 0; tc < numTc; ++tc) {
                    mIslandTcMass[tc] += tcMasses[tc];
                    if (tcMoles[tc] >= mHiTcMoleFraction[tc]) {
                        mHiTcMoleFraction[tc]     = tcMoles[tc];
                        mHiTcMoleFractionNode[tc] = node;
                    }
                    if (tcMoles[tc] <= mLoTcMoleFraction[tc]) {
                        mLoTcMoleFraction[tc]     = tcMoles[tc];
                        mLoTcMoleFractionNode[tc] = node;
                    }
                }
            }

            //TODO largest pressure correction, requires a new getter from nodes classes.

            /// - Accumulate island totals.
            mIslandVolume += fluidNodes[node].getVolume();
            mIslandMass   += fluid->getMass();
            mIslandEnergy += fluid->getMass() * fluid->getSpecificEnthalpy();
        }
    }
}
