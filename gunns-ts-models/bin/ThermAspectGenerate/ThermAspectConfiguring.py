# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
####################################################################################################
## created: Mar 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################

#===================================================================================================
# NetworkConfig
#===================================================================================================
## @brief
##
class ThermAspectConfig():
    ## @brief:
    ## Constructs the object with default, uninitialized members. The members need to be
    ## set explicitly by a top-level orchestrator.
    def __init__(self):

        notSet = "[%s must explicitly be set]"

        ## Name of top-level script that creates the ThermAspectBuilder that creates this object.
        self.cCallingScript = notSet % "cCallingScript"

        ## Path to repository home.
        self.cHome = notSet % "cHome"

        ## Assumption to use for any specific heat values (Cp) not provided directly.
        self.cAssumedCp = 896.0

        #Multi-network, sim-level files to generate ...............
        ## Path and file name of ICD file.
        self.cIcdFile = notSet % "cIcdFile"
        ## Path and file name of PTCS Enumeration header file.
        self.cEnumFile = notSet % "cEnumFile"

        #Files to read ............................................
        ## List of xml-files that define symbols used in the thermal registries.
        self.cSymFiles = []

        ## List of NetworkConfig objects. The members in these objects are path/file names
        ## of the thermal xml files that will be parsed/generated in the execute() method.
        ## The config objects will be used to initialize each IndivNetworkBuilder that
        ## this class creates.
        self.cListNetworkConfigs = []
