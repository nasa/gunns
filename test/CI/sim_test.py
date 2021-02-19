#!/usr/bin/python2.6
# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# Commenting out all the argparse stuff since it's not standard in python2.6
#import argparse
from subprocess import call
from os import environ, path
from SimTestRunner import SimTestRunner

def main():
    # Define command line options using argparse
#    argParser = argparse.ArgumentParser()
#    argParser.add_argument("-e", "--env", action="store_true", help="Configures the GUNNS Trick simulation build environment.")
#    argParser.add_argument("-c", "--compile", action="store_true", help="Compiles the GUNNS Trick sim.")
#    argParser.add_argument("-r", "--run", action="store_true", help="Runs the GUNNS Trick simulation tests.")
#    args = argParser.parse_args()
    # Instantiate the SimTest Runner class.
    simRunner = SimTestRunner()
    # Run various functions depending on the command line options given. Run all is default
#    if args.env:
#        simRunner.configure_environment()
#    elif args.compile:
#        simRunner.build_trick_sim()
#    elif args.run:
#        simRunner.run_trick_sim()
#    else:
    print ""
    print ""
    print "===================================================================================="
    print ""
    print "Running the full set of SIM_test Simulation tests"
    print "For help with usage, run the script as follows ./sim_test -h"
    print ""
    print "===================================================================================="
    print ""
    print ""
    simRunner.configure_environment()
    simRunner.build_trick_sim()
    simRunner.run_trick_sim()
    simRunner.scan_test_results()

if __name__ == "__main__":
    main()
