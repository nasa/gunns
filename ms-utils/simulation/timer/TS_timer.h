#ifndef TS_timer_EXISTS
#define TS_timer_EXISTS
/**
@defgroup  TSM_UTILITIES_SIMULATION_TIMER_MODEL  Timer Model (C)
@ingroup   TSM_UTILITIES_SIMULATION_TIMER

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
    (Set up the data structures for a MET timer)

SWIG: (No)

REFERENCE:
    (TBD)

ASSUMPTIONS AND LIMITATIONS:
    ((TBD))

PROGRAMMERS:
   ( ((Paul Carradine) (L3) (June 2009) (CxTF)) )

@{
*/

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// Timer
///
/// This unit provides a timer model that will be used for MET (Mission Elapsed Time).  The "pre"
/// flag is set to "-1" for pre-launch while the MET timer is counting down and "1" at launch.
/// Seconds is the total number of seconds (plus or minus) to/from launch.
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  This data structure contains the TIMER state.
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct TS_TIMER_TAG {

    int       pre;        // --   Pre launch flag (-1=pre launch, 1=post launch)
    int       day;        // --   Day
    int       hour;       // --   Hour
    int       min;        // --   Minute
    int       sec;        // --   Second
    double    seconds;    // --   Seconds since or prior to launch

    int       hold;       // --   Hold flag (1=hold, 0=count)

} TS_TIMER_TYPE;

//=================================================================================
// This is a special global pointer that is used by the Health and Status message routine to get the
// MET time for timestamping messages.
//=================================================================================
extern TS_TIMER_TYPE * ts_global_met_ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void TS_timer_update(             // RETURN: --   void
    TS_TIMER_TYPE *timer_ptr,     // INOUT   --   Timer data structure
    double          dt);           // IN:     --   delta time

void TS_timer_init(                // RETURN: --   void
   TS_TIMER_TYPE *timer_ptr);      // INOUT   --   Timer data structure

#ifdef __cplusplus
}
#endif

/// @}

#endif
