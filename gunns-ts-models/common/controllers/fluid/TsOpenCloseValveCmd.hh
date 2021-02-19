#ifndef TsOpenCloseValveCmd_EXISTS
#define TsOpenCloseValveCmd_EXISTS

/**
@file
@brief     TS21 Fluid Controller Open/Close Valve Command declarations.

@defgroup  TSM_CONTROLLER_FLUID_OPEN_CLOSE_VALVE_CMD  Open/Close Valve Command
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Type to use on command port for a TS21 Fluid Controller powered valve with command data consisting of enable, open
            and close command flags.)

@details
REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - (The user of this class is responsible for handling both open and close command flags true.)

 LIBRARY DEPENDENCY:
 - ()

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Open/Close Valve Command
///
/// @details  Powered valve composite command with enabled, open and close flags.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpenCloseValveCmd {
    public:
        bool          mEnable;   /**< (--)  Enable command flag.  */
        bool          mOpen;     /**< (--)  Open command flag.  */
        bool          mClose;    /**< (--)  Close command flag.  */
        /// @brief    Default constructs this Open/Close Valve command.
        TsOpenCloseValveCmd(const bool enable = false, const bool open = false, const bool close = false);
        /// @brief    Copy constructs this Open/Close Valve command.
        TsOpenCloseValveCmd(const TsOpenCloseValveCmd& that);
        /// @brief    Default destructs this Open/Close Valve command.
        virtual ~TsOpenCloseValveCmd();
        /// @brief    Assigns this Open/Close Valve command.
        TsOpenCloseValveCmd& operator=(const TsOpenCloseValveCmd& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  enable   (--)  Enable command flag.
/// @param[in]  open     (--)  Open command flag.
/// @param[in]  close    (--)  Close command flag.
///
/// @details    Default constructs this Powered Open/Close Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsOpenCloseValveCmd::TsOpenCloseValveCmd(const bool enable, const bool open, const bool close)
    :
    mEnable(enable),
    mOpen(open),
    mClose(close)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this Powered Open/Close Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsOpenCloseValveCmd::TsOpenCloseValveCmd(const TsOpenCloseValveCmd& that)
    :
    mEnable(that.mEnable),
    mOpen(that.mOpen),
    mClose(that.mClose)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Powered Open/Close Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsOpenCloseValveCmd::~TsOpenCloseValveCmd()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source assigned from.
///
/// @returns    TsOpenCloseValveCmd&  (--)  Destination assigned to.
///
/// @details    Assigns this Powered Open/Close Valve command.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsOpenCloseValveCmd& TsOpenCloseValveCmd::operator=(const TsOpenCloseValveCmd &that)
{
    if(this != &that) {
        mEnable  = that.mEnable;
        mOpen    = that.mOpen;
        mClose   = that.mClose;
    }
    return *this;
}

#endif
