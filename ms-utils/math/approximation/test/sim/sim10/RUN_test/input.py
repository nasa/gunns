import trick
from trick.sim_objects import *

trick.frame_log_on()
trick.real_time_enable()
trick.exec_set_software_frame(0.5)
trick.itimer_enable()
trick.TMM_reduced_checkpoint(False)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)
trick_vs.sim_control_panel.set_enabled(True)
trick.trick_view_set_enabled(1)
