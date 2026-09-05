# Script meant to test the nominal and off nominal cases for NetExport
# Use input arg 'no-gui' (e.g. ./testnetExport.sh no-gui) to open result in terminal instead of browser

# Create output dir if it doesn't exist
mkdir -p output

# Force copy test files to output
cp -aT test_files/nominal_networks/ output/
cp -aT test_files/off_nominal_networks/ output/

divider="--------------------------------------------------------------------------------------------------"

# get list of nominal networks and strip the path and extension
nominal_networks=(test_files/nominal_networks/*.xml)
nominal_networks=("${nominal_networks[@]#*test_files/nominal_networks/}")
nominal_networks=("${nominal_networks[@]%.xml}")

# get list of off-nominal networks and strip the path and extension
off_nominal_networks=(test_files/off_nominal_networks/*.xml)
off_nominal_networks=("${off_nominal_networks[@]#*test_files/off_nominal_networks/}")
off_nominal_networks=("${off_nominal_networks[@]%.xml}")

# TODO: Handle the case for no input file, which pops up a GUI to
# open a file. Need to figure out how to handle that, if possible.
# If so, do the case of selecting a file and not selecting a file.


# Nominal cases
echo "====== Nominal Tests ============================================================================="
for network in "${nominal_networks[@]}"
do
    coverage run -a ../netexport.py output/$network.xml 2>&1 | tee output/${network}_output.txt
    echo $divider
done

# Off nominal
echo "====== Off Nominal Tests ========================================================================="
for network in "${off_nominal_networks[@]}"
do
    coverage run -a ../netexport.py output/$network.xml 2>&1 | tee output/${network}_output.txt
    echo $divider
done

echo "====== Compare C++ Output for Nominal Cases ======================================================"
for network in "${nominal_networks[@]}"
do
    for fileType in "hh" "cpp"
    do
        # The only difference between the test and output files should be the timestamp. This results in a
        # diff output of 4 lines. If the diff output is longer than 4 lines, there are more differences.
        diffout=$(diff truth_output/$network.$fileType output/$network.$fileType)
        numlines=$(echo "$diffout" | wc -l)
        if [[ $numlines > "4" ]]; then
            echo "FAILED: $network.$fileType"
            echo "diff output:"
            diff -u truth_output/$network.$fileType output/$network.$fileType
        else
            echo "PASSED: $network.$fileType"
        fi
        echo $divider
    done
done

echo "====== Compare netexport.py output ==============================================================="
echo "TODO"

echo "====== Line Coverage Report ======================================================================"

if [[ $1 == "no-gui" ]]; then
    # Generate report in terminal
    coverage report -m
else
    # Generate html report and open in browser
    coverage html
    firefox htmlcov/index.html
fi