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
#include "strings/Strings.hh"

/// - Trick includes:
#include "sim_services/MonteCarlo/include/montecarlo_c_intf.h"

/// - System includes:
#include <cfloat>
#include <iostream> //TODO testing
#include <sstream>
#include <fstream>

//TODO
GunnsMonteCarloPso::GunnsMonteCarloPso()
    :
    mConfigData(),
    mInStatesMaster(0),
    mGlobalRunCounter(0),
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
    mGlobalRunCounter = -1;
    mRunCounter       = -1;
    mEpoch            =  1;

    /// - Start the global cost/epoch history file
    std::string pathFile = "pso_cost_history.csv"; //TODO prepend path, etc.
    std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
    if (file.fail()) {
//        GUNNS_WARNING("error opening file: " << pathFile);
    } else {
        /// - Write the header row.
        file << "Epoch,Global_Best_Cost " << std::endl;
    }
    file.close();

    printStates();
    printGlobalBest();
}

//TODO have user option to select between random & uniform
void GunnsMonteCarloPso::initSwarm()
{
    switch (mConfigData.mInitDistribution) {
        case (GunnsMonteCarloPsoConfigData::RANDOM) :
            randomizeSwarmState();
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsMonteCarloPsoConfigData::MIN_MAX_CORNERS) :
            std::cout << "init MIN_MAX_CORNERS\n";
            minMaxSwarmState();
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsMonteCarloPsoConfigData::FILE) :
            readFileSwarmState(false);
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsMonteCarloPsoConfigData::FILE_CONTINUOUS) :
            std::cout << "init FILE_CONTINUOUS\n";
            readFileSwarmState(true);
            printStates();
            break;
        default :   // invalid selection
            randomizeSwarmState();
            randomizeSwarmVelocity();
            initBestCosts();
            break;
    };
}

//TODO
void GunnsMonteCarloPso::initBestCosts()
{
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
// Initializes half of the swarm state to (min, min, ...) and have to (max, max, ...).
void GunnsMonteCarloPso::minMaxSwarmState()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        GunnsMonteCarloPsoState& state = mParticles.at(i).mCurrentState;
        for (unsigned int j=0; j<state.mState.size(); ++j) {
            if (i < mConfigData.mNumParticles / 2) {
                state.mState.at(j) = mInStatesMaster->at(j).mMinimum;
            } else {
                state.mState.at(j) = mInStatesMaster->at(j).mMaximum;
            }
        }
    }
}

//TODO
/// @param[in] continuous (--) If true, also inits velocity and best state.
// initializes just the swarm from file
void GunnsMonteCarloPso::readFileSwarmState(const bool continuous)
{
    std::string pathFile = "pso_state.csv"; //TODO prepend path, etc.
    std::ifstream file (pathFile.c_str(), (std::ifstream::in));
    if (file.fail()) {
//        GUNNS_WARNING("error opening file: " << pathFile);
    } else {
        /// - Read the whole file into a stream.
        std::stringstream fString;
        fString << file.rdbuf();

        //TODO check file for correct columns/rows for our swarm

        /// - Split the stream by separate lines.
        std::vector<std::string> fLines = Strings::split(fString.str(), "\n");

        for (unsigned int line=1; line<fLines.size(); ++line) {
            std::istringstream in(fLines.at(line));
            std::string particle;  // dummy to hold the 0th column value
            in >> particle;
            if (line < 2) {
                /// - Initialize the global best state from the 1st line (header is 0th line).
                in >> mGlobalBestState.mCost;
                for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
                    in >> mGlobalBestState.mState.at(i);
                }
            } else if (line < mParticles.size() + 2) {
                /// - Initialize the particle states from the subsequent lines.
                in >> mParticles.at(line-2).mCurrentState.mCost;
                for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
                    in >> mParticles.at(line-2).mCurrentState.mState.at(i);
                }
                /// - For continuous propagation, also read the velocity and best state.
                if (continuous) {
                    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
                        in >> mParticles.at(line-2).mCurrentState.mVelocity.at(i);
                    }
                    in >> mParticles.at(line-2).mBestState.mCost;
                    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
                        in >> mParticles.at(line-2).mBestState.mState.at(i);
                    }
                }
            }
        }
    }
    file.close();
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
    mGlobalRunCounter++;
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

        /// - Append to the global cost/epoch history file
        std::string pathFile = "pso_cost_history.csv"; //TODO prepend path, etc.
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::app));
        if (file.fail()) {
    //        GUNNS_WARNING("error opening file: " << pathFile);
        } else {
            /// - Write the data row for this epoch.
            file << mEpoch << "," << mGlobalBestState.mCost << std::endl;
        }
        file.close();
    }

    // point hte active particle to the particle that's up next
    mActiveParticle = &mParticles.at(mRunCounter);
    mActiveParticle->mCurrentState.mRunId = mGlobalRunCounter;
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
//TODO this is never getting called when runnign from pso.py...
void GunnsMonteCarloPso::propagateSwarm(const double inertiaWeight)
{
//    randomizeSwarmState(); TODO for testing
    std::cout << "PSO propagateSwarm" << std::endl;
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
void GunnsMonteCarloPso::assignCost(const double cost, const double runId, const double runIdReturned)
{
    // find the particle whose state run id matches the returned runIf, and assign this cost to it
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        if (runIdReturned == mParticles.at(i).mCurrentState.mRunId) {
            mParticles.at(i).mCurrentState.mCost = cost;
            return;
        }
    }
    // if there was no match, we've done something wrong
    std::cout << "ERROR, RUN ID NO MATCH" << std::endl;
//    mActiveParticle->mCurrentState.mCost = cost;
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

void GunnsMonteCarloPso::shutdown() const
{
    std::cout << "PSO shutdown" << std::endl;
    printGlobalBest();
    /// - Write the swarm state to an output file.
    std::string pathFile = "pso_state.csv"; //TODO prepend path, etc.
    std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
    if (file.fail()) {
//        GUNNS_WARNING("error opening file: " << pathFile);
    } else {
        /// - Write the header row.
        //TODO do we need particle velocity?
        //TODO do we need particle personal best state?
        file << "Particle cost";
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " pos_" << j;
        }
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " vel_" << j;
        }
        file << " best_cost";
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " best_pos_" << j;
        }
        file << std::endl;

        /// - Write the first data row as the global best state.
        file << "global_best " << mGlobalBestState.mCost;
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " " << mGlobalBestState.mState.at(j);
        }
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " 0.0"; // zero velocity
        }
        file << " " << mGlobalBestState.mCost;
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " " << mGlobalBestState.mState.at(j);
        }
        file << std::endl;

        /// - Write a data row for each particle state.
        for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
            file << i << " " << mParticles.at(i).mCurrentState.mCost;
            for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
                file << " " << mParticles.at(i).mCurrentState.mState.at(j);
            }
            for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
                file << " " << mParticles.at(i).mCurrentState.mVelocity.at(j);
            }
            file << " " << mParticles.at(i).mBestState.mCost;
            for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
                file << " " << mParticles.at(i).mBestState.mState.at(j);
            }
            file << std::endl;
        }
    }
    file.close();
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

    mRunId = -1; //TODO so actual run id's start counting from zero in updateMasterPre

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
        const double error = mOutDoublesMaster.at(i).mOutput - mOutDoublesMaster.at(i).mTarget;
        // simple sum of error^2 * weight:
        totalCost += error * error * mOutDoublesMaster.at(i).mCostWeight;
    }
    std::cout << " cost: " << totalCost << " runId: " << mRunId << "/" << mRunIdReturned << std::endl;
    //TODO when running with multiple parallel slaves using python wrapper, the slave runs still
    // come back in a different order, so we must send the run ID along with the cost to the optimizer
    // so it can assign the cost to the correct run.
    mOptimizer.assignCost(totalCost, mRunId, mRunIdReturned);
}

//TODO from Chip, recommend lookup 'Pareto front', multi-objective optimization

//TODO
void GunnsMonteCarlo::updateMasterShutdown()
{
    std::cout << "updateMasterShutdown" << std::endl;
    mOptimizer.update(); // this propagates the swarm at the end of the last epoch, so we can save it to the file for pso.py
    mOptimizer.shutdown();
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
