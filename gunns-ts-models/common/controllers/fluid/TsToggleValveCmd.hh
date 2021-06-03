#ifndef TsToggleValveCmd_EXISTS
#define TsToggleValveCmd_EXISTS

/**
@file
@brief     TS21 Fluid Controller Toggle Valve Command declarations.

@defgroup  TSM_CONTROLLER_FLUID_TOGGLE_VALVE_CMD  Toggle Valve Command
@ingroup   TSM_CONTROLLER_FLUID

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Type to use as command for a TS21 Fluid Controller powered valve with command data consisting of an enable flag
            and a toggle flag.)

@details
 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - (The user of this class is responsible for determining the position to toggle to.)

 LIBRARY DEPENDENCY:
 - ((TsToggleValveCmd.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2012-10)

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    TS21 Fluid Controller Toggle Valve Command
///
/// @details  Powered valve composite command with enabled and toggle flags. Queues are supported,
///           in which case the getters should be used to process the queue and return the result.
////////////////////////////////////////////////////////////////////////////////////////////////////
class TsToggleValveCmd {
    TS_MAKE_SIM_COMPATIBLE(TsToggleValveCmd);
    public:
        bool          mEnable;             /**< (--)  Enable command flag.  */
        bool          mToggle;             /**< (--)  Toggle command flag.  */
        /// @brief    Default constructs this Toggle Valve command.
        TsToggleValveCmd(const bool enable = false, const bool toggle = false);
        /// @brief    Default destructs this Toggle Valve command.
        virtual ~TsToggleValveCmd();
        /// @brief    Get the enable command.
        bool getEnable()  const;
        /// @brief    Get the toggle command.
        bool getToggle()  const;
        /// @brief    Updates this toggle valve command.
        /// @callgraph
        void update(const double dt);
    protected:
        bool*         mEnable_queue;       /**< (--)  Pointer to enable command flag queue.  */
        bool*         mToggle_queue;       /**< (--)  Pointer to toggle command flag queue.  */
        int           mEnable_queue_size;  /**< (--)  Enable command flag queue size.  */
        int           mToggle_queue_size;  /**< (--)  Toggle command flag queue size.  */
        bool          mArmed;              /**< (--)  Toggle command armed flag.  */
        int           mToggleCount;        /**< (--)  Toggle counter for debug. */
    private:
        /// @brief    Unavailable since declared private and not implemented.
        TsToggleValveCmd(const TsToggleValveCmd&);
        /// @brief    Unavailable since declared private and not implemented.
        TsToggleValveCmd& operator=(const TsToggleValveCmd&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (--) Toggle command.
///
/// @details  Gets the Powered Toggle Valve enable command resulting from queue processing.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsToggleValveCmd::getToggle() const
{
     return mToggle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (--) Enable command.
///
/// @details  Gets the Powered Toggle Valve enable command resulting from queue processing.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool TsToggleValveCmd::getEnable() const
{
     return mEnable;
}

#endif
