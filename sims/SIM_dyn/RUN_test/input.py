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
trick.sim_services.exec_set_terminate_time(3600)
trick.exec_set_software_frame(0.001)
trick.TMM_reduced_checkpoint(False)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)

dyn.bodyEomInputData.mPosition[0]      =  0.0
dyn.bodyEomInputData.mPosition[1]      =  0.0
dyn.bodyEomInputData.mPosition[2]      =  0.0
dyn.bodyEomInputData.mVelocity[0]      =  0.0
dyn.bodyEomInputData.mVelocity[1]      =  0.0
dyn.bodyEomInputData.mVelocity[2]      =  0.0
#dyn.bodyEomInputData.mEulerSequence    = trick.GunnsDynEulerFactory.XYZ
dyn.bodyEomInputData.mEulerSequence    = 123
dyn.bodyEomInputData.mEulerAngles[0]   =  0.0
dyn.bodyEomInputData.mEulerAngles[1]   =  0.0
dyn.bodyEomInputData.mEulerAngles[2]   =  0.0
dyn.bodyEomInputData.mAngVelocity[0]   =  0.0
dyn.bodyEomInputData.mAngVelocity[1]   =  0.0
dyn.bodyEomInputData.mAngVelocity[2]   =  0.0
dyn.bodyEomInputData.mMassProps.mMass  =  1.0e6
dyn.bodyEomInputData.mMassProps.mCM[0] =  0.0
dyn.bodyEomInputData.mMassProps.mCM[1] =  0.0
dyn.bodyEomInputData.mMassProps.mCM[2] =  0.0
dyn.bodyEomInputData.mMassProps.mIxx   =  1.0e7
dyn.bodyEomInputData.mMassProps.mIxy   = -2.0e4
dyn.bodyEomInputData.mMassProps.mIxz   = -3.0e4
dyn.bodyEomInputData.mMassProps.mIyy   =  4.0e7
dyn.bodyEomInputData.mMassProps.mIyz   = -5.0e4
dyn.bodyEomInputData.mMassProps.mIzz   =  6.0e7

dyn.bodyEom.mStateDeriv.addAppliedForce(dyn.bodyForce)
dyn.bodyEom.mStateDeriv.addAppliedTorque(dyn.bodyTorque)

dyn_integloop.getIntegrator(trick.Euler, 13)
