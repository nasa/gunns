#ifndef GunnsBasicSocket_EXISTS
#define GunnsBasicSocket_EXISTS

/**
@file
@brief    GUNNS Basic Socket Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_SOCKET    GUNNS Basic Socket Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic Socket Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsBasicSocket.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-08) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsBasicConductor.hh"

class GunnsBasicJumperPlug;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Socket Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Socket
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicSocketConfigData : public GunnsBasicConductorConfigData
{
    public:
        bool mSelfSealing;             /**< (--) trick_chkpnt_io(**) Self-seals when disconnected */
        /// @brief Default constructs this Basic Socket configuration data.
        GunnsBasicSocketConfigData(const std::string& name                = "",
                                   GunnsNodeList*     nodes               = 0,
                                   const double       defaultConductivity = 0.0,
                                   const bool         selfSealing         = false);
        /// @brief Default destructs this Basic Socket configuration data.
        virtual ~GunnsBasicSocketConfigData();
        /// @brief Copy constructs this Basic Socket configuration data.
        GunnsBasicSocketConfigData(const GunnsBasicSocketConfigData& that);
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicSocketConfigData& operator =(const GunnsBasicSocketConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Socket Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Socket
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicSocketInputData : public GunnsBasicConductorInputData
{
    public:
        /// @brief    Default constructs this Basic Socket input data.
        GunnsBasicSocketInputData(const bool   malfBlockageFlag  = false,
                                  const double malfBlockageValue = 0.0);
        /// @brief    Default destructs this Basic Socket input data.
        virtual ~GunnsBasicSocketInputData();
        /// @brief    Copy constructs this Basic Socket input data.
        GunnsBasicSocketInputData(const GunnsBasicSocketInputData& that);
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicSocketInputData& operator =(const GunnsBasicSocketInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Socket Class
///
/// @details  The Basic Socket class controls access of Jumper links to network nodes.  In an
///           electrical network, a socket would be a hole in a circuit breadboard, or an electrical
///           socket in the wall of your house, etc, whereas a jumper is a cable that plugs into the
///           socket.  The socket is itself a conductor between the access-conrolled node and an
///           ambient node (usually ground or the surrounding "room").  Thus, a non-self-sealing
///           socket would discharge to the ambient node when not connected.  This feature is mainly
///           included for symmetry with the fluid version.
///
///           A socket controls access of jumpers to the controlled node by remembering which jumper
///           plug is currently connected, and only allowing one plug to connect at a time.  A plug
///           requests to connect to or disconnect one of its plugs from the socket.  The socket
///           responds by returning the network node index of the resulting node that the request
///           should leave that end of the jumper (the jumper link port) connected to.  When
///           connection is successful, the jumper gets the access-controlled node.  When
///           disconnecting, the jumper gets the socket's ambient node.  Thus, unplugging a cable
///           from your wall outlet results in the cable being "connected" to the ambient node, i.e.
///           the room.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicSocket : public GunnsBasicConductor
{
        TS_MAKE_SIM_COMPATIBLE(GunnsBasicSocket);

    public:
        /// @brief  Default Basic Socket Constructor
        GunnsBasicSocket();
        /// @brief  Default Basic Socket Destructor
        virtual ~GunnsBasicSocket();
        /// @brief  Initialize method
        void initialize(const GunnsBasicSocketConfigData& configData,
                        const GunnsBasicSocketInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);
        /// @brief  Processes request from a plug to connect to this socket.
        int  requestConnection(const GunnsBasicJumperPlug* plug);
        /// @brief  Processes request from a plug to disconnect from this socket.
        int  requestDisconnection(const GunnsBasicJumperPlug* plug);
        /// @brief  Returns true if the requesting plug is connected to this socket.
        bool isConnected(const GunnsBasicJumperPlug* plug) const;
        /// @brief  Returns true if any plug is connected to this socket.
        bool isConnected() const;

    protected:
        bool                        mSelfSealing;   /**< (--) trick_chkpnt_io(**) Self-seals when disconnected      */
        const GunnsBasicJumperPlug* mConnection;    /**< (--)                     Pointer to connected plug or null */
        bool                        mHasConnection; /**< (--)                     Has a plug connected              */
        /// @brief  Checks the link initialization was complete
        void checkInitialization() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Updates the State of the Basic Socket
        virtual void updateState(const double);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicSocket(const GunnsBasicSocket& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicSocket& operator =(const GunnsBasicSocket& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] plug (--) Pointer to the the plug requesting its connection status.
///
/// @return   bool (--) Connection status of the plug, true if connected to this socket.
///
/// @details  This overloaded version of the method returns true if the plug is currently connected
///           to this socket, otherwise returns false.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicSocket::isConnected(const GunnsBasicJumperPlug* plug) const
{
    return plug == mConnection;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool (--) True if any plug is currently connected to this socket.
///
/// @details  This overloaded version of the method returns true if any plug is currently connected
///           to this socket, otherwise returns false.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsBasicSocket::isConnected() const
{
    return mHasConnection;
}

#endif
