#!/bin/tcsh
# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
# The following lines are the "convert_TD_output.sh" shell script.
# The outputs of Thermal Desktop case sets are moved from the 'output' directory
# and given a name specific to the network. 'dos2unix' is then run.
#
# programmer: joseph.valerioti@nasa.gov     Feb 2012
# =================================================================================================

echo "\nCopying Thermal Desktop output to iss/models/ptcs/thermal/ThermalDesktop/ . . ."

# Airlock -----------------------
echo " - Airlock"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/al/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/al/TdNetworkConfig_al.xml

# COL -----------------------
echo " - COL"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/col/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/col/TdNetworkConfig_col.xml

# JEM -----------------------
echo " - JEM"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/jem/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/jem/TdNetworkConfig_jem.xml

# Node 1 -----------------------
echo " - Node 1"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n1/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n1/TdNetworkConfig_n1.xml

# Node 2 -----------------------
echo " - Node 2"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n2/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n2/TdNetworkConfig_n2.xml

# Node 3 -----------------------
echo " - Node 3"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n3/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n3/TdNetworkConfig_n3.xml

# Lab -----------------------
echo " - Lab"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/lab/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/lab/TdNetworkConfig_lab.xml

# PMM -----------------------
echo " - PMM"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/pmm/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/pmm/TdNetworkConfig_pmm.xml

# Truss S0 -----------------------
echo " - TrussS0"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussS0/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussS0/TdNetworkConfig_trussS0.xml

# Truss P1 -----------------------
echo " - TrussP1"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussP1/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussP1/TdNetworkConfig_trussP1.xml

# Truss S1 -----------------------
echo " - TrussS1"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussS1/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussS1/TdNetworkConfig_trussS1.xml

# Truss Port -----------------------
echo " - TrussPort"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussPort/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussPort/TdNetworkConfig_trussPort.xml

# Truss Stbd -----------------------
echo " - TrussStbd"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussStbd/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussStbd/TdNetworkConfig_trussStbd.xml

# Truss Z1 -----------------------
echo " - TrussZ1"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussZ1/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussZ1/TdNetworkConfig_trussZ1.xml

# Truss MT -----------------------
echo " - TrussMT"
cp $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussMt/outputs/case0/TdNetworkConfig.xml \
$TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussMt/TdNetworkConfig_trussMt.xml

# run dos2unix script to get rid of pesky newline characters
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/al/TdNetworkConfig_al.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/col/TdNetworkConfig_col.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/jem/TdNetworkConfig_jem.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/lab/TdNetworkConfig_lab.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n1/TdNetworkConfig_n1.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n2/TdNetworkConfig_n2.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/n3/TdNetworkConfig_n3.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/pmm/TdNetworkConfig_pmm.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussS0/TdNetworkConfig_trussS0.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussP1/TdNetworkConfig_trussP1.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussS1/TdNetworkConfig_trussS1.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussPort/TdNetworkConfig_trussPort.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussStbd/TdNetworkConfig_trussStbd.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussZ1/TdNetworkConfig_trussZ1.xml
dos2unix $TS_ISS_HOME/iss/models/ptcs/thermal/ThermalDesktop/trussMt/TdNetworkConfig_trussMt.xml

echo "\nDone.\n"

