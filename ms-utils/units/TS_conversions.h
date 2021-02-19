#ifndef TS_conversions_EXISTS
#define TS_conversions_EXISTS
/**
@defgroup  TSM_UTILITIES_UNITS_CONV Constants, Properties, and Conversions (C)
@ingroup   TSM_UTILITIES_UNITS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
    (Declaration and Definition of constants to be available to all modules
     in the CxTF simulation suite.)

REFERENCE:
    (TBD)

ASSUMPTIONS AND LIMITATIONS:
    (TBD)

PROGRAMMERS:
    (
     ((Paul Carradine) (L3) (Apr 2008) (Initial prototype))
     ((Hadi Tjandrasa) (L3) (Dec 2009) (Add SEC_PER_DAY and YEAR_SEC))
     ((Robert Paul) (L3) (Thu Sep 09, 2010 13:06:42 CDT) (Add dBW conversion))
    )

@{
*/

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------
// TOLERANCE FACTORS
//----------------------------------------------------------
extern const double TS_10_TO_THE_MINUS_8;
extern const double TS_10_TO_THE_MINUS_12;

//===============================================================================================
// CONVERSION FACTORS
//===============================================================================================

//------------------------------
// TIME
//------------------------------
extern const double TS_SEC_PER_DAY;
extern const double TS_SEC_PER_HR;
extern const double TS_MIN_PER_HR;
extern const double TS_SEC_PER_MIN;

extern const double TS_DAY_PER_SEC;
extern const double TS_HR_PER_SEC;
extern const double TS_HR_PER_MIN;
extern const double TS_HR_PER_DAY;
extern const double TS_MIN_PER_SEC;

extern const int    TS_DAYS_PER_YEAR;
extern const int    TS_MONTHS_PER_YEAR;
extern const int    TS_MAX_YEAR;
extern const int    TS_MIN_YEAR;

extern const int    TS_START_DAY_NORM[];
extern const int    TS_START_DAY_LEAP[];

extern const double TS_LEAP_YEAR_SEC;
extern const double TS_COMMON_YEAR_SEC;

extern const double TS_SEC_PER_MSEC;
extern const double TS_MSEC_PER_SEC;

//------------------------------
// FREQUENCY
//------------------------------
extern const double TS_HZ_PER_MHZ;
extern const double TS_MHZ_PER_HZ;

//------------------------------
// Length
//------------------------------
extern const double TS_IN_PER_FT;
extern const double TS_FT_PER_IN;

extern const double TS_FT_PER_YD;
extern const double TS_YD_PER_FT;

extern const double TS_CM_PER_IN;
extern const double TS_IN_PER_CM;

extern const double TS_M_PER_IN;
extern const double TS_IN_PER_M;

extern const double TS_FT_PER_M;
extern const double TS_M_PER_FT;

extern const double TS_FT_PER_MI;
extern const double TS_MI_PER_FT;

extern const double TS_FT_PER_NMI;
extern const double TS_NMI_PER_FT;

extern const double TS_M_PER_KM;
extern const double TS_KM_PER_M;

//------------------------------
// Area
//------------------------------
extern const double TS_IN2_PER_FT2;
extern const double TS_FT2_PER_IN2;

extern const double TS_IN2_PER_M2;
extern const double TS_M2_PER_IN2;

extern const double TS_FT2_PER_M2;
extern const double TS_M2_PER_FT2;

//------------------------------
// Volume
//------------------------------
extern const double TS_IN3_PER_FT3;
extern const double TS_FT3_PER_IN3;

extern const double TS_IN3_PER_M3;
extern const double TS_M3_PER_IN3;

extern const double TS_FT3_PER_M3;
extern const double TS_M3_PER_FT3;

//------------------------------
// Pressure
//------------------------------
extern const double TS_PA_PER_PSI;
extern const double TS_PSI_PER_PA;

extern const double TS_TORR_PER_PSI;
extern const double TS_PSI_PER_TORR;

extern const double TS_ATM_PER_PSI;
extern const double TS_PSI_PER_ATM;

extern const double TS_ATM_PER_PA;
extern const double TS_PA_PER_ATM;

extern const double TS_MMHG_PER_PSI;

//------------------------------
// Weight/Mass
//------------------------------
extern const double TS_KG_PER_LBM;
extern const double TS_G_PER_LBM;
extern const double TS_LBM_PER_KG;
extern const double TS_LBM_PER_G;

//------------------------------
// Mass Flow
//------------------------------
extern const double TS_KG_PER_SEC_PER_LBM_PER_HR;
extern const double TS_LBM_PER_HR_PER_KG_PER_SEC;

extern const double TS_KG_PER_SEC_PER_LBM_PER_SEC;
extern const double TS_LBM_PER_SEC_PER_KG_PER_SEC;

//------------------------------
// Energy
//------------------------------
extern const double TS_BTU_PER_J;
extern const double TS_J_PER_BTU;
extern const double TS_KJ_PER_BTU;
extern const double TS_BTU_PER_KJ;

//------------------------------
// Power
//------------------------------
extern const double TS_BTU_PER_W_S;
extern const double TS_W_S_PER_BTU;
extern const double TS_BTU_PER_HR_PER_W;
extern const double TS_W_PER_BTU_PER_HR;
extern const double TS_BTU_PER_HR_PER_KW;
extern const double TS_KW_PER_BTU_PER_HR;

//------------------------------
// Specific Heat
//------------------------------
extern const double TS_BTU_OVER_LBMF_PER_KJ_OVER_KGK;
extern const double TS_KJ_OVER_KGK_PER_BTU_OVER_LBMF;
extern const double TS_J_OVER_KGK_PER_BTU_OVER_LBMF;

//------------------------------
// Temperature
//------------------------------
extern const double TS_F_PER_C;
extern const double TS_C_PER_F;

//------------------------------
// Viscosity
//------------------------------
extern const double TS_LBF_OVER_FTHR_PER_CP;

//------------------------------
// Percentage
//------------------------------
extern const double TS_PERCENTAGE;

//------------------------------
// Angles
//------------------------------
extern const double TS_DEG_PER_RAD;
extern const double TS_RAD_PER_DEG;

extern const double TS_MIL_PER_RAD;
extern const double TS_RAD_PER_MIL;

extern const double TS_MIL_PER_DEG;
extern const double TS_DEG_PER_MIL;

extern const double TS_RAD_PER_REV;
extern const double TS_REV_PER_RAD;
extern const double TS_RAD_PER_ARCSEC;
extern const double TS_ARCSEC_PER_RAD;
extern const double TS_RAD_PER_ARCMIN;
extern const double TS_ARCMIN_PER_RAD;

extern const double TS_PI;
extern const double TS_TWO_PI;
extern const double TS_SQRT_PI;
extern const double TS_2SQRT_PI;
extern const double TS_SQRT_2PI;
extern const double TS_PI_OVER_2;
extern const double TS_PI_OVER_3;
extern const double TS_PI_OVER_4;
extern const double TS_PI_OVER_6;
extern const double TS_4_PI_OVER_3;


//===============================================================================================
// MATHEMATICAL & PHYSICAL CONSTANTS
//===============================================================================================

extern const double TS_STEFAN_BOLTZMANN_CONST_SI;     //   W / (M2 K4)
extern const double TS_STEFAN_BOLTZMANN_CONST_ENG;    //   BTU / (hr ft2 R4)

extern const double TS_BOLTZMANN_CONST;               // J/K
extern const double TS_BOLTZMANN_CONST_dBW;           // dBW/ (K Hz)

extern const double TS_STD_GRAVITY_SI;                //   m / s2
extern const double TS_STD_GRAVITY_ENG;               //   ft / s2

extern const double TS_GC;                            //   (ft lbm)/(lbf s2)

extern const double TS_SPEED_LIGHT_SI;                //   m / s (exact-NIST)
extern const double TS_SPEED_LIGHT_SQ_SI;             //   m2 / s2

extern const double TS_UNIV_GAS_CONST_ENG;            //   (lbf ft)/(lbmol R)
extern const double TS_UNIV_GAS_CONST_SI;             //   J/(mol K)

extern const double TS_HP_PER_FTLBF_OVER_MIN;          // ft*lbf/min

extern const double TS_EARTH_EQUATORIAL_RADIUS;        // M
extern const double TS_EARTH_POLAR_RADIUS;             // M
//----------------------------------------------------------
// Square roots
//----------------------------------------------------------
extern const double TS_SQRT2;
extern const double TS_SQRT1_2;
extern const double TS_SQRT3;


//----------------------------------------------------------
// EPS
//----------------------------------------------------------
extern const double TS_ELECTRON_CHARGE;                      //  Coulomb


//===============================================================================================
// TEMPERATURE CONVERSIONS
//===============================================================================================

//----------------------------------------------------------
// Fahrenheit conversion routines.
//----------------------------------------------------------
double TS_convert_degF_to_degR(                // RETURN  R   temperature in Rankine
                            double  temperature  // IN      F   temperature in Fahrenheit
);

double TS_convert_degF_to_degC(                // RETURN  C   temperature in Celsius
                            double  temperature  // IN      F   temperature in Fahrenheit
);

double TS_convert_degF_to_degK(                // RETURN  K   temperature in Kelvin
                            double  temperature  // IN      F   temperature in Fahrenheit
);

//----------------------------------------------------------
// Rankine conversion routines.
//----------------------------------------------------------
double TS_convert_degR_to_degF(                // RETURN  F   temperature in Fahrenheit
                            double  temperature  // IN      R   temperature in Rankine
);

double TS_convert_degR_to_degC(                // RETURN  C   temperature in Celsius
                            double  temperature  // IN      R   temperature in Rankine
);

double TS_convert_degR_to_degK(                // RETURN  K   temperature in Kelvin
                            double  temperature  // IN      R   temperature in Rankine
);

//----------------------------------------------------------
// Celsius conversion routines.
//----------------------------------------------------------
double TS_convert_degC_to_degF(                // RETURN  F   temperature in Fahrenheit
                            double  temperature  // IN      C   temperature in Celsius
);

double TS_convert_degC_to_degR(                // RETURN  R   temperature in Rankin
                            double  temperature  // IN      C   temperature in Celsius
);

double TS_convert_degC_to_degK(                // RETURN  K   temperature in Kelvin
                            double  temperature  // IN      C   temperature in Celsius
);

//----------------------------------------------------------
// Kelvin conversion routines.
//----------------------------------------------------------
double TS_convert_degK_to_degC(                     // RETURN  C   temperature in Celsius
                                 double  temperature  // IN      K   temperature in Kelvin
);

double TS_convert_degK_to_degF(                     // RETURN  F   temperature in Fahrenheit
                                 double  temperature  // IN      K   temperature in Kelvin
);
double TS_convert_degK_to_degR(                     // RETURN  R   temperature in Rankine
                                 double  temperature  // IN      K   temperature in Kelvin
);

//===============================================================================================
// POWER RATIO CONVERSIONS
// Warning these functions protect against floating point exceptions.
//===============================================================================================

//----------------------------------------------------------
// Convert power ratio to decibels.
//----------------------------------------------------------
double TS_convert_PR_to_dB(                         // RETURN  R   power ratio in dB
                                 double  pratio);     // IN      F   power ratio

//----------------------------------------------------------
// Convert decibels to power ratio.
//----------------------------------------------------------
double TS_convert_dB_to_PR(                         // RETURN  R   power ratio
                                 double  db);         // IN      F   power ratio in dB
//----------------------------------------------------------
// Watts to dBW conversion routine.
//----------------------------------------------------------
double TS_convert_W_to_dBW(                         // RETURN  dB  Decibel Watts
                                 double  power        // IN    v*amp Power in Watts
);

#ifdef __cplusplus
}
#endif

///@} // end of ingroup

#endif
