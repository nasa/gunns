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

# Set up lag amount in the lag ring buffer
testDistributedIf.lagBuffer.mDelayFrames = 1
testDistributedIf.lagBufferLiquidReturn.mDelayFrames = 1
testDistributedIf.lagBufferLiquidSupply.mDelayFrames = 1
testDistributedIf.lagBufferLiquidAccum.mDelayFrames = 1

# Open vlv0 so that pair0 will take Supply mode.
#testDistributedIf.fluid.netInput.vlv0.mPosition  = 1.0
#testDistributedIf.fluid2.netInput.vlv0.mPosition = 1.0

# Open vlv1 so that pair1 will take Supply mode.
testDistributedIf.fluid.netInput.vlv1.mPosition  = 1.0
testDistributedIf.fluid2.netInput.vlv1.mPosition = 1.0

# Turn on Island mode for testDistrubedIf fluidLiquid network so
# fluid island analyzer can be used.
testDistributedIf.fluidLiquid.netSolver.setIslandMode(trick.Gunns.SOLVE)

# configure testDistrubedIf fluidLiquidAccum valves
testDistributedIf.fluidLiquidAccum.netInput.accumVlv1.mPosition  = 0.0
testDistributedIf.fluidLiquidAccum.netInput.accumVlv2.mPosition  = 0.0

testDistributedIf.fluidLiquidAccum.netInput.intfcVlv1.mPosition  = 0.0
testDistributedIf.fluidLiquidAccum.netInput.intfcVlv2.mPosition  = 0.0

testDistributedIf.fluidLiquidAccum.netInput.bypassIfVlv.mPosition  = 1.0

testDistributedIf.fluidLiquidAccum.netSolver.setIslandMode(trick.Gunns.SOLVE)

# Register DistributedIf links with each other to prevent capacitance looping
testDistributedIf.fluidLiquid.intfcSupplyOut.addOtherIf(testDistributedIf.fluidLiquid.intfcSupplyIn)
testDistributedIf.fluidLiquid.intfcSupplyOut.addOtherIf(testDistributedIf.fluidLiquid.intfcReturnOut)
testDistributedIf.fluidLiquid.intfcSupplyOut.addOtherIf(testDistributedIf.fluidLiquid.intfcReturnIn)
testDistributedIf.fluidLiquid.intfcSupplyIn.addOtherIf(testDistributedIf.fluidLiquid.intfcSupplyOut)
testDistributedIf.fluidLiquid.intfcSupplyIn.addOtherIf(testDistributedIf.fluidLiquid.intfcReturnOut)
testDistributedIf.fluidLiquid.intfcSupplyIn.addOtherIf(testDistributedIf.fluidLiquid.intfcReturnIn)
testDistributedIf.fluidLiquid.intfcReturnOut.addOtherIf(testDistributedIf.fluidLiquid.intfcSupplyIn)
testDistributedIf.fluidLiquid.intfcReturnOut.addOtherIf(testDistributedIf.fluidLiquid.intfcSupplyOut)
testDistributedIf.fluidLiquid.intfcReturnOut.addOtherIf(testDistributedIf.fluidLiquid.intfcReturnIn)
testDistributedIf.fluidLiquid.intfcReturnIn.addOtherIf(testDistributedIf.fluidLiquid.intfcSupplyIn)
testDistributedIf.fluidLiquid.intfcReturnIn.addOtherIf(testDistributedIf.fluidLiquid.intfcSupplyOut)
testDistributedIf.fluidLiquid.intfcReturnIn.addOtherIf(testDistributedIf.fluidLiquid.intfcReturnOut)

#---------------------------------------------
# Setup Data Logging
#---------------------------------------------
execfile("Log_setup/Log_setup.py")
log_setup(0.1)

