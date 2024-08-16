####################################################################################################
## @copyright Copyright 2019 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
from XmlEntryAnalyzing import XmlEntryAnalyzer
from XmlParsing import XmlParser, TagNotFound
from ThermSupport import ThermError

#===================================================================================================
# ThermRegistryEntryAnalyzer
#===================================================================================================
## @brief:
## This class contains members used in storing and analyzing xml-data from an entry in the Thermal
## Aspect Registry file. The class is passed an xml element from construction, and from
## this is gathered data about the link. Its functions are inherited from the parent class. They
## are called externally by IndivNetworkBuilder in validating the user-defined xml-data.
class ThermRegistryEntryAnalyzer(XmlEntryAnalyzer):
    ## @brief:
    ## Default constructs the object with default, uninitialized members.
    def __init__(self):
        uninitialized = "[not initialized]"

        ## Base class constructor.
        XmlEntryAnalyzer.__init__(self)

        ## Initialization flag.
        self.mIsInitialized = False

        ## The initial temperature of the node.
        self.mInitTemp = uninitialized
        ## The mass of the capacitor attached to this node.
        self.mMass = uninitialized
        ## Value of specific heat (J/K/kg) to assume if a mass is not provided.
        self.mAssumedCp = uninitialized
        ## The capacitance of the capacitor attached to this node.
        self.mCapacitance = uninitialized

        ## A map of potentials attached to this node. (potName, conductivity [W/K], element)
        self.mPotentialList = []
        ## A list of data for conductors attached to this node. (toNode, conductivity [W/K])
        self.mConductionList = []
        ## A map of conductors attached to this node. (radName, toNode, coefficient [m2], element)
        self.mRadiationList = []

        ## The editGroup of the entry. Will be None unless otherwise set.
        self.mEditGroup = None

    #===============================================================================================
    ## @brief:
    ## Initializes the class using two arguments.
    ## @param[in]: entry    ElementTree xml-element of data from Thermal, Htr, or Panel registry
    ## @param[in]: symMap   dictionary with the definitions of symbols used in the registries
    def initialize(self, entry, symMap={}, assumedCp=896.0):

        ## Base class initializer
        XmlEntryAnalyzer.initialize(self, entry, symMap)

        ## Value of specific heat (J/K/kg) to assume if a mass is not provided.
        self.mAssumedCp = assumedCp

        ## Initialize properties specific to ThermRegistry entries.
        self.loadInitTemp()
        self.loadNode()
        self.loadPotential()
        self.loadConduction()
        self.loadRadiation()

        ## Raise the flag only if we've made it this far.
        self.mIsInitialized = True

    #-----------------------------------------------------------------------------------------------
    ## Reads and sets initial temperature.
    def loadInitTemp(self):
        ## The initial value of the link, in this case a temperature value in K. Check if tag
        ## <initTemp> is given; if not, set it to a default.
        initTemp = self.getUnrequiredData(self.mEntry, "initTemp")
        if None == initTemp:
            initTemp = -1
        self.mInitTemp = initTemp

    #-----------------------------------------------------------------------------------------------
    ## Reads <node> element and sets relevant members.
    def loadNode(self):
        ## Get capacitor data
        capacitors = self.mParser.getElements(self.mEntry, "capacitor")

        ## Make sure only zero or one <capacitor> is given.
        if 0 == len(capacitors):
            self.mMass = 0
            self.mCapacitance = 0
            return
        elif len(capacitors) > 1:
            raise ThermError("Can only have one <capacitor> per node.")

        ## Read capacitor's editGroup if found. Otherwise, leave it as None.
        try:
            self.mEditGroup = self.mParser.getChildText(capacitors[0], "editGroup", self.mName)
        except TagNotFound:
            pass

        ## Make sure the cap value, defined with symbols, can be properly evaluated.
        capExpr = self.mParser.getChildText(capacitors[0], "capacitance", self.mName)
        capacitance = self.getValidatedValue(capExpr, self.mName)
        ## Default the mass to an estimation based on an assumed specific heat.
        mass = capacitance / self.mAssumedCp

        ## The capacitance expression is typically just a previously defined symbol.
        ## If so, and its properly prefixed with "cap_", then a "mass_" symbol will also exist.
        if "cap_" in capExpr:
            try:
                massSym = capExpr.replace("cap_", "mass_")
                mass = self.getValidatedValue(massSym, self.mName)

            ## If there's an error, like a negative mass, just use the previous estimation.
            except ThermError:
                pass

        ## Set the entry's mass and capacitance.
        self.mMass = mass
        self.mCapacitance = capacitance

    #-----------------------------------------------------------------------------------------------
    ## @brief:
    ## Reads <potential> element and sets relevant members.
    def loadPotential(self):
        ## Get potential data
        potentials = self.mParser.getElements(self.mEntry, "potential")

        ## Set name.
        potName = 'pot_' + self.mName

        ## Make sure only zero or one <potential> is given.
        if 0 == len(potentials):
            return
        elif len(potentials) > 1:
            raise ThermError("Can only have one <potential> per node.")

        ## The conductivity of the link, in this case a value in W/K. Check if tag
        ## <conductivity> is given; if not, set it to a default.
        conductivity = self.getUnrequiredData(potentials[0], "conductivity")
        if None == conductivity:
            conductivity = 1e12

        ## Define the potential using a map that provides:
        ## {potName : (conductivity, element)}
        self.mPotentialList.append( (potName, conductivity, potentials[0]) )

    #-----------------------------------------------------------------------------------------------
    ## Reads <conduction> element and sets relevant members.
    def loadConduction(self):

        ## Loop through all <cond> tags.
        for condElement in self.mParser.getElements(self.mEntry, "conduction"):
            try:
                ## Make sure the cond value, defined with symbols, can be properly evaluated.
                conductivity = self.mParser.getChildText(condElement, "conductivity", self.mName)
                conductivity = self.getValidatedValue(conductivity, self.mName)

                ## The conduction will be skipped unless exactly one <to> tag is found.
                toNodeElems = self.mParser.getElements(condElement, "to", True, self.mName)
                if 1 == len(toNodeElems):
                    toNode = self.mParser.getText(toNodeElems[0])
                else:
                    raise ThermError("Must provide exactly one <to> node.")

                ## Define the conductor using a map that provides {toNode : conductivity}
                self.mConductionList.append( (toNode, conductivity) )

            ## Do not write data if <to> tag not found or node not registered.
            except (ThermError, TagNotFound), e:
                print e,
                print "Conduction link will be skipped (%s)." % self.mName

    #-----------------------------------------------------------------------------------------------
    ## Reads <radiation> element and sets relevant members.
    def loadRadiation(self):

        ## Loop through all <radiation> tags.
        radElements = self.mParser.getElements(self.mEntry, "radiation")
        for i in range(0, len(radElements) ):

            ## Set name.
            radName = "rad%i_%s" % (i, self.mName)

            try:
                ## Make sure the coeff value, defined with symbols, can be properly evaluated.
                coeff = self.mParser.getChildText(radElements[i], "coefficient", radName)

            ## If no <coefficient> is found, we will default to the TD value. Store a 0.0 for now.
            except TagNotFound, e:
                coeff = "0.0"

            try:
                ## Verify coefficient value found above.
                coeff = self.getValidatedValue(coeff, radName)

                ## The radiation will be skipped unless exactly one <to> tag is found.
                toNodeElems = self.mParser.getElements(radElements[i], "to", True, self.mName)
                if 1 == len(toNodeElems):
                    toNode = self.mParser.getText(toNodeElems[0])
                else:
                    raise ThermError("Must provide exactly one <to> node.")

                ## Define the radiation link using a map that provides:
                ## {radName : (toNode, coefficient, element)}
                self.mRadiationList.append( (radName, toNode, coeff, radElements[i]) )

            ## Do not write data if <to> tag not found or node not registered.
            except (ThermError, TagNotFound), e:
                print e, "Radiation link \"%s\" will be skipped (%s)." % (radName, self.mName)
                continue
