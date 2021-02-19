/************************ TRICK HEADER ******************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (Timer model)

REFERENCE:
    ()

ASSUMPTIONS AND LIMITATIONS:
    ()

LIBRARY DEPENDENCY:
    (
     ()
    )

PROGRAMMERS:
   (
    ( (Paul Carradine) (L3) (June 2009) (Initial model) )
   )
********************************************************************************/
#include "TS_timer.h"

//=================================================================================
// This is a special global pointer that is used by the Health and Status message routine to get the
// MET time for timestamping messages.
//=================================================================================
TS_TIMER_TYPE * ts_global_met_ptr;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param     timer_ptr     Pointer to the timer data structure
/// @param     dt            time step
///
/// Timer model for MET (Mission Elapsed Time).
///
/// This unit provides a timer model that will be used for MET (Mission Elapsed Time).  The "pre"
/// flag is set to "-1" for pre-launch while the MET timer is counting down and "1" at launch.
/// Seconds is the total number of seconds (plus or minus) to/from launch.
///
/// @note This gets compiled with "cc -Wtraditional -Wold-style-definition" so it is impossible
///       to compile w/o warnings. If you use ANSI style arguments you will get a
///       "warning: traditional C rejects ISO C style function definitions" message. If you use
///       K&R style argument you will get an "warning: old-style function definition" warning !!!
////////////////////////////////////////////////////////////////////////////////////////////////////
void TS_timer_update(TS_TIMER_TYPE *timer_ptr, double dt)
{
   int pre;     // pre launch flag, -1 prior to launch, 1 after launch
   int days;    // temp MET days
   int hours;   // temp MET hours
   int mins;    // temp MET minutes
   int secs;    // temp MET seconds

   double seconds = timer_ptr->seconds;  // temp MET total seconds

   //------------------------------------------------------------
   // If no hold event is ocurring, increment counter
   //------------------------------------------------------------
   if ( !timer_ptr->hold ) {
      seconds += dt;
   }

   //------------------------------------------------------------
   // Set the pre launch flag
   //------------------------------------------------------------
   if (seconds < 0.0) {
      pre = -1;
   } else {
      pre = 1;
   }

   //------------------------------------------------------------
   // Multiply by pre-launch flag so that these display (time) variables are not negative
   //------------------------------------------------------------
   days  = pre * ( (int)seconds / 86400 );
   hours = pre * ( (int)(seconds / 3600) - (days * 24) );
   mins  = pre * ( (int)(seconds / 60) - (days * 1440) - (hours * 60) );
   secs  = pre * ( (int)seconds % 60 );

   timer_ptr->pre     = pre;
   timer_ptr->day     = days;
   timer_ptr->hour    = hours;
   timer_ptr->min     = mins;
   timer_ptr->sec     = secs;
   timer_ptr->seconds = seconds;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param     timer_ptr     Pointer to the timer data structure
///
/// This function sets up the timer model and should be called proir to entering RUN mode.
///
/// @note This gets compiled with "cc -Wtraditional -Wold-style-definition" so it is impossible
///       to compile w/o warnings. If you use ANSI style arguments you will get a
///       "warning: traditional C rejects ISO C style function definitions" message. If you use
///       K&R style argument you will get an "warning: old-style function definition" warning !!!
///////////////////////////////////////////////////////////////////////////////////////////////////
void TS_timer_init(TS_TIMER_TYPE* timer_ptr)
{

   TS_timer_update( timer_ptr, 0.0);

   //=================================================================================
   // set global pointer (ts_global_met_ptr) to the timer data
   // NOTE: see the header file for the declaration of ts_global_met_ptr
   //=================================================================================
   ts_global_met_ptr = timer_ptr;
}

