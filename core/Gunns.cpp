/**
@file
@brief    GUNNS Orchestrator implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (See Gunns.hh)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   (
    (core/GunnsBasicLink.o)
    (core/GunnsFluidNode.o)
    (core/GunnsInfraFunctions.o)
    (core/GunnsFluidFlowOrchestrator.o)
    (core/GunnsMinorStepLog.o)
    (math/linear_algebra/Sor.o)
    (math/linear_algebra/CholeskyLdu.o)
#ifdef GUNNS_CUDA_ENABLE
    (math/linear_algebra/cuda/CudaDenseDecomp.o)
    (math/linear_algebra/cuda/CudaSparseSolve.o)
#endif
    (software/exceptions/TsInitializationException.o)
    (software/exceptions/TsOutOfBoundsException.o)
    (software/exceptions/TsNumericalException.o)
   )

 PROGRAMMERS:
   (
    (Jason Harvey) (L-3) (Design) (April 2011)
    (Kevin Supak)  (L-3) (Design) (April 2011)
    (Teems Lovett) (L-3) (Design) (April 2011)
   )
*/

#include <cmath>
#include <cfloat>
#include <cstdio>

#include "core/Gunns.hh"
#include "core/GunnsBasicLink.hh"
#include "core/GunnsFluidNode.hh"
#include "core/GunnsInfraFunctions.hh"
#include "core/GunnsFluidFlowOrchestrator.hh"
#include "math/linear_algebra/Sor.hh"
#include "math/linear_algebra/CholeskyLdu.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsNumericalException.hh"

#ifdef GUNNS_CUDA_ENABLE
#include "math/linear_algebra/cuda/CudaDenseDecomp.hh"
#include "math/linear_algebra/cuda/CudaSparseSolve.hh"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name                 (--) Name of the Gunns object for fault messaging
/// @param[in] convergenceTolerance (--) Error tolerance for minor step convergence
/// @param[in] minLinearizationP    (--) Minimum potential for linearization in link admittances
/// @param[in] minorStepLimit       (--) Maximum allowed minor steps per major step
/// @param[in] decompositionLimit   (--) Maximum allowed matrix decompositions per major step
///
/// @details  Default constructs this Gunns configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsConfigData::GunnsConfigData(const std::string& name,
                                 const double       convergenceTolerance,
                                 const double       minLinearizationP,
                                 const unsigned int minorStepLimit,
                                 const unsigned int decompositionLimit)
    :
    mName                     (name),
    mConvergenceTolerance     (convergenceTolerance),
    mMinLinearizationPotential(minLinearizationP),
    mMinorStepLimit           (minorStepLimit),
    mDecompositionLimit       (decompositionLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Input source to copy
///
/// @details  Copy constructs this Gunns configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsConfigData::GunnsConfigData(const GunnsConfigData& that)
    :
    mName                     (that.mName),
    mConvergenceTolerance     (that.mConvergenceTolerance),
    mMinLinearizationPotential(that.mMinLinearizationPotential),
    mMinorStepLimit           (that.mMinorStepLimit),
    mDecompositionLimit       (that.mDecompositionLimit)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Gunns configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsConfigData::~GunnsConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Gunns object.
////////////////////////////////////////////////////////////////////////////////////////////////////
Gunns::Gunns()
    :
    mStepLog               (),
    mName                  (),
    mNumLinks              (0),
    mNumNodes              (0),
    mLinks                 (0),
    mNodes                 (0),
    mFlowOrchestrator      (0),
    mOwnsFlowOrchestrator  (false),
    mAdmittanceMatrix      (0),
    mAdmittanceMatrixIsland(0),
    mSourceVector          (0),
    mSourceVectorIsland    (0),
    mPotentialVector       (0),
    mPotentialVectorIsland (0),
    mMinorPotentialVector  (0),
    mMajorPotentialVector  (0),
    mSlavePotentialVector  (0),
    mNetCapDeltaPotential  (0),
    mIslandVectors         (),
    mNodeIslandNumbers     (0),
    mIslandCount           (0),
    mIslandMaxSize         (0),
    mIslandMode            (OFF),
    mSolverCpu             (0),
    mSolverGpuDense        (0),
    mSolverGpuSparse       (0),
    mGpuEnabled            (false),
    mGpuMode               (NO_GPU),
    mGpuSizeThreshold      (9999999),
    mConvergenceTolerance  (1.0),
    mNetworkSize           (0),
    mMinorStepLimit        (1),
    mDecompositionLimit    (1),
    mRebuild               (false),
    mLinearNetwork         (true),
    mSolverMode            (NORMAL),
    mRunMode               (RUN),
    mInitAttempted         (false),
    mInitFlag              (false),
    mLinkAdmittanceMatrices(0),
    mLinkPotentialVectors  (0),
    mLinkSourceVectors     (0),
    mLinkOverrideVectors   (0),
    mLinkNodeMaps          (0),
    mLinkNumPorts          (0),
    mLinkAdmittanceMaps    (0),
    mMajorStepCount        (0),
    mConvergenceFailCount  (0),
    mLinkResetStepFailCount(0),
    mMinorStepCount        (0),
    mMaxMinorStepCount     (0),
    mLastMinorStep         (0),
    mAvgMinorStepCount     (0.0),
    mDecompositionCount    (0),
    mMaxDecompositionCount (0),
    mLastDecomposition     (0),
    mAvgDecompositionCount (0.0),
    mSolveTimeWorking      (0.0),
    mSolveTime             (0.0),
    mStepTime              (0.0),
    mWorstCaseTiming       (false),
    mLinksConvergence      (0),
    mNodesConvergence      (0),
    mDebugSavedSlice       (0),
    mDebugSavedNode        (0),
    mDebugDesiredSlice     (0),
    mDebugDesiredStep      (0),
    mDebugDesiredNode      (-1),
    mVerbose               (false),
    mSorActive             (false),
    mSorWeight             (1.0),
    mSorMaxIter            (100),
    mSorTolerance          (1.0E-12),
    mSorLastIteration      (-1),
    mSorFailCount          (0),
    mLastSolverMode        (NORMAL),
    mLastIslandMode        (OFF),
    mLastRunMode           (RUN)
{
#ifdef GUNNS_CUDA_ENABLE
    mGpuEnabled      = true;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Gunns object.
////////////////////////////////////////////////////////////////////////////////////////////////////
Gunns::~Gunns()
{
    cleanup();
    {
        if (mFlowOrchestrator and mOwnsFlowOrchestrator) {
            delete mFlowOrchestrator;
            mFlowOrchestrator     = 0;
            mOwnsFlowOrchestrator = false;
        }
    } {
        delete [] mNodes;
        mNodes = 0;
    }
    if (mSolverGpuSparse) {
        delete mSolverGpuSparse;
        mSolverGpuSparse = 0;
    }
    if (mSolverGpuDense) {
        delete mSolverGpuDense;
        mSolverGpuDense = 0;
    } {
        delete mSolverCpu;
        mSolverCpu = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::cleanup()
{
    {
        delete [] mLinkNumPorts;
        mLinkNumPorts = 0;
    } {
        delete [] mLinkAdmittanceMaps;
        mLinkAdmittanceMaps = 0;
    } {
        delete [] mLinkNodeMaps;
        mLinkNodeMaps = 0;
    } {
        delete [] mLinkOverrideVectors;
        mLinkOverrideVectors = 0;
    } {
        delete [] mLinkSourceVectors;
        mLinkSourceVectors = 0;
    } {
        delete [] mLinkPotentialVectors;
        mLinkPotentialVectors = 0;
    } {
        delete [] mLinkAdmittanceMatrices;
        mLinkAdmittanceMatrices = 0;
    }
    TS_DELETE_ARRAY(mLinksConvergence);
    TS_DELETE_ARRAY(mNodesConvergence);
    {
        delete [] mLinks;
        mLinks = 0;
    }
    TS_DELETE_ARRAY(mDebugSavedNode);
    TS_DELETE_ARRAY(mDebugSavedSlice);
    TS_DELETE_ARRAY(mNodeIslandNumbers);
    TS_DELETE_ARRAY(mNetCapDeltaPotential);
    TS_DELETE_ARRAY(mSlavePotentialVector);
    TS_DELETE_ARRAY(mMajorPotentialVector);
    TS_DELETE_ARRAY(mMinorPotentialVector);
    TS_DELETE_ARRAY(mPotentialVector);
    TS_DELETE_ARRAY(mSourceVector);
    {
        delete [] mPotentialVectorIsland;
        mPotentialVectorIsland = 0;
    } {
        delete [] mSourceVectorIsland;
        mSourceVectorIsland = 0;
    } {
        delete [] mAdmittanceMatrixIsland;
        mAdmittanceMatrixIsland = 0;
    } {
        delete [] mAdmittanceMatrix;
        mAdmittanceMatrix = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  mode       (--)  GpuMode enumeration value to take.
/// @param[in]  threshold  (--)  Network/island size threshold value to take.
///
/// @details  Rejects any GPU modes and outputs an H&S warning if the compilation is not GPU
///           enabled.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::setGpuOptions(const GpuMode mode, const int threshold)
{
    mGpuSizeThreshold = threshold;
    if (NO_GPU != mode and not mGpuEnabled) {
        mGpuMode = NO_GPU;
        GUNNS_WARNING("GPU mode rejected because the solver compilation is not GPU compatible.");
    } else {
        mGpuMode = mode;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData  (--) Input configuration data
/// @param[in,out] linksVector (--) Input network links vector
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Gunns object with configuration data, allocates memory for the system
///           vectors, hooks up the link objects to the system vectors, calls validation checks on
///           nodes & links, and distributes initial node potentials to all affected links.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::initialize(const GunnsConfigData& configData, std::vector<GunnsBasicLink*>& linksVector)
{
    mInitFlag = false;

    /// - Issue an error if initialization is attempted more than once.
    if (mInitAttempted) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Initialization attempted more than once.");
    }
    mInitAttempted = true;

    cleanup();

    /// - Validate and load the Config data.  The mMinLinearizationPotential is handled below.  The
    ///   last node is always assumed to be the ground or vacuum node, and is not counted in the
    ///   size of the system.  We validate the config data last because it uses the name in the
    ///   error messages, so the name has to be loaded in first.
    GUNNS_NAME_ERREX("Gunns", configData.mName);
    mConvergenceTolerance = configData.mConvergenceTolerance;
    mMinorStepLimit       = configData.mMinorStepLimit;
    mDecompositionLimit   = configData.mDecompositionLimit;
    mNetworkSize          = mNumNodes - 1;
    validateConfigData(configData);

    /// - Allocate linear algebra solvers.
    mSolverCpu = new CholeskyLdu();
#ifdef GUNNS_CUDA_ENABLE
    mSolverGpuDense  = new CudaDenseDecomp();
    mSolverGpuSparse = new CudaSparseSolve();
#endif

    /// - Allocate system arrays based on network size.
    const int matrixSize = mNetworkSize * mNetworkSize;
    mAdmittanceMatrix       = new double[matrixSize];
    mAdmittanceMatrixIsland = new double[matrixSize];
    mSourceVectorIsland     = new double[mNetworkSize];
    mPotentialVectorIsland  = new double[mNetworkSize];
    TS_NEW_PRIM_ARRAY_EXT(mSourceVector,         mNetworkSize,       double, configData.mName + ".mSourceVector");
    TS_NEW_PRIM_ARRAY_EXT(mPotentialVector,      mNetworkSize,       double, configData.mName + ".mPotentialVector");
    TS_NEW_PRIM_ARRAY_EXT(mMinorPotentialVector, mNetworkSize,       double, configData.mName + ".mMinorPotentialVector");
    TS_NEW_PRIM_ARRAY_EXT(mMajorPotentialVector, mNetworkSize,       double, configData.mName + ".mMajorPotentialVector");
    TS_NEW_PRIM_ARRAY_EXT(mSlavePotentialVector, mNetworkSize,       double, configData.mName + ".mSlavePotentialVector");
    TS_NEW_PRIM_ARRAY_EXT(mNetCapDeltaPotential, matrixSize,         double, configData.mName + ".mNetCapDeltaPotential");
    TS_NEW_PRIM_ARRAY_EXT(mNodeIslandNumbers,    mNetworkSize,       int,    configData.mName + ".mNodeIslandNumbers");
    TS_NEW_PRIM_ARRAY_EXT(mDebugSavedSlice,      mNetworkSize,       double, configData.mName + ".mDebugSavedSlice");
    TS_NEW_PRIM_ARRAY_EXT(mDebugSavedNode,      (mMinorStepLimit+1), double, configData.mName + ".mDebugSavedNode");

    /// - Clear initial garbage values out of allocated memory.
    for (int i = 0; i < mNetworkSize; ++i) {
        mSourceVector[i]          = 0.0;
        mSourceVectorIsland[i]    = 0.0;
        mPotentialVector[i]       = 0.0;
        mPotentialVectorIsland[i] = 0.0;
        mMinorPotentialVector[i]  = 0.0;
        mMajorPotentialVector[i]  = 0.0;
        mSlavePotentialVector[i]  = 0.0;
        mNodeIslandNumbers[i]     = i;
        mDebugSavedSlice[i]       = 0.0;

        /// - Pre-load the 2D island vectors' 1st dimension with vectors of ints, one for each row
        ///   in the matrix - so that we don't have to keep pushing & popping them during runtime.
        std::vector<int> v;
        mIslandVectors.push_back(v);
    }
    for (int i = 0; i < mNetworkSize*mNetworkSize; ++i) {
        mAdmittanceMatrix[i]       = 0.0;
        mAdmittanceMatrixIsland[i] = 0.0;
        mNetCapDeltaPotential[i]   = 0.0;
    }
    clearDebugNode();

    mNumLinks              = linksVector.size();
    mDebugDesiredSlice     = 0;
    mDebugDesiredStep      = 0;
    mDebugDesiredNode      = -1;

    /// - Allocate a variable size array of pointers to the network links.
    mLinks = new GunnsBasicLink*[mNumLinks];

    /// - Set up the network links.
    for (int link = 0; link < mNumLinks; ++link) {
        mLinks[link] = linksVector[link];

        /// - Pass network information to each link in the network.
        mLinks[link]->setMinLinearizationPotential(configData.mMinLinearizationPotential);

        /// - Flag this network as non-linear if any link object is non-linear.
        if (mLinks[link]->isNonLinear()) {
            mLinearNetwork = false;
        }
    }

    /// - Allocate arrays to store link & node convergence info for non-linear networks.
    if (not mLinearNetwork) {
        TS_NEW_PRIM_ARRAY_EXT(mNodesConvergence, mNetworkSize, double, configData.mName + ".mNodesConvergence");
        for (int node = 0; node < mNetworkSize; ++node) {
            mNodesConvergence[node] = 0.0;
        }

        TS_NEW_PRIM_ARRAY_EXT(mLinksConvergence, mNumLinks, GunnsBasicLink::SolutionResult, configData.mName + ".mLinksConvergence");
        for (int link = 0; link < mNumLinks; ++link) {
            mLinksConvergence[link] = GunnsBasicLink::CONFIRM;
        }
    }

    verifyLinkInitialization();

    mLinkAdmittanceMatrices = new double*[mNumLinks];
    mLinkPotentialVectors   = new double*[mNumLinks];
    mLinkSourceVectors      = new double*[mNumLinks];
    mLinkOverrideVectors    = new bool*  [mNumLinks];
    mLinkNodeMaps           = new int*   [mNumLinks];
    mLinkAdmittanceMaps     = new GunnsBasicLinkAdmittanceMap*[mNumLinks];
    mLinkNumPorts           = new int    [mNumLinks];

    /// - Prepare nodes for startup, and load in their initial potentials for distribution to all
    ///   incident links.
    for (int node = 0; node < mNetworkSize; ++node) {
        mNodes[node]->setIslandVector(&mIslandVectors[node]);
        mNodes[node]->prepareForStart();
        mPotentialVector[node] = mNodes[node]->getPotential();
    }

    /// - Load the system vectors with pointers to the link object's parameters.
    for (int link = 0; link < mNumLinks; ++link) {
        mLinkAdmittanceMatrices[link] = mLinks[link]->getAdmittanceMatrix();
        mLinkPotentialVectors[link]   = mLinks[link]->getPotentialVector();
        mLinkSourceVectors[link]      = mLinks[link]->getSourceVector();
        mLinkOverrideVectors[link]    = mLinks[link]->getOverrideVector();
        mLinkNodeMaps[link]           = mLinks[link]->getNodeMap();
        mLinkAdmittanceMaps[link]     = mLinks[link]->getAdmittanceMap();
        mLinkNumPorts[link]           = mLinks[link]->getNumberPorts();
    }

    /// - Point the nodes to their network capacitance delta-potentials array.
    for (int node = 0; node < mNetworkSize; ++node) {
        mNodes[node]->setNetCapDeltaPotential(&mNetCapDeltaPotential[node*mNetworkSize]);
    }

    verifyNodeInitialization();
    mFlowOrchestrator->initialize(mName + ".mFlowOrchestrator", mLinks, mNodes,
                                  mLinkNodeMaps, mLinkNumPorts);

    /// - Zero the potential and reset the state of the vacuum/ground boundary node.  This node is
    ///   always the last node in the list.
    mNodes[mNumNodes-1]->resetContentState();

    /// - Output the initialized potential vector to all links, ensure admittance matrix is built on
    ///   the first pass after init, and flag this network as having successfully completed
    ///   initialization.
    outputPotentialVector();
    saveMinorPotentialVector();
    saveMajorPotentialVector();

    /// - Call the links to process special write data to the sim bus.
    for (int link = 0; link < mNumLinks; ++link) {
        mLinks[link]->processOutputs();
    }

    /// - Initialize the minor step log.
    mStepLog.initialize(mName + ".mStepLog", mNetworkSize, mNumLinks, mLinks);

    /// - Perform functions common to initialization and restart.
    initializeRestartCommonFunctions();

    GUNNS_INFO("initialized with " << mNumLinks << " links, " << mNumNodes << " nodes, " << listAllModes());

    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Input configuration data
///
/// @throws   TsInitializationException
///
/// @details  This method checks all config data for valid range.  All members of the
///           GunnsConfigData class have default values that are invalid, to force the user to
///           explicitly define their desired values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::validateConfigData(const GunnsConfigData& configData)
{
    /// - Issue an error if convergence tolerance <= 0.
    if (configData.mConvergenceTolerance <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Convergence tolerance is too small (<= 0.0).");
    }

    /// - Issue an error if minimum linearization potential <= 0.
    if (configData.mMinLinearizationPotential <= 0.0) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Minimum linearization potential is too small (<= 0.0).");
    }

    /// - Issue an error if minor step limit < 1.
    if (configData.mMinorStepLimit < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Minor step limit is too small (<1).");
    }

    /// - Issue an error if decomposition limit < 1.
    if (configData.mDecompositionLimit < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Decomposition limit is too small (<1).");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  This method checks all links for proper initialization, and makes sure they are not
///           duplicated in the network.  Exception is thrown on any fault.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::verifyLinkInitialization()
{
    for (int link = 0; link < mNumLinks; ++link) {

        /// - Issue an error if this link has not been properly initialized.
        if (!mLinks[link]->isInitialized()) {
            std::ostringstream msg;
            msg << "Link " << mLinks[link]->getName() << " has not been initialized.";
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", msg.str());
        }

        /// - Issue an error if this link is present more than once in the link vector.
        for (int otherLink = 0; otherLink < link; ++otherLink) {

            if (mLinks[otherLink] == mLinks[link]) {
                std::ostringstream msg;
                msg << "Link " << mLinks[link]->getName() << " has been added more than once.";
                GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", msg.str());
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  This method checks all nodes for proper initialization.  Exception is thrown on any
///           fault.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::verifyNodeInitialization()
{
    /// - Issue an error if network size < 1.  A network must include at least one regular node.
    if (mNetworkSize < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Number of non-ground nodes is too small (<1).");
    }

    /// - Validate all nodes and issue an error if any fail their validation checks.  If
    ///   a node fails to validate it will Issue an error, so we need to try & catch.  For
    ///   consistency with the rest of Gunns, we still ask the node for its init flag.
    for (int node = 0; node < mNumNodes; ++node) {

        try {
            mNodes[node]->validate();
        } catch (...) {}

        if (!mNodes[node]->isInitialized()) {
            std::ostringstream msg;
            msg << "Node " << node << " has not been initialized.";
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", msg.str());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] nodeList    (--) Input nodes structure
///
/// @throws   TsInitializationException
///
/// @note     This method must be called before any link initialization and before Gunns::initialize
///
/// @details  This method initializes the fluid nodes array for fluid networks.  Only one call to
///           initializeFluidNodes or initializeNodes is allowed, otherwise we throw exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::initializeFluidNodes(GunnsNodeList& nodeList)
{
    if (mNodes or mFlowOrchestrator) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Node initialization attempted more than once.");
    }

    /// - Load the fluid nodes array
    mNumNodes = nodeList.mNumNodes;
    mNodes = new GunnsBasicNode*[mNumNodes];

    GunnsFluidNode* fluidNodes = static_cast<GunnsFluidNode*> (nodeList.mNodes);

    for (int node = 0; node < mNumNodes; ++node) {
        mNodes[node] = &(fluidNodes[node]);
    }

    mFlowOrchestrator     = new GunnsFluidFlowOrchestrator(mNumLinks, mNumNodes);
    mOwnsFlowOrchestrator = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] nodeList (--) Input nodes structure
///
/// @note     This method must be called before any link initialization and before Gunns::initialize
///
/// @details  This method initializes the basic nodes array for basic networks.  Only one call to
///           initializeFluidNodes or initializeNodes is allowed, otherwise we throw exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::initializeNodes(GunnsNodeList& nodeList)
{
    if (mNodes or mFlowOrchestrator) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Node initialization attempted more than once.");
    }

    /// - Load the basic nodes array
    mNumNodes = nodeList.mNumNodes;
    mNodes = new GunnsBasicNode*[mNumNodes];

    for (int node = 0; node < mNumNodes; ++node) {
        mNodes[node] = &(nodeList.mNodes[node]);
    }

    mFlowOrchestrator     = new GunnsBasicFlowOrchestrator(mNumLinks, mNumNodes);
    mOwnsFlowOrchestrator = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] orchestrator (--) Pointer to the orchestrator object to use.
///
/// @details  If this solver already owns its flow orchestrator, it is deleted.  Then we store the
///           given orchestrator pointer in mFlowOrchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::setFlowOrchestrator(GunnsBasicFlowOrchestrator* orchestrator)
{
    if (mFlowOrchestrator and mOwnsFlowOrchestrator) {
        delete mFlowOrchestrator;
    }
    mFlowOrchestrator     = orchestrator;
    mOwnsFlowOrchestrator = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Prepares the Gunns solver for Run after a checkpoint has been loaded.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::restart()
{
    mInitFlag = false;

    /// - Perform functions common to initialization and restart.
    initializeRestartCommonFunctions();

    /// - Restart network nodes and links.  Catch any exceptions and flag the network as failed to
    ///   restart and not initialized.  Instead of restarting the ground node, just ensure it is
    ///   zeroed as in a normal step.
    try {
        for (int node = 0; node < mNetworkSize; ++node) {
            mNodes[node]->restart();
        }

        mNodes[mNumNodes-1]->resetContentState();
        mNodes[mNumNodes-1]->setPotential(0.0);

        for (int link = 0; link < mNumLinks; ++link) {
            mLinks[link]->restart();
        }
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "failed to restart.")
    }

    GUNNS_INFO("restarted with " << listAllModes());

    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details   Performs functions common to initialization and restart.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::initializeRestartCommonFunctions()
{
    /// - Reset island information.
    mIslandCount            = 0;
    mIslandMaxSize          = 0;

    /// - Reset performance metrics.
    mConvergenceFailCount   = 0;
    mLinkResetStepFailCount = 0;
    mMaxMinorStepCount      = 0;
    mLastMinorStep          = 0;
    mMaxDecompositionCount  = 0;
    mLastDecomposition      = 0;

    /// - Reset the worst-case timing mode flag.
    mWorstCaseTiming        = false;   

    /// - Reset last-pass mode stats.
    mLastSolverMode         = mSolverMode;
    mLastIslandMode         = mIslandMode;
    mLastRunMode            = mRunMode;

    /// - Force a rebuild of the admittance matrix on first pass in Run so that we don't solve on a
    ///   bad or stale matrix.
    mRebuild                = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep (s) The time step of the major frame
///
/// @throws   TsOutOfBoundsException, TsNumericalException, TsInitializationException
///
/// @details  Cycles this Gunns object to iterate the network in run-time.
///
/// @note     TsNumericalException and TsOutOfBoundsException being thrown in run-time are not
///           necessarily fatal, so they should be caught and the sim allowed to keep running.
///           Both types can be avoided by proper network setup.
///
/// @note     TsInitializationException being thrown in run-time should be fatal, as they indicate
///           a network that has not been properly set up.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::step(const double timeStep)
{
    double startTime = GunnsInfraFunctions::clockTime();

    /// - Check for proper initialization and run-time mode settings.
    checkStepInputs();
    if (PAUSE == mRunMode) return;
    ++mMajorStepCount;
    mStepLog.beginMajorStep();

    /// - Call the links to process special read data from the sim bus.
    for (int link = 0; link < mNumLinks; ++link) {
        mLinks[link]->processInputs();
    }

    /// - Reset the fluxes into and out of the nodes so that they can properly integrate new flows
    ///   this pass.
    for (int node = 0; node < mNumNodes; ++node) {
        mNodes[node]->resetFlows();
    }

    /// - Build & solve the system of equations.
    bool isConverged = false;
    try {
        isConverged = iterateMinorSteps(timeStep);
    } catch (TsNumericalException& e) {
        mStepLog.recordStepResult(mLastDecomposition, GunnsMinorStepData::MATH_FAIL);
        mStepLog.endMajorStep();
        throw e;
    }

    /// - System performance metrics: update the average minor frame count per major frame.
    mAvgMinorStepCount     = double (mMinorStepCount)     / double (mMajorStepCount);
    mAvgDecompositionCount = double (mDecompositionCount) / double (mMajorStepCount);
    if (mLastDecomposition > mMaxDecompositionCount) {
        mMaxDecompositionCount = mLastDecomposition;
    }

    if (isConverged) {
        /// - Compute & transport flows.
        mFlowOrchestrator->update(timeStep);

        /// - Once the nodes have been updated, call the links to process final outputs.
        for (int link = mNumLinks-1; link >= 0; --link) {
            mLinks[link]->processOutputs();
        }

    /// - If the network did not converge, instead of processing final link & node outputs, reset
    ///   the potential solution to the last major frame, including overrides, and issue a warning.
    } else {
        ++mConvergenceFailCount;
        resetToMajorPotentialVector();
        overridePotential();
        outputPotentialVector();
        mStepLog.recordPotential(mPotentialVector);
        GUNNS_WARNING("failed to converge.");
    }

    /// - Save the potential vector for next pass.  This covers linear networks, and the last minor
    ///   step in non-linear networks.  Linear networks don't currently need this, but may in the
    ///   future.
    saveMajorPotentialVector();

    mRebuild = false;
    mStepLog.endMajorStep();

    mSolveTime = mSolveTimeWorking;
    mStepTime  = GunnsInfraFunctions::clockTime() - startTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep (s) The time step of the major frame
///
/// @return   bool (--) Returns true if the network converged on a solution, false otherwise
///
/// @throws   TsNumericalException
///
/// @details  This method iterates the network minor steps until the network solution has converged.
///           Linear networks only perform one step, whereas non-linear networks iterate up to a
///           preset limit of steps.  During each step, we step the links to get their contributions
///           to the system, then solve the network, then call the links again to output the results
///           to them.  Handshake with non-linear link objects is performed for acceptance of the
///           the solution, and the links are allowed to adjust themselves to prepare for the next
///           iteration.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Gunns::iterateMinorSteps(const double timeStep)
{
    bool networkConverged = false;
    int  convergedStep    = 0;
    mSolveTimeWorking     = 0.0;
    GunnsBasicLink::SolutionResult result = GunnsBasicLink::CONFIRM;

    /// - A linear network is always considered converged by definition, and minor steps are
    ///   only performed in non-linear networks that are in NORMAL mode.
    int minorStepLimit = mMinorStepLimit;
    if (mLinearNetwork or (NORMAL != mSolverMode)) {
        minorStepLimit   = 1;
        networkConverged = true;

    /// - For non-linear networks, zero out the saved node minor step potentials when recording.
    } else if (mDebugDesiredNode > -1) {
        clearDebugNode();
    }

    /// - Minor step loop.
    mLastDecomposition = 0;
    for (mLastMinorStep = 1; mLastMinorStep <= minorStepLimit; ++mLastMinorStep) {

        /// - Performance metrics: update the minor step counts.
        ++mMinorStepCount;
        if (mLastMinorStep > mMaxMinorStepCount) {
            mMaxMinorStepCount = mLastMinorStep;
        }
        mStepLog.beginMinorStep(mMajorStepCount, mLastMinorStep);

        /// - If the result of the previous minor step is DELAY, then we skip stepping the links and
        ///   building/solving the system.
        if (GunnsBasicLink::DELAY != result) {

            /// - Step each link in the network.  On the first minor step, we call the link's main
            ///   step method.  On subsequent minor steps (in a non-linear network), we call non-
            ///   linear link's minorStep method.
            for (int link = 0; link < mNumLinks; ++link) {

                if (1 == mLastMinorStep) {
                    mLinks[link]->step(timeStep);
                }

                else if(mLinks[link]->isNonLinear()) {
                    mLinks[link]->minorStep(timeStep, mLastMinorStep);
                }

                /// - Rebuild the system if any link declares it is changing the admittance matrix.
                if(mLinks[link]->needAdmittanceUpdate()) {
                    mRebuild = true;
                }
            }

            /// - Force rebuild of the system every minor step for worst-case timing mode.
            if (mWorstCaseTiming) {
                mRebuild = true;
            }

            /// - Now that we have all the link contributions to the system of equations, build and
            ///   solve the system for a new solution vector, handle overrides of the solution, and
            ///   output the solution to the links.
            try {
                buildAndSolveSystem(mLastMinorStep, timeStep);
            } catch (TsOutOfBoundsException &e) {
                networkConverged = false;
                /// - Pause recording minor step potentials when the recorded node fails to converge.
                mDebugDesiredNode = -1;
                mStepLog.recordNodesConvergence(mNodesConvergence);
                mStepLog.recordStepResult(mLastDecomposition, GunnsMinorStepData::DECOMP_LIMIT);
                break;
            }
            overridePotential();
            outputPotentialVector();
        } else {
            /// - Record links' admittance matrix and source vector here because buildAndSolveSystem
            ///   wasn't called.
            mStepLog.recordLinkContributions();
        }
        mStepLog.recordPotential(mPotentialVector);

        /// - For non-linear networks in NORMAL mode, the first thing to do after the solution is to
        ///   check for convergence, and rejection or delay of the solution by any links.  We do not
        ///   need to check for convergence if the network is delayed from the previous step.
        if (!mLinearNetwork && (NORMAL == mSolverMode)) {
            saveDebugNode(mLastMinorStep);
            if (GunnsBasicLink::DELAY == result or checkSystemConvergence(mLastMinorStep)) {
                convergedStep++;
            }
            mStepLog.recordNodesConvergence(mNodesConvergence);

            /// - Any links rejecting the solution will kick the entire network state back to the
            ///   previous minor step, and the network goes back to being un-converged.
            result = confirmSolutionAcceptance(convergedStep, mLastMinorStep);

            // We use if/else structure instead of switch/case to avoid ambiguity with break
            // statements.
            if (GunnsBasicLink::REJECT == result) {
                resetToMinorPotentialVector();
                outputPotentialVector();
                resetLinksToMinorStep(convergedStep, mLastMinorStep);
                convergedStep = 0;

            /// - If the links delayed or confirmed the solution, save the potential vector for the
            ///   next minor step.  Break out of the minor step loop if the solution is converged
            ///   and all links have confirmed.
            } else {
                saveMinorPotentialVector();
                if (GunnsBasicLink::CONFIRM == result && convergedStep > 0) {
                    networkConverged = true;
                    if ( (mWorstCaseTiming and (mDecompositionLimit <= mLastDecomposition))
                            or not mWorstCaseTiming) {
                        mStepLog.recordStepResult(mLastDecomposition, GunnsMinorStepData::SUCCESS);
                        break;
                    }
                }
            }
        }
        if (GunnsBasicLink::CONFIRM == result) {
            mStepLog.recordStepResult(mLastDecomposition, GunnsMinorStepData::CONFIRM);
        } else if (GunnsBasicLink::DELAY == result) {
            mStepLog.recordStepResult(mLastDecomposition, GunnsMinorStepData::DELAY);
        } else {
            mStepLog.recordStepResult(mLastDecomposition, GunnsMinorStepData::REJECT);
        }
    }

    return networkConverged;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] minorStep (--) The absolute minor step # that the network is on
/// @param[in] timeStep  (s)  Execution time step
///
/// @returns  int (--) The number of matrix decomposition attempted this call (0 or 1).
///
/// @throws   TsNumericalException, TsOutOfBoundsException
///
/// @details  This function builds and solves the system of equations using numerical methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
int Gunns::buildAndSolveSystem(const int minorStep, const double timeStep)
{
    //TODO prototype SOR/Cholesky mix
    buildSourceVector();
    bool needDecomposition = false;
    if (mRebuild or mSorActive or mDebugDesiredStep != 0) {
        buildAdmittanceMatrix();
        conditionAdmittanceMatrix();
        needDecomposition = true;
        mRebuild = false;
    }
    mStepLog.recordLinkContributions();

    //if sorResult = -1, then sor didnt' converge, so throw a warning, reset mPotentialVector back
    //to the previous minor step, and go to Cholesky.
    //if sor is not active, go to Cholesky like normal.
    mSorLastIteration = -1;
    if (mSorActive and NORMAL == mSolverMode) {
        mSorLastIteration = Sor::solve(mPotentialVector, mAdmittanceMatrix, mSourceVector,
                                       mNetworkSize, mSorWeight, mSorMaxIter, mSorTolerance);
        if (mSorLastIteration < 0) {
            resetToMinorPotentialVector();
            mSorFailCount++;
            GUNNS_WARNING("SOR failed to converge, reverting to Cholesky this step.");
        }
    }
    //do cholesky if SOR either failed to converge or isn't being attempted.
    int result = 0;
    if (mSorLastIteration < 0) {
        if (needDecomposition) {
            /// - Save pre-decomposition admittance matrix elements for debugging on the desired step.
            if (mDebugDesiredStep == minorStep or mDebugDesiredStep < 0) {
                saveDebugAdmittance();
            }

            /// - Only decompose the admittance matrix in NORMAL mode when the matrix has changed, to
            ///   save CPU time.  This is by far the biggest CPU hog in GUNNS.  For a 400 node network,
            ///   90% of CPU time is spent in this method.
            if (NORMAL == mSolverMode) {
                if (mDecompositionLimit > mLastDecomposition) {
                    result = 1;
                    mLastDecomposition++;
                    mDecompositionCount++;

                    /// - Decompose admittance matrix by islands.  This builds a new sub-matrix for
                    ///   each island, then copies the decomposed values back into the main
                    ///   admittance matrix.
                    if (SOLVE == mIslandMode) {
                        /// - Loop over all islands, form a sub-matrix for each island and condition
                        ///   it.  Only decompose islands that contain >1 nodes.
                        for (int island = 0; island < mNetworkSize; ++island) {
                            const int n = mIslandVectors[island].size();
                            if ( (0 < n) and (GPU_SPARSE != mGpuMode) ) {
                                /// - Form sub-matrix for island from the main matrix.
                                for (int i=0, ij=0; i<n; ++i) {
                                    const int in = mIslandVectors[island][i]*mNetworkSize;
                                    for (int j=0; j<n; ++j, ++ij) {
                                        mAdmittanceMatrixIsland[ij] =
                                                mAdmittanceMatrix[in + mIslandVectors[island][j]];
                                    }
                                }
                                if (1 < n) {
                                    decompose(mAdmittanceMatrixIsland, n, island);
                                }
                                /// - Copy decomposed sub-matrix back into main matrix.
                                for (int i=0, ij=0; i<n; ++i) {
                                    const int in = mIslandVectors[island][i]*mNetworkSize;
                                    for (int j=0; j<n; ++j, ++ij) {
                                        mAdmittanceMatrix[in + mIslandVectors[island][j]] =
                                                mAdmittanceMatrixIsland[ij];
                                    }
                                }
                            }
                        }

                    /// - Decompose the full matrix without islands.
                    } else {
                        decompose(mAdmittanceMatrix, mNetworkSize);
                    }
                } else {
                    throw TsOutOfBoundsException("Iteration Limit Exceeded", "Gunns",
                                                 "decomposition limit exceeded.");
                }
            }
        }
        /// - Solve the system of equations.  The result of this is a new potential vector.  This is
        ///   only needed in NORMAL mode.  In DUMMY mode, the links are responsible for their own
        ///   potential.  In SLAVE mode, an external potential vector is received from the caller.
        /// - Node network capacitance calculations before and after the network solution.
        if (NORMAL == mSolverMode) {
            perturbNetworkCapacitances();
            solveCholesky();
            cleanPotentialVector();
            computeNetworkCapacitances(timeStep);
        }
    }

    return(result);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out] A      (--) The admittance sub-matrix to be decomposed.
/// @param[in]     size   (--) The number of rows in the admittance sub-matrix.
/// @param[in]     island (--) Optional island # for error messages.
///
/// @throws   TsNumericalException
///
/// @details  Calls the CPU or GPU decomposition based on the matrix size relative to the CPU/GPU
///           cutoff.  In a Trick environment, also records the elapsed wall time of the
///           decomposition for timing analysis.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::decompose(double *A, const int size, const int island)
{
    double startTime = GunnsInfraFunctions::clockTime();
    if ( (size >= mGpuSizeThreshold) and (GPU_DENSE == mGpuMode) ) {
        handleDecompose(mSolverGpuDense, A, size, island);
        /// - Since mSolverGpuDense only populates the upper triangle U of the decomposed A = LDU
        ///   and solve needs L as well, copy U transpose into L.  This loses negligible time
        ///   compared to the time already spent in the decomposition.  This allows us to keep the
        ///   fully populated LDU, which simplifies debug slices.
        for (int row=1, rowNcol=size; row<mNetworkSize; ++row, rowNcol=row*size) {
            for (int col=0, colNrow=row; col<row; ++col, colNrow+=size, ++rowNcol) {
                A[rowNcol] = A[colNrow];
            }
        }
    } else if ( (GPU_SPARSE != mGpuMode) or (size < mGpuSizeThreshold) ) {
        handleDecompose(mSolverCpu, A, size, island);
    }
    mSolveTimeWorking += GunnsInfraFunctions::clockTime() - startTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  This method checks the network's mode settings for validity and makes sure the
///           network has completed initialization prior to stepping.
///
/// @note     The TsInitializationException should be fatal to the load.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::checkStepInputs()
{
    if (mLastSolverMode != mSolverMode) {
        mLastSolverMode  = mSolverMode;
        GUNNS_INFO("solver mode changed to " << getSolverModeString() << ".");
    }

    if (mLastIslandMode != mIslandMode) {
        mLastIslandMode  = mIslandMode;
        GUNNS_INFO("island mode changed to " << getIslandModeString() << ".");
    }

    if (mLastRunMode != mRunMode) {
        mLastRunMode  = mRunMode;
        GUNNS_INFO("run mode changed to " << getRunModeString() << ".");
    }

    /// - Prevent invalid GPU modes.
    if (NO_GPU != mGpuMode) {
        if (not mGpuEnabled) {
            mGpuMode = NO_GPU;
            GUNNS_WARNING("mGpuMode downmoded to NO_GPU because this solver isn't GPU enabled.");
        } else if (mGpuSizeThreshold > mNetworkSize) {
            mGpuMode = NO_GPU;
            GUNNS_WARNING("mGpuMode downmoded to NO_GPU because the entire network size is smaller than the GPU threshold.");
        }
        if (mGpuSizeThreshold < 2) {
            mGpuSizeThreshold = 2;
            GUNNS_WARNING("mGpuSizeThreshold reset to the minimum operating value of 2.");
        }
    }

    /// - Issue an error if the network has not been initialized.  We use the TS_HS_EXCEPTION macro
    ///   rather than GUNNS_ERROR so we can indicate the unknown name.
    if (!mInitFlag) {
        TS_HS_EXCEPTION(TS_HS_ERROR, "GUNNS", "Network has not been initialized.",
                        TsInitializationException, "Invalid Initialization Data", "Unknown");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] minorStep (--) The absolute minor step # that the network is on
///
/// @return   bool (--) Returns true if the solution has converged, false otherwise
///
/// @details  This method checks for convergence of the system's potential vector solution.  The
///           delta between the previous minor step's solution and the current solution must be
///           below a defined tolerance, for each node individually.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool Gunns::checkSystemConvergence(const int minorStep)
{
    /// - Record the last node found that fails to converge, otherwise -1 indicates all nodes have
    ///   converged.
    int lastNonConvergingNode = -1;
    for (int node = 0; node < mNetworkSize; ++node) {

        mNodesConvergence[node] = fabs(mMinorPotentialVector[node] - mPotentialVector[node]);
        if (mNodesConvergence[node] > mConvergenceTolerance) {
            lastNonConvergingNode = node;

            /// - Pause recording minor step potentials when the recorded node fails to converge.
            if (minorStep == mMinorStepLimit and node == mDebugDesiredNode) {
                mDebugDesiredNode = -1;
            }
        }
    }
    return (lastNonConvergingNode < 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The # of minor steps since the network last converged
/// @param[in] absoluteStep  (--) The absolute minor step number that the network is on
///
/// @return   GunnsBasicLink::SolutionResult (--) The combined assessment of the solution from all
///                                               of the non-linear links in the network
///
/// @details  This method checks all non-linear links for acceptance of the solution.  An example
///           would be an open check valve seeing reverse pressure gradient indicating reverse flow,
///           in which case the link would need to change its state to closed and re-solve the
///           system.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::SolutionResult Gunns::confirmSolutionAcceptance(const int convergedStep,
                                                                const int absoluteStep)
{
    GunnsBasicLink::SolutionResult result = GunnsBasicLink::CONFIRM;

    /// - Loop over all the non-linear links so they can assess the solution.  If one rejects the
    ///   solution, we still give the remaining links a chance to assess, because multiple links
    ///   might be rejecting the solution simultaneously (say for 2 diodes in parallel), and we want
    ///   all of them to adjust for the next minor-step at the same time.
    for (int link = 0; link < mNumLinks; ++link) {

        if (mLinks[link]->isNonLinear()) {

            /// - Get the link's result.  We don't allow links to delay prior to system convergence,
            ///   so change such a result to confirmed until after we've converged.
            GunnsBasicLink::SolutionResult linkResult =
                    mLinks[link]->confirmSolutionAcceptable(convergedStep, absoluteStep);
            mStepLog.recordLinkResult(link, linkResult);
            if ((0 == convergedStep) and (GunnsBasicLink::DELAY == linkResult)) {
                linkResult = GunnsBasicLink::CONFIRM;
            }

            /// - If any link rejects the solution, then the solution is rejected for the entire
            ///   network.  We use if/else structure instead of switch/case to avoid ambiguity with
            //    break statements.
            if (GunnsBasicLink::REJECT == linkResult) {
                result = GunnsBasicLink::REJECT;
            }

            /// - If any link delays the solution, then the entire network solution is delayed, but
            ///   only if no link is rejecting.  Rejecting overrides delaying.
            else if (GunnsBasicLink::DELAY == linkResult && GunnsBasicLink::REJECT != result) {
                result = GunnsBasicLink::DELAY;
            }
            mLinksConvergence[link] = linkResult;
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method assembles the system admittance matrix from the individual link's
///           contributions.  Looping through the links, we use the link's admittance map to know
///           where to add its contribution terms into the main matrix.  Any out-of-bounds number in
///           the link's map denote that location doesn't map to anywhere in the network system of
///           equations, and we omit including the link's contribution at that location.  This will
///           usually be the Ground node, but might occur for spare terms in a link's compressed
///           matrix, or for bad link code that we must protect against, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::buildAdmittanceMatrix()
{
    const int admittanceMatrixSize = mNetworkSize * mNetworkSize;
    for (int i = 0; i < admittanceMatrixSize; ++i) {
        mAdmittanceMatrix[i] = 0.0;
    }

    for (int link = 0; link < mNumLinks; ++link) {
        for (unsigned int linkMap = 0; linkMap < mLinkAdmittanceMaps[link]->mSize; ++linkMap) {
            const int networkMap = mLinkAdmittanceMaps[link]->mMap[linkMap];
            if (networkMap > -1 and networkMap < admittanceMatrixSize) {
                mAdmittanceMatrix[networkMap] += mLinkAdmittanceMatrices[link][linkMap];
            }
        }
    }

    if (OFF != mIslandMode) buildIslands();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method searches through the admittance matrix and groups nodes into partitions,
///           or islands.  This allows the islands to be decomposed separately, which greatly speeds
///           up the total network solution if the largest island node count is significantly
///           smaller than the total network size.
///
///           All nodes in the same island are connected to each other by conductances (represented
///           in the matrix as non-zero off-diagonals).  Separate islands have no conductances
///           between them.
///
///           This method assembles vectors of node numbers for each island.  These are sorted by
///           ascending node number.  These are passed to the nodes, which can then be accessed by
///           incident links for their information about what else is in the island they are on.
///           This can be useful for implementing stability filters, etc.
///
///           Finding the islands incurs about 5% more CPU time than not doing it, more or less
///           depending on how large or sparse the matrix is.  This method only sweeps thru the
///           matrix once, and only does column searches when it hits off-diagonals, which takes
///           advantage of sparsity.  This should speed proportional to # nodes * # off-diagonals.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::buildIslands()
{
    /// - Start with all nodes on their own islands
    for (int node = 0; node < mNetworkSize; ++node) {
        mNodeIslandNumbers[node] = node;
    }

    /// - Find all off-diagonals.  They connect 2 nodes in an island.
    for (int row=0; row<mNetworkSize-1; ++row) {
        for (int col=row+1, a=row*mNetworkSize+col; col<mNetworkSize; col++, a++) {
            if (mAdmittanceMatrix[a] != 0.0) {

                /// - Find the lowest island # of all populated rows in this column, including the
                ///   diagonal.
                int minNum = mNodeIslandNumbers[col];
                if (mNodeIslandNumbers[row] < minNum) minNum = mNodeIslandNumbers[row];  // checks the current row
                for (int row2=row+1, a2=row2*mNetworkSize+col; row2<col; row2++, a2+=mNetworkSize) {
                    if (mAdmittanceMatrix[a2] != 0.0 and mNodeIslandNumbers[row2] < minNum) {
                        minNum = mNodeIslandNumbers[row2];                               // checks the off-diagonals
                    }
                }

                /// - For any island # found in this column not equal to the new lowest island #,
                ///   all nodes with that island # must be merged to the new lowest #.
                for (int row2=row+1, a2=row2*mNetworkSize+col; row2<col; row2++, a2+=mNetworkSize) {
                    if (mAdmittanceMatrix[a2] != 0.0) {
                        mergeIslands(mNodeIslandNumbers[row2], minNum); // sets all off-diags down the column
                    }
                }
                mergeIslands(mNodeIslandNumbers[row], minNum);          // sets the current row
                mergeIslands(mNodeIslandNumbers[col], minNum);          // sets the diagonal
            }
        }
    }

    /// - Populate island vectors with the node numbers, and give each node a pointer to its vector.
    for (int island = 0; island < mNetworkSize; ++island) {
        mIslandVectors[island].clear();
    }
    for (int node = 0; node < mNetworkSize; ++node) {
        mNodes[node]->setIslandVector(&mIslandVectors[mNodeIslandNumbers[node]]);
        mIslandVectors[mNodeIslandNumbers[node]].push_back(node);
    }

    /// - Record island metrics.
    mIslandMaxSize = 0;
    mIslandCount   = 0;
    for (int island = 0; island < mNetworkSize; ++island) {
        int size = mIslandVectors[island].size();
        if (size > 0)              mIslandCount++;
        if (size > mIslandMaxSize) mIslandMaxSize = size;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  from  (--)  Island number to be moved from.
/// @param[in]  to    (--)  Island number to be moved to.
///
/// @details  This method re-assigns all nodes belonging to one island to another island.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::mergeIslands(const int from, const int to)
{
    if (from != to) {
        for (int node = 0; node < mNetworkSize; ++node) {
            if (from == mNodeIslandNumbers[node]) mNodeIslandNumbers[node] = to;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method assembles the system source vector from the individual link's
///           contributions.  Similarly to the admittance matrix, we use the link's node mapping to
///           know where to add its contribution terms into the main vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::buildSourceVector()
{
    for (int i = 0; i < mNetworkSize; ++i) {
        mSourceVector[i] = 0.0;
    }

    for (int link = 0; link < mNumLinks; ++link) {

        for (int port = 0; port < mLinkNumPorts[link]; ++port) {
            const int node = mLinkNodeMaps[link][port];

            // The vacuum/ground node is not actually in the system, so we leave it off.
            if (node < mNetworkSize) {
                mSourceVector[node] += mLinkSourceVectors[link][port];
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsNumericalException
///
/// @details  This method calls the Cholesky solve method to find a new network potential vector
///           solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::solveCholesky()
{
    /// - In GPU_SPARSE, the mSolverGpuSparse->decompose doesn't actually decompose [A], but is
    ///   only used to compress [A] into the format needed by the GPU sparse solver.  The sparse
    ///   solver mSolverGpuSparse->solve does the decomposition and solving on the GPU in one go,
    ///   but we never get back the decomposed [A].  So our implementation and use of GPU_SPARSE
    ///   differs from GPU_DENSE and NO_GPU in 2 important ways:
    ///     The "decompose" method is called immediately before "solve", here in this method.
    ///     We can't re-use decomposed [A] for the next solution to save time.  We have to decompose
    ///       the same [A] every solution, even if [A] hasn't changed.
    if (GPU_SPARSE == mGpuMode) {

        if (SOLVE == mIslandMode) {
            /// - We have to solve each island differently for < or >= the GPU size threshold:
            ///   for <, this island's [A] has already been decomposed, so we do the
            ///          mSolverCpu->solve just like the NO_GPU option.
            ///   for >=, [A] has not been decomposed, so we must call mSolverGpuSparse->decompose
            ///          then ->solve.
            ///   Both options must solve to a temporary {p} that we then unpack into
            ///   mPotentialVector at the end of all islands.
            for (int island = 0; island < mNetworkSize; ++island) {
                const int n = mIslandVectors[island].size();
                /// - Form sub-matrix for island from the main matrix.
                for (int i=0, ij=0; i<n; ++i) {
                    mSourceVectorIsland[i] = mSourceVector[mIslandVectors[island][i]];
                    const int in = mIslandVectors[island][i]*mNetworkSize;
                    for (int j=0; j<n; ++j, ++ij) {
                        mAdmittanceMatrixIsland[ij] =
                                mAdmittanceMatrix[in + mIslandVectors[island][j]];
                    }
                }
                double startTime = GunnsInfraFunctions::clockTime();
                if (n >= mGpuSizeThreshold) {
                    handleDecompose(mSolverGpuSparse, mAdmittanceMatrixIsland, n, island);
                    handleSolve(mSolverGpuSparse, mAdmittanceMatrixIsland, mSourceVectorIsland,
                                mPotentialVectorIsland, n, island);
                } else if (n > 0) {
                    handleSolve(mSolverCpu, mAdmittanceMatrixIsland, mSourceVectorIsland,
                                mPotentialVectorIsland, n, island);
                }
                mSolveTimeWorking += GunnsInfraFunctions::clockTime() - startTime;
                /// - Copy solved potential vector back into main potential vector.
                for (int i=0; i<n; ++i) {
                    mPotentialVector[mIslandVectors[island][i]] = mPotentialVectorIsland[i];
                }
            }
        } else {
            /// - For no islands, the entire system is solved by mSolverGpuSparse. Same as above,
            ///   the difference is GpuSparse must ->decompose, then -> solve.
            double startTime = GunnsInfraFunctions::clockTime();
            handleDecompose(mSolverGpuSparse, mAdmittanceMatrix, mNetworkSize);
            handleSolve(mSolverGpuSparse, mAdmittanceMatrix, mSourceVector,
                        mPotentialVector, mNetworkSize);
            mSolveTimeWorking += GunnsInfraFunctions::clockTime() - startTime;
        }
    } else {
        double startTime = GunnsInfraFunctions::clockTime();
        handleSolve(mSolverCpu, mAdmittanceMatrix, mSourceVector, mPotentialVector, mNetworkSize);
        mSolveTimeWorking += GunnsInfraFunctions::clockTime() - startTime;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] solver (--) Pointer to the linear algebra solver to call.
/// @param[in] A      (--) The admittance matrix to decompose.
/// @param[in] size   (--) The size N of the N x N admittance matrix.
/// @param[in] island (--) The optional network island number associated with this matrix.
///
/// @throws   TsNumericalException
///
/// @details  Performs the actual call to the linear algebra Decompose method, catches any returned
///           exceptions, reformats the error information to a H&S error message.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::handleDecompose(CholeskyLdu* solver, double* A, const int size, const int island)
{
    try {
        solver->Decompose(A, size);
    } catch (TsNumericalException& e) {
        std::ostringstream msg;
        msg << e.getMessage() << " " << e.getThrowingEntityName() << " " << e.getCause();
        if (island > -1) {
            msg << " in island " << island;
        }
        msg << ".";
        GUNNS_ERROR(TsNumericalException, "Error Return Value", msg.str());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] solver (--) Pointer to the linear algebra solver to call.
/// @param[in] A      (--) The admittance matrix for the solution.
/// @param[in] w      (--) The source vector for the solution.
/// @param[in] p      (--) The solution vector for the soution.
/// @param[in] size   (--) The size N of the N x N admittance matrix.
/// @param[in] island (--) The optional network island number associated with this matrix.
///
/// @throws   TsNumericalException
///
/// @details  Performs the actual call to the linear algebra Solve method, catches any returned
///           exceptions, reformats the error information to a H&S error message.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::handleSolve(CholeskyLdu* solver, double* A, double* w, double* p, const int size, const int island)
{
    try {
        solver->Solve(A, w, p, size);
    } catch (TsNumericalException& e) {
        std::ostringstream msg;
        msg << e.getMessage() << " " << e.getThrowingEntityName() << " " << e.getCause();
        if (island > -1) {
            msg << " in island " << island;
        }
        msg << ".";
        GUNNS_ERROR(TsNumericalException, "Error Return Value", msg.str());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes out potential vector values that are smaller than a minimum absolute value.
///           This prevents a network from approaching dirty zeroes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::cleanPotentialVector()
{
    for (int node = 0; node < mNetworkSize; ++node) {
        if (fabs(mPotentialVector[node]) < DBL_EPSILON) {
            mPotentialVector[node] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsNumericalException
///
/// @details  This method is the first half of the network capacitance computations.  For each node
///           that requests its network capacitance, requested flux is added to the node's source
///           vector and the system of equations solved using the last matrix decomposition.  The
///           computeNetworkCapacitances() method is called later to finish the computations.  Nodes
///           that do not request their value have it reset here.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::perturbNetworkCapacitances()
{
    for (int node = 0; node < mNetworkSize; ++node) {
        const double fluxPerturbartion = mNodes[node]->getNetworkCapacitanceRequest();
        if (fluxPerturbartion > DBL_EPSILON) {
            /// - When requested, perturb the node's source vector, solve, store the perturbed
            ///   potential in the node's capacitance term, and reset the source vector.
            const double savedSourceVector = mSourceVector[node];
            mSourceVector[node] += fluxPerturbartion;
            solveCholesky();
            mNodes[node]->setNetworkCapacitance(mPotentialVector[node]);
            mSourceVector[node] = savedSourceVector;

            /// - Initially set the node's delta-potential array to the perturbed potentials.
            const int offset = node * mNetworkSize;
            for (int i=0,j=offset; i<mNetworkSize; ++i,++j) {
                mNetCapDeltaPotential[j] = mPotentialVector[i];
            }
        } else {
            /// - Reset the node's capacitance term when not requested.
            mNodes[node]->setNetworkCapacitance(0.0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  timeStep  (s)  Execution time step.
///
/// @details  This method is the second half of the network capacitance computations.  The
///           perturbed node potential in response to the requested flux computed in
///           perturbNetworkCapacitances() is compared to the actual network potential solution and
///           the capacitance computed from the delta.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::computeNetworkCapacitances(const double timeStep)
{
    for (int node = 0; node < mNetworkSize; ++node) {
        const double fluxPerturbartion = mNodes[node]->getNetworkCapacitanceRequest();
        if (fluxPerturbartion > DBL_EPSILON) {
            /// - Generic GUNNS capacitance is the flux needed to cause a unit increase in node
            ///   potential.  The perturbed node potential was temporarily stored in the node in
            ///   perturbNetworkCapacitances(), and we overwrite that with the new capacitance
            ///   value.
            const double deltaPotential = fabs(mNodes[node]->getNetworkCapacitance()
                                             - mPotentialVector[node]);
            if (deltaPotential > DBL_EPSILON) {
                mNodes[node]->setNetworkCapacitance(timeStep * fluxPerturbartion / deltaPotential);
            } else {
                mNodes[node]->setNetworkCapacitance(0.0);
            }

            /// - Subtract the final node potentials to finish the node's delta-potential array.
            const int offset = node * mNetworkSize;
            for (int i=0,j=offset; i<mNetworkSize; ++i,++j) {
                mNetCapDeltaPotential[j] -= mPotentialVector[i];
            }

            /// - Reset the node's request when done.
            mNodes[node]->setNetworkCapacitanceRequest(0.0);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method overrides the solution potential vector with inputs from the network links
////          or an externally supplied array, depending on the solver mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::overridePotential()
{
    /// - In SLAVE mode, override the solution potential vector with the externally supplied slave
    ///   vector.  Also set the previous potential vector for when we switch back to NORMAL mode.
    if (SLAVE == mSolverMode) {
        for (int node = 0; node < mNetworkSize; ++node) {
            mPotentialVector[node]      = mSlavePotentialVector[node];
            mMinorPotentialVector[node] = mSlavePotentialVector[node]; //TODO really needed???
                                                                       //TODO need to set major too?
        }
    }

    /// - In NORMAL mode, override the solution potential vector with the link's value.  The links
    ///   can override the potential at each port.  We check the link override flag for each port,
    ///   and if set, we copy the link's potential vector value at that port into the corresponding
    ///   location in the solution potential vector.
    else if (NORMAL == mSolverMode){
        for (int link = 0; link < mNumLinks; ++link) {
            for (int port = 0; port < mLinkNumPorts[link]; ++port) {
                const int node = mLinkNodeMaps[link][port];
                if (mLinkOverrideVectors[link][port] && (node < mNetworkSize)) {
                    /// - Do not allow override of the ground node
                    mPotentialVector[node] = mLinkPotentialVectors[link][port];
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method makes sure the admittance matrix is adequately conditioned.  An
///           ill-conditioned matrix usually results from non-capacitive nodes that are isolated
///           from any other sources.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::conditionAdmittanceMatrix()
{
    /// - Add a very small conductance to Ground to non-capacitive nodes to ensure proper
    ///   conditioning.  A non-capacitive node has its diagonal term equal to the sum of the
    ///   off-diagonals in its row in the matrix.  Single or islands of non-capacitive nodes that
    ///   are isolated from any other source cause the matrix to be singular, ill-conditioned, and
    ///   unsolvable.  This phantom "leak" prevents the singular matrix at the cost of a
    ///   negligible error in the solution.  This causes the isolated nodes to solve to zero
    ///   potential.
    for (int row = 0; row < mNetworkSize; ++row) {

        double rowSum = 0.0;
        for (int index = row*mNetworkSize; index < (row+1)*mNetworkSize; ++index) {
            rowSum += mAdmittanceMatrix[index];
        }

        if (fabs(rowSum) < DBL_EPSILON) {
            const int diagonal = row*mNetworkSize+row;
            mAdmittanceMatrix[diagonal] += std::max(mAdmittanceMatrix[diagonal], DBL_EPSILON) * 1.0E-15;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method outputs the potential vector solution to the nodes and link objects.
///           Similar to how we copy in the link's contributions to the system of equations, this
///           method uses the link's node mapping to know where in the potential vector solution to
///           copy to the link ports from.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::outputPotentialVector()
{
    /// - In DUMMY mode, the links calculate their own potential so we don't output the solution
    ///   vector to them.
    if (DUMMY != mSolverMode) {

        /// - Output new potential to the nodes, always setting the vacuum/ground node to zero.
        for (int node = 0; node < mNetworkSize; ++node) {
            mNodes[node]->setPotential(mPotentialVector[node]);
        }
        mNodes[mNetworkSize]->setPotential(0.0);

        /// - Output new potential to the links, always output zero for the vacuum/ground node.
        for (int link = 0; link < mNumLinks; ++link) {

            for (int port = 0; port < mLinkNumPorts[link]; ++port) {
                if (mLinkNodeMaps[link][port] < mNetworkSize) {
                    mLinkPotentialVectors[link][port] = mPotentialVector[mLinkNodeMaps[link][port]];
                } else {
                    mLinkPotentialVectors[link][port] = 0.0;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] convergedStep (--) The # of minor steps since the network last converged
/// @param[in] minorStep     (--) The absolute minor step # that the network is on
///
/// @details  This method calls the non-linear links to adjust themselves in response to the network
///           reverting to the previous minor step solution.  The links can take action, if
///           necessary, to get a better solution on the next minor step.  Any link that reports a
///           problem with this action causes a counter to be incremented and warning message sent,
///           but there is no other affect on the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::resetLinksToMinorStep(const int convergedStep, const int minorStep)
{
    for (int link = 0; link < mNumLinks; ++link) {

        if (mLinks[link]->isNonLinear() &&
                !mLinks[link]->resetLastMinorStep(convergedStep, minorStep)) {
            ++mLinkResetStepFailCount;
            GUNNS_WARNING(mLinks[link]->getName() << " failed to reset to last minor step.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method resets the potential vector to the last saved minor step solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::resetToMinorPotentialVector()
{
    for (int node = 0; node < mNetworkSize; ++node) {
        mPotentialVector[node] = mMinorPotentialVector[node];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method saves the new minor step potential solution for later.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::saveMinorPotentialVector()
{
    for (int node = 0; node < mNetworkSize; ++node) {
        mMinorPotentialVector[node] = mPotentialVector[node];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method resets the potential vector to the last saved major step solution.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::resetToMajorPotentialVector()
{
    for (int node = 0; node < mNetworkSize; ++node) {
        mPotentialVector[node] = mMajorPotentialVector[node];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method saves the new major step potential solution for later.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::saveMajorPotentialVector()
{
    for (int node = 0; node < mNetworkSize; ++node) {
        mMajorPotentialVector[node] = mPotentialVector[node];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method copies the diagonal or desired row of the admittance matrix into a saved
///           array for debugging.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::saveDebugAdmittance()
{
    /// - For desired slice # outside the valid row/node numbers, record the diagonal.
    if (mDebugDesiredSlice < 0 or mDebugDesiredSlice >= mNetworkSize) {
        for (int col = 0; col < mNetworkSize; ++col) {
            mDebugSavedSlice[col] = mAdmittanceMatrix[col + col*mNetworkSize];
        }

    /// - For desired slice # equal to a valid row/node number, record that row.
    } else {
        const int rowStart = mDebugDesiredSlice * mNetworkSize;
        for (int col = 0; col < mNetworkSize; ++col) {
            mDebugSavedSlice[col] = mAdmittanceMatrix[col + rowStart];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] minorStep (--) The absolute minor step # that the network is on
///
/// @details  This method copies a node's potential for each minor step into a saved array for
///           debugging.  The 0'th position stores the node number for reference.
////////////////////////////////////////////////////////////////////////////////////////////////////
void Gunns::saveDebugNode(const int minorStep)
{
    if (mDebugDesiredNode > -1 and mDebugDesiredNode < mNetworkSize) {
        mDebugSavedNode[minorStep] = mPotentialVector[mDebugDesiredNode];
        if (1 == minorStep) {
            mDebugSavedNode[0]     = static_cast<double>(mDebugDesiredNode);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method clears the node minor step potentials debug array.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::clearDebugNode()
{
    for (int i = 0; i < mMinorStepLimit+1; ++i) {
        mDebugSavedNode[i]       = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws    TsOutOfBoundsException
///
/// @returns   std::string (--) The mSolverMode enumerated state as a descriptive string.
///
/// @details   Returns the current solver mode enumeration state as a string, or throws an exception
///            if the enumeration is an invalid state.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Gunns::getSolverModeString()
{
    switch (mSolverMode) {
        case NORMAL :
            return ("NORMAL");
        case DUMMY :
            return ("DUMMY");
        case SLAVE :
            return ("SLAVE");
        default :
            GUNNS_ERROR(TsOutOfBoundsException, "Invalid Input Argument",
                        "An mSolverMode enum state is not recognized.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws    TsOutOfBoundsException
///
/// @returns   std::string (--) The mIslandMode enumerated state as a descriptive string.
///
/// @details   Returns the current island mode enumeration state as a string, or throws an exception
///            if the enumeration is an invalid state.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Gunns::getIslandModeString()
{
    switch (mIslandMode) {
        case OFF :
            return ("OFF");
        case FIND :
            return ("FIND");
        case SOLVE :
            return ("SOLVE");
        default :
            GUNNS_ERROR(TsOutOfBoundsException, "Invalid Input Argument",
                        "An mIslandMode enum state is not recognized.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws    TsOutOfBoundsException
///
/// @returns   std::string (--) The mRunMode enumerated state as a descriptive string.
///
/// @details   Returns the current run mode enumeration state as a string, or throws an exception if
///            the enumeration is an invalid state.
////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Gunns::getRunModeString()
{
    switch (mRunMode) {
        case RUN :
            return ("RUN");
        case PAUSE :
            return ("PAUSE");
        default :
            GUNNS_ERROR(TsOutOfBoundsException, "Invalid Input Argument",
                        "An mRunMode enum state is not recognized.");
    }
}
