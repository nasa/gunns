# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)
simControlPanel = trick.SimControlPanel()
trick.add_external_application(simControlPanel)
# trickView = trick.TrickView()
# trick.add_external_application(trickView)
# trickView.set_auto_open_file('TV_demo_standalone.tv')
# trick.real_time_enable()
trick.sim_services.exec_set_terminate_time(86400)
trick.exec_set_software_frame(0.0125)
trick.TMM_reduced_checkpoint(False)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)

################################################################################
# GUNNS config and input settings
################################################################################
brayton.fluid.driveShaft.addImpeller(brayton.fluid.compressor)
brayton.fluid.driveShaft.addImpeller(brayton.fluid.turbine)

# TODO: delete
PIPE_CONDUCTIVITY = 0.0125 #0.0038125 #m2
brayton.fluid.netInput.driveShaft.mMotorSpeed = 42000
brayton.fluid.netInput.driveShaft.mMalfSpeedOverrideValue = 42000 #rpm
brayton.fluid.netInput.driveShaft.mMalfSpeedOverrideFlag = True
brayton.fluid.netConfig.hxRecupHighT.mMaxConductivity = PIPE_CONDUCTIVITY
brayton.fluid.netConfig.hxRecupLowT.mMaxConductivity = PIPE_CONDUCTIVITY
brayton.fluid.netConfig.hxWithRadiator.mMaxConductivity = PIPE_CONDUCTIVITY
brayton.fluid.netConfig.hxWithReactor.mMaxConductivity = PIPE_CONDUCTIVITY
brayton.fluid.netConfig.turbine.mCoeffLowSpeed1 = 1.05 #2.3
brayton.fluid.netConfig.turbine.mCoeffHighSpeed1 = 0.85 #2.2
brayton.fluid.netConfig.turbine.mMaxFlowLowSpeed = 2.5	#1.5
brayton.fluid.netConfig.turbine.mMaxFlowHighSpeed = 1.8	#1.2
brayton.fluid.netConfig.driveShaft.mFrictionConstant = 7.37E-7