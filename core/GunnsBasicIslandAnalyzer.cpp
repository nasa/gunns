/**
@file
@brief     GUNNS Basic Island Analyzer Spotter implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsNetworkSpotter.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsInitializationException.o))
*/

#include "GunnsBasicIslandAnalyzer.hh"
#include "GunnsBasicNode.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Instance name for self-identification in messages.
///
/// @details  Default constructs this GUNNS Basic Island Analyzer Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicIslandAnalyzerConfigData::GunnsBasicIslandAnalyzerConfigData(const std::string& name)
    :
    GunnsNetworkSpotterConfigData(name)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Island Analyzer Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicIslandAnalyzerConfigData::~GunnsBasicIslandAnalyzerConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  postStepCounter  (--)  Initial counter to support unit tests.
///
/// @details  Default constructs this GUNNS Basic Island Analyzer Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicIslandAnalyzerInputData::GunnsBasicIslandAnalyzerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Island Analyzer Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicIslandAnalyzerInputData::~GunnsBasicIslandAnalyzerInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeList  (--)  Reference to the network node list.
///
/// @details Default constructs this GUNNS Basic Island Analyzer Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicIslandAnalyzer::GunnsBasicIslandAnalyzer(GunnsNodeList& nodeList)
    :
    GunnsNetworkSpotter(),
    mNodeList          (nodeList),
    mAttachedNode      (0),
    mIslandSize        (0),
    mIslandNodes       (0),
    mIslandNetFlux     (0.0),
    mHiPotential       (0.0),
    mHiPotentialNode   (0),
    mLoPotential       (0.0),
    mLoPotentialNode   (0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Basic Island Analyzer Spotter.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicIslandAnalyzer::~GunnsBasicIslandAnalyzer()
{
    TS_DELETE_ARRAY(mIslandNodes);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
/// @param[in]  inputData   (--)  Instance input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Basic Island Analyzer Spotter with its configuration and
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicIslandAnalyzer::initialize(const GunnsNetworkSpotterConfigData* configData,
                                          const GunnsNetworkSpotterInputData*  inputData)
{
    /// - Initialize the base class.
    GunnsNetworkSpotter::initialize(configData, inputData);

    /// - Reset the init flag.
    mInitFlag = false;

    /// - Validate & type-cast config & input data.
    const GunnsBasicIslandAnalyzerConfigData* config = validateConfig(configData);
    validateInput(inputData);

    // This hack is to avoid a compile warning about config being unused in the macro below in a
    // non-Trick environment.
    (void)(sizeof(config->mName));

    /// - Allocate array for the island nodes.
    TS_NEW_PRIM_ARRAY_EXT(mIslandNodes, mNodeList.mNumNodes, bool, config->mName + ".mIslandNodes");

    mAttachedNode = -1;
    resetStateData();

    /// - Set the init flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  configData  (--)  Instance configuration data.
///
/// @returns  GunnsBasicIslandAnalyzerConfigData (--) Type-casted and validated config data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base config data class pointer to this spotter's config data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsBasicIslandAnalyzerConfigData* GunnsBasicIslandAnalyzer::validateConfig(const GunnsNetworkSpotterConfigData* config)
{
    const GunnsBasicIslandAnalyzerConfigData* result = dynamic_cast<const GunnsBasicIslandAnalyzerConfigData*>(config);
    if (!result) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Bad config data pointer type.");
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputData  (--)  Instance input data.
///
/// @returns  GunnsBasicIslandAnalyzerInputData (--) Type-casted and validated input data pointer.
///
/// @throws   TsInitializationException
///
/// @details  Type-casts the base input data class pointer to this spotter's input data type,
///           checks for valid type-cast and validates contained data.
////////////////////////////////////////////////////////////////////////////////////////////////////
const GunnsBasicIslandAnalyzerInputData* GunnsBasicIslandAnalyzer::validateInput(const GunnsNetworkSpotterInputData* input)
{
    const GunnsBasicIslandAnalyzerInputData* result = dynamic_cast<const GunnsBasicIslandAnalyzerInputData*>(input);
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
void GunnsBasicIslandAnalyzer::stepPreSolver(const double dt __attribute__((unused)))
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
void GunnsBasicIslandAnalyzer::stepPostSolver(const double dt __attribute__((unused)))
{
    resetStateData();
    if (-1 < mAttachedNode and (mNodeList.mNumNodes - 1) > mAttachedNode) {
        analyze();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Clear & initialize state parameters prior to analyzing the island.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicIslandAnalyzer::resetStateData()
{
    mIslandSize        =  0;
    mIslandNetFlux     =  0.0;
    mHiPotential       =  0.0;
    mHiPotentialNode   = -1;
    mLoPotential       =  0.0;
    mLoPotentialNode   = -1;

    for (int node = 0; node < mNodeList.mNumNodes; ++node) {
        mIslandNodes[node] = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Determines details about the attached island.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicIslandAnalyzer::analyze()
{
    /// - Get the attached node's island vector.
    GunnsBasicNode*         basicNodes = static_cast<GunnsBasicNode*> (mNodeList.mNodes);
    const std::vector<int>* island     = basicNodes[mAttachedNode].getIslandVector();

    if (island) {

        /// - Store the number of nodes in the island.
        mIslandSize = static_cast<int>(island->size());

        /// - Set search parameters to initial limits.
        mHiPotential       = -1.0E15;
        mLoPotential       =  1.0E15;

        /// - Loop over the nodes in the island.
        for (int i = 0; i < mIslandSize; ++i) {
            const int        node  = (*island)[i];

            /// - Indicate which nodes are in the island.
            mIslandNodes[node] = true;

            /// - Sum the island net flux.
            mIslandNetFlux += basicNodes[node].getNetFlux();

            /// - Find high and low Potentials.
            const double potential = basicNodes[node].getPotential();
            if (potential >= mHiPotential) {
                mHiPotential     = potential;
                mHiPotentialNode = node;
            }
            if (potential <= mLoPotential) {
                mLoPotential     = potential;
                mLoPotentialNode = node;
            }
        }
    }
}
