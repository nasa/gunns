# @copyright Copyright 2024 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved. */
#

################################################################################
#
# Define the command line options to configure the simulation.
#
def print_usage_message( ):
   
   global crc_host
   global crc_port
   global local_settings

   print(' ')
   print('Simulation Command Line Configuration Options:')
   print('  -h --help              : Print this help message.')
   print('  -r --crcHost [name]    : Name of RTI CRC Host, currently:', crc_host )
   print('  -p --crcPort [number]  : Port number for the RTI CRC, currently:', crc_port )
   print('  -s --settings [string] : RTI CRC local settings:', local_settings )
   print(' ')

   trick.exec_terminate_with_return( -1,
                                     sys._getframe(0).f_code.co_filename,
                                     sys._getframe(0).f_lineno,
                                     'Print usage message.')
   return
# END: print_usage_message


################################################################################
def parse_command_line( ):
   
   global print_usage
   global crc_host
   global crc_port
   global local_settings
   global override_settings
   
   # Get the Trick command line arguments.
   argc = trick.command_line_args_get_argc()
   argv = trick.command_line_args_get_argv()
   
   # Process the command line arguments.
   # argv[0]=S_main*.exe, argv[1]=RUN/input.py file
   index = 2
   while (index < argc):
            
      if ((str(argv[index]) == '-h') | (str(argv[index]) == '--help')):
         print_usage = True
      
      elif ((str(argv[index]) == '-r') | (str(argv[index]) == '--crcHost')):
         index = index + 1
         if (index < argc):
            crc_host = str(argv[index])
         else:
            print('ERROR: Missing --crcHost [name] argument.')
            print_usage = True
      
      elif ((str(argv[index]) == '-p') | (str(argv[index]) == '--crcPort')):
         index = index + 1
         if (index < argc):
            crc_port = int(str(argv[index]))
         else:
            print('ERROR: Missing --crcPort [port] argument.')
            print_usage = True
      
      elif ((str(argv[index]) == '-s') | (str(argv[index]) == '--settings')):
         index = index + 1
         if (index < argc):
            local_settings = str(argv[index])
            override_settings = True
         else:
            print('ERROR: Missing --settings [string] argument.')
            print_usage = True
         
      else:
         print('ERROR: Unknown command line argument ' + str(argv[index]))
         print_usage = True
   
      index = index + 1
   
   return
# END: parse_command_line


################################################################################
# Global configuration variables

# Default: Don't show usage:
print_usage = False

# Default: CRC Host string:
crc_host = "js-er7-rti.jsc.nasa.gov"

# Default: CRC port value.
crc_port = 8989

# Default: local settings string:
override_settings = False

# Parse the command line.
parse_command_line()

# Check to see if setting were overridden.
# If not, then form the local_settings string since host and port may have been set.
if not override_settings:
   local_settings = 'crcHost = ' + crc_host + '\n crcPort = ' + str(crc_port)

if (print_usage == True):
   print_usage_message()


#trick setup
trick.sim_services.exec_set_trap_sigfpe(1)
simControlPanel = trick.SimControlPanel()
trick.add_external_application(simControlPanel)
trick.real_time_enable()
trick.sim_services.exec_set_terminate_time(86400)
trick.exec_set_software_frame(0.1)
trick.exec_set_time_tic_value(100000000)
trick.TMM_reduced_checkpoint(False)
trick_mm.mm.set_expanded_arrays(True)
trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)

################################################################################
# HLA Setup
################################################################################
from Modified_data.TrickHLA.TrickHLAFederateConfig import *

# Configure the CRC, the default is the local host but can be overridden
# with command line arguments --crcHost and --crcPort or --settings.
#THLA.federate.local_settings = 'crcHost = localhost\n crcPort = 8989'
THLA.federate.local_settings = local_settings

# Configure the federate
Federation_name = "HARVEY_DEMO"
Federate_name   = "FED_2"

# THLA configuration
federate = TrickHLAFederateConfig( thla_federate        = THLA.federate,
                                   thla_manager         = THLA.manager,
                                   thla_control         = THLA.execution_control,
                                   thla_config          = THLA.simple_sim_config,
                                   thla_federation_name = Federation_name,
                                   thla_federate_name   = Federate_name,
                                   thla_enabled         = True )

# Add required federates.
federate.add_known_federate( True, "FED_1")
federate.add_known_federate( True, "FED_2")

# Set time management parameters
federate.set_HLA_base_time_units( trick.HLA_BASE_TIME_10_NANOSECONDS )
federate.set_lookahead_time( 0.1 )
federate.set_least_common_time_step( 0.1 )
federate.set_time_regulating( True )
federate.set_time_constrained( True )

# Add the sim configuration FOM.
federate.add_FOM_module( 'FOMs/DistIf/SimpleSimConfigFOM.xml' )

# Configure the Fluid Distributed Interface messages
federate.add_FOM_module( 'FOMs/DistIf/FluidDistIfFOM.xml' )
from Modified_data.DistIf.FluidDistIfObjectConfig import *

# Interfaces
data_obj = FluidDistIfAToBObjectConfig(
   thla_federate_name = 'FluidDistIf_harveyOut_to_esvIn',
   bus_name           = 'harvey.fluid.harveyIn',
   isBusA             = False,
   FOM_type           = 'FluidDistIfDataBase.FluidDistIfData_6_4' )
federate.add_fed_object( data_obj )

data_obj = FluidDistIfBToAObjectConfig(
   thla_federate_name = 'FluidDistIf_esvIn_to_harveyOut',
   bus_name           = 'harvey.fluid.harveyIn',
   isBusA             = False,
   FOM_type           = 'FluidDistIfDataBase.FluidDistIfData_6_4' )
federate.add_fed_object( data_obj )

data_obj = FluidDistIfAToBObjectConfig(
   thla_federate_name = 'FluidDistIf_harveyIn_to_esvOut',
   bus_name           = 'harvey.fluid.harveyOut',
   isBusA             = False,
   FOM_type           = 'FluidDistIfDataBase.FluidDistIfData_6_4' )
federate.add_fed_object( data_obj )

data_obj = FluidDistIfBToAObjectConfig(
   thla_federate_name = 'FluidDistIf_esvOut_to_harveyIn',
   bus_name           = 'harvey.fluid.harveyOut',
   isBusA             = False,
   FOM_type           = 'FluidDistIfDataBase.FluidDistIfData_6_4' )
federate.add_fed_object( data_obj )

# After all configuration is defined, initialize the federate
federate.initialize()
