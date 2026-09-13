#!/bin/bash

# Script meant to generate truth data for testing against.
# Copies files from test_files into truth_output, runs the netexport.py script, and captures the output.
# This should be run when test files are modified.

# Force copy test files to truth_output
cp -aT test_files/netexport/nominal/ truth_output/
cp -aT test_files/netexport/off_nominal/ truth_output/

divider="--------------------------------------------------------------------------------------------------"

# get list of nominal networks and strip the path and extension
nominal=(test_files/netexport/nominal/*.xml)
nominal=("${nominal[@]#*test_files/netexport/nominal/}")
nominal=("${nominal[@]%.xml}")

# get list of off-nominal networks and strip the path and extension
off_nominal=(test_files/netexport/off_nominal/*.xml)
off_nominal=("${off_nominal[@]#*test_files/netexport/off_nominal/}")
off_nominal=("${off_nominal[@]%.xml}")

# Nominal cases
echo "====== Nominal Cases ============================================================================="
for network in "${nominal[@]}"
do
    python3 ../netexport.py truth_output/$network.xml 2>&1 | tee truth_output/${network}_output.txt
    echo $divider
done

# Off nominal
echo "====== Off Nominal Cases ========================================================================="
for network in "${off_nominal[@]}"
do
    python3 ../netexport.py truth_output/$network.xml 2>&1 | tee truth_output/${network}_output.txt
    echo $divider
done

echo "====== Done ======================================================================================"