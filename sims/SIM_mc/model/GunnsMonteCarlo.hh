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
        double              mCost;  /**< (1) TODO */
};

//TODO factor optimization stuff out to separate optimization class
struct GunnsMonteCarloPsoParticle
{
    public:
        GunnsMonteCarloPsoState mCurrentState; /**< (1) Current state of this particle. */
        GunnsMonteCarloPsoState mBestState;    /**< (1) Personal best state of this particle. */
};

//TODO
struct GunnsMonteCarloPsoConfigData
{
    public:
        unsigned int mNumParticles;    /**< *o (1) trick_chkpnt_io(**) Number of particles in the PSO swarm. */
        unsigned int mMaxEpoch;        /**< *o (1) trick_chkpnt_io(**) Maximum number of epochs, or iterations, in the total run. */
        double       mInertiaWeight;   // TODO < 1 initial particle inertia weight
        double       mCognitiveCoeff;  //TODO typicallly between 1-3
        double       mSocialCoeff;     //TODO typicallly between 1-3
        unsigned int mRandomSeed;      /**< *o (1) trick_chkpnt_io(**) the seed value for RNJesus */
};

//TODO Particle Swarm Optimization
class GunnsMonteCarloPso
{
    public:
        GunnsMonteCarloPsoConfigData             mConfigData;      /**< *o (1) trick_chkpnt_io(**) The configuration data. */
        const std::vector<GunnsMonteCarloInput>* mInStatesMaster;  /**< ** (1) trick_chkpnt_io(**) Pointer to the Master state space description. */
        unsigned int                             mRunCounter;      /**< *o (1) trick_chkpnt_io(**) Count of the elapsed runs in the current epoch. */
        int                                      mEpoch;           /**< *o (1) trick_chkpnt_io(**) The current epoch number. */
        std::vector<GunnsMonteCarloPsoParticle>  mParticles;       /**< ** (1) trick_chkpnt_io(**) The PSO particle swarm. */
        GunnsMonteCarloPsoParticle*              mActiveParticle;  /**< *o (1) trick_chkpnt_io(**) The PSO particle currently being run. */
        GunnsMonteCarloPsoState                  mGlobalBestState; /**< *o (1) trick_chkpnt_io(**) Best state from all particles. */
        double                                   mAnnealingCoeff;  /**< *o (1) trick_chkpnt_io(**) TODO change this to start/end inertia weights */
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
        void randomizeSwarm();
        //TODO
        void update();
        /// @brief TODO
        void propagateSwarm(const double inertiaWeight);
        /// @brief TODO
        double uniformRand() { return std::rand() / RAND_MAX; }
        /// @brief TODO this can be called from input file before init
        unsigned int getNumRuns() const {return mConfigData.mNumParticles * mConfigData.mMaxEpoch;}
        /// @brief TODO returns the MC var states from the active particle to teh MC manager to send to teh slave
        const std::vector<double>* getState() const;

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
    // so this doesn't need to be direclty associated with a network.
    //TODO should have a polymorphic interface to the optimziation object
    // the optijmizer object will have its own config data, like PSO swarm size, weights, etc.
    //TODO #slaves corresponds to # parallel runs (CPU's) in the compute farm, not PSO swarm size
    //TODO short-term goals:
    // - assign particles to slave runs
    // - update epoch (iteration)
    // - send particle states as inputs to slave runs
    //   - have an extra 'active' particle pointer that points to the particle who's up next,
    //     and that creates a fixed set ot terms to give to trick as teh MC vars
    //   - since this, and the particle's state members, don't get created until init, we can't assign
    //     the MC vars to Trick in the input file, have to do it in init
//        GunnsOptimizer*      mOptimizer; //TODO pointer to the optimizer object
        GunnsMonteCarloPso   mOptimizer;        /**< *o (1) trick_chkpnt_io(**) The optimizer object. */
        bool                 mIsMaster;         /**< *o (1) trick_chkpnt_io(**) This instance is in the Monte Carlo Master role. */
        bool                 mIsSlave;          /**< *o (1) trick_chkpnt_io(**) This instance is in the Monte Carlo Slave role. */
        int                  mSlaveId;          /**< *o (1) trick_chkpnt_io(**) The Slave role identifier of this instance. */
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
