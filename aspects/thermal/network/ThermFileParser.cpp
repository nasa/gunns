/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   (
    (parsing/ParseTool.o)
    (parsing/tinyxml/tinyxml.o)
    (software/exceptions/TsInitializationException.o)
    (software/exceptions/TsParseException.o)
   )

PROGRAMMERS:
   ((Joe Valerioti) (L3) (Dec 2012)))
***************************************************************************************************/
#include "ThermFileParser.hh"
#include "parsing/ParseTool.hh" //needed to check file accessibility in openFile
#include "software/exceptions/TsParseException.hh"
#include "aspects/thermal/PtcsMacros.hh"
#include "math/MsMath.hh" //needed for fabs() and FLT_EPSILON in confirmNodeBuild()
#include <algorithm> //needed for find() function in getCapEditGroupId()
#include <iostream>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Integer used to designate a node name that is not yet registered in the Node map.
////////////////////////////////////////////////////////////////////////////////////////////////////
const int ThermFileParser::NOT_FOUND  = -99;
/**************************************************************************************************/
/* ThermFileParser class */
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the ThermFileParser with the name of the network as an argument.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermFileParser::ThermFileParser(const std::string name)
    :
    areNodesRegistered(false),
    mName(name),
    mNodeFile(),
    mCondFile(),
    mRadFile(),
    mHtrFile(),
    mPanFile(),
    mEtcFile(),
    mThermInputFile(),
    mThermInputFileRad(),
    numNodes(0),
    vNodeNames(0),
    mNodeMap(),
    vCapEditGroupList(0),
    numLinksCap(0),
    vCapNames(0),
    vCapPorts(0),
    vCapTemperatures(0),
    vCapCapacitances(0),
    vCapEditGroupIdentifiers(0),
    numLinksCond(0),
    vCondNames(0),
    vCondPorts0(0),
    vCondPorts1(0),
    vCondConductivities(0),
    numLinksRad(0),
    numInputEntriesRad(0),
    vRadNames(0),
    vRadPorts0(0),
    vRadPorts1(0),
    vRadCoefficients(0),
    vRadViewScalars(0),
    numLinksHtr(0),
    vHtrNames(0),
    vHtrScalars(0),
    vHtrPorts(0),
    vHtrFracs(0),
    numLinksPan(0),
    vPanNames(0),
    vPanAreas(0),
    vPanAbsorptivities(0),
    vPanPorts(0),
    vPanFracs(0),
    numLinksPot(0),
    vPotNames(0),
    vPotPorts(0),
    vPotTemperatures(0),
    vPotConductivities(0),
    numLinksSrc(0),
    vSrcNames(0),
    vSrcInits(0),
    vSrcScalars(0),
    vSrcPorts(0),
    vSrcFracs(0)
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this ThermFileParser.
////////////////////////////////////////////////////////////////////////////////////////////////////
ThermFileParser::~ThermFileParser()
{
    // nothing to do
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throw    TsParseException
///
/// @details  This is intended to be called prior to the initialize method, to count and store the
///           number of nodes for a ThermalNetwork to access prior to its initialization when used
///           in a super-network.
///
/// @note  This leaves numNodes != 0, so the readNodeFile method must reset it to zero prior to
///        counting nodes again.
///
/// @note  Since this function is intended to be called prior to initialization, H&S errors thrown
///        from the call to readFile won't have an instance name.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::preCountNodes()
{
    /// - Reset number of nodes for a fresh count.
    numNodes = 0;
    /// - Cause the node file to be parsed and node elements counted in numNodes.
    readFile(mNodeFile, "node", &ThermFileParser::countNode);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name  (--)  Name of object, usually the same as the associated ThermalNetwork.
///
/// @details  Parses thermal xml files and builds data vectors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::initialize(const std::string& name)
{
    /// - File names should be set previously by the parser's ThermalNetwork owner.
    try
    {
        /// - For backward compatibility with old sims that don't pass the network name in the
        ///   constructor, we reset the name to this method's argument.  This argument only overrides
        ///   the network name if it was previously empty.
        if ("" == mName and "" != name) {
            mName = name;
        }

        /// - Validate the object name.
        if ("" == mName) {
            TS_PTCS_ERREX(TsInitializationException, "initialization error", "a ThermFileParser has empty object name.")
        }

        /// - Read each file and build data vectors.
        readNodeFile();
        readCondFile();
        readRadFile();
        readHtrFile();
        readPanFile();
        readEtcFile();

        /// - Read ThermInput files for any overrides.
        readThermInputFile();
        readThermInputFileRad();

    } catch(TsParseException& e)
    {
        /// - Catch validateFile() exception and re-throw.
        TS_PTCS_ERREX(TsParseException, "parsing error", "Error reading thermal config-files.")
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throw      TsParseException if mNodeFile member is null
///
/// @details    Builds data vectors by parsing a node config-file. The node file contains
///             an XML inventory of all nodes and their associated data.
///             The list of nodes is also used to build a vector of capacitive links, since a
///             capacitive link is simply a connection between one node and vacuum.
///
/// @note       The node file should be in the following format. It should end with a SPACE node
///             that has zero capacitance.
///             \verbatim
///             <?xml version="1.0" ?>
///             <list>
///                 <node>
///                     <name>LAB_1</name>
///                     <temperature units='K'>300.00</temperature>
///                     <capacitance units='J/K'>2256.10</capacitance>
///                 </node>
///                 <node>
///                     <name>SPACE_1</name>
///                     <temperature units='K'>0.00</temperature>
///                     <capacitance units='J/K'>0.00</capacitance>
///                 </node>
///             </list>
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readNodeFile()
{
    /// - Reset node registration flag.
    areNodesRegistered = false;

    /// - Clear the counts and vectors.
    clearNode();
    clearCap();

    /// - Every instance of this class must use a node-file.
    TS_PTCS_IF_ERREX(mNodeFile.empty(), TsParseException, "parsing error", "Null ptr to node-file.")

    /// -
    readFile(mNodeFile, "capEditing", &ThermFileParser::registerCapEditGroups);

    /// - Read the XML file, calling the specifics needed for Nodes & Capacitance Links.
    readFile(mNodeFile, "node", &ThermFileParser::registerNode);

    /// - At the file's conclusion, make sure the last node is the space/ground/vacuum node.
    confirmNodeBuild();

    /// - Activate node registration flag.
    areNodesRegistered = true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Builds data vectors by parsing a conduction-link config-file. The cond-file contains
///             an XML inventory of all conduction links and their associated data.
///
/// @note       The conduction-link file should be in the following format.
///             \verbatim
///             <?xml version="1.0" ?>
///             <list>
///                 <conduction>
///                     <node0>LAB_CVIU_1</node0>
///                     <node1>LAB_STRUCT_D_1</node1>
///                     <conductivity units='W/K'>1.000000</conductivity>
///                 </conduction>
///                 <conduction>
///                     <node0>LAB_CP_D0_7</node0>
///                     <node1>LAB_MDM_1</node1>
///                     <conductivity units='W/K'>1.000000</conductivity>
///                 </conduction>
///             </list>
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readCondFile()
{
    /// - Clear the counts and vectors.
    clearCond();

    /// - Not every instance of this class will use a cond-file. Sometimes the pointer will be
    ///   null, and this method should just do nothing.
    if (mCondFile.empty())
    {
        return;
    }

    /// - Read the XML file, calling the specifics needed for Conduction Links.
    readFile(mCondFile, "conduction", &ThermFileParser::registerCond);

    /// - If a file was read, make sure relevant data was actually included.
    confirmDataFound(numLinksCond, "cond-file", mCondFile);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Builds data vectors by parsing a radiation-link config-file. The rad-file contains
///             an XML inventory of all radiation links and their associated data.
///
/// @note       The radiation-link file should be in the following format.
///             \verbatim
///             <?xml version="1.0" ?>
///             <list>
///                 <radiation>
///                     <node0>P1_ATA_PALLET_1</node0>
///                     <node1>SPACE_1</node1>
///                     <coefficient units='m2'>4.079400</coefficient>
///                 </radiation>
///                 <radiation>
///                     <node0>S1_RAD_11</node0>
///                     <node1>SPACE_1</node1>
///                     <coefficient units='m2'>13.843000</coefficient>
///                 </radiation>
///             </list>
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readRadFile()
{
    /// - Clear the counts and vectors.
    clearRad();

    /// - Not every instance of this class will use a rad-file. Sometimes the pointer will be
    ///   null, and this method should just do nothing.
    if (mRadFile.empty())
    {
        return;
    }

    /// - Read the XML file, calling the specifics needed for Radiation links.
    readFile(mRadFile, "radiation", &ThermFileParser::registerRad);

    /// - If a file was read, make sure relevant data was actually included.
    confirmDataFound(numLinksRad, "rad-file", mRadFile);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Builds data vectors by parsing a heater registry config-file. The htr-file contains
///             an XML inventory of all heaters and source links  and their associated data.
///
/// @note       The heater file should be in the following format. Only the <name>, <node>, and
///             <tune> data is parsed by this class. The other data is relevant to the electrical
///             and signal aspects.
///             \verbatim
///             <?xml version="1.0" ?>
///             <list>
///                 <source name='src_AL_dissipation'>
///                     <node>AL_DISSIPATION_1</node>
///                     <flux units='W'>20.0</flux>
///                 </source>
///                 <heater name='htr_AL_shell_1P'>
///                     <node>AL_SHELL_11<frac>0.143</frac></node>
///                     <node>AL_SHELL_12<frac>0.357</frac></node>
///                     <node>AL_SHELL_13<frac>0.143</frac></node>
///                     <node>AL_SHELL_14<frac>0.357</frac></node>
///                     <scalar>1.0</scalar>
///                     <control type='MDM'>
///                         <lower units='K'>270</lower>
///                         <upper units='K'>280</upper>
///                     </control>
///                     <elec isRegistered='true'>
///                         <powerSource>rpcm AL1A4A B</powerSource>
///                         <rpc>7</rpc>
///                         <ohms>41.46</ohms>
///                     </elec>
///                     <info>RTD 1: W031RT001; RTD 2: W031RT002</info>
///                 </heater>
///                 <heater name='htr_AL_shell_1S'>
///                     <node>AL_SHELL_11<frac>0.00</frac></node>
///                     <node>AL_SHELL_12<frac>0.4285</frac></node>
///                     <node>AL_SHELL_13<frac>0.143</frac></node>
///                     <node>AL_SHELL_14<frac>0.4285</frac></node>
///                     <scalar>1.0</scalar>
///                     <control type='thermostat'>
///                         <instance isInstantiated='true'>tstatAlShell1S</instance>
///                         <sensors>2</sensors>
///                         <logicType>OR</logicType>
///                         <lower units='K'>290</lower>
///                         <upper units='K'>300</upper>
///                     </control>
///                     <elec isRegistered='true'>
///                         <powerSource>rpcm N1RS1 B</powerSource>
///                         <rpc>1</rpc>
///                         <ohms>41.46</ohms>
///                     </elec>
///                     <info>RTD 1: W019RT011; RTD 2: W019RT012</info>
///                 </heater>
///             </list>
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readHtrFile()
{
    /// - Clear the counts and vectors.
    clearHtr();
    clearSrc();

    /// - Not every instance of this class will use a htr-file. Sometimes the pointer will be
    ///   null, and this method should just do nothing.
    if (mHtrFile.empty())
    {
        return;
    }

    /// - Read the XML file, calling the specifics needed for Heater links.
    readFile(mHtrFile, "heater", &ThermFileParser::registerHtr);

    /// - Read the XML file, calling the specifics needed for Source Links.
    readFile(mHtrFile, "source", &ThermFileParser::registerSrc);

    /// - If a file was read, make sure relevant data was actually included.
    confirmDataFound(numLinksHtr || numLinksSrc, "htr-file", mHtrFile);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Builds data vectors by parsing a thermal-panel config-file. The pan-file contains
///             an XML inventory of all thermal-panels and their associated data.
///
/// @note       The panel file should be in the following format. Only the <name>, <node>, <area>,
///             and <absorp> data is parsed by this class. The other data is relevant to the
///             environment aspect.
///             \verbatim
///             <panel name='pan_CUP_WIN_1'>
///                 <node>CUP_WIN_1</node>
///                 <des>cupola panel win1</des>
///                 <area units='m2'>0.298</area>
///                 <absorp>0.22</absorp>
///                 <frame>N3</frame>
///                 <position units='m'>
///                     <x>-0.734</x>
///                     <y>-0.001</y>
///                     <z>-2.966</z>
///                 </position>
///                 <surfaceNormal units='m'>
///                     <x>-0.866</x>
///                     <y>0</y>
///                     <z>-0.5</z>
///                 </surfaceNormal>
///             </panel>
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readPanFile()
{
    /// - Clear the counts and vectors.
    clearPan();

    /// - Not every instance of this class will use a pan-file. Sometimes the pointer will be
    ///   null, and this method should just do nothing.
    if (mPanFile.empty())
    {
        return;
    }

    /// - Read the XML file, calling the specifics needed for heater links.
    readFile(mPanFile, "panel", &ThermFileParser::registerPan);

    /// - If a file was read, make sure relevant data was actually included.
    confirmDataFound(numLinksPan, "pan-file", mPanFile);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Builds data vectors by parsing an etc-file, the thermal config-file that contains
///             all remaining link types ("et cetera"). The etc-file contains
///             an XML inventory of all potential links and their associated data.
///
/// @note       The etc-file should be in the following format.
///             \verbatim
///             <?xml version="1.0" ?>
///             <list>
///                 <potential>
///                     <name>Airlock air</name>
///                     <node>AL_AIR_1</node>
///                     <temperature units='K'>296.0</temperature>
///                     <conductivity units='W/K'>1e12</conductivity>
///                 </potential>
///                 <potential>
///                     <name>sublimator</name>
///                     <node>AL_EMU_SUB_1</node>
///                     <temperature units='K'>296.00</temperature>
///                     <conductivity units='W/K'>1e12</conductivity>
///                 </potential>
///             </list>
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readEtcFile()
{
    /// - Clear the counts and vectors.
    clearPot();

    /// - Not every instance of this class will use an etc-file. Sometimes the pointer will be
    ///   null, and this method should just do nothing.
    if (mEtcFile.empty())
    {
        return;
    }

    /// - Read the XML file, calling the specifics needed for Potential Links.
    readFile(mEtcFile, "potential", &ThermFileParser::registerPot);

    /// - If a file was read, make sure relevant data was actually included.
    confirmDataFound(numLinksPot, "etc-file", mEtcFile);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Edits existing link data by reading overrides described in the ThermInput file.
///
/// @note       The ThermInput-file should be in the following format.
///             \verbatim
///             <?xml version="1.0" ?>
///             <list>
///                 <node name='CECLSS_26'>
///                     <initTemp units='K'>12.34</initTemp>
///                 </node>
///                 <node name='CECLSS_46'>
///                     <initTemp units='K'>56.78</initTemp>
///                 </node>
///             </list>
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readThermInputFile()
{
    /// - Not every instance of this class will use an ThermInput file. Sometimes the pointer will
    ///   be null, and this method should just do nothing.
    if (mThermInputFile.empty())
    {
        return;
    }
    try
    {
        /// - Read the XML file, calling the specifics needed for Input Overrides.
        readFile(mThermInputFile, "node", &ThermFileParser::registerInput);

    } catch(TsParseException& e)
    {
        /// - Issue warning that ThermInput file will be ignored.
        TS_PTCS_WARNING("Ignoring ThermInputFile: " << mThermInputFile);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details    Edits existing link data by reading overrides described in the ThermInput file.
///
/// @note       The ThermInput-file should be in the following format.
///             \verbatim
///             <?xml version="1.0" ?>
///             <list>
///                 <node name='CECLSS_26'>
///                     <initTemp units='K'>12.34</initTemp>
///                 </node>
///                 <node name='CECLSS_46'>
///                     <initTemp units='K'>56.78</initTemp>
///                 </node>
///             </list>
///             \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readThermInputFileRad()
{
    /// - Not every instance of this class will use an ThermInput file. Sometimes the pointer will
    ///   be null, and this method should just do nothing.
    if (mThermInputFileRad.empty())
    {
        return;
    }
    try
    {
        /// - Read the XML file, calling the specifics needed for Input Overrides.
        readFile(mThermInputFileRad, "radiation", &ThermFileParser::registerInputRad);

    } catch(TsParseException& e)
    {
        /// - Issue warning that ThermInput file will be ignored.
        TS_PTCS_WARNING("Ignoring ThermInputFileRad: " << mThermInputFileRad);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  xmlFile       (--)  XML file path and name
/// @param[in]  linkTag       (--)  XML tag which contains the thermal link data
/// @param[in]  registerLink  (--)  A pointer to a void function that takes a TiXmlElement pointer;
///                                 used to represent one of the seven registerXxx() methods
///
/// @throw    TsParseException if error found during parsing of XML-file.
///
/// @details  Private method used to generically read any of the thermal XML files. Opens the file
///           and accesses the <list> of thermal link data. Loops through every element in
///           the list and calls the register() method given by the function pointer argument.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::readFile(std::string& xmlFile, const char* linkTag,
        void (ThermFileParser::*registerLink)(TiXmlElement*))
{
    try
    {
        /// - Instantiate a TiXmlDocument for loading data from the file.
        TiXmlDocument doc;

        /// - Validate file accessibility and load XML data.
        openFile(doc, xmlFile);

        /// - Get the element data from the <list> tag. Raise an error if not found.
        TiXmlElement* list = getElement(&doc, "list", true);
        /// - Get the element data from the <linkTag> tag. Don't raise an error if not found.
        TiXmlElement* elem = getElement(list, linkTag, false);

        /// - Loop through all relevant elements listed in the XML file.
        while (0 != elem)
        {
            /// - Call given register() function to populate data vectors.
            (this->*registerLink)(elem);

            /// - Advance to the next element.
            elem = elem->NextSiblingElement(linkTag);
        }

    } catch(TsParseException& e)
    {
        /// - Re-throw.
        TS_PTCS_ERREX(TsParseException, "XML file error", xmlFile)
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  doc      (--) TiXmlDocument object to load XML file data into
/// @param[in]      xmlFile  (--) path and name of XML file
///
/// @throw      TsParseException if unable to access file or if XML load fails
///
/// @details    Loads given xml file into TiXmlDocument member.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::openFile(TiXmlDocument& doc, std::string& xmlFile)
{
    try
    {
        /// - Make sure the file is accessible.
        ParseTool::validateFile(xmlFile.c_str());

    } catch(TsParseException& e)
    {
        /// - Catch validateFile() exception and re-throw.
        TS_PTCS_ERREX(TsParseException, "file accessibility error", "Cannot open file for parsing.")
    }

    /// - Call TinyXml LoadFile().
    doc.LoadFile(xmlFile);

    /// - Throw an exception if document was not able to load.
    if( !doc.LoadFile() )
    {
        TS_PTCS_ERREX(TsParseException, "invalid XML format", xmlFile)
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clears Node name vector and map object. Resets the number of Nodes count.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::clearNode()
{
    /// - Clear any Node data.
    vNodeNames.clear();
    mNodeMap.clear();
    /// - Clear the number of nodes.
    numNodes = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clears Capacitance link temperature, capacitance, and name vectors. Resets the
///           number of Capacitance links count.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::clearCap()
{
    /// - Clear Capacitance Link data vectors.
    vCapNames.clear();
    vCapPorts.clear();
    vCapTemperatures.clear();
    vCapCapacitances.clear();
    /// - Clear the number of capacitance links.
    numLinksCap = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clears Conduction link name, port0, port1, and conductivity vectors. Resets the
///           number of Conduction links count.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::clearCond()
{
    /// - Clear conduction data vectors.
    vCondNames.clear();
    vCondPorts0.clear();
    vCondPorts1.clear();
    vCondConductivities.clear();
    /// - Clear the number of conduction links.
    numLinksCond = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clears Radiation link port0, port1, coefficient, and name vectors. Resets the
///           number of Radiation links count.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::clearRad()
{
    /// - Clear radiation data vectors.
    vRadNames.clear();
    vRadPorts0.clear();
    vRadPorts1.clear();
    vRadCoefficients.clear();
    vRadViewScalars.clear();
    /// - Clear the number of radiation links.
    numLinksRad = 0;
    numInputEntriesRad = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clears Heater ports, efficiency, name, and fraction vectors. Resets the
///           number of Heaters count.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::clearHtr()
{
    /// - Clear the Thermal Heater data vectors.
    vHtrScalars.clear();
    vHtrNames.clear();
    vHtrPorts.clear();
    vHtrFracs.clear();
    /// - Clear the number of heaters.
    numLinksHtr = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clears ThermalPanel port1, name, surface area, absorptivity, and view factor vectors.
///           Resets the number of ThermalPanels count.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::clearPan()
{
    /// - Clear Thermal Panel data vectors.
    vPanPorts.clear();
    vPanNames.clear();
    vPanAreas.clear();
    vPanAbsorptivities.clear();
    /// - Clear the number of panels.
    numLinksPan = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clears Potential link node, name, temperature, and conductivity vectors. Resets the
///           number of Potential links count.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::clearPot()
{
    /// - Clear Potential Link data vectors.
    vPotNames.clear();
    vPotPorts.clear();
    vPotTemperatures.clear();
    vPotConductivities.clear();
    /// - Clear the number of potential links.
    numLinksPot = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Clears Source port, flux, and name vectors. Resets the
///           number of Sources count.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::clearSrc()
{
    /// - Clear Source Link data vectors.
    vSrcNames.clear();
    vSrcPorts.clear();
    vSrcInits.clear();
    /// - Clear the number of sources.
    numLinksSrc = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node  (--)  TiXmlElement pointer to a set of <node> data
///
/// @details  Adds the node name to the map key, assigning to it the value of the numNodes count,
///           which is later incremented. Also pushes the node onto the node names vector.
///           Populates Capacitance link vectors with the strings stored in the tags
///           from the node-file. The Capacitance links count is then incremented as well.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerCapEditGroups(TiXmlElement* capEditing)
{
    /// - Get the element data from the <linkTag> tag.
    TiXmlElement* elem = getElement(capEditing, "group", true);

    /// - Loop through all relevant elements listed in the XML file.
    while (0 != elem)
    {
        /// - Read text from <group> tag.
        std::string group = getText(elem, 0);
        vCapEditGroupList.push_back(group);

        /// - Advance to the next element.
        elem = elem->NextSiblingElement("group");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node  (--)  TiXmlElement pointer to a set of <node> data, not used.
///
/// @details  This increments the node count each time this is called.  The node argument isn't used
///           here but provides the function signature needed to be used as a function pointer in
///           the readFile method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::countNode(TiXmlElement* node __attribute__((unused)))
{
    numNodes++;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node  (--)  TiXmlElement pointer to a set of <node> data
///
/// @details  Adds the node name to the map key, assigning to it the value of the numNodes count,
///           which is later incremented. Also pushes the node onto the node names vector.
///           Populates Capacitance link vectors with the strings stored in the tags
///           from the node-file. The Capacitance links count is then incremented as well.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerNode(TiXmlElement* node)
{
    /// Create a baseline nodeName.
    std::string nodeName = "(error setting name)";

    try
    {
        /// - Get node name and validate.
        nodeName = getText(node, "name");
        validateNode( nodeName );

        /// - Get data.
        const double temperature = ParseTool::convertToDouble( getText(node, "temperature"), TS_HS_PTCS,
                                                               "Thermal cap link: " + nodeName + ".");
        const double capacitance = ParseTool::convertToDouble( getText(node, "capacitance"), TS_HS_PTCS,
                                                               "Thermal cap link: " + nodeName + ".");
        /// - Get cap-edit-group identifier.
        const int groupId = getCapEditGroupId(node);

        /// - If everything above was successful, store
        ///   Node/Capacitance Link data into their corresponding vectors.
        vNodeNames.push_back(nodeName);
        vCapNames.push_back("cap_" + nodeName);
        vCapPorts.push_back(numNodes);
        vCapTemperatures.push_back(temperature);
        vCapCapacitances.push_back(capacitance);
        vCapEditGroupIdentifiers.push_back(groupId);

        /// - Populate the NodeMap and increment the counts.
        mNodeMap[nodeName] = numNodes;
        numNodes++;
        numLinksCap++;

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not register node: "<< nodeName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  conduction  (--)  TiXmlElement pointer to a set of <conduction> data
///
/// @details  Populates conduction link vectors with the strings stored in the tags
///           from the cond-file. The conduction links count is then incremented.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerCond(TiXmlElement* conduction)
{
    /// Create a baseline linkName.
    std::string linkName = "(error setting name)";

    try
    {
        /// - Get the nodes at each port.
        const std::string node0 = getText(conduction, "node0");
        const std::string node1 = getText(conduction, "node1");

        /// - Look for a <name> tag. If it doesn't exist, just build a name based on the port nodes.
        if ( 0 == getElement(conduction, "name", false) )
        {
            /// - Generic name.
            linkName = "cond " + node0 + " to " + node1;

        } else
        {
            /// - Given name.
            linkName = getText(conduction, "name");
        }

        /// - Validate that the nodes exist.
        const int nodeIndex0 = validateNode(node0);
        const int nodeIndex1 = validateNode(node1);

        /// - Get remaining data.
        const double conduct = ParseTool::convertToDouble( getText(conduction, "conductivity"), TS_HS_PTCS,
                                                           "Thermal cond link: " + linkName + ".");
        /// - If everything above was successful,
        ///   store conduction link data into their corresponding vectors.
        vCondNames.push_back(linkName);
        vCondPorts0.push_back( nodeIndex0 );
        vCondPorts1.push_back( nodeIndex1 );
        vCondConductivities.push_back(conduct);

        /// - Increment the count.
        numLinksCond++;

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not register cond link: "<< linkName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  radiation  (--)  TiXmlElement pointer to a set of <radiation> data
///
/// @details  Populates radiation link vectors with the strings stored in the tags
///           from the rad-file. The radiation links count is then incremented.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerRad(TiXmlElement* radiation)
{
    /// Create a baseline linkName.
    std::string linkName = "(error setting name)";

    try
    {
        /// - Get the nodes at each port.
        const std::string node0 = getText(radiation, "node0");
        const std::string node1 = getText(radiation, "node1");

        /// - Look for a <name> tag. If it doesn't exist, just build a name based on the port nodes.
        if ( 0 == getElement(radiation, "name", false) )
        {
            /// - Generic name.
            linkName = "rad " + node0 + " to " + node1;

        } else
        {
            /// - Given name.
            linkName = getText(radiation, "name");
        }

        /// - Validate that the nodes exist.
        const int nodeIndex0 = validateNode(node0);
        const int nodeIndex1 = validateNode(node1);

        /// - Get remaining data.
        const double radk = ParseTool::convertToDouble( getText(radiation, "coefficient"), TS_HS_PTCS,
                                                        "Thermal rad link: " + linkName + ".");
        /// - If everything above was successful,
        ///   store radiation link data into their corresponding vectors.
        vRadNames.push_back(linkName);
        vRadPorts0.push_back( nodeIndex0 );
        vRadPorts1.push_back( nodeIndex1 );
        vRadCoefficients.push_back(radk);

        /// - Increment the count.
        numLinksRad++;

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not register rad link: "<< linkName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  heater  (--)  TiXmlElement pointer to a set of <heater> data
///
/// @details  Populates heater link vectors with the strings stored in the tags
///           from the htr-file. The heater links count is then incremented.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerHtr(TiXmlElement* heater)
{
    /// - Get link's name attribute.
    std::string linkName = getName(heater, "heater");

    try
    {
        /// - Create temporary vectors to hold the port numbers and flux-application-fractions
        ///   for this panel.
        std::vector<int> portsVector;
        std::vector<double> fracsVector;

        /// - Populate the ports and fraction vectors based on the data in the pan-file.
        buildMultiPortVectors(portsVector, fracsVector, heater, "htr", linkName);

        /// - Set tuningScalar to default value.
        double scalar = 1.0;

        /// - Get scalar text from XML if provided.
        const char* scalarText = getText(heater, "scalar", false);
        if (0 != scalarText)
        {
            scalar = ParseTool::convertToDouble(
                    scalarText, TS_HS_PTCS, "Thermal htr link: " + linkName + ".");
        }
        /// - If everything above was successful,
        ///   store heater link data into their corresponding vectors.
        vHtrNames.push_back(linkName);
        vHtrPorts.push_back(portsVector);
        vHtrFracs.push_back(fracsVector);
        vHtrScalars.push_back(scalar);

        /// - Increment the count.
        numLinksHtr++;

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not register htr link: "<< linkName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  panel  (--)  TiXmlElement pointer to a set of <panel> data
///
/// @details  Populates panel link vectors with the strings stored in the tags
///           from the pan-file. The panel links count is then incremented.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerPan(TiXmlElement* panel)
{
    /// - Get link's name attribute.
    std::string linkName = getName(panel, "panel");

    try
    {
        /// - Create temporary vectors to hold the port numbers and flux-application-fractions
        ///   for this panel.
        std::vector<int> portsVector;
        std::vector<double> fracsVector;

        /// - Populate the ports and fraction vectors based on the data in the pan-file.
        buildMultiPortVectors(portsVector, fracsVector, panel, "pan", linkName);

        /// - Get remaining data.
        const double area = ParseTool::convertToDouble( getText(panel, "area"), TS_HS_PTCS,
                                                        "Thermal panel: " + linkName + ".");
        const double alpha = ParseTool::convertToDouble( getText(panel, "absorp"), TS_HS_PTCS,
                                                         "Thermal panel: " + linkName + ".");
        /// - If everything above was successful,
        ///   store panel link data into their corresponding vectors.
        vPanNames.push_back(linkName);
        vPanPorts.push_back(portsVector);
        vPanFracs.push_back(fracsVector);
        vPanAreas.push_back(area);
        vPanAbsorptivities.push_back(alpha);

        /// - Increment the count.
        numLinksPan++;

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not register panel: "<< linkName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  potential  (--)  TiXmlElement pointer to a set of <potential> data
///
/// @details  Populates potential link vectors with the strings stored in the tags
///           from the etc-file. The potential links count is then incremented.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerPot(TiXmlElement* potential)
{
    /// Create a baseline linkName.
    std::string linkName = "(error setting name)";

    try
    {
        /// - Get name first.
        linkName = getText( potential, "name");

        /// - Get node and validate.
        const std::string node = getText(potential, "node");
        const int nodeIndex = validateNode(node);

        /// - Get remaining data.
        const double temp = ParseTool::convertToDouble( getText(potential, "temperature"), TS_HS_PTCS,
                                                        "Thermal pot link: " + linkName + ".");
        const double conduct = ParseTool::convertToDouble( getText(potential, "conductivity"), TS_HS_PTCS,
                                                           "Thermal pot link: " + linkName + ".");
        /// - If everything above was successful,
        ///   store potential Link data into their corresponding vectors.
        vPotNames.push_back(linkName);
        vPotPorts.push_back( nodeIndex );
        vPotTemperatures.push_back(temp);
        vPotConductivities.push_back(conduct);

        /// - Increment the count.
        numLinksPot++;

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not register pot link: "<< linkName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  source  (--)  TiXmlElement pointer to a set of <source> data
///
/// @details  Populates source link vectors with the strings stored in the tags
///           from the etc-file. The source links count is then incremented.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerSrc(TiXmlElement* source)
{
    /// - Get link's name attribute.
    std::string linkName = getName(source, "source");

    try
    {
        /// - Create temporary vectors to hold the port numbers and flux-application-fractions
        ///   for this panel.
        std::vector<int> portsVector;
        std::vector<double> fracsVector;

        /// - Populate the ports and fraction vectors based on the data in the pan-file.
        buildMultiPortVectors(portsVector, fracsVector, source, "src", linkName);

        /// - Set data to default value.
        double initFlux = 0.0;
        double scalar = 1.0;

        /// - Get initFlux text from XML if provided.
        const char* initFluxText = getText(source, "initFlux", false);
        if (0 != initFluxText)
        {
            initFlux = ParseTool::convertToDouble(
                    initFluxText, TS_HS_PTCS, "Thermal src link: " + linkName + ".");
        }
        /// - Get scalar text from XML if provided.
        const char* scalarText = getText(source, "scalar", false);
        if (0 != scalarText)
        {
            scalar = ParseTool::convertToDouble(
                    scalarText, TS_HS_PTCS, "Thermal src link: " + linkName + ".");
        }
        /// - If everything above was successful,
        ///   store source link data into their corresponding vectors.
        vSrcNames.push_back(linkName);
        vSrcPorts.push_back(portsVector);
        vSrcFracs.push_back(fracsVector);
        vSrcInits.push_back(initFlux);
        vSrcScalars.push_back(scalar);

        /// - Increment the count.
        numLinksSrc++;

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not register src link: "<< linkName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node  (--)  TiXmlElement pointer to a set of <node> input data
///
/// @details  Edits data on the previously registered node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerInput(TiXmlElement* node)
{
    /// - Get node's name attribute.
    std::string nodeName = getName(node, "node");

    try
    {
        /// - Get the index of the node within the previously constructed nodeMap.
        const int nodeIndex = validateNode(nodeName);

        /// - Get initTemp text from XML if provided.
        const char* initTempText = getText(node, "initTemp", false);
        if (0 != initTempText)
        {
            const double initTemp = ParseTool::convertToDouble(
                    initTempText, TS_HS_PTCS, "ThermInput override: " + nodeName + ".");

            /// - Change the temperature to the override value.
            vCapTemperatures.at(nodeIndex) = initTemp;
        }

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not apply input data: "<< nodeName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  radiation  (--)  TiXmlElement pointer to a set of <radiation> input data
///
/// @details  Edits data on the previously registered radiation link object.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::registerInputRad(TiXmlElement* radiation)
{
    /// Create a baseline linkName.
    std::string linkName = "(error setting name)";

    if (numInputEntriesRad > numLinksRad) {
        TS_PTCS_WARNING("More radiation input data than radiation links. Did not register rad link input data: "<< linkName);
    }

    try
    {
        /// - Get the nodes at each port.
        const std::string node0 = getText(radiation, "node0");
        const std::string node1 = getText(radiation, "node1");

        /// - Look for a <name> tag. If it doesn't exist, just build a name based on the port nodes.
        if ( 0 == getElement(radiation, "name", false) )
        {
            /// - Generic name.
            linkName = "rad " + node0 + " to " + node1;

        } else
        {
            /// - Given name.
            linkName = getText(radiation, "name");
        }

        /// - Validate that the nodes exist.
        validateNode(node0);
        validateNode(node1);

        /// - Get input data.
        const double viewScalar = ParseTool::convertToDouble( getText(radiation, "viewScalar"), TS_HS_PTCS,
                                                        "Thermal rad link: " + linkName + ".");
        /// - If everything above was successful,
        vRadViewScalars.push_back(viewScalar);

        /// - Increment the count.
        numInputEntriesRad++;

    } catch(...)
    {
        /// - Issue warning that the specific link was skipped.
        TS_PTCS_WARNING("Did not register rad link input data: "<< linkName);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] parent   (--)  pointer to parent XML element
/// @param[in] linkType (--)  the type of link (rad,pot,htr,etc.) currently being processed
///
/// @return   name (--) string with text from the "name" attribute within the XML element
///
/// @details  Private method used to get the name attribute of a TinyXML element.
////////////////////////////////////////////////////////////////////////////////////////////////////
const std::string ThermFileParser::getName(TiXmlElement* parent, const char* linkType)
{
    /// Create a baseline linkName.
    std::string name = "(error setting name)";

    /// - Query the "name" attribute.
    int attribute = parent->QueryStringAttribute("name", &name);

    /// - Check success.
    if ( attribute != TIXML_SUCCESS)
    {
        /// - Name not obtained.
        TS_PTCS_WARNING("Attribute 'name' not found: <" << linkType << " name='myName'>");
    }
    return name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] parent  (--) pointer to parent XML element
/// @param[in] tag     (--) name of tag interested in getting
///
/// @throw    TsParseException if no text is given within the XML tag
///
/// @return   text (--) string with text from XML element
///
/// @details  Private method used to safely get the text from a tag in an XML element.
////////////////////////////////////////////////////////////////////////////////////////////////////
const char* ThermFileParser::getText(TiXmlElement* parent, const char* tag,
        const bool raiseErrorIfNotFound)
{
    /// - Instantiate an element pointer.
    TiXmlElement* child = parent;

    if (0 != tag)
    {
        child = getElement(parent,tag,raiseErrorIfNotFound);
    }

    if (0 == child)
    {
        return 0;
    }

    /// - Read Text
    const char* text = child->GetText();

    /// - GetText() returns zero if failed. Throw exception.
    TS_PTCS_IF_ERREX(0 == text && true == raiseErrorIfNotFound, TsParseException,
            "No text given in XML tag,", tag);

    return text;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] parent               (--) pointer to parent XML element
/// @param[in] tag                  (--) name of tag interested in getting
/// @param[in] raiseErrorIfNotFound (--) optional, default true; if true, method throws exception
///                                      if the tag cannot be found
///
/// @throw    TsParseException if cannot find the given tag within the parent element
///
/// @return    child   (--) pointer to child XML element
///
/// @details  Private method used to return an element in an XML tree.
////////////////////////////////////////////////////////////////////////////////////////////////////
TiXmlElement* ThermFileParser::getElement(TiXmlNode* parent, const char* tag,
        const bool raiseErrorIfNotFound)
{
    TiXmlElement* child = parent->FirstChildElement(tag);

    /// - If desired, report error if element pointer returns null.
    TS_PTCS_IF_ERREX(0 == child && true == raiseErrorIfNotFound, TsParseException,
            "XML tag not found,", tag);

    return child;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  parent  (--) pointer to parent XML element
/// @param[in]  tag     (--) name of tag interested in counting
///
/// @details  Private method used to count elements.
////////////////////////////////////////////////////////////////////////////////////////////////////
int ThermFileParser::countElement(TiXmlNode* parent, const char* tag)
{
    int count = 0;
    for(TiXmlNode* child = parent->FirstChild(tag); child; child = child->NextSiblingElement(tag))
    {
        count++;
    }
    return count;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node  (--)  TiXmlElement pointer to a set of <node> config data
///
/// @return   groupId (--)  integer identifying the capacitance-edit-group identifier
///
/// @details  Private method used to determine the index of a cap-edit-group name within the
///           vCapEditGroupList vector.
////////////////////////////////////////////////////////////////////////////////////////////////////
int ThermFileParser::getCapEditGroupId(TiXmlElement* node)
{
    /// - Get editGroup text from XML if provided.
    const char* editGroup = getText(node, "editGroup", false);

    if (0 == editGroup)
    {
        return NOT_FOUND;
    }

    /// - Find the index of the cap-edit-group.
    std::vector<std::string>::iterator iter = std::find(vCapEditGroupList.begin(), vCapEditGroupList.end(), editGroup);

    if ( vCapEditGroupList.end() == iter)
    {
        /// - Name not obtained.
        TS_PTCS_ERROR("- '" << editGroup << "' not previously defined in <capEditing>.");
        return NOT_FOUND;
    }

    return static_cast<int>(std::distance(vCapEditGroupList.begin(), iter));
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]  vectorInt    (--)  pointer to vector<int> holding port numbers for a multi-port link
/// @param[in,out]  vectorDouble (--)  pointer to vector<double> holding flux-application-fractions for a multi-port link
/// @param[in]      parent       (--)  pointer to XML element
/// @param[in]      linkType     (--)  the type of link (rad,pot,htr,etc.) currently being processed
/// @param[in]      name         (--)  the name of the specific link currently being processed
///
/// @details  Builds ports and fractions vectors for multi-port links.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::buildMultiPortVectors(
        std::vector<int>& vectorInt,
        std::vector<double>& vectorDouble,
        TiXmlNode* parent,
        const std::string& linkType,
        const std::string& name)
{
    /// - Count the nodes and get the first one.
    const int numPorts = countElement(parent, "node");
    TiXmlElement* node = getElement(parent, "node");

    /// - Loop through all <node> elements.
    while (0 != node)
    {
        /// - Get the node name and validate that it has been registered.
        const std::string nodeName = node->GetText();

        /// - Get the port number from the nodeName, and push onto the portsVector.
        vectorInt.push_back( validateNode(nodeName) );

        /// - Initialize the flux-application-fraction to be 1.0, the default value for single-port.
        double fraction = 1.0;

        /// - The flux-application-fraction is NOT one if the panel is multi-port.
        if (numPorts > 1)
        {
            /// - Get the fraction datum.
            fraction = ParseTool::convertToDouble( getText(node, "frac"), TS_HS_PTCS,
                                                "Thermal " + linkType + " link: " + name + ".");
        }
        /// - Push onto fracsVector.
        vectorDouble.push_back(fraction);

        /// - Advance to the next element.
        node = node->NextSiblingElement("node");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeName  (--)  name of node in question
///
/// @return     The index of the node within the node map object. If the node is not valid,
///             an exception is thrown.
///
/// @throw      TsParseException on a node that is not valid.
///
/// @details    Verifies that a node name provided in a config-file is valid.
///             A node would NOT be valid if
///               (A) a node/capacitance link is requesting a node that is blank or
///                   has already been registered, or
///               (B) any other type of link is requesting a node that is not found in the node
///                   map. This happens if the node was not included in the node-file.
////////////////////////////////////////////////////////////////////////////////////////////////////
int ThermFileParser::validateNode(const std::string& nodeString)
{
    /// - Get the node index from string.
    const int nodeIndex = getMapLocation(nodeString);

    /// - If we are in the process of registering nodes (parsing the node file), we cannot have
    ///   blank nodes or duplicates.
    if (false == areNodesRegistered)
    {
        /// - It's bad if the node is blank or has already been registered
        TS_PTCS_IF_ERREX(strcmp("", nodeString.c_str() ) == 0 || NOT_FOUND != nodeIndex,
                TsParseException,
                "blank/repeated node name,", nodeString);

    /// - If nodes are already registered, then the links may only call a node which is
    ///   already in the NodeMap.
    } else if (NOT_FOUND == nodeIndex)
    {
        /// - Throw exception.
        TS_PTCS_ERREX(TsParseException, "unregistered node name", nodeString);
    }

    return nodeIndex;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  nodeName  (--)  the name of a node (e.g. "LAB_STRUCT_1")
///
/// @return     The value of the node name within the node map object. If the node is not found,
///             the method returns a NOT_FOUND.
///
/// @details    Returns the location of the node within the node map. This method is used to
///             determine the nodes that a Gunns link connects to. Thermal Desktop describes
///             a node with a string, and Gunns needs to reference that node with an integer.
////////////////////////////////////////////////////////////////////////////////////////////////////
int ThermFileParser::getMapLocation(const std::string& nodeName)
{
    /// - Use the find() function in the map to return the iterator number.
    std::map<const std::string, int>::iterator iter = mNodeMap.find(nodeName);

    /// - If the iterator gets to the end without matching the string, then the node is not found.
    if ( iter == mNodeMap.end() )
    {
        /// - Return a NOT_FOUND to indicate the node is not in the map key.
        return NOT_FOUND;
    }

    /// - If the string did have a match, return the iterator value converted to an int.
    return iter->second;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throw    TsParseException if the capacitance vector is empty
///
/// @details  Checks whether the space/ground/vacuum node is given at the end of the node list.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::confirmNodeBuild()
{
    /// - If the capacitances vector is empty, the back() call below will fail.
    if (vCapCapacitances.empty() )
    {
        /// - Warn the user of the possible cause of the error.
        TS_PTCS_ERROR("ThermalNetwork contained no nodes.");
        TS_PTCS_ERROR("Make sure node-file set in default_data job!");
        /// - Throw exception
        TS_PTCS_ERREX(TsParseException, "empty capacitances vector,",
                "No node data found in xml-file.");

    /// - Make sure that the last node has zero capacitance.
    } else  if (std::fabs( vCapCapacitances.back() ) < static_cast<double>(FLT_EPSILON) )
    {
        /// - Remove the last capacitance link since the space node doesn't need one.
        vCapNames.pop_back();
        vCapTemperatures.pop_back();
        vCapCapacitances.pop_back();
        numLinksCap--;

    } else
    {
        /// - Issue a warning. The last node listed should be the ground/vacuum/space node, and
        ///   if it is not given zero capacitance, the user likely made an error.
        TS_PTCS_WARNING("ThermalNetwork ground/space node (" << vNodeNames.back()
            << ") did not have zero capacitance." );
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  count    (--)  number of links found after file parsing
/// @param[in]  fileType (--)  the type of file (rad,pot,htr,etc.) currently being processed
/// @param[in]  xmlFile  (--)  XML file path and name
///
/// @throw    TsParseException if the count is zero
///
/// @details  Checks whether the parsed file contained the expected data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void ThermFileParser::confirmDataFound(int count, const char* fileType, std::string& xmlFile)
{
    std::ostringstream subtype;
    subtype << "no link data found in" << fileType <<",";

    /// - If no links were found, the wrong file was probably read.
    TS_PTCS_IF_ERREX(count <= 0, TsParseException, subtype.str(), xmlFile);
}
