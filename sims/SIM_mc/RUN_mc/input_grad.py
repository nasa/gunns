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
    ['mc.model.conductor1.mFlowRate',             0.01824900229844743, 1.0],
    ['mc.model.conductor2.mFlowRate',             0.03649800459689486, 1.0],
    ]

# Set the initial state.
initialState = trick.alloc_type(len(input_vars), "double")
# In this example, we're starting 2 of the parameters at +10% of truth, and the
# others exactly at truth:
initialState = [0.0011, 0.0022, 0.0005, 0.00025]

# Add a Gradient Descent optimizer to the MC manager.
mc.monteCarlo.addOptimizer(trick.GunnsOptimFactory.GRADIENT_DESCENT)

# Create a Gradient Descent configuration data object and set the values.
theGradConfig = trick.GunnsOptimGradientDescentConfigData()

theGradConfig.mNumVars         = len(input_vars)
theGradConfig.mMaxEpoch        = 50
theGradConfig.mPropagationGain = 0.1
theGradConfig.mInitialState    = initialState

# Give the configuration to the optimizer.
mc.monteCarlo.mOptimizer.setConfigData(theGradConfig)

mc.monteCarlo.setVerbosityLevel(1)

# Add the Slave input variables (currently only doubles are supported).
for var in input_vars:
    # Register MC variable with the Master/Optimizer
    mc.monteCarlo.addInput(var[0], trick.get_address(var[0]), var[2], var[3])
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
