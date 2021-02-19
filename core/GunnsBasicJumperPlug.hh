#ifndef GunnsBasicJumperPlug_EXISTS
#define GunnsBasicJumperPlug_EXISTS

/**
@file
@brief    GUNNS Basic Jumper Plug Class declarations

@defgroup  TSM_GUNNS_BASIC_JUMPER_PLUG    GUNNS Basic Jumper Plug Class
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Class for the GUNNS Basic Jumper Plug.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsBasicJumperPlug.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2012-08))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsBasicSocket.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Jumper Plug
///
/// @details  The GUNNS Basic Jumper Plug class serves as an interface between GUNNS Basic Jumper
///           and Basic Socket links.  It simulates a plug, or end, of a jumper that plugs the
///           jumper into a socket.  It contains a list of pointers to the GUNNS Socket links that
///           it is allowed to plug into.
///
///           This is a pure virtual class, and is intended to be extended in a network manager.
///           The derived class will contain an enumerated list of socket names, control targets for
///           the users to request connection, disconnection, and current connection state of the
///           plugs, and implements these virtual methods to allow access to those target parameters
///           by the jumper links.
///
/// @note     This class is NOT a GUNNS link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicJumperPlug
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicJumperPlug);
    public:
        /// @brief   Default constructs this Basic Jumper Plug.
        GunnsBasicJumperPlug(const std::string& name, const int listSize);
        /// @brief   Default destructs this Basic Jumper Plug.
        virtual ~GunnsBasicJumperPlug();
        /// @brief   Sets the direct connection request of the plug to the given index.
        void         setDirectConnectionRequest(const int index);
        /// @brief   Sets the active connection of the plug to the given index.
        virtual void setActiveConnection       (const int index) = 0;
        /// @brief   Sets the last disconnected socket of this plug to the given index.
        virtual void setLastDisconnection      (const int index __attribute__((unused))) {;} //TODO make pure virtual when all drawings updated new new socket list template
        /// @brief   Sets the connection request of the plug to the given index.
        virtual void setConnectionRequest      (const int index) = 0;
        /// @brief   Sets the disconnection request of the plug to the given index.
        virtual void setDisconnectionRequest   (const int index) = 0;
        /// @brief   Gets the index of the direct connection requested socket.
        int          getDirectConnectionRequest() const;
        /// @brief   Gets the index of the currently connected socket.
        virtual int  getActiveConnection()                    = 0;
        /// @brief   Gets the index of the socket last disconnected from.
        virtual int  getLastDisconnection() {return getNoConnection();} //TODO make pure virtual when all drawings updated new new socket list template
        /// @brief   Gets the index of the requested socket to plug into.
        virtual int  getConnectionRequest()                   = 0;
        /// @brief   Gets the index of the requested socket to unplug from.
        virtual int  getDisconnectionRequest()                = 0;
        /// @brief   Returns the index of the no-connection socket, also the size of the list.
        virtual int  getNoConnection()                        = 0;
        /// @brief   Returns a pointer to the socket at the given index in the list.
        GunnsBasicLink* getSocket(const int index) const;
        /// @brief   Adds a socket pointer to the list.
        void         addSocket(GunnsBasicLink& socket);
        /// @brief   Initializes the plug's connection and validates the initial state.
        int          initialize(const int initNode, const int initSocket);
        /// @brief   Updates the State of the plug and returns the connected node.
        int          update(const int currentNode);
        /// @brief   Returns the plug's name.
        const char*  getName() const;

    protected:
        std::string      mName;                     /**< *o (--) trick_chkpnt_io(**) Plug object name for error messages */
        GunnsBasicLink** mSockets;                  /**< ** (--) trick_chkpnt_io(**) Socket pointers array               */
        int              mNumSockets;               /**< *o (--) trick_chkpnt_io(**) Number of sockets in the list       */
        int              mConnectedNode;            /**< *o (--) trick_chkpnt_io(**) Index of the connected network node */
        /// @details The direct connection request is a shortcut that allows the user to move the
        ///          plug directly from one socket to another (or no socket connection), without
        ///          having to first command the plug to disconnect from the current socket, and
        ///          then command a new connection, etc.  It is meant to be input from a user
        ///          control that matches the derived socket list enum type.  When used, it
        ///          overrides the separate connect & disconnect controls.  Its use is optional, so
        ///          leaving it alone enables use of the connect & disconnect controls.
        int              mDirectConnectionRequest;  /**<    (--)                     Index of the desired socket for direct connection */
        bool             mDirectConnectionDisable;  /**<    (--)                     True disables & resets the direct connection request */
        /// @brief   Processes direct connection requests for the plug.
        virtual void processDirectConnectionRequest();
        /// @brief   Processes socket connection requests for the plug.
        void         processConnectionRequest();
        /// @brief   Processes socket disconnection requests for the plug.
        void         processDisconnectionRequest();
        /// @brief   Updates the socket list's active connection for the plug.
        void         updateActiveConnection();
        /// @brief   Requests connection to the socket.
        virtual int  connectToSocket(const int index);
        /// @brief   Requests disconnection from the socket.
        virtual int  disconnectFromSocket(const int index);
        /// @brief   Requests connection status from the socket.
        virtual bool isSocketConnected(const int index);

    private:
        /// @brief   Copy constructor is not implemented.
        GunnsBasicJumperPlug(const GunnsBasicJumperPlug&);
        /// @brief   Assignment operator is not implemented.
        GunnsBasicJumperPlug& operator =(const GunnsBasicJumperPlug&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index (--) Index of the socket to connect directly to.
///
/// @details  Stores the desired socket index in the plug's mDirectConnectionRequest parameter so
///           the plug will attempt to connect directly to that socket when it is updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicJumperPlug::setDirectConnectionRequest(const int index)
{
    mDirectConnectionRequest = index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) The index of the socket that this plug is being requested to connect directly
///                    to.
///
/// @details  Returns the index of the socket that this plug is being requested to connect directly
///           to.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsBasicJumperPlug::getDirectConnectionRequest() const
{
    return mDirectConnectionRequest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  char* (--) The plug's name.
///
/// @details  Returns the plug's name as a char*.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const char* GunnsBasicJumperPlug::getName() const
{
    return mName.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index (--) Index of the desired socket in the socket list.
///
/// @returns  int (--) The network node index resulting from the socket connection.
///
/// @throws   TsOutOfBoundsException, TsInvalidStateTransitionException
///
/// @details  Type-casts the socket pointer to a Basic Socket, connects to the socket and returns
///           the resulting connected node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsBasicJumperPlug::connectToSocket(const int index)
{
    return static_cast <GunnsBasicSocket*> (getSocket(index))->requestConnection(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index (--) Index of the desired socket in the socket list.
///
/// @returns  int (--) The network node index resulting from the socket disconnection.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Type-casts the socket pointer to a Basic Socket, disconnects from the socket and
///           returns the resulting connected node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsBasicJumperPlug::disconnectFromSocket(const int index)
{
    return static_cast <GunnsBasicSocket*> (getSocket(index))->requestDisconnection(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index (--) Index of the desired socket in the socket list.
///
/// @returns  bool (--) True if this plug is connected to the socket.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Type-casts the socket pointer to a Basic Socket and returns whether this plug is
///           connected to the socket.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicJumperPlug::isSocketConnected(const int index)
{
    return static_cast <GunnsBasicSocket*> (getSocket(index))->isConnected(this);
}

/// @}

#endif
