#ifndef MonoFluid_EXISTS
#define MonoFluid_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_FLUID_MONO_FLUID  MonoFluid Model
@ingroup   TSM_GUNNS_FLUID_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling a single species fluid.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((MonoFluid.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "properties/FluidProperties.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    MonoFluid Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the
///           MonoFluid input data (temperature, pressure, mass flow rate and mass).
////////////////////////////////////////////////////////////////////////////////////////////////////
class MonoFluidInputData
{
    public:
        /// (K)    Temperature of the fluid
        double  mTemperature; // (K)    Temperature of the fluid
        /// (kPa)  Pressure of the fluid
        double  mPressure;    // (kPa)  Pressure of the fluid
        /// (kg/s) Mass flow rate of the fluid
        double  mFlowRate;    // (kg/s) Mass flow rate of the fluid
        /// (kg)   Mass of the fluid
        double  mMass;        // (kg)   Mass of the fluid
        /// @brief Default constructs this MonoFluid input data.
        MonoFluidInputData(const double temperature = 0.0,
                           const double pressure    = 0.0,
                           const double flowRate    = 0.0,
                           const double mass        = 0.0);
        /// @brief Copy constructs this MonoFluid input data.
        MonoFluidInputData(const MonoFluidInputData& that);
        /// @brief Default destructs this MonoFluid input data.
        virtual ~MonoFluidInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        MonoFluidInputData& operator=(const MonoFluidInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Class for MonoFluid, a single species fluid.
///
/// @note     This class is intended for the sole use of PolyFluid to model a constituent. PolyFluid
///           is responsible for validation before making any calls. There are no public methods and
///           PolyFluid is made a friend. This class would have been declared in the protected
///           section of PolyFluid had Trick supported that.
///
/// @details  Provides the class for a single species fluid containing state (temperature, pressure,
///           mass flow rate, mass and moles) as well as properties (type, molecular weight,
///           density, dynamic viscosity, specific heat, specific enthalpy and adiabatic index.
////////////////////////////////////////////////////////////////////////////////////////////////////
class MonoFluid
{
    TS_MAKE_SIM_COMPATIBLE(MonoFluid);
    public:
        /// @brief Default constructs this MonoFluid.
        MonoFluid();
        /// @brief Default destructs this MonoFluid.
        virtual ~MonoFluid();
        /// @brief Returns the density at the specified temperature and pressure.
        double computeDensity(const double temperature, const double pressure = 0.0) const;
        /// @brief Construct this MonoFluid with properties and input data.
        MonoFluid(const FluidProperties* properties, const MonoFluidInputData& inputData);
        /// @brief Copy construct this MonoFluid.
        MonoFluid(const MonoFluid& that);
        /// @brief Initializes this MonoFluid with configuration (properties) and input
        /// (initial state) data.
        void initialize(const FluidProperties* properties, const MonoFluidInputData& inputData);
        /// @brief  Validates this MonoFluid Model initial state.
        void validate();
        /// @brief Computes this MonoFluid derived state and properties.
        void derive();
        /// @brief Returns the temperature of this MonoFluid.
        double getTemperature() const;
        /// @brief Returns the pressure of this MonoFluid.
        double getPressure() const;
        /// @brief Returns the mass flow rate of this MonoFluid.
        double getFlowRate() const;
        /// @brief Returns the mass of this MonoFluid.
        double getMass() const;
        /// @brief Returns the moles of this MonoFluid.
        double getMole() const;
        /// @brief Sets the temperature of this MonoFluid.
        void setTemperature(const double temperature);
        /// @brief Sets the pressure of this MonoFluid.
        void setPressure(const double pressure);
        /// @brief Sets the mass flow rate of this MonoFluid.
        void setFlowRate(const double flowRate);
        /// @brief Returns a const pointer to the properties of this MonoFluid.
        const FluidProperties* getProperties() const;
        /// @brief Returns the type of this MonoFluid.
        FluidProperties::FluidType getType() const;
        /// @brief Returns the phase of this MonoFluid.
        FluidProperties::FluidPhase getPhase() const;
        /// @brief Returns the molecular weight of this MonoFluid.
        double getMWeight() const;
        /// @brief Returns the density of this MonoFluid.
        double getDensity() const;
        /// @brief Returns the viscosity of this MonoFluid.
        double getViscosity() const;
        /// @brief Returns the specific heat of this MonoFluid.
        double getSpecificHeat() const;
        /// @brief Returns the specific enthalpy of this MonoFluid.
        double getSpecificEnthalpy() const;
        /// @brief Returns the thermal conductivity (W/m/K) of this MonoFluid.
        double getThermalConductivity() const;
        /// @brief Returns the Prandtl number -- of this MonoFluid.
        double getPrandtlNumber() const;
        /// @brief Returns the adiabatic index (gamma) of this MonoFluid.
        double getAdiabaticIndex() const;
        /// @brief Sets the mass and moles of this MonoFluid.
        void setMass(const double mass);
        /// @brief Sets the moles and mass of this MonoFluid.
        void setMole(const double mole);
        /// @brief Resets (zeros) the state of this MonoFluid.
        void resetState();
        /// @brief Updates the state of this MonoFluid. by copying from the specified MonoFluid.
        void setState(const MonoFluid* that);
        /// @brief Updates the state of this MonoFluid by adding in the specified  MonoFluid at the
        /// (optional) specified mass flow rate.
        void addState(const MonoFluid* that, const double flowRate = 0.0);
        /// @brief Returns the temperature at the specified specific enthalpy.
        double computeTemperature(const double specificEnthalpy) const;
        /// @brief Returns the specific enthalpy at the specified temperature and pressure.
        double computeSpecificEnthalpy(const double temperature,
                                       const double pressure = 0.0) const;
        /// @brief Returns the pressure at the specified temperature and density.
        double computePressure(const double temperature, const double density) const;
    protected:
        friend class PolyFluid;
        double                 mTemperature; /**< (K)                          Temperature of the fluid */
        double                 mPressure;    /**< (kPa)                        Pressure of the fluid */
        double                 mFlowRate;    /**< (kg/s)                       Mass flow rate of the fluid */
        double                 mMass;        /**< (kg)                         Mass of the fluid */
        double                 mMole;        /**< (kg*mol)                     Moles of the fluid */
        const FluidProperties* mProperties;  /**< (--)     trick_chkpnt_io(**) Pointer to the properties of the fluid */
        static const double    mMinFlowRate; /**< (kg/s)                       Minimum flow rate for source override */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details
        /// Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        MonoFluid& operator =(const MonoFluid&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Temperature (K) of this MonoFluid
///
/// @details  Returns the current temperature of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getTemperature() const
{
    return mTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Pressure (kPa) of this MonoFluid
///
/// @details  Returns the current pressure of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getPressure() const
{
    return mPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Mass Flow Rate (kg/m3) of this MonoFlui
///
/// @details  Returns the current mass flow rate (kg/s) of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getFlowRate() const
{
    return mFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Mass (kg) of this MonoFluid
///
/// @details  Returns the current mass of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getMass() const
{
    return mMass;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Moles (kg*mol) of this MonoFluid
///
/// @details  Returns the current moles of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getMole() const
{
    return mMole;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Pointer to the FluidProperties of this MonoFluid
///
/// @details  Returns a const pointer to the properties of this MonoFluid, null if not initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const FluidProperties* MonoFluid::getProperties() const
{
    return mProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- The type of this MonoFluid.
///
/// @details  Returns the type of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline FluidProperties::FluidType MonoFluid::getType() const
{
    return mProperties->getType();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   -- The phase (gas, liquid, solid) of this MonoFluid.
///
/// @details  Returns the phase of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline FluidProperties::FluidPhase MonoFluid::getPhase() const
{
    return mProperties->getPhase();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (1/mol)  The molecular weight of this MonoFluid
///
/// @details  Returns the molecular weight of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getMWeight() const
{
    return mProperties->getMWeight();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (kg/m3)  The density of this MonoFluid
///
/// @details  Returns the density of this MonoFluid at the current temperature and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getDensity() const
{
    return mProperties->getDensity(mTemperature, mPressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (Pa*s)  The dynamic viscosity of this MonoFluid
///
/// @details  Returns the dynamic viscosity of this MonoFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getViscosity() const
{
    return mProperties->getViscosity(mTemperature, mPressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (J/kg/K)  The specific heat of this MonoFluid
///
/// @details  Returns the specific heat (J/kg/K) of this MonoFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getSpecificHeat() const
{
    return mProperties->getSpecificHeat(mTemperature, mPressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (J/kg)  The specific enthalpy of this MonoFluid
///
/// @details  Returns the specific enthalpy of this MonoFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getSpecificEnthalpy() const
{
    return mProperties->getSpecificEnthalpy(mTemperature, mPressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (W/m/K)  The thermal conductivity of this MonoFluid
///
/// @details  Returns the thermal conductivity of this MonoFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getThermalConductivity() const
{
    return mProperties->getThermalConductivity(mTemperature, mPressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   --  The Prandtl number of this MonoFluid
///
/// @details  Returns the Prandtl number of this MonoFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getPrandtlNumber() const
{
    return mProperties->getPrandtlNumber(mTemperature, mPressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   --  The adiabatic index of this MonoFluid
///
/// @details  Returns the adiabatic index (gamma) of this MonoFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::getAdiabaticIndex() const
{
    return mProperties->getAdiabaticIndex(mTemperature, mPressure);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   temperature  (K) Temperature of this MonoFluid
///
/// @return   void
///
/// @details  Sets the temperature of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MonoFluid::setTemperature(const double temperature)
{
    mTemperature = temperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   pressure  (kPa)  Pressure of this MonoFluid
///
/// @return   void
///
/// @details  Sets the pressure of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MonoFluid::setPressure(const double pressure)
{
    mPressure = pressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   flowRate  (kg/s)  Mass flow rate of this MonoFluid
///
/// @return   void
///
/// @details  Sets the mass flow rate of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MonoFluid::setFlowRate(const double flowRate)
{
    mFlowRate = flowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   mass  (kg)   Mass of this MonoFluid
///
/// @return   void
///
/// @details  Sets the mass and updates the moles of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MonoFluid::setMass(const double mass)
{
    mMass = mass;
    mMole = mass / mProperties->getMWeight();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   mole  (kg*mol)  Moles of this MonoFluid
///
/// @return   void
///
/// @details  Sets the moles and updates the mass of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MonoFluid::setMole(const double mole)
{
    mMole = mole;
    mMass = mole * mProperties->getMWeight();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   void
///
/// @details  Resets (zeros) the state of this MonoFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MonoFluid::resetState()
{
    mTemperature = 0.0;
    mPressure    = 0.0;
    mFlowRate    = 0.0;
    mMass        = 0.0;
    mMole        = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   src         --     Pointer to the MonoFluid to set the state from
///
/// @return   void
///
/// @details  Updates the state of this MonoFluid from the specified MonoFluid state.
///           The mass and moles are not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MonoFluid::setState(const MonoFluid* src)
{
    /// - Copy the temperature, pressure and mass flow rate from source MonoFluid to this MonoFluid.
    mTemperature = src->mTemperature;
    mPressure    = src->mPressure;
    mFlowRate    = src->mFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   specificEnthalpy   (J/kg)     Specific enthalpy to compute temperature at
///
/// @return   (K)  The temperature of this MonoFluid at the specified specific enthalpy
///
/// @details  Returns the temperature of this MonoFluid at the specified specific enthalpy.
///           The state of this MonoFluid is not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::computeTemperature(const double specificEnthalpy) const
{
    return mProperties->getTemperature(specificEnthalpy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   temperature   (K)    Temperature to compute specific enthalpy at
/// @param[in]   pressure      (kPa)  Pressure to compute specific enthalpy at
///
/// @return   (J/kg) The specific enthalpy this MonoFluid at the specified temperature and pressure.
///
/// @details  Returns the specific enthalpy for this type of MonoFluid at the specified temperature
///           and pressure. The state of this MonoFluid is not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::computeSpecificEnthalpy(const double temperature,
                                                 const double pressure) const
{
    return mProperties->getSpecificEnthalpy(temperature, pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   temperature   (K)     Temperature to compute pressure at
/// @param[in]   density       (kg/m3) Density to compute pressure at
///
/// @return   (kPa) The pressure of this MonoFluid at the specified temperature and density.
///
/// @details  Returns the pressure for this type of MonoFluid at the specified temperature and
///           density. The state of this MonoFluid is not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::computePressure(const double temperature, const double density) const
{
    return mProperties->getPressure(temperature, density);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   temperature   (K)    Temperature to compute density at
/// @param[in]   pressure      (kPa)  Pressure to compute density at
///
/// @return   (kg/m3)  The density of this MonoFluid at the specified temperature and pressure.
///
/// @details  Returns the density for this type of MonoFluid at the specified temperature and
///           pressure. The state of this MonoFluid is not updated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double MonoFluid::computeDensity(const double temperature,  const double pressure) const
{
    return mProperties->getDensity(temperature, pressure);
}

#endif
