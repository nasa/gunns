/**
@file     GunnsOptimGradientDescent.cpp
@brief    GUNNS Gradient Descent Optimization implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((core/optimization/GunnsOptimGradientDescent.o))
*/

/// - GUNNS includes:
#include "GunnsOptimGradientDescent.hh"
#include "core/GunnsMacros.hh"
#include "math/MsMath.hh"
#include "strings/Strings.hh"

/// - System includes:
#include <cfloat>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

//TODO
GunnsOptimGradientDescentState::GunnsOptimGradientDescentState(const unsigned int size)
    :
    mState(),
    mDeltaState(0.0),
    mCost(0.0),
    mDeltaCost(0.0),
    mCostGradient(0.0),
    mRunId(-99.99)
{
    for (unsigned int i=0; i<size; ++i) {
        mState.push_back(0.0);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) The object to be assigned equal to.
///
/// @returns  GunnsOptimGradientDescentState& (--) Reference to this object.
///
/// @details  Assigns this GUNNS Gradient Descent Optimizer state to the values of the given state
///           object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimGradientDescentState& GunnsOptimGradientDescentState::operator =(const GunnsOptimGradientDescentState& that)
{
    if (this != &that) {
        this->mState        = that.mState;
        this->mCost         = that.mCost;
        this->mCostGradient = that.mCostGradient;
        //TODO
//        this->mRunId        = that.mRunId;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Gradient Descent Optimizer configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimGradientDescentConfigData::GunnsOptimGradientDescentConfigData()
    :
    mNumVars(0),
    mMaxEpoch(0),
    mPropagationGain(0.0),
    mRandomSeed(0),
    mInitState(RANDOM)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gradient Descent Optimizer configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimGradientDescentConfigData::~GunnsOptimGradientDescentConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) The object to be assigned equal to.
///
/// @returns  GunnsOptimGradientDescentConfigData& (--) Reference to this object.
///
/// @details  Assigns this GUNNS Gradient Descent Optimizer configuration data to the values of the
///           given configuration data object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimGradientDescentConfigData& GunnsOptimGradientDescentConfigData::operator =(const GunnsOptimGradientDescentConfigData& that)
{
    if (this != &that) {
        mNumVars         = that.mNumVars;
        mMaxEpoch        = that.mMaxEpoch;
        mPropagationGain = that.mPropagationGain;
        mRandomSeed      = that.mRandomSeed;
        mInitState       = that.mInitState;
    }
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Gradient Descent Optimizer.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimGradientDescent::GunnsOptimGradientDescent()
    :
    GunnsOptimBase(),
    mConfigData(),
    mGradients(),
    mActiveState(0),
    mState()
{
    mName = "GunnsOptimGradientDescent";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Gradient Descent Optimizer.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimGradientDescent::~GunnsOptimGradientDescent()
{
    delete mState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) Pointer to the source configuration data object to copy.
///
/// @throws   std::runtime_error
///
/// @details  Checks the given configuration data is the correct type, then copies its values into
///           our internal config data object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::setConfigData(const GunnsOptimBaseConfigData* configData)
{
    const GunnsOptimGradientDescentConfigData* config = dynamic_cast<const GunnsOptimGradientDescentConfigData*>(configData);
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
/// @details  Initializes this Gradient Descent Optimizer.  Validates the configuration,
///           initializes the state, TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::initialize(const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster)
{
    /// - Store the pointer to the MC input variables, validate, and seed the RNG.
    mInStatesMaster = inStatesMaster;
    validate();
    std::srand(mConfigData.mRandomSeed);

    //TODO create 2*N gradient objects, an increase & decrease for each MC variable N
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        mGradients.push_back(GunnsOptimGradientDescentState(mInStatesMaster->size()));
        mGradients.push_back(GunnsOptimGradientDescentState(mInStatesMaster->size()));
    }

    //TODO create the global state object
    mState = new GunnsOptimGradientDescentState(mInStatesMaster->size());

    mActiveState = &mGradients.at(0);

    /// - Initialize the states and run counters.
    initState();
    mGlobalRunCounter = -1;
    mRunCounter       = -1;
    mEpoch            =  1;
//
//    /// - Start the global cost/epoch history file
//    {
//        std::string pathFile = "pso_cost_history.csv";
//        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
//        if (file.fail()) {
//            throw std::runtime_error(mName + " error opening file: " + pathFile);
//        } else {
//            /// - Write the header row.
//            file << "Epoch,Global_Best_Cost " << std::endl;
//        }
//        file.close();
//    }
//
//    /// - Start the swarm state history file
//    {
//        std::string pathFile = "pso_swarm_history.csv";
//        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
//        if (file.fail()) {
//            throw std::runtime_error(mName + " error opening file: " + pathFile);
//        } else {
//            /// - Write the header row: Epoch,Pos_0_0,Pos_0_1,Cost_0,Pos_1_0,Pos_1_1,Cost_1, ... ,Pos_Best_0, ...
//            file << "Epoch";
//            for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
//                for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//                    file << ",Pos_" << i << "_" << j;
//                }
//                file << ",Cost_" << i;
//            }
//            for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//                file << ",Pos_Best" << "_" << j;
//            }
//            file << ",Cost_Best";
//            file << std::endl;
//        }
//        file.close();
//    }
//
//    printStates();
//    printGlobalBest();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::range_error
///
/// @details  Unlike the rest of GUNNS, here we don't use the H&S system or TsException types and
///           opt to just throw standard exceptions.  Because this MC stuff could be used to
///           optimize non-GUNNS models, the user might not want to bother setting up the H&S.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::validate()
{
    GunnsOptimBase::validate();

    /// - Throw if MC variables size is zero.
    if (mConfigData.mNumVars < 1) {
        throw std::range_error(mName + " config data # MC variables < 1.");
    }

    /// - Throw if number of epochs is zero.
    if (mConfigData.mMaxEpoch < 1) {
        throw std::range_error(mName + " config data max epoch < 1.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Initializes the state by the prescribed method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::initState()
{
    //TODO
    switch (mConfigData.mInitState) {
        case (GunnsOptimGradientDescentConfigData::RANDOM) :
            randomizeState();
//            randomizeSwarmVelocity();
//            initBestCosts();
            break;
        case (GunnsOptimGradientDescentConfigData::FILE) :
            readFileState();
//            randomizeSwarmVelocity();
//            initBestCosts();
            break;
        case (GunnsOptimGradientDescentConfigData::MIN_CORNER) :
            setMinCornerState();
//            randomizeSwarmVelocity();
//            initBestCosts();
            break;
        case (GunnsOptimGradientDescentConfigData::MAX_CORNER) :
            setMaxCornerState();
//            randomizeSwarmVelocity();
//            initBestCosts();
            break;
        case (GunnsOptimGradientDescentConfigData::MIDRANGE) :
            setMedianState();
//            printStates();
            break;
        default :   // invalid selection
            randomizeState();
            break;
    };
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets each MC variable to a random position within its min & max range.  This is a
///           uniform random distribution.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::randomizeState()
{
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        const double range = mInStatesMaster->at(i).mMaximum - mInStatesMaster->at(i).mMinimum;
        mState->mState.at(i) = mInStatesMaster->at(i).mMinimum + range * uniformRand();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the state for each MC variable to the minimum of its range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::setMinCornerState()
{
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        mState->mState.at(i) = mInStatesMaster->at(i).mMinimum;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the state for each MC variable to the maximum of its range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::setMaxCornerState()
{
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        mState->mState.at(i) = mInStatesMaster->at(i).mMaximum;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the state for each MC variable to the median of its range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::setMedianState()
{
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        const double range = mInStatesMaster->at(i).mMaximum - mInStatesMaster->at(i).mMinimum;
        mState->mState.at(i) = mInStatesMaster->at(i).mMinimum + range * 0.5;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Sets the TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::readFileState()
{
    //TODO truth target:
    mState->mState.at(0) = 0.001;
    mState->mState.at(1) = 0.002;
    mState->mState.at(2) = 0.0005;
    mState->mState.at(3) = 0.00025;
    // let's start at truth + 10%:
    mState->mState.at(0) = 0.0011;
    mState->mState.at(1) = 0.0022;
//    mState->mState.at(2) = 0.00050;
//    mState->mState.at(3) = 0.000275;

//    std::string pathFile = "pso_state.csv";
//    std::ifstream file (pathFile.c_str(), (std::ifstream::in));
//    if (file.fail()) {
//        throw std::runtime_error(mName + " error opening file: " + pathFile);
//    } else {
//        /// - Read the whole file into a stream.
//        std::stringstream fString;
//        fString << file.rdbuf();
//
//        /// - Split the stream by separate lines.
//        std::vector<std::string> fLines = Strings::split(fString.str(), "\n");
//
//        /// - Check the file for correct sizes.
//        const unsigned int expectedRows = 2 + mConfigData.mNumParticles;
//        const unsigned int expectedCols = 3 + 3 * mInStatesMaster->size();
//
//        if (expectedRows != fLines.size()) {
//            throw std::runtime_error(mName + " file has wrong number of rows: " + pathFile);
//        }
//        if (expectedCols != Strings::split(fLines.at(0), " ").size()) {
//            throw std::runtime_error(mName + " file has wrong number of columns: " + pathFile);
//        }
//
//        for (unsigned int line=1; line<fLines.size(); ++line) {
//            std::istringstream in(fLines.at(line));
//            std::string particle;  // dummy to hold the 0th column value
//            in >> particle;
//            if (line < 2) {
//                /// - Initialize the global best state from the 1st line (header is 0th line).
//                in >> mGlobalBestState.mCost;
//                for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
//                    in >> mGlobalBestState.mState.at(i);
//                }
//            } else if (line < mParticles.size() + 2) {
//                /// - Initialize the particle states from the subsequent lines.
//                in >> mParticles.at(line-2).mCurrentState.mCost;
//                for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
//                    in >> mParticles.at(line-2).mCurrentState.mState.at(i);
//                }
//                /// - For continuous propagation, also read the velocity and best state.
//                if (continuous) {
//                    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
//                        in >> mParticles.at(line-2).mCurrentState.mVelocity.at(i);
//                    }
//                    in >> mParticles.at(line-2).mBestState.mCost;
//                    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
//                        in >> mParticles.at(line-2).mBestState.mState.at(i);
//                    }
//                }
//            }
//        }
//    }
//    file.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets each dimension of each particle's current state velocity to a random value within
///           the +/- maximum velocity for that state..  This is a uniform random distribution.
////////////////////////////////////////////////////////////////////////////////////////////////////
//void GunnsOptimGradientDescent::randomizeSwarmVelocity()
//{
//    for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
//        GunnsOptimGradientDescentState& state = mParticles.at(i).mCurrentState;
//        for (unsigned int j=0; j<state.mVelocity.size(); ++j) {
//            state.mVelocity.at(j) = mMaxVelocity.at(j) * (1.0 - 2.0 * uniformRand());
//        }
//    }
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Updates the TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::update()
{
    mGlobalRunCounter++;
    mRunCounter++;

    // start with initial Global State
    // 1. Run Global State slave and get its cost: assignCost()
    // 2. compute delta-states for the Gradients: setGradientDeltaStates()
    // 3. run Gradient slaves and get their costs
    // 4. compute the gradient's cost gradients, as each finishes: assignCost()
    // 5. Propagate the Global State: propagateState()
    // 6. go to 1 & repeat

    if (1 == mRunCounter) {
        setGradientStates();
        std::cout << "Gradient States:" << std::endl;
        for (unsigned int i=0; i<mGradients.size(); ++i) {
            std::cout << "  " << i;
            for (unsigned int j=0; j<mGradients.at(i).mState.size(); ++j) {
                std::cout << ", " << mGradients.at(i).mState.at(j);
            }
            std::cout << std::endl;
        }
    } else if (mRunCounter > (mGradients.size())) {
        /// - Counter has just incremented past the last gradient state, so reset the counter,
        ///   propagate and global state.
        mRunCounter = 0;
        mEpoch++;

        // propagate the state
        propagateState(mConfigData.mPropagationGain);
        printStates();

//        /// - Append to the global cost/epoch history file
//        {
//            std::string pathFile = "pso_cost_history.csv";
//            std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::app));
//            if (file.fail()) {
//                throw std::runtime_error(mName + " error opening file: " + pathFile);
//            } else {
//                /// - Write the data row for this epoch.
//                file << mEpoch << "," << mGlobalBestState.mCost << std::endl;
//            }
//            file.close();
//        }
//
//        /// - Append to the swarm state history file
//        {
//            std::string pathFile = "pso_swarm_history.csv";
//            std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::app));
//            if (file.fail()) {
//                throw std::runtime_error(mName + " error opening file: " + pathFile);
//            } else {
//                /// - Write the data row for this epoch.
//                file << mEpoch;
//                for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
//                    for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//                        file << "," << mParticles.at(i).mCurrentState.mState.at(j);
//                    }
//                    file << "," << mParticles.at(i).mCurrentState.mCost;
//                }
//                for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//                    file << "," << mGlobalBestState.mState.at(j);
//                }
//                file << "," << mGlobalBestState.mCost << std::endl;
//            }
//            file.close();
//        }
    }

    /// - Point the MC manager to the state that's up next.  The global state goes on run 0,
    ///   followed by the gradient states.
    if (0 == mRunCounter) {
        mActiveState = mState;
    } else {
        mActiveState = &mGradients.at(mRunCounter - 1);
    }
    mActiveState->mRunId = mGlobalRunCounter;

    if (mVerbosityLevel > 0) {
        std::cout << "Gradient update Epoch " << mEpoch << ", run " << mRunCounter << ", best cost: "
                  << mState->mCost << std::endl;
    }
}

// TODO the sequence:
// start with initial Global State
// 1. Run Global State slave and get its cost: assignCost()
// 2. compute delta-states for the Gradients: setGradientDeltaStates()
// 3. run Gradient slaves and get their costs
// 4. compute the gradient's cost gradients, as each finishes: assignCost()
// 5. Propagate the Global State: propagateState()
// 6. go to 1 & repeat
//
// So, for N MC variables, the batch run sizes alternate between
// 1 for the Global and 2N for the gradients, so for N=4:
// 1, 8, 1, 8, 1, 8, ...
// Contrast to PSO which is always the same:
// 30, 30, 30, 30, ...
// Since this alternating batch size is more complicated, we won't bother to parallelize it until
// the Trick MC upgrade, and for now this prototype will assume 1 Slave core, all runs in series

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] gain (--) The current propagation gain.
///
/// @details  Propagates the optimizer state.  TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::propagateState(const double gain)
{
    // for each gradient we have to decide whether we want to use its increase or decrease gradient
    // we use the one that has the better (larger negative number) delta-cost
    std::cout << "propagateState:" << std::endl;
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {

        //TODO might be a simpler way to do this?
        const unsigned int gInc = i * 2;
        const unsigned int gDec = gInc + 1;

        int useGradient = -1; // use neither
        if (mGradients.at(gInc).mDeltaCost < 0.0) {
            useGradient = 0; // use the state increase
        }
        if (mGradients.at(gDec).mDeltaCost < 0.0 and
            mGradients.at(gDec).mDeltaCost < mGradients.at(gInc).mDeltaCost) {
            useGradient = 1; // use the state decrease
        }

        // solve costGradient = d(cost)/d(state) for d(state) to make cost = 0.
        // d(state) = d(cost) / costGradient
        // d(cost) = 0 - cost
        // d(state) = -cost / costGradient
        //
        // example say cost function = 4*state - 2
        // for cost = 0 = 4*state - 2, 2 = 4*state, state = 0.5
        // say current state = 10
        // current cost = 4*10 - 2 = 38
        // gradient = d(cost)/d(state) = d(4*state - 2)/d(state) = 4
        // d(state) is then -cost / gradient = -38 / 4 = -9.5, GOOD

        // now propagate the state by the chosen gradient
        double deltaState = 0.0;
        if (0 == useGradient) {
            // the cost improves (decreases) with an increase in the state, but the gradient is negative,
            // and we want the state to increase, so flip the negative gradient sign to positive
            deltaState = -1.0 * mState->mCost / mGradients.at(gInc).mCostGradient;
            std::cout << i << ", " << useGradient << ", " << deltaState << ", " << mState->mCost << ", " << mGradients.at(gInc).mCostGradient << std::endl;
        } else if (1 == useGradient) {
            // the cost improves (decreases) with a decrease in the state, so the gradient is positive,
            // but we want the state to decrease so flip the positive gradient sign to negative.
            deltaState = -1.0 * mState->mCost / mGradients.at(gDec).mCostGradient;
            std::cout << i << ", " << useGradient << ", " << deltaState << ", " << mState->mCost << ", " << mGradients.at(gDec).mCostGradient << std::endl;
        }

        // all that assumes one state by itself, so we divide it by the number of states to
        // combine them, and then apply the propagation gain
        mState->mState.at(i) += deltaState * mConfigData.mPropagationGain / mInStatesMaster->size();

        // limit state to its min/max range
        mState->mState.at(i) = MsMath::limitRange(mInStatesMaster->at(i).mMinimum,
                                                  mState->mState.at(i),
                                                  mInStatesMaster->at(i).mMaximum);
    }
}

//TODO
void GunnsOptimGradientDescent::setGradientStates()
{
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        const unsigned int gInc = i * 2;
        const unsigned int gDec = gInc + 1;

        // copy the global state into the gradient, then delta will be applied below.
        mGradients.at(gInc) = *mState;
        mGradients.at(gDec) = *mState;

        // now compute and load the new gradient states from this, limit to the state range
        const double stateDeltaFactor = 0.001; //TODO delta is 0.1% of state TODO config?

        double previousState = mGradients.at(gInc).mState.at(i);
        mGradients.at(gInc).mState.at(i) += stateDeltaFactor * previousState;
        mGradients.at(gInc).mState.at(i) = MsMath::limitRange(mInStatesMaster->at(i).mMinimum,
                                                              mGradients.at(gInc).mState.at(i),
                                                              mInStatesMaster->at(i).mMaximum);
        mGradients.at(gInc).mDeltaState = mGradients.at(gInc).mState.at(i) - previousState;

        previousState = mGradients.at(gDec).mState.at(i);
        mGradients.at(gDec).mState.at(i) -= stateDeltaFactor * previousState;
        mGradients.at(gDec).mState.at(i) = MsMath::limitRange(mInStatesMaster->at(i).mMinimum,
                                                              mGradients.at(gDec).mState.at(i),
                                                              mInStatesMaster->at(i).mMaximum);
        mGradients.at(gDec).mDeltaState = mGradients.at(gDec).mState.at(i) - previousState;
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
void GunnsOptimGradientDescent::assignCost(const double cost, const double runId __attribute__((unused)), const double runIdReturned)
{
    /// - Find the state whose latest run ID matches the returned ID, and assign this cost and the
    ///   computed cost gradient to it.
    for (unsigned int i=0; i<mGradients.size(); ++i) {
        if (runIdReturned == mGradients.at(i).mRunId) {
            mGradients.at(i).mDeltaCost = cost - mState->mCost;
            double costGradient = 0.0;
            if (fabs(mGradients.at(i).mDeltaState) > DBL_EPSILON) {
                costGradient = mGradients.at(i).mDeltaCost / mGradients.at(i).mDeltaState;
            }
            mGradients.at(i).mCostGradient = costGradient;
            mGradients.at(i).mCost         = cost;
            std::cout << "gradient: " << i << ", cost: " << cost << ", gradient: " << costGradient << std::endl;
            return;
        }
    }
    if (runIdReturned == mState->mRunId) {
        mState->mCostGradient = 0.0; // unused
        mState->mCost         = cost;
        std::cout << "mState cost: " << cost << std::endl;
        return;
    }

    /// - If there was no match, something has gone very wrong.
    std::ostringstream msg;
    msg << mName << " run ID: " << runIdReturned << ", no match to any state.";
    throw std::runtime_error(msg.str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Outputs the current particle states to the console.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::printStates() const
{
    if (mVerbosityLevel > 0) {
        std::cout << "Cost Gradients states: ";
        for (unsigned int i=0; i<mGradients.size(); ++i) {
            std::cout << std::endl << "  " << i;
            std::cout << ", " << mGradients.at(i).mCostGradient;
//            for (unsigned int j=0; j<mGradients.at(i).mState.size(); ++j) {
//                std::cout << ", " << mGradients.at(i).mState.at(j);
//            }
        }
        std::cout << std::endl;

        std::cout << "Global state: ";
        for (unsigned int i=0; i<mState->mState.size(); ++i) {
            std::cout << mState->mState.at(i) << ", ";
        }
        std::cout << " cost: " << mState->mCost << std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Outputs the current global best state to the console.
////////////////////////////////////////////////////////////////////////////////////////////////////
//void GunnsOptimGradientDescent::printGlobalBest() const
//{
//    if (mVerbosityLevel > 0) {
//        std::cout << "PSO global best state: ";
//        for (unsigned int i=0; i<mGlobalBestState.mState.size(); ++i) {
//            std::cout << mGlobalBestState.mState.at(i) << ", ";
//        }
//        std::cout << " cost: " << mGlobalBestState.mCost << std::endl;
//    }
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Writes the final states to the output files.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::shutdown() const
{
    printStates();

//    /// - Write the swarm state to an output file.
//    std::string pathFile = "pso_state.csv";
//    std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
//    if (file.fail()) {
//        throw std::runtime_error(mName + " error opening file: " + pathFile);
//    } else {
//        /// - Write the header row.
//        file << "Particle cost";
//        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//            file << " pos_" << j;
//        }
//        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//            file << " vel_" << j;
//        }
//        file << " best_cost";
//        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//            file << " best_pos_" << j;
//        }
//        file << std::endl;
//
//        /// - Write the first data row as the global best state.
//        file << "global_best " << mGlobalBestState.mCost;
//        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//            file << " " << mGlobalBestState.mState.at(j);
//        }
//        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//            file << " 0.0"; // global best state is position only
//        }
//        file << " " << mGlobalBestState.mCost;
//        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//            file << " " << mGlobalBestState.mState.at(j);
//        }
//        file << std::endl;
//
//        /// - Write a data row for each particle state.
//        for (unsigned int i=0; i<mConfigData.mNumParticles; ++i) {
//            file << i << " " << mParticles.at(i).mCurrentState.mCost;
//            for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//                file << " " << mParticles.at(i).mCurrentState.mState.at(j);
//            }
//            for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//                file << " " << mParticles.at(i).mCurrentState.mVelocity.at(j);
//            }
//            file << " " << mParticles.at(i).mBestState.mCost;
//            for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
//                file << " " << mParticles.at(i).mBestState.mState.at(j);
//            }
//            file << std::endl;
//        }
//    }
//    file.close();
}
