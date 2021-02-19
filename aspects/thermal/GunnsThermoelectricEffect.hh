#ifndef GunnsThermoelectricEffect_EXISTS
#define GunnsThermoelectricEffect_EXISTS

/**
@file
@brief    GUNNS thermoelectric effect declarations

@defgroup  TSM_GUNNS_THERMAL_ELECTRIC_EFFECT    GUNNS Thermoelectric Effect Model
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (This is a utility class for modeling the Seebeck, Peltier and Thompson thermoelectric effects and
   Joule heating in a thermoelectric device (TED).  This utility model can be included in a GUNNS
   spotter, link or other classes as needed.

   This models the thermal aspect of one TED -- the electrical aspect is a GunnsBasicPotential
   (voltage source + conductor) in an electrical network.  This thermal aspect model could be
   included in a thermal network by embedding in a link, or it could be included in an electrical
   network by embedding in a spotter.

   We define one TED to be any number of thermocouples (p-n conductor pairs) that are thermally in
   parallel (i.e. share the same end plates) and electrically in series.  This model can be
   configured to represent one or more thermocouples in such an arrangement in the TED.  The
   configuration data defining the thermoelectric properties of the thermocouples is flexible in
   scope: it can define a single p-type or n-type element that is then multiplied by 2 * the
   number of thermocouples in the TED, or it can define the overall properties of the entire TED.
   See details in the GunnsThermoelectricEffectConfigData class below.

   This design can be used for both thermoelectric heat generation and electrical generation.  Heat
   and current can flow in either direction.  This can also be used as a temperature-dependent
   conductor (electrical and/or thermal) by zeroing the Seebeck coefficients or not hooking up the
   input current or output voltage interfaces.

   This models one TED.  Multiple TED's can be staged or cascaded together in real-world
   applications.  In these cases, simply instantiate this utility separately for each stage, each
   with its own objects.  Stages can be thermally in series (i.e. the "hot" plate of one TE stage is
   connected to the "cold" plate of the next) or thermally in parallel (both stages "hot" plates
   connect to the same sink node, etc).  In these cases, the stages can share the same thermal
   network node where they interface, but make sure to combine their thermal capacitance into just
   one thermal capacitor link on each node.  The same goes for the electrical aspect staged in
   parallel or series.)

REQUIREMENTS:
  ()

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ((If material properties are given for one p-type element, then they are assumed the same for the
    n-type element, with opposite polarity.)
   (All thermocouples in the TED are thermally in parallel and electrically in series.)
   (This model does not ensure that efficiency is < 1.  That is left to the user's configuration
    data.))

LIBRARY DEPENDENCY:
  (GunnsThermoelectricEffect.o)

PROGRAMMERS:
  (
   ((Jason Harvey) (L-3 Communications) (September 2015) (Initial))
  )
@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermoelectric Effect Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermoelectric
///           Effect configuration data.
///
/// @note     You can define the material properties for one p-type element, or for the overall TED
///           as a whole.  You must use the same choice for all config parameters -- don't define
///           one parameter for an individual element and others for the entire TED, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermoelectricEffectConfigData
{
    public:
        /// @details  If material properties are given for the overall TED, then use 0.5 here.
        double mNumThermocouples;              /**< (--)    trick_chkpnt_io(**) Number of thermocouples (p-n pairs) thermally in parallel and electrically in series. */
        /// @details  If material properties are given for the overall TED, then use 1.0 here.
        double mGeometryFactor;                /**< (m)     trick_chkpnt_io(**) Cross-sectional area over length of each p-type or n-type element. */
        /// @details  You can define this for one p-type element, OR for the overall TED as a whole.
        double mElectricalResistivityCoeff[2]; /**< (ohm*m) trick_chkpnt_io(**) Line coefficients of electrical resistivity of material function of temperature. */
        /// @details  You can define this for one p-type element, OR for the overall TED as a whole.
        double mSeebeckCoeff[3];               /**< (V/K)   trick_chkpnt_io(**) 2nd-order polynomial coefficients of Seebeck coefficient of material function of temperature. */
        /// @details  You can define this for one p-type element, OR for the overall TED as a whole.
        double mThermalConductivityCoeff[3];   /**< (W/m/K) trick_chkpnt_io(**) 2nd-order polynomial coefficients of thermal conductivity of material function of temperature. */
        /// @details  If material properties are given for the overall TED and thermal conductivity
        ///           already includes the end-plates, then use 0.0 here.
        double mEndPlateThermalConductance;    /**< (W/K)   trick_chkpnt_io(**) Thermal conductance of one end plate. */
        double mMinTemperature;                /**< (K)     trick_chkpnt_io(**) Minimum temperature for evaluating material properties functions. */
        double mMaxTemperature;                /**< (K)     trick_chkpnt_io(**) Maximum temperature for evaluating material properties functions. */
        /// @brief  Default constructs this Thermoelectric Effect configuration data.
        GunnsThermoelectricEffectConfigData(const double numThermocouples            = 0.0,
                                            const double geometryFactor              = 0.0,
                                            const double electricalResistivityCoeff0 = 0.0,
                                            const double electricalResistivityCoeff1 = 0.0,
                                            const double seebeckCoeff0               = 0.0,
                                            const double seebeckCoeff1               = 0.0,
                                            const double seebeckCoeff2               = 0.0,
                                            const double thermalConductivityCoeff0   = 0.0,
                                            const double thermalConductivityCoeff1   = 0.0,
                                            const double thermalConductivityCoeff2   = 0.0,
                                            const double endPlateThermalConductance  = 0.0,
                                            const double minTemperature              = 0.0,
                                            const double maxTemperature              = 0.0);
        /// @brief  Default destructs this Thermoelectric Effect configuration data.
        virtual ~GunnsThermoelectricEffectConfigData();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsThermoelectricEffectConfigData(const GunnsThermoelectricEffectConfigData& that);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermoelectricEffectConfigData& operator =(const GunnsThermoelectricEffectConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermoelectric Effect Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermoelectric
///           Effect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermoelectricEffectInputData
{
    public:
        bool   mMalfThermoelectricEffectsFlag;   /**< (--)  Initial thermoelectric effects scalar malfunction flag. */
        double mMalfThermoelectricEffectsScalar; /**< (--)  Initial thermoelectric effects scaler malfunction value. */
        double mCurrent;                         /**< (amp) Initial electrical current thru the device. */
        double mTemperatureHot;                  /**< (K)   Initial "hot" side temperature. */
        double mTemperatureCold;                 /**< (K)   Initial "cold" side temperature. */
        /// @brief   Default constructs this ThermalSource InputData.
        GunnsThermoelectricEffectInputData(const bool   malfThermoelectricEffectsFlag   = false,
                                           const double malfThermoelectricEffectsScalar = 0.0,
                                           const double current                         = 0.0,
                                           const double temperatureHot                  = 0.0,
                                           const double temperatureCold                 = 0.0);
        /// @brief  Default destructs this Thermoelectric Effect input data.
        virtual ~GunnsThermoelectricEffectInputData();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsThermoelectricEffectInputData(const GunnsThermoelectricEffectInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsThermoelectricEffectInputData& operator =(const GunnsThermoelectricEffectInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermoelectric effect model utitlity class.
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermoelectricEffect
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermoelectricEffect);

    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfThermoelectricEffectsFlag;   /**< (--) Thermoelectric effects scalar malfunction flag. */
        double mMalfThermoelectricEffectsScalar; /**< (--) Thermoelectric effects scaler malfunction value. */
        /// @}
        /// @brief  Default Constructor.
        GunnsThermoelectricEffect();
        /// @brief  Default Destructor.
        virtual ~GunnsThermoelectricEffect();
        /// @brief  Initializes the model
        void   initialize(const GunnsThermoelectricEffectConfigData& configData,
                          const GunnsThermoelectricEffectInputData&  inputData,
                          const std::string&                         name);
        /// @brief  Resets class attributes for a simulation restart.
        void   restart();
        /// @brief  Updates the model state.
        void   update();
        /// @brief  Sets the electrical current through the device.
        void   setCurrent(const double current);
        /// @brief  Sets the hot-side terminal temperature of the device.
        void   setTemperatureHot(const double temperature);
        /// @brief  Sets the cold-side terminal temperature of the device.
        void   setTemperatureCold(const double temperature);
        /// @brief Sets and resets the thermoelectric effects scale malfunction.
        void   setMalfThermoelectricEffects(const bool   flag  = false,
                                            const double value = 0.0);
        /// @brief  Gets the thermal conductance of the device.
        double getThermalConductance() const;
        /// @brief  Gets the hot terminal heat flux (power).
        double getHeatFluxHot() const;
        /// @brief  Gets the cold terminal heat flux (power).
        double getHeatFluxCold() const;
        /// @brief  Gets the thru heat flux (power).
        double getHeatFluxThru() const;
        /// @brief  Gets the electrical conductance of the device.
        double getElectricalConductance() const;
        /// @brief  Gets the Seebeck effect voltage of the device.
        double getVoltage() const;
        /// @brief  Returns true if the model has been successfully initialized.
        bool   isInitialized() const;

    protected:
        std::string mName;                        /**< (--)  trick_chkpnt_io(**) Instance name for output messages. */
        double      mResistanceCoeffs[2];         /**< (ohm) trick_chkpnt_io(**) Line coefficients of electrical resistance of device function of temperature. */
        double      mSeebeckCoeffs[3];            /**< (V/K) trick_chkpnt_io(**) 2nd-order polynomial coefficients of Seebeck coefficient of device function of temperature. */
        double      mThermalConductanceCoeffs[3]; /**< (W/K) trick_chkpnt_io(**) 2nd-order polynomial coefficients of thermal conductance of device function of temperature. */
        double      mMinTemperature;              /**< (K)   trick_chkpnt_io(**) Minimum temperature for evaluating material properties functions. */
        double      mMaxTemperature;              /**< (K)   trick_chkpnt_io(**) Maximum temperature for evaluating material properties functions. */
        double      mCurrent;                     /**< (amp) trick_chkpnt_io(**) Electrical current thru the device. */
        double      mTemperatureHot;              /**< (K)   trick_chkpnt_io(**) Hot-side terminal temperature. */
        double      mTemperatureCold;             /**< (K)   trick_chkpnt_io(**) Cold-side terminal temperature. */
        double      mThermalConductance;          /**< (W/K)                     Output thermal conductance. */
        double      mHeatFluxHot;                 /**< (W)                       Output hot-side terminal heat flux (power). */
        double      mHeatFluxCold;                /**< (W)                       Output cold-side terminal heat flux (power). */
        double      mHeatFluxThru;                /**< (W)                       Output through heat flux (power) from hot to cold terminals. */
        double      mElectricalConductance;       /**< (1/ohm)                   Output electrical conductance. */
        double      mVoltage;                     /**< (V)                       Output source voltage. */
        bool        mInitFlag;                    /**< (--)  trick_chkpnt_io(**) Model initialization complete flag. */
        /// @brief  Validates the Thermoelectric Effect initial state.
        void   validate(const GunnsThermoelectricEffectConfigData& configData,
                        const GunnsThermoelectricEffectInputData&  inputData) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsThermoelectricEffect(const GunnsThermoelectricEffect& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsThermoelectricEffect& operator =(const GunnsThermoelectricEffect& that);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] current  (amp)  Electrical current through the link.
///
/// @details  Sets mCurrent equal to the supplied value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void   GunnsThermoelectricEffect::setCurrent(const double current)
{
    mCurrent = current;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] temperature  (K)  Temperature at the hot-side terminal.
///
/// @details  Sets mTemperatureHot equal to the supplied value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void   GunnsThermoelectricEffect::setTemperatureHot(const double temperature)
{
    mTemperatureHot = temperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] temperature  (K)  Temperature at the cold-side terminal.
///
/// @details  Sets mTemperatureCold equal to the supplied value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void   GunnsThermoelectricEffect::setTemperatureCold(const double temperature)
{
    mTemperatureCold = temperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) Malfunction activation flag, true activates.
/// @param[in] value (--) Malfunction scale factor value.
///
/// @details Sets the thermoelectric effects scale malf to the given state.  Calling this method
///          with default arguments resets the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void   GunnsThermoelectricEffect::setMalfThermoelectricEffects(const bool   flag,
                                                                      const double value)
{
    mMalfThermoelectricEffectsFlag   = flag;
    mMalfThermoelectricEffectsScalar = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W/K) Thermal conductance through the device.
///
/// @details  Returns mThermalConductance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermoelectricEffect::getThermalConductance() const
{
    return mThermalConductance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Heat flux added to the hot-side terminal.
///
/// @details  Returns mHeatFluxHot.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermoelectricEffect::getHeatFluxHot() const
{
    return mHeatFluxHot;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Heat flux added to the cold-side terminal.
///
/// @details  Returns mHeatFluxCold.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermoelectricEffect::getHeatFluxCold() const
{
    return mHeatFluxCold;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Heat flux conducted through the device.
///
/// @details  Returns mHeatFluxThru.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermoelectricEffect::getHeatFluxThru() const
{
    return mHeatFluxThru;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (1/ohm) Electrical conductance through the device.
///
/// @details  Returns mElectricalConductance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermoelectricEffect::getElectricalConductance() const
{
    return mElectricalConductance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (V) Electrical voltage created by the device.
///
/// @details  Returns mVoltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermoelectricEffect::getVoltage() const
{
    return mVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if the model is successfully initialized and ready to update.
///
/// @details  Returns mInitFlag.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool   GunnsThermoelectricEffect::isInitialized() const
{
    return mInitFlag;
}

#endif
