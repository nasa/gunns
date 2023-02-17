/**
@file     GunnsMonteCarlo.cpp
@brief    GUNNS Monte Carlo Manager implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ()
*/

/// - GUNNS inlcudes:
#include "GunnsMonteCarlo.hh"
#include "math/MsMath.hh"

/// - Trick includes:
#include "sim_services/MonteCarlo/include/montecarlo_c_intf.h"

/// - System includes:
#include <cfloat>
#include <iostream> //TODO testing

//TODO
GunnsMonteCarloPso::GunnsMonteCarloPso()
    :
    mConfigData(),
    mInStatesMaster(0),
    mRunCounter(0),
    mEpoch(0),
    mParticles(),
    mActiveParticle(0),
    mGlobalBestState(),
    mAnnealingCoeff(0.0),
    mMaxVelocity()
{
    // nothing to do
}

//TODO
GunnsMonteCarloPso::~GunnsMonteCarloPso()
{
    // nothing to do
}

//TODO
void GunnsMonteCarloPso::initialize(
        const std::vector<GunnsMonteCarloInput>* inStatesMaster)
{
    //TODO validate config and bail:
    // # particles > 0
    // # max epoch > 0
    // # init state > 0

    mInStatesMaster = inStatesMaster;
    std::srand(mConfigData.mRandomSeed);

    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        mParticles.push_back(GunnsMonteCarloPsoParticle());
        //TODO extract to Particle::initialize
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            mParticles.at(i).mCurrentState.mState.push_back(0.0);
            mParticles.at(i).mCurrentState.mVelocity.push_back(0.0);
            mParticles.at(i).mCurrentState.mAcceleration.push_back(0.0);
            mParticles.at(i).mBestState.mState.push_back(0.0);
            mParticles.at(i).mBestState.mCost = DBL_MAX;
        }
    }
    mActiveParticle = &mParticles.at(0);

    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        mGlobalBestState.mState.push_back(0.0);
        /// mConfigData.mMaxVelocity is a scale factor on the range between min & max values of the
        /// states parameter.  So mConfigData.mMaxVelocity = 1 limits maximum velocity to +/- range
        mMaxVelocity.push_back(mConfigData.mMaxVelocity
                * (mInStatesMaster->at(i).mMaximum - mInStatesMaster->at(i).mMinimum));
    }
    mGlobalBestState.mCost = DBL_MAX;

    initSwarm();
//    mRunCounter = UINT_MAX - 1;
//    mEpoch      = -1;
    mRunCounter = -1;
    mEpoch      =  0;

    printStates();
    printGlobalBest();
}

//TODO have user option to select between random & uniform
void GunnsMonteCarloPso::initSwarm()
{
    //for now just randomize
    if (true) {
        randomizeSwarmState();
        randomizeSwarmVelocity();
    } else {
        uniformSwarm();
        randomizeSwarmVelocity();
    }

    // cheat and init particle 0 state to [min, min...] and particle n to [max, max...],
    // with zero velocity
//    GunnsMonteCarloPsoState& state0 = mParticles.at(0).mCurrentState;
//    GunnsMonteCarloPsoState& stateN = mParticles.at(mConfigData.mNumParticles - 1).mCurrentState;
//    for (unsigned int j=0; j<state0.mState.size(); ++j) {
//        state0.mState.at(j) = mInStatesMaster->at(j).mMinimum;
//        stateN.mState.at(j) = mInStatesMaster->at(j).mMaximum;
//        state0.mVelocity.at(j) = 0.0;
//        stateN.mVelocity.at(j) = 0.0;
//    }
    // TODO cheat and init 0th particle to correct answer, see what happens...
//    GunnsMonteCarloPsoState& state0 = mParticles.at(0).mCurrentState;
//    state0.mState.at(0) = 0.001;
//    state0.mState.at(1) = 0.002;
//    state0.mState.at(2) = 0.0005;
//    state0.mState.at(3) = 0.00025;

    /// - Initialize the global best state cost and all particle's best state cost to a high number
    ///   for improvement.
    mGlobalBestState.mCost = DBL_MAX;
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        mParticles.at(i).mBestState.mCost = DBL_MAX;
    }
}

//TODO
void GunnsMonteCarloPso::randomizeSwarmState()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsMonteCarloPsoState& state = mParticles.at(i).mCurrentState;

        for (unsigned int j=0; j<state.mState.size(); ++j) {
            const double range = mInStatesMaster->at(j).mMaximum - mInStatesMaster->at(j).mMinimum;
            state.mState.at(j) = mInStatesMaster->at(j).mMinimum + range * rand() / RAND_MAX;
        }
    }
}

//TODO
/// Initializes velocity with a uniform random distribution between +/- max velocity.
void GunnsMonteCarloPso::randomizeSwarmVelocity()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        GunnsMonteCarloPsoState& state = mParticles.at(i).mCurrentState;
        for (unsigned int j=0; j<state.mVelocity.size(); ++j) {
            state.mVelocity.at(j) = mMaxVelocity.at(j) * (1.0 - 2.0 * rand() / RAND_MAX);
        }
    }
}

//TODO attempt to set swarm to a uniform distribution over the state space
void GunnsMonteCarloPso::uniformSwarm()
{
    // for each state variable we want to evenly distribute the particles
    // how many values?  it depends on how many particles we have vs. the state size
    // if we want 2 values for each state varialbe (say at 33%, 66%), then
    // to fully populate all combinations, we need # particles = 2^state_vars...
    // for 4 state vars: 2^4 = 16... so what do we do with more or less particles?
    // Compute the # of values from the # states and # particles:
    // values^state = particles
    // values = particles^(1/state)
    // 16^(1/4) = 2
    // round down to int:
    // 10^(1/4) = 1.8 ==> 1
    // this example leads to all particles starting at the median of each state range,
    //   and multiple particles with same starting state
    // but if we round up, then we'll run out of particles and can't cover all combo's
    //... so just round down, and alert the user
    //TODO init validates that # state > 0
    const int spread = std::floor(powf(mConfigData.mNumParticles, 1.0 / mInStatesMaster->size()));

    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        const double range = mInStatesMaster->at(i).mMaximum - mInStatesMaster->at(i).mMinimum;
    }
}

//TODO
void GunnsMonteCarloPso::update()
{
    // update the run count & epoch.  each epoch runs each particle once.
    mRunCounter++;
    if (mRunCounter >= mConfigData.mNumParticles) {
        mRunCounter = 0;
        mEpoch++;

        // update inertia weight for new epoch
        // for annealing, ramp from mInertiaWeight to mInertiaWeightEnd as approach max epoch
        double inertiaWeight = mConfigData.mInertiaWeight
                             + (mConfigData.mInertiaWeightEnd - mConfigData.mInertiaWeight)
                             * mEpoch / mConfigData.mMaxEpoch;

        // propagate the swarm
        propagateSwarm(inertiaWeight);
        printStates();
        printGlobalBest();
    }

    // point hte active particle to the particle that's up next
    mActiveParticle = &mParticles.at(mRunCounter);
    std::cout << "PSO update Epoch " << mEpoch << ", run " << mRunCounter << ", best cost: " << mGlobalBestState.mCost << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the personal best state of each particle, and updates the global best state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsMonteCarloPso::updateBestStates()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsMonteCarloPsoState& state = mParticles.at(i).mCurrentState;

        // find & update the global best state
        if (state.mCost < mGlobalBestState.mCost) {
            mGlobalBestState = state;
        }

        // update each particle's personal best state
        if (state.mCost < mParticles.at(i).mBestState.mCost) {
            mParticles.at(i).mBestState = state;
        }
    }
}

//TODO updates the swarm state between iterations
//https://en.wikipedia.org/wiki/Particle_swarm_optimization
//TODO see Chip Birge's matlab implementation pso_new.m,
// - use the Common PSO propagation and 'wraparound' at boundaries
// - redo our annealing to match Chip's, start & end inertia, etc.
void GunnsMonteCarloPso::propagateSwarm(const double inertiaWeight)
{
//    randomizeSwarmState(); TODO for testing
    updateBestStates();

    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsMonteCarloPsoState& state = mParticles.at(i).mCurrentState;

        /// - Deltas from particle's current state to global best and personal best states.
        std::vector<double> globalStateDelta;
        std::vector<double> personalStateDelta;
        globalStateDelta.clear();
        personalStateDelta.clear();
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            globalStateDelta.push_back(mGlobalBestState.mState.at(j) - state.mState.at(j));
            personalStateDelta.push_back(mParticles.at(i).mBestState.mState.at(j) - state.mState.at(j));
        }

        /// - Update particle state.
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            const double cognitiveRand = 1.0 * rand() / RAND_MAX;
            const double socialRand    = 1.0 * rand() / RAND_MAX;
            state.mAcceleration.at(j) = mConfigData.mCognitiveCoeff * cognitiveRand * personalStateDelta.at(j)
                                      + mConfigData.mSocialCoeff    * socialRand    * globalStateDelta.at(j);
            state.mVelocity.at(j) = inertiaWeight * state.mVelocity.at(j) + state.mAcceleration.at(j);
            // limit velocity before adding to state
            if (state.mVelocity.at(j) > mMaxVelocity.at(j)) {
                state.mVelocity.at(j) = mMaxVelocity.at(j);
            } else if (state.mVelocity.at(j) < -mMaxVelocity.at(j)) {
                state.mVelocity.at(j) = -mMaxVelocity.at(j);
            }
            state.mState.at(j) += state.mVelocity.at(j);
        }

        /// - State space boundary check & correction.
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {

            // for wraparound, leave velocity alone and place position on the other side
//            if (state.mState.at(j) < mInStatesMaster->at(j).mMinimum) {
//                state.mState.at(j) = mInStatesMaster->at(j).mMaximum;
//            } else if (state.mState.at(j) > mInStatesMaster->at(j).mMaximum) {
//                state.mState.at(j) = mInStatesMaster->at(j).mMinimum;
//            }

            // TODO reflection seems to work better.
            // for reflection, place position on this side and reverse the velocity.
            if (state.mState.at(j) < mInStatesMaster->at(j).mMinimum) {
                state.mState.at(j) = mInStatesMaster->at(j).mMinimum;
                state.mVelocity.at(j) *= -1.0;
            } else if (state.mState.at(j) > mInStatesMaster->at(j).mMaximum) {
                state.mState.at(j) = mInStatesMaster->at(j).mMaximum;
                state.mVelocity.at(j) *= -1.0;
            }
        }
    }
}

//TODO
const std::vector<double>* GunnsMonteCarloPso::getState() const
{
    return &mActiveParticle->mCurrentState.mState;
}

//TODO
void GunnsMonteCarloPso::assignCost(const double cost)
{
    mActiveParticle->mCurrentState.mCost = cost;
}

//TODO delete, dead code
double GunnsMonteCarloPso::computeVectorMagnitude(const std::vector<double>& vec) const
{
    double rss = 0.0;
    for (unsigned int i=0; i<vec.size(); ++i) {
        rss += vec.at(i) * vec.at(i);
    }
    return sqrt(rss);
}

//TODO delete, dead code
//TODO does nothing if given vector magnitude < DBL_EPSILON
/// @param[in, out] vec
/// @param[in]      magnitude
void GunnsMonteCarloPso::normalizeVector(std::vector<double>& vec, const double magnitude) const
{
    const double vecMag = computeVectorMagnitude(vec);
    if (vecMag > DBL_EPSILON) {
        const double factor = magnitude / vecMag;
        for (unsigned int i=0; i<vec.size(); ++i) {
            vec.at(i) *= factor;
        }
    }
}

//TODO
void GunnsMonteCarloPso::printStates() const
{
    std::cout << "PSO particle states: ";
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        const GunnsMonteCarloPsoState& state = mParticles.at(i).mCurrentState;

        std::cout << std::endl << "  " << i << " P";
        for (unsigned int j=0; j<state.mState.size(); ++j) {
            std::cout << ", " << state.mState.at(j);
        }
        std::cout << std::endl << "  " << i << " V";
        for (unsigned int j=0; j<state.mState.size(); ++j) {
            std::cout << ", " << state.mVelocity.at(j);
        }
        std::cout << std::endl << "  " << i << " A";
        for (unsigned int j=0; j<state.mState.size(); ++j) {
//            std::cout << ", " << state.mAcceleration.at(j);
            printf(", %e", state.mAcceleration.at(j));
        }
    }
    std::cout << std::endl;
}

//TODO
void GunnsMonteCarloPso::printGlobalBest() const
{
    std::cout << "PSO global best state: ";
    for (unsigned int i=0; i<mGlobalBestState.mState.size(); ++i) {
        std::cout << mGlobalBestState.mState.at(i) << ", ";
    }
    std::cout << " cost: " << mGlobalBestState.mCost << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Monte Carlo Manager Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMonteCarlo::GunnsMonteCarlo()
    :
    mIsMaster(false),
    mIsSlave(false),
    mSlaveId(0),
    mRunId(0),
    mRunIdReturned(0),
    mInStatesMaster(),
    mOutDoublesSlave(),
    mOutDoublesMaster(),
    mSumCostWeights(0.0),
    mName(),
    mInitFlag(false)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Monte Carlo Manager Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMonteCarlo::~GunnsMonteCarlo()
{
    // nothing to do
}

//TODO
void GunnsMonteCarlo::initMaster()
{
    mIsSlave  = mc_is_slave();
    mIsMaster = not mIsSlave;
    if (not mIsMaster) {
        //TODO throw error
    } else {
        mSlaveId = -1; // not a slave
    }

    for (unsigned int i=0; i<mOutDoublesMaster.size(); ++i) {
        mSumCostWeights += mOutDoublesMaster.at(i).mCostWeight;
    }

    if (mSumCostWeights < DBL_EPSILON) {
        //TODO throw error
    }

//    mRunId = 1; //TODO

    /// - Initialize the optimizer.
    //TODO change args once optimizer has config data
    mOptimizer.initialize(&mInStatesMaster);
}

//TODO
void GunnsMonteCarlo::initSlave()
{
    mIsSlave  = mc_is_slave();
    mIsMaster = not mIsSlave;
    if (not mIsSlave) {
        //TODO throw error
    } else {
        mSlaveId = mc_get_slave_id();
    }
}

//TODO
void GunnsMonteCarlo::updateMasterPre()
{
    std::cout << "updateMasterPre" << std::endl;
    // for now, randomly determine the state values, limited between min & max
//    for (unsigned int i=0; i<mInStatesMaster.size(); ++i) {
//        const double range = mInStatesMaster.at(i).mMaximum - mInStatesMaster.at(i).mMinimum;
//        const double state = mInStatesMaster.at(i).mMinimum + range * rand() / RAND_MAX;
//        //TODO get the state from the optimizer
//        *mInStatesMaster.at(i).mAddress = state;
//        std::cout << " " << mInStatesMaster.at(i).mName
//                  << " " << *mInStatesMaster.at(i).mAddress << std::endl;
//    }

    mOptimizer.update();
    const std::vector<double>* state = mOptimizer.getState();

    mRunId += 1.0;

    for (unsigned int i=0; i<mInStatesMaster.size(); ++i) {
        const double value = state->at(i);
        *mInStatesMaster.at(i).mAddress = value;
        std::cout << " " << mInStatesMaster.at(i).mName
                  << " " << *mInStatesMaster.at(i).mAddress << std::endl;
    }
}

//TODO
void GunnsMonteCarlo::updateMasterPost()
{
    // read Slave output values from the MC Master/Slave buffer
    std::cout << "updateMasterPost " << mOutDoublesMaster.size();

    for (unsigned int i=0; i<mOutDoublesMaster.size(); ++i) {
        mc_read((char*) &mOutDoublesMaster.at(i).mOutput, sizeof(double));
        std::cout << " " << mOutDoublesMaster.at(i).mOutput;
    }
    mc_read((char*) &mRunIdReturned, sizeof(double));

    // do cost function
    //TODO long term extract to user-customizable function
    //TODO maybe move to slave for the whole-run use case
    double totalCost = 0.0;
    for (unsigned int i=0; i<mOutDoublesMaster.size(); ++i) {
        const double error = fabs(mOutDoublesMaster.at(i).mOutput - mOutDoublesMaster.at(i).mTarget);
        // weighted average of the error:
//        totalCost += error * mOutDoublesMaster.at(i).mCostWeight / mSumCostWeights;
        // simple sum of error * weight:
        totalCost += error * mOutDoublesMaster.at(i).mCostWeight;
    }
    std::cout << " cost: " << totalCost << " runId: " << mRunId << "/" << mRunIdReturned << std::endl;
    mOptimizer.assignCost(totalCost);
}

//TODO
void GunnsMonteCarlo::updateMasterShutdown()
{
    std::cout << "updateMasterShutdown" << std::endl;
    mOptimizer.printGlobalBest();
}

//TODO
void GunnsMonteCarlo::updateSlavePre()
{
    // nothing to do yet
}

//TODO
void GunnsMonteCarlo::updateSlavePost()
{
    // write Slave output values to the MC Master/Slave buffer
    for (unsigned int i=0; i<mOutDoublesSlave.size(); ++i) {
        mc_write((char*) mOutDoublesSlave.at(i), sizeof(double));
    }
    //TODO note Trick doesn't handle this if the run id's are ints -- value of the int
    // gets garbled by the time it makes it back to Master
    mRunIdReturned = mRunId;
    mc_write((char*) &mRunIdReturned, sizeof(double));
}

// TODO
void GunnsMonteCarlo::addInDouble(double* address, const double min, const double max, const std::string& name)
{
    if (not mc_is_slave()) {
        GunnsMonteCarloInput state;
        state.mAddress  = address;
        state.mMinimum  = min;
        state.mMaximum  = max;
        state.mName     = name;
        mInStatesMaster.push_back(state);
    }
}

// TODO in the Slave, store the addresses of the target variables that are to be
//  output to the Master.  On the Master side, size the vector that
//  will receive the values from the Slave.
// This allows the Slave output data to be defined by the input file
void GunnsMonteCarlo::addOutDouble(double* outDouble, const double targetValue, const double costWeight)
{
    if (mc_is_slave()) {
        mOutDoublesSlave.push_back(outDouble);
    } else {
        GunnsMonteCarloTarget target;
        target.mOutput     = 0.0;
        target.mTarget     = targetValue;
        target.mCostWeight = costWeight;
        mOutDoublesMaster.push_back(target);
    }
}
