# @copyright Copyright 2023 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#
#####################################################
# Set up the data recording group control parameters.
#####################################################

def log_setup( log_cycle ) :

   # Build a data recording group name.
   recording_group_name = "Log_Data"

   # Create a recording group.
   #dr_group = trick.DRBinary( recording_group_name )
   dr_group = trick.DRAscii( recording_group_name )
   dr_group.thisown = 0

   # Set the logging cycle frequency.
   dr_group.set_cycle( log_cycle  )

   # Set up other logging parameters.
   dr_group.set_freq( trick.DR_Always )
   dr_group.enable()

   log_variables = [
                    'vehicle.coolantLoopFluid.radBypassVlv.mPosition',
                    'vehicle.cabinFluid.gasSelectValve.mPosition',
                    'vehicle.cabinFluid.tcv.mPosition',
                    'vehicle.signal.sensorAtcsPumpInT.mSensedOutput',
                    'vehicle.signal.sensorCabinT.mSensedOutput',
                    'vehicle.signal.sensorCabinRH.mSensedOutput',
                    'vehicle.signal.sensorCabinPPO2.mSensedOutput',
                    'vehicle.powerElect.battery.mCurrent',
                    'vehicle.powerElect.battery.mSoc',
                    'vehicle.powerElect.solarArrayRegulator.mFlux',
                    'vehicle.powerElect.solarSwitch.mSwitch.mSwitchCommandedClosed',
                    'vehicle.powerElect.netSolver.mLastMinorStep',
                    'vehicle.powerElect.solarArrayRegulator.mOutputPower',
                    'vehicle.powerElect.solarArrayRegulator.mPotentialVector[1]',
                    'vehicle.passiveThermal.fwdShield.mTemperature',
                    'vehicle.passiveThermal.fwdShell.mTemperature',
                    'vehicle.passiveThermal.aftShell.mTemperature',
                    'vehicle.passiveThermal.aftShield.mTemperature',
                    'vehicle.passiveThermal.radiator.mTemperature',
                    'vehicle.signal.sensorCabinP.mSensedOutput',
                    'vehicle.signal.sensorCabinPPCO2.mSensedOutput',
                    'vehicle.powerElect.batterySwitch.mSwitch.mSwitchCommandedClosed',
                    'vehicle.powerElect.netNodes[4].mPotential',
                   ]
   for var in log_variables:
       dr_group.add_variable(var)

   #########################################################
   # Add the data recording group to Trick's data recording.
   #########################################################
   trick.add_data_record_group( dr_group, trick.DR_Buffer )

   return
