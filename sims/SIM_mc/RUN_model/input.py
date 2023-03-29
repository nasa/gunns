# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)
simControlPanel = trick.SimControlPanel()
trick.add_external_application(simControlPanel)
trickView = trick.TrickView()
trick.add_external_application(trickView)
trick.real_time_enable()
trick.sim_services.exec_set_terminate_time(10.1)
trick.exec_set_software_frame(0.0125)
trick.TMM_reduced_checkpoint(False)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)

# Schedule driver inputs:
trick.add_read(0.0, "mc.model.valve1.setPosition(1.0)")
trick.add_read(0.0, "mc.model.valve2.setPosition(1.0)")
trick.add_read(1.0, "mc.model.valve2.setPosition(0.5)")
trick.add_read(2.0, "mc.model.valve2.setPosition(0.0)")
trick.add_read(2.0, "mc.model.valve1.setPosition(0.5)")
trick.add_read(3.0, "mc.model.valve1.setPosition(0.0)")
trick.add_read(4.0, "mc.model.valve1.setPosition(0.5)")
trick.add_read(5.0, "mc.model.valve1.setPosition(1.0)")
trick.add_read(6.0, "mc.model.valve2.setPosition(0.5)")
trick.add_read(7.0, "mc.model.valve2.setPosition(1.0)")
trick.add_read(8.0, "mc.model.valve1.setPosition(0.5)")

# Set up data log:
log_variables = [
    "mc.model.valve1.mPosition",
    "mc.model.valve2.mPosition",
    "mc.model.netNodes[1].mContent.mPressure",
    "mc.model.netNodes[2].mContent.mPressure",
    "mc.model.conductor1.mFlowRate",
    "mc.model.conductor2.mFlowRate",
    ]
recording_group_name = "Log_Data"
dr_group = trick.DRAscii( recording_group_name )
dr_group.thisown = 0
dr_group.set_cycle(0.1)
dr_group.set_freq( trick.DR_Always )
dr_group.enable()
for var in log_variables:
    dr_group.add_variable(var)
trick.add_data_record_group(dr_group, trick.DR_Buffer)

# Note: default tuning of model gives steady-state flows & pressures @ time 10 sec:
# mc.model.conductor1.mFlowRate = 0.01824900229844743
# mc.model.conductor2.mFlowRate = 0.03649800459689486
# mc.model.valve1.mFlowRate     = 0.05474700689532216
# mc.model.valve2.mFlowRate     = 0.05474700689532223
# mc.model.netNodes[0].mContent.mPressure = 101.3249999774057
# mc.model.netNodes[1].mContent.mPressure = 101.0447089840857
# mc.model.netNodes[2].mContent.mPressure =  90.37713522203975
#
# The tuning that gives the above performance is:
# mc.model.netConfig.conductor1.mMaxConductivity = 0.001
# mc.model.netConfig.conductor2.mMaxConductivity = 0.002
# mc.model.netConfig.valve1.mMaxConductivity     = 0.0005
# mc.model.netConfig.valve2.mMaxConductivity     = 0.00025
