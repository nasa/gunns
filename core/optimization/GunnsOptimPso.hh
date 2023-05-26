#ifndef GunnsOptimPso_EXISTS
#define GunnsOptimPso_EXISTS

/**
@file     GunnsOptimPso.hh
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
- ((GunnsOptimPso.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-05) (Initial))

@{
*/

#include "GunnsOptimBase.hh"
#include "GunnsOptimMonteCarloTypes.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

//TODO PROBLEMS:
// - running in parallel with pso.py, all the run costs remain zero
//   and no useful state is found.  pso_state.csv results in all zero costs.
//   it's like the cost function is never being called
//   But it works just fine in serial mode from RUN_mc/input.py
//

struct GunnsOptimPsoState
{
    public:
        std::vector<double> mState; /**< (1) TODO */
        std::vector<double> mVelocity; /**< (1) TODO */
        std::vector<double> mAcceleration; /**< (1) TODO */
        double              mCost;  /**< (1) TODO */
        double              mRunId; /**< (1) TODO */
        /// @brief Assignment operator for this PSO particle state.
        GunnsOptimPsoState& operator =(const GunnsOptimPsoState& that) {
            if (this != &that) {
                this->mState = that.mState;
                this->mCost  = that.mCost;
                this->mRunId = that.mRunId;
            }
            return *this;
        }
};

//TODO factor optimization stuff out to separate optimization class
struct GunnsOptimPsoParticle
{
    public:
        GunnsOptimPsoState mCurrentState; /**< (1) Current state of this particle. */
        GunnsOptimPsoState mBestState;    /**< (1) Personal best state of this particle. */
};

//TODO
class GunnsOptimPsoConfigData : public GunnsOptimBaseConfigData
{
    public:
        enum SwarmDistribution {
            RANDOM          = 0, ///< Uniform random distribution
            MIN_MAX_CORNERS = 1, ///< Half the swarm at max corner, half at min corner
            FILE            = 2, ///< Read from file position, use new random velocity and empty best state
            FILE_CONTINUOUS = 3, ///< Read from file position, velocity and best state.
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
        /// @brief Default constructor.
        GunnsOptimPsoConfigData();
        /// @brief Default destructor.
        virtual ~GunnsOptimPsoConfigData();
        /// @brief Assignment operator.
        GunnsOptimPsoConfigData& operator =(const GunnsOptimPsoConfigData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimPsoConfigData(const GunnsOptimPsoConfigData&);
};

//TODO Particle Swarm Optimization
class GunnsOptimPso : public GunnsOptimBase
{
    public:
        GunnsOptimPsoConfigData             mConfigData;      /**< *o (1) trick_chkpnt_io(**) The configuration data. */
//        const std::vector<GunnsOptimMonteCarloInput>* mInStatesMaster;  /**< ** (1) trick_chkpnt_io(**) Pointer to the Master state space description. */
//        int                                      mGlobalRunCounter; /**< *o (1) trick_chkpnt_io(**) Count of the total elapsed runs from all epochs. */
//        int                                      mRunCounter;      /**< *o (1) trick_chkpnt_io(**) Count of the elapsed runs in the current epoch. */
//        int                                      mEpoch;           /**< *o (1) trick_chkpnt_io(**) The current epoch number. */
        std::vector<GunnsOptimPsoParticle>  mParticles;       /**< ** (1) trick_chkpnt_io(**) The PSO particle swarm. */
        GunnsOptimPsoParticle*              mActiveParticle;  /**< *o (1) trick_chkpnt_io(**) The PSO particle currently being run. */
        GunnsOptimPsoState                  mGlobalBestState; /**< *o (1) trick_chkpnt_io(**) Best state from all particles. */
        double                                   mAnnealingCoeff;  /**< *o (1) trick_chkpnt_io(**) TODO change this to start/end inertia weights */
        std::vector<double>                      mMaxVelocity;     /**< *o (1) trick_chkpnt_io(**) Maximum velocity of state parameters. */
        /// @brief Constructs the GUNNS Monte Carlo Particle Swarm Optimization object.
        GunnsOptimPso();
        /// @brief Destructs the GUNNS Monte Carlo Particle Swarm Optimization object.
        virtual ~GunnsOptimPso();
        //TODO
        virtual void setConfigData(const GunnsOptimBaseConfigData* configData);
        /// @brief TODO replace args with polymorphic config data, similar to solver-links.
        virtual void initialize(const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster);
        /// @brief TODO this will become pure virtual, generic i/f with MC
        /// TODO maybe instead of getState() the master passes its vector into here as an in/out and we update it for them
        //   with our active state
        //TODO
        void initSwarm();
        //TODO
        void initBestCosts();
        //TODO
        void randomizeSwarmState();
        //TODO
        void randomizeSwarmVelocity();
        //TODO
        void minMaxSwarmState();
        //TODO
        void readFileSwarmState(const bool continuous);
        //TODO
        void uniformSwarm();
        //TODO
        virtual void update();
        //TODO
        void updateBestStates();
        /// @brief TODO
        void propagateSwarm(const double inertiaWeight);
        /// @brief TODO this can be called from input file before init
        virtual unsigned int getNumRuns() const {return mConfigData.mNumParticles * mConfigData.mMaxEpoch;}
        /// @brief TODO returns the MC var states from the active particle to teh MC manager to send to teh slave
        virtual const std::vector<double>* getState() const;
        //TODO
        void printStates() const;
        //TODO
        void printGlobalBest() const;
        //TODO
        virtual void assignCost(const double cost, double runID, double runIdReturned);
        //TODO
        virtual void shutdown() const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimPso(const GunnsOptimPso&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsOptimPso& operator =(const GunnsOptimPso&);
};

/// @}

#endif
