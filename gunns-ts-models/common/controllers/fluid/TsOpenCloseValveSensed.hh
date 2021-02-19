#ifndef TsOpenCloseValveSensed_EXISTS
#define TsOpenCloseValveSensed_EXISTS

/**
@file
@brief     TS21 Fluid Controller Open/Close Valve Sensed Position declarations.

@defgroup  TSM_CONTROLLER_FLUID_OPEN_CLOSE_VALVE_SENSED Open/Close Valve Sensed Position
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Type to use on sensor port for a TS21 Fluid Controller powered valve with sensor data consisting of open and close
            sensed position flags.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - (The user of this class is responsible for handling both open and closed sensed position flags
    true.)

 LIBRARY DEPENDENCY:
 - ()

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Open/Close Valve Sensed Position
///
/// @details  Powered valve composite sensed position with open and close flags.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsOpenCloseValveSensed
{
    public:
        bool          mOpen;     /**< (--)  Open sensed position flag.  */
        bool          mClose;    /**< (--)  Close sensed position flag.  */
        /// @brief    Default constructs this Open/Close Valve sensed position.
        TsOpenCloseValveSensed(const bool open = false, const bool close = false);
        /// @brief    Copy constructs this Open/Close Valve sensed position.
        TsOpenCloseValveSensed(const TsOpenCloseValveSensed& that);
        /// @brief    Default destructs this Open/Close Valve sensed position.
        virtual ~TsOpenCloseValveSensed();
        /// @brief    Assigns this Open/Close Valve sensed position.
        TsOpenCloseValveSensed& operator=(const TsOpenCloseValveSensed& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  open     (--)  Open sensed position flag.
/// @param[in]  close    (--)  Close sensed position flag.
///
/// @details    Default constructs this powered Open/Close Valve sensed position.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsOpenCloseValveSensed::TsOpenCloseValveSensed(const bool open, const bool close)
    :
    mOpen(open),
    mClose(close)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source to copy.
///
/// @details    Copy constructs this powered Open/Close Valve sensed position.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsOpenCloseValveSensed::TsOpenCloseValveSensed(const TsOpenCloseValveSensed& that)
    :
    mOpen(that.mOpen),
    mClose(that.mClose)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this powered Open/Close Valve sensed position.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsOpenCloseValveSensed::~TsOpenCloseValveSensed()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  that   (--)  Source assigned from.
///
/// @returns    TsOpenCloseValveSensed&  (--)  Destination assigned to.
///
/// @details    Assigns this powered Open/Close Valve sensed position.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline TsOpenCloseValveSensed& TsOpenCloseValveSensed::operator=(const TsOpenCloseValveSensed &that)
{
    if(this != &that) {
        mOpen    = that.mOpen;
        mClose   = that.mClose;
    }
    return *this;
}

#endif
