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
/// @brief    GUNNS Optimization Test Configuration Data
///
/// @details  This provides the configuration data for the GUNNS Optimization Test.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimTestConfigData : public GunnsOptimBaseConfigData
{
    public:
        /// @brief Default constructor.
        GunnsOptimTestConfigData();
        /// @brief Default destructor.
        virtual ~GunnsOptimTestConfigData();

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsOptimTestConfigData& operator =(const GunnsOptimTestConfigData&);
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimTestConfigData(const GunnsOptimTestConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Test
///
/// @details  This implements a Optimization Test scheme for testing the GUNNS optimization code
///           and Monte Carlo Manager.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimTest : public GunnsOptimBase
{
    public:
        unsigned int         mNumRuns;         /**<    (1) trick_chkpnt_io(**) Number of returns value for test. */
        unsigned int         mConfigCounter;   /**< *o (1) trick_chkpnt_io(**) Counter of calls to the setConfigData method. */
        unsigned int         mInitCounter;     /**<    (1) trick_chkpnt_io(**) Counter of calls to the initialize method. */
        unsigned int         mUpdateCounter;   /**<    (1) trick_chkpnt_io(**) Counter of calls to the update method. */
        unsigned int         mShutdownCounter; /**<    (1) trick_chkpnt_io(**) Counter of calls to the shutdown method. */
        double               mCost;            /**<    (1) trick_chkpnt_io(**) Test cost value. */
        double               mRunId;           /**<    (1) trick_chkpnt_io(**) Test run ID value. */
        double               mRunIdReturned;   /**<    (1) trick_chkpnt_io(**) Test returned run ID value. */
        std::vector<double>* mState;           /**<    (1) trick_chkpnt_io(**) Pointer to a test state. */
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
        /// @brief Public function to call constraintInputs with the given argument for testing.
        void accessConstrainInputs(std::vector<double>& vec);

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
/// @details  Returns the value of mNumRuns.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int GunnsOptimTest::getNumRuns() const
{
    return mNumRuns;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<double>* (--) Pointer to the vector of the test state.
///
/// @details  Returns the value of the mState pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<double>* GunnsOptimTest::getState()
{
    return mState;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] vec (--) Vector of doubles to be constrained by the MC input constraints.
///
/// @throws   std::range_error
///
/// @details  Passes the given vector the the protected constrainInputs function.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsOptimTest::accessConstrainInputs(std::vector<double>& vec)
{
    constrainInputs(vec);
}

#endif
