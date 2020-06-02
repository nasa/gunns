#ifndef GunnsFluidSocket_EXISTS
#define GunnsFluidSocket_EXISTS

/**
@file
@brief    GUNNS Fluid Socket Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_SOCKET    GUNNS Fluid Socket Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Socket Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsFluidSocket.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-08) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsFluidConductor.hh"

class GunnsFluidJumperPlug;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Socket Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Socket
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSocketConfigData : public GunnsFluidConductorConfigData
{
    public:
        bool mSelfSealing;                                 /**< (--) trick_chkpnt_io(**) Self-seals when disconnected */
        /// @brief Default constructs this Fluid Socket configuration data.
        GunnsFluidSocketConfigData(const std::string& name                 = "",
                                   GunnsNodeList*     nodes                = 0,
                                   const double       maxConductivity      = 0.0,
                                   const double       expansionScaleFactor = 0.0,
                                   const bool         selfSealing          = false);
        /// @brief Default destructs this Fluid Socket configuration data.
        virtual ~GunnsFluidSocketConfigData();
        /// @brief Copy constructs this Fluid Socket configuration data.
        GunnsFluidSocketConfigData(const GunnsFluidSocketConfigData& that);
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSocketConfigData& operator =(const GunnsFluidSocketConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Socket Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Socket
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSocketInputData : public GunnsFluidConductorInputData
{
    public:
        bool   mMalfLeakRateFlag;                         /**< (--)   Leak-thru malfunction flag  */
        double mMalfLeakRateValue;                        /**< (kg/s) Leak-thru malfunction value */
        /// @brief    Default constructs this Fluid Socket input data.
        GunnsFluidSocketInputData(const bool   malfBlockageFlag  = false,
                                  const double malfBlockageValue = 0.0,
                                  const bool   malfLeakRateFlag  = false,
                                  const double malfLeakRateValue = 0.0);
        /// @brief    Default destructs this Fluid Socket input data.
        virtual ~GunnsFluidSocketInputData();
        /// @brief    Copy constructs this Fluid Socket input data.
        GunnsFluidSocketInputData(const GunnsFluidSocketInputData& that);
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSocketInputData& operator =(const GunnsFluidSocketInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Socket Class
///
/// @details  The Fluid Socket class controls access of Jumper links to network nodes.  In an
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
class GunnsFluidSocket : public GunnsFluidConductor
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidSocket);

    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool                        mMalfLeakRateFlag;  /**< (--)   Leak-thru malfunction flag    */
        double                      mMalfLeakRateValue; /**< (kg/s) Leak-thru malfunction value   */
        /// @}

        /// @brief  Default Fluid Socket Constructor
        GunnsFluidSocket();
        /// @brief  Default Fluid Socket Destructor
        virtual ~GunnsFluidSocket();
        /// @brief  Initialize method
        void initialize(const GunnsFluidSocketConfigData& configData,
                        const GunnsFluidSocketInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);
        /// @brief  Processes request from a plug to connect to this socket.
        int  requestConnection(const GunnsFluidJumperPlug* plug);
        /// @brief  Processes request from a plug to disconnect from this socket.
        int  requestDisconnection(const GunnsFluidJumperPlug* plug);
        /// @brief  Returns true if the requesting plug is connected to this socket.
        bool isConnected(const GunnsFluidJumperPlug* plug) const;
        /// @brief  Returns true if any plug is connected to this socket.
        bool isConnected() const;
        /// @brief  Sets and resets the leak rate malfunction
        void setMalfLeakRate(const bool flag = false, const double value = 0.0);

    protected:
        bool                        mSelfSealing;       /**< (--)   trick_chkpnt_io(**) Self-seals when disconnected  */
        const GunnsFluidJumperPlug* mConnection;        /**< (--)                       Pointer to connected plug     */
        bool                        mHasConnection;     /**< (--)                       Has a plug connected          */
        double                      mPreviousLeakRate;  /**< (kg/s)                     Previous leak malf value      */
        double                      mLeakConductivity;  /**< (m2)                       Conductivity of the leak-thru */
        /// @brief  Validates the Fluid Socket input data.
        void validate(const GunnsFluidSocketInputData& inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Checks the link initialization was complete.
        void checkInitialization() const;
        /// @brief Updates the state of the Fluid Socket.
        virtual void updateState(const double);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSocket(const GunnsFluidSocket& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSocket& operator =(const GunnsFluidSocket& that);
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
inline bool GunnsFluidSocket::isConnected(const GunnsFluidJumperPlug* plug) const
{
    return plug == mConnection;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool (--) True if any plug is currently connected to this socket.
///
/// @details  This overloaded version of the method returns true if any plug is currently connected
///           to this socket, otherwise returns false.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidSocket::isConnected() const
{
    return (0 != mConnection);
}

#endif
