# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)
simControlPanel = trick.SimControlPanel()
trick.add_external_application(simControlPanel)
trickView = trick.TrickView()
trick.add_external_application(trickView)
trick.real_time_enable()
trick.sim_services.exec_set_terminate_time(1000)
trick.exec_set_software_frame(1.0)
trick.TMM_reduced_checkpoint(False)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(False)
#trick_sys.sched.set_freeze_command(False)

# timing network setup
timingBasic.network.N = 4
