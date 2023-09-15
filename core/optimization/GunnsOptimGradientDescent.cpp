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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] size (--) The number of variables to be optimized.
///
/// @details  Default constructs this GUNNS Gradient Descent Optimizer State.
////////////////////////////////////////////////////////////////////////////////////////////////////
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
///           object.  This only assigns the state vector, and leaves the remaining variables alone.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimGradientDescentState& GunnsOptimGradientDescentState::operator =(const GunnsOptimGradientDescentState& that)
{
    if (this != &that) {
        this->mState = that.mState;
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
    mInitialState(0)
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
        mInitialState    = that.mInitialState;
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
/// @details  Initializes this Gradient Descent Optimizer.  Validates the configuration, initializes
///           the state and run counters.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::initialize(const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster)
{
    /// - Store the pointer to the MC input variables and validate the configuration data.
    mInStatesMaster = inStatesMaster;
    validate();

    /// - Create the 2*N gradient objects, an increase & decrease for each MC variable N.
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {
        mGradients.push_back(GunnsOptimGradientDescentState(mInStatesMaster->size()));
        mGradients.push_back(GunnsOptimGradientDescentState(mInStatesMaster->size()));
    }

    /// - Create the global state object.
    mState = new GunnsOptimGradientDescentState(mInStatesMaster->size());

    /// - Point the active MC state to the first gradient object.
    mActiveState = &mGradients.at(0);

    /// - Initialize the global state and run counters.
    initState();
    mGlobalRunCounter = -1;
    mRunCounter       = -1;
    mEpoch            =  1;

    /// - Start the global cost/epoch history file
    {
        std::string pathFile = "grad_cost_history.csv";
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
        if (file.fail()) {
            throw std::runtime_error(mName + " error opening file: " + pathFile);
        } else {
            /// - Write the header row.
            file << "Epoch,Global_Cost " << std::endl;
        }
        file.close();
    }

    /// - Start the gradients history file
    {
        std::string pathFile = "grad_gradients_history.csv";
        std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
        if (file.fail()) {
            throw std::runtime_error(mName + " error opening file: " + pathFile);
        } else {
            /// - Write the header row.
            file << "Epoch,Index,Direction,State,DeltaState,Cost,DeltaCost,CostGradient" << std::endl;
        }
        file.close();
    }

    printStates();
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
/// @details  Initializes the state by copying its values from the configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::initState()
{
    for (unsigned int i=0; i<mConfigData.mNumVars; ++i) {
        mState->mState.at(i) = MsMath::limitRange(mInStatesMaster->at(i).mMinimum,
                                                  mConfigData.mInitialState[i],
                                                  mInStatesMaster->at(i).mMaximum);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Main update function for the Gradient Descent Optimizer.  Increments the run counters,
///           updates the gradient states and propagates the global state.  The update sequence:
///           0. Start with the initial Global State
///           1. Run Global State slave and get its cost: assignCost()
///           2. Compute delta-states for the Gradients: setGradientDeltaStates()
///           3. Run Gradient slaves and compute their cost gradients as each finishes: assignCosts()
///           4. Propagate the Global State: propagateState()
///           5. Go to 1 & repeat
///
///           So, for N MC variables, the MC batch run sizes alternate between 1 for the Global and
///           2N for the gradients, so for N=4:
///              1, 8, 1, 8, 1, 8, ...
///           Contrast to PSO which is always the same:
///              30, 30, 30, 30, ...
///           TODO Since this alternating batch size is more complicated, we won't bother to
///           parallelize it until the Trick MC upgrade, and for now this implementation assumes 1
///           Slave core, all runs in series.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::update()
{
    mGlobalRunCounter++;
    mRunCounter++;

    if (1 == mRunCounter) {
        /// - On the 1st run of each epoch, compute all the gradient states for this epoch.
        setGradientStates();
        if (mVerbosityLevel > 0) {
            std::cout << "Gradient States:" << std::endl;
            for (unsigned int i=0; i<mGradients.size(); ++i) {
                std::cout << "  " << i;
                for (unsigned int j=0; j<mGradients.at(i).mState.size(); ++j) {
                    std::cout << ", " << mGradients.at(i).mState.at(j);
                }
                std::cout << std::endl;
            }
        }
    } else if (mRunCounter > (static_cast<int>(mGradients.size()))) {
        /// - On the 0th run of each epoch, propagate the global state.
        mRunCounter = 0;
        mEpoch++;

        propagateState(mConfigData.mPropagationGain);
        printStates();

        /// - Append to the global cost/epoch history file
        {
            std::string pathFile = "grad_cost_history.csv";
            std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::app));
            if (file.fail()) {
                throw std::runtime_error(mName + " error opening file: " + pathFile);
            } else {
                /// - Write the data row for this epoch.
                file << mEpoch << "," << mState->mCost << std::endl;
            }
            file.close();
        }
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] gain (--) The current propagation gain.
///
/// @details  Propagates the global state.  For each N optimized variables, this chooses which of
///           its gradients to propagate along, as the gradient that has the lowest delta-cost
///           (largest negative number) of the two.  If both delta-costs are zero, then the variable
///           has reached its final value and won't be improved further.  Then the global state is
///           updated as the integral, from its previous state, of the chosen gradients.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::propagateState(const double gain)
{
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {

        /// - Compute indexes of the increase and decrease delta-state gradient objects for this
        ///   optimizer variable.
        const unsigned int gInc = i * 2;
        const unsigned int gDec = gInc + 1;

        /// - Choose which gradient to use, or neither.
        int useGradient = -1;       // use neither
        if (mGradients.at(gInc).mDeltaCost < 0.0 and mGradients.at(gInc).mCostGradient != 0.0) {
            useGradient = 0;        // use the state increase
        }
        if (mGradients.at(gDec).mDeltaCost < 0.0 and mGradients.at(gDec).mCostGradient != 0.0 and
            mGradients.at(gDec).mDeltaCost < mGradients.at(gInc).mDeltaCost) {
            useGradient = 1;        // use the state decrease
        }

        /// - Compute the delta-state from the chosen gradient.
        double deltaState = 0.0;
        if (0 == useGradient) {
            /// - The cost improves (decreases) with an increase in the state, but the gradient is
            ///   negative, and we want the state to increase, so flip the negative gradient sign to
            ///   positive.  Divide by zero prevented in logic above.
            deltaState = -1.0 * mState->mCost / mGradients.at(gInc).mCostGradient;
        } else if (1 == useGradient) {
            /// - The cost improves (decreases) with a decrease in the state, so the gradient is
            ///   positive, but we want the state to decrease so flip the positive gradient sign to
            ///   negative.  Divide by zero prevented in logic above.
            deltaState = -1.0 * mState->mCost / mGradients.at(gDec).mCostGradient;
        } // else nothing to do

        /// - Propagate the state by the delta-state.  We divide by the total number of state so
        ///   reduce overshoot caused by their effects compounding in the cost, and also apply a
        ///   user-provided gain to further reduce overshoot.
        mState->mState.at(i) += deltaState * mConfigData.mPropagationGain / mInStatesMaster->size();

        /// - Limit the new state to its min/max range.
        mState->mState.at(i) = MsMath::limitRange(mInStatesMaster->at(i).mMinimum,
                                                  mState->mState.at(i),
                                                  mInStatesMaster->at(i).mMaximum);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the states used to determine the gradients for each variable being optimized.
///           Starting with the state from the previous propagation run, this adds and subtracts the
///           deltas to each optimizing variable, and computes the corresponding delta-states.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::setGradientStates()
{
    for (unsigned int i=0; i<mInStatesMaster->size(); ++i) {

        /// - Compute indexes of the increase and decrease delta-state gradient objects for this
        ///   optimizer variable.
        const unsigned int gInc = i * 2;
        const unsigned int gDec = gInc + 1;

        /// - Copy the global state into each gradient, then the delta will be applied below.
        mGradients.at(gInc) = *mState;
        mGradients.at(gDec) = *mState;

        /// - The delta-state is currently hardcoded as 0.1% of the previous state magnitude,
        ///   unless the previous state magnitude is zero, in which case the delta-state is 0.1%
        ///   of the state range.
        const double stateRange = mInStatesMaster->at(i).mMaximum - mInStatesMaster->at(i).mMinimum;
        const double stateDeltaFactor = 0.001;

        /// - Increment and decrement the state by its delta-states, re-limit it to its min/max
        ///   range, and compute the delta-state from the difference between the new and old values.
        double previousState = mGradients.at(gInc).mState.at(i);
        if (0.0 == previousState) {
            mGradients.at(gInc).mState.at(i) += stateDeltaFactor * stateRange;
        } else {
            mGradients.at(gInc).mState.at(i) += stateDeltaFactor * fabs(previousState);
        }
        mGradients.at(gInc).mState.at(i) = MsMath::limitRange(mInStatesMaster->at(i).mMinimum,
                                                              mGradients.at(gInc).mState.at(i),
                                                              mInStatesMaster->at(i).mMaximum);
        mGradients.at(gInc).mDeltaState = mGradients.at(gInc).mState.at(i) - previousState;

        previousState = mGradients.at(gDec).mState.at(i);
        if (0.0 == previousState) {
            mGradients.at(gDec).mState.at(i) -= stateDeltaFactor * stateRange;
        } else {
            mGradients.at(gDec).mState.at(i) -= stateDeltaFactor * fabs(previousState);
        }
        mGradients.at(gDec).mState.at(i) = MsMath::limitRange(mInStatesMaster->at(i).mMinimum,
                                                              mGradients.at(gDec).mState.at(i),
                                                              mInStatesMaster->at(i).mMaximum);
        mGradients.at(gDec).mDeltaState = mGradients.at(gDec).mState.at(i) - previousState;

        /// - Append to the gradients history file
        {
            std::string pathFile = "grad_gradients_history.csv";
            std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::app));
            if (file.fail()) {
                throw std::runtime_error(mName + " error opening file: " + pathFile);
            } else {
                file << mEpoch << "," << i << ",INC," << mGradients.at(gInc).mState.at(i) << ","
                     << mGradients.at(gInc).mDeltaState << "," << mGradients.at(gInc).mCost << ","
                     << mGradients.at(gInc).mDeltaCost << "," << mGradients.at(gInc).mCostGradient
                     << std::endl;
                file << mEpoch << "," << i << ",DEC," << mGradients.at(gDec).mState.at(i) << ","
                     << mGradients.at(gDec).mDeltaState << "," << mGradients.at(gDec).mCost << ","
                     << mGradients.at(gDec).mDeltaCost << "," << mGradients.at(gDec).mCostGradient
                     << std::endl;
            }
            file.close();
        }
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
            return;
        }
    }
    if (runIdReturned == mState->mRunId) {
        mState->mCostGradient = 0.0; // unused
        mState->mCost         = cost;
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
/// @throws   std::runtime_error
///
/// @details  Writes the final state to the output file.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimGradientDescent::shutdown()
{
    printStates();

    /// - Write the global state to an output file.
    std::string pathFile = "grad_state.csv";
    std::ofstream file (pathFile.c_str(), (std::ofstream::out | std::ofstream::trunc));
    if (file.fail()) {
        throw std::runtime_error(mName + " error opening file: " + pathFile);
    } else {
        /// - Write the header row.
        file << "Global_cost";
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << ",state_" << j;
        }
        file << std::endl;

        /// - Write the data row as the global state.
        file << mState->mCost;
        for (unsigned int j=0; j<mInStatesMaster->size(); ++j) {
            file << "," << mState->mState.at(j);
        }
        file << std::endl;
    }
    file.close();
}
