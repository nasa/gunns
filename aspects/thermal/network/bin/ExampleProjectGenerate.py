#!/usr/bin/python
## @copyright Copyright 2019 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
import os,sys

## Set the sys.path to the generation utilities.
gunnsHome = os.environ["GUNNS_HOME"]
sys.path.append(os.path.join(os.path.dirname(__file__), gunnsHome + '/aspects/thermal/network/bin/ThermAspectGenerate'))

## Include all necessary classes.
from ThermSupport import ThermError
from ThermAspectConfiguring import ThermAspectConfig
from ThermAspectBuilding import ThermAspectBuilder
from IndivNetworkConfiguring import IndivNetworkConfig
from IndivNetworkBuilding import IndivNetworkBuilder

## Name of this file
thisScript = os.path.basename(__file__)

## All networks that are eligible to be built.
tNetworks = ['al', 'col', 'jem', 'lab','n1', 'n2', 'n3', 'pmm', 'trussS0', 'trussP1', 'trussS1', 'trussPort', 'trussStbd', 'trussZ1', 'trussMt', 'hx']

## Upper-level paths.
tIssHome = os.environ["TS_ISS_HOME"] + '/'
tAspectRegistry = tIssHome + "iss/aspect_registry/thermal/"
tPtcsModules = tIssHome + "iss/sims/S_modules/ptcs/network/"

## File name specifics
tRegisPath = tAspectRegistry + "ThermRegistry_%s.xml"
tTdPath = tIssHome + "iss/models/ptcs/thermal/ThermalDesktop/%s/TdNetworkConfig_%s.xml"
tNodePath = tPtcsModules + "nodes/ThermNodes_%s.xml"
tCondPath = tPtcsModules + "conduction/ThermLinksCond_%s.xml"
tRadPath = tPtcsModules + "radiation/ThermLinksRad_%s.xml"
tHtrPath = tPtcsModules + "heater/HtrRegistry_%s.xml"
tPanPath = tPtcsModules + "panels/ThermPanels_%s.xml"
tEtcPath = tPtcsModules + "other/ThermLinksEtc_%s.xml"

## Path and names of symbols files, used to create a dictionary of symbols used in the ThermRegistry
tSymPath = tIssHome + "iss/aspect_registry/thermal/symbols/"
tSymFiles = [tSymPath + "symbolsCap.xml", tSymPath + "symbolsCond.xml", tSymPath + "symbolsMisc.xml"]

## Name and path of ptcs enumeration header to generate
tEnumFile = tIssHome + "iss/sims/S_modules/ptcs/enum/IssPtcsEnum.hh"

## Name of icd file and path of trickView file
tIcdFile = tIssHome + "iss/icd/PTCS.txt"
tTvPath = tIssHome + "iss/sims/S_modules/ptcs/TV_thermal/TV_icd_%s.tv"

## Uniform ICD variables
tIcdSys = "iss"
tIcdBus = "sim_bus"
tIcdTrickSeq = "P_PTCS "

#==================================================================================================
# Main script
#==================================================================================================
if __name__ == "__main__":
    print "\n======================================================================"
    print thisScript

    ## Instantiate ThermAspectConfig object.
    thermAspectConfig = ThermAspectConfig()

    ## Name of top-level script that creates the ThermAspectBuilder that creates this object.
    thermAspectConfig.cCallingScript = thisScript
    ## Path to repository home.
    thermAspectConfig.cHome = tIssHome
    ## Assumption to use for any specific heat values (Cp) not provided directly.
    thermAspectConfig.cAssumedCp = 896.0
    ## Set ICD file
    thermAspectConfig.cIcdFile = tIcdFile
    ## Set enumeration header file.
    thermAspectConfig.cEnumFile = tEnumFile
    ## Set symbol_loading module and files to read.
    thermAspectConfig.cSymFiles = tSymFiles

    ## Loop through all individual networks.
    for network in tNetworks:

        ## Instantiate IndivNetworkConfig object.
        indivNetworkConfig = IndivNetworkConfig()

        ## Uniform ICD variables
        ## Name of sim system
        indivNetworkConfig.cIcdSettings.mSys = tIcdSys
        ## Name of icd bus
        indivNetworkConfig.cIcdSettings.mBus = tIcdBus
        ## Name of icd trick sequence, or P-value.
        indivNetworkConfig.cIcdSettings.mTrickSeq = tIcdTrickSeq

        ## Name (abbreviation) of network to be built
        indivNetworkConfig.cNetwork = network

        ## Boolean to determine if the mass of structural nodes should be adjusted to match a
        ## total-mass expected value.
        indivNetworkConfig.cIsMassAdjustable = True

        #Files to read ............................................
        ## Path and file name of Thermal Aspect Registry file.
        indivNetworkConfig.cRegisFile = tRegisPath % network
        ## Path and file name of Thermal Desktop file (TdNetworkConfig).
        ##
        ## The trussPort, trussStbd, trussS0 and trussS1 xml files were hand-modified
        ## to adjust the radiation coeffecients without using Thermal Desktop
        ## BH  06/11/2015
        ##
        if 'trussPort' == network:
           temp_tTdPath = tIssHome + "iss/models/ptcs/thermal/ThermalDesktop/trussPort/TdNetworkConfig_trussPortTuned.xml"
           indivNetworkConfig.cTdFile = temp_tTdPath
           print "NOTE: Using %s." %  indivNetworkConfig.cTdFile
        elif 'trussStbd' == network:
           temp_tTdPath = tIssHome + "iss/models/ptcs/thermal/ThermalDesktop/trussStbd/TdNetworkConfig_trussStbdTuned.xml"
           indivNetworkConfig.cTdFile = temp_tTdPath
           print "NOTE: Using %s." %  indivNetworkConfig.cTdFile
        elif 'trussP1' == network:
           temp_tTdPath = tIssHome + "iss/models/ptcs/thermal/ThermalDesktop/trussP1/TdNetworkConfig_trussP1Tuned.xml"
           indivNetworkConfig.cTdFile = temp_tTdPath
           print "NOTE: Using %s." %  indivNetworkConfig.cTdFile
        elif 'trussS1' == network:
           temp_tTdPath = tIssHome + "iss/models/ptcs/thermal/ThermalDesktop/trussS1/TdNetworkConfig_trussS1Tuned.xml"
           indivNetworkConfig.cTdFile = temp_tTdPath
           print "NOTE: Using %s." %  indivNetworkConfig.cTdFile
        else:
           indivNetworkConfig.cTdFile = tTdPath % (network,network)


        ## Path and file name of HtrRegistry file.
        indivNetworkConfig.cHtrFile = tHtrPath % network
        ## Path and file name of ThermalPanel registry/config file.
        indivNetworkConfig.cPanFile = tPanPath % network

        #Files to generate ........................................
        ## Path and file name of Node configuration file.
        indivNetworkConfig.cNodeFile = tNodePath % network
        ## Path and file name of Conduction configuration file.
        indivNetworkConfig.cCondFile = tCondPath % network
        ## Path and file name of Radiation configuration file.
        indivNetworkConfig.cRadFile = tRadPath % network
        ## Path and file name of Et.Cetera (just potentials, really) configuration file.
        indivNetworkConfig.cEtcFile = tEtcPath % network
        ## Path and file name of TrickView file. Each network produces its own TrickView file.
        indivNetworkConfig.cTvFile = tTvPath % network

        ## Nullify unneeded files in the MT network
        if 'trussMt' == network:
            indivNetworkConfig.cEtcFile = None

        ## Nullify unneeded files in COL and JEM network
        if 'col' == network or 'jem' == network:
            indivNetworkConfig.cHtrFile = None

        ## Nullify unneeded files in the HX network.
        if 'hx' == network:
            indivNetworkConfig.cTdFile = None
            indivNetworkConfig.cHtrFile = indivNetworkConfig.cHtrFile.replace('HtrRegis','SrcRegis')
            indivNetworkConfig.cPanFile = None

            ## Change the HX to operate at a higher rate
            indivNetworkConfig.cIcdSettings.mPtcsRate = "HX_RATE"

        ## Append the newly-constructed mIndivNetworkConfig to the list of network configs.
        thermAspectConfig.cListNetworkConfigs.append(indivNetworkConfig)

    try:
        ## Instantiate, initialize, and execute the ThermAspectBuilder.
        builder = ThermAspectBuilder()
        builder.initialize(thermAspectConfig)
        builder.execute()

    except Exception, e:
        ## Build error message, checking first to see if colors are enabled.
        if sys.stdout.isatty():
            # print in red
            print "\033[31mERROR!!!!!!!!!!!!!!!\033[0m",
        else:
            # print normal
            print "ERROR!!!!!!!!!!!!!!!!!!!!!!!!!!",
        print e
        print "Building of PTCS networks has FAILED.\n"

    print "======================================================================"
