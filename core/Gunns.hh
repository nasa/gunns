#ifndef Gunns_EXISTS
#define Gunns_EXISTS

/**
@file
@brief    GUNNS Orchestrator declarations

@defgroup  TSM_GUNNS_CORE_ORCHESTRATOR  GUNNS Orchestrator
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (This is the main orchestrator class for the General-Use Nodal Network Solver (GUNNS).  Gunns is
  a generic reusable solver of systems of equations of the form [A]{x} = {b}.  See below for a
  description of these terms.  The system is represented by a network of Node objects
  (GunnsBasicNode class and subs) connected by Link objects (GunnsBasicLink class and subs).  All
  "models" in a Gunns network are GunnsBasicLink-derived objects.  The users only interface with
  the network through their link objects, for execution and data access.  Access to network node
  data is also through the link objects attached to the desired nodes.

- This Gunns class, also called the "solver", operates on a vector of link objects passed in by
  the manager at initialization.  In run-time, Gunns polls each link object for its contributions
  to the system of equations, then solves the system for the new network potentials {x}, then
  outputs these potentials to the links and calls the links to process the flows resulting from
  the new solution.

- There are many types of networks that can be simulated with Gunns.  Fluid, thermal, and
  electrical are examples.  "Basic" networks, which use the "Basic" link and node classes, are the
  general form of Gunns.  Passive thermal and electrical models use this general form.  Fluid
  networks are a special case since additional calculation and transport of fluid properties needs
  to take place that other basic networks don't need.  This is why we have extra "Fluid" classes
  for the nodes & links.  Any general non-fluid problem will use the general, "Basic" network.)

REQUIREMENTS:
- (TBD)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((core/Gunns.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-05) (Design)
-  (Kevin Supak)  (L-3 Communications) (2011-05) (Design)
-  (Teems Lovett) (L-3 Communications) (2011-05) (Design))

@{
*/

#include <string>
#include <vector>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicLink.hh"
#include "core/GunnsMinorStepLog.hh"

/// - Forward declare classes used for pointer attributes and method arguments.
class  GunnsBasicNode;
class  GunnsBasicFlowOrchestrator;
struct GunnsNodeList;
class  PolyFluidConfigData;
class  CholeskyLdu;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Configuration Data
///
/// @details  This class provides a data structure for the Gunns configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsConfigData
{
    public:
        std::string mName;                 /**< (--) trick_chkpnt_io(**) Name of the network for messaging */
        double mConvergenceTolerance;      /**< (--) trick_chkpnt_io(**) Error tolerance for minor step convergence */
        double mMinLinearizationPotential; /**< (--) trick_chkpnt_io(**) Min potential for linearization in links */
        int    mMinorStepLimit;            /**< (--) trick_chkpnt_io(**) Maximum allowed minor steps per major step */
        int    mDecompositionLimit;        /**< (--) trick_chkpnt_io(**) Maximum allowed matrix decompositions per major step */

        /// @brief Default constructs this Gunns configuration data.
        GunnsConfigData(
            const std::string& name                 = "",
            const double       convergenceTolerance = 0.0,
            const double       minLinearizationP    = 0.0,
            const unsigned int minorStepLimit       = 0,
            const unsigned int decompositionLimit   = 0);
        /// @brief Copy constructs this Gunns configuration data.
        GunnsConfigData(const GunnsConfigData& that);
        /// @brief Default destructs this Gunns configuration data.
        virtual ~GunnsConfigData();

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsConfigData& operator =(const GunnsConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    General-Use Nodal Network Solver (GUNNS)
///
/// @details  This is the main Gunns orchestrator class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class Gunns
{
    TS_MAKE_SIM_COMPATIBLE(Gunns);

    public:
        /// @brief Enumeration of the valid solver modes.
        enum SolverMode {
            NORMAL = 0,  ///< Solves for new potential vector.
            DUMMY  = 1,  ///< Doesn't solve, links responsible for their own potential vector.
            SLAVE  = 2   ///< Potential vector supplied by external source.
        };

        /// @brief Enumeration of the island modes.
        //TODO maybe also have a mode where islands are pre-determined & fixed or supplied external
        enum IslandMode {
            OFF     = 0, ///< Islands are not found or used in the solver.
            FIND    = 1, ///< Islands are determined & output to nodes but not used in solver.
            SOLVE   = 2  ///< Islands are decomposed separately by building new island admittance matrices.
        };

        /// @brief Enumeration of the valid run modes.
        enum RunMode {
            RUN   = 0,   ///< Solver updates per the solver mode.
            PAUSE = 1    ///< Solver is paused.
        };

        /// @brief Enumeration of the valid GPU modes.
        enum GpuMode {
            NO_GPU     = 0,   ///< CPU only, doesn't use GPU at all.
            GPU_DENSE  = 1,   ///< Uses GPU dense matrix math for decomposition.
            GPU_SPARSE = 2    ///< Uses GPU sparse matrix math for decomposition & solution.
        };

        /// @name     Step data logger.
        /// @{
        /// @details  This is public to allow Trick jobs to directly call functions in it.
        GunnsMinorStepLog mStepLog;  /**< (--) trick_chkpnt_io(**) Step data logger for debugging */
        /// @}

        /// @brief Default constructs this Gunns object.
        Gunns();

        /// @brief Default destructs this Gunns object.
        virtual ~Gunns();

        /// @brief Initializes this Gunns object with configuration data.
        void initialize(const GunnsConfigData&        configData,
                        std::vector<GunnsBasicLink*>& linksVector);

        /// @brief This method initializes the fluid nodes array for fluid networks.
        void initializeFluidNodes(GunnsNodeList& nodeList);

        /// @brief This method initializes the basic nodes array for basic networks.
        void initializeNodes(GunnsNodeList& nodeList);

        /// @brief This method restarts this Gunns object.
        void restart();

        /// @brief Cycles this Gunns object to iterate the network in run-time.
        void step(const double timeStep);

        /// @brief Sets the solver to NORMAL mode.
        void setNormalMode();

        /// @brief Sets the solver to DUMMY mode.
        void setDummyMode();

        /// @brief Sets the solver to SLAVE mode.
        void setSlaveMode();

        /// @brief Sets the solver Island mode to the given value.
        void setIslandMode(const Gunns::IslandMode mode);

        /// @brief Sets the solver GPU mode and size threshold.
        void setGpuOptions(const Gunns::GpuMode mode, const int threshold);

        /// @brief Sets the solver run mode to RUN.
        void setRunMode();

        /// @brief Sets the solver run mode to PAUSE.
        void setPauseMode();

        /// @brief Sets the solver worst-case timing flag.
        void setWorstCaseTiming(const bool flag);

        /// @brief Points the solver to use the given flow orchestrator.
        void setFlowOrchestrator(GunnsBasicFlowOrchestrator* orchestrator);

        /// @brief Sets the slave potential vector values to the given array values.
        void setSlavePotentialVector(const double* potentials);

        /// @brief Gets the Admittance Matrix.
        double* getAdmittanceMatrix() const;

        /// @brief Gets the Source Vector array.
        double* getSourceVector() const;

        /// @brief Gets the Potential Vector.
        double* getPotentialVector() const;

        /// @brief Gets the average minor steps per major step.
        double getAvgMinorStepCount() const;

        /// @brief Gets the total minor step count.
        int getMinorStepCount() const;

        /// @brief Gets the total major step count.
        int getMajorStepCount() const;

        /// @brief Gets the highest number of minor steps that has occurred in a major step.
        int getMaxMinorStepCount() const;

        /// @brief Gets the number of convergence failures.
        int getConvergenceFailCount() const;

        /// @brief Gets the number of failures of links to reset to the last minor step.
        int getLinkResetStepFailCount() const;

        /// @brief Gets the number of admittance matrix decompositions.
        int getDecompositionCount() const;

        /// @brief Gets the most recent solve time value.
        double getSolveTime() const;

        /// @brief Gets the most recent step time value.
        double getStepTime() const;

        /// @brief Returns whether GPU solving is enabled.
        bool isGpuEnabled() const;

        /// @brief Gets the number of links orchestrated by this solver.
        int getNumLinks() const;

        /// @brief Gets the number of nodes solved by this solver, not counting the Ground node.
        int getNetworkSize() const;

        /// @brief Gets the array of links orchestrated by this solver.
        GunnsBasicLink** getLinks() const;

        /// @brief Gets the last minor step link solution results.
        const GunnsBasicLink::SolutionResult* getLinksConvergence() const;

        /// @brief Gets the last minor step node delta-potentials.
        const double* getNodesConvergence() const;

    protected:
        std::string mName;                /**< *o (--) trick_chkpnt_io(**) Name of the network for messaging */
        int mNumLinks;                    /**< *o (--) trick_chkpnt_io(**) Number of links in the network */
        int mNumNodes;                    /**< *o (--) trick_chkpnt_io(**) Number of nodes including ground */
        GunnsBasicLink** mLinks;          /**< ** (--) trick_chkpnt_io(**) Pointers to the network links */
        GunnsBasicNode** mNodes;          /**< ** (--) trick_chkpnt_io(**) Pointers to the network nodes */
        GunnsBasicFlowOrchestrator* mFlowOrchestrator; /**< ** (--) trick_chkpnt_io(**) The flow orchestrator. */
        bool mOwnsFlowOrchestrator;       /**< *o (--) trick_chkpnt_io(**) This solver owns and will delete the flow orchestrator when true. */

        /// @details  The admittance matrix [A] in the [A]{x} = {b} system of equations.  This is an
        ///           n X n symmetrical positive definite matrix representing the ability of the
        ///           network nodes to admit flux (one node per matrix diagonal) and the ability of
        ///           the network links to conduct flux between nodes (the off-diagonals).  The size
        ///           n of the matrix is given by mNetworkSize.
        double* mAdmittanceMatrix;        /**< ** (--) trick_chkpnt_io(**) Admittance matrix in the system of equations */
        double* mAdmittanceMatrixIsland;  /**< ** (--) trick_chkpnt_io(**) Working array for islands admittance matrix decompositions */

        /// @details  The source vector {b} in the [A]{x} = {b} system of equations.  This is an
        ///           array of size n = mNetworkSize representing the generic flux into each node
        ///           from external and internal flux sources, as well as the effective flux
        ///           represented by various effects.
        double* mSourceVector;            /**<    (--) trick_chkpnt_io(**) Source vector in the system of equations */
        double* mSourceVectorIsland;      /**< ** (--) trick_chkpnt_io(**) Working array for islands source vector */

        /// @details  The solution potential vector {x} in the [A]{x} = {b} system of equations.
        ///           This is an array of size n = mNetworkSize containing the solved generic
        ///           potential at each network node.  Generic potential is analogous between
        ///           various types of networks, i.e. fluid (pressure), electrical (voltage),
        ///           thermal (temperature), or any other system that can be represented in
        ///           [A]{x} = {b}.
        double* mPotentialVector;         /**<    (--) trick_chkpnt_io(**) Final solution of network potentials */
        double* mPotentialVectorIsland;   /**< ** (--) trick_chkpnt_io(**) Working array for islands final solution of network potentials */
        double* mMinorPotentialVector;    /**<    (--) trick_chkpnt_io(**) Last minor step pass of mPotentialVector */
        double* mMajorPotentialVector;    /**<    (--) trick_chkpnt_io(**) Last major step pass of mPotentialVector */
        double* mSlavePotentialVector;    /**<    (--) trick_chkpnt_io(**) Input potential vector for SLAVE mode */
        double* mNetCapDeltaPotential;    /**<    (--) trick_chkpnt_io(**) Network capacitance delta-potential arrays for each node */

        // I will surely be yelled at for this...
        // don't bother checkpoint/restarting these because they're rebuilt every pass anyway
        // TV won't view vectors anyway unless we use TS_NEW_STL_OBJECT macro, but this probably
        //   won't allow nesting
        std::vector< std::vector<int> > mIslandVectors; /**< ** (--) trick_chkpnt_io(**) Admittance matrix island vectors */
        int* mNodeIslandNumbers;          /**<    (--) trick_chkpnt_io(**) Node island assignments */
        int  mIslandCount;                /**<    (--) trick_chkpnt_io(**) Number of islands */
        int  mIslandMaxSize;              /**<    (--) trick_chkpnt_io(**) Size of the largest island */
        IslandMode mIslandMode;           /**<    (--)                     Operating mode of island functions */

        /// @details  Linear algebra solver classes.
        CholeskyLdu* mSolverCpu;          /**< ** (--) trick_chkpnt_io(**) CPU-based matrix decomposition and system solution. */
        CholeskyLdu* mSolverGpuDense;     /**< ** (--) trick_chkpnt_io(**) GPU-based dense matrix decomposition. */
        CholeskyLdu* mSolverGpuSparse;    /**< ** (--) trick_chkpnt_io(**) GPU-based sparse matrix decomposition and system solution. */
        bool         mGpuEnabled;         /**< *o (--) trick_chkpnt_io(**) True if GPU solvers are enabled. */
        GpuMode      mGpuMode;            /**<    (--) trick_chkpnt_io(**) GPU or CPU solution method being used. */
        int          mGpuSizeThreshold;   /**<    (--) trick_chkpnt_io(**) Only network islands at least this size are decomposed/solved on the GPU. */

        /// @details  The tolerance for potential error for considering the network to be solved.
        ///           This is used in non-linear networks between minor steps.  The network is
        ///           converged when the deltas between new and last-pass potentials sum to less
        ///           than this term.  This is meant to be configured for each network depending on
        ///           accuracy and timing needs.
        double  mConvergenceTolerance;    /**<    (--) trick_chkpnt_io(**) Error tolerance for minor step convergence */

        /// @details  The size of the system of equations & the number of nodes in the system, not
        ///           counting the vacuum/ground node.  This value is used to allocate the memory of
        ///           the various network arrays and vectors.
        int     mNetworkSize;             /**< *o (--) trick_chkpnt_io(**) The number of solvable nodes in the system */

        /// @details  The maximum number of minor step iterations on non-linear networks per major
        ///           step (each Gunns::step() call), before we give up and declare the network as
        ///           failed to converge.  This is meant to be configured for the needs of
        ///           individual networks.
        int     mMinorStepLimit;          /**<    (--) trick_chkpnt_io(**) Maximum number of minor step iterations */

        /// @details  The maximum number of admittance matrix decompositions on non-linear networks
        ///           per major step (each Gunns::step() call), before we give up and declare the
        ///           network as failed to converge.  This is meant to be configured for the needs
        ///           of individual networks.
        int     mDecompositionLimit;      /**<    (--) trick_chkpnt_io(**) Maximum number of decomposition iterations */

        /// @details  Flag indicating the admittance matrix should be rebuilt from the link
        ///           contributions and re-decomposed.  Since matrix decomposition takes the most
        ///           amount of time, we only bother to do it if it has changed.
        bool    mRebuild;                 /**<    (--) trick_chkpnt_io(**) The admittance matrix needs to be rebuilt */

        /// @details  Indicates whether this is a linear or non-linear network (true = linear).
        ///           Changing it in run-time will crash the network.
        bool    mLinearNetwork;           /**< *o (--) trick_chkpnt_io(**) This is a linear network */
        SolverMode mSolverMode;           /**<    (--)                     The desired solver mode to execute */
        RunMode    mRunMode;              /**<    (--)                     The desired run mode */
        bool    mInitAttempted;           /**< *o (--) trick_chkpnt_io(**) The network initialization has been attempted */
        bool    mInitFlag;                /**< *o (--) trick_chkpnt_io(**) The network has been successfully initialized */

        /// @name     Link connectivity information.
        /// @{
        /// @details  These pointers create the solver-link interface.  The solver accesses these
        ///           link parameters directly for speed.
        double** mLinkAdmittanceMatrices; /**< ** (--) trick_chkpnt_io(**) Pointers to link admittance matrix contributions */
        double** mLinkPotentialVectors;   /**< ** (--) trick_chkpnt_io(**) Pointers to link potential vectors */
        double** mLinkSourceVectors;      /**< ** (--) trick_chkpnt_io(**) Pointers to link source vectors */
        bool**   mLinkOverrideVectors;    /**< ** (--) trick_chkpnt_io(**) Pointers to link potential override vectors */
        int**    mLinkNodeMaps;           /**< ** (--) trick_chkpnt_io(**) Pointers to link node mapping */
        int*     mLinkNumPorts;           /**< ** (--) trick_chkpnt_io(**) Number of ports for the link */
        GunnsBasicLinkAdmittanceMap** mLinkAdmittanceMaps; /**< ** (--) trick_chkpnt_io(**) Pointers to link admittance mapping */
        /// @}

        /// @name     Performance metrics.
        /// @{
        /// @details  Various counters for monitoring of network performance and stability.  Most of
        ///           these only apply to non-linear networks, but the major step count and
        ///           decomposition count also apply to linear networks.  Some of these terms are
        ///           reset to zero during the restart job on a checkpoint load -- these are
        ///           noted as "since restart" in the comment.  Others, such as the major step
        ///           count, are not zeroed during restart and are preserved in checkpoints - this
        ///           indicates how far the network has been propagated since a clean init.
        int     mMajorStepCount;          /**<    (--)                     Total elapsed major step count since init */
        int     mConvergenceFailCount;    /**<    (--) trick_chkpnt_io(**) Number of convergence failures since restart */
        int     mLinkResetStepFailCount;  /**<    (--) trick_chkpnt_io(**) Number of failures of links to reset to the last minor step since restart */
        int     mMinorStepCount;          /**<    (--)                     Total elapsed minor step count since init */
        int     mMaxMinorStepCount;       /**<    (--) trick_chkpnt_io(**) Highest number of minor steps that has occurred in a major step since restart */
        int     mLastMinorStep;           /**<    (--) trick_chkpnt_io(**) Last minor step count within the major step since restart */
        double  mAvgMinorStepCount;       /**<    (--)                     Average minor steps per major step since init */
        int     mDecompositionCount;      /**<    (--)                     Total matrix decomposition count since init */
        int     mMaxDecompositionCount;   /**<    (--) trick_chkpnt_io(**) Highest number of matrix decompositions that have occurred in a major step since restart */
        int     mLastDecomposition;       /**<    (--) trick_chkpnt_io(**) Last matrix decomposition count within the major step since restart */
        double  mAvgDecompositionCount;   /**<    (--)                     Average matrix decompositions per major step since init */
        double  mSolveTimeWorking;        /**< ** (s)  trick_chkpnt_io(**) Working term for accumulating total value for mSolveTime. */
        double  mSolveTime;               /**<    (s)  trick_chkpnt_io(**) Total duration of all solver decomposition and solve calls the last major step. */
        double  mStepTime;                /**<    (s)  trick_chkpnt_io(**) Total duration of the last step call. */
        /// @details  Forces the solver to keep decomposing the matrix up to the decomposition limit
        ///           each major step regardless of convergence, which uses the maximum CPU.  This
        ///           allows users to budget for worst-case timing scenarios.
        bool    mWorstCaseTiming;         /**<    (--) trick_chkpnt_io(**) Forces maximum decompositions every major step */
        GunnsBasicLink::SolutionResult* mLinksConvergence; /**< (--) trick_chkpnt_io(**) Last minor step solution result of the links */
        double* mNodesConvergence;        /**<    (--) trick_chkpnt_io(**) Last minor step delta-potential of the nodes */
        /// @}

        /// @name     Debug aids.
        /// @{
        double* mDebugSavedSlice;         /**<    (--) trick_chkpnt_io(**) Saved slice of the admittance matrix for debug */
        double* mDebugSavedNode;          /**<    (--) trick_chkpnt_io(**) Saved node minor step potentials for debug */
        /// @details  Controls which slice of the admittance matrix is recorded for debug.  A valid
        ///           row number records that row of the admittance matrix into mDebugSavedSlice.
        ///           A value below zero or above the last row records the diagonal of the matrix.
        int     mDebugDesiredSlice;       /**<    (--)                     Desired pre-decomposition slice */
        /// @details  Controls which minor step the admittance matrix is recorded on for debug.  A
        ///           value of 0 (the default) turns off this feature, and <0 records the last minor
        ///           frame.  To save CPU time, only use this feature when needed for debugging.
        int     mDebugDesiredStep;        /**<    (--)                     Desired minor step for debug arrays, <0 for final */
        /// @details  Controls which node's minor step potentials are being recorded into
        ///           mDebugSavedNode.  A value <0 or above the last row turns off this feature.
        ///           The term is set to -1 to pause recording when that nodes fails to converge.
        int     mDebugDesiredNode;        /**<    (--)                     Desired node to record minor step potentials for */
        bool    mVerbose;                 /**<    (--)                     Verbose console output option flag */
        /// @}

        /// @name     SOR method attributes.
        /// @{
        /// @details  The Successive Over-Relaxation method is an indirect solution method
        ///           alternative to the Cholesky direct method.  This implementation is currently
        ///           only a prototype.
        //TODO do away with SOR
        bool    mSorActive;               /**<    (--)                     Use SOR numerical method before Cholesky */
        float   mSorWeight;               /**<    (--)                     SOR weight factor (omega) term (0-2) */
        int     mSorMaxIter;              /**<    (--)                     SOR iteration limit */
        double  mSorTolerance;            /**<    (--)                     SOR convergence tolerance */
        int     mSorLastIteration;        /**<    (--)                     SOR iteration count in last call */
        int     mSorFailCount;            /**<    (--)                     SOR number of convergence failures */
        /// @}

        /// @name     Last-pass states.
        /// @{
        /// @details  Some last-pass values are saved for responding to state changes.
        SolverMode mLastSolverMode;       /**< ** (--) trick_chkpnt_io(**) The last-pass solver mode. */
        IslandMode mLastIslandMode;       /**< ** (--) trick_chkpnt_io(**) The last-pass island mode. */
        RunMode    mLastRunMode;          /**< ** (--) trick_chkpnt_io(**) The last-pass run mode. */
        /// @}

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        Gunns(const Gunns& that);

        /// @brief Deletes allocated memory objects.
        void       cleanup();

        /// @brief Verifies the network configuration data is properly set.
        void       validateConfigData(const GunnsConfigData& configData);

        /// @brief Verifies that the link objects have been properly initialized.
        void       verifyLinkInitialization();

        /// @brief Verifies that the nodes have been properly initialized.
        void       verifyNodeInitialization();

        /// @brief Performs functions common to initialization and restart.
        void       initializeRestartCommonFunctions();

        /// @brief Decomposes an admittance matrix based on size and GPU options.
        void       decompose(double *A, const int size, const int island = -1);

        /// @brief Verifies network initialization and step method arguments.
        void       checkStepInputs();

        /// @brief Iterates through minor steps for network solution convergence.
        bool       iterateMinorSteps(const double timeStep);

        /// @brief Builds and solves the system of equations.
        int        buildAndSolveSystem(const int minorStep, const double timeStep);

        /// @brief Assembles the system admittance matrix from individual link contributions.
        void       buildAdmittanceMatrix();

        /// @brief Finds and assembles the islands in the admittance matrix.
        void       buildIslands();

        /// @brief Moves all nodes from island to island.
        void       mergeIslands(const int from, const int to);

        /// @brief Assembles the system source vector from individual link contributions.
        void       buildSourceVector();

        /// @brief Calls the Cholesky solve method.
        void       solveCholesky();

        /// @brief Cleans up dirty zeroes in the potential vector solution.
        void       cleanPotentialVector();

        /// @brief Finds potential deltas for network capacitances.
        void       perturbNetworkCapacitances();

        /// @brief Updates the node network capacitances.
        void       computeNetworkCapacitances(const double timeStep);

        /// @brief Overrides the system potential vector.
        void       overridePotential();

        /// @brief Corrects some causes of an ill-conditioned admittance matrix.
        void       conditionAdmittanceMatrix();

        /// @brief Outputs the potential solution to the network links.
        void       outputPotentialVector();

        /// @brief Resets the current potential vector back to the previous minor step.
        void       resetToMinorPotentialVector();

        /// @brief Saves the current potential vector minor step solution for later.
        void       saveMinorPotentialVector();

        /// @brief Resets the current potential vector back to the previous major step.
        void       resetToMajorPotentialVector();

        /// @brief Saves the current potential vector major step solution for later.
        void       saveMajorPotentialVector();

        /// @brief Checks for convergence of a non-linear network solution between minor steps.
        bool       checkSystemConvergence(const int minorStep);

        /// @brief Resets the links to the previous minor step.
        void       resetLinksToMinorStep(const int convergedStep, const int minorStep);

        /// @brief Checks each link for assessment of a non-linear network minor step solution.
        GunnsBasicLink::SolutionResult confirmSolutionAcceptance(const int convergedStep,
                                                                 const int absoluteStep);

        /// @brief Saves elements of the admittance matrix for debugging.
        void       saveDebugAdmittance();

        /// @brief Saves the minor step potentials of a node for debugging.
        void       saveDebugNode(const int minorStep);

        /// @brief Clears the minor step node potentials debug array.
        void       clearDebugNode();

        /// @brief Returns the current solver mode state as a string.
        std::string getSolverModeString();

        /// @brief Returns the current island mode state as a string.
        std::string getIslandModeString();

        /// @brief Returns the current run mode state as a string.
        std::string getRunModeString();

        /// @brief Returns a string listing various solver mode states.
        std::string listAllModes();

        /// @brief Calls and error handles the given solver Decompose method.
        void        handleDecompose(CholeskyLdu* cholesky, double* A, const int size, const int island = -1);

        /// @brief Calls and error handles the given solver Solve method.
        void        handleSolve(CholeskyLdu* cholesky, double* A, double* w, double* p, const int size, const int island = -1);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        Gunns&      operator =(const Gunns& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the network solver mode to NORMAL.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::setNormalMode()
{
    mSolverMode = NORMAL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the network solver mode to DUMMY.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::setDummyMode()
{
    mSolverMode = DUMMY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the network solver mode to SLAVE.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::setSlaveMode()
{
    mSolverMode = SLAVE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// param[in]  mode  (--)  Desired island mode.
///
/// @details  This method sets the network solver Island mode to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::setIslandMode(const Gunns::IslandMode mode)
{
    mIslandMode = mode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the solver run mode to RUN.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::setRunMode()
{
    mRunMode = RUN;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the solver run mode to PAUSE.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::setPauseMode()
{
    mRunMode = PAUSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// param[in]  flag  (--)  True for worst-case timing, false for normal operation.
///
/// @details  This sets the mWorstCaseTiming flag to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::setWorstCaseTiming(const bool flag)
{
    mWorstCaseTiming = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] potentials (--) Array of node potentials to use for SLAVE mode.
///
/// @details  This copies the values from the given array into mSlavePotentialVector, but only when
///           the solver is in SLAVE mode, otherwise does nothing.  This assumes that the given
///           potentials array is as large as the network size.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Gunns::setSlavePotentialVector(const double* potentials)
{
    if (SLAVE == mSolverMode) {
        for (int node = 0; node < mNetworkSize; ++node) {
            mSlavePotentialVector[node] = potentials[node];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double* -- Returns the admittance matrix.
///
/// @details  This method gets the admittance matrix.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* Gunns::getAdmittanceMatrix() const
{
    return mAdmittanceMatrix;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double* -- Returns the source vector.
///
/// @details  This method gets the source vector array.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* Gunns::getSourceVector() const
{
    return mSourceVector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double* -- Returns the potential vector.
///
/// @details  This method gets the potential vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* Gunns::getPotentialVector() const
{
    return mPotentialVector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double -- Returns the average minor step count.
///
/// @details  This method gets the average minor steps per major step.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Gunns::getAvgMinorStepCount() const
{
    return mAvgMinorStepCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int -- Returns the minor step count.
///
/// @details  This method gets the average minor steps per major step.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int Gunns::getMinorStepCount() const
{
    return mMinorStepCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int -- Returns the major step count.
///
/// @details  This method gets the total major step count.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int Gunns::getMajorStepCount() const
{
    return mMajorStepCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int -- Returns the max minor step count.
/////
/// @details  This method gets the highest number of minor steps that has occurred in a major step.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int Gunns::getMaxMinorStepCount() const
{
    return mMaxMinorStepCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int -- Returns the convergence fail count.
///
/// @details  This method gets the number of convergence failures.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int Gunns::getConvergenceFailCount() const
{
    return mConvergenceFailCount;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int -- Returns the link reset step fail count.
//
/// @details  This method gets the number of failures of links to reset to the last minor step.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int Gunns::getLinkResetStepFailCount() const
{
    return mLinkResetStepFailCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return int -- Returns the decomposition count.
///
/// @details  This method gets the number of admittance matrix decompopsitions.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int Gunns::getDecompositionCount() const
{
    return mDecompositionCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   std::string (--) A list of various modes and their states as a string.
///
/// @details  This string includes the solver mode, island mode, and run mode.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline std::string Gunns::listAllModes()
{
    return("solver: " + getSolverModeString() + ", islands: " + getIslandModeString() + ", run: " +
           getRunModeString() + ".");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (s) Elapsed wall time of all decomposition & solve calls the last major step.
///
/// @details  Returns the mSolveTime value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Gunns::getSolveTime() const
{
    return mSolveTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (s) Elapsed wall time of the last step.
///
/// @details  Returns the mStepTime value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double Gunns::getStepTime() const
{
    return mStepTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool (--) True if GPU solving is enabled, false otherwise.
///
/// @details  Returns the value of the mGpuEnabled attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Gunns::isGpuEnabled() const
{
    return mGpuEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  int (--) The number of links orchestrated by this solver.
///
/// @details  Returns mNumLinks.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int Gunns::getNumLinks() const
{
    return mNumLinks;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  int (--) The number of nodes in the network, not counting the Ground node.
///
/// @details  Returns mNetworkSize.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int Gunns::getNetworkSize() const
{
    return mNetworkSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  GunnsBasicLink** (--) Array of pointers to the links orchestrated by this solver.
///
/// @details  Returns mLinks, the length of which is given by getNumLinks().
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsBasicLink** Gunns::getLinks() const
{
    return mLinks;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  GunnsBasicLink::SolutionResult* (--) Array of link last minor step solution results.
///
/// @details  Returns mLinksConvergence, the length of which is given by getNumLinks().
///
/// @note     Linear networks will return NULL, as they don't allocate mLinksConvergence.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsBasicLink::SolutionResult* Gunns::getLinksConvergence() const
{
    return mLinksConvergence;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  double* (--) Array of node last minor step delta-potentials.
///
/// @details  Returns mNodesConvergence, the length of which is given by getNetworkSize().
///
/// @note     Linear networks will return NULL, as they don't allocate mNodesConvergence.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const double* Gunns::getNodesConvergence() const
{
    return mNodesConvergence;
}

#endif
