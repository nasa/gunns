/****************************** TRICK HEADER ******************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
 (
    (software/exceptions/TsInitializationException.o)
    (simulation/hs/TsHsMsg.o)
    (TsOpticSmokeDetector.o)
    (common/sensors/TsNoise.o)
    (common/sensors/SensorAnalog.o)
 )

 PROGRAMMERS:
 (
  ((Jose A. Perez) (L3-Comm) (Mar 23, 2012) (TS21) (Initial))
 )
 **************************************************************************************************/
#include <float.h>
#include "TsOpticSmokeDetector.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "simulation/hs/TsHsMsg.hh"
#include "software/exceptions/TsHsException.hh"
#include "common/sensors/TsNoise.hh"
#include "math/MsMath.hh"
#include "GenericMacros.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor for Config Data
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpticSmokeDetectorConfigData::TsOpticSmokeDetectorConfigData()
    :
    obsMaxPercentage     (104.12),  // Obs.% JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    obsMinPercentage     (-10.0),   // Obs.% JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    obsSmokeContributer  (6.7),     // Tunes the obscuration smoke contribution according to JMEWS.
    scatterMaxPercentage (2.47),    // %/ft. JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    scatterMinPercentage (0.11),    // %/ft. JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    bitOnMaxScatterValue (1.42),    // %/ft. JMEWS Data. Ref. SSP 41002 Table 3.3.4.1.1.3-1 for ISS SD.
    nominalObscuration   (8.24),    // Obs.% JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    nominalScatter       (0.16768), // %/ft. JMEWS Data. Ref. D684-10508-02-02: Sec. 3.3 for ISS SD.
    bitRecoveryTime      (1.5),     // sec.  D684-10508-02-02: Sec. 3.3 for ISS SD: "Quite Period".
    obsOffValue          (53.45),   // Obs.% Per instructor provided data.
    scatOffValue         (0.1006),  // %/ft. Per instructor provided data.
    obsPercentVoltSlope  (-0.08),   // v/%.  Slope in form of y=mx+b. SSP 41002 Rev.P.
    obsVoltIntercept     (4.0),     // v.    Slope y-intercept in form of y=mx+b. SSP 41002 Rev.P..
    scatPercentVoltSlope (2.01),    // v/(%/ft). Slope in form of y=mx+b. SSP 41002 Rev.P.
    scatVoltIntercept    (0.0),     // v.    Slope y-intercept in form of y=mx+b. SSP 41002 Rev.P..
    obsSensorConfigD     (),
    scatterSensorConfigD ()
{
    obsSensorConfigD.mMinRange              = -10.0;  // Obs.%
    obsSensorConfigD.mMaxRange              = 104.12; // Obs.%
    obsSensorConfigD.mNominalNoiseScale     = 0.01;
    obsSensorConfigD.mNoiseFunction         = TsNoise::getNoise;
    scatterSensorConfigD.mMinRange          = 0.0;    // %/ft
    scatterSensorConfigD.mMaxRange          = 2.47;   // %/ft
    scatterSensorConfigD.mNominalNoiseScale = 0.001;
    scatterSensorConfigD.mNoiseFunction     = TsNoise::getNoise;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor for Config Data
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpticSmokeDetectorConfigData::~TsOpticSmokeDetectorConfigData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor for Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpticSmokeDetectorInputData::TsOpticSmokeDetectorInputData()
    :
    isLoadOn            (0),
    isPowered           (false),
    isBitEnabled        (false),
    smokeLevelFactor    (0.0),
    bitTimer            (0.0),
    obsSensorInputD     (),
    scatterSensorInputD ()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor for Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpticSmokeDetectorInputData::~TsOpticSmokeDetectorInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default class constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpticSmokeDetector::TsOpticSmokeDetector()
    :
    obscurationSensor       (),
    scatterSensor           (),
    smokeLevelFactor        (0.0),
    malfTotalFail           (false),
    malfPowerToOn           (false),
    isLoadOn                (0),
    isPowered               (false),
    isBitEnabled            (false),
    isBitInProcess          (false),
    obscurationPercentage   (0.0),
    scatterPercentage       (0.0),
    bitTimer                (0.0),
    obsSensedVoltage        (0.0),
    scatSensedVoltage       (0.0),
    mName                   (),
    mInitialized            (false),
    configD                 ()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default class destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
TsOpticSmokeDetector::~TsOpticSmokeDetector()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws TsInitializationException
///
/// @details  Initialization method that initializes the input and config data of the SD.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpticSmokeDetector::initialize(
    const TsOpticSmokeDetectorConfigData &configData,
    const TsOpticSmokeDetectorInputData  &inputData,
    const std::string                    &name)
{
    mInitialized = false;

    /// - Verify and initialize the fan name.
    TS_GENERIC_NAME_ERREX("TsOpticSmokeDetector");

    //Config Data
    configD.obsMaxPercentage     = configData.obsMaxPercentage;
    configD.obsMinPercentage     = configData.obsMinPercentage;
    configD.obsSmokeContributer  = configData.obsSmokeContributer;
    configD.scatterMaxPercentage = configData.scatterMaxPercentage;
    configD.scatterMinPercentage = configData.scatterMinPercentage;
    configD.bitOnMaxScatterValue = configData.bitOnMaxScatterValue;
    configD.nominalObscuration   = configData.nominalObscuration;
    configD.nominalScatter       = configData.nominalScatter;
    configD.bitRecoveryTime      = configData.bitRecoveryTime;
    configD.obsOffValue          = configData.obsOffValue;
    configD.scatOffValue         = configData.scatOffValue;

    configD.obsSensorConfigD.mMinRange     = configD.obsMinPercentage;
    configD.obsSensorConfigD.mOffValue     = configD.obsOffValue;
    configD.scatterSensorConfigD.mOffValue = configD.scatOffValue;

    //Input Data
    isPowered        = inputData.isPowered;
    isLoadOn         = inputData.isLoadOn;
    isBitEnabled     = inputData.isBitEnabled;
    smokeLevelFactor = inputData.smokeLevelFactor;
    bitTimer         = inputData.bitTimer;

    validate();

    obscurationSensor.initialize(configD.obsSensorConfigD, inputData.obsSensorInputD, "FDS SD Obs Sensor");
    scatterSensor.initialize(configD.scatterSensorConfigD, inputData.scatterSensorInputD, "FDS SD Scat Sensor");

    mInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The Optic Smoke Detector consist of two photoelectric sensors, the obscuration and
///           scatter sensors. The obscuration sensor measures the light intensity the light source
///           is producing. The scatter sensor measures the 'light scattering' of the light source
///           due to smoke or any other type of light obstruction. The smoke detector is capable of
///           performing an Active Bit test when ever it is commanded to do so. This is done to
///           verify the smoke detector is functioning properly. The smoke detector outputs are
///           obscuration and scatter percentages. Its inputs are a power command,a bit enable
///           command, and malfunctions. The malfunctions include malfunctioning the scatter and
///           obscuration telemetry, a power malfunction and a malfunction of the Smoke Detector
///           as a whole.
///
/// @return void
///////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpticSmokeDetector::update(
    const float dt)
{
    isLoadOn = static_cast<int>((isPowered || malfPowerToOn) && !malfTotalFail);

    if (isLoadOn)
    {
        if(isBitEnabled || isBitInProcess)
        {
            performBitTest(dt);
        }
        else
        {
            bitTimer = 0.0;

            obscurationPercentage = configD.nominalObscuration + (configD.obsSmokeContributer * smokeLevelFactor);

            scatterPercentage = configD.nominalScatter + (configD.scatterMaxPercentage - configD.nominalScatter) * smokeLevelFactor;
        }
    }
    else
    {
        obscurationPercentage = configD.obsOffValue;
        scatterPercentage     = configD.scatOffValue;
    }

    obscurationPercentage = MsMath::limitRange(configD.obsMinPercentage, obscurationPercentage, configD.obsMaxPercentage);
    scatterPercentage     = MsMath::limitRange(0.0, scatterPercentage, configD.scatterMaxPercentage);

    bool sensorPower = static_cast<bool>(isLoadOn);
    obscurationSensor.setTruthInput(obscurationPercentage);
    obscurationSensor.setPowerFlag(sensorPower);
    scatterSensor.setTruthInput(scatterPercentage);
    scatterSensor.setPowerFlag(sensorPower);

    obscurationSensor.update(dt);
    scatterSensor.update(dt);

    /// - Coltru needs a sensed voltage. Convert values to voltage linearly.
    //        voltage = m * x + b
    obsSensedVoltage  = configD.obsPercentVoltSlope * obscurationSensor.getSensedOutput() + configD.obsVoltIntercept;
    scatSensedVoltage = configD.scatPercentVoltSlope * scatterSensor.getSensedOutput() + configD.scatVoltIntercept;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method that checks if input and config data were initialized properly.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpticSmokeDetector::validate()
{
    /// - Throw an exception if min obscuration is greater than or equal to max obscuration percentage.
    TS_GENERIC_IF_ERREX((configD.obsMinPercentage >= configD.obsMaxPercentage), TsInitializationException,
                        "Invalid Configuration Data", ": obsMinPercentage input must be less than obsMaxPercentage input.");

    /// - Throw an exception if min scatter is greater than or equal to max scatter percentage.
    TS_GENERIC_IF_ERREX((configD.scatterMinPercentage >= configD.scatterMaxPercentage), TsInitializationException,
                        "Invalid Configuration Data", ": scatterMinPercentage input must be less than scatterMaxPercentage input.");

    /// - Throw an exception if nominal obscuration is less than or equal to min obscuration percentage.
    TS_GENERIC_IF_ERREX((configD.nominalObscuration <= configD.obsMinPercentage), TsInitializationException,
                        "Invalid Configuration Data", ": nominalObscuration input must be greater than obsMinPercentage input.");

    /// - Throw an exception if nominal scatter is less than or equal to min scatter percentage.
    TS_GENERIC_IF_ERREX((configD.nominalScatter <= configD.scatterMinPercentage), TsInitializationException,
                        "Invalid Configuration Data", ": nominalScatter input must be greater than scatterMinPercentage input.");

    /// - Throw an exception if max bit-on scatter is less or equal to min scatter percentage.
    TS_GENERIC_IF_ERREX((configD.bitOnMaxScatterValue <= configD.scatterMinPercentage), TsInitializationException,
                        "Invalid Configuration Data", ": bitOnMaxScatterValue input must be greater than scatterMinPercentage input.");

    /// - Throw an exception if bit-recovery-time is less than or equal to tolerance.
    TS_GENERIC_IF_ERREX((configD.bitRecoveryTime <= FLT_MIN), TsInitializationException,
                        "Invalid Configuration Data", ": bitRecoveryTime input value cannot be negative, zero or be close to zero.");

    /// - Throw an exception if smokeLevelFactor is not within 0.0 and 1.0.
    TS_GENERIC_IF_ERREX((smokeLevelFactor < 0.0 || smokeLevelFactor > 1.0), TsInitializationException,
                        "Invalid Initialization Data", ": smokeLevelFactor input value can only be from 0.0 to 1.0.");

    /// - Throw an exception if bit timer is less than 0.0.
    TS_GENERIC_IF_ERREX((bitTimer < 0.0), TsInitializationException,
                        "Invalid Initialization Data", ": bitTimer input value cannot be negative.");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Method that executes the smoke detector's Active Bit Test when ever the isBitEnable
///           flag is set to true. Nominal Bit Test neglecting noise performs the following:
///
///                    |                OBSCURATION BIT DIAGRAM
///         Max Obs.  _|_             _ _ _ _ _ _ _ _ _ _ _ _
///                    |             :                        :
///      Nominal Obs. _|_ _ _ _ _ _ _:                        :_ _ _ _ _
///                    |
///                    |_____________|_______|________________|_________
///                    |             |       |                |
///                    x0            x1      x2               x3
///
///
///                    |                  SCATTER BIT DIAGRAM
///   Bit-on Max Scat._|_             _ _ _ _
///                    |             :       :
///     Nominal Scat. _|_ _ _ _ _ _ _:       :_ _ _ _ _ _ _ _ _ _ _ _ _
///                    |
///                    |_____________|_______|________________|_________
///                    |             |       |                |
///                    x0            x1      x2               x3
///
///
///                                            LEGEND
///                x0 - Any time step the diagram starts at.
///                x1 - Time FSW sets Bit command; isBitEnable is set to true.
///                x2 - Time FSW removes the Bit command; isBitEnable is set to false.
///                x3 - Time smoke detector fully completes the Bit test; bitTimer exceeds bitRecoveryTime.
///                Y axis - Percentage (%).
///                X axis - Time (s).
///             * When Obscuration or Scatter telemetry is malfunctioned, dashed lines shift according
///               to the malfunction factor, hence causing Active Bit test to fail.
////////////////////////////////////////////////////////////////////////////////////////////////////
void TsOpticSmokeDetector::performBitTest(
    const float dt)
{
    isBitInProcess = bitTimer <= configD.bitRecoveryTime;

    obscurationPercentage = configD.obsMaxPercentage;

    if(isBitEnabled)
    {
        scatterPercentage = configD.bitOnMaxScatterValue;
    }
    else
    {
        bitTimer += dt;

        scatterPercentage = configD.scatterMinPercentage;
    }
}
