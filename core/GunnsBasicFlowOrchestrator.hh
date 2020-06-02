#ifndef GunnsBasicFlowOrchestrator_EXISTS
#define GunnsBasicFlowOrchestrator_EXISTS

/**
@file
@brief     GUNNS Basic Flow Orchestrator declarations

@defgroup  TSM_GUNNS_CORE_BASIC_FLOW_ORCH    GUNNS Basic Flow Orchestrator
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Basic Flow Orchestrator.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBasicFlowOrchestrator.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI NSS) (2018-04) (Initial))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"

// Forward-declare pointer types
class GunnsBasicLink;
class GunnsBasicNode;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Flow Orchestrator Class.
///
/// @details  This orchestrates the flow transport between nodes through the links of a GUNNS basic
///           network.  This is updated after the network solution.  This calls the links to compute
///           their flows based on the network solution, and transport flows to and from the nodes.
///           Then this calls the nodes to integrate their total flows and balance their internal
///           state.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicFlowOrchestrator
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicFlowOrchestrator);
    public:
        /// @brief  Default constructor.
        GunnsBasicFlowOrchestrator(const int& numLinks, const int& numNodes);
        /// @brief  Default destructor.
        virtual     ~GunnsBasicFlowOrchestrator();
        /// @brief  Initializes the GUNNS Basic Flow Orchestrator.
        virtual void initialize(const std::string& name, GunnsBasicLink** links, GunnsBasicNode** nodes);
        /// @brief  Updates the flow transport through the links and integration in the nodes.
        virtual void update(const double dt);
        /// @brief  Returns whether this GUNNS Basic Flow Orchestrator has been successfully initialized & validated.
        bool         isInitialized() const;
        //TODO delete when #98 is completed
        void         setVerbose(const bool verbose);

    protected:
        const int&       mNumLinks; /**< ** (--) trick_chkpnt_io(**) Number of links in the network. */
        const int&       mNumNodes; /**< ** (--) trick_chkpnt_io(**) The number of nodes in the netowrk, including Ground. */
        GunnsBasicLink** mLinks;    /**< ** (--) trick_chkpnt_io(**) Array of pointers to the network links. */
        GunnsBasicNode** mNodes;    /**< ** (--) trick_chkpnt_io(**) Array of pointers to the network nodes. */
        std::string      mName;     /**< *o (--) trick_chkpnt_io(**) Instance name for self-identification in messages. */
        bool             mInitFlag; /**< *o (--) trick_chkpnt_io(**) Initialization status flag (T is good). */
        bool             mVerbose;  /**<    (--) TODO delete when #98 is completed */
        /// @brief  Validates the initialization of this GUNNS Basic Flow Orchestrator.
        void         validate();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsBasicFlowOrchestrator(const GunnsBasicFlowOrchestrator& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicFlowOrchestrator& operator =(const GunnsBasicFlowOrchestrator& that);
};

/// @}

//TODO
inline void GunnsBasicFlowOrchestrator::setVerbose(const bool verbose)
{
    mVerbose = verbose;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Returns true if initialization & validation were successful.
///
/// @details  Returns whether the object has been successfully initialized & validated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicFlowOrchestrator::isInitialized() const
{
    return mInitFlag;
}

#endif
