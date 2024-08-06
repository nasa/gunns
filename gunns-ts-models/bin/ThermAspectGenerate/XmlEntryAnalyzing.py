# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
####################################################################################################
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
from XmlParsing import XmlParser, TagNotFound
from ThermSupport import ThermError

#===================================================================================================
# XmlEntryAnalyzer
#===================================================================================================
## @brief:
## This class contains members and methods used in storing and analyzing xml-data from a Thermal,
## Heater, or Panel registry file. The class is passed an xml element from construction, and from
## this is gathered data about the link, heater, or panel. It's functions are called externally by
## IndivNetworkBuilder in validating the user-defined xml-data.
class XmlEntryAnalyzer:
    ## @brief:
    ## Default constructs the object with default, uninitialized members.
    def __init__(self):
        uninitialized = "[not initialized]"

        ## An XmlParser() object for getting data from xml elements.
        self.mParser = XmlParser()
        ## An xml entry from a registry file. This object is only associated with a single entry.
        self.mEntry = uninitialized
        ## A dictionary with the definitions of symbols used in the registries
        self.mSymMap = uninitialized
        ## Name of the Thermal Gunns link, as read from the registry entry.
        self.mName = uninitialized
        ## The link's number in the network's array. Initialized to an invalid value of -1.
        self.mEnumIndex = -1
        ## The link's description
        self.mDescription = uninitialized
        ## Initialization flag.
        self.mIsInitialized = False

    #===============================================================================================
    ## @brief:
    ## Constructs the class using two arguments.
    ## @param[in]: entry    ElementTree xml-element of data from Thermal, Htr, or Panel registry
    ## @param[in]: symMap   dictionary with the definitions of symbols used in the registries
    def initialize(self, entry, symMap={}):

        ## An xml entry from a registry file. This object is only associated with a single entry.
        self.mEntry = entry
        ## A dictionary with the definitions of symbols used in the registries
        self.mSymMap = symMap
        ## Name of the Thermal Gunns node/link, as read from the registry entry.
        self.mName = entry.get("name")

        ## The node entry must have a name. Raise exception if otherwise.
        if None == self.mName:
            raise ThermError("Must have a name attribute.")

        ## The link's description as read from registry entry.
        try:
            self.mDescription = self.mParser.getChildText(entry, "des")
        except TagNotFound:
            self.mDescription = ""

    #===============================================================================================
    ## @brief:
    ## Evaluate an expression value based on symbols defined in mSymMap.
    ## @param[in]: expr    a mathematical expression that may contain symbols defined in mSymMap
    ## @param[in]: info    optional string used in error reporting
    ## @return:    the evaluated expression in numerical form
    def getValidatedValue(self, expr, info=''):
        try:
            value = eval(expr, globals(), self.mSymMap)
        except (NameError, SyntaxError), e:
            print e,
            raise ThermError("Invalid value: %s (%s)." % (expr, info))

        ## Do not accept negative values.
        if value < 0:
            raise ThermError("Cannot accept a negative. %s = %f" % (expr, value))

        return value
    #-----------------------------------------------------------------------------------------------
    ## Reads specific piece of xml data.
    ## @param[in]: element xml-element from which <tag> should be pulled
    ## @param[in]: tag     tag of xml-element of data to read
    def getUnrequiredData(self, element, tag):
        ## Get data element. Return none if not found.
        try:
            return self.getValidatedValue(self.mParser.getChildText(element, tag), "<"+tag+">")
        except TagNotFound:
            return None
