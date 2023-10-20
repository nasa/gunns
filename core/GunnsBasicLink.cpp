/**
@file
@brief    GUNNS Basic Link implementation

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the classes for modeling the GUNNS basic link.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     ()

LIBRARY DEPENDENCY:
    (
     (GunnsBasicNode.o)
     (simulation/hs/TsHsMsg.o)
     (software/exceptions/TsInitializationException.o)
     (software/exceptions/TsOutOfBoundsException.o)
    )

 PROGRAMMERS:
    (
     (Jason Harvey) (L3) (2011-02) (Initial Prototype))
     (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
    )
*/
#include "GunnsBasicLink.hh"
#include "math/MsMath.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

/// @details  Default Max Conductance
const double GunnsBasicLink::mConductanceLimit = 1.0E15;

/// @details  Limit 100 * DBL_EPSILON for some applications
const double GunnsBasicLink::m100EpsilonLimit = 100.0 * DBL_EPSILON;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name     (--) Link name
/// @param[in] nodeList (--) Network nodes structure
///
/// @details  Constructs the Basic Link Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLinkConfigData::GunnsBasicLinkConfigData(const std::string& name,
                                                   GunnsNodeList*     nodeList)
    :
    mName(name),
    mNodeList(nodeList)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy Constructs the Basic Link Config data
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLinkConfigData::GunnsBasicLinkConfigData(const GunnsBasicLinkConfigData& that)
    :
    mName(that.mName),
    mNodeList(that.mNodeList)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Link Config Data Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLinkConfigData::~GunnsBasicLinkConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] malfBlockageFlag  (--) Blockage malfunction flag
/// @param[in] malfBlockageValue (--) Blockage malfunction fractional value (0-1)
///
/// @details  Default constructs this Basic Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLinkInputData::GunnsBasicLinkInputData(const bool   malfBlockageFlag,
                                                 const double malfBlockageValue)
    :
    mInitialNodeMap(0),
    mMalfBlockageFlag(malfBlockageFlag),
    mMalfBlockageValue(malfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] that (--) Object to copy
///
/// @details  Copy constructs this Basic Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLinkInputData::GunnsBasicLinkInputData(const GunnsBasicLinkInputData& that)
    :
    mInitialNodeMap(that.mInitialNodeMap),
    mMalfBlockageFlag(that.mMalfBlockageFlag),
    mMalfBlockageValue(that.mMalfBlockageValue)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLinkInputData::~GunnsBasicLinkInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Constructs the Basic Link admittance map.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLinkAdmittanceMap::GunnsBasicLinkAdmittanceMap()
    :
    mSize(0),
    mMap(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Link admittance map.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLinkAdmittanceMap::~GunnsBasicLinkAdmittanceMap()
{
    freeMap();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) Name of this map instance for identifying the unique memory allocation.
/// @param[in] size (--) Size the array to allocate.
///
/// @details  This frees the map array, if it is already allocated, then allocates the new array to
///           the given size, and zeroes all entries.  This is used during link initialization, and
///           can be used during run to re-size the array when desired.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLinkAdmittanceMap::allocateMap(const std::string& name, const unsigned int size)
{
    freeMap();
    TS_NEW_PRIM_ARRAY_EXT(mMap, size, int, name + ".mMap");
    mSize = size;
    for (unsigned int i=0; i<size; ++i) {
        mMap[i] = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This deletes the map array.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLinkAdmittanceMap::freeMap()
{
    TS_DELETE_ARRAY(mMap);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] numPorts (--) The number of ports the link has
///
/// @details  Constructs the Basic Link Object
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::GunnsBasicLink(const int numPorts)
    :
    mMalfBlockageFlag(false),
    mMalfBlockageValue(0.0),
    mName(),
    mNodes(0),
    mNodeList(0),
    mPotentialVector(0),
    mAdmittanceMatrix(0),
    mSourceVector(0),
    mDefaultNodeMap(0),
    mNodeMap(0),
    mAdmittanceMap(),
    mOverrideVector(0),
    mPortDirections(0),
    mNumPorts(numPorts),
    mAdmittanceUpdate(false),
    mFlux(0.0),
    mPotentialDrop(0.0),
    mPower(0.0),
    mMinLinearizationPotential(1.0),
    mInitFlag(false),
    mNodeMapLocked(false),
    mUserPortSelect(-1),
    mUserPortSelectNode(-1),
    mUserPortSetControl(READY)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs the Basic Link Object.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink::~GunnsBasicLink()
{
    cleanup();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Deletes allocated memory objects
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::cleanup()
{
    TS_DELETE_ARRAY(mAdmittanceMatrix);
    TS_DELETE_ARRAY(mPortDirections);
    TS_DELETE_ARRAY(mOverrideVector);
    TS_DELETE_ARRAY(mPotentialVector);
    TS_DELETE_ARRAY(mSourceVector);
    TS_DELETE_ARRAY(mNodeMap);
    TS_DELETE_ARRAY(mNodes);
    {
        delete [] mDefaultNodeMap;
        mDefaultNodeMap = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]     configData (--) GUNNS Basic Link Config Data
/// @param[in,out] inputData  (--) GUNNS Basic Link Input Data
/// @param[in]     links      (--) Network links
/// @param[in]     ports      (--) Network port mapping array
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Basic Link with configuration and input data and sets up its
///           connectivity to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::initialize(const GunnsBasicLinkConfigData& configData,
                                const GunnsBasicLinkInputData&  inputData,
                                std::vector<GunnsBasicLink*>&   links,
                                const int*                      ports)
{
    /// - Reset init flag.
    mInitFlag = false;

    /// - Have to call delete in case someone calls this method twice during a run
    cleanup();

    /// - Add this link to the network links vector.
    links.push_back(this);

    /// - Initialize link name from config data.
    GUNNS_NAME_ERREX("GunnsBasicLink", configData.mName);

    /// - Validate the link config and port mapping data.  This must be done before memory is
    ///   allocated to insure that the portMap is not null.
    validate(configData, inputData, ports);

    /// - Do dynamic allocation of link members.
    allocateMatrixAndVectors(configData.mName);

    /// - Set and validate the link node pointers and set the node map.  The node map assignment
    ///   can come from two places, either as arguments to this method, or in the input data
    ///   mInitialNodeMap array.  The mInitialNodeMap array overrides the method arguments,
    ///   if it is not null.  Temporarily unlock the node map so it can be initialized, then return
    ///   the lock to its user-defined state.
    mNodeList = configData.mNodeList;
    const bool locked = mNodeMapLocked;
    mNodeMapLocked = false;
    if (inputData.mInitialNodeMap) {
        registerNodes(inputData.mInitialNodeMap, "input file", true);
    } else {
        registerNodes(ports, "this", false);
    }
    mNodeMapLocked = locked;
    validateNodes();

    /// - Initialize remaining class attributes.
    mFlux                      = 0.0;
    mPotentialDrop             = 0.0;
    mMinLinearizationPotential = 0.0;
    mMalfBlockageFlag          = inputData.mMalfBlockageFlag;
    mMalfBlockageValue         = inputData.mMalfBlockageValue;
    initializeRestartCommonFunctions();

    /// - Set init flag on successful validation.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Performs reset of link attributes needed after a checkpoint load.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::restart()
{
    initializeRestartCommonFunctions();
    registerNodes(mDefaultNodeMap, "restart", true);
    restartModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::restartModel()
{
    /// - Base class implementation is empty.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] portMap (--) Network port mapping array
/// @param[in] source  (--) Source of the port assignment command for output message (i.e. "user")
/// @param[in] verbose (--) Whether to output H&S messages indicating the move took place
///
/// @details  Assigns each port to the node position given by the portMap argument.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::registerNodes(const int* portMap, const std::string& source, const bool verbose)
{
    for (int i = 0; i < mNumPorts; ++i) {
        mDefaultNodeMap[i] = portMap[i];
        setPort(i, portMap[i], source, verbose);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Validates this GUNNS Basic Link initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::allocateMatrixAndVectors(const std::string& name)
{
    /// - Allocate link arrays based on the number of ports the link has.
    mDefaultNodeMap = new int[mNumPorts];
    TS_NEW_PRIM_POINTER_ARRAY_EXT(mNodes,            mNumPorts,           GunnsBasicNode,                 name + ".mNodes");
    TS_NEW_PRIM_ARRAY_EXT        (mNodeMap,          mNumPorts,           int,                            name + ".mNodeMap");
    TS_NEW_PRIM_ARRAY_EXT        (mSourceVector,     mNumPorts,           double,                         name + ".mSourceVector");
    TS_NEW_PRIM_ARRAY_EXT        (mPotentialVector,  mNumPorts,           double,                         name + ".mPotentialVector");
    TS_NEW_PRIM_ARRAY_EXT        (mOverrideVector,   mNumPorts,           bool,                           name + ".mOverrideVector");
    TS_NEW_PRIM_ARRAY_EXT        (mPortDirections,   mNumPorts,           GunnsBasicLink::PortDirection,  name + ".mPortDirections");
    allocateAdmittanceMatrix();

    // Initialize the arrays out to keep Valgrind happy.  Initialize the node map to an invalid
    // node number so that validation will know if any ports failed to map.
    for (int i = 0; i < mNumPorts; ++i) {
        mDefaultNodeMap[i]  = -1;
        mNodeMap[i]         = -1;
        mSourceVector[i]    = 0.0;
        mPotentialVector[i] = 0.0;
        mOverrideVector[i]  = false;
        mPortDirections[i]  = NONE;
    }

    /// - Allocate the admittance map.
    createAdmittanceMap();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Allocates the admittance matrix and fills it with zeros.  This default implementation
///           makes the matrix with the default uncompressed size, nPorts * nPorts.  Derived classes
///           can override this function to allocate their matrix with custom sizes as needed, such
///           as in links that don't need an admittance matrix, or use a compressed matrix format.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::allocateAdmittanceMatrix()
{
    TS_NEW_PRIM_ARRAY_EXT(mAdmittanceMatrix, mNumPorts*mNumPorts, double, mName + ".mAdmittanceMatrix");
    for (int i = 0; i < mNumPorts*mNumPorts; ++i) {
        mAdmittanceMatrix[i] = 0.0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default implementation, which loads the admittance map for the normal
///           symmetrical link admittance matrix of size nPorts * nPorts.  Custom links can override
///           this method to load their custom or compressed maps as needed.
///
/// @note  Custom implementations should always use a value of -1 to denote mapping to the network
///        ground node or a non-node.  This is how the solver knows to avoid copying the
///        corresponding admittance value from the link to the network matrix.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::updateAdmittanceMap()
{
    const int networkSize = getGroundNodeIndex();
    for (int port1 = 0; port1 < mNumPorts; ++port1) {
        const int node1 = mNodeMap[port1];

        for (int port2 = 0, a = port1*mNumPorts; port2 < mNumPorts; ++port2, ++a) {
            const int node2 = mNodeMap[port2];

            if (node1 == networkSize or node2 == networkSize) {
                /// - Use a value of -1 to tell the solver this is the Ground node.
                mAdmittanceMap.mMap[a] = -1;
            } else {
                mAdmittanceMap.mMap[a] = node1 * networkSize + node2;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] configData (--) GUNNS Basic Link Config Data
/// @param[in] inputData  (--) GUNNS Basic Link Input Data
/// @param[in] portMap    (--) Port nodes assignment array
///
/// @throws   TsInitializationException
///
/// @details  Validates this Gunns Basic Link initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::validate(const GunnsBasicLinkConfigData& configData,
                              const GunnsBasicLinkInputData&  inputData,
                              const int* portMap) const
{
    /// - Issue an error if node list is NULL.
    if (0 == configData.mNodeList) {
        GUNNS_ERROR(TsInitializationException, "Invalid Configuration Data",
                    "Link has null node list.");
    }

    /// - Issue an error if port map is NULL.
    if (0 == portMap) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Link has null port map.");
    }

    /// - Issue an error if the number of ports is invalid.
    if (mNumPorts < 1) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Link has number of ports < 1.");
    }

    /// - Issue an error if blockage malfunction value not inside (0-1).
    if (!MsMath::isInRange(0.0, inputData.mMalfBlockageValue, 1.0)) {
        GUNNS_ERROR(TsInitializationException, "Invalid Input Data",
                    "Link has blockage outside valid range (0-1).");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Validates the nodes attached to the link ports
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::validateNodes() const
{
    for (int port = 0; port < mNumPorts; ++port) {

        /// - Issue an error if a port has not been mapped.
        if (mNodeMap[port] < 0) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                        "Link has incomplete node mapping.");
        }

        /// - Issue an error on null pointers to nodes.
        else if (!mNodes[port]) {
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                        "Link has null node pointers.");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--) Returns true if the link is changing the system admittance matrix
///
/// @details  Returns a flag indicating whether the link needs an admittance update.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicLink::needAdmittanceUpdate()
{
    bool update = mAdmittanceUpdate;
    mAdmittanceUpdate = false;  //Clear flag if set
    return update;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port    (--) The port to be assigned
/// @param[in] node    (--) The desired node to assign the port to
/// @param[in] source  (--) Source of the port assignment command for output message (i.e. "user")
/// @param[in] verbose (--) Whether to output H&S messages indicating the move took place
///
/// @returns   bool (--) Returns true if the port was successfully set.
///
/// @details  Attaches the desired port to the desired node, if all link port assignment rules are
///           satisfied.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicLink::setPort(const int port, const int node, const std::string& source,
                             const bool verbose)
{
    const bool result = (not mNodeMapLocked) and checkPortRules(port, node);
    if (result and (node != mNodeMap[port])) {
        if (verbose) {
            GUNNS_INFO(source << " moved port " << port << " from node " << mNodeMap[port]
                                                        << " to node " << node << ".");
        }

        /// - Upon port change, indicate to the solver that an admittance matrix update is required,
        ///   to avoid the solver re-using old decompositions that don't reflect the new ports.  The
        ///   only links that would not require this are those that never set the admittance matrix,
        ///   but as these are only the "source" links and are relatively few, we go ahead and force
        ///   it here for all links.
        mNodeMap[port] = node;
        //        mNodes[port] = &mNodeList->mNodes[node];
        mAdmittanceUpdate = true;
    }

    /// - Regardless of whether the node map was changed, always ensure the node pointers and
    ///   admittance map match the node map.
    updateNodePointer(port);
    updateAdmittanceMap();

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port   (--) The port to be updated
///
/// @details  Sets the port node pointer to the address of the node object number from the network
///           node list indicated by the port's node map.  This ensures the node pointed to always
///           matches the node map value.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::updateNodePointer(const int port)
{
    if (checkValidPortNumber(port)) {
        mNodes[port] = &mNodeList->mNodes[mNodeMap[port]];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if all rules checks pass
///
/// @details  Checks the requested port & node arguments for validity.  First checks against rules
///           that apply to all Gunns links, then calls a virtual method for the derived link to
///           check its own specific rules.  Health & Status warning are generated for violations.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicLink::checkPortRules(const int port, const int node) const
{
    bool result = true;

    /// - Requested port number must be in the link class's range.
    if (not checkValidPortNumber(port)) {
        GUNNS_WARNING("aborted setting a port with invalid port number: " << port << ".");
        result = false;
    }

    /// - Requested node number must be in the network's node list range.
    else if (not checkValidNodeNumber(node)) {
        GUNNS_WARNING("aborted setting a port with invalid node number: " << node << ".");
        result = false;
    }

    /// - Verify port/node duplication rules are satisfied.
    else {
        result = checkPortDuplication(port, node);
    }

    /// - Check any optional derived class rules.
    if (result) {
        result = checkSpecificPortRules(port, node);
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port (--) The port to be assigned
/// @param[in] node (--) The desired node to assign the port to
///
/// @returns  bool  (--) Returns true if port/node duplication checks passed
///
/// @details  Checks that the same node is not mapped to duplicate ports.  By default, no two ports
///           in a link should ever map to the same node, unless it is the network's vacuum/ground
///           node.  Derived link classes can override this method to make exceptions to this rule
///           as needed.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicLink::checkPortDuplication(const int port, const int node) const
{
    bool result = true;

    for (int otherPort = 0; otherPort < mNumPorts; ++otherPort) {

        if ((port != otherPort) && (mNodeMap[otherPort] == node) &&
                (node < getGroundNodeIndex())) {
            GUNNS_WARNING("aborted setting a port: ports cannot share the same non-boundary"
                          << " node " << node << ".");
            result = false;
        }
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fromPort (--) The port the flux is transported out of (defaults to 0)
/// @param[in] toPort   (--) The port the flux is transported into (default to 1)
///
/// @throws   TsOutOfBoundsException
///
/// @details  Makes calls to accumulate flux in the input & output terms of the receiving and
///           sending nodes, respectively.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::transportFlux(const int fromPort, const int toPort)
{
    if (checkValidPortNumber(toPort) && checkValidPortNumber(fromPort)) {
        if (mFlux > 0.0) {
            mNodes[fromPort]->collectOutflux(mFlux);
            mNodes[toPort  ]->collectInflux (mFlux);

        } else if (mFlux < 0.0) {
            mNodes[toPort  ]->collectOutflux(-mFlux);
            mNodes[fromPort]->collectInflux (-mFlux);
        }
    }
    else {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                    "method called with bad port number.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] fromPort (--) The port the delta is referenced from (defaults to 0)
/// @param[in] toPort   (--) The port the delta is referenced to (defaults to 1)
///
/// @returns   double   (--) Generic delta-potential
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the difference in potential across the link as (toPort - fromPort).  For
///           instance if potential at toPort = 4.0 and at fromPort = 3.0, the delta-potential is
///           +1.0 and is thought of as a potential rise.
////////////////////////////////////////////////////////////////////////////////////////////////////
double GunnsBasicLink::getDeltaPotential(const int toPort, const int fromPort)
{
    double value = 0.0;
    if (checkValidPortNumber(toPort) && checkValidPortNumber(fromPort)) {
        value = mPotentialVector[toPort] - mPotentialVector[fromPort];
    } else {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                    "method called with bad port number.");
    }
    return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Handles user commands to dynamically re-map the selected port to the desired node.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::processUserPortCommand()
{
    if (DEFAULT == mUserPortSetControl) {
        /// - DEFAULT command attempts to return the given port to its default node map.  If the
        ///   given port is invalid then a failure indication is given.
        if (checkValidPortNumber(mUserPortSelect)) {
            mUserPortSelectNode = mDefaultNodeMap[mUserPortSelect];
            mUserPortSetControl = EXECUTE;
        } else {
            GUNNS_WARNING("aborted setting a port with invalid port number: " << mUserPortSelect << ".");
            mUserPortSetControl = FAILED;
            mUserPortSelect     = -1;
            mUserPortSelectNode = -1;
        }

    } else if (GROUND  == mUserPortSetControl) {
        /// - GROUND command attempts to map the given port to the network ground node.
        mUserPortSelectNode = getGroundNodeIndex();
        mUserPortSetControl = EXECUTE;
    }

    if (EXECUTE == mUserPortSetControl) {
        /// - Attempt the user's assignment and reset the control based on the result.
        if (setPort(mUserPortSelect, mUserPortSelectNode, "user", true)) {
            mUserPortSetControl = READY;
        } else {
            mUserPortSetControl = FAILED;
        }

        /// - Reset the port and node selections to bad values to force the user to enter desired
        ///   values each time.  This reduces risk of accidental mapping of the link.
        mUserPortSelect     = -1;
        mUserPortSelectNode = -1;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates
/// @param[in] value (--) Fraction of blocked flow path area (0-1)
///
/// @details  Sets the malf blockage flag and value to given parameters.  Calling this method with
///           default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::setMalfBlockage(const bool flag, const double value)
{
    mMalfBlockageFlag  = flag;
    mMalfBlockageValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port  (--)  Port number to check for validity
///
/// @returns  bool  (--)  Returns true if the port number is valid, false otherwise.
///
/// @details  Port number is valid if it is between 0 and (mNumPorts-1) inclusive.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicLink::checkValidPortNumber(const int port) const
{
    return ((-1 < port) and (mNumPorts > port));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] node  (--)  Node number to check for validity
///
/// @returns  bool  (--)  Returns true if the node number is valid, false otherwise.
///
/// @details  Node number is valid if it is between 0 and the ground node inclusive.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool GunnsBasicLink::checkValidNodeNumber(const int node) const
{
    return ((-1 < node) and (getGroundNodeIndex() >= node));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port  (--)  Port number to activate the override for.
/// @param[in] value (--)  Value to set the port's potential vector to.
///
/// @details  This sets the override flag for the given port true, and sets the link's potential
///           vector at the port to the given value.  The port is checked for a valid port number
///           and if it's invalid, a warning message is sent an no other action taken.
///
///           Note that any value can be assigned to the potential vector, regardless of whether it
///           makes sense in the application's aspect (i.e. can give negative temperatures in a
///           thermal aspect).  It is the user's responsibility to limit to appropriate values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::setPortOverride(const int port, const double value)
{
    if (checkValidPortNumber(port)) {
        mOverrideVector[port]  = true;
        mPotentialVector[port] = value;
    } else {
        GUNNS_WARNING("ignored setPortOverride call given invalid port number.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] port  (--)  Port number to deactivate the override for.
///
/// @details  This sets the override flag for the given port false.  The link's potential vector is
///           left alone, assuming that the solver will return it to normal network solution values
///           after the next solution.  The port is checked for a valid port number and if it's
///           invalid, a warning message is sent an no other action taken.
///
///           Calling this method with default arguments results in the override flag being reset
///           for all ports.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicLink::resetPortOverride(const int port)
{
    if (-1 == port) {
        for (int i=0; i<mNumPorts; ++i) {
            mOverrideVector[i] = false;
        }
    } else if (checkValidPortNumber(port)) {
        mOverrideVector[port] = false;
    } else {
        GUNNS_WARNING("ignored resetPortOverride call given invalid port number.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   int -- The index of the network's ground node.
///
/// @details  Returns the index of the network's ground node.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsBasicLink::getGroundNodeIndex() const
{
    return mNodeList->mNumNodes - 1;
}
