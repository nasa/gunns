/**
@file
@brief    GUNNS Fluid Jumper Plug Class implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Class for the GUNNS Fluid Jumper Plug.)

REQUIREMENTS:
   ()

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY DEPENDENCY:
   ((GunnsFluidSocket.o)
    (GunnsBasicJumperPlug.o))

PROGRAMMERS:
   ((Jason Harvey) (L-3 Communications) (Initial) (2012-08))
*/

#include "GunnsFluidJumperPlug.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name     (--) The name of the plug object for error messages.
/// @param[in] listSize (--) The number of sockets that will be in the list.
///
/// @details  Default constructs this Fluid Jumper Plug.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumperPlug::GunnsFluidJumperPlug(const std::string& name, const int listSize)
    :
    GunnsBasicJumperPlug(name, listSize)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Jumper Plug.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsFluidJumperPlug::~GunnsFluidJumperPlug()
{
    // nothing to do
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
void GunnsFluidJumperPlug::processDirectConnectionRequest()
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
            GunnsFluidSocket* socket = static_cast<GunnsFluidSocket*>(getSocket(mDirectConnectionRequest));
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
