# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
import subprocess
import os
import sys

# Note that this sim runner now requires Trick 17+.

class SimRunner(object):
    def __init__(self):
        self.this_files_path = os.path.dirname(os.path.realpath(__file__))
        self.gunns_home = os.path.join(self.this_files_path, '../..')
        os.chdir(self.gunns_home)
        self.gunns_home = os.getcwd()
        self.env_file = os.path.join(self.gunns_home, 'test', 'CI', 'env_setup.profile')
        self.trick_sim_input_file = ""
        self.test_results_file = ""

    def configure_environment(self):
        self.shell_source(self.env_file)
        self.trick_env_file = os.path.join(self.gunns_home, 'bin', 'bashrc')
        self.shell_source(self.trick_env_file)
        os.environ["GUNNS_HOME"] = self.gunns_home

    def build_trick_sim(self):
        subprocess.call(["make", "spotless"], cwd=self.trick_sim_home)
        subprocess.call(["trick-CP", ""], cwd=self.trick_sim_home)

    def run_trick_sim(self):
        subprocess.call(["ls", "-al"], cwd=self.trick_sim_home)
        subprocess.call([self.trick_sim_input_file], cwd=self.trick_sim_home, shell=True)

    def scan_test_results(self):
        if 'failure message' in open(self.test_results_file).read():
            sys.exit(1)

    def shell_source(self, script):
        """Emulates the "source" command in a shell. Will set some environment variables."""
        pipe = subprocess.Popen("unset module; . %s; env" % script, stdout=subprocess.PIPE, shell=True)
        output = pipe.communicate()[0]
        env = dict((line.split("=", 1)  for line in output.splitlines()))
        os.environ.update(env)

