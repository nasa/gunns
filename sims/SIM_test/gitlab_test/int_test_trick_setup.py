# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#---------------------------------------------
# Setup Trick for Eclss Integration Tests
#---------------------------------------------
trick.real_time_disable()
trick.sim_services.exec_set_terminate_time(360)
trick.exec_set_software_frame(0.0125)
trick.TMM_reduced_checkpoint(False)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(False)
trick.sim_control_panel_set_enabled(False)
trick.trick_view_set_enabled(False)
trick.remove_external_application(simControlPanel)
