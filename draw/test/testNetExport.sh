#!/bin/bash

# Script meant to test the nominal and off nominal cases for NetExport
# Use input arg 'no-gui' (e.g. ./testnetExport.sh no-gui) to open result in terminal instead of browser

# Create output dir if it doesn't exist
mkdir -p output

# Force copy test files to output
cp -aT test_files/netexport/nominal/ output/
cp -aT test_files/netexport/off_nominal/ output/

divider="--------------------------------------------------------------------------------------------------"

# get list of nominal networks and strip the path and extension
nominal=(test_files/netexport/nominal/*.xml)
nominal=("${nominal[@]#*test_files/netexport/nominal/}")
nominal=("${nominal[@]%.xml}")

# get list of off-nominal networks and strip the path and extension
off_nominal=(test_files/netexport/off_nominal/*.xml)
off_nominal=("${off_nominal[@]#*test_files/netexport/off_nominal/}")
off_nominal=("${off_nominal[@]%.xml}")


# Checks the difference between the test output and the truth output.
# Input - the name of the file to be checked.
checkFileDiff () {
    # The only difference between the test and output files is the timestamp. This results in a
    # diff output of 4 lines. If the diff output is longer than 4 lines, there are more differences.
    diffout=$(diff truth_output/$1 output/$1)

    if [[ $diffout == "" ]]; then
        numlines="0"
    else
        numlines=$(echo "$diffout" | wc -l)
    fi

    if [[ $numlines > "4" ]]; then
        echo "FAILED: $1"
        echo "diff output:"
        diff -u truth_output/$1 output/$1
    else
        echo "PASSED: $1"
    fi
}



# TODO: Handle the case for no input file, which pops up a GUI to
# open a file. Need to figure out how to handle that, if possible.
# If so, do the case of selecting a file and not selecting a file.


# Nominal cases
echo "====== Nominal Tests ============================================================================="
for network in "${nominal[@]}"
do
    coverage run -a ../netexport.py output/$network.xml 2>&1 | tee output/${network}_output.txt
    echo $divider
done

# Off nominal
echo "====== Off Nominal Tests ========================================================================="
for network in "${off_nominal[@]}"
do
    coverage run -a ../netexport.py output/$network.xml 2>&1 | tee output/${network}_output.txt
    echo $divider
done

echo "====== Compare C++ Output for Nominal Cases ======================================================"
for network in "${nominal[@]}"
do
    for fileType in "hh" "cpp"
    do
        checkFileDiff $network.$fileType
        echo $divider
    done
done

echo "====== Compare network maintenance output ========================================================"
echo "TODO"


echo "====== Compare netexport.py output ==============================================================="
for network in "${nominal[@]}" "${off_nominal[@]}"
do
    checkFileDiff ${network}_output.txt
    echo $divider
done

echo "====== Line Coverage Report ======================================================================"

if [[ $1 == "no-gui" ]]; then
    # Generate report in terminal
    coverage report -m
else
    # Generate html report and open in browser
    coverage html
    firefox htmlcov/index.html
fi