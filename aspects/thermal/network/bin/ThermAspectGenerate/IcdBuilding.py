####################################################################################################
## @copyright Copyright 2019 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
from IcdSupport import IcdSettings, IcdExchange
from ThermSupport import ThermError, ThermHeaders
from XmlParsing import XmlParser, TagNotFound

#===================================================================================================
# IcdBuilder
#===================================================================================================
## @brief:
## This class handles all the Icd processing for a IndivNetworkBuilder. Each IndivNetworkBuilder
## will have its own IcdBuilder. The IcdBuilder gets data from the xml-elements in
## the ThermRegistry and converts them into IcdExchanges. It prints the IcdExchanges,
## in the correct format, to both an icd txt-file and a TrickView tv-file.
class IcdBuilder:
    ## @brief:
    ## Default constructs the object with default, uninitialized members.
    def __init__(self):
        uninitialized = "[not initialized]"

        ## IcdSettings() object contains specifics used in Thermal Aspect ICD printing.
        ## For example, the IcdBuilder needs to know that:
        ## - On the registry, a capacitance link uses the XML tag <temperature> to describe
        ##    an ICD "READ" exchange.
        ## - The unit for temperature is "K".
        ## - The PTCS member that contains the temperature is
        ##    "therm.mCapacitanceLinks[%s].mTemperature".
        self.mIcdSettings = uninitialized

        ## Name of network using this IcdBuilder() class.
        self.mNetwork = uninitialized

        ## List of Icd Exchange() objects. This list is appended with all the WRITE/READ groups
        ## that are described in the thermal registry files. After all data has been read,
        ## the IcdBuilder loops through each and prints them to the PTCS.txt icd-file.
        self.mListOfExchanges = []

        ## An XmlParser() object for setting data in xml elements.
        self.mParser = XmlParser()

    #===============================================================================================
    ## @brief:
    ## Public function. Initializes the class with data common to all icd actions.
    ## @param[in]: network      string of abbreviated network name, as found in Thermal file names
    ##                                                ex: 'trussMid' -> ThermNodes_trussMid.xml
    ## @param[in]: icdSettings  IcdSettings() obj containing specifics used in Thermal ICD printing
    def initialize(self, network, icdSettings):

        ## Network this IcdBuilder was made for.
        self.mNetwork = network

        ## ICD settings specific to this network
        self.mIcdSettings = icdSettings

    #===============================================================================================
    ## @brief:
    ## Public function, called by IndivNetworkBuilder. This function reads the data in a
    ## ThermRegistryEntryAnalyzer object and from it, creates and initializes IcdExchange objects.
    ## @param[in]: entryObj      An initialized ThermRegistryEntryAnalyzer object
    ## @param[in]: linkType      type of Gunns object, either "potential" or "capacitance"
    ## @param[in]: enumIndex     the number in the PTCS enumeration (its array index in the
    ##                               Gunns thermal link array)
    def extractAndProcessIcd(self, entryObj, linkType, enumIndex ):

        ## Get link type, id, and description.
        comments = entryObj.mDescription
        identifier = entryObj.mName
        if "potential" == linkType:
            identifier = "pot_" + identifier

        ## Get icd data contained in entryObj.
        linkElements = self.mParser.getElements(entryObj.mEntry, linkType)
        if len(linkElements) != 1:
            return

        ## Process Icd.
        self.processIcd(linkElements[0], identifier, linkType, enumIndex, comments )

    #===============================================================================================
    ## @brief:
    ## Public function, called by IndivNetworkBuilder. This function reads the data in a
    ## ThermRegistryEntryAnalyzer object and from it, creates and initializes IcdExchange objects.
    ## @param[in]: element       xml-element containing an icd job, whose tag depends on linkType
    ## @param[in]: identifier    string that identifies the particular link, used
    ##                               in the PTCS enumeration
    ## @param[in]: linkType      type of Gunns object, either "potential" or "capacitance"
    ## @param[in]: enumIndex     the number in the PTCS enumeration (its array index in the
    ##                               Gunns thermal link array)
    ## @param[in]: comments      comments describing icd exchange
    def processIcd(self, element, identifier, linkType, enumIndex, comments ):

        ## Declare generic data.
        pui = "xxxx"
        subsys = "ptcs"
        type = "double"

        try:
            ## Find the icd category, the tag name under which the icd data is contained
            ## (<temperature>, <heatFlux>, <viewScalar>, etc).
            categoryThatPtcsReads = self.mIcdSettings.mCategoryThatPtcsReads[linkType]
            categoryThatPtcsWrites = self.mIcdSettings.mCategoryThatPtcsWrites[linkType]

            ## Get icd data.
            otherSubsWrites = self.mParser.getElements(element, categoryThatPtcsReads)
            otherSubsReads = self.mParser.getElements(element, categoryThatPtcsWrites)

            #.............................
            ## PTCS Writing
            if len(otherSubsReads) > 0:
                ## Create an IcdExchange object and append it to mListOfExchanges.
                self.createIcdExchange(identifier, linkType, enumIndex, categoryThatPtcsWrites, "",\
                                       None, otherSubsReads, pui, subsys, type, comments)
            #.............................
            ## PTCS Reading
            for i in range(0, len(otherSubsWrites) ):
                ## Create an IcdExchange object and append it to mListOfExchanges.
                self.createIcdExchange(identifier, linkType, enumIndex, categoryThatPtcsReads, str(i),\
                                       otherSubsWrites[i], None, pui, subsys, type, comments)

        except (ThermError, TagNotFound), e:
                print e,
                print "%s job will not be written on ICD." % identifier
    #===============================================================================================
    ## @brief:
    ## Public function, called internally by processIcd() method as well as externally by
    ## IndivNetworkBuilder object (for htr, rad, panel links). This function is passed xml-elements
    ## as read from the Thermal Aspect Registry. At least one of the elements (write or read) must
    ## be None. This is because the PTCS subsystem will always be the missing half of the icd
    ## exchange. The function fills the empty element with an appropriate PTCS xml-element that
    ## it creates. The data is then used to instantiate and initialize an IcdExchange object.
    ## @param[in]: id            string that identifies the particular link, used
    ##                               in the PTCS enumeration
    ## @param[in]: linkType      type of Gunns link object, "potential", "capacitor", "heater", etc
    ## @param[in]: enumIndex     the number in the PTCS enumeration (its array index in the
    ##                               Gunns thermal link array)
    ## @param[in]: category      string, xml tag in the ThermRegistry of the icd job
    ##                               corresponding to each type of thermal Gunns link
    ##                               ex: <viewScalar> for rad read, <temperature> for cap write
    ## @param[in]: iter          string, the number of the iteration currently being processed
    ##                               ex: iteration '3' of a capacitor with five <temperature> reads
    ## @param[in]: writeElement  xml-element with data needed to construct a WRITE job,
    ##                               children must be <rate>, <simObject>, and <simVarName>,
    ##                               or a single <fromOtherIcd>
    ## @param[in]: readElements  xml-elements with data needed to construct READ jobs,
    ##                               children must be <rate>, <simObject>, and <simVarName>
    ## @param[in]: pui           pui representing data being exchange, or just "xxxx"
    ## @param[in]: subsys        subsystem involved in data exchange, ex: "ptcs"
    ## @param[in]: varType       type of data being exchanged, ex: "float", "double"
    ## @param[in]: comments      comments describing icd exchange
    def createIcdExchange(self, id, linkType, enumIndex, category, iter, writeElement, readElements,\
                          pui, subsys, varType, comments):

        ## Create a default Ptcs icd job.
        ptcsIcdElement = self.mParser.newElement("icd", None, {"isReadyForIcd":'true'})
        self.mParser.newElement("rate", ptcsIcdElement).text = self.mIcdSettings.mPtcsRate
        self.mParser.newElement("simObject", ptcsIcdElement).text = self.mIcdSettings.mPtcsSimObject
        self.mParser.newElement("simVarName", ptcsIcdElement).text = "[not set]"

        ## If no writeElement passed, then Ptcs does the writing.
        if None == writeElement:
            ## Substitute the simVarName for Ptcs writes.
            ptcsIcdElement.find("simVarName").text = self.mNetwork + self.mIcdSettings.mSvnWrite[linkType] % id
            writeElement = ptcsIcdElement

        ## If no readElements passed, then Ptcs does the reading.
        elif None == readElements:
            ## Substitute the simVarName for Ptcs reads.
            simVarName = self.mNetwork + self.mIcdSettings.mSvnRead[linkType].replace("%i", iter)
            ptcsIcdElement.find("simVarName").text = simVarName  % id
            readElements = [ptcsIcdElement]

        else: # We have a problem.
            raise ThermError("At least one element (read or write) must be None. (%s)" % id)

        ## Instantiate newIcdExchange
        newIcdExchange = IcdExchange()

        ## Set units for Exchange.
        units = self.mIcdSettings.mUnitsMap[category]
        varName = id + "_" + self.getVarNameSuffix(category, iter)

        ## Initialize IcdExchange object with tailored data.
        newIcdExchange.initialize(id, enumIndex, varName, writeElement, readElements,\
                                                pui, subsys, varType, units, comments)
        ## Append to list of jobs.
        self.mListOfExchanges.append(newIcdExchange)

    #===============================================================================================
    ## @brief:
    ## Public function, called externally by ThermAspectBuilder object. Prints list of IcdExchanges
    ## to given ICD and TrickView files.
    ## @param[in]:  f_icd     pointer to opened ICD .txt-file
    ## @param[in]:  f_tv      pointer to opened TrickView .tv-file
    def writeToIcdAndTvFiles(self, f_icd, f_tv):

        ## Write each icd.
        f_icd.write("\n# %s\n" % self.mNetwork)

        ## Write trickView XML header.
        f_tv.write(ThermHeaders().tv)

        for exchange in self.mListOfExchanges:

            ## Create temporary actions list.
            actions = [exchange.mWriteAction] + exchange.mReadActions

            ## Loop through the actions list.
            for action in actions:
                ## If PTCS is reading from another ICD, the write action will be None.
                if None == action:
                    continue
                ## Print the action to the ICD file.
                f_icd.write(action.mCommentChar)
                f_icd.write(exchange.mPui + '\t')                     # pui number or 'xxxx'
                f_icd.write(self.mIcdSettings.mSys + '\t')            # "iss", etc.
                f_icd.write(self.mIcdSettings.mBus + '\t')            # "sim_bus"
                f_icd.write(exchange.mSubsys + '\t')                  # "ptcs"
                f_icd.write(exchange.mVarName + '\t')
                f_icd.write(action.mAction + '\t')                    # "WRITE" or "READ"
                f_icd.write(self.mIcdSettings.mTrickSeq + action.mRate + '\t')  # "P_PTCS" and RATE from registry
                f_icd.write(action.mSimObject + '\t')                 # Trick SimObject and Sim names
                f_icd.write(action.mSimVarName + '\t')                # from variable server (TrickView)
                f_icd.write(exchange.mType + '\t')                    # "double"
                f_icd.write('(' + exchange.mUnits + ')' + '\t')       # "(K)" for kelvin or "(W)" for Watts
                f_icd.write(exchange.mComments + '\n')                # description from registry and \n

                ## Don't print to the TrickView file if commented out of the ICD.
                if "#" == action.mCommentChar:
                    continue
                ## Store the TV name as the simVarName with a number instead of an id.
                tv_name = action.mSimVarName.replace(exchange.mEnumId, str(exchange.mEnumIndex))
                ## The sim name is the second half of the simObject.
                tv_sim = action.mSimObject.rsplit(' ')[1]

                ## Write to file.
                f_tv.write('    <variable>\n')
                f_tv.write('        <name>%s.%s</name>\n' % (tv_sim,tv_name))
                f_tv.write('        <tvDouble>\n')
                f_tv.write('            <value>0</value>\n')
                f_tv.write('            <format>Decimal</format>\n')
                f_tv.write('        </tvDouble>\n')
                f_tv.write('    </variable>\n')

        ## Close tv file.
        f_tv.write("</trickView>\n")
        f_tv.close()
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called by createIcdExchange() function. Defines a suffix to add to a varName.
    ## If the category is a 'heatFlux' job, the iteration number is appended.
    ## @param[in]: category      string describing the icd job  ex: <heatFlux> or <temperature>
    ## @param[in]: iter          string, the number of the iteration currently being processed
    ##                               ex: iteration '3' of a capacitor with five <temperature> reads
    ## @return:    suffix        string to append to the icd variable name
    def getVarNameSuffix(self, category, iter):

        suffix = category
        if 'heatFlux' == category:
            suffix = category + iter
        return suffix
