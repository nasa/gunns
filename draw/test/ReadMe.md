# Intro
The purpose of this test suite is to exercise the capability of these python scripts: [netexport.py](../netexport.py), [supexport.py](../supexport.py), & [supcreate.py](../supcreate.py).
Specifically, these python scripts can generate output files, modify input networks, & generate console outputs.
As GUNNS receives updates, these capabilities need to be continuously checked.
This is done by having a set of test networks, located in the [test_files](test_files) folder.

The networks in `test_files/**/nominal` are intended to test the usual capabilities of the python scripts, i.e. the scripts should generate files as needed, maintain input files, & exit without error.
Note that some of these networks contain intentionally outdated components.
This is to demonstrate that the export python scripts can properly update such components.

The networks in `test_files/**/off_nominal` are intended to test the error-handling capabilities of the python scripts.
The scripts should exit with error, and explain that error in the console output.

# Method
Each python script has a corresponding test bash script: [testNetExport.sh](testNetExport.sh), [testSupExport.sh](testSupExport.sh), & [testSupCreate.sh](testSupCreate.sh).
Each of them do the following:

1. Every relevant test file in [test_files](test_files) is copied to the [output](output) folder.
2. The corresponding python script is run, also generating a line coverage report in the process.
3. The resulting output files are compared to the files in [truth_output](truth_output).

# Gererating Truth Data
When adding new test networks or when [netexport.py](../netexport.py), [supexport.py](../supexport.py), or [supcreate.py](../supcreate.py) are updated, new truth data may need to be generated.
To do so, run the corresponding script ([generateNetExportTruthData.sh](generateNetExportTruthData.sh), [generateSupExportTruthData.sh](generateSupExportTruthData.sh), or [generateSupCreateTruthData.sh](../test/generateSupCreateTruthData.sh)), then commit the updated files in the [truth_output](truth_output) folder.