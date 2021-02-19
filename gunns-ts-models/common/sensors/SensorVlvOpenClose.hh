#ifndef SensorVlvOpenClose_EXISTS
#define SensorVlvOpenClose_EXISTS

/**
@defgroup  TSM_SENSORS_SensorVlvOpenClose  Valve Position Sensors - Discrete Open and Close (SensorVlvOpenClose)
@ingroup   TSM_SENSORS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 - (Provides an open and close sensor for a valve.)

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - (
    (SensorVlvOpenClose.o)
   )

 PROGRAMMERS:
 - ((Chuck Sorensen) (LZT) (June, 2012) (TS21) (initial))
@{
*/

#include <string>

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "common/sensors/SensorBooleanAi.hh"

/// @brief ConfigData for model
class SensorVlvOpenCloseConfigData  {
public:

    SensorBooleanAiConfigData mOpen;    /**< (--) trick_chkpnt_io(**) Open sensor config  */
    SensorBooleanAiConfigData mClose;   /**< (--) trick_chkpnt_io(**) CLose sensor config */

    SensorVlvOpenCloseConfigData(
            const SensorBooleanAiConfigData &open  = SensorBooleanAiConfigData(false, 1.0,  0.0001),
            const SensorBooleanAiConfigData &close = SensorBooleanAiConfigData(false, 0.0,  0.0001));
    virtual ~SensorVlvOpenCloseConfigData();

    // ok to use compiler generated copy constructor and operator=

protected:

private:

};


/// @brief InputData for model
class SensorVlvOpenCloseInputData  {
public:

    SensorBooleanAiInputData mOpen;         /**< (--) open sensor input    */
    SensorBooleanAiInputData mClose;        /**< (--) close sensor input   */

    SensorVlvOpenCloseInputData(const bool isPowered = true);
    virtual ~SensorVlvOpenCloseInputData();

    // ok to use compiler generated copy constructor and operator=

protected:

private:

};

/// @brief Valve sensor set hold an Open sensor and close sensor.
class SensorVlvOpenClose  {
    TS_MAKE_SIM_COMPATIBLE(SensorVlvOpenClose);
public:

    SensorVlvOpenClose();
    virtual ~SensorVlvOpenClose();
    void initialize(const SensorVlvOpenCloseConfigData &config,
                    const SensorVlvOpenCloseInputData &input,
                    const std::string& name);
    bool isClosed() const;
    bool isOpen() const;
    virtual void sense(const double deltaTime, const bool isPowered, const double position);
    void setPowerFlag(const bool power);
    void setTruthInput(const double truth);
    void update(const double deltaTime);
    bool isInitialized() const;

protected:

    SensorBooleanAi mOpen;         /**< (--) open   sensor  */
    SensorBooleanAi mClose;        /**< (--) closed sensor  */
    bool  mInitFlag;               /**< (--) Sensor object has been initialized */

private:
    /// @brief keep private, never used
    SensorVlvOpenClose(const SensorVlvOpenClose &rhs);

    /// @brief keep private, never used
    SensorVlvOpenClose& operator= (const SensorVlvOpenClose &rhs);

};


/// @}

/// @brief returns the sensed position of the Open sensor
/// @return (--) sensed value of open sensor
inline bool SensorVlvOpenClose::isOpen() const {
    return mOpen.getSensedOutput();
}

/// @brief returns the sensed position of the Closed sensor
/// @return (--) sensed value of closed sensor
inline bool SensorVlvOpenClose::isClosed() const {
    return mClose.getSensedOutput();
}

/// @param[in] power (--) power flag
/// @brief set the powerFlag to the open and close sensors
inline void SensorVlvOpenClose::setPowerFlag(const bool power) {
    mClose.setPowerFlag(power);
    mOpen.setPowerFlag(power);
}

/// @param[in] truth (--) truth input
/// @brief set the truth input to the open and close sensors
inline void SensorVlvOpenClose::setTruthInput(const double truth) {
    mClose.setTruthInput(truth);
    mOpen.setTruthInput(truth);
}


/// @returns bool (--) True if the sensor has been properly initialized, false if not.
/// @brief  Returns the initialized status of the sensor pair.
inline bool SensorVlvOpenClose::isInitialized() const {
    return mInitFlag;
}



#endif //SensorVlvOpenClose_EXISTS
