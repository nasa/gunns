# @copyright Copyright 2019 United States Government as represented by the Administrator of the
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

   # Logging terms are listed in this file:
   execfile('Log_setup/Log_variables.py', globals())
   for var in log_variables:
       dr_group.add_variable(var)

   #########################################################
   # Add the data recording group to Trick's data recording.
   #########################################################
   trick.add_data_record_group( dr_group, trick.DR_Buffer )

   return
