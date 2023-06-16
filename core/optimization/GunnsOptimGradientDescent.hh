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
/// @details  This describes TODO
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
        enum StateDefinition {
            RANDOM     = 0, ///< Random within the state range
            FILE       = 1, ///< Defined in a file
            MIN_CORNER = 2, ///< At the minimum range corner of all variables
            MAX_CORNER = 3, ///< At the maximum range corner of all variables
            MIDRANGE   = 4, ///< At the midpoint of all variables
        };
        unsigned int    mNumVars;         /**< (1) Number of Monte Carlo variables. */
        unsigned int    mMaxEpoch;        /**< (1) Maximum number of epochs, or iterations, in the total run. */
        double          mPropagationGain; /**< (1) TODO */
        unsigned int    mRandomSeed;      /**< (1) Seed for the random number generator. */
        StateDefinition mInitState;       /**< (1) How the initial state is defined. */
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
///           and the GUNNS Monte Carlo Manager. TODO
///
/// TODO
/// Prototype the Gradient Descent thing!
/// - 2 main reasons:
///   1) see if the base class interface w/ MC manager can work with other algorithms than PSO
///   2) see if this works for finding the exact solution
/// - for each N MC variables, each iteration will have 2*N + 1 Slave runs:
///   - each N will have a run finding the cost gradient to increasing the variable, and a run
///     finding the cost gradient to decreasing the variable
///   - final run propagates the MC vars to the next state and evaluates it
///     - there will be a Gain factor on how much we propagate each MC var, like 0.5 the distance to
///       zero cost, etc.  This helps to avoid overshoot.
///     - Also we must account for there are multiple variables affecting the cost when we update the
///       state, so we must limit their combined deltas to avoid overshooting.
/// - The initial state must be given by the user.  We could use the last state from the PSO, or
///   all min/max, or midway between each range, etc.
///   - let user either specify the initial state or tell us how we want to init it
///
/// Prototype performance notes:
/// - RUN_mc/input_grad.py
/// - it's working for tuning just the 2 conductors in the test model, but doesn't work if trying
///   to tune all 4 (2 conductors + 2 valves).
/// - When trying to tune all 4, couple of problems (limitation?) emerge:
///   - we're seeing conductors tune the wrong way because as their conductance increases (wrong
///     direction) the pressure decreases more.  Adjusting the relative cost weights doesn't seem
///     to help.
///   - even if we start the 2 conductors at the truth tuning, because there is >0 cost due to the
///     error in the 2 valves, changing the conductors lowers the cost - the changed conductor is
///     compensating for the error caused by the valves - this sends the conductor tuning away from
///     it's truth value.
/// - lower propagation gain can get more accurate results, because less overshoot,
///    but takes more iterations to get there.  Still can't converge on exact solution because
///    gain eventually causes overshoot in the propagation.
///    - and you still have to start in the correct trough - just tuning the 2 conductors, but
///      starting them at 2x truth, they converge on the wrong values -- wrong trough
/// - So: this is working, but with limitations:
///   - you must start in the correct trough, which is usually very close to truth.  This is not
///     a good method to start with - start with a PSO or something else that is better at getting
///     close from a totally unknown starting position.
///   - Only seems to work for 1 or 2 variables

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
        virtual const std::vector<double>* getState() const;
        /// @brief Stores the given cost to the state object associated with the given Slave run.
        virtual void assignCost(const double cost, double runID, double runIdReturned);
        /// @brief Writes final outputs.
        virtual void shutdown() const;

    protected:
        /// @brief Validates the configuration and input data for this Gradient Descent.
        virtual void validate();
        /// @brief Initializes the particle states with the configured distribution setting.
        void initState();
//        /// @brief Initializes the stored best costs for the swarm global and each particle.
//        void initBestCosts();
        /// @brief Randomizes the state.
        void randomizeState();
//        /// @brief Randomizes the initial particle state velocities.
//        void randomizeSwarmVelocity();
        /// @brief Sets the state to the minimum of each MC variable range.
        void setMinCornerState();
        /// @brief Sets the state to the maximum of each MC variable range.
        void setMaxCornerState();
        /// @brief Sets the state to the median of each MC variable range.
        void setMedianState();
        /// @brief Reads the state from a file.
        void readFileState();
//        /// @brief Updates the global swarm best state and each particle's personal best state.
//        void updateBestStates();
        /// @brief TODO
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
inline const std::vector<double>* GunnsOptimGradientDescent::getState() const
{
    return &mActiveState->mState;
}

#endif
