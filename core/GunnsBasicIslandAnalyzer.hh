#ifndef GunnsBasicIslandAnalyzer_EXISTS
#define GunnsBasicIslandAnalyzer_EXISTS

/**
@file
@brief     GUNNS Basic Island Analyzer Spotter declarations

@defgroup  TSM_GUNNS_CORE_BASIC_ISLAND_ANALYZER    GUNNS Basic Island Analyzer Spotter
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Basic Island Analyzer Spotter.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBasicIslandAnalyzer.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2014-03) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsNetworkSpotter.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Island Analyzer Spotter Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Basic
///           Island Analyzer Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicIslandAnalyzerConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        /// @brief  Default constructs this GUNNS Basic Island Analyzer Spotter configuration data.
        GunnsBasicIslandAnalyzerConfigData(const std::string& name);
        /// @brief  Default destructs this GUNNS Basic Island Analyzer Spotter configuration data.
        virtual ~GunnsBasicIslandAnalyzerConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsBasicIslandAnalyzerConfigData(const GunnsBasicIslandAnalyzerConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicIslandAnalyzerConfigData& operator =(const GunnsBasicIslandAnalyzerConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Island Analyzer Spotter Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Basic
///           Island Analyzer Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicIslandAnalyzerInputData : public GunnsNetworkSpotterInputData
{
    public:
        /// @brief  Default constructs this GUNNS Basic Island Analyzer Spotter input data.
        GunnsBasicIslandAnalyzerInputData();
        /// @brief  Default destructs this GUNNS Basic Island Analyzer Spotter input data.
        virtual ~GunnsBasicIslandAnalyzerInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsBasicIslandAnalyzerInputData(const GunnsBasicIslandAnalyzerInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicIslandAnalyzerInputData& operator =(const GunnsBasicIslandAnalyzerInputData& that);
};

// Forward-declare structures used as references.
struct GunnsNodeList;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Island Analyzer Spotter Class.
///
/// @details  This spotter is used to determine properties of the island that a given node belongs
///           to.  An example is finding the lowest Potential node in the island, which can sometimes
///           be used as a leak detection, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicIslandAnalyzer : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicIslandAnalyzer);
    public:
        /// @brief   Default constructor.
        GunnsBasicIslandAnalyzer(GunnsNodeList& nodeList);
        /// @brief   Default destructor.
        virtual     ~GunnsBasicIslandAnalyzer();
        /// @brief   Initializes the GUNNS Basic Island Analyzer Spotter with configuration and
        ///          input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief   Steps the GUNNS Basic Island Analyzer Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief   Steps the GUNNS Basic Island Analyzer Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief   Sets the node number whose island the Spotter will analyze.
        void         setAttachedNode(const int node);
        /// @brief   Gets the node number whose island the Spotter is analyzing.
        int          getAttachedNode() const;
        /// @brief   Gets the number of nodes in the island the Spotter is analyzing.
        int          getIslandSize() const;
        /// @brief   Gets the flags for nodes present in the island the Spotter is analyzing.
        bool*        getIslandNodes() const;
        /// @brief   Gets the total net flux of all nodes in the island the Spotter is analyzing.
        double       getIslandNetFlux() const;

    protected:
        GunnsNodeList& mNodeList;        /**< (--)                     Reference to the network node list         */
        int            mAttachedNode;    /**< (--)                     The network node whose island is analyzed  */
        int            mIslandSize;      /**< (--)                     Number of nodes in the attached island     */
        bool*          mIslandNodes;     /**< (--) trick_chkpnt_io(**) Flags for nodes present in the island      */
        double         mIslandNetFlux;   /**< (--)                     Sum of net flux in all nodes in the island */
        double         mHiPotential;     /**< (--)                     Highest node potential in the island       */
        int            mHiPotentialNode; /**< (--)                     Island node with the highest potential     */
        double         mLoPotential;     /**< (--)                     Lowest node potential in the island        */
        int            mLoPotentialNode; /**< (--)                     Island node with the lowest potential      */
        /// @brief   Validates the supplied configuration data.
        const GunnsBasicIslandAnalyzerConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsBasicIslandAnalyzerInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
        /// @brief   Clears values prior to analyzing the island.
        void resetStateData();
        /// @brief   Performs analysis on the island.
        void analyze();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsBasicIslandAnalyzer(const GunnsBasicIslandAnalyzer& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicIslandAnalyzer& operator =(const GunnsBasicIslandAnalyzer& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node  (--)  The node number whose island is to be analyzed.
///
/// @details No checks are done on validity of the node number argument relative to the number of
///          nodes in the network.  Invalid node numbers simply cause this object to go idle.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicIslandAnalyzer::setAttachedNode(const int node)
{
    mAttachedNode = node;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns int (--) The network node number whose island is being analyzed.
///
/// @details The returned node number is not necessarily within the number of nodes in the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsBasicIslandAnalyzer::getAttachedNode() const
{
    return mAttachedNode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns int (--) The number of nodes in the island the Spotter is analyzing.
///
/// @details The returned value will range from zero to the number nodes in the network, not
///          including the ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsBasicIslandAnalyzer::getIslandSize() const
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
inline bool* GunnsBasicIslandAnalyzer::getIslandNodes() const
{
    return mIslandNodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (--) The total net flux of all nodes in the island the Spotter is analyzing.
///
/// @details The returned value will be zero if this object is not currently analyzing a valid
///          valid node's island.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicIslandAnalyzer::getIslandNetFlux() const
{
    return mIslandNetFlux;
}

#endif
