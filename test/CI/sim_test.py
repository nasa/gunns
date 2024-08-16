#!/usr/bin/python2.7
# Copyright 2020 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#

import os
import sys

if 'failure message' in open(os.environ["GUNNS_HOME"]+"/sims/SIM_test/RUN_test/results/SIM_test_int_test_results.xml").read():
    sys.exit(1)
