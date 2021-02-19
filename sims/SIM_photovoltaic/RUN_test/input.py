# @copyright Copyright 2019 United States Government as represented by the Administrator of the
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

# This demonstrates how to customize the # of strings in sections, and a custom load order for the
# shunting regulator:
#PvArraySectionNumStrings = trick.alloc_type(3, "unsigned int")
#PvArraySectionNumStrings = [3, 4, 3]
#pvSimObject.pvNetwork.netConfig.PvArray.mNumStringsBySection = PvArraySectionNumStrings
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(2, 2)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(2, 1)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(2, 0)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(1, 3)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(1, 2)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(1, 1)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(1, 0)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(0, 2)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(0, 1)
#pvSimObject.pvNetwork.netConfig.SAR.addStringLoadOrder(0, 0)
