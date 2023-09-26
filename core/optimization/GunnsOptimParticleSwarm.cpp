/**
@file     GunnsOptimParticleSwarm.cpp
@brief    GUNNS Particle Swarm Optimization implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((core/optimization/GunnsOptimParticleSwarm.o))
*/

/// - GUNNS includes:
#include "GunnsOptimParticleSwarm.hh"
#include "core/GunnsMacros.hh"
#include "math/MsMath.hh"
#include "strings/Strings.hh"

/// - System includes:
#include <cfloat>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Default constructs this GUNNS Particle Swarm Optimizer particle state.  We don't trust
///         the implicit constructor to zero the initial values.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimParticleSwarmState::GunnsOptimParticleSwarmState()
    :
    mState(),
    mVelocity(),
    mAcceleration(),
    mCost(0.0),
    mRunId(0.0)
{
    mState.clear();
    mVelocity.clear();
    mAcceleration.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Default destructs this GUNNS Particle Swarm Optimizer particle state.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimParticleSwarmState::~GunnsOptimParticleSwarmState()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) The object to be assigned equal to.
///
/// @returns  GunnsOptimParticleSwarmState& (--) Reference to this object.
///
/// @details  Assigns this GUNNS Particle Swarm Optimizer particle state to the values of the given
///           particle state object.  We don't copy the velocity and acceleration.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimParticleSwarmState& GunnsOptimParticleSwarmState::operator =(const GunnsOptimParticleSwarmState& that)
{
    if (this != &that) {
        this->mState = that.mState;
        this->mCost  = that.mCost;
        this->mRunId = that.mRunId;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] stateSize (--) Number of dimensions in the state space.
///
/// @details  Initializes this PSO particle.  Sizes the current and best states to the number of
///           dimensions of the state space, and zeroes them.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarmParticle::initialize(const unsigned int stateSize)
{
    for (unsigned int j=0; j<stateSize; ++j) {
        mCurrentState.mState.push_back(0.0);
        mCurrentState.mVelocity.push_back(0.0);
        mCurrentState.mAcceleration.push_back(0.0);
        mBestState.mState.push_back(0.0);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Particle Swarm Optimizer configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimParticleSwarmConfigData::GunnsOptimParticleSwarmConfigData()
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Particle Swarm Optimizer configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimParticleSwarmConfigData::~GunnsOptimParticleSwarmConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) The object to be assigned equal to.
///
/// @returns  GunnsOptimParticleSwarmConfigData& (--) Reference to this object.
///
/// @details  Assigns this GUNNS Particle Swarm Optimizer configuration data to the values of the
///           given configuration data object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimParticleSwarmConfigData& GunnsOptimParticleSwarmConfigData::operator =(const GunnsOptimParticleSwarmConfigData& that)
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Particle Swarm Optimizer.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimParticleSwarm::GunnsOptimParticleSwarm()
    :
    GunnsOptimBase(),
    mConfigData(),
    mParticles(),
    mActiveParticle(0),
    mGlobalBestState(),
    mMaxVelocity()
{
    mName = "GunnsOptimParticleSwarm";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Particle Swarm Optimizer.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimParticleSwarm::~GunnsOptimParticleSwarm()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Pointer to the source configuration data object to copy.
///
/// @throws   std::runtime_error
///
/// @details  Checks the given configuration data is the correct type, then copies its values into
///           our internal config data object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::setConfigData(const GunnsOptimBaseConfigData* configData)
{
    const GunnsOptimParticleSwarmConfigData* config = dynamic_cast<const GunnsOptimParticleSwarmConfigData*>(configData);
    if (config) {
        mConfigData = *config;
    } else {
        throw std::runtime_error(mName + " bad config data type.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inStatesMaster (--) Pointer to the MC input variables description.
///
/// @throws   std::range_error, std::runtime_error
///
/// @details  Initializes this PSO.  Validates the configuration, initializes the swarm states,
///           establishes maximum velocity for each state parameter, writes header rows to the
///           output files.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::initialize(const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster)
{
    /// - Store the pointer to the MC input variables, validate, and seed the RNG.
    mInStatesMaster = inStatesMaster;
    validate();
    std::srand(mConfigData.mRandomSeed);

    /// - Create and initialize the swarm particles.
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        mParticles.push_back(GunnsOptimParticleSwarmParticle());
        mParticles.at(i).initialize(mInStatesMaster->size());
    }
    mActiveParticle = &mParticles.at(0);

    /// - Initialize the global best state and compute the max velocity for each state parameter.
    ///   mConfigData.mMaxVelocity is a scale factor on the range between min & max values of the
    ///   state parameter.  So mConfigData.mMaxVelocity = 1 limits maximum velocity to +/- range
    ///   per epoch.
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        mGlobalBestState.mState.push_back(0.0);
        mMaxVelocity.push_back(mConfigData.mMaxVelocity
                * (mInStatesMaster->at(i).mMaximum - mInStatesMaster->at(i).mMinimum));
    }

    /// - Initialize the particle states and run counters.
    initSwarm();
    mGlobalRunCounter = -1;
    mRunCounter       = -1;
    mEpoch            =  1;

    /// - Start the global cost/epoch history file
    if (mConfigData.mInitDistribution != GunnsOptimParticleSwarmConfigData::FILE_CONTINUOUS) {
        std::string pathFile = "pso_cost_history.csv";
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
        if (file.fail()) {
            throw std::runtime_error(mName + " error opening file: " + pathFile);
        } else {
            /// - Write the header row.
            file << "Epoch,Global_Best_Cost" << std::endl;
        }
        file.close();
    }

    /// - Start the swarm state history file
    if (mConfigData.mInitDistribution != GunnsOptimParticleSwarmConfigData::FILE_CONTINUOUS) {
        std::string pathFile = "pso_swarm_history.csv";
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
        if (file.fail()) {
            throw std::runtime_error(mName + " error opening file: " + pathFile);
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::range_error
///
/// @details  Unlike the rest of GUNNS, here we don't use the H&S system or TsException types and
///           opt to just throw standard exceptions.  Because this MC stuff could be used to
///           optimize non-GUNNS models, the user might not want to bother setting up the H&S.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::validate()
{
    GunnsOptimBase::validate();

    /// - Throw if swarm size is zero.
    if (mConfigData.mNumParticles < 1) {
        throw std::range_error(mName + " config data # particles < 1.");
    }

    /// - Throw if number of epochs is zero.
    if (mConfigData.mMaxEpoch < 1) {
        throw std::range_error(mName + " config data max epoch < 1.");
    }

    /// - Throw if inertia weights <= 0.
    if (mConfigData.mInertiaWeight < DBL_EPSILON) {
        throw std::range_error(mName + " config data inertia weight <= 0.");
    }
    if (mConfigData.mInertiaWeightEnd < DBL_EPSILON) {
        throw std::range_error(mName + " config data ending inertia weight <= 0.");
    }

    /// - Throw if coefficients <= 0.
    if (mConfigData.mCognitiveCoeff < DBL_EPSILON) {
        throw std::range_error(mName + " config data cognitive coefficient <= 0.");
    }
    if (mConfigData.mSocialCoeff < DBL_EPSILON) {
        throw std::range_error(mName + " config data social coefficient <= 0.");
    }

    /// - Throw if max velocity out of range.
    if (mConfigData.mMaxVelocity < DBL_EPSILON) {
        throw std::range_error(mName + " config data max velocity <= 0.");
    }
    if (mConfigData.mMaxVelocity > 1.0) {
        throw std::range_error(mName + " config data max velocity > 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error, std::range_error
///
/// @details  Initializes the particle states to the desired distribution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::initSwarm()
{
    switch (mConfigData.mInitDistribution) {
        case (GunnsOptimParticleSwarmConfigData::RANDOM) :
            std::cout << "init RANDOM\n";
            randomizeSwarmState();
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsOptimParticleSwarmConfigData::MIN_MAX_CORNERS) :
            std::cout << "init MIN_MAX_CORNERS\n";
            minMaxSwarmState();
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsOptimParticleSwarmConfigData::FILE) :
            std::cout << "init FILE\n";
            readFileSwarmState(false);
            randomizeSwarmVelocity();
            initBestCosts();
            break;
        case (GunnsOptimParticleSwarmConfigData::FILE_CONTINUOUS) :
            std::cout << "init FILE_CONTINUOUS\n";
            readFileSwarmState(true);
            printStates();
            break;
        default :   // invalid selection
            throw std::range_error(mName + " invalid initial swarm state distribution selection.");
            break;
    };
    applyStateConstraints();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Initializes the global best state cost and all particle's best state cost to a high
///           number for improvement during the optimization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::initBestCosts()
{
    mGlobalBestState.mCost = DBL_MAX;
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        mParticles.at(i).mBestState.mCost = DBL_MAX;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets each dimension of each particle's current state position to a random value
///           within the state's min & max range.  This is a uniform random distribution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::randomizeSwarmState()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        GunnsOptimParticleSwarmState& state = mParticles.at(i).mCurrentState;
        for (unsigned int j=0; j<state.mState.size(); ++j) {
            const double range = mInStatesMaster->at(j).mMaximum - mInStatesMaster->at(j).mMinimum;
            state.mState.at(j) = mInStatesMaster->at(j).mMinimum + range * uniformRand();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the current state of the first half of the swarm particles to (min, min, ...),
///           and the second half to (max, max, ...).
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::minMaxSwarmState()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        GunnsOptimParticleSwarmState& state = mParticles.at(i).mCurrentState;
        for (unsigned int j=0; j<state.mState.size(); ++j) {
            if (i < mConfigData.mNumParticles / 2) {
                state.mState.at(j) = mInStatesMaster->at(j).mMinimum;
            } else {
                state.mState.at(j) = mInStatesMaster->at(j).mMaximum;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] continuous (--) If true, also reads velocity and best state.
///
/// @throws   std::runtime_error
///
/// @details  Sets the global best state and particle current states to values read from the swarm
///           state file.  The continuous argument, if set, causes us to also read the particle
///           velocities and particle personal best state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::readFileSwarmState(const bool continuous)
{
    std::string pathFile = "pso_state.csv";
    std::ifstream file (pathFile.c_str(), (std::ifstream::in));
    if (file.fail()) {
        throw std::runtime_error(mName + " error opening file: " + pathFile);
    } else {
        /// - Read the whole file into a stream.
        std::stringstream fString;
        fString << file.rdbuf();

        /// - Split the stream by separate lines.
        std::vector<std::string> fLines = Strings::split(fString.str(), "\n");

        /// - Check the file for correct sizes.
        const unsigned int expectedRows = 3 + mConfigData.mNumParticles;
        const unsigned int expectedCols = 4 + 3 * mInStatesMaster->size();

        if (expectedRows != fLines.size()) {
            throw std::runtime_error(mName + " file has wrong number of rows: " + pathFile);
        }
        if (expectedCols != Strings::split(fLines.at(0), " ").size()) {
            throw std::runtime_error(mName + " file has wrong number of columns: " + pathFile);
        }

        for (unsigned int line=1; line<fLines.size(); ++line) {
            std::istringstream in(fLines.at(line));
            std::string particle;  // dummy to hold the 0th column value
            int         epoch;     // dummy to hold the epoch value
            in >> particle;
            in >> epoch;
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets each dimension of each particle's current state velocity to a random value within
///           the +/- maximum velocity for that state..  This is a uniform random distribution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::randomizeSwarmVelocity()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        GunnsOptimParticleSwarmState& state = mParticles.at(i).mCurrentState;
        for (unsigned int j=0; j<state.mVelocity.size(); ++j) {
            state.mVelocity.at(j) = mMaxVelocity.at(j) * (1.0 - 2.0 * uniformRand());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Updates the swarm between each particle run.  Each epoch runs each particle once.
///           We update the active particle pointer so that when the MC manager calls getState, we
///           return the state for the particle that's next up to run.  At the start of a new epoch,
///           the entire swarm is propagated to its next state, and the results of the previous
///           epoch are written to the output files.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::update()
{
    mGlobalRunCounter++;
    mRunCounter++;

    /// - Updates for the next epoch.
    if (mRunCounter >= static_cast<int>(mConfigData.mNumParticles)) {
        mRunCounter = 0;
        mEpoch++;

        /// - Ramp the global particle inertia weight towards its ending value.
        double inertiaWeight = mConfigData.mInertiaWeight
                             + (mConfigData.mInertiaWeightEnd - mConfigData.mInertiaWeight)
                             * mEpoch / mConfigData.mMaxEpoch;

        // propagate the swarm
        propagateSwarm(inertiaWeight);
        printStates();
        printGlobalBest();

        /// - Append to the global cost/epoch history file
        {
            std::string pathFile = "pso_cost_history.csv";
            std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::app));
            if (file.fail()) {
                throw std::runtime_error(mName + " error opening file: " + pathFile);
            } else {
                /// - Write the data row for this epoch.
                file << mEpoch << "," << mGlobalBestState.mCost << std::endl;
            }
            file.close();
        }

        /// - Append to the swarm state history file
        {
            std::string pathFile = "pso_swarm_history.csv";
            std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::app));
            if (file.fail()) {
                throw std::runtime_error(mName + " error opening file: " + pathFile);
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

    /// - Point the active particle to the particle that's up next.
    mActiveParticle = &mParticles.at(mRunCounter);
    mActiveParticle->mCurrentState.mRunId = mGlobalRunCounter;

    if (mVerbosityLevel > 0) {
        std::cout << "PSO update Epoch " << mEpoch << ", run " << mRunCounter << ", best cost: "
                  << mGlobalBestState.mCost << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the personal best state of each particle, and updates the global best state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::updateBestStates()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        // reference to the particle's current state object
        GunnsOptimParticleSwarmState& state = mParticles.at(i).mCurrentState;

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inertiaWeight (--) The current swarm inertia weight.
///
/// @details  Propagates the swarm particle states.  Acceleration vector in the state space is set
///           to a new mix of the error vectors between the current state position and the personal
///           and global best positions, weighted and given random magnitudes.  Acceleration is
///           integrated into velocity and velocity into position, as you would in physics.
///           However, velocity in each state component is limited to its maximum range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::propagateSwarm(const double inertiaWeight)
{
    updateBestStates();

    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {

        /// - Deltas from particle's current state to global best and personal best states.
        GunnsOptimParticleSwarmState& state = mParticles.at(i).mCurrentState;
        std::vector<double> globalStateDelta;
        std::vector<double> personalStateDelta;
        globalStateDelta.clear();
        personalStateDelta.clear();
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            globalStateDelta.push_back(mGlobalBestState.mState.at(j) - state.mState.at(j));
            personalStateDelta.push_back(mParticles.at(i).mBestState.mState.at(j) - state.mState.at(j));
        }

        /// - Update particle state, limiting velocity before integrating into position.
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            state.mAcceleration.at(j) = mConfigData.mCognitiveCoeff * uniformRand() * personalStateDelta.at(j)
                                      + mConfigData.mSocialCoeff    * uniformRand() * globalStateDelta.at(j);
            state.mVelocity.at(j) = inertiaWeight * state.mVelocity.at(j) + state.mAcceleration.at(j);
            if (state.mVelocity.at(j) > mMaxVelocity.at(j)) {
                state.mVelocity.at(j) = mMaxVelocity.at(j);
            } else if (state.mVelocity.at(j) < -mMaxVelocity.at(j)) {
                state.mVelocity.at(j) = -mMaxVelocity.at(j);
            }
            state.mState.at(j) += state.mVelocity.at(j);
        }

        /// - State space boundary check & correction.  We implement reflection since it seems to
        ///   work better than wrap-around.  For reflection, when we cross a state boundary, we set
        ///   the state position to the boundary and reverse its velocity.  This is done
        ///   independently for each state component.
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            if (state.mState.at(j) < mInStatesMaster->at(j).mMinimum) {
                state.mState.at(j) = mInStatesMaster->at(j).mMinimum;
                state.mVelocity.at(j) *= -1.0;
            } else if (state.mState.at(j) > mInStatesMaster->at(j).mMaximum) {
                state.mState.at(j) = mInStatesMaster->at(j).mMaximum;
                state.mVelocity.at(j) *= -1.0;
            }
        }
    }

    applyStateConstraints();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Applies the MC input variable constraints to all particle state positions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::applyStateConstraints()
{
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        GunnsOptimParticleSwarmState& state = mParticles.at(i).mCurrentState;
        constrainInputs(state.mState);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] cost          (--) The cost value to assign to the particle.
/// @param[in] runId         (--) Not used.
/// @param[in] runIdReturned (--) The run ID associated with this cost value.
///
/// @throws   std::runtime_error
///
/// @details  Sets the cost function value result from the MC Slave run to the particle state that
///           was the input to that run.  The results come back from the Slave runs in a different
///           order than they are launched, because of variability in timing of the Slave runs on
///           their cores.  So we have to actively correlate the returned value with the run ID and
///           particle it corresponds to.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::assignCost(const double cost, const double runId __attribute__((unused)), const double runIdReturned)
{
    /// - Find the particle whose latest run ID matches the returned ID, and assign this cost to it.
    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
        if (runIdReturned == mParticles.at(i).mCurrentState.mRunId) {
            mParticles.at(i).mCurrentState.mCost = cost;
            return;
        }
    }

    /// - If there was no match, something has gone very wrong.
    std::ostringstream msg;
    msg << mName << " run ID: " << runIdReturned << ", no match to any particle.";
    throw std::runtime_error(msg.str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Outputs the current particle states to the console.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::printStates() const
{
    if (mVerbosityLevel > 0) {
        std::cout << "PSO particle states: ";
        for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
            const GunnsOptimParticleSwarmState& state = mParticles.at(i).mCurrentState;

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
                /// - Format the acceleration as scientific for now, but we might need this on the
                ///   others as well, depending on the state space size.  The default cout tends to
                ///   truncate too much.
                printf(", %e", state.mAcceleration.at(j));
            }
        }
        std::cout << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Outputs the current global best state to the console.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::printGlobalBest() const
{
    if (mVerbosityLevel > 0) {
        std::cout << "PSO global best state: ";
        for (unsigned int i=0; i<mGlobalBestState.mState.size(); ++i) {
            std::cout << mGlobalBestState.mState.at(i) << ", ";
        }
        std::cout << " cost: " << mGlobalBestState.mCost << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Writes the final states to the output files.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimParticleSwarm::shutdown()
{
    printGlobalBest();

    /// - Write the swarm state to an output file.
    std::string pathFile = "pso_state.csv";
    std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
    if (file.fail()) {
        throw std::runtime_error(mName + " error opening file: " + pathFile);
    } else {
        /// - Write the header row.
        file << "Particle epoch cost";
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
        file << "global_best " << mEpoch << " " << mGlobalBestState.mCost;
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " " << mGlobalBestState.mState.at(j);
        }
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " 0.0"; // global best state is position only
        }
        file << " " << mGlobalBestState.mCost;
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << " " << mGlobalBestState.mState.at(j);
        }
        file << std::endl;

        /// - Write a data row for each particle state.
        for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
            file << i << " " << mEpoch << " " << mParticles.at(i).mCurrentState.mCost;
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
