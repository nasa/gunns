#ifndef GunnsFluidIslandAnalyzer_EXISTS
#define GunnsFluidIslandAnalyzer_EXISTS

/**
@file
@brief     GUNNS Fluid Island Analyzer Spotter declarations

@defgroup  TSM_GUNNS_CORE_FLUID_ISLAND_ANALYZER    GUNNS Fluid Island Analyzer Spotter
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Fluid Island Analyzer Spotter.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidIslandAnalyzer.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2014-03) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsNetworkSpotter.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Island Analyzer Spotter Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Island Analyzer Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidIslandAnalyzerConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        /// @brief  Default constructs this GUNNS Fluid Island Analyzer Spotter configuration data.
        GunnsFluidIslandAnalyzerConfigData(const std::string& name);
        /// @brief  Default destructs this GUNNS Fluid Island Analyzer Spotter configuration data.
        virtual ~GunnsFluidIslandAnalyzerConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidIslandAnalyzerConfigData(const GunnsFluidIslandAnalyzerConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidIslandAnalyzerConfigData& operator =(const GunnsFluidIslandAnalyzerConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Island Analyzer Spotter Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Island Analyzer Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidIslandAnalyzerInputData : public GunnsNetworkSpotterInputData
{
    public:
        /// @brief  Default constructs this GUNNS Fluid Island Analyzer Spotter input data.
        GunnsFluidIslandAnalyzerInputData();
        /// @brief  Default destructs this GUNNS Fluid Island Analyzer Spotter input data.
        virtual ~GunnsFluidIslandAnalyzerInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidIslandAnalyzerInputData(const GunnsFluidIslandAnalyzerInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidIslandAnalyzerInputData& operator =(const GunnsFluidIslandAnalyzerInputData& that);
};

// Forward-declare structures used as references.
struct GunnsNodeList;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Island Analyzer Spotter Class.
///
/// @details  This spotter is used to determine properties of the island that a given node belongs
///           to.  An example is finding the lowest pressure node in the island, which can sometimes
///           be used as a leak detection, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidIslandAnalyzer : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidIslandAnalyzer);
    public:
        /// @brief   Default constructor.
        GunnsFluidIslandAnalyzer(GunnsNodeList& nodeList);
        /// @brief   Default destructor.
        virtual     ~GunnsFluidIslandAnalyzer();
        /// @brief   Initializes the GUNNS Fluid Island Analyzer Spotter with configuration and
        ///          input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief   Steps the GUNNS Fluid Island Analyzer Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief   Steps the GUNNS Fluid Island Analyzer Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief   Sets the node number whose island the Spotter will analyze.
        void         setAttachedNode(const int node);
        /// @brief   Gets the node number whose island the Spotter is analyzing.
        int          getAttachedNode() const;
        /// @brief   Gets the number of nodes in the island the Spotter is analyzing.
        int          getIslandSize() const;
        /// @brief   Gets the flags for nodes present in the island the Spotter is analyzing.
        bool*        getIslandNodes() const;
        /// @brief   Gets the total fluid volume of all nodes in the island the Spotter is analyzing.
        double       getIslandVolume() const;
        /// @brief   Gets the total fluid mass of all nodes in the island the Spotter is analyzing.
        double       getIslandMass() const;

    protected:
        GunnsNodeList& mNodeList;              /**< (--)                     Reference to the network node list                     */
        int            mAttachedNode;          /**< (--)                     The network node whose island is analyzed              */
        int            mIslandSize;            /**< (--)                     Number of nodes in the attached island                 */
        bool*          mIslandNodes;           /**< (--) trick_chkpnt_io(**) Flags for nodes present in the island                  */
        double         mIslandVolume;          /**< (m3)                     Total fluid volume of all nodes in the island          */
        double         mIslandMass;            /**< (kg)                     Total fluid mass in all nodes in the island            */
        double*        mIslandConstituentMass; /**< (kg) trick_chkpnt_io(**) Total constituent mass in all nodes in the island      */
        double         mIslandEnergy;          /**< (J)                      Total fluid enthalpy * mass in all nodes in the island */
        double         mHiPressure;            /**< (kPa)                    Highest node pressure in the island                    */
        int            mHiPressureNode;        /**< (--)                     Island node with the highest pressure                  */
        double         mLoPressure;            /**< (kPa)                    Lowest node pressure in the island                     */
        int            mLoPressureNode;        /**< (--)                     Island node with the lowest pressure                   */
        double         mHiTemperature;         /**< (K)                      Highest node temperature in the island                 */
        int            mHiTemperatureNode;     /**< (--)                     Island node with the highest temperature               */
        double         mLoTemperature;         /**< (K)                      Lowest node temperature in the island                  */
        int            mLoTemperatureNode;     /**< (--)                     Island node with the lowest temperature                */
        double*        mHiMoleFraction;        /**< (--) trick_chkpnt_io(**) Highest constituent mole fraction in the island        */
        int*           mHiMoleFractionNode;    /**< (--) trick_chkpnt_io(**) Island node with the highest constituent mole fraction */
        double*        mLoMoleFraction;        /**< (--) trick_chkpnt_io(**) Lowest constituent mole fraction in the island         */
        int*           mLoMoleFractionNode;    /**< (--) trick_chkpnt_io(**) Island node with the lowest constituent mole fraction  */
        double*        mIslandTcMass;          /**< (kg) trick_chkpnt_io(**) Total trace compound mass in all nodes in the island */
        double*        mHiTcMoleFraction;      /**< (--) trick_chkpnt_io(**) Highest trace compound mole fraction in the island        */
        int*           mHiTcMoleFractionNode;  /**< (--) trick_chkpnt_io(**) Island node with the highest trace compound mole fraction */
        double*        mLoTcMoleFraction;      /**< (--) trick_chkpnt_io(**) Lowest trace compound mole fraction in the island         */
        int*           mLoTcMoleFractionNode;  /**< (--) trick_chkpnt_io(**) Island node with the lowest trace compound mole fraction  */
        /// @brief   Validates the supplied configuration data.
        const GunnsFluidIslandAnalyzerConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsFluidIslandAnalyzerInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
        /// @brief   Clears values prior to analyzing the island.
        void resetStateData();
        /// @brief   Performs analysis on the island.
        void analyze();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidIslandAnalyzer(const GunnsFluidIslandAnalyzer& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidIslandAnalyzer& operator =(const GunnsFluidIslandAnalyzer& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node  (--)  The node number whose island is to be analyzed.
///
/// @details No checks are done on validity of the node number argument relative to the number of
///          nodes in the network.  Invalid node numbers simply cause this object to go idle.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidIslandAnalyzer::setAttachedNode(const int node)
{
    mAttachedNode = node;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns int (--) The network node number whose island is being analyzed.
///
/// @details The returned node number is not necessarily within the number of nodes in the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsFluidIslandAnalyzer::getAttachedNode() const
{
    return mAttachedNode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns int (--) The number of nodes in the island the Spotter is analyzing.
///
/// @details The returned value will range from zero to the number nodes in the network, not
///          including the ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsFluidIslandAnalyzer::getIslandSize() const
{
    return mIslandSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns bool* (--) Pointer to an array of flags indicating presence of that node number in the
///                     island being analyzed.
///
/// @details The returned pointer will be null if this object has not yet been initialized.  The
///          pointed-to array is inside this object so won't move during a run.  The array is equal
///          in size to the number of nodes in the network, not including the ground node.  Each
///          index in the array contains a flag that is only true if that node number is currently
///          in the island being analyzed.  The attached node will always indicate true.  If this
///          object is not currently attached to any node, then the entire array will be false.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool* GunnsFluidIslandAnalyzer::getIslandNodes() const
{
    return mIslandNodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (m3) The total fluid volume of all nodes in the island the Spotter is analyzing.
///
/// @details The returned value will be zero if this object is not currently analyzing a valid
///          valid node's island.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidIslandAnalyzer::getIslandVolume() const
{
    return mIslandVolume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (kg) The total fluid mass of all nodes in the island the Spotter is analyzing.
///
/// @details The returned value will be zero if this object is not currently analyzing a valid
///          valid node's island.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidIslandAnalyzer::getIslandMass() const
{
    return mIslandMass;
}

#endif
