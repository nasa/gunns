#ifndef GunnsFluidSensor_EXISTS
#define GunnsFluidSensor_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_SENSOR    Sensor Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Sensor link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidSensor.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Add) (2011-10))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Sensor Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Sensor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSensorConfigData : public GunnsFluidConductorConfigData
{
    public:
        /// @brief    Default constructs this Sensor configuration data.
        GunnsFluidSensorConfigData(const std::string& name                 = "",
                                   GunnsNodeList*     nodes                = 0,
                                   const double       maxConductivity      = 0.0,
                                   const double       expansionScaleFactor = 0.0);
        /// @brief    Copy constructs this Sensor configuration data.
        GunnsFluidSensorConfigData(const GunnsFluidSensorConfigData& that);
        /// @brief    Default destructs this Sensor configuration data.
        virtual ~GunnsFluidSensorConfigData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSensorConfigData& operator =(const GunnsFluidSensorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Sensor Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Sensor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSensorInputData : public GunnsFluidConductorInputData
{
    public:
        /// @brief    Default constructs this Sensor input data.
        GunnsFluidSensorInputData(const bool   malfBlockageFlag  = false,
                                  const double malfBlockageValue = 0.0);
        /// @brief    Copy constructs this Sensor input data.
        GunnsFluidSensorInputData(const GunnsFluidSensorInputData& that);
        /// @brief    Default destructs this Sensor input data.
        virtual ~GunnsFluidSensorInputData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSensorInputData& operator =(const GunnsFluidSensorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sensor Model
///
/// @details  The GUNNS Fluid Sensor link model provides truth values for temperature, pressure,
///           delta pressure and flow rate sensors. Temperature and pressure are taken from the
///           port 0 fluid. Delta pressure is the difference between the port 0 and port 1 fluid
///           pressures. Mass and volumetric flow rates are positive from port 0 to port 1, which
///           corresponds to a positive delta pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSensor : public  GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSensor);
    public:
        /// @brief    Default constructs this Sensor.
        GunnsFluidSensor();
        /// @brief    Default destructs this Sensor.
        virtual ~GunnsFluidSensor();
        /// @brief    Initializes this Sensor with configuration and input data.
        void initialize(const GunnsFluidSensorConfigData& configData,
                        const GunnsFluidSensorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);
        /// @brief    Transport Flows.
        virtual void transportFlows(const double dt);
        /// @brief    Returns the truth value from this Sensor for a fluid temperature sensor
        double       getTemperature() const;
        /// @brief    Returns the truth value from this Sensor for a fluid pressure sensor
        double       getPressure() const;
        /// @brief    Returns the truth value from this Sensor for a fluid delta pressure sensor
        double       getDeltaPressure() const;
        /// @brief    Returns the truth value from this Sensor for a fluid mass flow rate sensor
        double       getMassFlowRate() const;
        /// @brief    Returns the truth value from this Sensor for a fluid volumetric flow rate sensor
        double       getVolumetricFlowRate() const;
        /// @brief    Returns the truth values from this Sensor for a fluid partial pressure sensors
        double*      getPartialPressure();

    protected:
        double mTemperature;                                /**< (K)     Truth value for a fluid temperature sensor */
        double mPressure;                                   /**< (kPa)   Truth value for a fluid pressure sensor. */
        double mDeltaPressure;                              /**< (kPa)   Truth value for a fluid delta pressure sensor. */
        double mMassFlowRate;                               /**< (kg/s)  Truth value for a fluid mass flow rate sensor. */
        double mVolumetricFlowRate;                         /**< (m3/s)  Truth value for a fluid volumetric flow rate sensor. */
        double mPartialPressure[FluidProperties::NO_FLUID]; /**< (kPa)   Array of truth values for a fluid partial pressure sensor. */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the state of the fluid sensed outputs.
        virtual void updateSensedValues();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSensor(const GunnsFluidSensor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSensor& operator =(const GunnsFluidSensor&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (K) The temperature.
///
/// @details    Returns the truth value from this Sensor for a fluid temperature sensor
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSensor::getTemperature() const
{
    return mTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (kPa) The pressure.
///
/// @details    Returns the truth value from this Sensor for a fluid pressure sensor
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSensor::getPressure() const
{
    return mPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (kPa) The delta pressure.
///
/// @details    Returns the truth value from this Sensor for a fluid delta pressure sensor
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSensor::getDeltaPressure() const
{
    return mDeltaPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (kg/s) The mass flow rate.
///
/// @details    Returns the truth value from this Sensor for a fluid mass flow rate sensor
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSensor::getMassFlowRate() const
{
    return mMassFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double (m3/s) The volumetric flow rate.
///
/// @details    Returns the truth value from this Sensor for a fluid volumetric flow rate sensor
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSensor::getVolumetricFlowRate() const
{
    return mVolumetricFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return     double* -- The array of partial pressures (individual pressures in kPa).
///
/// @details    Returns the truth values from this Sensor for a fluid partial pressure sensors
////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsFluidSensor::getPartialPressure()
{
    return mPartialPressure;
}

#endif

