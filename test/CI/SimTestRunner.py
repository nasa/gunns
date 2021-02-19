# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
import subprocess
import os
from SimRunner import SimRunner

class SimTestRunner(SimRunner):
    def __init__(self):
        super(SimTestRunner, self).__init__()
        self.trick_sim_home = os.path.join(self.gunns_home, 'sims', 'SIM_test')
        self.trick_sim_input_file ="./S_main*.exe RUN_test/input_int_test.py" 
        self.test_results_file = self.trick_sim_home + "/RUN_test/results/SIM_test_int_test_results.xml"

    def configure_environment(self):
        print "===================================================================================="
        print "Setting up build environment for SIM_test Simulation."
        print "===================================================================================="
        super(SimTestRunner, self).configure_environment()

    def build_trick_sim(self):
        print "===================================================================================="
        print "Compiling SIM_test Simulation."
        print "===================================================================================="
        super(SimTestRunner, self).build_trick_sim()

    def run_trick_sim(self):
        print "===================================================================================="
        print "Running SIM_test Simulation."
        print "===================================================================================="
        super(SimTestRunner, self).run_trick_sim()

    def scan_test_results(self):
        print "===================================================================================="
        print "Scanning SIM_test Results."
        print "===================================================================================="
        super(SimTestRunner, self).scan_test_results()
