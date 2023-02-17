# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)

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

# Configure the optimizer.
mc.monteCarlo.mOptimizer.mConfigData.mNumParticles     = 20
mc.monteCarlo.mOptimizer.mConfigData.mMaxEpoch         = 200
mc.monteCarlo.mOptimizer.mConfigData.mInertiaWeight    = 0.5
mc.monteCarlo.mOptimizer.mConfigData.mInertiaWeightEnd = 0.5
mc.monteCarlo.mOptimizer.mConfigData.mCognitiveCoeff   = 2.0
mc.monteCarlo.mOptimizer.mConfigData.mSocialCoeff      = 2.0
mc.monteCarlo.mOptimizer.mConfigData.mMaxVelocity      = 1.0
mc.monteCarlo.mOptimizer.mConfigData.mRandomSeed       = 42

# Best results so far for 20x200 swarm: coeffs: 0.5/2/2, cost: 0.188
# inert weight 0.4: 0.377
# inert weight 0.6: 0.238
# coeffs 2.1: 0.250
# coeffs 1.9: 0.188
# coeffs 2.1/1.9: 0.378
# coeffs 1.9/2.1: 0.232
# So notihing seems better than 0.5/2/2 for standard PSO
# For 0.5/2/2, 4 00 epochs: 0.155
# For 0.5/2/2, 1000 epochs: 0.103 - pretty good answer, so is converging just very slowly
#     and once gets within a certain amount, has very slow approach

# Compare Trelea vs. Common:
# Common:   0.5/2/2 for 20x200 swarm: 0.188
# Trelea 1: 0.6/1.7/1.7 for 20x200 swarm: 0.360
# Trelea 2: 0.729/1.494/1.494 for 20x200 swarm: 0.197
# Trelea 2: 0.729/1.494/1.494 for 20x1000 swarm: 0.197  # got stuck convergd on local minum by 200th epoch
# So Trelea really not better than Common

# Commpare 10x2000 vs. 20x1000 swarm for Common 0.5/2/2:
# 20x1000: 0.103
# 10x2000: 0.203 (got stuck after ~1000 epochs and never improved, all particles converged on local minimum)

# Compare annealing:
# start/end 0.5/0.5, gains 2/2, 20x200 Common: 0.188
# start/end 0.9/0.1, gains 2/2, 20x200 Common: 0.366
# start/end 0.1/0.9, gains 2/2, 20x200 Common: 0.388
# start/end 0.7/0.3, gains 2/2, 20x200 Common: 0.340
# start/end 0.3/0.7, gains 2/2, 20x200 Common: 0.347
# start/end 0.6/0.4, gains 2/2, 20x200 Common: 0.300
# start/end 0.4/0.6, gains 2/2, 20x200 Common: 0.296


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

# Add a Monte Carlo slave for each core.
#import multiprocessing
#for i in range(multiprocessing.cpu_count()):
#    trick.mc_add_slave("localhost")
    
# Sets the total number of Slave runs to perform.
# Must call this after you've set the optimizer config data above!
trick.mc_set_num_runs(mc.monteCarlo.mOptimizer.getNumRuns())

# Stop each Slave run after elapsed simulation time.
trick.stop(10)
