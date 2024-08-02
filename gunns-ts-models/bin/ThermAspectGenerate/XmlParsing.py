# @copyright Copyright 2019 United States Government as represented by the Administrator of the
#            National Aeronautics and Space Administration.  All Rights Reserved.
#
####################################################################################################
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
import xml.etree.ElementTree as ET
from xml.parsers.expat import ExpatError
from ThermSupport import ThermError
import unicodedata
#===================================================================================================
# TagNotFound
#===================================================================================================
## @brief:
## This class is a simple exception that is raised if a tag is not found. Sometimes, it's a big
## problem; other times, it's not.
class TagNotFound(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value).replace('"','')
#===================================================================================================
# CommentedTreeBuilder
#===================================================================================================
class CommentedTreeBuilder ( ET.XMLTreeBuilder ):
    def __init__ ( self, html = 0, target = None ):
        ET.XMLTreeBuilder.__init__( self, html, target )
        self._parser.CommentHandler = self.handle_comment

    def handle_comment ( self, data ):
        self._target.start( ET.Comment, {} )
        self._target.data( data )
        self._target.end( ET.Comment )
#===================================================================================================
# XmlParser
#===================================================================================================
## @brief:
## This class is used to parse xml data from a file. It contains utility functions to retrieving
## data and raising errors. It contains none of its own members.
class XmlParser:
    #===============================================================================================
    ## @brief:
    ## Open an xml document and return the entire data tree.
    ## @param[in]: xmlFile  file name of well-formed xml document to parse
    ## @return:    root     ElementTree of entire xml data structure
    def loadFile(self, xmlFile):
        # Get root from the xml.
        try:
            root = ET.parse(xmlFile, parser = CommentedTreeBuilder()).getroot()
        except IOError, e:
            print e
            raise ThermError("Error opening file: %s" % xmlFile)
        except ExpatError, e:
            print e
            raise ThermError("Error parsing file: %s" % xmlFile)
        return root

    #===============================================================================================
    ## @brief:
    ## Get children from XML element and check for success.
    ## @param[in]: parent                ElementTree xml-element to search under
    ## @param[in]: tag                   string of tag name to find within parent element
    ## @param[in]: raiseErrorIfNotFound  boolean, if True will raise error if tag not found
    ## @param[in]: info                  optional info to report if error raised
    ## @return:    children              ElementTree data in the given tag
    def getElements(self, parent, tag, raiseErrorIfNotFound=False, info=''):
        if None == tag:
            return []

        ## Info format for error reporting.
        format = (tag, parent.tag, info)

        ## Get all children under the proper tag.
        children = parent.findall(tag)

        ######### This check is omitted because it takes too long!
        ## It is a common mistake to misprint a tag that should have an uppercase letter. For this
        ## scenario, check for a case-sensitivity issue.
        #if len(children) != len(ET.fromstring(ET.tostring(parent).lower()).findall(tag.lower())):
        #    raise ThermError("Inconsistent case. Make sure it's '%s' within <%s> (%s)." % format)

        ## If not found...
        if True == raiseErrorIfNotFound and (None == children or len(children) <= 0):
            raise TagNotFound("Cannot find <%s> within <%s> (%s)." % format)
        return children

    #===============================================================================================
    ## @brief:
    ## Get string from xml data and check for success.
    ## @param[in]: element  ElementTree xml-element to search for text
    ## @param[in]: info    optional info to report if error raised
    ## @return:    text    string of text from the given tag
    def getText(self, element, info=''):
        text = self.checkForUnicode(element.text, info)
        if None == text or "" == text:
            format = (element.tag, info)
            raise ThermError("No text provided in tag <%s> (%s)." % format )
        return text

    #===============================================================================================
    ## @brief:
    ## Get string from xml data and check for success.
    ## @param[in]: parent  ElementTree xml-element to search under
    ## @param[in]: tag     string of tag name to find within parent element
    ## @param[in]: info    optional info to report if error raised
    ## @return:    text    string of text from the given tag
    def getChildText(self, parent, tag, info=''):
        children = self.getElements(parent, tag, True, info)
        return self.getText(children[0], info)

    #===============================================================================================
    ## @brief:
    ## Create a new ElementTree element. If parent provided, the new element will be a SubElement
    ## under the given parent.
    ## @param[in]: tag       string of tag name to find within parent element
    ## @param[in]: parent    ElementTree xml-element to search under
    ## @param[in]: attrDict  dictionary with attributes to give to the element; ex. {"units","K"}
    def newElement(self, tag, parent=None, attrDict=None):
        if None == parent:
            if None == attrDict:
                return ET.Element(tag)
            else:
                return ET.Element(tag, attrDict)
        else:
            if None == attrDict:
                return ET.SubElement(parent, tag)
            else:
                return ET.SubElement(parent, tag, attrDict)
    #===============================================================================================
    ## @brief:
    ## Round a value to the given decimal and return its string.
    ## @param[in]: val   floating point value to be rounded
    ## @param[in]: dec   decimal places to round/format the given value
    ## @return:    string rounded/formatted to desired decimal places, or the original val if n/a.
    def roundValue(self, val, dec):
        ## Default code to floating point notation.
        code = "f"
        try:
            rnd = round(float(val),dec)

            ## If the number is large, use scientific notation.
            if abs(rnd) > 1e9:
                code = "e"

            format = "%." + str(dec) + code;
            return (format % rnd)

        except ValueError, e:
            raise ThermError("Cannot be converted to a float: %s" % val)

    #===============================================================================================
    ## @brief:
    ##
    ## @param[in]: s   floating point value to be rounded
    ## @param[in]: s   decimal places to round/format the given value
    ## @return:    string
    def checkForUnicode(self, s, info=''):
        if isinstance(s, unicode):
            raise ThermError("Unicode character detected.")
        else:
            return s
