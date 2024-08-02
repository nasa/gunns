# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
# See pso.py.  This file configures the Trick monte carlo instance for a single epoch
# within the batch of epochs managed by pso.py.  This file contains the common configuration
# used for all epochs, while file epoch_configuration.py contains deltas for the current
# epoch.

# Trick setup
trick.sim_services.exec_set_trap_sigfpe(1)

# Create a PSO configuration data object.
thePsoConfig = trick.GunnsOptimParticleSwarmConfigData()

# Load the configuration for this epoch
f = "RUN_mc/epoch_configuration.py"
exec(compile(open(f, "rb").read(), f, 'exec'), globals(), locals())

# List of tuples of [name, units, min range, max range]
input_vars = [
    ['mc.model.netConfig.conductor1.mMaxConductivity', '1', 0.0, 0.01,   0],
    ['mc.model.netConfig.conductor2.mMaxConductivity', '1', 0.0, 0.02,   0],
    ['mc.model.netConfig.valve1.mMaxConductivity',     '1', 0.0, 0.005,  0],
    ['mc.model.netConfig.valve2.mMaxConductivity',     '1', 0.0, 0.0025, 0],
    ]

# List of tuples of [name, target value, cost weight]
output_vars = [
    ['mc.model.netNodes[1].mContent.mPressure', 101.0447089840857,     1.0],
    ['mc.model.netNodes[2].mContent.mPressure',  90.37713522203975,    1.0],
    ['mc.model.conductor1.mFlowRate',             0.01824900229844743, 1.0],
    ['mc.model.conductor2.mFlowRate',             0.03649800459689486, 1.0],
    ]

# Add a PSO optimizer to the MC manager.
mc.monteCarlo.addOptimizer(trick.GunnsOptimFactory.PSO)

# Configure the sim for a single epoch, as the Python wrapper pso.py is handling the multiple epochs.
thePsoConfig.mNumParticles     = 30
thePsoConfig.mMaxEpoch         = 1
thePsoConfig.mCognitiveCoeff   = 2.0
thePsoConfig.mSocialCoeff      = 2.0
thePsoConfig.mMaxVelocity      = 0.5

# Give the configuration to the optimizer.
mc.monteCarlo.mOptimizer.setConfigData(thePsoConfig)

# Add the Slave input variables (currently only doubles are supported).
for var in input_vars:
    # Register MC variable with the Master/Optimizer
    mc.monteCarlo.addInput(var[0], trick.get_address(var[0]), var[2], var[3], var[4])
    # Create a calculated variable and add it to Monte Carlo.
    mcvar = trick.MonteVarCalculated(var[0], var[1])
    trick_mc.mc.add_variable(mcvar)

# Add the run ID to send from Master to Slave:
mcvar = trick.MonteVarCalculated('mc.monteCarlo.mRunId', '1')
trick_mc.mc.add_variable(mcvar)

# Add the Slave output variables (currently only doubles are supported).
for var in output_vars:
    mc.monteCarlo.addOutput(var[0], trick.get_address(var[0]), var[1], var[2])

# Add the scripted input driver data to the model
try:
    with open('input_driver_data.csv', 'r') as f:
        lines = f.readlines()
        var_list = lines[0].strip().split(',')
        for var in var_list[1:]:
            mc.monteCarlo.addDriver(trick.get_address(var))
        for line in lines[1:]:
            mc.monteCarlo.addDriverDataRow(line.strip())
        f.close()
except:
    pass

# Add the desired output target data to the model
try:
    with open('output_target_data.csv', 'r') as f:
        lines = f.readlines()
        var_list = lines[0].strip().split(',')
        for line in lines[1:]:
            mc.monteCarlo.addOutputDataRow(line.strip())
        f.close()
except:
    pass

# Enable Monte Carlo.
trick.mc_set_enabled(1)

# Add a Monte Carlo slave for each core, or the number of particles, whichever is less.
import multiprocessing
for i in range(min(multiprocessing.cpu_count(), thePsoConfig.mNumParticles)):
    trick.mc_add_slave("localhost")

# Sets the total number of Slave runs to perform.
# Must call this after you've set the optimizer config data above!
trick.mc_set_num_runs(mc.monteCarlo.mOptimizer.getNumRuns())

# Stop each Slave run after elapsed simulation time.
trick.stop(10.1)
