/*******************************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
   (SGMT time conversion routines.)
REFERENCE:
   (N/A)
ASSUMPTIONS AND LIMITATIONS:
   (N/A)
CLASS:
   (N/A)
LIBRARY DEPENDENCY:
   ((TS_sgmt_conversion.o)
    (simulation/hs/TsHsMsgWrapper.o)
PROGRAMMERS:
   (((Hadi Tjandrasa) (L3-Com) (Sep 2009) (Initial version)))
   (((Jim Reeves) (GHG) (November 2010) (Moved some constants to CMN/constants and
                                         created a function to validate a UTC structure and
                                         another function to subtract two UTC structures)))
*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "TS_sgmt_conversion.h"
#include "simulation/hs/TsHsMsgWrapper.hh"


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   year        IN: Year in 4 digit integer.
/// @param   *sec_ptr   OUT: This (one) given year in seconds.
///
/// @details
///  A function to convert the given year to seconds.
///
/// @return   TS_YES (Leap year) or TS_NO (Common year).
///
///////////////////////////////////////////////////////////////////////////////////////////////////

TS_FLAG  TS_convert_year2second(
  int      year,           /*     IN: --  Year */
  double  *sec_ptr)        /*    OUT: s   Seconds */
{
  TS_FLAG ret_stat;       // Return status

   /// Determine if it's a leap or common year and return the corresponding seconds.
  ret_stat = TS_is_leap_year(year);
  if ( ret_stat == TS_YES ) {
     *sec_ptr = TS_LEAP_YEAR_SEC;
  }
  else {
     *sec_ptr = TS_COMMON_YEAR_SEC;
  }

  return( ret_stat );

}/*EOF TS_convert_year2second*/


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   year   IN: Year in 4 digit integer.
/// @param   month  IN: Month (1 to 12)
///
/// @details
///  A function to convert the given month (1 to 12) to days (1 up to 366).
///
/// @return   The number of (accumulated) days (1 - 335) for the given month; 0 otherwise.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

int  TS_convert_month2day(
  int   year,            /*     IN: --  Year */
  int   month)           /*     IN: --  Month (1-12) */
{
  static int list_common[]= {0, 0,31,59,90,120,151,181,212,243,273,304,334};  //Days in common month
  static int   list_leap[]= {0, 0,31,60,91,121,152,182,213,244,274,305,335};  //Days in leap month
  int  days = 0;

   /// If the given month is within the correct range, convert it to days.
   /// It evaluates the year to get the proper list for conversion.
  if ( (month > 0) && (month < 13) ) {
    if ( TS_is_leap_year(year) == TS_YES ) {
       days = list_leap[month];
    }
    else {
     days = list_common[month];
    }
  }

  return( days );

}/*EOF TS_convert_month2day*/


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  *UTC_ptr    IN: Pointer to an SGMT structure.
/// @param  *sec_ptr   OUT: Pointer to a time in seconds.
///
/// @details
///  A function to Convert UTC (Y,M,D,H,M,S) structure to seconds. The year is not included
///  (computed) in the conversion. The year is used only to determine whether
///  it is a leap or common year.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

void TS_convert_time_sgmt2second(
  TS_TIME_UTC_S_TYPE  *UTC_ptr,      /* IN: --  Time structure Y,M,D,H,M,S */
  double               *sec_ptr)     /* OUT: s  Time in seconds */
{
  int  mdays;                        // Days in a month

  mdays = TS_convert_month2day( UTC_ptr->year, UTC_ptr->month );

   /// NOTE: Jan 1 @ 9am ==> 001:09:00:00
  *sec_ptr = (mdays + (UTC_ptr->day-1))*TS_SEC_PER_DAY +
             UTC_ptr->hour*(int)TS_SEC_PER_HR +
             UTC_ptr->minute*TS_SEC_PER_MIN + UTC_ptr->second;

}/*EOF TS_convert_time_sgmt2second*/


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  *UTC_ptr    IN: Pointer to an SGMT structure.
/// @param  *dhms_ptr  OUT: Pointer to a time in "DDD:HH:MM:SS" format.
///
/// @details
///  A functon to Convert UTC (Y,M,D,H,M,S) structure to a "DDD:HH:MM:SS" string,
///  excluding the year. The year is used only to detetermine whether
///  it is a leap or common year.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

void  TS_convert_time_sgmt2string(
  TS_TIME_UTC_S_TYPE  *UTC_ptr,      /* IN:  --  Countdown clock unit */
  char                 *dhms_ptr)     /* OUT: --  Time structure Hour,Min,Sec */
{
  int  mdays;                         // Days in a month

  mdays = TS_convert_month2day( UTC_ptr->year, UTC_ptr->month );

   /// NOTE: Jan 1 @ 9am ==> 001:09:00:00
   ///       "second" is chopped (int), assumed already rounded-up.
  sprintf(dhms_ptr, "%03d:%02d:%02d:%02d",
          mdays+UTC_ptr->day, UTC_ptr->hour, UTC_ptr->minute, (int)floor(UTC_ptr->second));

}/*EOF TS_convert_time_sgmt2string*/


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   iformat      IN: Format Flag: TS_SGMT_COLON_FORMAT or TS_SGMT_NO_COLON_FORMAT.
/// @param   seconds      IN: Total seconds representing days, hours, minutes and seconds.
/// @param  *year_ptr  INOUT: Pointer to a year in 4 digit integer.
/// @param  *sgmt_ptr    OUT: Pointer to a string in "YY:DDD:HH:MM:SS" or in "YYDDDHHMMSS" format.
///
/// @details
///  A function to Convert a given year plus seconds to either a string format "YY:DDD:HH:MM:SS"
///  or "YYDDDHHMMSS".
///  It will udpate the year if the seconds conversion overflows to the next year.
///            or in "YYDDDHHMMSS" for TS_SGMT_NO_COLON_FORMAT.
///
/// @return   TS_YES(Good) or TS_NO(Bad).
///////////////////////////////////////////////////////////////////////////////////////////////////

TS_FLAG  TS_convert_yseconds2sgmt(
  TS_SGMT_FORMAT_E_TYPE iformat,     /*     IN: --  Format flag */
  double                 seconds,     /*     IN: s   Total seconds */
  int                   *year_ptr,    /*  INOUT: --  Year in 4 digits */
  char                  *sgmt_ptr)    /*    OUT: --  SGMT string */
{
  double   ysec;                          // Year in seconds
  int      dtemp;                         // Days
  int      htemp;                         // Hours
  int      stemp;                         // Seconds
  TS_FLAG leap_flag;                     // Leap or common flag
  TS_FLAG ret_status = TS_YES;  // Return status

  leap_flag = TS_convert_year2second( *year_ptr, &ysec );

   /// Check if seconds overflows to next year
  if ( seconds >= ysec ) {
     *year_ptr += 1;
     seconds -= ysec;
  }

   /// Compute days and hours.
  stemp = (int)(seconds + 0.5);   // Round up instead of chopping
  dtemp = stemp / (int)TS_SEC_PER_DAY;
  htemp = (stemp - dtemp*(int)TS_SEC_PER_DAY) / (int)TS_SEC_PER_HR;

   /// Convert to SGMT string per requested format.
  if (iformat == TS_SGMT_NO_COLON_FORMAT) {
     sprintf( sgmt_ptr,"%02d%03d%02d%02d%02d",
                (*year_ptr % 100),
                dtemp + 1,
                htemp,
                ( ( stemp - ((dtemp*(int)TS_SEC_PER_DAY) + (htemp*(int)TS_SEC_PER_HR)) ) /
                  (int)TS_MIN_PER_HR ),
                stemp % (int)TS_SEC_PER_MIN );
  }
  else if (iformat == TS_SGMT_COLON_FORMAT) {
     sprintf( sgmt_ptr,"%02d:%03d:%02d:%02d:%02d",
                (*year_ptr % 100),
                dtemp + 1,
                htemp,
                ( ( stemp - ((dtemp*(int)TS_SEC_PER_DAY) + (htemp*(int)TS_SEC_PER_HR)) ) /
                  (int)TS_MIN_PER_HR ),
                stemp % (int)TS_SEC_PER_MIN );
  }
  else {
     ret_status = TS_NO;
  }

  return( ret_status );

}/*EOF TS_convert_yseconds2sgmt*/


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param  year   IN: Year in 4 digit integer.
///
/// @details
///  A function to determine if the given year is a leap or common year.
///
/// @return   TS_YES (Leap year) or TS_NO (Common year).
///
///////////////////////////////////////////////////////////////////////////////////////////////////

TS_FLAG  TS_is_leap_year(
  int      year )          /*     IN: --  Year */
{
  TS_FLAG ret_stat;       // Return status

  if ( (!(year % 4) && (year % 100)) || !(year % 400) ) {
     ret_stat = TS_YES ;
  }
  else {
     ret_stat = TS_NO ;
  }

  return( ret_stat );

}/*EOF TS_is_leap_year*/


///////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   job_cycle     IN: Job cycle in second.
/// @param  *hms_ptr       IN: Pointer to a time in "-HH:MM:SS" or "HH:MM:SS" string.
/// @param  *count_ptr    OUT: Pointer to a time in countdown cycle.
///
/// @details
///  A function to convert "-HH:MM:SS" or "HH:MM:SS" time string to countdown unit.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

void  TS_convert_time_string_hms2count(
  double  job_cycle,                 /* IN:  --  Job cycle in seconds */
  char   *hms_ptr,                   /* IN:  --  Time HH:MM:SS string */
  int    *count_ptr)                 /* OUT: --  Time in countdown unit */
{
  int   slen;         // string length

  slen = strlen(hms_ptr);

  if (CMN_N_HMS_TIME_SIZE == slen) {
     if (hms_ptr[0] != '-') {
        TS_hs_msg(TS_HS_ERROR, "LPS", "Countdown cycle must be negative (%s)", hms_ptr);
     }
     else {
        *count_ptr = (int)floor((double)(-(atoi(&hms_ptr[1])*(int)TS_SEC_PER_HR +
                     atoi(&hms_ptr[4])*(int)TS_MIN_PER_HR + atoi(&hms_ptr[7]))) / job_cycle);
     }
  }
  else if ((CMN_N_HMS_TIME_SIZE-1) == slen) {
     *count_ptr = (int)floor((double)(atoi(hms_ptr)*(int)TS_SEC_PER_HR +
                  atoi(&hms_ptr[3])*(int)TS_MIN_PER_HR + atoi(&hms_ptr[6])) / job_cycle);
  }
  else {
     TS_hs_msg(TS_HS_ERROR, "LPS", "hh:mm:ss format is incorrect (%s)", hms_ptr);
  }

}/*EOF TS_convert_time_string_hms2count*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   utc   -- UTC structure
///
/// @details
/// For the provided UTC structure returns the number of days that are in the current month if
/// the year and month are valid.  Returns zero if year and/or month is not valid.
////////////////////////////////////////////////////////////////////////////////////////////////////
int TS_UTC_days_in_month(                  /* RETURN -- Return days in the current month         */
  TS_TIME_UTC_S_TYPE  utc)                 /* IN:    -- UTC structure                            */
{
  int days_in_month = 0;

  if ((utc.year  >= TS_MIN_YEAR) && (utc.year  <= TS_MAX_YEAR       ) &&
      (utc.month >              0) && (utc.month <= TS_MONTHS_PER_YEAR)) {

      int *start_day     = TS_START_DAY_NORM;

      if (TS_is_leap_year(utc.year)) {
          start_day = TS_START_DAY_LEAP;
      }

      days_in_month = start_day[utc.month + 1] - start_day[utc.month];
  }

  return days_in_month;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   utc   -- UTC structure
///
/// @details
/// Check the time in the UTC structure and return TS_TRUE if it is valid.
////////////////////////////////////////////////////////////////////////////////////////////////////
TS_FLAG TS_UTC_is_valid(                  /* RETURN -- TS_TRUE if time is valid               */
  TS_TIME_UTC_S_TYPE  utc)                 /* IN:    -- UTC structure                            */
{
  TS_FLAG  retval = TS_FALSE;

  int days_in_month = TS_UTC_days_in_month(utc);

  if (days_in_month > 0) {

      if ((utc.day    >  0) && (utc.day    <=      days_in_month  ) &&
          (utc.hour   >= 0) && (utc.hour   <  (int)TS_HR_PER_DAY) &&
          (utc.minute >= 0) && (utc.minute <  (int)TS_MIN_PER_HR) &&
          (utc.second >= 0) && (utc.second <       TS_SEC_PER_HR)) {

          retval = TS_TRUE;
      }
  }

  return retval;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   utc_a   -- Time structure holding UTC a
/// @param   utc_b   -- Time structure holding UTC b
///
/// @details
/// Subtracts UTC structure  "b" from "a" and returns the difference in seconds.
/// Takes into account the year.
////////////////////////////////////////////////////////////////////////////////////////////////////
double TS_UTC_subtraction(                 /* RETURN s  Time difference in seconds               */
  TS_TIME_UTC_S_TYPE  utc_a,               /* IN:    -- Time structure holding UTC a             */
  TS_TIME_UTC_S_TYPE  utc_b)               /* IN:    -- Time structure holding UTC b             */
{
  int    year             = 0;
  double seconds_per_year = 0.0;
  double seconds_a        = 0.0;
  double seconds_b        = 0.0;

  TS_convert_time_sgmt2second(&utc_a, &seconds_a);
  TS_convert_time_sgmt2second(&utc_b, &seconds_b);

  for (year=utc_b.year; year<utc_a.year; year++) {
      TS_convert_year2second(year, &seconds_per_year);
      seconds_a += seconds_per_year;
  }

  for (year=utc_a.year; year<utc_b.year; year++) {
      TS_convert_year2second(year, &seconds_per_year);
      seconds_b += seconds_per_year;
  }

  return (seconds_a - seconds_b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param   utc_a   --  Time structure holding UTC a
/// @param   delta_t (s) Time to be added (in seconds)
///
/// @details
/// Adds delta_t (in seconds) to UTC structure "a" and returns the sum as a UTC structure.
/// Takes into account the year.
////////////////////////////////////////////////////////////////////////////////////////////////////
TS_TIME_UTC_S_TYPE TS_UTC_addition(        // RETURN --  Time structure holding UTC_a + delta_t
  TS_TIME_UTC_S_TYPE  utc_a,               // IN:    --  Time structure holding UTC a
  double delta_t)                          // IN:    (s) Time to be added
{
  TS_TIME_UTC_S_TYPE utc_b;

  // Do something really neato here to get utc_b. But, for now...

  utc_b.year   = utc_a.year;
  utc_b.month  = utc_a.month;
  utc_b.day    = utc_a.day;
  utc_b.minute = utc_a.minute;
  utc_b.second = utc_a.second;

  return (utc_b);
}


