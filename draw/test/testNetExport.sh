# Script meant to test the nominal and off nominal cases for NetExport

# Create output dir if it doesn't exist
mkdir -p output

# Force copy test files to output
cp -aT test_files/ output/

# Nominal cases
coverage run -a ../netexport.py output/NetExportBaseNominal.xml
coverage run -a ../netexport.py output/NetExportThermalNominal.xml
coverage run -a ../netexport.py output/NetExportPowerNominal.xml
coverage run -a ../netexport.py output/NetExportFluidNominal.xml

# Off nominal
# TODO

# Generate report in terminal
coverage report -m

# Generate html report and open in browser
# coverage html
# firefox htmlcov/index.html