#trick setup
simControlPanel = trick.SimControlPanel()
trick.add_external_application(simControlPanel)
trick.real_time_enable()
trick.sim_services.exec_set_terminate_time(86400)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)
