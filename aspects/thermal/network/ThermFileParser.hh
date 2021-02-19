#ifndef ThermFileParser_EXISTS
#define ThermFileParser_EXISTS
/************************************** TRICK HEADER ***********************************************
@defgroup TSM_GUNNS_THERMAL_NETWORK_ThermFileParser Thermal links config-file parser
@ingroup  TSM_GUNNS_THERMAL_NETWORK

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (The ThermFileParser is a member of the ThermalNetwork class. In a "default_data" job,
    it is configured with up to six XML files, each containing the data needed each of the
    of the seven thermal Gunns links.
          - node-file: (required) Nodes/Capacitance links, given by <node> tags
          - cond-file: Conduction links, <conduction>
          - rad-file:  Radiation links, <radiation>
          - htr-file:  Heater links, <heater>
          - pan-file:  ThermalPanel links, <panel>
          - etc-file:  et.cetera; Other link types, namely <potential> and <source>
    ThermalNetwork calls the ThermFileParser's initialize() method, which parses each file's
    specific XML tag structure, and stores the relevant data into its link-specific vectors.
    ThermalNetwork accesses these vectors and uses their data to construct a GUNNS network.)

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   (This class utilizes tinyxml, hence is limited to take in only XML files.)

LIBRARY DEPENDENCY:
   (ThermFileParser.o)

PROGRAMMERS:
   ((Joe Valerioti) (L3) (Dec 2012)))
@{
***************************************************************************************************/
#include "parsing/tinyxml/tinyxml.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <string>
#include <vector>
#include <map>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Parses the ThermalNetwork config-files and stores the data into vectors.
///
/// @details  The ThermFileParser class is only used as a member of the ThermalNetwork
///           class. It is ThermalNetwork's tool for parsing the six different thermal XML
///           configuration files (node, cond, rad, htr, pan, etc). ThermalNetwork calls the
///           initialize() method, which stores the thermal data from these files into vectors.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ThermFileParser
{
    TS_MAKE_SIM_COMPATIBLE(ThermFileParser);

    /// - ThermalNetwork needs access to ThermFileParser's data vectors.
    friend class ThermalNetwork;

    public:
        /// @brief  Default constructor for the ThermFileParser xml reader.
        ThermFileParser(const std::string name = "");
        /// @brief  Default destructor for the ThermFileParser xml reader.
        virtual ~ThermFileParser();

    protected:
        static const int                   NOT_FOUND;                /**<    (--)  trick_chkpnt_io(**) result for a node not registered in the Node map */
        bool                               areNodesRegistered;       /**<    (--)                      True if Nodes file successfully parsed. */
        std::string                        mName;                    /**<    (--)  trick_chkpnt_io(**) Parser name. */

        // File names /////////////////////////////////////////////////////////////////////////////////
        std::string                        mNodeFile;                /**<    (--)  trick_chkpnt_io(**) xml file with node & capacitance link data */
        std::string                        mCondFile;                /**<    (--)  trick_chkpnt_io(**) xml file with conduction link data */
        std::string                        mRadFile;                 /**<    (--)  trick_chkpnt_io(**) xml file with radiation link data */
        std::string                        mHtrFile;                 /**<    (--)  trick_chkpnt_io(**) xml file with heater link data */
        std::string                        mPanFile;                 /**<    (--)  trick_chkpnt_io(**) xml file with thermal-panel data */
        std::string                        mEtcFile;                 /**<    (--)  trick_chkpnt_io(**) xml file with other thermal link data */

        std::string                        mThermInputFile;          /**<    (--)  trick_chkpnt_io(**) name of thermal input-file to parse */
        std::string                        mThermInputFileRad;       /**<    (--)  trick_chkpnt_io(**) name of radiation thermal input-file to parse */

        // Node attributes ////////////////////////////////////////////////////////////////////////////
        int                                numNodes;                 /**<    (--)                      number of nodes in the network */
        std::vector<std::string>           vNodeNames;               /**<    (--)                      vector of node name strings */
        std::map<const std::string, int>   mNodeMap;                 /**<    (--)                      Node Map. key: node name, value: port number.*/
        std::vector<std::string>           vCapEditGroupList;        /**< ** (--)                      vector of non-repeating cap-edit group names*/

        // Capacitance Link attributes ////////////////////////////////////////////////////////////////
        int                                numLinksCap;              /**<    (--)                      number of capacitance links in the network */
        std::vector<std::string>           vCapNames;                /**< ** (--)                      vector of cap-link names */
        std::vector<int>                   vCapPorts;                /**< ** (--)                      vector of cap-link port numbers */
        std::vector<double>                vCapTemperatures;         /**< ** (K)                       vector of cap-link temperature values */
        std::vector<double>                vCapCapacitances;         /**< ** (J/K)                     vector of cap-link capacitance values */
        std::vector<int>                   vCapEditGroupIdentifiers; /**< ** (--)                      vector of cap-link edit group identifiers */

        // Conduction Link attributes //////////////////////////////////////////////////////////////////////////////
        int                                numLinksCond;             /**<    (--)                      number of conduction links in the network */
        std::vector<std::string>           vCondNames;               /**< ** (--)                      vector of cond-link names */
        std::vector<int>                   vCondPorts0;              /**< ** (--)                      vector of cond-link port0 nodes */
        std::vector<int>                   vCondPorts1;              /**< ** (--)                      vector of cond-link port1 nodes */
        std::vector<double>                vCondConductivities;      /**< ** (W/K)                     vector of cond-link conductivities */

        // Radiation Link attributes ///////////////////////////////////////////////////////////////////////////////
        int                                numLinksRad;              /**<    (--)                      number of radiation links in the network */
        int                                numInputEntriesRad;       /**<    (--)                      number of radiation link Entries */
        std::vector<std::string>           vRadNames;                /**< ** (--)                      vector of rad-link names */
        std::vector<int>                   vRadPorts0;               /**< ** (--)                      vector of rad-link port0 numbers */
        std::vector<int>                   vRadPorts1;               /**< ** (--)                      vector of rad-link port1 numbers */
        std::vector<double>                vRadCoefficients;         /**< ** (m2)                      vector of radiation coefficients */
        std::vector<double>                vRadViewScalars;          /**< ** (--)                      vector of view scalar values */

        // Heater Link attributes //////////////////////////////////////////////////////////////////////////////////
        int                                numLinksHtr;              /**<    (--)                      number of heater links in the network */
        std::vector<std::string>           vHtrNames;                /**< ** (--)                      vector of heater-link names */
        std::vector<double>                vHtrScalars;              /**< ** (--)                      vector of heater elec-to-thermal tuning scalars */
        std::vector< std::vector<int> >    vHtrPorts;                /**< ** (--)                      vector of htr port number vectors */
        std::vector< std::vector<double> > vHtrFracs;                /**< ** (--)                      vector of htr powerdraw-fraction vectors */

        // ThermalPanel attributes /////////////////////////////////////////////////////////////////////////////////
        int                                numLinksPan;              /**<    (--)                      number of panel links in the network */
        std::vector<std::string>           vPanNames;                /**< ** (--)                      vector of panel names */
        std::vector<double>                vPanAreas;                /**< ** (m2)                      vector of panel surface area */
        std::vector<double>                vPanAbsorptivities;       /**< ** (--)                      vector of panel absorptivities */
        std::vector< std::vector<int> >    vPanPorts;                /**< ** (--)                      vector of panel port number vectors */
        std::vector< std::vector<double> > vPanFracs;                /**< ** (--)                      vector of panel flux-application-fraction vectors */

        // Potential Link attributes ///////////////////////////////////////////////////////////////////////////////
        int                                numLinksPot;              /**<    (--)                      number of potential links in the network */
        std::vector<std::string>           vPotNames;                /**< ** (--)                      vector of pot-link names */
        std::vector<int>                   vPotPorts;                /**< ** (--)                      vector of pot-link port numbers */
        std::vector<double>                vPotTemperatures;         /**< ** (K)                       vector of pot-link temperature values */
        std::vector<double>                vPotConductivities;       /**< ** (W/K)                     vector of pot-link conductivity values */

        // Source Link attributes //////////////////////////////////////////////////////////////////////////////////
        int                                numLinksSrc;              /**<    (--)                      number of source links in the network */
        std::vector<std::string>           vSrcNames;                /**< ** (--)                      vector of source-link names */
        std::vector<double>                vSrcInits;                /**< ** (W)                       vector of source-link flux values */
        std::vector<double>                vSrcScalars;              /**< ** (--)                      vector of source-link tuning scalars */
        std::vector< std::vector<int> >    vSrcPorts;                /**< ** (--)                      vector of src port number vectors */
        std::vector< std::vector<double> > vSrcFracs;                /**< ** (--)                      vector of src flux-application-fraction vectors */

        /// @brief  Parses the node xml file and counts the number of node elements in it.
        void preCountNodes();
        /// @brief  Parses thermal xml files and builds data vectors. Only ThermalNetwork may call this
        ///         method, so it is protected (ThermalNetwork is a friend).
        void initialize(const std::string& name);

        /// @brief  Read Node/CapLink config-file.
        void readNodeFile();
        /// @brief  Read CondLink config-file.
        void readCondFile();
        /// @brief  Read RadLink config-file.
        void readRadFile();
        /// @brief  Read Htr config-file.
        void readHtrFile();
        /// @brief  Read Panel config-file.
        void readPanFile();
        /// @brief  Read Other Link config-file.
        void readEtcFile();

        /// @brief  Edits existing link data by reading overrides described in the ThermInput file.
        void readThermInputFile();
        /// @brief  Edits existing link data by reading overrides described in the ThermInput file.
        void readThermInputFileRad();

        /// @brief  Private method used to generically ready any of the thermal XML files.
        void readFile(std::string& xmlFile, const char* linkTag, void (ThermFileParser::*registerLink)(TiXmlElement*));
        /// @brief  Private method used to validate file accessibility and set doc member.
        void openFile(TiXmlDocument& doc, std::string& xmlFile);

        /// @brief  Clears Node data vectors.
        void clearNode();
        /// @brief  Clears Capacitance link data vectors.
        void clearCap();
        /// @brief  Clears Conduction link data vectors.
        void clearCond();
        /// @brief  Clear Radiation link data vectors.
        void clearRad();
        /// @brief  Clears Heater data vectors.
        void clearHtr();
        /// @brief  Clears ThermalPanel data vectors.
        void clearPan();
        /// @brief  Clears Potential link data vectors.
        void clearPot();
        /// @brief  Clears Source link data vectors.
        void clearSrc();

        /// @brief  Populates vector of cap-edit group names.
        void registerCapEditGroups(TiXmlElement* capEditing);
        /// @brief  Counts the nodes.
        void countNode(TiXmlElement* node);
        /// @brief  Populates Node & Capacitance link data vectors.
        void registerNode(TiXmlElement* node);
        /// @brief  Populates Conduction link data vectors.
        void registerCond(TiXmlElement* conduction);
        /// @brief  Populates Radiation link data vectors.
        void registerRad(TiXmlElement* radiation);
        /// @brief  Populates Heater link data vectors.
        void registerHtr(TiXmlElement* heater);
        /// @brief  Populates Panel link data vectors.
        void registerPan(TiXmlElement* panel);
        /// @brief  Populates Potential link data vectors.
        void registerPot(TiXmlElement* potential);
        /// @brief  Populates Source link data vectors.
        void registerSrc(TiXmlElement* source);

        /// @brief  Populates Source link data vectors.
        void registerInput(TiXmlElement* node);
        /// @brief  Populates Radiation link data vectors.
        void registerInputRad(TiXmlElement* radiation);

        /// @brief  Private method used to get the name attribute of a TinyXML element.
        const std::string getName(TiXmlElement* parent, const char* linkType);
        /// @brief  Private method used to get and verify the text of TinyXML element.
        const char* getText(TiXmlElement* parent, const char* tag,
                const bool raiseErrorIfNotFound = true);
        /// @brief  Private method used to check the existence of and return an element.
        TiXmlElement* getElement(TiXmlNode* parent, const char* tag,
                const bool raiseErrorIfNotFound = true);
        /// @brief  Private method used to count elements.
        int countElement(TiXmlNode* parent, const char* tag);
        /// @brief  Get cap-edit group identifier based on editGroup name provided in XML.
        int getCapEditGroupId(TiXmlElement* node);

        /// @brief  Builds ports and fractions vectors for multi-port links.
        void buildMultiPortVectors(
                std::vector<int>& vectorInt,
                std::vector<double>& vectorDouble,
                TiXmlNode* parent,
                const std::string& linkType,
                const std::string& name);

        /// @brief  Verifies that a Node name provided in a config-file is valid.
        int validateNode(const std::string& nodeString);
        /// @brief  Returns the index location of the node name, or NOT_FOUND if not registered.
        int getMapLocation(const std::string& nodeName);
        /// @brief  Checks whether the space/ground/vacuum node is given at the end of the node list.
        void confirmNodeBuild();
        /// @brief  Checks whether the parsed file contained the expected data.
        void confirmDataFound(int count, const char* info, std::string& xmlFile);

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ThermFileParser(const ThermFileParser&);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ThermFileParser& operator=(const ThermFileParser&);
};
/// @}

#endif
