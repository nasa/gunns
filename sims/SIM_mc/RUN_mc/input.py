# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)

# Add the Slave output variables (currently only doubles are supported).
output_vars = [
    'mc.model.conductor1.mFlowRate',
    'mc.model.conductor2.mFlowRate',
    'mc.model.netNodes[1].mContent.mPressure',
    'mc.model.netNodes[2].mContent.mPressure',
    ]

for var in output_vars:
    mc.monteCarlo.addOutDouble(trick.get_address(var))
                               
# Enable Monte Carlo.
trick.mc_set_enabled(1)

# Add a Monte Carlo slave for each core.
#import multiprocessing
#for i in range(multiprocessing.cpu_count()):
#    trick.mc_add_slave("localhost")
    
# Sets the number of runs to perform to 20. Trick will not exceed the number of values in an input file.
trick.mc_set_num_runs(2)

# Stop the simulation run after elapsed simulation time.
trick.stop(10)
