#ifndef GunnsOptimMonteCarlo_EXISTS
#define GunnsOptimMonteCarlo_EXISTS

/**
@file     GunnsOptimMonteCarlo.hh
@brief    GUNNS Optimization Monte Carlo Manager declarations

@defgroup  TSM_GUNNS_CORE_OPTIM_MC    GUNNS Optimization Monte Carlo Manager
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Refer to class details below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- ((Only variables of type double are supported as model inputs, outputs, and driver inputs.))

LIBRARY_DEPENDENCY:
- ((GunnsOptimMonteCarlo.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-05) (Initial))

@{
*/

#include "core/optimization/GunnsOptimMonteCarloTypes.hh"
#include "core/optimization/GunnsOptimFactory.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Optimization Monte Carlo Manager
///
/// @details  This implements Trick Monte Carlo (MC) jobs and manages interfaces between models in
///           the Trick sim and an optimization scheme, for the purpose of optimizing models in MC
///           runs.
///
///           Once this is added to the sim, the configuration of the optimizer and model inputs &
///           outputs is done entirely in the Trick input file, so as to minimize changes to the sim
///           code and rebuilds of the sim executable.
///
///           Although this is in the GUNNS repository, this manager and the various optimizers are
///           not actually GUNNS-specific, and could be used to optimize any models in a Trick sim.
///
///           The optimizers have a generic interface with the models and this manager, for easy
///           plug & play of different optimizers, configured by the input file.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsOptimMonteCarlo
{
    TS_MAKE_SIM_COMPATIBLE(GunnsOptimMonteCarlo);

    public:
        GunnsOptimBase* mOptimizer; /**< *o (1) trick_chkpnt_io(**) Pointer to the optimizer object. */
        double          mRunId;     /**< *o (1) trick_chkpnt_io(**) The slave run identifier. */
        /// @brief Constructs the GUNNS Monte Carlo Manager object.
        GunnsOptimMonteCarlo(const std::string& name = "");
        /// @brief Destructs the GUNNS Monte Carlo Manager object.
        virtual ~GunnsOptimMonteCarlo();
        /// @brief Creates an optimizer of the given type that this Monte Carlo Manager will use.
        void addOptimizer(const GunnsOptimFactory::OptimizerType type);
        /// @brief Adds a MC model input variable, a variable that is to be optimized.
        void addInput(const std::string& varName, double* address, const double min, const double max, GunnsOptimMonteCarloConstraint* constraint = 0);
        /// @brief Adds a model output variable that is compared against a target scalar value for the optimization cost function.
        void addOutput(const std::string& varName, double* address, const double targetValue = 0.0, const double costWeight = 1.0);
        /// @brief Adds the set of values of the model output variables for the next time step of the target trajectory.
        void addOutputDataRow(const std::string& values);
        /// @brief Adds a model driver input variable that will drive the model state over each Slave run.
        void addDriver(double* address);
        /// @brief Adds the set of values to drive the model driver input variables to for the next time step.
        void addDriverDataRow(const std::string& values);
        /// @brief Sets the amount of detail printed to the console.
        void setVerbosityLevel(const unsigned int verbosity);
        /// @brief Trick "monte_slave_init" job, initializes the Slave role.
        void initSlave();
        /// @brief Trick "monte_slave_pre" job, updates the Slave role prior to the Slave run.
        void updateSlavePre();
        /// @brief Trick "monte_slave_post" job, updates the Slave role after to the Slave run.
        void updateSlavePost();
        /// @brief Trick "monte_master_init" job, initializes the Master role.
        void initMaster();
        /// @brief Trick "monte_master_pre" job, updates the Master role before each Slave run.
        void updateMasterPre();
        /// @brief Trick "monte_master_post" job, updates the Master role after each Slave run.
        void updateMasterPost();
        /// @brief Trick "monte_master_shutdown" job, updates the Master role after all Slave runs.
        void updateMasterShutdown();
        /// @brief Trick "scheduled job", drives inputs to the model cyclically in each Slave run.
        void updateSlaveInputs();
        /// @brief Trick "scheduled job", drives outputs from the model cyclically in each Slave run.
        void updateSlaveOutputs();

    protected:
        std::string                             mName;             /**< *o (1) trick_chkpnt_io(**) Name of this instance for output messages. */
        bool                                    mIsMaster;         /**< *o (1) trick_chkpnt_io(**) This instance is in the Monte Carlo Master role. */
        bool                                    mIsSlave;          /**< *o (1) trick_chkpnt_io(**) This instance is in the Monte Carlo Slave role. */
        int                                     mSlaveId;          /**< *o (1) trick_chkpnt_io(**) The Slave role identifier of this instance. */
        double                                  mRunIdReturned;    /**< *o (1) trick_chkpnt_io(**) The returned run identifier from the Slave. */
        unsigned int                            mModelStepCount;   /**< *o (1) trick_chkpnt_io(**) Elapsed model step count for updating slave inputs. */
        unsigned int                            mVerbosityLevel;   /**< *o (1) trick_chkpnt_io(**) The amount of output to the console, higher values output more detail. */
        std::vector<GunnsOptimMonteCarloInput>  mInputs;           /**< ** (1) trick_chkpnt_io(**) Monte Carlo inputs, the model variables to be optimized. */
        std::vector<GunnsOptimMonteCarloDriver> mDrivers;          /**< ** (1) trick_chkpnt_io(**) Model driver variables and trajectory data. */
        std::vector<GunnsOptimMonteCarloOutput> mOutputs;          /**< ** (1) trick_chkpnt_io(**) Model output variables and target scalar or trajectory data. */
        GunnsOptimFactory                       mOptimizerFactory; /**< *o (1) trick_chkpnt_io(**) The optimizer factory. */
        /// @brief Computes and returns the cost function for a value compared to its target.
        virtual double computeCostFunction(const double value, const double target, const double weight) const;
        /// @brief Computes the cost function for the scalar targets.
        void computeScalarCosts();
        /// @brief Parses the given string of values into the given vector.
        void parseDataRow(std::vector<double>& data, const std::string& values) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsOptimMonteCarlo(const GunnsOptimMonteCarlo&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsOptimMonteCarlo& operator =(const GunnsOptimMonteCarlo&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] type (--) Enumeration of the desired optimizer type.
///
/// @details  Points mOptimizer to a newly created optimizer object of the given type.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsOptimMonteCarlo::addOptimizer(const GunnsOptimFactory::OptimizerType type)
{
    mOptimizer = mOptimizerFactory.createOptimizer(type);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] verbosity (--) The desired amount of console output: 0 is none and higher values add
///                           more detail.
///
/// @details  Sets mVerbosityLevel to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsOptimMonteCarlo::setVerbosityLevel(const unsigned int verbosity)
{
    mVerbosityLevel = verbosity;
}

#endif
