/**
@file     GunnsOptimMonteCarlo.cpp
@brief    GUNNS Optimization Monte Carlo Manager implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((GunnsOptimBase.o)
   (core/GunnsInfraFunctions.o))
*/

/// - GUNNS includes:
#include "GunnsOptimMonteCarlo.hh"
#include "core/GunnsInfraFunctions.hh"
#include "core/GunnsMacros.hh"
#include "math/MsMath.hh"
#include "strings/Strings.hh"

/// - System includes:
#include <cfloat>
#include <iostream>
#include <sstream>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) Instance name for console outputs.
///
/// @details  Constructs the Monte Carlo Manager Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimMonteCarlo::GunnsOptimMonteCarlo(const std::string& name)
    :
    mOptimizer(0),
    mRunId(0),
    mName(name),
    mIsMaster(false),
    mIsSlave(false),
    mSlaveId(0),
    mRunIdReturned(0),
    mModelStepCount(0),
    mVerbosityLevel(0),
    mInputs(),
    mDrivers(),
    mOutputs(),
    mOptimizerFactory()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Monte Carlo Manager Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsOptimMonteCarlo::~GunnsOptimMonteCarlo()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Initializes the Trick MC Master role.  Should be called by a "monte_master_init" Trick
///           job.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::initMaster()
{
    mIsSlave  = GunnsInfraFunctions::mcIsSlave();
    mIsMaster = not mIsSlave;
    if (mIsMaster) {
        mSlaveId = -1; // indicates this is not a slave
    } else {
        /// - Unlike the rest of GUNNS, here we don't use the H&S system or TsException types and
        ///   opt to just throw standard exceptions.  Because this MC stuff could be used to
        ///   optimize non-GUNNS models, the user might not want to bother setting up the H&S.
        throw std::runtime_error(mName + " initMaster called from non-Master role.");
    }

    mRunId = -1; // so actual run id's start counting from zero in updateMasterPre()

    /// - Initialize the optimizer and sync its output verbosity setting with this manager.
    if (mOptimizer) {
        mOptimizer->setVerbosityLevel(mVerbosityLevel);
        mOptimizer->initialize(&mInputs);
    }

    if (mVerbosityLevel > 0) {
        std::cout << mName << " Output targets:" << std::endl;
        for (unsigned int i=0; i<mOutputs.size(); ++i) {
            std::cout << " " << mOutputs.at(i).mName;
            if (mOutputs.at(i).mIsScalarTarget) {
                std::cout << " " << "scalar: " << mOutputs.at(i).mTargetScalar << std::endl;
            } else {
                std::cout << " " << "trajectory:" << std::endl;
                for (unsigned int j=0; j<mOutputs.at(i).mTargetTraj.size(); ++j) {
                    std::cout << "   " << mOutputs.at(i).mTargetTraj.at(j) << std::endl;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   std::runtime_error
///
/// @details  Initializes the Trick MC Slave role.  Should be called by a "monte_slave_init" Trick
///           job.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::initSlave()
{
    mIsSlave  = GunnsInfraFunctions::mcIsSlave();
    mIsMaster = not mIsSlave;
    if (mIsSlave) {
        mSlaveId = GunnsInfraFunctions::mcSlaveId();
    } else {
        throw std::runtime_error(mName + " initSlave called from non-Slave role.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the Trick MC Master role prior to starting each Slave run.  Should be called
///           by a "monte_master_pre" Trick job.  This updates the optimizer and Slave run ID.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::updateMasterPre()
{
    if (mVerbosityLevel > 0) {
        std::cout << mName << " updateMasterPre" << std::endl;
    }

    mRunId += 1.0;

    if (mOptimizer) {
        mOptimizer->update();

        /// - Get the optimizer's input state for the next Slave run, and load it into the Slave
        ///   model.
        const std::vector<double>* state = mOptimizer->getState();

        for (unsigned int i=0; i<mInputs.size(); ++i) {
            if (state) {
                const double value = state->at(i);
                *mInputs.at(i).mAddress = value;
            }
            if (mVerbosityLevel > 0) {
                std::cout << " " << mInputs.at(i).mName
                          << " " << *mInputs.at(i).mAddress << std::endl;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the Trick MC Master role when each Slave run is finished.  Should be called
///           by a "monte_master_post" Trick job.  This retrieves the Slave run ID and Slave cost
///           value result from the MC Master/Slave buffer and passes them to the optimizer.  The
///           Slave runs finish in a random order from when they are launched, so we must use their
///           returned run ID to correlate their results to the optimizer state that they ran.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::updateMasterPost()
{
    /// - Read the Slave cost result and run ID from the MC Master/Slave buffer.
    double cost = 0.0;
    GunnsInfraFunctions::mcRead(cost); // from GunnsInfraMacros
    GunnsInfraFunctions::mcRead(mRunIdReturned);

    if (mVerbosityLevel > 0) {
        std::cout << "updateMasterPost " << std::endl;
        std::cout << " cost: " << cost << " runId: " << mRunId << "/" << mRunIdReturned << std::endl;
    }

    /// - Pass the cost result and run ID to the optimizer.
    if (mOptimizer) {
        mOptimizer->assignCost(cost, mRunId, mRunIdReturned);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] value  (--) The value for which to return the cost function.
/// @param[in] target (--) The target value to compare the given value against.
/// @param[in] weight (--) The weight scalar applied to the error for this cost.
///
/// @returns  double (--) The computed cost function result.
///
/// @details  Computes the cost as the square of weighted error,
///           where weighted error = weight * (value - target).
///           This virtual function can be overriden as needed to implement other functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsOptimMonteCarlo::computeCostFunction(const double value, const double target, const double weight) const
{
    const double weightedError = weight * (value - target);
    return (weightedError * weightedError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes and stores the cost function result for each scalar target.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::computeScalarCosts()
{
    for (unsigned int i=0; i<mOutputs.size(); ++i) {
        if (mOutputs.at(i).mIsScalarTarget) {
            mOutputs.at(i).mCost = computeCostFunction(*mOutputs.at(i).mAddress,
                                                        mOutputs.at(i).mTargetScalar,
                                                        mOutputs.at(i).mCostWeight);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Shuts down the Trick MC Master role after all Slave runs and their Master post jobs
///           have finished.  Should be called by a "monte_master_shutdown" Trick job.  This
///           updates the optimizer one last time, then calls its shutdown function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::updateMasterShutdown()
{
    if (mVerbosityLevel > 0) {
        std::cout << "updateMasterShutdown" << std::endl;
    }
    if (mOptimizer) {
        mOptimizer->update();
        mOptimizer->shutdown();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the Trick MC Slave role at the start of each Slave run.  Should be called by
///           a "monte_slave_pre" Trick job.  Currently this does nothing, but we leave this as a
///           placeholder in case it becomes important later.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::updateSlavePre()
{
    // nothing to do yet, placeholder for now
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the Trick MC Slave role at the end of each Slave run.  Should be called by a
///           "trick_slave_post" Trick job.  This computes the final cost function result for this
///           run, and send the result and the run ID back to the Master role through the Trick MC
///           Master/Slave buffer.
///
///           The Trick MC Master/Slave buffer is a TCP port, which has a limited buffer size
///           (8 KB, default), so it's not appropriate to use this for large amounts of data, like
///           the model output trajectories for the run.  Some MC applications save the model output
///           to Trick DP log files, but we skip all that by just computing the cost function in the
///           Slave, so we only have to send the final cost value back.  Some future users of this
///           may want to use log files, but that implementation is TBD.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::updateSlavePost()
{
    computeScalarCosts();
    double cost = 0.0;
    for (unsigned int i=0; i<mOutputs.size(); ++i) {
        cost += mOutputs.at(i).mCost;
    }

    /// - Write the total cost result for this run to the MC Master/Slave buffer.
    GunnsInfraFunctions::mcWrite(cost); // from GunnsInfraMacros

    /// - Write the run ID to the MC Master/Slave buffer.  We use a double for the run ID, because
    ///   when we tried to use int during development, the int values were getting garbled by the
    ///   time they made it back to the Master role.  If we could figure out why and fix that, then
    ///   it would be better to switch back to integers.
    mRunIdReturned = mRunId;
    GunnsInfraFunctions::mcWrite(mRunIdReturned);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Drives the inputs to the model, prior to each model step in the Slave run.  This
///           should be called by either a "scheduled" Trick job prior to the model step, or by the
///           model itself at the start of its step, before any of these inputs are used.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::updateSlaveInputs()
{
    for (unsigned int i=0; i<mDrivers.size(); ++i) {
        if (mModelStepCount < mDrivers.at(i).mTrajectory.size()) {
            *mDrivers.at(i).mAddress = mDrivers.at(i).mTrajectory.at(mModelStepCount);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Updates the cost function for the Slave run after each model step.  This should be
///           called by either a "scheduled" Trick job after to the model step, or by the model
///           itself at the end of its step.  This updates the cost for each target variable
///           separately.  They will be summed and weighted in updateMasterPost().  This cost value
///           grows with accumulated error after each model step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::updateSlaveOutputs()
{
    for (unsigned int i=0; i<mOutputs.size(); ++i) {
        if (mModelStepCount < mOutputs.at(i).mTargetTraj.size() and not mOutputs.at(i).mIsScalarTarget) {
            mOutputs.at(i).mCost += computeCostFunction(*mOutputs.at(i).mAddress,
                                                         mOutputs.at(i).mTargetTraj.at(mModelStepCount),
                                                         mOutputs.at(i).mCostWeight);
        }
    }

    /// - Update the model step count.  This is used to make sure we don't overstep past the end of
    ///   the input driver and target trajectory value data.
    mModelStepCount++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] varName    (--) The variable name, as a string, for reporting.
/// @param[in] address    (--) Memory address of the input variable.
/// @param[in] min        (--) Minimum value of the search range for this variable.
/// @param[in] max        (--) Maximum value of the search range for this variable.
/// @param[in] constraint (--) Optional constraint on this input.
///
/// @details  This adds a Monte Carlo input variable, one that we are trying to optimize.  These
///           define the state space of the optimization search.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::addInput(const std::string& varName, double* address, const double min,
                                    const double max, GunnsOptimMonteCarloConstraint* constraint)
{
    if (not GunnsInfraFunctions::mcIsSlave()) { // from GunnsInfraMacros
        GunnsOptimMonteCarloInput newInput;
        newInput.mName     = varName;
        newInput.mAddress  = address;
        newInput.mMinimum  = min;
        newInput.mMaximum  = max;
        if (constraint) {
            newInput.addNewConstraint(constraint);
        }
        mInputs.push_back(newInput);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] varName     (--) The variable name, as a string, for reporting.
/// @param[in] address     (--) Memory address of the output variable.
/// @param[in] targetValue (--) The optional scalar target value.
/// @param[in] costWeight  (--) The relative weight of this variable in the cost function.
///
/// @details  This adds a model output variable for which we are trying to achieve its target
///           values by optimizing the input variables.  This function adds an optional scalar
///           target value, for optimizing a steady-state model.  A target trajectory for a non-
///           steady-state model can be specified with teh addOutputDataRow function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::addOutput(const std::string& varName, double* address, const double targetValue, const double costWeight)
{
    GunnsOptimMonteCarloOutput newOutput;
    newOutput.mName       = varName;
    newOutput.mAddress    = address;
    newOutput.mCost       = 0.0;
    newOutput.mCostWeight = costWeight;
    newOutput.mIsScalarTarget = true; // assume scalar for now, addOutputDataRow will reset this
    newOutput.mTargetScalar = targetValue;
    newOutput.mTargetTraj.clear();
    mOutputs.push_back(newOutput);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] values (--) Comma-separated string of values of all output variables at the next time step.
///
/// @details  Adds the trajectory values for the output variables for the next time step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::addOutputDataRow(const std::string& values)
{
    std::vector<double> newDataRow;
    parseDataRow(newDataRow, values);
    for (unsigned int i=0; i<newDataRow.size(); ++i) {
        mOutputs.at(i).mTargetTraj.push_back(newDataRow.at(i));
        mOutputs.at(i).mIsScalarTarget = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] address     (--) Memory address of the driver variable.
///
/// @details  Adds a driver variable which is a model variable that is driven along a trajectory
///           as the run progresses.  Values for the trajectory are given in the addDriverDataRow()
///           function.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::addDriver(double* address)
{
    GunnsOptimMonteCarloDriver newDriver;
    newDriver.mAddress = address;
    newDriver.mTrajectory.clear();
    mDrivers.push_back(newDriver);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] values (--) Comma-separated string of values of all driver variables at the next time step.
///
/// @details  Adds the trajectory values for the driver variables for the next time step.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::addDriverDataRow(const std::string& values)
{
    std::vector<double> newDataRow;
    parseDataRow(newDataRow, values);
    for (unsigned int i=0; i<mDrivers.size(); ++i) {
        mDrivers.at(i).mTrajectory.push_back(newDataRow.at(i));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] data   (--) Reference to a vector of doubles to be added to.
/// @param[in]  values (--) Comma-separated string of values.
///
/// @details  Parses the given string of comma-separted floating point values, converts them to
///           double-precision floating point format, and adds them to the given vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsOptimMonteCarlo::parseDataRow(std::vector<double>& data, const std::string& values) const
{
    std::string s = values;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> tokens;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);
    for (unsigned int i=1; i<tokens.size(); ++i) {
        data.push_back(atof(tokens.at(i).c_str()));
    }
}
