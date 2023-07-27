#ifndef GunnsOptimGradientDescent_EXISTS
#define GunnsOptimGradientDescent_EXISTS

/**
@file     GunnsOptimGradientDescent.hh
@brief    GUNNS Gradient Descent Optimization declarations

@defgroup  TSM_GUNNS_CORE_OPTIM_GRAD_DESCENT    GUNNS Gradient Descent Optimization
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
- ((GunnsOptimGradientDescent.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-06) (Initial))

@{
*/

#include "GunnsOptimBase.hh"
#include "GunnsOptimMonteCarloTypes.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gradient Descent Optimizer State
///
/// @details  This provides the data describing a state in the GUNNS Gradient Descent Optimizer.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsOptimGradientDescentState
{
    public:
        std::vector<double> mState;        /**< (1) The state. */
        double              mDeltaState;   /**< (1) Delta-state of the state parameter. */
        double              mCost;         /**< (1) Cost function result. */
        double              mDeltaCost;    /**< (1) Delta-cost result from the delta-state. */
        double              mCostGradient; /**< (1) The cost gradient result, delta-cost over delta-state. */
        double              mRunId;        /**< (1) MC Slave run ID. */
        /// @brief Default constructor for this Gradient Descent Optimizer state.
        GunnsOptimGradientDescentState(const unsigned int size = 0);
        /// @brief Custom assignment operator for this Gradient Descent Optimizer state.
        GunnsOptimGradientDescentState& operator =(const GunnsOptimGradientDescentState& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gradient Descent Optimizer Configuration Data
///
/// @details  This provides the configuration data for the GUNNS Gradient Descent Optimizer.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimGradientDescentConfigData : public GunnsOptimBaseConfigData
{
    public:
        unsigned int    mNumVars;         /**< (1) Number of Monte Carlo variables. */
        unsigned int    mMaxEpoch;        /**< (1) Maximum number of epochs, or iterations, in the total run. */
        double          mPropagationGain; /**< (1) Scales the amount of propagation of states along their gradient. */
        double*         mInitialState;    /**< (1) Starting values for the state to begin optimizing from. */
        /// @brief Default constructor.
        GunnsOptimGradientDescentConfigData();
        /// @brief Default destructor.
        virtual ~GunnsOptimGradientDescentConfigData();
        /// @brief Assignment operator.
        GunnsOptimGradientDescentConfigData& operator =(const GunnsOptimGradientDescentConfigData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimGradientDescentConfigData(const GunnsOptimGradientDescentConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Gradient Descent Optimizer
///
/// @details  This implements a Gradient Descent Optimization scheme for use with Trick Monte Carlo
///           and the GUNNS Monte Carlo Manager.  From the given initial state, this will approach
///           the minimum of the local trough in the state space.  For each MC input state variable,
///           this determines its gradients of delta-cost over delta-state for a small increase and
///           decrease of the state value.  The delta-state that improves the cost the most is
///           propagated along its gradient to a new state value used for the next iteration, and
///           the process repeats.
///
///           For each iteration, this performs 2*N + 1 MC slave runs: two for each N MC input state
///           variables being optimized to determine their cost gradients, and one to propagate
///           along the chosen gradients to the next state.  The 2*N cost gradient runs can be
///           parallelized, but their set must be serial with the one propagation run.
///
///           This seems to work well enough for optimizing one or two variables, but with some
///           limitations:
///           - This fails when attempting to optimize 3 or more variables, at least on the one
///             model that we've tested with so far.  For multi-variate optimization in general,
///             another optimizer like Particle Swarm might be better.
///           - This only finds the minimum of the local trough containing the given initial state,
///             and is not guaranteed to find the global minimum.  We recommend this be used in
///             concert with another optimizer that is better at getting close to the global minimum
///             this using this Gradient Descent to approach closer to the minimum.
///           - Because this method is ill-suited for finding the global minimum, this requires the
///             user to explicitly define the starting state, and this provides no other options for
///             guessing at a starting state on its own.
///           - Using a lower propagation gain (see the configuration data) can obtain a more
///             accurate result, because it overshoots the local minimum less, but takes more
///             iterations.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimGradientDescent : public GunnsOptimBase
{
    public:
        GunnsOptimGradientDescentConfigData         mConfigData;  /**< *o (1) trick_chkpnt_io(**) The configuration data. */
        std::vector<GunnsOptimGradientDescentState> mGradients;   /**< ** (1) trick_chkpnt_io(**) States for the MC variable gradients. */
        GunnsOptimGradientDescentState*             mActiveState; /**< *o (1) trick_chkpnt_io(**) The state currently being run. */
        GunnsOptimGradientDescentState*             mState;       /**< *o (1) trick_chkpnt_io(**) The optimized state. */
        /// @brief Constructs the GUNNS Monte Carlo Gradient Descent Optimization object.
        GunnsOptimGradientDescent();
        /// @brief Destructs the GUNNS Monte Carlo Gradient Descent Optimization object.
        virtual ~GunnsOptimGradientDescent();
        /// @brief Copies the given Gradient Descent config data into internal storage.
        virtual void setConfigData(const GunnsOptimBaseConfigData* configData);
        /// @brief Initializes this Gradient Descent with a pointer to the Monte Carlo input variables.
        virtual void initialize(const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster);
        /// @brief Main update of this Gradient Descent; propagates the descent state.
        virtual void update();
        /// @brief Returns the number of individual MC Slave runs this Gradient Descent needs.
        virtual unsigned int getNumRuns() const;
        /// @brief Returns the MC input variable state of the active state object to the MC manager.
        virtual const std::vector<double>* getState();
        /// @brief Stores the given cost to the state object associated with the given Slave run.
        virtual void assignCost(const double cost, double runID, double runIdReturned);
        /// @brief Writes final outputs.
        virtual void shutdown();

    protected:
        /// @brief Validates the configuration and input data for this Gradient Descent.
        virtual void validate();
        /// @brief Initializes the particle states with the configured distribution setting.
        void initState();
        /// @brief Computes the delta-states for each gradient direction for each optimized variable.
        void setGradientStates();
        /// @brief Propagates the state to the next iteration.
        void propagateState(const double gain);
        /// @brief Outputs the states to the console.
        void printStates() const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimGradientDescent(const GunnsOptimGradientDescent&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsOptimGradientDescent& operator =(const GunnsOptimGradientDescent&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  unsigned int (--) The total number of Monte Carlo Slave runs in this Gradient Descent.
///
/// @details  Returns the number of Slave runs as the number of runs in an epoch (descent
///           iteration) times the configured number of epochs.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int GunnsOptimGradientDescent::getNumRuns() const
{
    return (mConfigData.mMaxEpoch * (2 * mConfigData.mNumVars + 1));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<double>* (--) Pointer to the vector of the active state.
///
/// @details  Returns a pointer to the active state object's state vector.  These values go into the
///           MC input variables for the next Slave run.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<double>* GunnsOptimGradientDescent::getState()
{
    return &mActiveState->mState;
}

#endif
