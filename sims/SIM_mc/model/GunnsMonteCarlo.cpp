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
    mAnnealingCoeff(0.0)
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
    mInStatesMaster = inStatesMaster;
    std::srand(mConfigData.mRandomSeed);

    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        mParticles.push_back(GunnsMonteCarloPsoParticle());
        //TODO extract to Particle::initialize
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            mParticles.at(i).mCurrentState.mState.push_back(0.0); // TODO uniform distro over given range, not random
            mParticles.at(i).mBestState.mState.push_back(0.0);
            mParticles.at(i).mBestState.mCost = DBL_MAX;
        }
    }
    mActiveParticle = &mParticles.at(0);

    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        mGlobalBestState.mState.push_back(0.0);
    }
    mGlobalBestState.mCost = DBL_MAX;

    initSwarm();
    mRunCounter = UINT_MAX - 1;
    mEpoch      = -1;
}

//TODO
void GunnsMonteCarloPso::initSwarm()
{
    //for now just randomize
    randomizeSwarm();
}

//TODO
void GunnsMonteCarloPso::randomizeSwarm()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsMonteCarloPsoState& state = mParticles.at(i).mCurrentState;

        for (unsigned int i=0; i<state.mState.size(); ++i) {
            const double range = mInStatesMaster->at(i).mMaximum - mInStatesMaster->at(i).mMinimum;
            state.mState.at(i) = mInStatesMaster->at(i).mMinimum + range * rand() / RAND_MAX;
        }
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

        // update inertia weight for new epoch TODO
        double inertiaWeight = mConfigData.mInertiaWeight;

        // propagate the swarm
        propagateSwarm(inertiaWeight);
    }

    // point hte active particle to the particle that's up next
    mActiveParticle = &mParticles.at(mRunCounter);
    std::cout << "PSO update Epoch " << mEpoch << ", run " << mRunCounter << std::endl;
}

//TODO updates the swarm state between iterations
//https://en.wikipedia.org/wiki/Particle_swarm_optimization
//TODO see Chip Birge's matlab implementation pso_new.m,
// - use the Common PSO propagation and 'wraparound' at boundaries
// - redo our annealing to match Chip's, start & end inertia, etc.
void GunnsMonteCarloPso::propagateSwarm(const double inertiaWeight)
{
    double globalCost = DBL_MAX;

    //TODO for now just randomize
    randomizeSwarm();

    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsMonteCarloPsoState& state = mParticles.at(i).mCurrentState;

        // find & update the global best state
//        if (state.mCost < globalCost) {
//            mGlobalBestState = mParticles.at(i).mCurrentState.mState; //TODO need assignment oper
//        }

        // update each particle's personal best state
//        if (state.mCost < mParticles.at(i).mBestState.mCost) {
//            mParticles.at(i).mBestState = state;
//        }

        // update particle acceleration
        // accel = inertia * (mCognitiveCoefficitn * rand() * (personal best state - current state)
        //                  + mSocialCoefficitn * rand() * (global best state - current state))
        // update particle velocity, velocity += accel
        // update particle position, position += velocity
    }
}

//TODO
const std::vector<double>* GunnsMonteCarloPso::getState() const
{
    return &mActiveParticle->mCurrentState.mState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Monte Carlo Manager Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsMonteCarlo::GunnsMonteCarlo()
    :
    mIsMaster(false),
    mIsSlave(false),
    mSlaveId(0),
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

    // do cost function - right now a weighted average of the error
    //TODO long term extract to user-customizable function
    //TODO maybe move to slave for the whole-run use case
    double totalCost = 0.0;
    for (unsigned int i=0; i<mOutDoublesMaster.size(); ++i) {
        const double error = fabs(mOutDoublesMaster.at(i).mOutput - mOutDoublesMaster.at(i).mTarget);
        totalCost += error * mOutDoublesMaster.at(i).mCostWeight / mSumCostWeights;
    }
    std::cout << " cost: " << totalCost << std::endl;
}

//TODO
void GunnsMonteCarlo::updateMasterShutdown()
{
    std::cout << "updateMasterShutdown" << std::endl;
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
