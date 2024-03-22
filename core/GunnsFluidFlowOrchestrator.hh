#ifndef GunnsFluidFlowOrchestrator_EXISTS
#define GunnsFluidFlowOrchestrator_EXISTS

/**
@file
@brief     GUNNS Fluid Flow Orchestrator declarations

@defgroup  TSM_GUNNS_CORE_FLUID_FLOW_ORCH    GUNNS Fluid Flow Orchestrator
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Fluid Flow Orchestrator.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- ((Design limitation: closed loops of all overflowing nodes do not conserve mass & energy.)
   (Design limitation: conservation errors can occur from a link that flows both to & from an
   overflowing node with multiple flow paths.))

LIBRARY DEPENDENCY:
- ((GunnsFluidFlowOrchestrator.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI NSS) (2018-04) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicFlowOrchestrator.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Flow Orchestrator Class.
///
/// @details  This orchestrates the flow transport between nodes through the links of a GUNNS fluid
///           network.  This is similar to basic networks except that in fluid networks, we must
///           update the links and nodes in the optimal order to help conserve mass & energy when
///           nodes are 'overflowing'.
///
///           A node is overflowing then there is more mass flow out of the node in a time step than
///           the amount of mass that was in the node at the start of the time step.  All of the
///           node's original mass is flowing out and being replaced by incoming flows, and some of
///           that incoming flow also flows through and out.  If a link transports its flows from
///           the node before the node's inflows and final state are known, then the fluid state
///           transported by the link is incorrect because it doesn't include the properties
///           (mixture and temperature) of the overflow fluid from the node's input.  This doesn't
///           conserve mass and energy.
///
///           We avoid the above problem by updating overflowing nodes before their outflow links
///           transport their flows.  To do so, we first have to update their inflow links, but then
///           those links must have their source nodes updated prior.  Some "chicken and egg" cases
///           occur where we can't determine an ultimate source node.  In these cases, we have to
///           force a link to transport before its source nodes are complete.  These cases are
///           described as design limitations in the Assumptions & Limitations, but all could be
///           avoided by proper network setup.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFlowOrchestrator : public GunnsBasicFlowOrchestrator
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidFlowOrchestrator);
    public:
        /// @brief  Enumeration of node completion and overflow states.
        enum NodeStates {
            INCOMPLETE  = 0,                                ///< Incomplete and not overflowing.
            OVERFLOWING = 1,                                ///< Incomplete and overflowing.
            COMPLETE    = 2                                 ///< Complete.
        };
        /// @brief  Default constructor.
        GunnsFluidFlowOrchestrator(const int& numLinks, const int& numNodes);
        /// @brief  Default destructor.
        virtual     ~GunnsFluidFlowOrchestrator();
        /// @brief  Initializes the GUNNS Fluid Flow Orchestrator.
        virtual void initialize(const std::string& name, GunnsBasicLink** links, GunnsBasicNode** nodes,
                                int** linkNodeMaps, int* linkNumPorts);
        /// @brief  Updates the flow transport through the links and integration in the nodes.
        virtual void update(const double dt);

    protected:
        bool*       mLinkStates;                            /**< (1) Completion state of links. */
        NodeStates* mNodeStates;                            /**< (1) Completion state of nodes. */
        int         mNumIncompleteLinks;                    /**< (1) Number of incomplete links. */
        /// @brief  Returns whether all of the link's source nodes are complete.
        bool linkSourceNodesReady(int link) const;
        /// @brief  Returns whether all of the node's inflowing links are complete.
        bool nodeInputLinksComplete(int node) const;
        /// @brief  Returns whether all of the network nodes and links are complete.
        bool checkAllComplete(const double dt);
        /// @brief  Returns the number of links that are not complete.
        int  countIncompleteLinks() const;
        /// @brief  Returns whether all network nodes are complete.
        virtual bool checkAllNodesComplete() const;
        /// @brief  Returns the index of the first incomplete link.
        unsigned int getFirstIncompleteLink() const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidFlowOrchestrator(const GunnsFluidFlowOrchestrator& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidFlowOrchestrator& operator =(const GunnsFluidFlowOrchestrator& that);
};

/// @}

#endif
