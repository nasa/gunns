####################################################################################################
## @copyright Copyright 2019 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
from ThermSupport import ThermError, ThermPrinter
from IndivNetworkConfiguring import IndivNetworkConfig
from XmlParsing import XmlParser, TagNotFound
from ThermRegistryEntryAnalyzing import ThermRegistryEntryAnalyzer
from SourceEntryAnalyzing import SourceEntryAnalyzer
from IcdBuilding import IcdBuilder

#===================================================================================================
# IndivNetworkBuilder
#===================================================================================================
## @brief:
## This class is the meat of the ThermAspectGenerate process. It manages all the data associated
## with a PTCS thermal network. A ThermAspectBuilder initializes the IndivNetworkBuilder with paths
## to all the files it needs to generate the bevy of Thermal config-files. During execute(),
## The IndivNetworkBuilder parses a TdNetworkConfig file, and then a ThermRegistry file,  both
## specific to an individual thermal network. It merges the data contained in these two files, and
## generates node, conduction, radiation, and potential data. Then, after parsing the network's
## heater and panel files, it builds a list of jobs to print on the PTCS icd.
class IndivNetworkBuilder():
    ## @brief:
    ## Default constructs the object with default, uninitialized members.
    def __init__(self):

        ## Initialization flag.
        self.mInitialized = False
        uninitialized = "[%s not initialized]"

        ## Name (abbreviation) of network to be built. Must be identical to string found
        ## in network-specific file names (e.g., 'trussMid' -> ThermNodes_trussMid.xml).
        self.mNetwork = uninitialized % "mNetwork"

        ## Name of top-level script that creates the ThermAspectBuilder that creates this object.
        self.mCallingScript = uninitialized % "mCallingScript"

        ## A dictionary with the definitions of symbols used in the registries
        self.mSymMap = uninitialized % "mSymMap"

        ## Assumption to use for any specific heat values (Cp) not provided directly.
        self.mAssumedCp = uninitialized % "mAssumedCp"

        ## An XmlParser() object for getting data from xml elements.
        self.mParser = XmlParser()
        ## A ThermPrinter() object for some printing utility functions.
        self.mPrinter = ThermPrinter()
        ## A dictionary used to maintain a record of Thermal Desktop nodes that were also
        ## included in the Thermal Aspect Registry.
        self.isRegistered = {}

        ## ICD builder object. Handles all ICD functionality.
        self.mIcdBuilder = IcdBuilder()

        #Files to read ............................................
        ## Path and file name of Thermal Aspect Registry file.
        self.mRegisFile = uninitialized % "mRegisFile"
        ## Path and file name of Thermal Desktop file (TdNetworkConfig).
        self.mTdFile = uninitialized % "mTdFile"
        ## Path and file name of HtrRegistry file.
        self.mHtrFile = uninitialized % "mHtrFile"
        ## Path and file name of ThermalPanel registry/config file.
        self.mPanFile = uninitialized % "mPanFile"

        #Files to generate ........................................
        ## Path and file name of Node configuration file.
        self.mNodeFile = uninitialized % "mNodeFile"
        ## Path and file name of Conduction configuration file.
        self.mCondFile = uninitialized % "mCondFile"
        ## Path and file name of Radiation configuration file.
        self.mRadFile = uninitialized % "mRadFile"
        ## Path and file name of Et.Cetera (potentials, sources) configuration file.
        self.mEtcFile = uninitialized % "mEtcFile"
        ## Path and file name of TrickView file. Each network produces its own TrickView file.
        self.mTvFile = uninitialized % "mTvFile"

        ## Initialize lists for enumerating.
        self.tdNodeList = []
        self.tdRadList = []
        self.regisNodeList = []
        self.masterNodeList = []
        self.mMassDict = {}
        self.mTempDict = {}
        self.mEditCapGroupList = []
        self.mPotList = []
        self.mSrcList = []
        self.mRadList = []
        self.mHtrList = []
        self.mPanList = []
        self.mCondNum = 0

        ## Boolean to determine if the mass of structural nodes should be adjusted to match a
        ## total-mass expected value.
        self.mIsMassAdjustable = uninitialized % "mIsMassAdjustable"
        ## Expected/Rated mass (kg) of the thermal network
        self.mExpectedMass = uninitialized % "mExpectedMass"
        ## Raw, unadjusted mass (kg) of the thermal network
        self.mUnadjustedMass = 0
        ## Scalar to multiply all structural node mass with the aim of matching the network's
        ## modeled mass to the rated mass.
        self.mStructMassScalar = 0

        ## Initialize new PTCS XML elements. These elements will be built up during the course of
        ## the network parsing/analysis. At the conclusion, when the xml trees are complete, they
        ## will be printed to generated thermal config-files.
        self.mNodeXml = self.mParser.newElement("list")
        self.mCondXml = self.mParser.newElement("list")
        self.mRadXml = self.mParser.newElement("list")
        self.mEtcXml = self.mParser.newElement("list")
    #===============================================================================================
    ## @brief:
    ## Public function, called by ThermAspectBuilder class. Initializes the object with its
    ## configuration data.
    ## @param[in]: config   IndivNetworkConfig object with its file names set explicitly by
    ##                      the top-level orchestrator.
    def initialize(self, config):

        ## Name (abbreviation) of network to be built. Must be identical to string found
        ## in network-specific file names (e.g., 'trussMid' -> ThermNodes_trussMid.xml).
        self.mNetwork = config.cNetwork

        ## Name of top-level script that creates the ThermAspectBuilder that creates this object.
        self.mCallingScript = config.cCallingScript

        ## A dictionary with the definitions of symbols used in the registries
        self.mSymMap = config.cSymMap

        ## Assumption to use for any specific heat values (Cp) not provided directly.
        self.mAssumedCp = config.cAssumedCp

        ## Boolean to determine if the mass of structural nodes should be adjusted to match a
        ## total-mass expected value.
        self.mIsMassAdjustable = config.cIsMassAdjustable

        ## Expected/Rated mass (kg) of the thermal network
        self.mExpectedMass = None

        ## Initialize file paths.
        self.mRegisFile = config.cRegisFile
        self.mTdFile = config.cTdFile
        self.mNodeFile = config.cNodeFile
        self.mCondFile = config.cCondFile
        self.mRadFile = config.cRadFile
        self.mPanFile = config.cPanFile
        self.mHtrFile = config.cHtrFile
        self.mEtcFile = config.cEtcFile
        self.mTvFile = config.cTvFile

        ## Initialize IcdBuilder object with basic ICD data.
        self.mIcdBuilder.initialize(self.mNetwork, config.cIcdSettings)

        ## Set initialization flag.
        self.mInitialized = True
    #===============================================================================================
    ## @brief:
    ## Public function, called by ThermAspectBuilder class. Reads Thermal Desktop data in
    ## TdNetworkConfig file. Reads user-generated data in Thermal Aspect Registry, Heater Registry,
    ## and ThermalPanel config file. Prints node, conduction, radiation, and potential data to
    ## xml files used directly in the sim.
    def execute(self):

        if False == self.mInitialized:
            raise ThermError("IndivNetworkBuilder (%s) not initialized." % self.mNetwork)

        try:
            ## Read the TdNetworkConfig file.
            tdData = self.readThermalDesktopData()

            ## Parse and analyze ThermRegistry.
            self.readThermRegistry()

            ## Perform Mass Analysis.
            self.performMassAnalysis()

            ## Append ThermalDesktop data to the end of the xml config-files.
            self.appendThermalDesktopData(tdData)

            ## Heaters and panels are both derived from GunnsThermalSource, and they both have very
            ## similar registry schemas. They can be parsed with the same function.
            self.readThermSourceFile("heater", self.mHtrFile, self.mHtrList)
            self.readThermSourceFile("panel", self.mPanFile, self.mPanList)
            self.readThermSourceFile("source", self.mHtrFile, self.mSrcList)

            ## Write XML trees to file.
            self.mPrinter.printThermXml(self.mNodeXml, self.mNodeFile, self.mCallingScript)
            self.mPrinter.printThermXml(self.mCondXml, self.mCondFile, self.mCallingScript)
            self.mPrinter.printThermXml(self.mRadXml, self.mRadFile, self.mCallingScript)
            self.mPrinter.printThermXml(self.mEtcXml, self.mEtcFile, self.mCallingScript)

        except ThermError, e:
            print e,
            raise ThermError("Error executing network (%s)." % self.mNetwork)
    #===============================================================================================
    ## @brief:
    ## Public function, called externally by ThermAspectBuilder object. Prints list of enumerations
    ## to given enum-file.
    ## @param[in]:  f_enum    pointer to opened enumeration file
    def writeToEnumFile(self, f_enum):
        ## Write each enumeration.
        f_enum.write("/// ==============================(%s)====================\n" % self.mNetwork)
        self.mPrinter.printEnum(f_enum, self.regisNodeList, "capacitanceLinks_" + self.mNetwork)
        self.mPrinter.printEnum(f_enum, self.mPotList, "potentialLinks_" + self.mNetwork)
        self.mPrinter.printEnum(f_enum, self.mSrcList, "sourceLinks_" + self.mNetwork)
        self.mPrinter.printEnum(f_enum, self.mHtrList, "heaterLinks_" + self.mNetwork)
        self.mPrinter.printEnum(f_enum, self.mRadList, "radiationLinks_" + self.mNetwork)
        self.mPrinter.printEnum(f_enum, self.mPanList, "panelLinks_" + self.mNetwork)
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in execute(). Reads the individual network's Thermal Desktop data,
    ## contained in the TdNetworkConfig file.
    ## @return: tdData   xml root element containing the node/cond/rad data from Thermal Desktop
    def readThermalDesktopData(self):

        if None == self.mTdFile:
            return None

        ## Load file and save root data.
        tdData = self.mParser.loadFile(self.mTdFile)

        ## Find node elements.
        tdNodeElements = self.mParser.getElements(tdData, "node", True, self.mNetwork)

        ## Loop through each TD entry, saving the node name and mass estimate.
        massList = []
        tempList = []
        for element in tdNodeElements:
            ## Get name and capacitance data.
            name = self.mParser.getChildText(element,"name", self.mNetwork)
            capElem = self.mParser.getElements(element,"capacitance")[0]
            temp = self.mParser.getChildText(element,"temperature", self.mNetwork)
            cap = self.mParser.getText(capElem, name)
            mass = float(cap)/self.mAssumedCp

            ## If there exists a symbol that explicitly defines the node's capacitance
            ## (cap_N2_SHIELD_23, for example), then we will overwrite the Thermal Desktop
            ## capacitance with this symbolic value.
            if ("cap_" + name) in self.mSymMap:
                ## Get the given explicit value.
                cap = self.mSymMap["cap_" + name]
                ## The mass will also be explicitly defined.
                mass = self.mSymMap["mass_" + name]
                ## Overwrite.
                capElem.text = self.mParser.roundValue(cap, 2)

            ## Append.
            self.tdNodeList.append(name)
            massList.append(mass)
            tempList.append(temp)

        ## For now, the masterNodeList is equal to the tdNodeList. Later, we will include nodes
        ## from the Thermal Aspect Registry.
        self.masterNodeList = self.tdNodeList

        ## Create a dictionary based on node names and a False. The value will be
        ## overwritten to True if the node is repeated in the Registry.
        self.isRegistered = dict( zip(self.tdNodeList, [False] * len(self.tdNodeList) ))

        ## Create dictionaries for mass, temp values.
        self.mMassDict = dict( zip(self.tdNodeList, massList))
        self.mTempDict = dict( zip(self.tdNodeList, tempList))

        return tdData
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in execute(). Reads the individual network's Thermal Aspect Registry.
    ## Analyzes the data in each entry, and appends it to lists that store the different link data
    ## in xml-form. Also gets and registers relevant icd information.
    def readThermRegistry(self):

        ## Skip the method if the ThermRegistry file is None.
        if None == self.mRegisFile:
            return

        ## Try to open the ThermRegistry file. Read and save its xml data.
        regisData = self.mParser.loadFile(self.mRegisFile)

        ## Read the expected total weight of the thermal network.
        exMassElem = self.mParser.getElements(regisData, "expectedMass", False)
        self.determineExpectedMass(exMassElem)

        ## Build a list of capEdit groups.
        capEditing = self.mParser.getElements(regisData, "capEditing", False)
        for capEdit in capEditing:
            groups = self.mParser.getElements(capEdit, "editGroup", False)
            for group in groups:
                self.mEditCapGroupList.append(self.mParser.getText(group))

        ## Get node entries from the xml.
        entries = self.mParser.getElements(regisData, "node", True, self.mNetwork)

        ## Instantiate EntryObject list.
        entryObjList = []

        ## We must do two loops. All the nodes must be registered first, so that other entries
        ## can check the existence of their <to> nodes.
        for entry in entries:
            try:
                ## We don't need to add the link to the ThermalNetwork if it's commented out.
                if 'true' == entry.get("commentOut"):
                    continue

                ## Create and initialize ThermRegistryAnalyzer() object.
                entryObj = ThermRegistryEntryAnalyzer()
                entryObj.initialize(entry, self.mSymMap, self.mAssumedCp)

                ## Register <node> / <capacitor> in network.
                self.registerNode(entryObj)

                ## Register <potential> in network.
                self.registerPot(entryObj)

                ## Append to list of entry objects.
                entryObjList.append(entryObj)

            except (ThermError, TagNotFound), e:
                print e, "Node not registered (%s)." % entryObj.mName

        ## We need the second loop so that a node in the beginning of a registry can refer to a node
        ## that isn't defined until farther down in the registry.
        for entryObj in entryObjList:
            ## Register <conduction> in network.
            self.registerCond(entryObj)

            ## Register <radiation> in network.
            self.registerRad(entryObj)

        ## Capacitance Editing groups......................................
        ## Develop an element of capEdit group names.
        groups = self.mParser.newElement("capEditing")
        for editGroup in self.mEditCapGroupList:
            self.mParser.newElement("group", groups).text = editGroup

        ## Insert the <capEditing> element into the NodeXml, only if needed.
        if len(self.mEditCapGroupList) > 0:
            self.mNodeXml.insert(0, groups)
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in execute(). Reads a individual network's heater registry or panel
    ## file.  Analyzes the data in each entry, and appends it to lists that store the different
    ## link data in xml-form. Also gets and registers relevant icd information. Used generically for
    ## heaters and panels since they both derive from GunnsThermalSource, and they both have very
    ## similar registry schemas.
    ## @param[in]: theType   "heater" or "panel"
    ## @param[in]: theFile   file name and path to HtrRegistry or panel config-file
    ## @param[in]: srcList   list object containing heater names or panel names
    def readThermSourceFile(self, theType, theFile, srcList):

        ## Skip the method if the ThermRegistry file is None.
        if None == theFile:
            return

        ## Try to open the source file.
        srcData = self.mParser.loadFile(theFile)

        ## Open it for reading and save its xml data.
        entries = self.mParser.getElements(srcData, theType)

        ## Loop through each source entry.
        for entry in entries:
            try:
                ## Skip the source if it's commented out.
                if 'true' == entry.get("commentOut"):
                    continue

                ## Create and initialize SourceEntryAnalyzer object.
                entryObj = SourceEntryAnalyzer()
                entryObj.initialize(theType, entry)

                ## Register source link in network.
                self.registerSrc(entryObj, theType, srcList)

            ## End of operation for individual source.
            except (ThermError, TagNotFound), e:
                print e, "%s link will be skipped (%s)." % (theType.capitalize(), entryObj.mName)
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in readThermRegistry(). Gets the data contained in a ThermRegistry
    ## entry, and organizes the data into an xml-subelement. Maintains list of node names for
    ## validation and enumeration.
    ## @param[in]: entryObj   ThermRegistryEntryAnalyzer object w/ data from ThermRegistry
    def registerNode(self, entryObj):

        ## Check for repeated node name.
        if entryObj.mName in self.regisNodeList:
            raise ThermError("Node previously defined in ThermRegistry.")

        ## Raise error if editGroup not defined in the header at the top of the registry.
        if None != entryObj.mEditGroup and entryObj.mEditGroup not in self.mEditCapGroupList:
            raise ThermError("editGroup not defined in <capEditing> (%s)." % entryObj.mEditGroup)

        ## Append or Overwrite node's mass in the dictionary.
        self.mMassDict[entryObj.mName] = entryObj.mMass

        ## Find the enumIndex of this node.
        enumIndex = len(self.regisNodeList)

        ## If no initial temperature given, default to either the value from Thermal Desktop or just
        ## a neutral value.
        if -1 == entryObj.mInitTemp:
            try:
                entryObj.mInitTemp = self.mTempDict[entryObj.mName]
            except:
                entryObj.mInitTemp = 296.0

        ## Create <node> element.
        n = self.mParser.newElement("node", self.mNodeXml)
        self.mParser.newElement("enum", n).text = str(enumIndex)
        self.mParser.newElement("name", n).text = entryObj.mName
        self.mParser.newElement("mass", n, {"units": "kg"}).text = \
                                self.mParser.roundValue(entryObj.mMass, 2)
        self.mParser.newElement("temperature", n, {"units": "K"}).text = \
                                self.mParser.roundValue(entryObj.mInitTemp, 2)
        self.mParser.newElement("capacitance", n, {"units": "J/K"}).text = \
                                self.mParser.roundValue(entryObj.mCapacitance, 2)

        ## Create an <editGroup> subelement if applicable.
        if None != entryObj.mEditGroup:
            ## Create element for editGroup.
            self.mParser.newElement("editGroup", n).text = entryObj.mEditGroup

        ## Add node to nodeLists for enumeration.
        self.regisNodeList.append(entryObj.mName)
        ## Check if the registry node has a duplicate in ThermalDesktop.
        if entryObj.mName in self.tdNodeList:
            self.isRegistered[entryObj.mName] = True
        else:
            self.masterNodeList.append(entryObj.mName)

        ## Process ICD jobs contained in the entryObj.
        self.mIcdBuilder.extractAndProcessIcd(entryObj, "capacitor", enumIndex)
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in readThermRegistry(). Gets the data contained in a ThermRegistry
    ## entry, and organizes the data into an xml-subelement. Maintains list of potential link names
    ## for enumeration.
    ## @param[in]: entryObj   ThermRegistryEntryAnalyzer object w/ data from ThermRegistry
    def registerPot(self, entryObj):

        ## Loop through all potential links in the radiation map.
        for (potName, conductivity, element) in entryObj.mPotentialList:
            try:
                ## Create <potential> element.
                p = self.mParser.newElement("potential", self.mEtcXml)
                self.mParser.newElement("name", p).text = potName
                self.mParser.newElement("node", p).text = entryObj.mName
                self.mParser.newElement("temperature", p, {"units": "K"}).text = \
                                        self.mParser.roundValue(entryObj.mInitTemp,2)
                self.mParser.newElement("conductivity", p, {"units": "W/K"}).text = \
                                        self.mParser.roundValue(conductivity,6)

                ## Find the enumIndex of this potential.
                enumIndex = len(self.mPotList)

                ## Process ICD jobs contained in the entryObj.
                self.mIcdBuilder.extractAndProcessIcd(entryObj, "potential", enumIndex)

                ## Add potential to mPotList for enumeration.
                self.mPotList.append(potName)

            except (ThermError, TagNotFound), e:
                print e,
                raise ThermError("Error in <potential>.")
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in readThermRegistry(). Gets the data contained in a ThermRegistry
    ## entry, and organizes the data into an xml-subelement.
    ## @param[in]: entryObj   ThermRegistryEntryAnalyzer object w/ data from ThermRegistry
    def registerCond(self, entryObj):
        ## Loop through all conductors in the conduction map.
        for (toNode, conductivity) in entryObj.mConductionList:

            try:
                ## Check that the <to> nodes are valid.
                self.validateNode(toNode, "to")

                ## Create <conduction> element.
                c = self.mParser.newElement("conduction", self.mCondXml)
                self.mParser.newElement("enum", c).text = str(self.mCondNum)
                self.mParser.newElement("node0", c).text = entryObj.mName
                self.mParser.newElement("node1", c).text = toNode
                self.mParser.newElement("conductivity", c, {"units": "W/K"}).text = \
                                        self.mParser.roundValue(conductivity,6)
                self.mCondNum = self.mCondNum+1

            ## Do not write data if <to> tag not found or node not registered.
            except ThermError, e:
                print e, "Conduction link will be skipped (for entry %s)." % entryObj.mName
                continue
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in readThermRegistry(). Gets the data contained in a ThermRegistry
    ## entry, and organizes the data into an xml-subelement. Maintains list of radiation link names
    ## for enumeration. Creates IcdExchange object for relevant <viewScalar> elements.
    ## @param[in]: entryObj   ThermRegistryEntryAnalyzer object w/ data from ThermRegistry
    def registerRad(self, entryObj):
        ## Loop through all radiation links in the radiation map.
        for (radName, toNode, coeff, element) in entryObj.mRadiationList:
            try:
                ## Verify not previously added.
                if radName in self.mRadList:
                    raise ThermError("Radiation link previously defined.")

                ## Check that the <to> nodes are valid.
                self.validateNode(toNode, "to")

                ## Create <radiation> element.
                r = self.mParser.newElement("radiation", self.mRadXml)
                self.mParser.newElement("node0", r).text = entryObj.mName
                self.mParser.newElement("node1", r) .text = toNode
                self.mParser.newElement("coefficient", r, {"units": "m2"}).text = self.mParser.roundValue(coeff,6)

                ## Find the enumIndex of this radiation link.
                enumIndex = len(self.mRadList)

                ## Add radName to radList for enumeration.
                self.mRadList.append(radName)

                ## Build Icd jobs.
                self.mIcdBuilder.processIcd(element, radName, "radiation", enumIndex, entryObj.mDescription)

            ## Do not write data if <to> tag not found or node not registered.
            except (ThermError, TagNotFound), e:
                print e, "Radiation link \"%s\" will be skipped (for entry %s)." % (radName, entryObj.mName)
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in readThermSourceFile(). Gets the data contained in a HtrRegistry or
    ## panel config-file entry, and organizes the data into an xml-subelement. Maintains list of
    ## link names for enumeration. Creates IcdExchange object for relevant icd elements.
    ## @param[in]: entryObj   SourceEntryAnalyzer object w/ data from HtrRegistry or panel config-file
    ## @param[in]: theType    "heater" or "panel" or "source"
    ## @param[in]: srcList    list object containing heater names or panel names
    def registerSrc(self, entryObj, theType, srcList):

        ## Verify not previously added.
        if entryObj.mName in srcList:
            raise ThermError("Link previously defined.")

        ## Find the enumIndex of this source.
        enumIndex = len(srcList)

        ## Check that the <to> nodes are valid.
        for node in entryObj.mNodeList:
            self.validateNode(node, "node")

        ## Append source to list of successfully-build sources.
        srcList.append(entryObj.mName)

        ## Build Icd jobs.
        self.mIcdBuilder.processIcd(entryObj.mEntry, entryObj.mName, theType, enumIndex, entryObj.mDescription)
    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in execute(). Appends ThermalDesktop data, in xml-format, to the end
    ## of the xml-tree created after reading the Thermal Aspect Registry. This function is called
    ## after ThermRegistry parsing so that the ThermRegistry data, which is user-defined, is
    ## always first.
    ## @param[in]: tdData   xml-tree of Thermal Desktop data from TdNetworkConfig file
    def appendThermalDesktopData(self, tdData):

        ## Initialize counter at length of registry node list.
        counter = len(self.regisNodeList)

        if None == tdData:
            ## Create <node> element for SPACE.
            n = self.mParser.newElement("node", self.mNodeXml)
            self.mParser.newElement("name", n).text = "SPACE_1"
            self.mParser.newElement("temperature", n, {"units": "K"}).text = "2.73"
            self.mParser.newElement("capacitance", n, {"units": "J/K"}).text = "0.00"
            return

        ## Loop through all TD nodes again with the goal of including non-registry nodes in the
        ## xml output.
        for tdNodeElement in self.mParser.getElements(tdData,"node", True, self.mNetwork):
            ## Get node name.
            node = self.mParser.getChildText(tdNodeElement,"name", self.mNetwork)

            ## Node info from the registry takes priority. But if the node is not in the registry,
            ## we will use its Thermal Desktop data.
            if False == self.isRegistered[node]:
                ## Insert enum element with the index of the node within the overall array.
                e = self.mParser.newElement("enum")
                e.text = str(counter)
                tdNodeElement.insert(0, e)

                ## Create a mass element and set value to that from the dictionary.
                m = self.mParser.newElement("mass", None, {"units": "kg"})
                m.text = self.mParser.roundValue(self.mMassDict[node], 2)

                ## If this is a STRUCT node, we want to adjust its mass so that the network's
                ## total mass matches the rated mass given in the registry.
                if self.mIsMassAdjustable and "STRUCT" in node:
                    ## Scale the mass and capacitance of the node to an adjusted value.
                    adjustedMass = self.mMassDict[node] * (1 + self.mStructMassScalar)

                    ## Make sure we don't cause the mass to be negative.
                    if 0 > adjustedMass:
                        print "Mass adjustment causes %s to go negative (%f)" % (node, adjustedMass)
                        print "No longer adjusting mass for this network."
                        self.mIsMassAdjustable = False

                    else:
                        ## Get capacitance data.
                        capElement = self.mParser.getElements(tdNodeElement,"capacitance")[0]
                        origCap = float(self.mParser.getText(capElement))

                        ## Scale the capacitance accordingly and overwrite.
                        capElement.text = self.mParser.roundValue(adjustedMass/self.mMassDict[node] * origCap, 2)
                        ## Overwrite the adjusted mass.
                        m.text = self.mParser.roundValue(adjustedMass, 2)

                ## Insert mass element.
                tdNodeElement.insert(2, m)

                ## Append and increment.
                self.mNodeXml.append(tdNodeElement)
                counter = counter + 1

        ## Append the conduction from Thermal Desktop.
        for tdCondElement in self.mParser.getElements(tdData,"conduction"):
            e = self.mParser.newElement("enum")
            e.text = str(self.mCondNum)
            tdCondElement.insert(0, e)
            self.mCondXml.append(tdCondElement)
            self.mCondNum = self.mCondNum+1

        ## Append the radiation from Thermal Desktop, checking for duplicates.
        for tdRadElement in self.mParser.getElements(tdData,"radiation"):

            ## We want to check for radiation link duplicates.
            n0 = self.mParser.getChildText(tdRadElement, "node0");
            for mRad in self.mRadList:

                ## First check if node0 is a match.
                if n0 in mRad:
                    i = self.mRadList.index(mRad)

                    ## It is! Now check if node1 is a match.
                    n1 = self.mParser.getChildText(tdRadElement, "node1");
                    if n1 == self.mParser.getChildText(self.mRadXml[i], "node1"):

                        ## It is! We have a duplicate radiation link. If no coefficient data is given,
                        ## overwrite with the data from TD.
                        if "0.000000" == self.mParser.getChildText(self.mRadXml[i],"coefficient"):
                            tdCoeffText = self.mParser.getChildText(tdRadElement, "coefficient")
                            self.mParser.getElements(self.mRadXml[i],"coefficient")[0].text = tdCoeffText

                        ## Set to None so that we do not append this TD element to the RadXml.
                        tdRadElement = None

            ## If no duplication was discovered, append the radiation to the master rad-link XML.
            if None != tdRadElement:
                self.mRadXml.append(tdRadElement)

    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in readThermRegistry(). Reads the expectedMass element from the
    ## registry and determines the net value.
    def determineExpectedMass(self, exMassElem):
        if not exMassElem:
            return
        ## An expected mass is provided. Initialize starting mass sum to zero.
        sumMass = 0;
        try:
            ## Find all the subcomponents or "segments" given.
            segments = self.mParser.getElements(exMassElem[0], "segment", True)

            ## Read the mass given for each segment.
            for segment in segments:
                segMass = self.mParser.getText(segment)
                ## Check validity
                if segMass < 0:
                    raise ValueError
                ## Sum.
                sumMass = sumMass + float(segMass)

            ## Set the ExpectedMass if the sum was successful.
            self.mExpectedMass = sumMass

        except ValueError, e:
            print "expectedMass for segment in '%s' is invalid: %s" % (self.mNetwork, segMass)
        except ThermError, e:
            print "Error reading expectedMass of network '%s'." % self.mNetwork

    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method, called in execute(). Determines the total mass of the network from all the
    ## individual masses per node. This sum is compared against the rated mass of the network given
    ## in the registry. The difference is applied proportionately to all 'STRUCT' nodes.
    def performMassAnalysis(self):
        ## Find both the TOTAL mass of all the nodes in the model, as well as the total
        ## mass of all the structural nodes (these are where the difference in mass
        ## will be applied).
        self.mUnadjustedMass = 0;
        massStruct = 0;
        for node, mass in self.mMassDict.iteritems():
            self.mUnadjustedMass = self.mUnadjustedMass + mass
            if "STRUCT" in node:
                massStruct = massStruct + mass

        ## Create "mass" Element to the NodesXml.
        m = self.mParser.newElement("mass")
        u = self.mParser.newElement("unadjusted", m, {"units": "kg"}).text = \
                                    self.mParser.roundValue(self.mUnadjustedMass, 1)

        ## If the registry included an expected mass...
        if None != self.mExpectedMass:
            ## Append an "expectedMass" element to the mass block if necessary.
            x = self.mParser.newElement("expected", m, {"units": "kg"}).text = \
                                        self.mParser.roundValue(self.mExpectedMass, 1)

            ## The struct-mass scalar becomes the fraction of the unaccounted mass over the mass of
            ## all structural nodes.
            self.mStructMassScalar = (self.mExpectedMass - self.mUnadjustedMass) / massStruct

        ## Insert new mass element into the nodes XML.
        self.mNodeXml.insert(0, m)

    #===============================================================================================
    ## @brief:
    ## Verify that the node has been properly registered in the thermal network.
    ## @param[in]: node   node name string to validate
    ## @param[in]: info   optional string used in error reporting
    def validateNode(self, node, tagInfo=''):
        if node not in self.masterNodeList:
            warning = "Node does not exist: <%s>%s</%s>." % (tagInfo, node, tagInfo)
            raise ThermError(warning)
