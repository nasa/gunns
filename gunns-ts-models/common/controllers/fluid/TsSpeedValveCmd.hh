#ifndef TsSpeedValveCmd_EXISTS
#define TsSpeedValveCmd_EXISTS

/**
@file
@brief     TS21 Fluid Controller Speed Valve Command declarations.

@defgroup  TSM_CONTROLLER_FLUID_SPEED_VALVE_CMD  Speed Valve Command
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Type to use on command port for a TS21 Fluid Controller powered valve with command data consisting of an enable flag
            and a speed value.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - (The user of this class is responsible for scaling the speed command to the commanded fractional position rate.)

 LIBRARY DEPENDENCY:
 - ()

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Speed Valve Command
///
/// @details  Powered valve composite command with enabled flag and speed value.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsSpeedValveCmd {
    public:
        bool          mEnable;  /**< (--)  Enable command flag.  */
        double        mSpeed;   /**< (--)  Speed command value.  */
        /// @brief    Default constructs this Speed Valve command.
        TsSpeedValveCmd(const bool enable = false, const double speed = 0.0);
        /// @brief    Copy constructs this Speed Valve command.
        TsSpeedValveCmd(const TsSpeedValveCmd& that);
        /// @brief    Default destructs this Speed Valve command.
        virtual ~TsSpeedValveCmd();
        /// @brief    Assigns this Speed Valve command.
        TsSpeedValveCmd& operator=(const TsSpeedValveCmd& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  enable  (--)  Enable command flag.
/// @param[in]  speed   (--)  Speed command value.
///
/// @details    Default constructs this Powered Speed Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsSpeedValveCmd::TsSpeedValveCmd(const bool enable, const double speed)
    :
    mEnable(enable),
    mSpeed(speed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Powered Speed Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsSpeedValveCmd::TsSpeedValveCmd(const TsSpeedValveCmd& that)
    :
    mEnable(that.mEnable),
    mSpeed(that.mSpeed)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Speed Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsSpeedValveCmd::~TsSpeedValveCmd()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source assigned from.
///
/// @returns    TsSpeedValveCmd&  (--)  Destination assigned to.
///
/// @details    Assigns this Powered Speed Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsSpeedValveCmd& TsSpeedValveCmd::operator=(const TsSpeedValveCmd &that)
{
    if(this != &that) {
        mEnable = that.mEnable;
        mSpeed  = that.mSpeed;
    }
    return *this;
}

#endif
