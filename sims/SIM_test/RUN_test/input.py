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

testSimObject.superFluid.addSubNetwork(testSimObject.subFluid1)
testSimObject.superFluid.addSubNetwork(testSimObject.subFluid2)
testSimObject.superFluid.registerSuperNodes()

testSimObject.superElect.addSubNetwork(testSimObject.subElect1)
testSimObject.superElect.addSubNetwork(testSimObject.subElect2)
testSimObject.superElect.registerSuperNodes()

testSimObject.superThermal.addSubNetwork(testSimObject.subThermal1)
testSimObject.superThermal.addSubNetwork(testSimObject.subThermal2)
testSimObject.superThermal.registerSuperNodes()

#testSimObject.turbine.DriveShaft.addReference(testSimObject.turbine.Fan1);
#testSimObject.turbine.DriveShaft.addReference(testSimObject.turbine.Turbine1);
#testSimObject.turbine.DriveShaft.addReference(testSimObject.turbine.LoadFan);

#testSimObject.turbine.netConfig.Fan1.mDriveRatio = 5;
#testSimObject.turbine.netConfig.LoadFan.mDriveRatio = 50;
#testSimObject.turbine.netConfig.DriveShaft.mInertia = 0.000005;
#testSimObject.turbine.netInput.Turbine1.mMotorSpeed = 563687;
#testSimObject.turbine.netConfig.DriveShaft.mInertia = 5;
#testSimObject.turbine.netInput.Fan1.mMotorSpeed = 563687;
#testSimObject.turbine.netInput.LoadFan.mMotorSpeed = 563687;

#trick.stop(0.444444444444444444444444444444444444444444445)
