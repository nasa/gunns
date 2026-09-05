# Script meant to generate truth data for testing against.
# Copies files from test_files into truth_output, runs the netexport.py script, and captures the output.

# Force copy test files to truth_output
cp -aT test_files/nominal_networks/ truth_output/
cp -aT test_files/off_nominal_networks/ truth_output/

divider="--------------------------------------------------------------------------------------------------"

# get list of nominal networks and strip the path and extension
nominal_networks=(test_files/nominal_networks/*.xml)
nominal_networks=("${nominal_networks[@]#*test_files/nominal_networks/}")
nominal_networks=("${nominal_networks[@]%.xml}")

# get list of off-nominal networks and strip the path and extension
off_nominal_networks=(test_files/off_nominal_networks/*.xml)
off_nominal_networks=("${off_nominal_networks[@]#*test_files/off_nominal_networks/}")
off_nominal_networks=("${off_nominal_networks[@]%.xml}")

# Nominal cases
echo "====== Nominal Cases ============================================================================="
for network in "${nominal_networks[@]}"
do
    python3 ../netexport.py truth_output/$network.xml 2>&1 | tee truth_output/${network}_output.txt
    echo $divider
done

# Off nominal
echo "====== Off Nominal Cases ========================================================================="
for network in "${off_nominal_networks[@]}"
do
    python3 ../netexport.py truth_output/$network.xml 2>&1 | tee truth_output/${network}_output.txt
    echo $divider
done

echo "====== Done ======================================================================================"