#ifndef GunnsMonteCarlo_EXISTS
#define GunnsMonteCarlo_EXISTS

/**
@file     GunnsMonteCarlo.hh
@brief    GUNNS Monte Carlo Manager declarations

@defgroup  TSM_GUNNS_CORE_MONTE_CARLO    GUNNS Monte Carlo Manager
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- TODO

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsMonteCarlo.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-01) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

//TODO currently only supports double. templatize to support double, float, int, etc.
struct GunnsMonteCarloInput
{
    public:
        std::string mName;    /**< (1) State model variable name. */
        double*     mAddress; /**< (1) State variable address. */
        double      mMinimum; /**< (1) Minimum state value. */
        double      mMaximum; /**< (1) Maximum state value. */
};

//TODO
struct GunnsMonteCarloTarget
{
    public:
        double mOutput;     /**< (1) Output value from Slave run. */
        double mTarget;     /**< (1) Target value to achieve. */
        double mCostWeight; /**< (1) Weight for the cost function. */
};

struct GunnsMonteCarloPsoState
{
    public:
        std::vector<double> mState; /**< (1) TODO */
        std::vector<double> mVelocity; /**< (1) TODO */
        std::vector<double> mAcceleration; /**< (1) TODO */
        double              mCost;  /**< (1) TODO */
        /// @brief Assignment operator for this PSO particle state.
        GunnsMonteCarloPsoState& operator =(const GunnsMonteCarloPsoState& that) {
            if (this != &that) {
                this->mState = that.mState;
                this->mCost  = that.mCost;
            }
            return *this;
        }
};

//TODO factor optimization stuff out to separate optimization class
struct GunnsMonteCarloPsoParticle
{
    public:
        GunnsMonteCarloPsoState mCurrentState; /**< (1) Current state of this particle. */
        GunnsMonteCarloPsoState mBestState;    /**< (1) Personal best state of this particle. */
};

//TODO
// weights for PSO variant:
//
// inertia
// cognitive
// social
struct GunnsMonteCarloPsoConfigData
{
    public:
        enum SwarmDistribution {
            RANDOM          = 0, ///< Uniform random distribution
            MIN_MAX_CORNERS = 1, ///< Half the swarm at max corner, half at min corner
            FILE            = 2  ///< Read from file
        };
        unsigned int      mNumParticles;     /**< *o (1) trick_chkpnt_io(**) Number of particles in the PSO swarm. */
        unsigned int      mMaxEpoch;         /**< *o (1) trick_chkpnt_io(**) Maximum number of epochs, or iterations, in the total run. */
        double            mInertiaWeight;    // TODO < 1 initial particle inertia weight
        double            mInertiaWeightEnd; // TODO for annealing
        double            mCognitiveCoeff;   //TODO typicallly between 1-3
        double            mSocialCoeff;      //TODO typicallly between 1-3
        double            mMaxVelocity;      /**< *o (1) trick_chkpnt_io(**) Maximum magnitude of particle state velocity. */
        unsigned int      mRandomSeed;       /**< *o (1) trick_chkpnt_io(**) the seed value for RNJesus */
        SwarmDistribution mInitDistribution; /**< *o (1) trick_chkpnt_io(**) Distribution of initial swarm particle states. */
};

//TODO Particle Swarm Optimization
class GunnsMonteCarloPso
{
    public:
        GunnsMonteCarloPsoConfigData             mConfigData;      /**< *o (1) trick_chkpnt_io(**) The configuration data. */
        const std::vector<GunnsMonteCarloInput>* mInStatesMaster;  /**< ** (1) trick_chkpnt_io(**) Pointer to the Master state space description. */
        int                                      mRunCounter;      /**< *o (1) trick_chkpnt_io(**) Count of the elapsed runs in the current epoch. */
        int                                      mEpoch;           /**< *o (1) trick_chkpnt_io(**) The current epoch number. */
        std::vector<GunnsMonteCarloPsoParticle>  mParticles;       /**< ** (1) trick_chkpnt_io(**) The PSO particle swarm. */
        GunnsMonteCarloPsoParticle*              mActiveParticle;  /**< *o (1) trick_chkpnt_io(**) The PSO particle currently being run. */
        GunnsMonteCarloPsoState                  mGlobalBestState; /**< *o (1) trick_chkpnt_io(**) Best state from all particles. */
        double                                   mAnnealingCoeff;  /**< *o (1) trick_chkpnt_io(**) TODO change this to start/end inertia weights */
        std::vector<double>                      mMaxVelocity;     /**< *o (1) trick_chkpnt_io(**) Maximum velocity of state parameters. */
        /// @brief Constructs the GUNNS Monte Carlo Particle Swarm Optimization object.
        GunnsMonteCarloPso();
        /// @brief Destructs the GUNNS Monte Carlo Particle Swarm Optimization object.
        virtual ~GunnsMonteCarloPso();
        /// @brief TODO replace args with polymorphic config data, similar to solver-links.
        void initialize(const std::vector<GunnsMonteCarloInput>* inStatesMaster);
        /// @brief TODO this will become pure virtual, generic i/f with MC
        /// TODO maybe instead of getState() the master passes its vector into here as an in/out and we update it for them
        //   with our active state
        //TODO
        void initSwarm();
        //TODO
        void randomizeSwarmState();
        //TODO
        void randomizeSwarmVelocity();
        //TODO
        void minMaxSwarmState();
        //TODO
        void readFileSwarmState();
        //TODO
        void uniformSwarm();
        //TODO
        void update();
        //TODO
        void updateBestStates();
        /// @brief TODO
        void propagateSwarm(const double inertiaWeight);
        /// @brief TODO
        double uniformRand() { return std::rand() / RAND_MAX; }
        /// @brief TODO this can be called from input file before init
        unsigned int getNumRuns() const {return mConfigData.mNumParticles * mConfigData.mMaxEpoch;}
        /// @brief TODO returns the MC var states from the active particle to teh MC manager to send to teh slave
        const std::vector<double>* getState() const;
        /// @brief Returns the RSS magnitude of the given vector's components.
        double computeVectorMagnitude(const std::vector<double>& vec) const;
        //TODO
        void normalizeVector(std::vector<double>& vec, const double magnitude) const;
        //TODO
        void printStates() const;
        //TODO
        void printGlobalBest() const;
        //TODO
        void assignCost(const double cost);
        //TODO
        void shutdown() const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsMonteCarloPso(const GunnsMonteCarloPso&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsMonteCarloPso& operator =(const GunnsMonteCarloPso&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Monte Carlo Manager
///
/// @details  This is the base link class for any connection between two nodes in a network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsMonteCarlo
{
    TS_MAKE_SIM_COMPATIBLE(GunnsMonteCarlo);

    public:
    //TODO embed all calls to Trick in an included Macro file (similar to but separate from SimCompatibility
    // for ease of porting outside of Trick)
    //TODO don't where this will end up.  Since you have to modify a Trick sim object to add the
    // monte carlo jobs, then might as well add this object directly to the sim object.  It's going to have
    // vectors of pointers to all the model input & output terms,
    // so this doesn't need to be directly associated with a network.
    //TODO should have a polymorphic interface to the optimization object
    // the optimizer object will have its own config data, like PSO swarm size, weights, etc.
    //TODO #slaves corresponds to # parallel runs (CPU's) in the compute farm, not PSO swarm size
    //TODO short-term goals:
    // - Start the Python wrapper - will handle epochs for parallelization of slave runs in each epoch,
    //   as well as init-ing states from saved state file
    //   - saved swarm state between epochs in same format as final output file, for consistency
    // advice from Chip:
    // - recommend 24-30 particles
    // - try initial particle states in the corners, or maybe all together in a single corner, etc.
    //   Configure the optimizer.
    // - save final results of all particles + global best, use taht as input to next
    //   run, and tend to use lower velocity limit on next run becuase you know you're
    //   closer to the globabl min
    // - try initial states at corners or one corner as an optional alternative to random
    //   (still with random vel)
    //TODO long-term issues:
    // - parallel slaves data coherency problems: these are only a problem if run parallel, i.e.
    //   more than 1 slave.  We can run serial: 1 slave, to avoid these for now, and solve these
    //   problems later:
    //   - ensure data coherence between returned data back from slave to particle that drove the inputs to slave
    //     - add data like epoch & run # as MC vars sent to slave and reflected back from slave, verify they match
    //       in the master post job.  Then set up large # of slaves and particles, different # so there is overlap
    //       compare reflected #'s from returned slave with send #'s in master post job, see if they sync
    //     - Trick documentation never explains how multiple parallel slaves are handled
    //     - Trick example sims only ever use one slave.  SIM_Cannon with amoeba optimization algo only uses 1 slave...
    //     - As expected, no data coherency between slave post and master post -- slave data comes back in a different
    //       order and at random times relative to calls to master post.  So, we must use the returned run ID to
    //       know which particle to put the returned data back into
    //       - however, data IS coherent if only 1 slave - parallelization causes the problem
    //   - ensure data back from all slaves in an epoch before propagating the swarm and starting next epoch
//        GunnsOptimizer*      mOptimizer; //TODO pointer to the optimizer object
        GunnsMonteCarloPso   mOptimizer;        /**< *o (1) trick_chkpnt_io(**) The optimizer object. */
        bool                 mIsMaster;         /**< *o (1) trick_chkpnt_io(**) This instance is in the Monte Carlo Master role. */
        bool                 mIsSlave;          /**< *o (1) trick_chkpnt_io(**) This instance is in the Monte Carlo Slave role. */
        int                  mSlaveId;          /**< *o (1) trick_chkpnt_io(**) The Slave role identifier of this instance. */
        double               mRunId;            /**< *o (1) trick_chkpnt_io(**) The run identifier. */
        double               mRunIdReturned;    /**< *o (1) trick_chkpnt_io(**) The returned run identifier from the Slave. */
        std::vector<GunnsMonteCarloInput> mInStatesMaster;  /**< ** (1) trick_chkpnt_io(**) State values in Master written to Slave. */
        std::vector<double*> mOutDoublesSlave;  /**< ** (1) trick_chkpnt_io(**) Pointers to doubles for output from Slave. */
        std::vector<GunnsMonteCarloTarget> mOutDoublesMaster; /**< ** (1) trick_chkpnt_io(**) Doubles values in Master read from Slave. */
        double               mSumCostWeights;   /**< *o (1) trick_chkpnt_io(**) Sum of the output target cost weights. */
        std::string          mName;             /**< *o (1) trick_chkpnt_io(**) Name of this instance for output messages. */
        bool                 mInitFlag;         /**< *o (1) trick_chkpnt_io(**) This instance has been initialized. */
        /// @brief Constructs the GUNNS Monte Carlo Manager object.
        GunnsMonteCarlo();
        /// @brief Destructs the GUNNS Monte Carlo Manager object.
        virtual ~GunnsMonteCarlo();
        /// @brief Initializes in the Master role.
        void initMaster();
        /// @brief Initializes in the Slave role.
        void initSlave();
        /// @brief Updates the Master role before each Slave run.
        void updateMasterPre();
        /// @brief Updates the Master role after each Slave run.
        void updateMasterPost();
        /// @brief Updates the Master role after all Slave runs.
        void updateMasterShutdown();
        /// @brief Updates the Slave role prior to the Slave run.
        void updateSlavePre();
        /// @brief Updates the Slave role after to the Slave run.
        void updateSlavePost();
        /// @brief Returns the object name.
        const char* getName() const;
        /// @brief Returns the initialization status.
        bool        isInitialized() const;
        /// @brief Adds a model variable to the Master-to-Slave data.
        void addInDouble(double* address, const double min, const double max, const std::string& name);
        /// @brief Adds a model variable to the Slave-to-Master data.
        void addOutDouble(double* outDouble, const double targetValue, const double costWeight);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsMonteCarlo(const GunnsMonteCarlo&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsMonteCarlo& operator =(const GunnsMonteCarlo&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   char* (--) The instance name.
///
/// @details  Returns the value of mName as a char*.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* GunnsMonteCarlo::getName() const
{
    return mName.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool (--) The initialization flag.
///
/// @details  Returns the initialization status.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsMonteCarlo::isInitialized() const
{
    return mInitFlag;
}

#endif
