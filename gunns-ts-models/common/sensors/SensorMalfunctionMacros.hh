#ifndef SensorMalfunctionMacros_EXISTS
#define SensorMalfunctionMacros_EXISTS

/**************************** TRICK HEADER ********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:
    (Macros to reduce the drudgery associated with malfunction mapping for the Sensor classes    )

REFERENCE:
    ()

ASSUMPTIONS AND LIMITATIONS:
    ()

PROGRAMMERS:
(   ((Chuck Sorensen) (LZT) (Initial) (Dec 2012))
)
**************************************************************************************************/

/// @details generate the public parms for a SensorAnalog
/// @param name (--) prefix to sensor (with underscores)
#define TS_SENSOR_ANALOG_PUBLIC_PARAMS(name)    \
bool    * name##_mMalfFailOffFlag;              \
bool    * name##_mMalfFailToFlag;               \
float   * name##_mMalfFailToValue;              \
bool    * name##_mMalfFailStuckFlag;            \
bool    * name##_mMalfPerfectSensor;            \
bool    * name##_mMalfIgnorePower;              \
bool    * name##_mMalfScaleFlag;                \
float   * name##_mMalfScaleValue;               \
bool    * name##_mMalfBiasFlag;                 \
float   * name##_mMalfBiasValue;                \
bool    * name##_mMalfDriftFlag;                \
float   * name##_mMalfDriftRate;                \
bool    * name##_mMalfNoiseFlag;                \
float   * name##_mMalfNoiseScale;               \
bool    * name##_mMalfResolutionFlag;           \
float   * name##_mMalfResolutionValue;

/// @details generate the params mapping for SensorAnalog
/// @param name   (--) prefix to sensor (with underscores)
/// @param target (--) prefix to sensor (C++ variable name)
#define TS_SENSOR_ANALOG_PARAM_MAPPING(name, target)            \
name##_mMalfFailOffFlag     = &target.mMalfFailOffFlag;         \
name##_mMalfFailToFlag      = &target.mMalfFailToFlag;          \
name##_mMalfFailToValue     = &target.mMalfFailToValue;         \
name##_mMalfFailStuckFlag   = &target.mMalfFailStuckFlag;       \
name##_mMalfPerfectSensor   = &target.mMalfPerfectSensor;       \
name##_mMalfIgnorePower     = &target.mMalfIgnorePower;         \
name##_mMalfScaleFlag       = &target.mMalfScaleFlag;           \
name##_mMalfScaleValue      = &target.mMalfScaleValue;          \
name##_mMalfBiasFlag        = &target.mMalfBiasFlag;            \
name##_mMalfBiasValue       = &target.mMalfBiasValue;           \
name##_mMalfDriftFlag       = &target.mMalfDriftFlag;           \
name##_mMalfDriftRate       = &target.mMalfDriftRate;           \
name##_mMalfNoiseFlag       = &target.mMalfNoiseFlag;           \
name##_mMalfNoiseScale      = &target.mMalfNoiseScale;          \
name##_mMalfResolutionFlag  = &target.mMalfResolutionFlag;      \
name##_mMalfResolutionValue = &target.mMalfResolutionValue;


/// @details macro defines param mappign variables for  SensorBooleanBi or SensorBooleanAi
/// @param name (--) prefix to sensor
#define TS_SENSOR_BOOLEAN_PUBLIC_PARAMS(name)   \
bool    * name##_mMalfFailOffFlag;              \
bool    * name##_mMalfFailToFlag;               \
bool    * name##_mMalfFailStuckFlag;            \
bool    * name##_mMalfPerfectSensor;            \
bool    * name##_mMalfIgnorePower;              \
bool    * name##_mMalfFailToValue;

/// @details macro assign param mapping variables for  SensorBooleanBi or SensorBooleanAi
/// @param name   (--) prefix to sensor (with underscores)
/// @param target (--) prefix to sensor (C++ variable name)
#define TS_SENSOR_BOOLEAN_PARAM_MAPPING(name, target)   \
name##_mMalfFailOffFlag   = &target.mMalfFailOffFlag;   \
name##_mMalfFailToFlag    = &target.mMalfFailToFlag;    \
name##_mMalfFailStuckFlag = &target.mMalfFailStuckFlag; \
name##_mMalfPerfectSensor = &target.mMalfPerfectSensor; \
name##_mMalfIgnorePower   = &target.mMalfIgnorePower;   \
name##_mMalfFailToValue   = &target.mMalfFailToValue;


///@}


#endif // SensorMalfunctionMacros_EXISTS
