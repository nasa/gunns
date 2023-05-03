# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)
simControlPanel = trick.SimControlPanel()
trick.add_external_application(simControlPanel)
trickView = trick.TrickView()
trick.add_external_application(trickView)
trickView.set_auto_open_file('TV_systems.tv')
trick.real_time_enable()
trick.sim_services.exec_set_terminate_time(86400)
trick.exec_set_software_frame(0.0125)
trick.TMM_reduced_checkpoint(False)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)

####################
# HX Thermal Setup:
####################
# Put this network's solver in dummy mode to save CPU.
vehicle.hxThermal.netSolver.setDummyMode()

#####################
# Solar Array Thermal
#####################
# Pending the eventual radiation model providing heat flux with the environment,
# we need to add some absorbed heat from sunlight to the solar array to balance out
# the power that is converted electrical output, to keep the thermal aspect from
# getting too cold.  At this point it is an incomplete model, and the temperature
# will still drift to unrealistic values.
trick.add_read(1.0, """vehicle.passiveThermal.solarArray.setExternalHeatFlux(1, 1825.0)""" )

####################
# Cabin Fluid Setup:
####################
# Configure Multi-Adsorber scrubbers link compounds.  These will maintain roughtly the
# starting ppCO2 and RH with 2 crew in normal state, until they run out of capacity.
# Assume this is the LiOH reaction for absorbing CO2.
vehicle.cabinFluid.netConfig.scrubbers.addCompound(trick.ChemicalCompound.CO2, 10.0, 0.095, 0.0, 0.0, 0.0, -189.9)
# For H2O, I'm just making up the heat of reaction.
vehicle.cabinFluid.netConfig.scrubbers.addCompound(trick.ChemicalCompound.H2O, 10.0, 0.065, 0.0, 0.0, 0.0, -200.0)

# Initial tcv position to maintain roughtly 70 F with 2 crew in normal state.
vehicle.cabinFluid.netInput.tcv.mPosition = 0.15

# Initial gasSelectValve to N2.  Cabin PPO2 will drop due to crew consumption until the valve
# is switched to O2.
vehicle.cabinFluid.netInput.gasSelectValve.mPosition = 1.0

# Initial cabin leak to create typical spacecraft nominal leak rate of 0.05 %/day.
vehicle.cabinFluid.netInput.cabinLeak.mMalfLeakHoleFlag  = True
vehicle.cabinFluid.netInput.cabinLeak.mMalfLeakHoleValue = 2.8e-10

#####################
# Coolant Loop Setup:
#####################
# Configure Ammonia Boiler to start & stop flow at certain times.
vehicle.coolantLoopFluid.netInput.nh3ControlVlv.mPosition = 0.0
trick.add_read(   1.0, """vehicle.coolantLoopFluid.nh3ControlVlv.setPosition(1.0)""" )
trick.add_read(2400.0, """vehicle.coolantLoopFluid.nh3ControlVlv.setPosition(0.0)""" )

##############################
# Electrical User Loads Setup:
##############################
vehicle.powerElect.loadSwitch3.addUserLoad(vehicle.powerElect.fwdShellHeater)
vehicle.powerElect.loadSwitch4.addUserLoad(vehicle.powerElect.aftShellHeater)
vehicle.powerElect.loadSwitch5.addUserLoad(vehicle.powerElect.cabinFan)
vehicle.powerElect.loadSwitch6.addUserLoad(vehicle.powerElect.pump)
vehicle.powerElect.loadSwitch7.addUserLoad(vehicle.powerElect.tcv)
vehicle.powerElect.loadSwitch8.addUserLoad(vehicle.powerElect.radBypassVlv)
vehicle.powerElect.loadSwitch9.addUserLoad(vehicle.powerElect.gasSelectValve)

#####################
# Setup Data Logging:
#####################
# This causes the sim to record data into a .csv file in this RUN_ folder.
f = "data_log_setup/log_setup.py"
exec(compile(open(f, 'rb').read(), f, 'exec'), globals(), locals())
log_setup(0.1)
