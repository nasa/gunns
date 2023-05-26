/**
@file     GunnsOptimPso.cpp
@brief    GUNNS Particle Swarm Optimization implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ()
*/

/// - GUNNS inlcudes:
#include "math/MsMath.hh"
#include "strings/Strings.hh"

/// - System includes:
#include <cfloat>
#include <iostream> //TODO testing
#include <sstream>
#include <fstream>
#include "GunnsOptimPso.hh"

//TODO
GunnsOptimPsoConfigData::GunnsOptimPsoConfigData()
    :
    mNumParticles(0),
    mMaxEpoch(0),
    mInertiaWeight(0.0),
    mInertiaWeightEnd(0.0),
    mCognitiveCoeff(0.0),
    mSocialCoeff(0.0),
    mMaxVelocity(0.0),
    mRandomSeed(0),
    mInitDistribution(RANDOM)
{
    // nothing to do
}

//TODO
GunnsOptimPsoConfigData::~GunnsOptimPsoConfigData()
{
    // nothing to do
}

//TODO
GunnsOptimPsoConfigData& GunnsOptimPsoConfigData::operator =(const GunnsOptimPsoConfigData& that)
{
    if (this != &that) {
        mNumParticles     = that.mNumParticles;
        mMaxEpoch         = that.mMaxEpoch;
        mInertiaWeight    = that.mInertiaWeight;
        mInertiaWeightEnd = that.mInertiaWeightEnd;
        mCognitiveCoeff   = that.mCognitiveCoeff;
        mSocialCoeff      = that.mSocialCoeff;
        mMaxVelocity      = that.mMaxVelocity;
        mRandomSeed       = that.mRandomSeed;
        mInitDistribution = that.mInitDistribution;
    }
    return *this;
}

//TODO
GunnsOptimPso::GunnsOptimPso()
    :
    GunnsOptimBase(),
    mConfigData(),
//    mInStatesMaster(0),
//    mGlobalRunCounter(0),
//    mRunCounter(0),
//    mEpoch(0),
    mParticles(),
    mActiveParticle(0),
    mGlobalBestState(),
    mAnnealingCoeff(0.0),
    mMaxVelocity()
{
    // nothing to do
}

//TODO
GunnsOptimPso::~GunnsOptimPso()
{
    // nothing to do
}

//TODO inline
void GunnsOptimPso::setConfigData(const GunnsOptimBaseConfigData* configData)
{
    const GunnsOptimPsoConfigData* config = dynamic_cast<const GunnsOptimPsoConfigData*>(configData);
    if (config) {
        mConfigData = *config;
    }
}

//TODO
void GunnsOptimPso::initialize(
        const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster)
{
    //TODO validate config and bail:
    // # particles > 0
    // # max epoch > 0
    // # init state > 0

    mInStatesMaster = inStatesMaster;
    std::srand(mConfigData.mRandomSeed);

    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        mParticles.push_back(GunnsOptimPsoParticle());
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
    {
        std::string pathFile = "pso_cost_history.csv"; //TODO prepend path, etc.
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
        if (file.fail()) {
            //        GUNNS_WARNING("error opening file: " << pathFile);
        } else {
            /// - Write the header row.
            file << "Epoch,Global_Best_Cost " << std::endl;
        }
        file.close();
    }

    /// - Start the swarm state history file
    {
        std::string pathFile = "pso_swarm_history.csv"; //TODO prepend path, etc.
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
        if (file.fail()) {
            //        GUNNS_WARNING("error opening file: " << pathFile);
        } else {
            /// - Write the header row: Epoch,Pos_0_0,Pos_0_1,Cost_0,Pos_1_0,Pos_1_1,Cost_1, ... ,Pos_Best_0, ...
            file << "Epoch";
            for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
                for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
                    file << ",Pos_" << i << "_" << j;
                }
                file << ",Cost_" << i;
            }
            for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
                file << ",Pos_Best" << "_" << j;
            }
            file << ",Cost_Best";
            file << std::endl;
        }
        file.close();
    }

    printStates();
    printGlobalBest();
}

//TODO have user option to select between random & uniform
void GunnsOptimPso::initSwarm()
{
    switch (mConfigData.mInitDistribution) {
        case (GunnsOptimPsoConfigData::RANDOM) :
            randomizeSwarmState();
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsOptimPsoConfigData::MIN_MAX_CORNERS) :
            std::cout << "init MIN_MAX_CORNERS\n";
            minMaxSwarmState();
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsOptimPsoConfigData::FILE) :
            readFileSwarmState(false);
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsOptimPsoConfigData::FILE_CONTINUOUS) :
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
void GunnsOptimPso::initBestCosts()
{
    /// - Initialize the global best state cost and all particle's best state cost to a high number
    ///   for improvement.
    mGlobalBestState.mCost = DBL_MAX;
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        mParticles.at(i).mBestState.mCost = DBL_MAX;
    }
}

//TODO
void GunnsOptimPso::randomizeSwarmState()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsOptimPsoState& state = mParticles.at(i).mCurrentState;

        for (unsigned int j=0; j<state.mState.size(); ++j) {
            const double range = mInStatesMaster->at(j).mMaximum - mInStatesMaster->at(j).mMinimum;
            state.mState.at(j) = mInStatesMaster->at(j).mMinimum + range * rand() / RAND_MAX;
        }
    }
}

//TODO
// Initializes half of the swarm state to (min, min, ...) and have to (max, max, ...).
void GunnsOptimPso::minMaxSwarmState()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        GunnsOptimPsoState& state = mParticles.at(i).mCurrentState;
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
void GunnsOptimPso::readFileSwarmState(const bool continuous)
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
void GunnsOptimPso::randomizeSwarmVelocity()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        GunnsOptimPsoState& state = mParticles.at(i).mCurrentState;
        for (unsigned int j=0; j<state.mVelocity.size(); ++j) {
            state.mVelocity.at(j) = mMaxVelocity.at(j) * (1.0 - 2.0 * rand() / RAND_MAX);
        }
    }
}

//TODO DEAD CODE
//TODO attempt to set swarm to a uniform distribution over the state space
void GunnsOptimPso::uniformSwarm()
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
void GunnsOptimPso::update()
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
        {
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

        /// - Append to the swarm state history file
        {
            std::string pathFile = "pso_swarm_history.csv"; //TODO prepend path, etc.
            std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::app));
            if (file.fail()) {
                //        GUNNS_WARNING("error opening file: " << pathFile);
            } else {
                /// - Write the data row for this epoch.
                file << mEpoch;
                for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
                    for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
                        file << "," << mParticles.at(i).mCurrentState.mState.at(j);
                    }
                    file << "," << mParticles.at(i).mCurrentState.mCost;
                }
                for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
                    file << "," << mGlobalBestState.mState.at(j);
                }
                file << "," << mGlobalBestState.mCost << std::endl;
            }
            file.close();
        }
    }

    // point hte active particle to the particle that's up next
    mActiveParticle = &mParticles.at(mRunCounter);
    mActiveParticle->mCurrentState.mRunId = mGlobalRunCounter;
    std::cout << "PSO update Epoch " << mEpoch << ", run " << mRunCounter << ", best cost: " << mGlobalBestState.mCost << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the personal best state of each particle, and updates the global best state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimPso::updateBestStates()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsOptimPsoState& state = mParticles.at(i).mCurrentState;

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
void GunnsOptimPso::propagateSwarm(const double inertiaWeight)
{
//    randomizeSwarmState(); TODO for testing
    std::cout << "PSO propagateSwarm" << std::endl;
    updateBestStates();

    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsOptimPsoState& state = mParticles.at(i).mCurrentState;

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
const std::vector<double>* GunnsOptimPso::getState() const
{
    return &mActiveParticle->mCurrentState.mState;
}

//TODO
void GunnsOptimPso::assignCost(const double cost, const double runId, const double runIdReturned)
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

//TODO
void GunnsOptimPso::printStates() const
{
    std::cout << "PSO particle states: ";
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        const GunnsOptimPsoState& state = mParticles.at(i).mCurrentState;

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
void GunnsOptimPso::printGlobalBest() const
{
    std::cout << "PSO global best state: ";
    for (unsigned int i=0; i<mGlobalBestState.mState.size(); ++i) {
        std::cout << mGlobalBestState.mState.at(i) << ", ";
    }
    std::cout << " cost: " << mGlobalBestState.mCost << std::endl;
}

void GunnsOptimPso::shutdown() const
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
