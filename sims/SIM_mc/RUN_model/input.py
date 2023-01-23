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
trick.sim_services.exec_set_terminate_time(86400)
trick.exec_set_software_frame(0.0125)
trick.TMM_reduced_checkpoint(False)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)

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
