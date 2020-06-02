#ifndef TsPositionValveCmd_EXISTS
#define TsPositionValveCmd_EXISTS

/**
@file
@brief     TS21 Fluid Controller Position Valve Command declarations.

@defgroup  TSM_CONTROLLER_FLUID_POSITION_VALVE_CMD Position Valve Command
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Type to use on command port for a TS21 Fluid Controller powered valve with command data consisting of an enable flag
            and a position value.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - (The user of this class is responsible for scaling the position command to the commanded fractional position.)

 LIBRARY DEPENDENCY:
 - ()

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Position Valve Command
///
/// @details  Powered valve composite command with enabled flag and position value.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsPositionValveCmd {
    public:
        bool          mEnable;   /**< (--)  Enable command flag.  */
        double        mPosition; /**< (--)  Position command value.  */
        /// @brief    Default constructs this Position Valve command.
        TsPositionValveCmd(const bool enable = false, const double position = 0.0);
        /// @brief    Copy constructs this Position Valve command.
        TsPositionValveCmd(const TsPositionValveCmd& that);
        /// @brief    Default destructs this Position Valve command.
        virtual ~TsPositionValveCmd();
        /// @brief    Assigns this Position Valve command.
        TsPositionValveCmd& operator=(const TsPositionValveCmd& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  enable   (--)  Enable command flag.
/// @param[in]  position (--)  Position command value.
///
/// @details    Default constructs this Powered Position Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsPositionValveCmd::TsPositionValveCmd(const bool enable, const double position)
    :
    mEnable(enable),
    mPosition(position)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Powered Position Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsPositionValveCmd::TsPositionValveCmd(const TsPositionValveCmd& that)
    :
    mEnable(that.mEnable),
    mPosition(that.mPosition)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Position Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsPositionValveCmd::~TsPositionValveCmd()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source assigned from.
///
/// @returns    TsPositionValveCmd&  (--)  Destination assigned to.
///
/// @details    Assigns this Powered Position Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsPositionValveCmd& TsPositionValveCmd::operator=(const TsPositionValveCmd &that)
{
    if(this != &that) {
        mEnable   = that.mEnable;
        mPosition = that.mPosition;
    }
    return *this;
}

#endif
