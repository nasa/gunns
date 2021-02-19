#ifndef GunnsFluidLiquidWaterSensor_EXISTS
#define GunnsFluidLiquidWaterSensor_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_LIQUID_WATER_SENSOR    Liquid Water Sensor Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Liquid Water Sensor link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidLiquidWaterSensor.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2012-04) (Initial))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Liquid Water Sensor Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Liquid Water Sensor link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLiquidWaterSensorConfigData : public GunnsFluidConductorConfigData
{
    public:
        /// @brief    Default constructs this Sensor configuration data.
        GunnsFluidLiquidWaterSensorConfigData(const std::string& name                 = "",
                                              GunnsNodeList*     nodes                = 0,
                                              const double       maxConductivity      = 0.0,
                                              const double       expansionScaleFactor = 0.0);
        /// @brief    Copy constructs this Sensor configuration data.
        GunnsFluidLiquidWaterSensorConfigData(const GunnsFluidLiquidWaterSensorConfigData& that);
        /// @brief    Default destructs this Sensor configuration data.
        virtual ~GunnsFluidLiquidWaterSensorConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidLiquidWaterSensorConfigData& operator =
                (const GunnsFluidLiquidWaterSensorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Liquid Water Sensor Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Liquid Water Sensor link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLiquidWaterSensorInputData : public GunnsFluidConductorInputData
{
    public:
        /// @brief    Default constructs this Sensor input data.
        GunnsFluidLiquidWaterSensorInputData(const bool   malfBlockageFlag  = false,
                                             const double malfBlockageValue = 0.0);
        /// @brief    Copy constructs this Sensor input data.
        GunnsFluidLiquidWaterSensorInputData(const GunnsFluidLiquidWaterSensorInputData& that);
        /// @brief    Default destructs this Sensor input data.
        virtual ~GunnsFluidLiquidWaterSensorInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidLiquidWaterSensorInputData& operator =
                (const GunnsFluidLiquidWaterSensorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Liquid Water Sensor Model
///
/// @details  The GUNNS Fluid Liquid Water Sensor link model provides the relative humidity and a
///           boolean output indicating whether any of the water in a gas mixture would be condensed
///           into liquid.  This is intended to be used in a gas network on nodes where gaseous H2O
///           is in the fluid config.  GUNNS doesn't model 2-phase mixtures, so the H2O is left in
///           gaseous phase even though temperatures & dewpoint would cause it to condense.  This
///           sensor indicates whether such condensation conditions are present in the port 0 node.
///           The link is a fluid conductor so flow can pass through it when it has a non-zero
///           conductivity.  Care should be taken when connecting this to vacuum at port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLiquidWaterSensor : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidLiquidWaterSensor);
    public:
        /// @brief    Default constructs this Sensor.
        GunnsFluidLiquidWaterSensor();
        /// @brief    Default destructs this Sensor.
        virtual ~GunnsFluidLiquidWaterSensor();
        /// @brief    Initializes this Sensor with configuration and input data.
        void initialize(const GunnsFluidLiquidWaterSensorConfigData& configData,
                        const GunnsFluidLiquidWaterSensorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);
        /// @brief    Returns true if liquid water is present.
        bool   getLiquidPresent() const;
        /// @brief    Returns the relative humidity.
        double getRelativeHumidity() const;

    protected:
        bool   mLiquidPresent;      /**< (--)  True when water can condense in the gas mixture (output to simbus). */
        double mRelativeHumidity;   /**< (--)  Relative humidity of the gas mixture (output to simbus).            */
        double mDewPoint;           /**< (K)   Dewpoint temperature of the gas mixture (output to simbus).         */
        double mSaturationPressure; /**< (kPa) Saturation pressure of water in the gas mixture (output to simbus). */
        /// @brief    Updates the state of this Sensor.
        virtual void updateState(const double dt);
        /// @brief    Validates the link initial conditions.
        void         validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidLiquidWaterSensor(const GunnsFluidLiquidWaterSensor&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidLiquidWaterSensor& operator =(const GunnsFluidLiquidWaterSensor&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) The Liquid Present flag.
///
/// @details  This getter method returns true if there is liquid present.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidLiquidWaterSensor::getLiquidPresent() const
{
    return mLiquidPresent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (--) Relative humidity of the gas mixture (1.0 = 100% humidity).
///
/// @details  This getter method returns the relative humidity of the gas mixture.  A value of 1.0
///           represents 100% humidity, and the value can go above 1.0 for saturated mixtures.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidLiquidWaterSensor::getRelativeHumidity() const
{
    return mRelativeHumidity;
}

#endif
