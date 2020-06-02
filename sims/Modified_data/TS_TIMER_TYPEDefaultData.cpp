/******************************************************************************
Purpose:
  (Health and status timer default data class)
******************************************************************************/
#include "TS_TIMER_TYPEDefaultData.hh"

void
TS_TIMER_TYPEDefaultData::initialize(
        TS_TIMER_TYPE * timer_ptr)
{
    timer_ptr->pre     = 0;
    timer_ptr->day     = 0;
    timer_ptr->hour    = 0;
    timer_ptr->min     = 0;
    timer_ptr->sec     = 0;

    timer_ptr->seconds = 0.0;

    timer_ptr->hold    = 0;
}
