# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)

# Load the configuration for this epoch
f = "RUN_mc/epoch_configuration.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())

# List of tuples of [name, units, min range, max range]
input_vars = [
    ['mc.model.netConfig.conductor1.mMaxConductivity', '1', 0.0, 0.01],
    ['mc.model.netConfig.conductor2.mMaxConductivity', '1', 0.0, 0.02],
    ['mc.model.netConfig.valve1.mMaxConductivity',     '1', 0.0, 0.005],
    ['mc.model.netConfig.valve2.mMaxConductivity',     '1', 0.0, 0.0025],
    ]

# List of tuples of [name, target value, cost weight]
output_vars = [
    ['mc.model.netNodes[1].mContent.mPressure', 101.0447089840857,     1.0],
    ['mc.model.netNodes[2].mContent.mPressure',  90.37713522203975,    1.0],
    ['mc.model.conductor1.mFlowRate',             0.01824900229844743, 5.0],
    ['mc.model.conductor2.mFlowRate',             0.03649800459689486, 5.0],
    ]

# Run the sim for a single epoch, as the Python wrapper is handling the multiple
# epochs.
mc.monteCarlo.mOptimizer.mConfigData.mNumParticles     = 30
mc.monteCarlo.mOptimizer.mConfigData.mMaxEpoch         = 1
mc.monteCarlo.mOptimizer.mConfigData.mCognitiveCoeff   = 2.0
mc.monteCarlo.mOptimizer.mConfigData.mSocialCoeff      = 2.0
mc.monteCarlo.mOptimizer.mConfigData.mMaxVelocity      = 0.5

# Add the Slave input variables (currently only doubles are supported).
for var in input_vars:
    # Register MC variable with the Master/Optimizer
    mc.monteCarlo.addInDouble(trick.get_address(var[0]), var[2], var[3], var[0])
    # Create a calculated variable and add it to Monte Carlo.
    mcvar = trick.MonteVarCalculated(var[0], var[1])
    trick_mc.mc.add_variable(mcvar)

# TODO add the run ID to send from Master to Slave:
mcvar = trick.MonteVarCalculated('mc.monteCarlo.mRunId', '1')
trick_mc.mc.add_variable(mcvar)

# Add the Slave output variables (currently only doubles are supported).
for var in output_vars:
    mc.monteCarlo.addOutDouble(trick.get_address(var[0]), var[1], var[2])

# Enable Monte Carlo.
trick.mc_set_enabled(1)

# Add a Monte Carlo slave for each core, or the number of particles, whichever is less.
import multiprocessing
for i in range(min(multiprocessing.cpu_count(), mc.monteCarlo.mOptimizer.mConfigData.mNumParticles)):
    trick.mc_add_slave("localhost")
    
# Sets the total number of Slave runs to perform.
# Must call this after you've set the optimizer config data above!
trick.mc_set_num_runs(mc.monteCarlo.mOptimizer.getNumRuns())

# Stop each Slave run after elapsed simulation time.
trick.stop(10)
