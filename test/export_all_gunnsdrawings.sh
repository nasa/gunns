#!/bin/tcsh -f

# Copyright 2019 United States Government as represented by the Administrator of the
# National Aeronautics and Space Administration.  All Rights Reserved.
#
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/draw/drawings/DrawElect.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/draw/drawings/DrawFluid.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/draw/drawings/DrawFluidTc.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/basic/test/TestBasicNetwork.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/electrical/PvArrayRegTest.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/electrical/test/TestElectSarNetwork.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/example_space_vehicle/ExampleCabinFluid.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/example_space_vehicle/ExampleCoolantLoopFluid.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/example_space_vehicle/ExampleHxThermal.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/example_space_vehicle/ExamplePassiveThermal.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/example_space_vehicle/ExamplePowerElect.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/fluid/test/TestFluidNetwork.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/fluid/test/distributedIf/DistributedIfFluid.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/fluid/test/distributedIf/DistributedIfFluidAccum.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/fluid/test/distributedIf/DistributedIfFluidLiquid.xml
python ${GUNNS_HOME}/draw/netexport.py ${GUNNS_HOME}/sims/networks/thermal/gunnshowThermalTest/GunnShowThermalTest.xml
