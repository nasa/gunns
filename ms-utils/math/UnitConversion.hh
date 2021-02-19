#ifndef UnitConversion_EXISTS
#define UnitConversion_EXISTS

/**
 @file
 @brief    Unit Conversion Constants and Functions declarations.

@defgroup  TSM_UTILITIES_MATH_UNIT_CONVERSION   Unit Conversion
@ingroup   TSM_UTILITIES_MATH

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE: (Provides a class with units conversion, related constants and conversion methods.)

@details
REQUIREMENTS:
- ()

REFERENCE:
- ((Conversions from http://physics.nist.gov/Pubs/SP811/appenB8.html)
   (Constants from http://physics.nist.gov/cuu/Constants/index.html))


ASSUMPTIONS AND LIMITATIONS:
- ()
 LIBRARY DEPENDENCY:
- ((math/UnitConversion.o))

 PROGRAMMERS:
- (((Kenneth McMurtrie) (Tietronix Software) (Add kilo<->unit conversion) (2011-10))
   ((Chris Ruhl) (USA) (April 13, 2012) (TS21) (BYTE Constants)))

@{
*/

#include <cmath>
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Unit Conversion Constants and Functions.
///
/// @details  Provides a class with units conversion, related constants and conversion methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UnitConversion
{
    public:
        /// @brief  Enumeration of the supported conversion methods.
        ///
        /// @note These enums correspond to the conversion functions, i.e. F_TO_R corresponds to the
        ///       convertDegFToDegR function.  These enums are used as indices to their corresponding
        ///       function in the convertFunctions array, and changes to this enum MUST be matched by
        ///       corresponding changes to the array initialization in the code body.
        ///
        /// @note For readability, group similar parameters (temperature, pressure) together.
        ///
        /// @note Do not use [unit out]_PER_[unit in] names here, to avoid name collisions with the
        ///       conversion constants below.  Instead, [unit in]_TO_[unit out] names are preferred.
        enum Type {
            NO_CONVERSION,                   ///< Do not convert.  THIS ONE MUST BE FIRST IN THE ENUM.
            UNIT_TO_MICRO,                   ///< Convert generic units to generic micro-units.
            UNIT_TO_MILLI,                   ///< Convert generic units to generic milli-units.
            UNIT_TO_KILO,                    ///< Convert generic units to generic kilo-units.
            UNIT_TO_MEGA,                    ///< Convert generic units to generic mega-units.
            FRAC_TO_PERCENT,                 ///< Convert fraction to percentage.
            UNIT_PER_SEC_TO_UNIT_PER_HOUR,   ///< Convert unit/s to unit/hr.
            F_TO_R,                          ///< Convert temperature from Fahrenheit to Rankine.
            F_TO_C,                          ///< Convert temperature from Fahrenheit to Celsius.
            F_TO_K,                          ///< Convert temperature from Fahrenheit to Kelvin.
            R_TO_F,                          ///< Convert temperature from Rankine to Fahrenheit.
            R_TO_C,                          ///< Convert temperature from Rankine to Celsius.
            R_TO_K,                          ///< Convert temperature from Rankine to Kelvin.
            C_TO_F,                          ///< Convert temperature from Celsius to Fahrenheit.
            C_TO_R,                          ///< Convert temperature from Celsius to Rankine.
            C_TO_K,                          ///< Convert temperature from Celsius to Kelvin.
            K_TO_C,                          ///< Convert temperature from Kelvin to Celsius.
            K_TO_F,                          ///< Convert temperature from Kelvin to Fahrenheit.
            K_TO_R,                          ///< Convert temperature from Kelvin to Rankine.
            KPA_TO_PSI,                      ///< Convert pressure from kilopascals to pounds per square inch.
            KPA_TO_MMHG,                     ///< Convert pressure from kilopascals to millimeters of mercury.
            KPA_TO_MILLITORR,                ///< Convert pressure from kilopascals to millitorr.
            KPA_TO_INH2O,                    ///< Convert pressure from kilopascals to inches of water.
            PSI_TO_KPA,                      ///< Convert pressure from pounds per square inch to kilopascals.
            KG_PER_SEC_TO_LBM_PER_HOUR,      ///< Convert flow from kilograms per second to pounds mass per hour.
            KG_PER_SEC_TO_SCFM0C,            ///< Convert flow from kilograms per second to standard cubic feet per minute @ 0 C.
            LBM_PER_HOUR_TO_KG_PER_SEC,      ///< Convert flow from pounds mass per hour to kilograms per second.
            PR_TO_DB,                        ///< Convert power from power ratio to decibels.
            DB_TO_PR,                        ///< Convert power from decibels to power ratio.
            W_TO_DBW,                        ///< Convert power from watts to decibel watts.
            RAD_TO_DEG,                      ///< Convert angle from radians to degrees.
            DEG_TO_RAD,                      ///< Convert angle from degrees to radians.
            RADPERSEC_TO_RPM,                ///< Convert angular rate from rad/s to revolutions per minute.
            RPM_TO_RADPERSEC,                ///< Convert angular rate from revolutions per minute to rad/s.
            NUM_CONVERSIONS                  ///< THIS ONE MUST BE LAST IN THE ENUM.
        };

        // NOTE: Do not name the conversion constants <unit in>_TO_<unit out>, to avoid name
        //       collisions with the enumerations above.  Instead, <unit out>_PER_<unit in> is
        //       preferred.
        static const double TEN_TO_THE_MINUS_8;             /**< (--)  Tolerance. */
        static const double TEN_TO_THE_MINUS_12;            /**< (--)  Tolerance. */
        static const double MILLI_PER_UNIT;                 /**< (--)  Unit to milli-unit conversion. */
        static const double UNIT_PER_MILLI;                 /**< (--)  Milli-unit to unit conversion. */
        static const double KILO_PER_UNIT;                  /**< (--)  Unit to kilo-unit conversion. */
        static const double UNIT_PER_KILO;                  /**< (--)  Kilo-Unit to unit conversion. */
        static const double MEGA_PER_UNIT;                  /**< (--)  Unit to mega-unit conversion. */
        static const double UNIT_PER_MEGA;                  /**< (--)  Mega-unit to unit conversion. */
        static const double SEC_PER_YEAR;                   /**< (--)  s/year Time conversion. */
        static const double SEC_PER_DAY;                    /**< (--)  s/day Time conversion. */
        static const double SEC_PER_HR;                     /**< (--)  s/hr Time conversion. */
        static const double SEC_PER_MIN;                    /**< (--)  s/min Time conversion. */
        static const double MIN_PER_HR;                     /**< (--)  min/hr Time conversion. */
        static const double HR_PER_SEC;                     /**< (--)  hr/s Time conversion. */
        static const double HR_PER_MIN;                     /**< (--)  hr/min Time conversion. */
        static const double HR_PER_DAY;                     /**< (--)  hr/day Time conversion. */
        static const double MIN_PER_SEC;                    /**< (--)  min/s Time conversion. */
        static const int    DAYS_PER_YEAR;                  /**< (--)  day/yr Time conversion. */
        static const int    DAYS_PER_WEEK;                  /**< (--)  day/wk Time conversion. */
        static const int    MONTHS_PER_YEAR;                /**< (--)  mo/yr Time conversion. */
        static const int    MAX_YEAR;                       /**< (--)  yr Time limit. */
        static const int    MIN_YEAR;                       /**< (--)  yr Time limit. */
        static const int    START_DAY_NORM[14];             /**< (--)  day Time conversion constant. */
        static const int    START_DAY_LEAP[14];             /**< (--)  day Time conversion constant. */
        static const double LEAP_YEAR_SEC;                  /**< (--)  s/yr Time conversion. */
        static const double COMMON_YEAR_SEC;                /**< (--)  s/yr Time conversion. */
        static const double SEC_PER_MSEC;                   /**< (--)  s/ms Time conversion. */
        static const double MSEC_PER_SEC;                   /**< (--)  ms/s Time conversion. */
        static const double HZ_PER_MHZ;                     /**< (--)  Hz/MHz Frequency conversion. */
        static const double MHZ_PER_HZ;                     /**< (--)  MHz/Hz Frequency conversion. */
        static const double IN_PER_FT_UTIL;                 /**< (--)  in/ft Length conversion. */
        static const double FT_PER_IN;                      /**< (--)  ft/in Length conversion. */
        static const double FT_PER_YD;                      /**< (--)  ft/yd Length conversion. */
        static const double YD_PER_FT;                      /**< (--)  yd/ft Length conversion. */
        static const double CM_PER_IN;                      /**< (--)  cm/in Length conversion. */
        static const double IN_PER_CM;                      /**< (--)  in/cm Length conversion. */
        static const double M_PER_IN;                       /**< (--)  m/in Length conversion. */
        static const double IN_PER_M;                       /**< (--)  in/m Length conversion. */
        static const double FT_PER_M;                       /**< (--)  ft/m Length conversion. */
        static const double M_PER_FT;                       /**< (--)  m/ft Length conversion. */
        static const double M_PER_KM;                       /**< (--)  m/km Length conversion. */
        static const double KM_PER_M;                       /**< (--)  km/m Length conversion. */
        static const double IN2_PER_FT2_UTIL;               /**< (--)  in2/ft2 Area conversion. */
        static const double FT2_PER_IN2;                    /**< (--)  ft2/in2 Area conversion. */
        static const double IN2_PER_M2;                     /**< (--)  in2/m2 Area conversion. */
        static const double M2_PER_IN2_UTIL;                /**< (--)  m2/in2 Area conversion. */
        static const double M2_PER_FT2;                     /**< (--)  m2/ft2 Area conversion. */
        static const double FT2_PER_M2;                     /**< (--)  ft2/m2 Area conversion. */
        static const double IN3_PER_FT3;                    /**< (--)  in3/ft3 Volume conversion. */
        static const double FT3_PER_IN3;                    /**< (--)  ft3/in3 Volume conversion. */
        static const double IN3_PER_M3;                     /**< (--)  in3/m3 Volume conversion. */
        static const double M3_PER_IN3;                     /**< (--)  m3/in3 Volume conversion. */
        static const double FT3_PER_M3;                     /**< (--)  ft3/m3 Volume conversion. */
        static const double M3_PER_FT3;                     /**< (--)  m3/ft3 Volume conversion. */
        static const double M3_PER_ML;                      /**< (--)  m3/ml Volume conversion. */
        static const double ML_PER_M3;                      /**< (--)  ml/m3 Volume conversion. */
        static const double PA_PER_PSI;                     /**< (--)  Pa/psi Pressure conversion. */
        static const double KPA_PER_PSI;                    /**< (--)  kPa/psi Pressure conversion. */
        static const double PSI_PER_PA;                     /**< (--)  psi/Pa Pressure conversion. */
        static const double PSI_PER_KPA;                    /**< (--)  psi/kPa Pressure conversion. */
        static const double TORR_PER_PSI;                   /**< (--)  torr/psi Pressure conversion. */
        static const double TORR_PER_KPA;                   /**< (--)  torr/kPa Pressure conversion. */
        static const double PSI_PER_TORR;                   /**< (--)  psi/torr Pressure conversion. */
        static const double ATM_PER_PSI;                    /**< (--)  atm/psi Pressure conversion. */
        static const double PSI_PER_ATM;                    /**< (--)  psi/atm Pressure conversion. */
        static const double ATM_PER_PA;                     /**< (--)  atm/pa Pressure conversion. */
        static const double PA_PER_ATM;                     /**< (--)  pa/atm Pressure conversion. */
        static const double MMHG_PER_PSI;                   /**< (--)  mmHg/psi Pressure conversion. */
        static const double KPA_PER_MMHG;                   /**< (--)  kPa/mmHg Pressure conversion. */
        static const double PA_PER_KPA;                     /**< (--)  pa/kpa Pressure conversion. */
        static const double KPA_PER_MMH2O;                  /**< (--)  kpa/mmH2O Pressure conversion. */
        static const double KPA_PER_KGF_PER_CM2;            /**< (--)  kpa/kgf/cm2 Pressure conversion. */
        static const double KPA_PER_PA;                     /**< (--)  kpa/pa Pressure conversion. */
        static const double KG_PER_LBM;                     /**< (--)  kg/lbm Weight/mass conversion. */
        static const double G_PER_LBM;                      /**< (--)  g/lbm Weight/mass conversion. */
        static const double LBM_PER_KG;                     /**< (--)  lbm/kg Weight/mass conversion. */
        static const double LBM_PER_G;                      /**< (--)  lbm/g Weight/mass conversion. */
        static const double KG_PER_SEC_PER_LBM_PER_HR;      /**< (--)  (kg/s)/(lbm/hr) Mass flow. */
        static const double LBM_PER_HR_PER_KG_PER_SEC;      /**< (--)  (lbm/hr)/(kg/s) Mass flow. */
        static const double KG_PER_SEC_PER_LBM_PER_SEC;     /**< (--)  (kg/s)/(lbm/s) Mass flow. */
        static const double LBM_PER_SEC_PER_KG_PER_SEC;     /**< (--)  (lbm/s)/(kg/s) Mass flow. */
        static const double SCFM0C_PER_KG_PER_SEC;          /**< (--)  (scfm @ 0 C)/(kg/s) Volumetric flow. */
        static const double N_PER_LBF;                      /**< (--)  N/lbf Force conversion. */
        static const double LBF_PER_N;                      /**< (--)  lbf/N Force conversion. */
        static const double N_M_PER_IN_LBF;                 /**< (--)  N*m/in/lbf Torque conversion. */
        static const double IN_LBF_PER_N_M;                 /**< (--)  in*lbf/N/m Torque conversion. */
        static const double BTU_PER_J;                      /**< (--)  (BTU/J) Energy conversion. */
        static const double J_PER_BTU;                      /**< (--)  (J/BTU) Energy conversion. */
        static const double KJ_PER_BTU;                     /**< (--)  (kJ/BTU) Energy conversion. */
        static const double BTU_PER_KJ;                     /**< (--)  (BTU/kJ) Energy conversion. */
        static const double BTU_PER_W_S;                    /**< (--)  (BTU/W/s) Power conversion. */
        static const double W_S_PER_BTU;                    /**< (--)  (W/s/BTU) Power conversion. */
        static const double W_PER_BTU_PER_HR;               /**< (--)  (W/BTU/hr) Power conversion. */
        static const double BTU_PER_HR_PER_W;               /**< (--)  (BTU/hr/W) Power conversion. */
        static const double BTU_PER_HR_PER_KW;              /**< (--)  (BTU/hr/kW) Power conversion. */
        static const double KW_PER_BTU_PER_HR;              /**< (--)  (kW/BTU/hr) Power conversion. */
        static const double BTU_OVER_LBMF_PER_KJ_OVER_KGK;  /**< (--)  (BTU/lbm)/(kJ/kg) Specific heat. */
        static const double KJ_OVER_KGK_PER_BTU_OVER_LBMF;  /**< (--)  (kJ/kg)/(BTU/lbm) Specific heat. */
        static const double J_OVER_KGK_PER_BTU_OVER_LBMF;   /**< (--)  (J/kg)/(BTU/lbm) Specific heat. */
        static const double F_PER_C;                        /**< (--)  F/C Temperature conversion. */
        static const double C_PER_F;                        /**< (--)  C/F Temperature conversion. */
        static const double ZERO_C_IN_K;                    /**< (--)  K Temperature conversion. */
        static const double ZERO_F_IN_R;                    /**< (--)  R Temperature conversion. */
        static const double ZERO_C_IN_F;                    /**< (--)  F Temperature conversion. */
        static const double MILLIRAD_TO_MICROGRAY;          /**< (--)  (ugray/mrad) absorbed radiation dose conversion. */
        static const double MILLIREM_TO_MICROSIEVERT;       /**< (--)  (usievert/mrem) equivalent radiation dose conversion. */
        static const double LBF_OVER_FTHR_PER_CP;           /**< (--)  (lbf/ft/hr)/(cp) Viscosity. */
        static const double PERCENTAGE;                     /**< (--)  Percentage conversion. */
        static const double DEG_PER_RAD_UTIL;               /**< (--)  d/r Angle conversion. */
        static const double RAD_PER_DEG;                    /**< (--)  r/d Angle conversion. */
        static const double MIL_PER_RAD;                    /**< (--)  mil/r Angle conversion. */
        static const double RAD_PER_MIL;                    /**< (--)  r/mil Angle conversion. */
        static const double SEC_PER_MIN_PER_2PI;            /**< (--)  S/min/rad Angle conversion. */
        static const double MIL_PER_DEG;                    /**< (--)  mil/d Angle conversion. */
        static const double DEG_PER_MIL;                    /**< (--)  d/mil Angle conversion. */
        static const double RAD_PER_REV;                    /**< (--)  r/rev Angle conversion. */
        static const double DEG_PER_REV;                    /**< (--)  d/rev Angle conversion. */
        static const double REV_PER_RAD;                    /**< (--)  rev/r Angle conversion. */
        static const double REV_PER_DEG;                    /**< (--)  rev/d Angle conversion. */
        static const double RAD_PER_ARCSEC;                 /**< (--)  r/arcs Angle conversion. */
        static const double RAD_PER_ARCMIN;                 /**< (--)  r/arcm Angle conversion. */
        static const double PI_UTIL;                        /**< (--)  r Angle constant. */
        static const double TWO_PI;                         /**< (--)  r Angle constant. */
        static const double SQRT_PI;                        /**< (--)  r Angle constant. */
        static const double TWO_SQRT_PI;                    /**< (--)  r Angle constant. */
        static const double SQRT_2PI;                       /**< (--)  r Angle constant. */
        static const double PI_OVER_2;                      /**< (--)  r Angle constant. */
        static const double PI_OVER_3;                      /**< (--)  r Angle constant. */
        static const double PI_OVER_4;                      /**< (--)  r Angle constant. */
        static const double PI_OVER_6;                      /**< (--)  r Angle constant. */
        static const double FOUR_PI_OVER_3;                 /**< (--)  r Angle constant. */
        static const double STD_DENSITY_WATER;              /**< (--)  kg/m3 standard density of water. */
        static const double STD_SPECIFIC_VOLUME_WATER;      /**< (--)  m3/kg standard specific volume of water. */
        static const double STD_DENSITY_AIR;                /**< (--)  kg/m3 standard density of dry air. */
        static const double STD_SPECIFIC_VOLUME_AIR;        /**< (--)  m3/kg standard specific volume of dry air. */
        static const double STEFAN_BOLTZMANN_CONST_SI;      /**< (--)  W/m2/K4 Physical Constant. */
        static const double STEFAN_BOLTZMANN_CONST_ENG;     /**< (--)  BTU/hr/ft2/R4 Physical Constant. */
        static const double BOLTZMANN_CONST;                /**< (--)  J/K Physical Constant. */
        static const double BOLTZMANN_CONST_DBW;            /**< (--)  DBW/K/Hz Physical Constant. */
        static const double STD_GRAVITY_SI;                 /**< (--)  m/s2 Physical Constant. */
        static const double STD_GRAVITY_ENG;                /**< (--)  ft/s2 Physical Constant. */
        static const double GC;                             /**< (--)  ft*lbm/lbf/s2 Physical Constant. */
        static const double SPEED_LIGHT_SI;                 /**< (--)  m/s Physical Constant. */
        static const double SPEED_LIGHT_SQ_SI;              /**< (--)  m2/s2 Physical Constant. */
        static const double UNIV_GAS_CONST_ENG;             /**< (--)  lbf*ft/lbmol/R Physical Constant. */
        static const double UNIV_GAS_CONST_SI;              /**< (--)  J/mol/K Physical Constant. */
        static const double HP_PER_FTLBF_OVER_MIN;          /**< (--)  ft*lbf/min Physical Constant. */
        static const double EARTH_EQUATORIAL_RADIUS;        /**< (--)  m Physical Constant. */
        static const double EARTH_POLAR_RADIUS;             /**< (--)  m Physical Constant. */
        static const double ELECTRON_CHARGE;                /**< (--)  Coulomb  Physical Constant. */
        static const double FARADAY;                        /**< (--)  Coulomb  / moule Constant. */
        static const double SQRT2;                          /**< (--)  Numeric Constant. */
        static const double SQRT1_2;                        /**< (--)  Numeric Constant. */
        static const double SQRT3;                          /**< (--)  Numeric Constant. */
        static const double SQRT3_2;                        /**< (--)  Numeric Constant. */
        static const int    ONE_BYTE = 256;                 /**< (--)  eight bits. */
        static const int    TWO_BYTES = 65536;              /**< (--)  Sixteen bits. */
        /// @brief Applies the conversion type to the input.
        static double convert(const Type type, const double input);
        /// @brief Converts nothing
        static double convertNothing(const double input);
        /// @brief Converts units to micro-units.
        static double convertUnitToMicro(const double unit);
        /// @brief Converts units to milli-units.
        static double convertUnitToMilli(const double unit);
        /// @brief Converts units to kilo-units.
        static double convertUnitToKilo(const double unit);
        /// @brief Converts units to mega-units.
        static double convertUnitToMega(const double unit);
        /// @brief Converts fraction to percent.
        static double convertFracToPercent(const double fraction);
        /// @brief Converts seconds to hours (or unit/s to unit/hr).
        static double convertUnitPerSecToUnitPerHour(const double unitPerSecond);
        /// @brief Converts temperature from Fahrenheit to Rankine
        static double convertDegFToDegR(const double fahrenheit);
        /// @brief Converts temperature from Fahrenheit to Celsius
        static double convertDegFToDegC(const double fahrenheit);
        /// @brief Converts temperature from Fahrenheit to Kelvin
        static double convertDegFToDegK(const double fahrenheit);
        /// @brief Converts temperature from Rankine to Fahrenheit
        static double convertDegRToDegF(const double rankine);
        /// @brief Converts temperature from Rankine to Celsius
        static double convertDegRToDegC(const double rankine);
        /// @brief Converts temperature from Rankine to Kelvin
        static double convertDegRToDegK(const double rankine);
        /// @brief Converts temperature from Celsius to Fahrenheit
        static double convertDegCToDegF(const double celsius);
        /// @brief Converts temperature from Celsius to Rankine
        static double convertDegCToDegR(const double celsius);
        /// @brief Converts temperature from Celsius to Kelvin
        static double convertDegCToDegK(const double celsius);
        /// @brief Converts temperature from Kelvin to Celsius
        static double convertDegKToDegC(const double kelvin);
        /// @brief Converts temperature from Kelvin to Fahrenheit
        static double convertDegKToDegF(const double kelvin);
        /// @brief Converts temperature from Kelvin to Rankine
        static double convertDegKToDegR(const double kelvin);
        /// @brief Converts pressure from kPa to psi.
        static double convertKpaToPsi(const double kPa);
        /// @brief Converts pressure from kPa to mmHg.
        static double convertKpaToMmhg(const double kPa);
        /// @brief Converts pressure from kPa to milli-torr.
        static double convertKpaToMilliTorr(const double kPa);
        /// @brief Converts pressure from kPa to inH2O.
        static double convertKpaToInh2o(const double kPa);
        /// @brief Converts pressure from psi to kpa.
        static double convertPsiToKpa(const double psi);
        /// @brief Converts flow from kilograms per second to pounds mass per hour.
        static double convertKgPerSecToLbmPerHour(const double kgPerSec);
        /// @brief Converts flow from kilograms per second to standard cubic feet per minute @ 0 C.
        static double convertKgPerSecToScfm0C(const double kgPerSec);
        /// @brief Converts flow from pounds mass per hour to kilograms per second.
        static double convertLbmPerHourToKgPerSec(const double lbmPerHour);
        /// @brief Converts power ratio to decibels.
        static double convertPrToDb(const double pratio);
        /// @brief Converts decibels to power ratio.
        static double convertDbToPr(const double db);
        /// @brief Converts watts to DBW.
        static double convertWToDbw(const double power);
        /// @brief Converts radians to degrees.
        static double convertRadToDeg(const double rad);
        /// @brief Converts degrees to radians.
        static double convertDegToRad(const double deg);
        /// @brief Converts rad/s to rpm.
        static double convertRadPerSecToRpm(const double radPerSec);
        /// @brief Converts rpm to rad/s.
        static double convertRpmtoRadPerSec(const double rpm);
        /// @brief Converts a value to a percentage of the basis value.
        static double convertToPercent(const double x, const double basisInv);
        /// @brief Converts a percentage of the basis value to a value.
        static double convertFromPercent(const double x, const double basis);
    protected:
        /// @brief A function pointer type of signature: double function(double), used to store
        ///        pointers to the convert functions.
        typedef double (*convertPtr)(double);
        static const convertPtr convertFunctions[NUM_CONVERSIONS]; /**< ** (--) Convert function pointers array */

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Default constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UnitConversion();
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UnitConversion(const UnitConversion&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Default destructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        ~UnitConversion();
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UnitConversion& operator =(const UnitConversion&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  input  (--)  Input argument which is simply returned.
///
/// @return   double  (--)  The input argument without modification.
///
/// @details  Does no conversion, and simply returns the input argument back to the caller.  This is
///           provided as an option for users to default to.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertNothing(const double input)
{
    return input;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  unit  (--)  Input in units.
///
/// @return   double  (--)  Units converted to micro-units.
///
/// @details  Converts units to micro-units by multiplying by 1E6.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertUnitToMicro(const double unit)
{
    return unit * UnitConversion::UNIT_PER_MEGA;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  unit  (--)  Input in units.
///
/// @return   double  (--)  Units converted to milli-units.
///
/// @details  Converts units to milli-units by multiplying by 1000.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertUnitToMilli(const double unit)
{
    return unit * UnitConversion::MILLI_PER_UNIT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  unit  (--)  Input in units.
///
/// @return   double  (--)  Units converted to milli-units.
///
/// @details  Converts units to kilo-units by multiplying by 0.001.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertUnitToKilo(const double unit)
{
    return unit * UnitConversion::KILO_PER_UNIT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  unit  (--)  Input in units.
///
/// @return   double  (--)  Units converted to mega-units.
///
/// @details  Converts units to mega-units by multiplying by 1E-6.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertUnitToMega(const double unit)
{
    return unit * UnitConversion::MEGA_PER_UNIT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fraction  (--)  Input fraction.
///
/// @return   double  (--)  fraction converted to percent.
///
/// @details  Converts fraction to percent by multiplying by 100.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertFracToPercent(const double fraction)
{
    return fraction * UnitConversion::PERCENTAGE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  unitPerSecond  (--)  Input unit/s.
///
/// @return   double     (--)  unit/hr.
///
/// @details  Converts unit/s to unit/hr.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertUnitPerSecToUnitPerHour(const double unitPerSecond)
{
    return unitPerSecond * UnitConversion::SEC_PER_HR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fahrenheit  (F)  Input temperature in Fahrenheit
///
/// @return   double  (R)  Temperature converted to Rankine
///
/// @details  Converts a temperature from Fahrenheit to Rankine.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegFToDegR(const double fahrenheit)
{
    return UnitConversion::ZERO_F_IN_R + fahrenheit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fahrenheit  (F)  Input temperature in Fahrenheit
///
/// @return   double  (C)  Temperature converted to Celsius
///
/// @details  Converts a temperature from Fahrenheit to Celsius.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegFToDegC(const double fahrenheit)
{
    return (fahrenheit - UnitConversion::ZERO_C_IN_F) * UnitConversion::C_PER_F;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  fahrenheit  (F)  Input temperature in Fahrenheit
///
/// @return   double  (K)  Temperature converted to Kelvin
///
/// @details  Converts a temperature from Fahrenheit to Kelvin.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegFToDegK(const double fahrenheit)
{
    return (fahrenheit + UnitConversion::ZERO_F_IN_R) * UnitConversion::C_PER_F;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  rankine  (R)  Input temperature in Rankine
///
/// @return   double  (F)  Temperature converted to Fahrenheit
///
/// @details  Converts a temperature from Rankine to Fahrenheit.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegRToDegF(const double rankine)
{
    return rankine - UnitConversion::ZERO_F_IN_R;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  rankine  (R)  Input temperature in Rankine
///
/// @return   double  (C)  Temperature converted to Celsius
///
/// @details  Converts a temperature from Rankine to Celsius.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegRToDegC(const double rankine)
{
    return rankine * UnitConversion::C_PER_F - UnitConversion::ZERO_C_IN_K;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  rankine  (R)  Input temperature in Rankine
///
/// @return   double  (K)  Temperature converted to Kelvin
///
/// @details  Converts a temperature from Rankine to Kelvin.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegRToDegK(const double rankine)
{
    return rankine * UnitConversion::C_PER_F;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  celsius  (C)  Input temperature in Celsius
///
/// @return   double  (F)  Temperature converted to Fahrenheit
///
/// @details  Converts a temperature from Celsius to Fahrenheit.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegCToDegF(const double celsius)
{
    return celsius * UnitConversion::F_PER_C + UnitConversion::ZERO_C_IN_F;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  celsius  (C)  Input temperature in Celsius
///
/// @return   double  (R)  Temperature converted to Rankine
///
/// @details  Converts a temperature from Celsius to Rankine.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegCToDegR(const double celsius)
{
    return (celsius + UnitConversion::ZERO_C_IN_K) * UnitConversion::F_PER_C;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  celsius  (C)  Input temperature in Celsius
///
/// @return   double  (K)  Temperature converted to Kelvin
///
/// @details  Converts a temperature from Celsius to Kelvin.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegCToDegK(const double celsius)
{
    return celsius + UnitConversion::ZERO_C_IN_K;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kelvin  (K)  Input temperature in Kelvin
///
/// @return   double  (C)  Temperature converted to Celsius
///
/// @details  Converts a temperature from Kelvin to Celsius.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegKToDegC(const double kelvin)
{
    return kelvin - UnitConversion::ZERO_C_IN_K;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kelvin  (K)  Input temperature in Kelvin
///
/// @return   double  (F)  Temperature converted to Fahrenheit
///
/// @details  Converts a temperature from Kelvin to Fahrenheit.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegKToDegF(const double kelvin)
{
    return kelvin * UnitConversion::F_PER_C - UnitConversion::ZERO_F_IN_R;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kelvin  (K)  Input temperature in Kelvin
///
/// @return   double  (R)  Temperature converted to Rankine
///
/// @details  Converts a temperature from Kelvin to Rankine.
///
/// @warning  Will happily convert temperatures below absolute zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegKToDegR(const double kelvin)
{
    return kelvin * UnitConversion::F_PER_C;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kPa  (kPa)  Input pressure in kiloPascals.
///
/// @return   double  (psi)  Pressure converted to pounds per square inch.
///
/// @details  Converts a pressure from kiloPascals to pounds per square inch.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertKpaToPsi(const double kPa)
{
    return kPa * UnitConversion::PSI_PER_KPA;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kPa  (kPa)  Input pressure in kiloPascals.
///
/// @return   double  (mmHg)  Pressure converted to millimeters of mercury.
///
/// @details  Converts a pressure from kiloPascals to millimeters of mercury.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertKpaToMmhg(const double kPa)
{
    return kPa / UnitConversion::KPA_PER_MMHG;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kPa  (kPa)  Input pressure in kiloPascals.
///
/// @return   double  (--)  Pressure converted to millitorr.
///
/// @details  Converts a pressure from kiloPascals to millitorr.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertKpaToMilliTorr(const double kPa)
{
    return kPa * UnitConversion::TORR_PER_KPA * UnitConversion::MILLI_PER_UNIT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kPa  (kPa)  Input pressure in kiloPascals.
///
/// @return   double  (--)  Pressure converted to inches H2O.
///
/// @details  Converts a pressure from kiloPascals to inches of water.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertKpaToInh2o(const double kPa)
{
    return kPa * UnitConversion::UNIT_PER_MILLI * UnitConversion::IN_PER_M / UnitConversion::KPA_PER_MMH2O;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  psi  (psi)  Input pressure in pounds per square inch.
///
/// @return   double  (kPa)  Pressure converted to kiloPascals.
///
/// @details  Converts a pressure from pounds per square inch to kiloPascals.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertPsiToKpa(const double psi)
{
    return psi * UnitConversion::KPA_PER_PSI;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kgPerSec  (kg/s)  Input flow in kilograms per second.
///
/// @return   double      (lbm/hr)  Flow in pounds mass per hour.
///
/// @details  Converts flow from kilograms per second to pounds mass per hour.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertKgPerSecToLbmPerHour(const double kgPerSec)
{
    return kgPerSec * UnitConversion::LBM_PER_HR_PER_KG_PER_SEC;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  kgPerSec  (kg/s)  Input flow in kilograms per second.
///
/// @return   double      (ft3/min)  Standard volumetric flow in cubic feet per minute.
///
/// @details  Converts flow from kilograms per second to standard cubic feet per minute @ 0 C.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertKgPerSecToScfm0C(const double kgPerSec)
{
    return kgPerSec * UnitConversion::SCFM0C_PER_KG_PER_SEC;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  lbmPerHour  (lbm/hr)  Input flow in pounds mass per hour.
///
/// @return   double        (kg/s)  Flow in kilograms per second.
///
/// @details  Converts flow from pounds mass per hour to kilograms per second.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertLbmPerHourToKgPerSec(const double lbmPerHour)
{
    return lbmPerHour * UnitConversion::KG_PER_SEC_PER_LBM_PER_HR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  db  (dB)  Input power ratio in decibels
///
/// @return   double  (--)  Decibels converted to power ratio
///
/// @details  Converts decibels to power ratio.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDbToPr(const double db)
{
    return pow(10.0, 0.1 * db);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  rad  (r)  Input in radians.
///
/// @return   double  (d)  Angle converted to degrees.
///
/// @details  Converts radians to degrees.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertRadToDeg(const double rad)
{
    return rad / UnitConversion::RAD_PER_DEG;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  deg  (d)  Input in degrees.
///
/// @return   double  (r)  Angle converted to radians.
///
/// @details  Converts degrees to radians.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertDegToRad(const double deg)
{
    return deg * UnitConversion::RAD_PER_DEG;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  radPerSec (r/s) Input in radians per second.
///
/// @return   double (rev/min) Angular rate converted to revolutions per minute.
///
/// @details  Converts radians per second to revolutions per minute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertRadPerSecToRpm(const double radPerSec)
{
    return radPerSec * UnitConversion::SEC_PER_MIN_PER_2PI;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  rpm (rev/min) Input in revolutions per minute.
///
/// @return   double (r/s) Angular rate converted to radians per second.
///
/// @details  Converts revolutions per minute to radians per second.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertRpmtoRadPerSec(const double rpm)
{
    return rpm / UnitConversion::SEC_PER_MIN_PER_2PI;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x         (--)  Input value to be converted to percent.
/// @param[in]    basisInv  (--)  Input inverse of value corresponding to 100%.
///
/// @return   Value converted to percent.
///
/// @note     Example: What percentage of 4 is 2?
///           Answer: UnitConversion::convertToPercent(2.0, 1.0 / 4.0)
///
/// @details  Converts a value to a percentage of the basis value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertToPercent(const double x, const double basisInv)
{
    return x * basisInv * 100.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    x      (--)  Input percent value to be converted.
/// @param[in]    basis  (--)  Input value corresponding to 100%.
///
/// @return   Value converted from percent.
///
/// @note     Example: What is 50% of 4?
///           Answer: UnitConversion::convertFromPercent(50.0, 4.0)
///
/// @details  Converts a percentage of the basis value to a value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double UnitConversion::convertFromPercent(const double x,  const double basis)
{
    return x * basis * 0.01;
}

#endif
