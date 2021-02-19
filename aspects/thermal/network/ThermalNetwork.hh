#ifndef ThermalNetwork_EXISTS
#define ThermalNetwork_EXISTS
/************************************** TRICK HEADER ***********************************************
@defgroup TSM_GUNNS_THERMAL_NETWORK ThermalNetwork generic orchestrator
@ingroup  TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (This class provides a generic orchestrator for a PTCS thermal network of any vehicle. Every
    Gunns thermal network will be an instantation of this class. Each is configured
    with a unique set of XML configuration files, set in the ThermFileParser member
    during a default_data job and read during initialize(). The ThermFileParser stores the
    config-file data into vectors, which are read by the ThermalNetwork class.
    ThermalNetwork allocates arrays of the seven different thermal link-types, then builds
    Gunns ConfigData and InputData class from the data.
    It then calls initialize() on all links, loading their Config/Input data
    and connecting them to their respective nodes in the network.)

REQUIREMENTS:
   (R.TS228-0023: Spacecraft thermal models shall simulate temperatures of passively
                cooled components.
    R.TS228-0024: Spacecraft thermal models shall provide thermal data to passively cooled
                components.
    R.TS228-0025: Spacecraft thermal models shall receive thermal data from passively
                cooled components.
    R.TS228-0026: Spacecraft thermal models shall provide thermal data to thermally
                protected components.)

REFERENCE:
    ()

ASSUMPTIONS AND LIMITATIONS:
    (The paths of the config-files must be given relative to the location of the actual
    executable, usually in the same directory as the S_define.)

LIBRARY DEPENDENCY:
    ((ThermalNetwork.o))

PROGRAMMERS:
    (
    (Joe Valerioti) (L3) (Apr 2011) (Initial Prototype)
    (Mike Moore)    (L3) (May 2011) (added Thermal Desktop parsing functionality)
    (Joe Valerioti) (L3) (Jun 2011) (testing, re-factoring)
                         (Feb 2012) (updated to read new ThermalPanel file)
                         (Mar 2012) (enhance error handling and reduce duplication)
                         (Dec 2012) (convert to XML parsing)
    )
@{
***************************************************************************************************/
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/thermal/network/ThermFileParser.hh"
#include "core/Gunns.hh"
#include "core/GunnsBasicNode.hh"
#include "aspects/thermal/GunnsThermalCapacitor.hh"
#include "core/GunnsBasicConductor.hh"
#include "aspects/thermal/GunnsThermalRadiation.hh"
#include "aspects/thermal/GunnsThermalHeater.hh"
#include "aspects/thermal/GunnsThermalPanel.hh"
#include "aspects/thermal/GunnsThermalPotential.hh"
#include "aspects/thermal/GunnsThermalSource.hh"
#include "core/GunnsBasicIslandAnalyzer.hh"
#include "core/network/GunnsNetworkBase.hh"
#include <string>
#include <vector>
class ThermalNetworkWingMan;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ThermalNetwork configuration data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           ThermalNetwork configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ThermalNetworkConfigData
{
    public:
        std::string  cNodeFile; /**< (--) trick_chkpnt_io(**) name of xml file with node & capacitance link data */
        std::string  cCondFile; /**< (--) trick_chkpnt_io(**) name of xml file with conduction link data */
        std::string  cRadFile;  /**< (--) trick_chkpnt_io(**) name of xml file with radiation link data */
        std::string  cHtrFile;  /**< (--) trick_chkpnt_io(**) name of xml file with heater/source link data */
        std::string  cPanFile;  /**< (--) trick_chkpnt_io(**) name of xml file with thermal-panel data */
        std::string  cEtcFile;  /**< (--) trick_chkpnt_io(**) name of xml file with other thermal link data */

        /// @brief   Default constructs this ThermalNetwork configuration data.
        ThermalNetworkConfigData(std::string nodeFile = "",
                                 std::string condFile = "",
                                 std::string radFile = "",
                                 std::string htrFile = "",
                                 std::string panFile = "",
                                 std::string etcFile = "");
        /// @brief   Default destructs this ThermalNetwork configuration data.
        ~ThermalNetworkConfigData();
        /// @brief   Copy constructs this ThermalNetwork configuration data.
        ThermalNetworkConfigData(const ThermalNetworkConfigData& that);

    private:
        /// @brief   Assignment operator unavailable since declared private and not implemented.
        ThermalNetworkConfigData& operator=(const ThermalNetworkConfigData&);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ThermalNetwork input data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           ThermalNetwork input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ThermalNetworkInputData
{
    public:
        std::string iThermInputFile; /**< (--) trick_chkpnt_io(**) name of xml file with thermal input override data */
        std::string iInputRadFile;   /**< (--) trick_chkpnt_io(**) name of xml file with radiation link input override data */

        /// @brief   Default constructs this ThermalNetwork configuration data.
        ThermalNetworkInputData(std::string thermInputFile = "",
                                std::string radInputFile = "");
        /// @brief   Default destructs this ThermalNetwork configuration data.
        ~ThermalNetworkInputData();
        /// @brief   Copy constructs this ThermalNetwork configuration data.
        ThermalNetworkInputData(const ThermalNetworkInputData& that);

    private:
        /// @brief   Assignment operator unavailable since declared private and not implemented.
        ThermalNetworkInputData& operator=(const ThermalNetworkInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Generic orchestrator for GUNNS thermal networks.
///
/// @details  The ThermalNetwork class serves as an orchestrator for any generic PTCS network.
///           Its purpose is to build and initialize a network of thermal GUNNS links.
///           Data on these links is read from XML files by the ThermFileParser member.
///           There are seven different thermal link-types: Capacitance, Conduction, Radiation,
///           Heater, Panel, Potential, and Source. Thermal networks are composed entirely of these
///           seven link-types. The links are dynamically-allocated into arrays, and therefore the
///           class can support any number of each link-type. ThermalNetwork initializes the links
///           with their Config/Input data and connects them to their respective nodes in the
///           GUNNS network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ThermalNetwork : public GunnsNetworkBase
{
    TS_MAKE_SIM_COMPATIBLE(ThermalNetwork);

    public:
        friend class ThermalNetworkWingMan;

        /// @details  Declared public to allow default data and python script access.
        ThermalNetworkConfigData mConfig;         /**< ** (--) trick_chkpnt_io(**) network configuration file names */
        /// @details  Declared public to allow default data and python script access.
        ThermalNetworkInputData mInput;           /**< ** (--) trick_chkpnt_io(**) network input file names */

        /// @name    Thermal link arrays.
        /// @{
        /// @details These pointers are new'd in initialize() to point to arrays of
        ///          one of the seven thermal link-types.
        GunnsThermalCapacitor* mCapacitanceLinks; /**<    (--) trick_chkpnt_io(**) GunnsThermalCapacitor object array */
        GunnsBasicConductor*   mConductionLinks;  /**<    (--) trick_chkpnt_io(**) GunnsBasicConductor object array   */
        GunnsThermalRadiation* mRadiationLinks;   /**<    (--) trick_chkpnt_io(**) GunnsThermalRadiation object array */
        GunnsThermalHeater*    mHeaters;          /**<    (--) trick_chkpnt_io(**) GunnsThermalHeater object array    */
        GunnsThermalPanel*     mPanels;           /**<    (--) trick_chkpnt_io(**) GunnsThermalPanel object array     */
        GunnsThermalPotential* mPotentialLinks;   /**<    (--) trick_chkpnt_io(**) GunnsThermalPotential object array */
        GunnsThermalSource*    mSources;          /**<    (--) trick_chkpnt_io(**) GunnsThermalSource object array    */
        /// @}

        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool mMalfHtrMiswireFlag;                 /**<    (--)                     flag to trip heater miswire malfunction */
        int* mMalfHtrIndexValue;                  /**<    (--) trick_chkpnt_io(**) array to cross up heater indices */
        /// @}

        /// @brief  Constructs the object with the name of the network as an argument.
        ThermalNetwork(const std::string& name = "");
        /// @brief  Default destructs this ThermalNetwork.
        virtual ~ThermalNetwork();

        /// @brief  Returns the number of nodes defined in this network.
        virtual int  getNumLocalNodes();
        /// @brief  Parses config files, initializes the nodes and allocates memory for links.
        virtual void initNodes(const std::string& name);
        /// @brief  Initializes the links, spotter and solver.
        virtual void initNetwork();
        /// @brief  Simulation preload checkpoint task.
        virtual void preloadCheckpoint();
        /// @brief  Simulation restart task.
        virtual void restart();
        /// @brief  Update network spotters before the solver solution.
        virtual void stepSpottersPre(const double timeStep);
        /// @brief  Update network spotters after the solver solution.
        virtual void stepSpottersPost(const double timeStep);
        /// @brief Sets the GUNNS solver to NORMAL mode.
        void setNormalMode();
        /// @brief Sets the GUNNS solver to DUMMY mode.
        void setDummyMode();
        /// @brief Sets the GUNNS solver to SLAVE mode.
        void setSlaveMode();
        /// @brief Sets the GUNNS solver Island mode to the given value.
        void setIslandMode(const Gunns::IslandMode mode);
        /// @brief Sets and resets the heater miswire malfunction.
        void setMalfHtrMiswire(const bool flag = false, const int* index = 0);

   protected:
        /// @details  The mHtrPowerElectrical array will be set by the simbus with values from EPS.
        double* mHtrPowerElectrical;         /**<    (W)  trick_chkpnt_io(**) array of power values to set to heaters */

        ThermFileParser parser;              /**< ** (--) trick_chkpnt_io(**) Parser object with methods to read XML config-files;
                                                                              Stores GUNNS link configuration data into
                                                                              vectors that ThermalNetwork can access.*/

        GunnsBasicIslandAnalyzer netIslandAnalyzer; /**< (--)                 Island analyzer for debugging */

        /// - Capacitance Edit Groups
        ////////////////////////////////////////////////////////////////////////////////////////////
        int     numCapEditGroups;            /**< *o (--) trick_chkpnt_io(**) Number of capacitor link capacitance edit groups    */
        double* mCapEditScaleFactor;         /**<    (--) trick_chkpnt_io(**) Capacitance edit scale factor control by edit group */
        double* mCapEditScalePrev;           /**<    (--) trick_chkpnt_io(**) Previous capacitance edit scale factor control      */

        /// - GUNNS core network objects
        /////////////////////////////////////////////////////////////////////////////////////////////
        GunnsBasicNode* pNodes;              /**<    (--) trick_chkpnt_io(**) array of nodes for this network */
        int indexSpaceNode;                  /**< *o (--) trick_chkpnt_io(**) index of the Gunns space/ground/vacuum node */

        /// - Counts of the different link-types
        /////////////////////////////////////////////////////////////////////////////////////////////
        int numLinksCap;                     /**< *o (--) trick_chkpnt_io(**) number of GunnsThermalCapacitor objects */
        int numLinksCond;                    /**< *o (--) trick_chkpnt_io(**) number of GunnsBasicConductor objects   */
        int numLinksRad;                     /**< *o (--) trick_chkpnt_io(**) number of GunnsThermalRadiation objects */
        int numLinksHtr;                     /**< *o (--) trick_chkpnt_io(**) number of GunnsThermalHeater objects    */
        int numLinksPan;                     /**< *o (--) trick_chkpnt_io(**) number of GunnsThermalPanel objects     */
        int numLinksPot;                     /**< *o (--) trick_chkpnt_io(**) number of GunnsThermalPotential objects */
        int numLinksSrc;                     /**< *o (--) trick_chkpnt_io(**) number of GunnsThermalSource objects    */

        /// - Pointers to thermal link config data arrays. These pointers are new'd in initialize()
        ///   to point to arrays of one of the seven thermal link ConfigData objects.
        /////////////////////////////////////////////////////////////////////////////////////////////
        GunnsThermalCapacitorConfigData** mCapacitanceConfigData;/**< ** (--) trick_chkpnt_io(**) Capacitor ConfigData array */
        GunnsBasicConductorConfigData**   mConductionConfigData; /**< ** (--) trick_chkpnt_io(**) Conductor ConfigData array */
        GunnsThermalRadiationConfigData** mRadiationConfigData;  /**< ** (--) trick_chkpnt_io(**) Radiation ConfigData array */
        GunnsThermalHeaterConfigData**    mHeaterConfigData;     /**< ** (--) trick_chkpnt_io(**) Heater ConfigData array    */
        GunnsThermalPanelConfigData**     mPanelConfigData;      /**< ** (--) trick_chkpnt_io(**) Panel ConfigData array     */
        GunnsThermalPotentialConfigData** mPotentialConfigData;  /**< ** (--) trick_chkpnt_io(**) Potential ConfigData array */
        GunnsThermalSourceConfigData**    mSourceConfigData;     /**< ** (--) trick_chkpnt_io(**) Source ConfigData array    */

        /// - Pointers to thermal link input data arrays. These pointers are new'd in initialize()
        ///   to point to arrays of one of the seven thermal link InputData objects.
        /////////////////////////////////////////////////////////////////////////////////////////////
        GunnsThermalCapacitorInputData** mCapacitanceInputData;  /**< ** (--) trick_chkpnt_io(**) Capacitor InputData array */
        GunnsBasicConductorInputData**   mConductionInputData;   /**< ** (--) trick_chkpnt_io(**) Conductor InputData array */
        GunnsThermalRadiationInputData** mRadiationInputData;    /**< ** (--) trick_chkpnt_io(**) Radiation InputData array */
        GunnsThermalHeaterInputData**    mHeaterInputData;       /**< ** (--) trick_chkpnt_io(**) Heater InputData array    */
        GunnsThermalPanelInputData**     mPanelInputData;        /**< ** (--) trick_chkpnt_io(**) Panel InputData array     */
        GunnsThermalPotentialInputData** mPotentialInputData;    /**< ** (--) trick_chkpnt_io(**) Potential InputData array */
        GunnsThermalSourceInputData**    mSourceInputData;       /**< ** (--) trick_chkpnt_io(**) Source InputData array    */

        /// @brief  Allocates and initializes the network's nodes objects.
        void buildNodes();
        /// @brief  Allocates arrays for each link-object type.
        void allocate();
        /// @brief  Validates this Thermal Network initial state.
        void validate();
        /// @brief  Builds config/input data arrays for links of each type.
        void buildConfig(const std::string& networkName);
        /// @brief  Initializes arrays for links of each type.
        void buildLinks();
        /// @brief  Uses a function pointer to call the initialize sub-function for each link type
        void buildLinkType(const char* linkType, const int total, void (ThermalNetwork::*pf) (int) );

        /// @brief  Sub-function to initialize a specific Capacitance link
        void initializeCap(int i);
        /// @brief  Sub-function to initialize a specific Conduction link
        void initializeCond(int i);
        /// @brief  Sub-function to initialize a specific Radiation link
        void initializeRad(int i);
        /// @brief  Sub-function to initialize a specific Heater link
        void initializeHtr(int i);
        /// @brief  Sub-function to initialize a specific Panel link
        void initializePan(int i);
        /// @brief  Sub-function to initialize a specific Potential link
        void initializePot(int i);
        /// @brief  Sub-function to initialize a specific Source link
        void initializeSrc(int i);

        /// @brief  Loops through each heater, setting its electrical power.
        void setHeaterPowers();
        /// @brief  Edits link capacitances by groups.
        void editCapacitanceGroups();
        /// @brief  Reverts link capacitances to default values prior to loading checkpoint.
        void preloadCapacitanceGroups();
        /// @brief  Applies link capacitance scales after loading checkpoint.
        void restartCapacitanceGroups();
        /// @brief  Applies link capacitance scales.
        void applyCapacitanceGroups(int group, double ratio);
        /// @brief  Deletes allocated arrays and config/input objects.
        void cleanUp();

   private:
      /// @details  Copy constructor unavailable since declared private and not implemented.
      ThermalNetwork(const ThermalNetwork&);
      /// @details  Assignment operator unavailable since declared private and not implemented.
      ThermalNetwork& operator=(const ThermalNetwork&);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the network solver mode to NORMAL.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void ThermalNetwork::setNormalMode()
{
    netSolver.setNormalMode();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the network solver mode to DUMMY.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void ThermalNetwork::setDummyMode()
{
    netSolver.setDummyMode();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the network solver mode to SLAVE.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void ThermalNetwork::setSlaveMode()
{
    netSolver.setSlaveMode();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method sets the network solver Island mode to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void ThermalNetwork::setIslandMode(const Gunns::IslandMode mode)
{
    netSolver.setIslandMode(mode);
}

#endif // ThermalNetwork_EXISTS
