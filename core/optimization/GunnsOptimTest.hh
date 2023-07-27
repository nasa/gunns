#ifndef GunnsOptimTest_EXISTS
#define GunnsOptimTest_EXISTS

/**
@file     GunnsOptimTest.hh
@brief    GUNNS Optimization Test declarations

@defgroup  TSM_GUNNS_CORE_OPTIM_TEST    GUNNS Optimization Test
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
- ((GunnsOptimTest.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-07) (Initial))

@{
*/

#include "GunnsOptimBase.hh"
#include "GunnsOptimMonteCarloTypes.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Test State
///
/// @details  This provides the data describing a state in the GUNNS Optimization Test.
////////////////////////////////////////////////////////////////////////////////////////////////////
//struct GunnsOptimTestState
//{
//    public:
//        std::vector<double> mState;        /**< (1) The state. */
//        double              mDeltaState;   /**< (1) Delta-state of the state parameter. */
//        double              mCost;         /**< (1) Cost function result. */
//        double              mDeltaCost;    /**< (1) Delta-cost result from the delta-state. */
//        double              mCostGradient; /**< (1) The cost gradient result, delta-cost over delta-state. */
//        double              mRunId;        /**< (1) MC Slave run ID. */
//        /// @brief Default constructor for this Optimization Test state.
//        GunnsOptimTestState(const unsigned int size = 0);
//        /// @brief Custom assignment operator for this Optimization Test state.
//        GunnsOptimTestState& operator =(const GunnsOptimTestState& that);
//};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Test Configuration Data
///
/// @details  This provides the configuration data for the GUNNS Optimization Test.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimTestConfigData : public GunnsOptimBaseConfigData
{
    public:
//        unsigned int    mNumVars;         /**< (1) Number of Monte Carlo variables. */
//        unsigned int    mMaxEpoch;        /**< (1) Maximum number of epochs, or iterations, in the total run. */
//        double          mPropagationGain; /**< (1) Scales the amount of propagation of states along their gradient. */
//        double*         mInitialState;    /**< (1) Starting values for the state to begin optimizing from. */
        /// @brief Default constructor.
        GunnsOptimTestConfigData();
        /// @brief Default destructor.
        virtual ~GunnsOptimTestConfigData();
        /// @brief Assignment operator.
        GunnsOptimTestConfigData& operator =(const GunnsOptimTestConfigData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimTestConfigData(const GunnsOptimTestConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Test
///
/// @details  This implements a Optimization Test scheme for testing the GUNNS optimization code
///           and Monte Carlo Manager.
///TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimTest : public GunnsOptimBase
{
    public:
        GunnsOptimTestConfigData mConfigData;  /**< *o (1) trick_chkpnt_io(**) The configuration data. */
        unsigned int             mNumRuns; //TODO
        unsigned int             mInitCounter; //TODO
        unsigned int             mUpdateCounter; //TODO
        unsigned int             mShutdownCounter; //TODO
        double                   mCost; //TODO
        double                   mRunId; //TODO
        double                   mRunIdReturned; //TODO
        std::vector<double>*     mState;        /**< (1) Pointer to a test state. */
        /// @brief Constructs the GUNNS Monte Carlo Optimization Test object.
        GunnsOptimTest();
        /// @brief Destructs the GUNNS Monte Carlo Optimization Test object.
        virtual ~GunnsOptimTest();
        /// @brief Copies the given Test config data into internal storage.
        virtual void setConfigData(const GunnsOptimBaseConfigData* configData);
        /// @brief Initializes this Test with a pointer to the Monte Carlo input variables.
        virtual void initialize(const std::vector<GunnsOptimMonteCarloInput>* inStatesMaster);
        /// @brief Main update of this Test; propagates the descent state.
        virtual void update();
        /// @brief Returns the number of individual MC Slave runs this Test needs.
        virtual unsigned int getNumRuns() const;
        /// @brief Returns the MC input variable state of the active state object to the MC manager.
        virtual const std::vector<double>* getState();
        /// @brief Stores the given cost to the state object associated with the given Slave run.
        virtual void assignCost(const double cost, double runID, double runIdReturned);
        /// @brief Writes final outputs.
        virtual void shutdown();

//    protected:
//        /// @brief Validates the configuration and input data for this Test.
//        virtual void validate();
//        /// @brief Initializes the particle states with the configured distribution setting.
//        void initState();
//        /// @brief Computes the delta-states for each gradient direction for each optimized variable.
//        void setGradientStates();
//        /// @brief Propagates the state to the next iteration.
//        void propagateState(const double gain);
//        /// @brief Outputs the states to the console.
//        void printStates() const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimTest(const GunnsOptimTest&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsOptimTest& operator =(const GunnsOptimTest&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  unsigned int (--) The total number of Monte Carlo Slave runs in this Test.
///
/// @details  TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int GunnsOptimTest::getNumRuns() const
{
    return mNumRuns;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<double>* (--) Pointer to the vector of the test state.
///
/// @details  TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<double>* GunnsOptimTest::getState()
{
    return mState;
}

#endif
