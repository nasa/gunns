# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)

# Apply constraints between input variables.  In this case we somehow know that
# conductor2's conductivity is exactly 2 times conductor1, so we implement that
# constraint using the LinearFit TsApproximation.
constraint = trick.GunnsOptimMonteCarloConstraint(trick.LinearFit(0.0, 2.0, 0.0, 0.01),
                                                  trick.get_address('mc.model.netConfig.conductor1.mMaxConductivity'))

# List of tuples of [name, units, min range, max range, constraint]
input_vars = [
    ['mc.model.netConfig.conductor1.mMaxConductivity', '1', 0.0, 0.01,   0],
    ['mc.model.netConfig.conductor2.mMaxConductivity', '1', 0.0, 0.02,   0],
    #['mc.model.netConfig.conductor2.mMaxConductivity', '1', 0.0, 0.02,   constraint],
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

# Create a PSO configuration data object and set the values.
thePsoConfig = trick.GunnsOptimParticleSwarmConfigData()

# Chip's recommendation:
thePsoConfig.mNumParticles     = 30
thePsoConfig.mMaxEpoch         = 100
# These seem to work best:
thePsoConfig.mInertiaWeight    = 0.5
thePsoConfig.mInertiaWeightEnd = 0.5
thePsoConfig.mCognitiveCoeff   = 2.0
thePsoConfig.mSocialCoeff      = 2.0
thePsoConfig.mMaxVelocity      = 0.5
thePsoConfig.mRandomSeed       = 42
thePsoConfig.mInitDistribution = trick.GunnsOptimParticleSwarmConfigData.MIN_MAX_CORNERS
#thePsoConfig.mInitDistribution = trick.GunnsOptimParticleSwarmConfigData.FILE
# After the above default run from MIN_MAX_CORNERS, max vel 0.5, 30x100 run,
# do another run with max vel = 0.1 using FILE_CONTINUOUS (picking up from the
# end of the previous run), this will converge to almost exact truth tuning:
#thePsoConfig.mMaxVelocity      = 0.1
#thePsoConfig.mInitDistribution = trick.GunnsOptimParticleSwarmConfigData.FILE_CONTINUOUS

# Give the configuration to the optimizer.
mc.monteCarlo.mOptimizer.setConfigData(thePsoConfig)

mc.monteCarlo.setVerbosityLevel(1)

# Add the Slave input variables (currently only doubles are supported).
for var in input_vars:
    # Register MC variable with the Master/Optimizer
    mc.monteCarlo.addInput(var[0], trick.get_address(var[0]), var[2], var[3], var[4])
    # Create a calculated variable and add it to Monte Carlo.
    mcvar = trick.MonteVarCalculated(var[0], var[1])
    trick_mc.mc.add_variable(mcvar)

# TODO add the run ID to send from Master to Slave:
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
#        for var in var_list[1:]:
#            mc.monteCarlo.addOutTargetDouble(var, trick.get_address(var))
        for line in lines[1:]:
            mc.monteCarlo.addOutputDataRow(line.strip())
        f.close()
except:
    pass

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
trick.stop(10.1)
