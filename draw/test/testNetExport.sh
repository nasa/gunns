# Script meant to test the nominal and off nominal cases for NetExport

# Create output dir if it doesn't exist
mkdir -p output

# Force copy test files to output
cp -aT test_files/ output/

divider="--------------------------------------------------------------------------------------------------"

nominal_networks=(
    "NetExportBaseNominal"
    "NetExportThermalNominal"
    "NetExportPowerNominal"
    "NetExportFluidNominal"
    )

off_nominal_networks=(
    "EmptyNetwork"
    "NetExportBaseOffNominal01"
    "NetExportBaseOffNominal02"
    "NetExportFluidOffNominal01"
    "NetExportFluidOffNominal02"
    )

# TODO: Handle the case for no input file, which pops up a GUI to
# open a file. Need to figure out how to handle that, if possible.
# If so, do the case of selecting a file and not selecting a file.


# Nominal cases
echo "====== Nominal Tests ============================================================================="
for network in "${nominal_networks[@]}"
do
    coverage run -a ../netexport.py output/$network.xml
    echo $divider
done

# Off nominal
echo "====== Off Nominal Tests ========================================================================="
for network in "${off_nominal_networks[@]}"
do
    coverage run -a ../netexport.py output/$network.xml
    echo $divider
done

echo "====== Compare C++ Output ========================================================================"
for network in "${nominal_networks[@]}"
do
    for fileType in "hh" "cpp"
    do
        # The only difference between the test and output files should be the timestamp. The results in a
        # diff output of 4 lines. If the diff output is longer than 4 lines, there are more differences.
        diffout=$(diff test_files/$network.$fileType output/$network.$fileType)
        numlines=$(echo "$diffout" | wc -l)
        if [[ $numlines != "4" ]]; then
            echo "FAILED: $network.$fileType"
            echo "diff output:"
            diff -u test_files/$network.$fileType output/$network.$fileType
        else
            echo "PASSED: $network.$fileType"
        fi
        echo $divider
    done
done

echo "====== Line Coverage Report ======================================================================"

# Generate report in terminal
coverage report -m

# Generate html report and open in browser
# coverage html
# firefox htmlcov/index.html