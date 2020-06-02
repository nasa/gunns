/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Conversion utilities.)

REFERENCE:
    (TBD)

ASSUMPTIONS AND LIMITATIONS:
    (TBD)

LIBRARY DEPENDENCIES:
   (
   (simulation/hs/TsHsMsgWrapper.o)
   )

PROGRAMMERS:
    (
     ((Paul Carradine) (L3) (Apr 2008) (Initial prototype))
     ((Hadi Tjandrasa) (L3) (Dec 2009) (Add SEC_PER_DAY))
     ((Robert Paul) (L3) (Thu Sep 09, 2010 13:06:42 CDT) (Add dBW conversion))
    )
*******************************************************************************/

#include <math.h>

#include "TS_conversions.h"
#include "simulation/hs/TsHsMsgWrapper.hh"

//----------------------------------------------------------
// TOLERANCE FACTORS
//----------------------------------------------------------
const double TS_10_TO_THE_MINUS_8  = 1E-8;
const double TS_10_TO_THE_MINUS_12 = 1E-12;


//===============================================================================================
// CONVERSION FACTORS
//===============================================================================================

//------------------------------
// TIME
//------------------------------
const double TS_SEC_PER_DAY       = 86400.0;
const double TS_SEC_PER_HR        = 3600.0;
const double TS_MIN_PER_HR        =   60.0;
const double TS_SEC_PER_MIN       =   60.0;

const double TS_DAY_PER_SEC       = 0.000011574;
const double TS_HR_PER_SEC        = 0.00027777778;
const double TS_HR_PER_MIN        = 0.01666666667;
const double TS_HR_PER_DAY        = 24.0;
const double TS_MIN_PER_SEC       = 0.01666666667;

const int    TS_DAYS_PER_YEAR     = 365;
const int    TS_MONTHS_PER_YEAR   = 12;
const int    TS_MAX_YEAR          = 2200;
const int    TS_MIN_YEAR          = 1970;

const int    TS_START_DAY_NORM[] = {0, 0,31,59,90,120,151,181,212,243,273,304,334,365};
const int    TS_START_DAY_LEAP[] = {0, 0,31,60,91,121,152,182,213,244,274,305,335,366};

const double TS_LEAP_YEAR_SEC      = 31622400.0;
const double TS_COMMON_YEAR_SEC    = 31536000.0;

const double TS_SEC_PER_MSEC      = 1.0E3;
const double TS_MSEC_PER_SEC      = 1.0E-3;

//------------------------------
// FREQUENCY
//------------------------------
const double TS_HZ_PER_MHZ        = 1.0E6;
const double TS_MHZ_PER_HZ        = 1.0E-6;

//------------------------------
// Length
//------------------------------
const double TS_IN_PER_FT         = 12.0;
const double TS_FT_PER_IN         = 0.08333333333333;

const double TS_FT_PER_YD         = 3.0;
const double TS_YD_PER_FT         = 0.33333333333333;

const double TS_CM_PER_IN         = 2.54;
const double TS_IN_PER_CM         = 0.3937007874016;

const double TS_M_PER_IN          = 0.0254;
const double TS_IN_PER_M          = 39.37007874;

const double TS_FT_PER_M          = 3.280839895013;
const double TS_M_PER_FT          = 0.3048;

const double TS_FT_PER_MI         = 5280.0;      // Mile
const double TS_MI_PER_FT         = 0.000189394;

const double TS_FT_PER_NMI        = 6076.115;    // International nautical mile
const double TS_NMI_PER_FT        = 0.000164579;

const double TS_M_PER_KM          = 1000.0;
const double TS_KM_PER_M          = 0.001;

//------------------------------
// Area
//------------------------------
const double TS_IN2_PER_FT2       = 144.0;
const double TS_FT2_PER_IN2       = 0.006944444444445;

const double TS_IN2_PER_M2        = 1550.003100006;
const double TS_M2_PER_IN2        = 0.00064516;

const double TS_FT2_PER_M2        = 10.76391041671;
const double TS_M2_PER_FT2        = 0.09290304;

//------------------------------
// Volume
//------------------------------
const double TS_IN3_PER_FT3     = 1728.0 ;
const double TS_FT3_PER_IN3     = 0.000578703704;

const double TS_IN3_PER_M3      = 61023.7441;
const double TS_M3_PER_IN3      = 1.6387064e-5;

const double TS_FT3_PER_M3      = 35.3146667;
const double TS_M3_PER_FT3      = 0.0283168466;

//------------------------------
// Pressure
//------------------------------
const double TS_PA_PER_PSI        = 6894.75;
const double TS_PSI_PER_PA        = 0.0001450378911491;

const double TS_TORR_PER_PSI      = 51.71487786825;
const double TS_PSI_PER_TORR      = 0.01933679515879;

const double TS_ATM_PER_PSI       = 0.0680458919319;
const double TS_PSI_PER_ATM       = 14.69596432068;

const double TS_PA_PER_ATM        = 101325.0;
const double TS_ATM_PER_PA        = 9.869232667e-6;

const double TS_MMHG_PER_PSI      = 51.7;

//------------------------------
// Weight/Mass
//------------------------------
const double TS_KG_PER_LBM        = 0.45359237;
const double TS_G_PER_LBM         = 453.59237;
const double TS_LBM_PER_KG        = 2.204622621849;
const double TS_LBM_PER_G         = 0.002204622621849;

//------------------------------p_correction->in.
// Mass Flow
//------------------------------
const double TS_KG_PER_SEC_PER_LBM_PER_HR = 0.000125998;
const double TS_LBM_PER_HR_PER_KG_PER_SEC = 7936.633915;

const double TS_KG_PER_SEC_PER_LBM_PER_SEC = 0.45359237002;
const double TS_LBM_PER_SEC_PER_KG_PER_SEC = 2.2046226217;

//------------------------------
// Energy
//------------------------------
const double TS_BTU_PER_J       = 0.00094781712;
const double TS_J_PER_BTU       = 1055.05585;
const double TS_KJ_PER_BTU      = 1.05505585;
const double TS_BTU_PER_KJ      = 0.9478171227;


//------------------------------
// Power
//------------------------------
const double TS_BTU_PER_W_S       = 0.0009478169879134;
const double TS_W_S_PER_BTU       = 1055.056;
const double TS_BTU_PER_HR_PER_W  = 3.4144;
const double TS_W_PER_BTU_PER_HR  = 0.2928;
const double TS_BTU_PER_HR_PER_KW = 3414.426;
const double TS_KW_PER_BTU_PER_HR = 0.00029287;


//------------------------------
// Temperature
//------------------------------
const double TS_F_PER_C         = 1.8;
const double TS_C_PER_F         = 0.5555555555556;

//------------------------------
// Viscosity
//------------------------------
const double TS_LBF_OVER_FTHR_PER_CP = 2.4190881537;

//------------------------------
// Percentage
//------------------------------
const double TS_PERCENTAGE = 100.0;

//------------------------------
// Specific Heat
//------------------------------
const double TS_BTU_OVER_LBMF_PER_KJ_OVER_KGK = 0.23884589663;
const double TS_KJ_OVER_KGK_PER_BTU_OVER_LBMF = 4.1868;
const double TS_J_OVER_KGK_PER_BTU_OVER_LBMF  = 4186.8;

//------------------------------
// Angles
//------------------------------
const double TS_DEG_PER_RAD       = 57.2957795130823 ;
const double TS_RAD_PER_DEG       = 0.0174532925199433 ;

const double TS_MIL_PER_RAD       = 1018.591635788 ;
const double TS_RAD_PER_MIL       = 9.81747704247e-4 ;

const double TS_MIL_PER_DEG       = 17.777777777777777;
const double TS_DEG_PER_MIL       = 0.05625;

const double TS_RAD_PER_REV       = 6.28318531;
const double TS_REV_PER_RAD       = 0.159154943;
const double TS_RAD_PER_ARCSEC    = 4.84813681e-6;
const double TS_ARCSEC_PER_RAD    = 206264.806293699;
const double TS_RAD_PER_ARCMIN    = 2.90888209e-4;
const double TS_ARCMIN_PER_RAD    = 3437.746766834;

const double TS_PI                = 3.14159265358979323846;
const double TS_TWO_PI            = 6.28318530717958647692;
const double TS_SQRT_PI           = 1.77245385090551602729;
const double TS_2SQRT_PI          = 3.54490770181103205459;
const double TS_SQRT_2PI          = 2.50662827463100050241;
const double TS_PI_OVER_2         = 1.57079632679489661923;
const double TS_PI_OVER_3         = 1.04719755119659774615;
const double TS_PI_OVER_4         = 0.78539816339744830962;
const double TS_PI_OVER_6         = 0.52359877559829887377;
const double TS_4_PI_OVER_3       = 4.18879020478639098461;


//===============================================================================================
// MATHEMATICAL & PHYSICAL CONSTANTS
//===============================================================================================
const double TS_STEFAN_BOLTZMANN_CONST_SI     = 5.67051e-8;     // W / (M2 K4)
const double TS_STEFAN_BOLTZMANN_CONST_ENG    = 0.1714e-8;      // BTU / (hr ft2 R4)

const double TS_BOLTZMANN_CONST               = 1.3806504e-23;  // J/K
const double TS_BOLTZMANN_CONST_dBW           = -228.599;       // dBW/ (K Hz)

const double TS_STD_GRAVITY_SI                = 9.80665;        // m / s2
const double TS_STD_GRAVITY_ENG               = 32.174;         // ft / s2
const double TS_GC                            = 32.17;          // (ft lbm)/(lbf s2)

const double TS_UNIV_GAS_CONST_ENG            = 1545.349;       // (lbf ft)/(lbmol R)
const double TS_UNIV_GAS_CONST_SI             = 8.314472;       //   J/(mol K)

const double TS_SPEED_LIGHT_SI                = 299792458.0;    //  m/s (exact-NIST)
const double TS_SPEED_LIGHT_SQ_SI     = 89875517873681764.0;    //  m2 / s2

// HP is horsepower
const double TS_HP_PER_FTLBF_OVER_MIN         = 33000;          // ft*lbf/min

const double TS_EARTH_EQUATORIAL_RADIUS       = 6378137.0;      // M
const double TS_EARTH_POLAR_RADIUS            = 6356752.3;      // M
//----------------------------------------------------------
// Square roots
//----------------------------------------------------------
const double TS_SQRT2      = 1.41421356237309504880168872421; //sqrt(2)
const double TS_SQRT1_2    = 0.70710678118654752440084436210; //sqrt(1/2)
const double TS_SQRT3      = 1.73205080756887729352744634151; //sqrt(3)

//----------------------------------------------------------
// EPS
//----------------------------------------------------------
const double TS_ELECTRON_CHARGE = .1592e-18;                      //  Coulomb


//===============================================================================================
// TEMPERATURE CONVERSIONS
//===============================================================================================

//----------------------------------------------------------
// Fahrenheit conversion routines.
//----------------------------------------------------------
double TS_convert_degF_to_degR(                     // RETURN  R   temperature in Rankine
                                 double  temperature) // IN      F   temperature in Fahrenheit
{
    return ( 459.67 + temperature );
}

double TS_convert_degF_to_degC(                     // RETURN  R   temperature in Celsius
                                 double  temperature) // IN      F   temperature in Fahrenheit
{
    return ( (temperature -32.0) * 5.0 / 9.0 );
}

double TS_convert_degF_to_degK(                     // RETURN  R   temperature in Kelvin
                                 double  temperature) // IN      F   temperature in Fahrenheit
{
    return ( (temperature + 459.67) * 5.0 / 9.0 );
}

//----------------------------------------------------------
// Rankine conversion routines.
//----------------------------------------------------------
double TS_convert_degR_to_degF(                     // RETURN  F   temperature in Fahrenheit
                                 double  temperature) // IN      R   temperature in Rankine
{
    return ( temperature - 459.67 );
}

double TS_convert_degR_to_degC(                     // RETURN  C   temperature in Celsius
                                 double  temperature) // IN      R   temperature in Rankine
{
    return ( (temperature - 459.67) * 5.0 / 9.0 );
}

double TS_convert_degR_to_degK(                     // RETURN  K   temperature in Kelvin
                                 double  temperature) // IN      R   temperature in Rankine
{
    return ( temperature * 5.0 / 9.0 );
}

//----------------------------------------------------------
// Celsius conversion routines.
//----------------------------------------------------------
double TS_convert_degC_to_degF(                     // RETURN  F   temperature in Fahrenheit
                                 double  temperature) // IN      C   temperature in Celsius
{
    return ( (temperature * 9.0 / 5.0) + 32.0 );
}

double TS_convert_degC_to_degR(                     // RETURN  R   temperature in Rankine
                                 double  temperature) // IN      C   temperature in Celsius
{
    return ( (temperature + 273.15) * 9.0 / 5.0 );
}

double TS_convert_degC_to_degK(                     // RETURN  K   temperature in Kelvin
                                 double  temperature) // IN      C   temperature in Celsius
{
    return ( temperature + 273.15 );
}

//----------------------------------------------------------
// Kelvin conversion routines.
//----------------------------------------------------------
double TS_convert_degK_to_degC(                     // RETURN  C   temperature in Celsius
                                 double  temperature) // IN      K   temperature in Kelvin
{
    return ( temperature - 273.15 );
}

double TS_convert_degK_to_degF(                     // RETURN  F   temperature in Fahrenheit
                                 double  temperature) // IN      K   temperature in Kelvin
{
    return ( (temperature * 9.0 / 5.0) - 459.67);
}

double TS_convert_degK_to_degR(                     // RETURN  R   temperature in Rankine
                                 double  temperature) // IN      K   temperature in Kelvin
{
    return ( temperature * 9.0 / 5.0 );
}

//===============================================================================================
// POWER RATIO CONVERSIONS
// Warning these functions protect against floating point exceptions.
//===============================================================================================

//----------------------------------------------------------
// Convert power ratio to decibels.
//----------------------------------------------------------
double TS_convert_PR_to_dB(                         // RETURN  R   power ratio in dB
                                 double  pratio)      // IN      F   power ratio
{
    return ( 10.0 * log10(pratio) );
}

//----------------------------------------------------------
// Convert decibels to power ratio.
//----------------------------------------------------------
double TS_convert_dB_to_PR(                         // RETURN  R   power ratio
                                 double  db)          // IN      F   power ratio in dB
{
    return ( pow(10.0, db/10.0 ) );
}

//----------------------------------------------------------
// Watts to dBW conversion routine.
//----------------------------------------------------------
double TS_convert_W_to_dBW(                         // RETURN  dB  Decibel Watts
                             double  power)           // IN    v*amp Power in Watts
{
   double dBW;
   if(power > 0.0)
   {
      dBW = 10.0*log10(power); // power value neglects division by 1 Watt.
   }
   else
   {
      hsSendMsgC(TS_HS_ERROR, "CMN", "log10 of value <= 0.0");
      dBW = 0.0;  // Since dB's are generally summed, a return value of 0.0
                  // effectively contributes nothing to further computations.
   }
   return dBW;
}
