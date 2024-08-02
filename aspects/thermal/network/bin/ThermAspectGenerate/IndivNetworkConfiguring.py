####################################################################################################
## @copyright Copyright 2019 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## created: Mar 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
from IcdSupport import IcdSettings

#===================================================================================================
# NetworkConfig
#===================================================================================================
## @brief
##
class IndivNetworkConfig():
    ## @brief:
    ## Constructs the object with default, uninitialized members. The members need to be
    ## set explicitly by a top-level orchestrator.
    def __init__(self):

        notSet = "[%s must explicitly be set]"

        ## Name (abbreviation) of network to be built. Must be identical to string found
        ## in network-specific file names (e.g., 'trussMid' -> ThermNodes_trussMid.xml).
        self.cNetwork = notSet % "cNetwork"

        ## This class will use the default IcdSettings unless explicitly changed.
        self.cIcdSettings = IcdSettings()

        ## Name of top-level script that creates the ThermAspectBuilder that creates this object.
        self.cCallingScript = notSet % "cCallingScript"

        ## A dictionary with the definitions of symbols used in the registries
        self.cSymMap = notSet % "cSymMap"

        ## Assumption to use for any specific heat values (Cp) not provided directly.
        self.cAssumedCp = notSet % "cAssumedCp"

        ## Boolean to determine if the mass of structural nodes should be adjusted to match a
        ## total-mass expected value.
        self.cIsMassAdjustable = notSet % "cIsMassAdjustable"

        #Files to read ............................................
        ## Path and file name of Thermal Aspect Registry file.
        self.cRegisFile = notSet % "cRegisFile"
        ## Path and file name of Thermal Desktop file (TdNetworkConfig).
        self.cTdFile = notSet % "cTdFile"
        ## Path and file name of HtrRegistry file.
        self.cHtrFile = notSet % "cHtrFile"
        ## Path and file name of ThermalPanel registry/config file.
        self.cPanFile = notSet % "cPanFile"

        #Files to generate ........................................
        ## Path and file name of Node configuration file.
        self.cNodeFile = notSet % "cNodeFile"
        ## Path and file name of Conduction configuration file.
        self.cCondFile = notSet % "cCondFile"
        ## Path and file name of Radiation configuration file.
        self.cRadFile = notSet % "cRadFile"
        ## Path and file name of Et.Cetera (potentials, sources) configuration file.
        self.cEtcFile = notSet % "cEtcFile"
        ## Path and file name of TrickView file. Each network produces its own TrickView file.
        self.cTvFile = notSet % "cTvFile"
