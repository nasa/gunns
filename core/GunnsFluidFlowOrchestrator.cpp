/**
@file
@brief     GUNNS Fluid Flow Orchestrator implementation

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
  ((core/GunnsFluidLink.o)
   (core/GunnsBasicFlowOrchestrator.o)
   (simulation/hs/TsHsMsg.o)
   (software/exceptions/TsOutOfBoundsException.o))
*/

#include "GunnsFluidFlowOrchestrator.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "core/GunnsFluidLink.hh"
#include "core/GunnsMacros.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  numLinks  (--)  The number of links in the network.
/// @param[in]  numNodes  (--)  The number of nodes in the network, including the Ground node.
///
/// @details  Default constructs this GUNNS Fluid Flow Orchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowOrchestrator::GunnsFluidFlowOrchestrator(const int& numLinks, const int& numNodes)
    :
    GunnsBasicFlowOrchestrator(numLinks, numNodes),
    mLinkStates(0),
    mNodeStates(0),
    mNumIncompleteLinks(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Fluid Flow Orchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidFlowOrchestrator::~GunnsFluidFlowOrchestrator()
{
    delete [] mNodeStates;
    mNodeStates = 0;
    delete [] mLinkStates;
    mLinkStates = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  name         (--)  Instance name for messages.
/// @param[in]  links        (--)  Pointer to the network links.
/// @param[in]  nodes        (--)  Pointer to the network nodes.
/// @param[in]  linkNodeMaps (--)  Pointer to the network links node maps.
/// @param[in]  linkNumPorts (--)  Pointer to the network links number of ports.
///
/// @details  Initializes this Fluid Flow Orchestrator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowOrchestrator::initialize(const std::string& name, GunnsBasicLink** links,
                                            GunnsBasicNode** nodes, int** linkNodeMaps,
                                            int* linkNumPorts)
{
    /// - Initialize the base class.
    GunnsBasicFlowOrchestrator::initialize(name, links, nodes, linkNodeMaps, linkNumPorts);

    /// - Reset the initialization complete flag.
    mInitFlag = false;

    /// - Allocate arrays and initialize state.
    mLinkStates = new bool[mNumLinks];
    for (int i=0; i<mNumLinks; ++i) {
        mLinkStates[i] = false;
    }
    mNodeStates = new NodeStates[mNumNodes];
    for (int i=0; i<mNumNodes; ++i) {
        mNodeStates[i] = INCOMPLETE;
    }
    mNumIncompleteLinks = 0;

    /// - Set the initialization complete flag.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @throws  TsOutOfBoundsException
///
/// @details  See the class details.  More notes:
///
///           When a node is not overflowing, outflow links take the node's previous content state
///           before the node's inflows are mixed in.  However if a node is overflowing, then the
///           node's inflows are mixed in before the outflowing link updates, so the outflow link
///           takes the node's final mixed state.
///
///           This will throw a TsOutOfBoundsException when we have to emergency break out of the do
///           loop to avoid an infinite loop.  This may leave some fluid transport and node state
///           balancing unfinished.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsFluidFlowOrchestrator::update(const double dt)
{
    /// - Call computeFlows on all links.  The links will compute their molar flow rates, determine
    ///   flow directions relative to the nodes, and schedule outflows with their source nodes.
    /// - Initially flag all links as incomplete.
    for (int link = 0; link < mNumLinks; ++link) {
        mLinks[link]->computeFlows(dt);
        mLinkStates[link] = false;
    }
    mNumIncompleteLinks = mNumLinks;

    /// - Initially flag all nodes as incomplete, except for the Ground node which is always
    ///   complete.
    mNodeStates[mNumNodes-1] = COMPLETE;
    for (int node = 0; node < mNumNodes-1; ++node) {
        if (mNodes[node]->isOverflowing(dt)) {
            mNodeStates[node] = OVERFLOWING;
        } else {
            mNodeStates[node] = INCOMPLETE;
        }
    }

    /// - Links and nodes flow transport and integration loop.  The loop is repeated until all nodes
    ///   and links are completed.
    try {
        do {
            /// - Complete all incomplete links that have all of their source nodes ready for
            ///   outflow.  Nodes are ready for outflow when they are either complete or
            ///   non-overflowing.
            for (int link = 0; link < mNumLinks; ++link) {
                if (not mLinkStates[link] and linkSourceNodesReady(link)) {
                    mLinks[link]->transportFlows(dt);
                    mLinkStates[link] = true;
                }
            }

            /// - Complete all incomplete nodes that have all of their input links complete.  Input
            ///   links are those that are flowing into the node.
            for (int node = 0; node < mNumNodes-1; ++node) {
                if (COMPLETE != mNodeStates[node] and nodeInputLinksComplete(node)) {
                    mNodes[node]->integrateFlows(dt);
                    mNodeStates[node] = COMPLETE;
                }
            }
        } while (not checkAllComplete(dt));
    } catch (TsOutOfBoundsException& e) {
        GUNNS_ERROR(TsOutOfBoundsException, "Escape Infinite Loop",
                "flow transport loop failed to converge and was aborted.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  dt  (s)  Integration time step.
///
/// @throws   TsOutOfBoundsException
///
/// @returns  bool  (--)  True if all nodes and links are complete.
///
/// @details  Checks completion of all nodes and links.  Updates the incomplete link count.  Breaks
///           a loop of overflowing nodes by forcing links to complete early until the loop is
///           broken.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidFlowOrchestrator::checkAllComplete(const double dt)
{
    /// - Return true to exit the transport loop when all nodes & links are complete:
    int incompleteLinks = countIncompleteLinks();
    if (0 == incompleteLinks and checkAllNodesComplete()) {
        mNumIncompleteLinks = 0;
        return true;
    }

    /// - If the number of incomplete links has not reduced since the last call, then we are stuck.
    ///   This occurs in the design limitation cases described in the Assumptions & Limitations.
    ///   We prevent the infinite do loop by picking a link and forcing it to complete.  Eventually
    ///   we will force enough links to complete to break the overflow node loops, and allow all
    ///   nodes & links to complete.
    ///
    ///   Because this forces a link to transport flows before its source nodes have completed, this
    ///   causes non-conservation of mass & energy in the source nodes.  So we warn of this event to
    ///   alert the users of non-conservation.
    ///
    ///   For further safeguarding against an infinite loop, we pass on exceptions thrown by
    ///   getFirstIncompleteLink, which will be thrown if there are no incomplete links but for
    ///   whatever reason there are some nodes that are stuck not completing.
    if (incompleteLinks >= mNumIncompleteLinks) {
        const unsigned int link = getFirstIncompleteLink();
        mLinks[link]->transportFlows(dt);
        mLinkStates[link] = true;
        GUNNS_WARNING("early overflow transport in link " << mLinks[link]->getName() <<
                      ", conservation errors may result.");
    }
    mNumIncompleteLinks = incompleteLinks;
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if all nodes are complete.
///
/// @details  Loops over all nodes and returns false as soon as one is found to be incomplete.  If
///           the loop finished and none were found incomplete, returns true.
///
/// @note     This method is only virtual to allow for manipulation in the unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidFlowOrchestrator::checkAllNodesComplete() const
{
    for (int node = 0; node < mNumNodes-1; ++node) {
        if (COMPLETE != mNodeStates[node]) {
            return false;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsOutOfBoundsException
///
/// @returns  int  (--)  The index of the first link that has not completed.
///
/// @details  Finds the first link that has not completed.  Because all links should have been
///           completed before the last node is updated, then this function shouldn't be called if
///           there are no incomplete links.  If there still are none, then there is a problem,
///           probably some nodes stuck not completing.  Then we throw an exception to break us out
///           of a potential infinite do loop.
////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int GunnsFluidFlowOrchestrator::getFirstIncompleteLink() const
{
    for (int link = 0; link < mNumLinks; ++link) {
        if (not mLinkStates[link]) {
            return link;
        }
    }

    throw TsOutOfBoundsException();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  link  (--)  Index of the link to check.
///
/// @returns  bool  (--)  True if the link is ready to do flow transport.
///
/// @details  Determines whether the link is ready to do its flow transport.  The link is ready when
///           all of its source nodes are either complete, or otherwise not overflowing.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidFlowOrchestrator::linkSourceNodesReady(int link) const
{
    /// - Return false (not ready) if any of the link's source nodes are overflowing and incomplete.
    for (int port = 0; port < mLinkNumPorts[link]; ++port) {
        if (GunnsBasicLink::SOURCE == mLinkPortDirections[link][port] or
            GunnsBasicLink::BOTH   == mLinkPortDirections[link][port]) {
            if (OVERFLOWING == mNodeStates[mLinkNodeMaps[link][port]]) {
                return false;
            }
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  node  (--)  Index of the node to check.
///
/// @returns  bool  (--)  True if the node is ready to do flow integration.
///
/// @details  Determines whether a node is ready to do its flow integration.  The node is ready when
///           all of its input flow links are complete.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsFluidFlowOrchestrator::nodeInputLinksComplete(int node) const
{
    /// - Loop over all links.  Find all links connected to this node that are flowing to this node.
    ///   If any is not complete, then return false (not ready).
    for (int link = 0; link < mNumLinks; ++link) {
        for (int port = 0; port < mLinkNumPorts[link]; ++port) {
            if (node == mLinkNodeMaps[link][port] and not mLinkStates[link]
                    and (GunnsBasicLink::SINK == mLinkPortDirections[link][port]
                      or GunnsBasicLink::BOTH == mLinkPortDirections[link][port])) {
                return false;
            }
        }
    }

    /// - After loop ends, return true because either no links are connected, or all connected links
    ///   are complete or not an input flow to this node.
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int  (--)  Number of incomplete links.
///
/// @details  Counts the number of false (incomplete) flags in the links completion states array.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsFluidFlowOrchestrator::countIncompleteLinks() const
{
    int result = 0;
    for (int link = 0; link < mNumLinks; ++link) {
        if (not mLinkStates[link]) {
            result++;
        }
    }
    return result;
}
