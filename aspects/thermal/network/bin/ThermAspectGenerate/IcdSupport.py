####################################################################################################
## @copyright Copyright 2019 United States Government as represented by the Administrator of the
##            National Aeronautics and Space Administration.  All Rights Reserved.
##
## created: Feb 2013
##
## @author: Joe Valerioti (L-3 STRATIS) joseph.valerioti@nasa.gov
####################################################################################################
## Include all necessary classes.
from XmlParsing import XmlParser

#===================================================================================================
# IcdSettings
#===================================================================================================
## @brief:
## The IcdSettings class is a simple data structure with no functions. It contains 3 types of
## dictionaries:
## (1) defines the category (xml tag) in the ThermRegistry of the icd job corresponding to each type
##     of thermal Gunns link; ex: <viewScalar> for rad read, <temperature> for cap write
## (2) defines the units associated with each category
## (3) defines the class members of the Thermal Gunns links that write/read the icd data
class IcdSettings():
    ## @brief:
    ## Default constructs with no arguments. Members are set to PTCS-system settings.
    def __init__(self):

        self.mPtcsRate = "PTCS_RATE"
        self.mPtcsSimObject = "PtcsSimObject ptcs"

        ## Dictionary for Category WRITE jobs.
        ## Determines the category of ICD job that each PTCS link writes.
        self.mCategoryThatPtcsWrites = {}
        self.mCategoryThatPtcsWrites['capacitor'] = "temperature"
        self.mCategoryThatPtcsWrites['potential'] = "heatFlux"
        self.mCategoryThatPtcsWrites['radiation'] = None
        self.mCategoryThatPtcsWrites['panel'] = None
        self.mCategoryThatPtcsWrites['source'] = "temperature"
        self.mCategoryThatPtcsWrites['heater'] = None

        ## Dictionary for Category READ jobs
        ## Determines the category of ICD job that each PTCS link reads.
        self.mCategoryThatPtcsReads = {}
        self.mCategoryThatPtcsReads['capacitor'] = "heatFlux"
        self.mCategoryThatPtcsReads['potential'] = "temperature"
        self.mCategoryThatPtcsReads['radiation'] = "viewScalar"
        self.mCategoryThatPtcsReads['panel'] = "incidentFlux"
        self.mCategoryThatPtcsReads['source'] = "heatFlux"
        self.mCategoryThatPtcsReads['heater'] = None

        ## Units dictionary. Determines the units associated with each ICD category.
        self.mUnitsMap = {}
        self.mUnitsMap['heatFlux'] = "W"
        self.mUnitsMap['temperature'] = "K"
        self.mUnitsMap['viewScalar'] = "--"
        self.mUnitsMap['incidentFlux'] = "W/m2"

        ## simVarName Write dictionary.
        ## Determines the PTCS member variable where data being written to the bus originates.
        self.mSvnWrite = {}
        self.mSvnWrite['capacitor'] = ".therm.mCapacitanceLinks[%s].mTemperature"
        self.mSvnWrite['potential'] = ".therm.mPotentialLinks[%s].mHeatFluxIntoNode"
        self.mSvnWrite['source']    = ".therm.mSources[%s].mAvgPortTemperature"

        ## simVarName Read dictionary.
        ## Determines the destination of data from another subsystem is written onto a PTCS link.
        self.mSvnRead = {}
        self.mSvnRead['capacitor']  = ".therm.mCapacitanceLinks[%s].mExternalHeatFlux[%i]"
        self.mSvnRead['potential']  = ".therm.mPotentialLinks[%s].mSourcePotential"
        self.mSvnRead['radiation']  = ".therm.mRadiationLinks[%s].mViewScalar"
        self.mSvnRead['panel']      = ".therm.mPanels[%s].mIncidentHeatFluxPerArea"
        self.mSvnRead['source']     = ".therm.mSources[%s].mDemandedFlux"

        notSet = "[must explicitly be set]"

        ## Name of sim system, ex: "iss"
        self.mSys = notSet
        ## Name of icd bus, ex: "sim_bus"
        self.mBus = notSet
        ## Name of icd trick sequence, or P-value, including space character! ex: "P_PTCS "
        self.mTrickSeq = notSet

#===================================================================================================
# IcdAction
#===================================================================================================
## @brief:
## This class contains the data of a single READ or WRITE action on the icd. Each line on an
## icd-file is one IcdAction() instantiation.
class IcdAction():
    ## @brief:
    ## Default constructs with no arguments. Members are defaulted to uninitialized.
    def __init__(self):
        uninitialized = "[%s not initialized]"

        ##@note: All the ICD data for an action is read from an entry in a thermal registry file.

        ## Character to print at the beginning of a line on the ICD-file. It will be "" if the
        ## action is active, or "#" (commented out) if the action is not active.
        self.mCommentChar = ""
        ## String describing the type of action, either "WRITE" or "READ"
        self.mAction = uninitialized % "mAction"
        ## String describing the rate of data exchange to/from the bus, ex: "ECLSS_RATE"
        self.mRate = uninitialized % "mRate"
        ## String describing the sim object class and instantiation, ex: "MechSimObject mech"
        self.mSimObject = uninitialized % "mSimObject"
        ## String describing the member variable to/from which a bus exchanges data,
        ## ex: obj.extSwitchFirmware[0][0].evsu.nominalTemp
        self.mSimVarName = uninitialized % "mSimVarName"
    #===============================================================================================
    ## @brief:
    ## Public function. Initializes the class with the data needed to print it on the icd.
    ## @param[in]: id                      string that identifies the particular link, used
    ##                                         in the PTCS enumeration
    ## @param[in]: atLeastOneActiveEntry   boolean, True if at least one action in the exchange
    ##                                         (WRITE/READ group) is active. If False, all actions
    ##                                         in the exchange will be commented out.
    ## @param[in]: action                  simple string "READ" or "WRITE"
    ## @param[in]: icdElement              xml-element containing <rate>, <simObject>, and
    ##                                         <simVarName> children
    def initialize(self, id, atLeastOneActiveEntry, action, icdElement):

        ## Create an XmlParser.
        parser = XmlParser()

        ## Comment out if not ready.
        if 'true' != icdElement.get("isReadyForIcd") or False == atLeastOneActiveEntry:
            self.mCommentChar = "#"

        ## Initialize job data.
        self.mAction = action
        self.mRate = parser.getChildText(icdElement, "rate", id)
        self.mSimObject = parser.getChildText(icdElement, "simObject", id)
        self.mSimVarName = parser.getChildText(icdElement, "simVarName", id).replace("%s",id)

#===================================================================================================
# IcdExchange
#===================================================================================================
## @brief:
## This class contains the data of an icd exchange, defined as group of one WRITE action and its
## corresponding reads. All actions in an icd exchange have the same sys, bus, trick sequence,
## pui, subsys, varName, type, units, and comments.
class IcdExchange():
    ## @brief:
    ## Constructs with arguments defaulted to uninitialized.
    def __init__(self):

        uninitialized = "[%s not initialized]"

        ## IcdAction objects.......................................
        ## Describes an icd WRITE action. There is only one Write per IcdExchange.
        self.mWriteAction = IcdAction()
        ## List of IcdAction objects that describe a READ action. There can be multiple Reads for
        ## every Write.
        self.mReadActions = []

        ## Enumeration data..........................................
        ## String that identifies a particular link, used in a PTCS enumeration
        self.mEnumId = uninitialized % "mEnumId"
        ## The index of a link in a PTCS enumeration (its array index in a Gunns thermal link array)
        self.mEnumIndex = uninitialized % "mEnumIndex"

        ## Exchange-specific ICD data..............................
        ## Program Unique Identifier, unique alphanumeric id used to identify a data interface.
        self.mPui = uninitialized % "mPui"
        ## The type of bus handling the data exchange.
        self.mSubsys = uninitialized % "mSubsys"
        ## The bus variable name to instantiate on the bus.
        self.mVarName = uninitialized % "mVarName"
        ## Type of data being exchanged, "float", "double", "integer", etc.
        self.mType = uninitialized % "mType"
        ## Units of data being exchanged.
        self.mUnits = uninitialized % "mUnits"
        ## Comments describing the ICD exchange.
        self.mComments = uninitialized % "mComments"

    #===============================================================================================
    ## @brief:
    ## Public function. Initializes the class with the data needed to print it on the icd.
    ## @param[in]: id            string that identifies the particular link, used
    ##                               in the PTCS enumeration
    ## @param[in]: enumIndex     the number in the PTCS enumeration (its array index in the
    ##                               Gunns thermal link array)
    ## @param[in]: varName       name of the icd variable name, must be unique
    ## @param[in]: writeElement  xml-element with data needed to construct a WRITE job,
    ##                               children must be <rate>, <simObject>, and <simVarName>,
    ##                               or a single <fromOtherIcd>
    ## @param[in]: readElements  xml-elements with data needed to construct READ jobs,
    ##                               children must be <rate>, <simObject>, and <simVarName>
    ## @param[in]: pui           pui representing data being exchange, or just "xxxx"
    ## @param[in]: subsys        subsystem involved in data exchange, ex: "ptcs"
    ## @param[in]: type          type of data being exchanged, ex: "float", "double"
    ## @param[in]: units         units string in Trick standard, ex: "W/m2", "K", "W", "--"
    ## @param[in]: comments      comments describing icd exchange
    def initialize(self, id, enumIndex, varName, writeElement, readElements,\
                   pui, subsys, type, units, comments=""):

        ## Initialize icd data.
        self.mPui = pui
        self.mSubsys = subsys
        self.mVarName = varName
        self.mType = type
        self.mUnits = units
        self.mComments = comments

        ## Initialize enum id and index.
        self.mEnumId = id
        self.mEnumIndex = enumIndex

        ## Determine if at least one action in the exchange is active.
        atLeastOneActiveEntry = self.isThereAtLeastOneActiveEntry(writeElement, readElements)

        ## Initialize WRITE job.
        if writeElement.find("fromOtherIcd") != None:

            ## If so, overwrite the varName for the READ job and disable the WRITE job.
            self.mVarName = writeElement.find("fromOtherIcd").text

            ## Clear mWriteAction so it doesn't get printed.
            self.mWriteAction = None

        else:
            ## Otherwise, get the <rate>, <simObject>, and <simVarName> as normal.
            self.mWriteAction.initialize(id, atLeastOneActiveEntry, "WRITE", writeElement)

        ## Initialize READ jobs.
        for readElement in readElements:
            newReadAction = IcdAction()

            ## Get the <rate>, <simObject>, and <simVarName>.
            newReadAction.initialize(id, atLeastOneActiveEntry, "READ", readElement)

            ## Append the newly construction readAction to the readAction list.
            self.mReadActions.append(newReadAction)

    # -------------------------------------------------------------------------------------------------
    ## @brief:
    ## Private method. Checks the "isReadyForIcd" attribute of each job in an icd exchange. If the
    ## write Job and at least one read job has "isReadyForIcd" == 'true', the method returns True.
    ## @param[in]: writeElement  xml-element with data needed to construct a WRITE job
    ## @param[in]: readElements  xml-elements with data needed to construct READ jobs
    ## @return:    boolean, True if write job and at least one read job has "isReadyForIcd" == 'true'
    def isThereAtLeastOneActiveEntry(self, writeElement, readElements):

        ## Entire exchange should be commented out if the write portion is not ready.
        if 'true' != writeElement.get("isReadyForIcd"):
            return False

        ## If one read job is ready in addition to the write, the exchange is considered active.
        for readElement in readElements:
            if 'true' == readElement.get("isReadyForIcd"):
                return True

        ## If no return encountered above, return False. Entire exchange will be commented out.
        return False
