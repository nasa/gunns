#!/usr/bin/env python3
# @copyright Copyright 2022 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
# @revs_title
# @revs_begin
# @rev_entry(Collin Blake, CACI, GUNNS, June 2022, --, Initial implementation.)
# @revs_end
#
import os
import sys

# More imports:
from argparse import *

##############################################################################################################################################################
### Outputs a standard heat exchanger ICD code where the user can define the number of segements of the HX and the flux index to read/write from.
### This allows this code to be used on either side of the heat exchanger. It does not check for appropriate variable names.
##############################################################################################################################################################
def format_and_output_hx(vars):

    #Test input call: python3 icdhelper.py -hx ihab sim_bus atcs_ihab itcs1CabHx ATCS_RATE IHabSubSystemsSimObject ihab_subsystems tcs.thermal.hx.cabinHx1 tcs.fluid.itcs1.CabHX CabHX 2 0

    system = vars[0]
    bus = vars[1]
    subsystem = vars[2]
    variableNamePrefix = vars[3]
    rateName = vars[4]
    simObjectType = vars[5]
    simObjectName = vars[6]
    thermalSideVarName = vars[7]
    fluidSideName = vars[8]
    fluidSideComponentName = vars[9]
    numSegs = int(vars[10])
    heatFluxIndex = vars[11]


    # First, we will output fluxes
    outStr = ''
    for i in range(0,numSegs):
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'Flux\tWRITE\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + fluidSideName + '.mSegEnergyGain[' + i + ']\tdouble\t(W)\t' + fluidSideComponentName + ' Segment ' + i + ' Flux\n'
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'Flux\tREAD\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + thermalSideVarName + 'Seg' + i + '.mExternalHeatFlux[' + heatFluxIndex + ']\tdouble\t(W)\t' + fluidSideComponentName + ' Segment ' + i + ' Flux\n'

    outStr += '\n'

    #Next we will do temperatures
    for i in range(0,numSegs):
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'Temp\tWRITE\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + thermalSideVarName + 'Seg' + i + '.mTemperature\tdouble\t(K)\t' + fluidSideComponentName + ' Segment ' + i + ' Temperature\n'
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'Temp\tREAD\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + fluidSideName + '.mSegTemperature[' + i + ']\tdouble\t(K)\t' + fluidSideComponentName + ' Segment ' + i + ' Temperature\n'

    print(outStr)


############################################################################################################################################################################
### Outputs the code for a regenerative heat exchanger and assumes the convention of cold side segment numbering for the thermal aspect, hot side is the reverse.
### The rhx is assumed to be cross-flow.
############################################################################################################################################################################
def format_and_output_rhx(vars):

    #Test input call: python3 icdhelper.py -rhx ihab sim_bus atcs_ihab itcs1RegenHx ATCS_RATE IHabSubSystemsSimObject ihab_subsystems tcs.thermal.hx.cabinHx1 tcs.fluid.itcs1.regenHXCold tcs.fluid.itcs1.regenHXHot regenHX 2

    system = vars[0]
    bus = vars[1]
    subsystem = vars[2]
    variableNamePrefix = vars[3]
    rateName = vars[4]
    simObjectType = vars[5]
    simObjectName = vars[6]
    thermalSideVarName = vars[7]
    fluidColdSideName = vars[8]
    fluidHotSideName = vars[9]
    fluidSideComponentName = vars[10]
    numSegs = int(vars[11])

    # By default
    j= numSegs -1-i

    # First, we will output fluxes
    outStr = ''
    for i in range(0,numSegs):
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'ColdFlux\tWRITE\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + fluidColdSideName + '.mSegEnergyGain[' + i + ']\tdouble\t(W)\t' + fluidSideComponentName + ' Segment ' + i + ' Flux\n'
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'ColdFlux\tREAD\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + thermalSideVarName + 'Seg' + i + '.mExternalHeatFlux[0]\tdouble\t(W)\t' + fluidSideComponentName + ' Segment ' + i + ' Flux\n'
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'HotFlux\tWRITE\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + fluidHotSideName + '.mSegEnergyGain[' + j + ']\tdouble\t(W)\t' + fluidSideComponentName + ' Segment ' + i + ' Flux\n'
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'HotFlux\tREAD\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + thermalSideVarName + 'Seg' + i + '.mExternalHeatFlux[1]\tdouble\t(W)\t' + fluidSideComponentName + ' Segment ' + i + ' Flux\n'

    outStr += '\n'

    #Next we will do temperatures
    for i in range(0,numSegs):
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'Temp\tWRITE\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + thermalSideVarName + 'Seg' + i + '.mTemperature\tdouble\t(K)\t' + fluidSideComponentName + ' Segment ' + i + ' Temperature\n'
        outStr += 'xxxx\t' + system + '\t' + system + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'Temp\tREAD\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + fluidColdSideName + '.mSegTemperature[' + i + ']\tdouble\t(K)\t' + fluidSideComponentName + ' Segment ' + i + ' Temperature\n'
        outStr += 'xxxx\t' + system + '\t' + bus + '\t' + subsystem + '\t' + variableNamePrefix + 'Seg' + i + 'Temp\tREAD\t' + rateName + '\t' + simObjectType + ' ' + simObjectName + '\t' + fluidHotSideName + '.mSegTemperature[' + j + ']\tdouble\t(K)\t' + fluidSideComponentName + ' Segment ' +i + ' Temperature\n'

    print(outStr)




#####################
# BEGIN MAIN SCRIPT #
#####################

# Assume Python 3 for now with new push for most repos moving to version 3. Init argument parser
parser = ArgumentParser(description='Generate icd connection code for common interfaces.', formatter_class=RawTextHelpFormatter, usage=SUPPRESS)

#####################
#       HX          #
#####################

# Heat exchanger helper and arguement definition
hxHelpStr = '\nGenerates icd code for a heat exchanger by defining required variables, number of segments, and which heat flux index being used. We assume certain common formats for the thermal segment naming convention. For example to generate this:\n\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1CabHxSeg0Temp  WRITE   ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.thermal.hx.cabinHx1Seg0.mTemperature    double  (K) CabHX Segment 1 Temperature\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1CabHxSeg0Temp  READ    ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.fluid.itcs1.CabHX.mSegTemperature[0]    double  (K) CabHX Segment 1 Temperature\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1CabHxSeg0Flux  WRITE   ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.fluid.itcs1.CabHX.mSegEnergyGain[0] double  (W) CabHX Segment 1 Flux\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1CabHxSeg0Flux  READ    ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.thermal.hx.cabinHx1Seg0.mExternalHeatFlux[0]    double  (W) CabHX Segment 1 Flux\n\n\
We need these variables, in order:\n\n\
ihab | atcs_ihab | itcs1CabHx | ATCS_RATE | IHabSubSystemsSimObject | ihab_subsystems | tcs.thermal.hx.cabinHx1 | tcs.fluid.itcs1.CabHX | CabHX | 1 | 0\n\n'

parser.add_argument('-hx', metavar=('[System |','Bus |','Subsystem |','VariableNamePrefix |','RateName |','SimObjectType |','SimObjectName |','ThermalSideVarName(minusSeg) |','FluidSideName |','FluidSideComponentName |','NumSegments |','HeatFluxIndex]'),nargs=12, help=hxHelpStr)

#####################
#       RHX         #
#####################

# Regenerative Heat exchanger helper and arguement definition
rhxHelpStr = '\nGenerates icd code for a CROSS-FLOW regnerative heat exchanger by defining required variables, number of segments, and which heat flux index being used. We assume certain common formats for the thermal segment naming convention. For example to generate this:\n\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1RegenHxSeg0ColdFlux  WRITE   ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.fluid.itcs1.regenHXCold.mSegEnergyGain[0] double  (W) regenHX Segment 1 Flux\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1RegenHxSeg0ColdFlux  READ    ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.thermal.hx.regenHxSeg0.mExternalHeatFlux[0]    double  (W) regenHX Segment 1 Flux\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1RegenHxSeg0HotFlux  WRITE   ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.fluid.itcs1.regenHXHot.mSegEnergyGain[0] double  (W) regenHX Segment 1 Flux\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1RegenHxSeg0HotFlux  READ    ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.thermal.hx.regenHxSeg0.mExternalHeatFlux[1]    double  (W) regenHX Segment 1 Flux\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1RegenHxSeg0Temp  WRITE   ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.thermal.hx.regenHxSeg0.mTemperature    double  (K) regenHX Segment 1 Temperature\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1RegneHxSeg0Temp  READ    ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.fluid.itcs1.regenHXCold.mSegTemperature[0]    double  (K) regenHX Segment 1 Temperature\n\
xxxx    ihab    sim_bus atcs_ihab   itcs1RegenHxSeg0Temp  READ    ATCS_RATE   IHabSubSystemsSimObject ihab_subsystems tcs.fluid.itcs1.regenHXHot.mSegTemperature[0]    double  (K) regenHX Segment 1 Temperature\n\n\
We need these variables, in order:\n\n\
ihab | atcs_ihab | itcs1RegenHx | ATCS_RATE | IHabSubSystemsSimObject | ihab_subsystems | tcs.thermal.hx.regenHx | tcs.fluid.itcs1.regenHXCold | tcs.fluid.itcs1.regenHXHot | regenHX | 1\n\n'

parser.add_argument('-rhx', metavar=('[System |','Bus |','Subsystem |','VariableNamePrefix |','RateName |','SimObjectType |','SimObjectName |','ThermalSideVarName(minusSeg) |','FluidColdSideName |','FluidHotSideName |','FluidSideComponentName |','NumSegments]'), nargs=12, help=rhxHelpStr)


# Parse the arguments and assign them to 'args'
args = parser.parse_args()

#Switch based on argument type. Add more types by adding an argument (see above) and the relative output string definition.
if args.hx:
    format_and_output_hx(args.hx)

elif args.rhx:
    format_and_output_rhx(args.rhx)
