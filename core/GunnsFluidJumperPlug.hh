#ifndef GunnsFluidJumperPlug_EXISTS
#define GunnsFluidJumperPlug_EXISTS

/**
@file
@brief    GUNNS Fluid Jumper Plug Class declarations

@defgroup  TSM_GUNNS_FLUID_JUMPER_PLUG    GUNNS Fluid Jumper Plug Class
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Class for the GUNNS Fluid Jumper Plug.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidJumperPlug.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2012-08))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsBasicJumperPlug.hh"
#include "GunnsFluidSocket.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Jumper Plug
///
/// @details  The GUNNS Fluid Jumper Plug class serves as an interface between GUNNS Fluid Jumper
///           and Fluid Socket links, similarly to the GUNNS Basic Jumper Plug class.  See the
///           description of GunnsBasicJumperPlug.  This class performs in identical fashion in
///           fluid networks.
///
///           This is a pure virtual class, and is intended to be extended in a network manager.
///           The derived class will contain an enumerated list of socket names, control targets for
///           the users to request connection, disconnection, and current connection state of the
///           plugs, and implements these virtual methods to allow access to those target parameters
///           by the jumper links.
///
/// @note     This class is NOT a GUNNS link.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidJumperPlug : public GunnsBasicJumperPlug
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidJumperPlug);
    public:
        /// @brief   Default constructs this Fluid Jumper Plug.
        GunnsFluidJumperPlug(const std::string& name, const int listSize);
        /// @brief   Default destructs this Fluid Jumper Plug.
        virtual ~GunnsFluidJumperPlug();

    protected:
        /// @brief   Processes direct connection requests for the plug.
        virtual void processDirectConnectionRequest();
        /// @brief   Requests connection to the socket.
        virtual int  connectToSocket(const int index);
        /// @brief   Requests disconnection from the socket.
        virtual int  disconnectFromSocket(const int index);
        /// @brief   Requests connection status from the socket.
        virtual bool isSocketConnected(const int index);

    private:
        /// @brief   Copy constructor is not implemented.
        GunnsFluidJumperPlug(const GunnsFluidJumperPlug&);
        /// @brief   Assignment operator is not implemented.
        GunnsFluidJumperPlug& operator =(const GunnsFluidJumperPlug&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index (--) Index of the desired socket in the socket list.
///
/// @returns  int (--) The network node index resulting from the socket connection.
///
/// @throws   TsOutOfBoundsException, TsInvalidStateTransitionException
///
/// @details  Type-casts the socket pointer to a Fluid Socket, connects to the socket and returns
///           the resulting connected node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsFluidJumperPlug::connectToSocket(const int index)
{
    return static_cast <GunnsFluidSocket*> (getSocket(index))->requestConnection(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index (--) Index of the desired socket in the socket list.
///
/// @returns  int (--) The network node index resulting from the socket disconnection.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Type-casts the socket pointer to a Fluid Socket, disconnects from the socket and
///           returns the resulting connected node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int GunnsFluidJumperPlug::disconnectFromSocket(const int index)
{
    return static_cast <GunnsFluidSocket*> (getSocket(index))->requestDisconnection(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] index (--) Index of the desired socket in the socket list.
///
/// @returns  bool (--) True if this plug is connected to the socket.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Type-casts the socket pointer to a Fluid Socket and returns whether this plug is
///           connected to the socket.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidJumperPlug::isSocketConnected(const int index)
{
    return static_cast <GunnsFluidSocket*> (getSocket(index))->isConnected(this);
}

/// @}

#endif
