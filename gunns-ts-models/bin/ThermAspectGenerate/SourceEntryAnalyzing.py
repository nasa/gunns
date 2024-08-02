# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
####################################################################################################
## created: Nov 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
from XmlEntryAnalyzing import XmlEntryAnalyzer
from XmlParsing import XmlParser, TagNotFound
from ThermSupport import ThermError

#===================================================================================================
# SourceEntryAnalyzer
#===================================================================================================
## @brief:
## This class contains members used in storing and analyzing xml-data from an entry in an Thermal
## xml file. The class is passed an xml element from construction, and from
## this is gathered data about the link. Its functions are inherited from the parent class. They
## are called externally by IndivNetworkBuilder in validating the user-defined xml-data.
class SourceEntryAnalyzer(XmlEntryAnalyzer):
    ## @brief:
    ## Default constructs the object with default, uninitialized members.
    def __init__(self):
        uninitialized = "[not initialized]"

        ## Base class constructor.
        XmlEntryAnalyzer.__init__(self)

        ## The initial temperature of the node.
        self.mLinkType = uninitialized

        ## A list of nodes to which this source is linked.
        self.mNodeList = []

        ## Other pieces of data that a source may have
        self.mScalar = None
        self.mArea = None
        self.mAbsorp = None
        self.mInitFlux = None
    #===============================================================================================
    ## @brief:
    ## Initializes the class using three arguments.
    ## @param[in]: linkType   type of link being read
    ## @param[in]: entry      ElementTree xml-element of data from Thermal, Htr, or Panel registry
    ## @param[in]: symMap     dictionary with the definitions of symbols used in the registries
    def initialize(self, linkType, entry, symMap={}):
        ## The initial temperature of the node.
        self.mLinkType = linkType.lower()

        if self.mLinkType != "heater" and \
           self.mLinkType != "source" and \
           self.mLinkType != "panel":
            raise ThermError("Link type does not exist (%s)." % self.mLinkType)

        ## Base class initializer
        XmlEntryAnalyzer.initialize(self, entry, symMap)

        ## Initialize properties specific to ThermalSource entries.
        self.loadData()

        ## Raise the flag only if we've made it this far.
        self.mIsInitialized = True
    #-----------------------------------------------------------------------------------------------
    ## Reads and sets data contained in XML.
    def loadData(self):
        ## Get node elements.
        nodeElements = self.mParser.getElements(self.mEntry, "node", True)

        ## Append nodes to list.
        for nodeElement in nodeElements:
            self.mNodeList.append(self.mParser.getText(nodeElement))

        ## The purpose of the following code is to make sure that if a piece of data is provided in
        ## the xml, it is valid. As in, not a string, an invalid expression, or negative.
        self.mScalar = self.getUnrequiredData(self.mEntry, "scalar")
        self.mArea = self.getUnrequiredData(self.mEntry, "area")
        self.mAbsorp = self.getUnrequiredData(self.mEntry, "absorp")
        self.mInitFlux = self.getUnrequiredData(self.mEntry, "initFlux")
