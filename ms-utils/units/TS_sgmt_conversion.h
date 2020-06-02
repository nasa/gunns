#ifndef TS_sgmt_conversion_EXISTS
#define TS_sgmt_conversion_EXISTS
/**
@defgroup  TSM_UTILITIES_UNITS_SGMT     SGMT Conversion Common Functions (C)
@ingroup   TSM_UTILITIES_UNITS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (SGMT enumerated type and data structure definition)
REFERENCES:
   (N/A)
ASSUMPTIONS AND LIMITATIONS:
   (N/A)
LIBRARY DEPENDENCIES:
    ((TS_sgmt_conversion.o))
PROGRAMMERS:
   (((Hadi Tjandrasa)  (L3-Com)  (Sep 2009) (Initial version)))

@{
*/

#include "software/flag/TS_flag.h"
#include "units/TS_conversions.h"

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
/// Size of ' HH:MM:SS' string
///////////////////////////////////////////////////////////////////////////////////////////////////
#define CMN_N_HMS_TIME_SIZE   9


///////////////////////////////////////////////////////////////////////////////////////////////////
/// Enumerated type for SGMT formatting type.
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
  TS_SGMT_NO_COLON_FORMAT = 12 ,         // No colon formatting for SGMT
  TS_SGMT_COLON_FORMAT    = 16           // Colon formatting for SGMT

}  TS_SGMT_FORMAT_E_TYPE;


///////////////////////////////////////////////////////////////////////////////////////////////////
/// UTC time data structure.
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  int     year;          // --  Year YYYY
  int     month;         // --  Month 1 - 12
  int     day;           // --  Day 1 - 31
  int     hour;          // --  Hour 0 - 23
  int     minute;        // --  minute 0 - 59
  double  second;        // s   Seconds 0.0 <= s < 60.0

}  TS_TIME_UTC_S_TYPE;


///////////////////////////////////////////////////////////////////////////////////////////////////
// SGMT function prototypes.
///////////////////////////////////////////////////////////////////////////////////////////////////
TS_FLAG  TS_convert_year2second(int year, double *sec_ptr);
int  TS_convert_month2day(int year, int month);
void TS_convert_time_sgmt2second( TS_TIME_UTC_S_TYPE *UTC_ptr, double *sec_ptr);
void TS_convert_time_sgmt2string( TS_TIME_UTC_S_TYPE *UTC_ptr, char *dhms_ptr);
TS_FLAG  TS_convert_yseconds2sgmt( TS_SGMT_FORMAT_E_TYPE iformat, double seconds,
                                                 int *year_ptr, char *sgmt_ptr);
TS_FLAG  TS_is_leap_year( int year );
void TS_convert_time_string_hms2count( double job_cycle, char *hms_ptr, int *count_ptr);

int TS_UTC_days_in_month(TS_TIME_UTC_S_TYPE  utc);
TS_FLAG TS_UTC_is_valid(TS_TIME_UTC_S_TYPE  utc);
double TS_UTC_subtraction(TS_TIME_UTC_S_TYPE  utc_a, TS_TIME_UTC_S_TYPE  utc_b);
TS_TIME_UTC_S_TYPE TS_UTC_addition(TS_TIME_UTC_S_TYPE  utc_a, double delta_t);

#ifdef __cplusplus
}
#endif

/// @}

#endif   //TS_SGMT_CONVERSION_H
