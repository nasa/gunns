####################################################################################################
## @copyright Copyright 2019 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
import os
from SymbolLoading import SymbolLoader
from ThermAspectConfiguring import ThermAspectConfig
from IndivNetworkBuilding import IndivNetworkBuilder
from ThermSupport import ThermError, ThermHeaders, ThermPrinter

#===================================================================================================
# ThermAspectBuilder
#===================================================================================================
## @brief:
## This class is the ts-models manager of the ThermAspectGenerate series. It is initialized with the
## config data it needs to build a series of thermal network config and icd files. A
## top-level orchestrator, which is sim-specific, needs only to instantiate this class, initialize
## it, and tell it to execute. The ThermAspectbuilder then loops over the networks: constructing,
## initializing, and executing an IndivNetworkBuilder for each one. The ThermAspectbuilder then
## commands each IndivNetworkBuilder to print its enumerations and icd jobs to communal files.
class ThermAspectBuilder():
    ## @brief:
    ## Default constructs the class.
    def __init__(self):

        ## Initialization flag.
        self.mInitialized = False
        uninitialized = "[%s not initialized]"

        ## Name of top-level script that creates this object.
        self.mCallingScript = uninitialized % "mCallingScript"

        ## Path to repository home.
        self.mHome = uninitialized % "mHome"

        #Multi-network, sim-level files to generate ...............
        ## Path and file name of ICD file.
        self.mIcdFile = uninitialized % "mIcdFile"
        ## Path and file name of PTCS Enumeration header file.
        self.mEnumFile = uninitialized % "mEnumFile"

        #Files to read ............................................
        ## List of xml-files that define symbols used in the thermal registries.
        self.mSymFiles = []
        ## Directory containing Thermal Aspect Registry files
        self.mRegisDir = uninitialized % "mRegisDir"

        ## List of IndivNetworkBuilder() objects. The ThermAspectBuilder will instantiate a
        ## mIndivNetworkBuilder for each network given in initialize(), then call execute() on each.
        self.mIndivNetworkBuilders = []

    #===============================================================================================
    ## @brief:
    ## Public function, called by a sim-specific top-level orchestrator. As an argument, the
    ## ThermAspectBuilder is given a list of NetworkConfigs. For each network,
    ## the ThermAspectBuilder instantiates  a IndivNetworkBuilder object and initializes it
    ## with the network-specific config object.
    ##
    ## @param[in]: thermAspectConfig   configuration data for the ThermAspectBuilder
    def initialize(self, thermAspectConfig):

        ## Initialize members.
        self.mCallingScript = thermAspectConfig.cCallingScript
        self.mHome = thermAspectConfig.cHome
        self.mIcdFile = thermAspectConfig.cIcdFile
        self.mEnumFile = thermAspectConfig.cEnumFile
        self.mSymFiles = thermAspectConfig.cSymFiles

        ## Load symbols dictionary from symbol xml files.
        print "Loading symbols."
        try:
            symMap = self.loadSymbols()
        except ThermError, e:
            print e
            raise ThermError("Error while loading symbols.")

        ## Initialize network builders.
        for networkConfig in thermAspectConfig.cListNetworkConfigs:

            ## Set sim-level members of IndivNetworkConfig object
            networkConfig.cCallingScript = self.mCallingScript
            networkConfig.cAssumedCp = thermAspectConfig.cAssumedCp
            networkConfig.cSymMap = symMap

            ## Instantiate an individual network builder and initialize.
            indivNetworkBuilder = IndivNetworkBuilder()
            indivNetworkBuilder.initialize(networkConfig)

            ## Append new network builder to array.
            self.mIndivNetworkBuilders.append(indivNetworkBuilder)

        ## Set ThermRegistry directory based on value in networkConfig
        self.mRegisDir = os.path.dirname( networkConfig.cRegisFile.replace(self.mHome, "") )

        ## Set initialization flag.
        self.mInitialized = True

    #===============================================================================================
    ## @brief:
    ## Public function, called by a sim-specific top-level orchestrator. This function opens the icd
    ## and enumeration header files. Then, it loops through the list of IndivNetworkBuilder objects
    ## created in initialize(), calling execute() on each one. It commands each IndivNetworkBuilder
    ## to print its icd and enum data to the respective files, then closes them.
    def execute(self):
        if False == self.mInitialized:
            raise ThermError("ThermAspectBuilder not initialized.")

        ## Instantiate a ThermPrinter object.
        myPrinter = ThermPrinter()

        ## Open enum and icd files.
        try:
            f_enum = myPrinter.openFileForWriting(self.mEnumFile)
            f_icd = myPrinter.openFileForWriting(self.mIcdFile)

            ## Write enum headers.
            f_enum.write(ThermHeaders().enum % self.mCallingScript)
            guard = os.path.basename(self.mEnumFile).replace('.hh','_')
            f_enum.write("#ifndef %sEXISTS\n" % guard)
            f_enum.write("#define %sEXISTS\n\n" % guard)

            ## Write icd header.
            f_icd.write(ThermHeaders().icd % (self.mRegisDir, self.mCallingScript))

        except ThermError, e:
            print e
            raise ThermError("Error setting up PTCS enum/icd files.")

        ## Indicate script is moving on to config-file writing.
        print "Generating thermal config-files for..."

        ## Loop through and build each network.
        for indivNetworkBuilder in self.mIndivNetworkBuilders:
            try:
                ## Print name of network to be built.
                print "   ~ " + indivNetworkBuilder.mNetwork

                ## Each network has a separate TrickView file.
                f_tv = myPrinter.openFileForWriting(indivNetworkBuilder.mTvFile)

                ## Execute each individual network
                indivNetworkBuilder.execute()

                ## Write enumerations.
                indivNetworkBuilder.writeToEnumFile(f_enum)

                ## Write ICD and TrickView files.
                indivNetworkBuilder.mIcdBuilder.writeToIcdAndTvFiles(f_icd, f_tv)

            except ThermError, e:
                print e
                print "Error during build: %s" % indivNetworkBuilder.mNetwork
                continue

        ## Close multi-network files.
        f_enum.write("#endif\n")
        f_enum.close()
        f_icd.close()

    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in execute(). Calls on a SymbolLoader class to build a dictionary from
    ## the symbols defined in the symbol xml files. This dictionary contains
    ## each symbol as a key, with its evaluated numerical expression as the value.
    ## @return:  symMap   dictionary with the definitions of symbols from the symbols files
    def loadSymbols(self):
        ## Create symbol dictionaries based on symbol xml-files.
        return SymbolLoader().execute(self.mSymFiles)
