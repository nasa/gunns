#ifndef GunnsOptimParticleSwarm_EXISTS
#define GunnsOptimParticleSwarm_EXISTS

/**
@file     GunnsOptimParticleSwarm.hh
@brief    GUNNS Particle Swarm Optimization declarations

@defgroup  TSM_GUNNS_CORE_OPTIM_PSO    GUNNS Particle Swarm Optimization
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Refer to class details below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsOptimParticleSwarm.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-05) (Initial))

@{
*/

#include "GunnsOptimBase.hh"
#include "GunnsOptimMonteCarloTypes.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Particle Swarm Optimizer Particle State
///
/// @details  This describes the state of a PSO particle, including its position, velocity and
///           acceleration in the state space as well as the associated MC Slave run ID and cost.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsOptimParticleSwarmState
{
    public:
        std::vector<double> mState;        /**< (1) Position state. */
        std::vector<double> mVelocity;     /**< (1) Velocity state. */
        std::vector<double> mAcceleration; /**< (1) Acceleration state. */
        double              mCost;         /**< (1) Cost function result. */
        double              mRunId;        /**< (1) MC Slave run ID. */
        /// @brief Default constructor for this PSO particle state.
        GunnsOptimParticleSwarmState();
        /// @brief Default destructor for this PSO particle state.
        virtual ~GunnsOptimParticleSwarmState();
        /// @brief Custom assignment operator for this PSO particle state.
        GunnsOptimParticleSwarmState& operator =(const GunnsOptimParticleSwarmState& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Particle Swarm Optimizer Particle
///
/// @details  This describes a PSO particle, which has a current state and a personal best state
///           that is updated and remembered as the optimization progresses.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsOptimParticleSwarmParticle
{
    public:
        GunnsOptimParticleSwarmState mCurrentState; /**< (1) Current state of this particle. */
        GunnsOptimParticleSwarmState mBestState;    /**< (1) Personal best state of this particle. */
        /// @brief Sizes and zeroes this particle's states.
        void initialize(const unsigned int stateSize);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Particle Swarm Optimizer Configuration Data
///
/// @details  This provides the configuration data for the GUNNS PSO.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimParticleSwarmConfigData : public GunnsOptimBaseConfigData
{
    public:
        enum SwarmDistribution {
            RANDOM          = 0, ///< Uniform random distribution
            MIN_MAX_CORNERS = 1, ///< Half the swarm at max corner, half at min corner
            FILE            = 2, ///< Read from file position, use new random velocity and empty best state
            FILE_CONTINUOUS = 3, ///< Read from file position, velocity and best state.
        };
        unsigned int      mNumParticles;     /**< (1) Number of particles in the PSO swarm. */
        unsigned int      mMaxEpoch;         /**< (1) Maximum number of epochs, or iterations, in the total run. */
        double            mInertiaWeight;    /**< (1) Initial (at first epoch) particle inertia weight. */
        double            mInertiaWeightEnd; /**< (1) Ending (at final epoch) particle inertia weight. */
        double            mCognitiveCoeff;   /**< (1) Swarm cognitive coefficient, or weight towards personal best state. */
        double            mSocialCoeff;      /**< (1) Swarm social coefficient, or weight towards global best state. */
        double            mMaxVelocity;      /**< (1) Maximum magnitude of particle state velocity. */
        unsigned int      mRandomSeed;       /**< (1) Seed for the random number generator. */
        SwarmDistribution mInitDistribution; /**< (1) Distribution of initial swarm particle states. */
        /// @brief Default constructor.
        GunnsOptimParticleSwarmConfigData();
        /// @brief Default destructor.
        virtual ~GunnsOptimParticleSwarmConfigData();
        /// @brief Assignment operator.
        GunnsOptimParticleSwarmConfigData& operator =(const GunnsOptimParticleSwarmConfigData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimParticleSwarmConfigData(const GunnsOptimParticleSwarmConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Particle Swarm Optimizer
///
/// @details  This implements a Particle Swarm Optimization (PSO) scheme for use with Trick Monte
///           Carlo and the GUNNS Monte Carlo Manager.  Each particle in the swarm represents a
///           state of the Monte Carlo input variables that are being optimized.  The particles
///           states start with some initial distribution and velocity in the state space.  As the
///           swarm propagates, each particle moves along a trajectory in the state space,
///           pulled towards the swarm's global best state and the particle's personal best state.
///           PSO is good at getting close to the global minimum in a large multi-variate search.
///
///           This PSO writes the swarm state and cost function histories to output files in the
///           Trick sim folder.  The swarm state can be initialized from the saved states from
///           previous runs, or it can be set to one of several default distributions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimParticleSwarm : public GunnsOptimBase
{
    public:
        GunnsOptimParticleSwarmConfigData            mConfigData;      /**< *o (1) trick_chkpnt_io(**) The configuration data. */
        std::vector<GunnsOptimParticleSwarmParticle> mParticles;       /**< ** (1) trick_chkpnt_io(**) The PSO particle swarm. */
        GunnsOptimParticleSwarmParticle*             mActiveParticle;  /**< *o (1) trick_chkpnt_io(**) The PSO particle currently being run. */
        GunnsOptimParticleSwarmState                 mGlobalBestState; /**< *o (1) trick_chkpnt_io(**) Best state from all particles. */
        std::vector<double>                          mMaxVelocity;     /**< *o (1) trick_chkpnt_io(**) Maximum velocity of state parameters. */
        /// @brief Constructs the GUNNS Monte Carlo Particle Swarm Optimization object.
        GunnsOptimParticleSwarm();
        /// @brief Destructs the GUNNS Monte Carlo Particle Swarm Optimization object.
        virtual ~GunnsOptimParticleSwarm();
        /// @brief Copies the given PSO config data into internal storage.
        virtual void setConfigData(const GunnsOptimBaseConfigData* configData);
        /// @brief Initializes this PSO with a pointer to the Monte Carlo input variables.
        virtual void initialize(const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster);
        /// @brief Main update of this PSO; propagates the swarm state.
        virtual void update();
        /// @brief Returns the number of individual MC Slave runs this PSO needs.
        virtual unsigned int getNumRuns() const;
        /// @brief Returns the MC input variable state of the active swarm particle to the MC manager.
        virtual const std::vector<double>* getState();
        /// @brief Stores the given cost to the swarm particle associated with the given Slave run.
        virtual void assignCost(const double cost, double runID, double runIdReturned);
        /// @brief Writes final outputs.
        virtual void shutdown();

    protected:
        /// @brief Validates the configuration and input data for this PSO.
        virtual void validate();
        /// @brief Initializes the particle states with the configured distribution setting.
        void initSwarm();
        /// @brief Initializes the stored best costs for the swarm global and each particle.
        void initBestCosts();
        /// @brief Randomizes the initial particle state positions.
        void randomizeSwarmState();
        /// @brief Randomizes the initial particle state velocities.
        void randomizeSwarmVelocity();
        /// @brief Initializes the particle states at the minimum and maximum corners of the state space.
        void minMaxSwarmState();
        /// @brief Reads the particles states from a file.
        void readFileSwarmState(const bool continuous);
        /// @brief Updates the global swarm best state and each particle's personal best state.
        void updateBestStates();
        /// @brief Propagates the particle states to the next iteration.
        void propagateSwarm(const double inertiaWeight);
        /// @brief Applies the MC input variable constraints to all particle states.
        void applyStateConstraints();
        /// @brief Outputs the swarm particle states to the console.
        void printStates() const;
        /// @brief Outputs the global best state to the console.
        void printGlobalBest() const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimParticleSwarm(const GunnsOptimParticleSwarm&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsOptimParticleSwarm& operator =(const GunnsOptimParticleSwarm&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  unsigned int (--) The total number of Monte Carlo Slave runs this PSO will run.
///
/// @details  Returns the number of Slave runs as the configured swarm size (number of particles)
///           times the configured number of swarm iterations (epochs).
////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int GunnsOptimParticleSwarm::getNumRuns() const
{
    return (mConfigData.mNumParticles * mConfigData.mMaxEpoch);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<double>* (--) Pointer to the vector of the active particle's current state.
///
/// @details  Returns a pointer to the active particles current position state.  These values go
///           into the MC input variables for the next Slave run.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<double>* GunnsOptimParticleSwarm::getState()
{
    return &mActiveParticle->mCurrentState.mState;
}

#endif
