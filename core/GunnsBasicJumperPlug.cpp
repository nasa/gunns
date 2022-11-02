/**
@file
@brief    GUNNS Basic Jumper Plug Class implementation

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Class for the GUNNS Basic Jumper Plug.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((GunnsBasicSocket.o)
    (software/exceptions/TsInvalidStateTransitionException.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Initial) (2012-08))
*/

#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsInvalidStateTransitionException.hh"

#include "GunnsBasicJumperPlug.hh"
#include "GunnsBasicLink.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name     (--) The name of the plug object for error messages.
/// @param[in] listSize (--) The number of sockets that will be in the list.
///
/// @details Default constructs this Basic Jumper Plug, by allocating the socket pointer array to
///          the given list size.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumperPlug::GunnsBasicJumperPlug(const std::string& name, const int listSize)
    :
    mName(),
    mSockets(0),
    mNumSockets(0),
    mConnectedNode(0),
    mDirectConnectionRequest(0),
    mDirectConnectionDisable(false)
{
    GUNNS_NAME_ERREX("GunnsBasicJumperPlug", name);
    mSockets = new GunnsBasicLink*[listSize];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Basic Jumper Plug.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicJumperPlug::~GunnsBasicJumperPlug()
{
    {
        delete [] mSockets;
        mSockets = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] socket (--) Reference to the socket object to be added to the list
///
/// @throws  TsInitializationException
///
/// @details If the list is not yet full, add the new socket's address to the pointer array and
///          update the number of sockets.  Otherwise, throw an exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicJumperPlug::addSocket(GunnsBasicLink& socket)
{
    if (mNumSockets < getNoConnection()) {
        mSockets[mNumSockets] = &socket;
        mNumSockets++;
    } else {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "too many sockets added to list.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] initNode   (--) Initial node prior to socket connection.
/// @param[in] initSocket (--) Index of the desired initial socket connection.
///
/// @returns  int (--) Index to the connected network node.
///
/// @throws   TsInitializationException
///
/// @details  Initializes the Plug's connection and validates the initial state.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsBasicJumperPlug::initialize(const int initNode, const int initSocket)
{
    /// - Check that all the sockets can be retrieved.
    for (int i = 0; i < getNoConnection(); ++i) {
        try {
            getSocket(i);
        } catch (TsOutOfBoundsException& e) {
            std::ostringstream msg;
            msg << "Missing socket at list position " << i << ".";
            GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data", msg.str());
        }
    }

    //TODO
    mDirectConnectionRequest = -99;
    mDirectConnectionDisable = false;

    /// - Throw an exception if there is not a valid socket assignment.
    if ( (initSocket < 0) or
         (initSocket > getNoConnection()) ) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Invalid initial socket assignment");
    }

    /// - Connect to the initial socket and throw an exception if connection failed.
    mConnectedNode = initNode;
    setConnectionRequest(initSocket);
    processConnectionRequest();
    updateActiveConnection();
    if (getActiveConnection() != initSocket) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                    "Failed to connect to initial socket.");
    }

    return mConnectedNode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  currentNode (--) The network node number the link's port is currently connected to.
///
/// @returns  int (--) Index to the connected network node.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Updates the connection state of the Plug and returns the connected node index.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsBasicJumperPlug::update(const int currentNode)
{
    /// - First set the plug's connected node equal to the current port connection of the link, so
    ///   that if this update of the plug results in no change in connection, it will return the
    ///   same node number back to the link.
    mConnectedNode = currentNode;

    /// - Process disconnect requests and update the active connection before processing connection
    ///   requests, to allow a "quick swap" from one socket to another in one pass.
    processDirectConnectionRequest();
    processDisconnectionRequest();
    updateActiveConnection();
    processConnectionRequest();
    updateActiveConnection();
    return mConnectedNode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index (--) Position in the list of the socket to be returned
///
/// @returns GunnsBasicLink* (--) Pointer to the link at the requested index
///
/// @throws  TsOutOfBoundsException
///
/// @details The requested index is first checked to make sure it is in the range of the actual
///          socket in the list.  If it is in range and the socket's address is not null, the
///          address is returned, otherwise throw an exception.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsBasicLink* GunnsBasicJumperPlug::getSocket(const int index) const
{
    if (index < 0 or index >= mNumSockets) {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                    "a link attempted to retrieve a bad socket index.");
    } else if (0 == mSockets[index]) {
        GUNNS_ERROR(TsOutOfBoundsException, "Input Argument Out Of Range",
                    "a requested socket index has null pointer.");
    } else {
        return mSockets[index];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsOutOfBoundsException
///
/// @details  Processes a direct connect request for this Plug.
///
/// @note     Unlike the connection and disconnection requests, which are meant to be applied
///           occasionally and asynchronously, the direct connection request is a continuously
///           applied command.  As such, the command term is only reset every pass when it is
///           disabled, and this allows the user to manually inspect that the command has been
///           received by the plug when it is enabled.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicJumperPlug::processDirectConnectionRequest()
{
    /// - Reset the direct connection request so that it cannot have an effect, if the disable flag
    ///   has been set by the user.
    if (mDirectConnectionDisable) {
        mDirectConnectionRequest = -99;
    }

    /// - If the direct connection request is set to any valid connection number that is different
    ///   than the current connection, disconnect from the current connection and attempt to connect
    ///   to the direct connection request.
    if (mDirectConnectionRequest >= 0 and
        mDirectConnectionRequest <= getNoConnection() and
        mDirectConnectionRequest != getActiveConnection()) {

        setDisconnectionRequest(getActiveConnection());
        if (mDirectConnectionRequest != getNoConnection()) {

            /// - Check to make sure the desired socket doesn't already have another connection, and
            ///   require the user to clear it before trying this command again.
            GunnsBasicSocket* socket = static_cast<GunnsBasicSocket*>(getSocket(mDirectConnectionRequest));
            if (socket->isConnected()) {
                GUNNS_WARNING("unable to complete direct connection, desired socket " << socket->getName()
                        << " already has a connection.  This plug is left disconnected.");

            /// - Disconnect from the current socket and connect to the new one.
            } else {
                setConnectionRequest(mDirectConnectionRequest);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsOutOfBoundsException
///
/// @details  Processes a connect request for this Plug.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicJumperPlug::processConnectionRequest()
{
    /// - Check for a connect request.
    if (getNoConnection() != getConnectionRequest()) {

        /// - Check to make sure we aren't already connected to a socket, and require the user to
        ///   disconnect from it before connecting to a new one.
        if (getNoConnection() != getActiveConnection()) {
            GUNNS_WARNING("ignored connection request, please un-plug from the current socket "
                          "before plugging into a new one.");

        /// - Ask the socket for a connection.  Connect to the returned node index if the request
        ///   wasn't denied.
        } else try {
            mConnectedNode = connectToSocket(getConnectionRequest());
            setLastDisconnection(getNoConnection());
        } catch (TsInvalidStateTransitionException& e) {
            //do nothing
        }
    }

    /// - Reset the connection request.
    setConnectionRequest(getNoConnection());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsOutOfBoundsException
///
/// @details  Processes a disconnect request for this plug.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicJumperPlug::processDisconnectionRequest()
{
    /// - Check for a disconnect request.
    if (getNoConnection() != getDisconnectionRequest()) {

        /// - Check to make sure we aren't already connected to a socket, and require the user to
        ///   disconnect from it before disconnecting to a new one.
        if ( (getActiveConnection() != getDisconnectionRequest()) and
             (getActiveConnection() != getNoConnection()) ){
            GUNNS_WARNING("ignored disconnection request, please un-plug from the current socket "
                          "before un-plugging to a new one (connecting to its ambient node).");

        /// Ask the socket for disconnection, and connect to the returned ambient node.
        } else {
            mConnectedNode = disconnectFromSocket(getDisconnectionRequest());
            setLastDisconnection(getDisconnectionRequest());
        }
    }

    /// - Reset the disconnection request.
    setDisconnectionRequest(getNoConnection());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  void
///
/// @throws   TsOutOfBoundsException
///
/// @details  Loops through each socket in the list and polls them for a connection to this plug.
///           Set the active connection status to the identified socket, or "no connection" if no
///           socket is connected.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsBasicJumperPlug::updateActiveConnection()
{
    int connection = getNoConnection();
    for (int i = 0; i < getNoConnection(); ++i) {
        if (isSocketConnected(i)) {
            connection = i;
            break;
        }
    }
    setActiveConnection(connection);
}
