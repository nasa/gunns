#ifndef FluidProperties_EXISTS
#define FluidProperties_EXISTS
/*
@defgroup  TSM_UTILITIES_PROPERTIES_FLUID  Fluid Properties
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling the properties of a single species of liquid or gas.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- (The constructors are protected, so only those Fluid Properties objects in its friend,
   the Predefined Fluid Properties class are available.)

 CLASS:
- ()

 LIBRARY DEPENDENCY:
- ((FluidProperties.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-05))

@{
*/

#include <cfloat>

#include "math/approximation/TsApproximation.hh"
#include "math/approximation/QuinticFit.hh"
#include "math/approximation/QuarticFit.hh"
#include "math/approximation/CubicFit.hh"
#include "math/approximation/LinearFit.hh"
#include "math/approximation/PowerFit.hh"
#include "math/approximation/ProductFit.hh"
#include "math/approximation/QuadLinFit.hh"
#include "math/approximation/QuadLinInvFit.hh"
#include "math/approximation/QuadraticFit.hh"
#include "math/approximation/QuadraticRootFit.hh"
#include "math/approximation/QuotientFit.hh"
#include "math/approximation/TsBilinearInterpolatorReverse.hh"
#include "properties/FluidTsatFit.hh"
#include "properties/FluidHvapFit.hh"
#include "FluidPropertiesDataWaterPvt.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Class for Fluid Properties.
///
/// @details  Provides the class for modeling the properties of single species of liquid or gas:
///           type, molecular weight, density as a function of temperature and pressure, dynamic
///           viscosity as a function of temperature and pressure, specific heat as a function of
///           temperature and pressure, specific enthalpy as a function of temperature and pressure,
///           pressure as a function of temperature and density, and temperature as a function of
///           specific enthalpy and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FluidProperties {
    TS_MAKE_SIM_COMPATIBLE(FluidProperties);
    public:
        friend class DefinedFluidProperties;
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Enumeration of the types of Fluids.
        ///
        /// @note     The last value is used to both define the length of the array and provide an
        ///           undefined Fluid type.
        ///
        /// @note     By convention, the enumeration begins with chemical composition for gases and
        ///           ends with name for liquids.
        ///
        /// @details  This enumeration is used to index the array of Fluid Properties.
        ////////////////////////////////////////////////////////////////////////////////////////////
        enum FluidType {
            GUNNS_CO          =  0,  ///<  carbon monoxide (ideal gas)
            GUNNS_CO2         =  1,  ///<  carbon dioxide (ideal gas)
            GUNNS_H2O         =  2,  ///<  water (ideal gas)
            GUNNS_N2          =  3,  ///<  nitrogen (ideal gas)
            GUNNS_O2          =  4,  ///<  oxygen (ideal gas)
            GUNNS_NH3         =  5,  ///<  ammonia (ideal gas)
            GUNNS_H2          =  6,  ///<  hydrogen (ideal gas)
            GUNNS_CH4         =  7,  ///<  methane (ideal gas)
            GUNNS_HCL         =  8,  ///<  hydrogen chloride (ideal gas)
            GUNNS_HCN         =  9,  ///<  hydrogen cyanide (ideal gas)
            GUNNS_HE          = 10,  ///<  helium (ideal gas)
            GUNNS_HE_REAL_GAS = 11,  ///<  helium (real-gas compressibility)
            GUNNS_XE_REAL_GAS = 12,  ///<  xenon (monatomic gas) (real-gas compressibility)
            GUNNS_N2_REAL_GAS = 13,  ///<  nitrogen (real-gas compressibility)
            GUNNS_O2_REAL_GAS = 14,  ///<  oxygen (real-gas compressibility)
            GUNNS_H2_REAL_GAS = 15,  ///<  hydrogen (real-gas compressibility)
            GUNNS_WATER       = 16,  ///<  water (liquid)
            GUNNS_HFE7000     = 17,  ///<  HFE-7000 (liquid)
            GUNNS_HFE7100     = 18,  ///<  HFE-7100 (liquid)
            GUNNS_PG30        = 19,  ///<  propylene glycol 30% (liquid)
            GUNNS_PG40        = 20,  ///<  propylene glycol 40% (liquid)
            GUNNS_PG50        = 21,  ///<  propylene glycol 50% (liquid)
            GUNNS_AMMONIA     = 22,  ///<  ammonia (liquid)
            GUNNS_OXYGEN      = 23,  ///<  oxygen (cryogenic liquid)
            GUNNS_HYDROGEN    = 24,  ///<  hydrogen (cryogenic liquid)
            GUNNS_METHANE     = 25,  ///<  methane (cryogenic liquid)
            GUNNS_NAK78       = 26,  ///<  eutectic potassium 78% sodium 22& (liquid metal)
            GUNNS_GALDEN170   = 27,  ///<  Galden HT-170 (liquid)
            GUNNS_WATER_PVT   = 28,  ///<  water (liquid with table lookup density)
            GUNNS_NTO         = 29,  ///<  nitrogen tetroxide (liquid)
            GUNNS_MMH         = 30,  ///<  monomethylhydrazine (liquid)
            GUNNS_HYDRAZINE   = 31,  ///<  anhydrous hydrazine (liquid)
            NO_FLUID          = 32   ///<  Invalid or number of fluids - Keep this last!
        };
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Enumeration of the phases of Fluids.
        ///
        /// @note     The last value is used to both define the number of phases and provide an
        ///           undefined Phase type.
        ///
        /// @details  This enumeration is used to specify the phase (gas or liquid) that the
        ///           fluid properties are valid for.
        ////////////////////////////////////////////////////////////////////////////////////////////
        enum FluidPhase {
            GAS      = 0,  ///< Gas phase
            LIQUID   = 1,  ///< Liquid phase
            NO_PHASE = 2   ///< Invalid or number of phases - Keep this last!
        };
        /// @brief Default constructs a Fluid Properties.
        FluidProperties(const FluidProperties::FluidType&  type                  = FluidProperties::NO_FLUID,
                        const FluidProperties::FluidPhase& phase                 = FluidProperties::NO_PHASE,
                        const double                       mWeight               = 0.0,
                        const double                       criticalTemperature   = 0.0,
                        TsApproximation*                   density               = 0,
                        TsApproximation*                   viscosity             = 0,
                        TsApproximation*                   specificHeat          = 0,
                        TsApproximation*                   thermalConductivity   = 0,
                        TsApproximation*                   prandtlNumber         = 0,
                        TsApproximation*                   adiabaticIndex        = 0,
                        TsApproximation*                   pressure              = 0,
                        TsApproximation*                   temperature           = 0,
                        TsApproximation*                   saturationPressure    = 0,
                        TsApproximation*                   saturationTemperature = 0,
                        TsApproximation*                   heatOfVaporization    = 0);
        /// @brief Default destructs a Fluid Properties.
        virtual ~FluidProperties();
        /// @brief Returns the type of this Fluid.
        FluidProperties::FluidType getType() const;
        /// @brief Returns the phase of this Fluid.
        FluidProperties::FluidPhase getPhase() const;
        /// @brief Returns the molecular weight (1/mol) of this Fluid.
        double getMWeight() const;
        /// @brief Returns the critical temperature (K) of this Fluid.
        double getCriticalTemperature() const;
        /// @brief Returns the density (kg/m3) of this Fluid.
        double getDensity(const double temperature, const double pressure = 0) const;
        /// @brief Returns the dynamic viscosity (Pa*s) of this Fluid.
        double getViscosity(const double temperature, const double pressure = 0) const;
        /// @brief Returns the specific heat (J/kg/K) of this Fluid.
        double getSpecificHeat(const double temperature, const double pressure = 0) const;
        /// @brief Returns the specific enthalpy (J/kg) of this Fluid.
        double getSpecificEnthalpy(const double temperature, const double pressure=0) const;
        /// @brief Returns the thermal conductivity (W/m/K) of this Fluid.
        double getThermalConductivity(const double temperature, const double pressure=0) const;
        /// @brief Returns the Prandtl number -- of this Fluid.
        double getPrandtlNumber(const double temperature, const double pressure=0) const;
        /// @brief Returns the adiabatic index (gamma) of this Fluid.
        double getAdiabaticIndex(const double temperature, const double pressure=0) const;
        /// @brief Returns the pressure (kPa) of this Fluid.
        double getPressure(const double temperature, const double density=0) const;
        /// @brief Returns the temperature (K) of this Fluid.
        double getTemperature(const double specificEnthalpy, const double pressure=0) const;
        /// @brief Returns the saturation pressure (kPa) of this Fluid.
        double getSaturationPressure(const double temperature) const;
        /// @brief Returns the saturation temperature (K) of this Fluid.
        double getSaturationTemperature(const double pressure) const;
        /// @brief Returns the latent heat of vaporization (kJ/kg) of this Fluid.
        double getHeatOfVaporization(const double temperature) const;
    protected:
        const FluidProperties::FluidType  mType;        /**< (--)    Type of this Fluid */
        const FluidProperties::FluidPhase mPhase;       /**< (--)    Phase of this Fluid */
        const double            mMWeight;               /**< (1/mol) Molecular weight of this Fluid */
        const double            mCriticalTemperature;   /**< (K)     Critical temperature of this Fluid */
        TsApproximation*        mDensity;               /**< (--)    Curve fit for density of this Fluid */
        TsApproximation*        mViscosity;             /**< (--)    Curve fit for dynamic viscosity of this Fluid */
    public:
        /// @details mSpecificHeat declared public to allow access to the coefficients by PolyFluid.
        TsApproximation*        mSpecificHeat;          /**< (--)    Curve fit for specific heat of this Fluid */
    protected:
        TsApproximation*        mThermalConductivity;   /**< (--)    Curve fit for thermal conductivity of this Fluid */
        TsApproximation*        mPrandtlNumber;         /**< (--)    Curve fit for Prandtl number of this Fluid */
        TsApproximation*        mAdiabaticIndex;        /**< (--)    Curve fit for adiabatic index of this Fluid */
        TsApproximation*        mPressure;              /**< (--)    Curve fit for pressure of this Fluid */
        TsApproximation*        mTemperature;           /**< (--)    Curve fit for temperature of this Fluid */
        TsApproximation*        mSaturationPressure;    /**< (--)    Curve fit for saturation pressure of this Fluid */
        TsApproximation*        mSaturationTemperature; /**< (--)    Curve fit for saturation temperature of this Fluid */
        TsApproximation*        mHeatOfVaporization;    /**< (--)    Curve fit for heat of vaporization of this Fluid */
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        FluidProperties(const FluidProperties&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        FluidProperties& operator =(const FluidProperties&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Class for the Defined Fluid Properties.
///
/// @details  Provides the Fluid Properties for a set of liquids and gases.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DefinedFluidProperties {
    TS_MAKE_SIM_COMPATIBLE(DefinedFluidProperties);
    public:
        /// @brief Default constructs a Fluid Properties.
        DefinedFluidProperties();
        /// @brief Default destructs a Fluid Properties.
        virtual ~DefinedFluidProperties();
        /// @brief returns a pointer to the properties of the specified fluid type.
        FluidProperties* getProperties(const FluidProperties::FluidType& type) const;
    protected:
        const double      mGasK;                           /**< (J/mol/K) Universal gas constant */
        QuotientFit       mDensityCO;                      /**< (--) CO density curve fit */
        LinearFit         mViscosityCO;                    /**< (--) CO dynamic viscosity curve fit */
        LinearFit         mSpecificHeatCO;                 /**< (--) CO specific heat curve fit */
        LinearFit         mThermalConductivityCO;          /**< (--) CO thermal conductivity curve fit */
        LinearFit         mPrandtlNumberCO;                /**< (--) CO prandtl number curve fit */
        LinearFit         mAdiabaticIndexCO;               /**< (--) CO adiabatic index curve fit */
        ProductFit        mPressureCO;                     /**< (--) CO pressure curve fit */
        QuadraticRootFit  mTemperatureCO;                  /**< (--) CO temperature curve fit */
        QuadraticFit      mSaturationPressureCO;           /**< (--) CO saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureCO;        /**< (--) CO saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationCO;           /**< (--) CO heat of vaporization curve fit */
        QuotientFit       mDensityCO2;                     /**< (--) CO2 density curve fit */
        LinearFit         mViscosityCO2;                   /**< (--) CO2 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatCO2;                /**< (--) CO2 specific heat curve fit */
        LinearFit         mThermalConductivityCO2;         /**< (--) CO2 thermal conductivity curve fit */
        LinearFit         mPrandtlNumberCO2;               /**< (--) CO2 prandtl number curve fit */
        LinearFit         mAdiabaticIndexCO2;              /**< (--) CO2 adiabatic index curve fit */
        ProductFit        mPressureCO2;                    /**< (--) CO2 pressure curve fit */
        QuadraticRootFit  mTemperatureCO2;                 /**< (--) CO2 temperature curve fit */
        QuadraticFit      mSaturationPressureCO2;          /**< (--) CO2 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureCO2;       /**< (--) CO2 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationCO2;          /**< (--) CO2 heat of vaporization curve fit */
        QuotientFit       mDensityH2O;                     /**< (--) H2O density curve fit */
        LinearFit         mViscosityH2O;                   /**< (--) H2O dynamic viscosity curve fit */
        LinearFit         mSpecificHeatH2O;                /**< (--) H2O specific heat curve fit */
        LinearFit         mThermalConductivityH2O;         /**< (--) H2O thermal conductivity curve fit */
        LinearFit         mPrandtlNumberH2O;               /**< (--) H2O prandtl number curve fit */
        LinearFit         mAdiabaticIndexH2O;              /**< (--) H2O adiabatic index curve fit */
        ProductFit        mPressureH2O;                    /**< (--) H2O pressure curve fit */
        QuadraticRootFit  mTemperatureH2O;                 /**< (--) H2O temperature curve fit */
        QuadraticFit      mSaturationPressureH2O;          /**< (--) H2O saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureH2O;       /**< (--) H2O saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationH2O;          /**< (--) H2O heat of vaporization curve fit */
        QuotientFit       mDensityN2;                      /**< (--) N2 density curve fit */
        LinearFit         mViscosityN2;                    /**< (--) N2 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatN2;                 /**< (--) N2 specific heat curve fit */
        LinearFit         mThermalConductivityN2;          /**< (--) N2 thermal conductivity curve fit */
        LinearFit         mPrandtlNumberN2;                /**< (--) N2 prandtl number curve fit */
        LinearFit         mAdiabaticIndexN2;               /**< (--) N2 adiabatic index curve fit */
        ProductFit        mPressureN2;                     /**< (--) N2 pressure curve fit */
        QuadraticRootFit  mTemperatureN2;                  /**< (--) N2 temperature curve fit */
        QuadraticFit      mSaturationPressureN2;           /**< (--) N2 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureN2;        /**< (--) N2 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationN2;           /**< (--) N2 heat of vaporization curve fit */
        QuotientFit       mDensityO2;                      /**< (--) O2 density curve fit */
        LinearFit         mViscosityO2;                    /**< (--) O2 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatO2;                 /**< (--) O2 specific heat curve fit */
        LinearFit         mThermalConductivityO2;          /**< (--) O2 thermal conductivity curve fit */
        LinearFit         mPrandtlNumberO2;                /**< (--) O2 prandtl number curve fit */
        LinearFit         mAdiabaticIndexO2;               /**< (--) O2 adiabatic index curve fit */
        ProductFit        mPressureO2;                     /**< (--) O2 pressure curve fit */
        QuadraticRootFit  mTemperatureO2;                  /**< (--) O2 temperature curve fit */
        QuadraticFit      mSaturationPressureO2;           /**< (--) O2 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureO2;        /**< (--) O2 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationO2;           /**< (--) O2 heat of vaporization curve fit */
        QuotientFit       mDensityNH3;                     /**< (--) NH3 density curve fit */
        LinearFit         mViscosityNH3;                   /**< (--) NH3 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatNH3;                /**< (--) NH3 specific heat curve fit */
        LinearFit         mThermalConductivityNH3;         /**< (--) NH3 thermal conductivity curve fit */
        LinearFit         mPrandtlNumberNH3;               /**< (--) NH3 prandtl number curve fit */
        LinearFit         mAdiabaticIndexNH3;              /**< (--) NH3 adiabatic index curve fit */
        ProductFit        mPressureNH3;                    /**< (--) NH3 pressure curve fit */
        QuadraticRootFit  mTemperatureNH3;                 /**< (--) NH3 temperature curve fit */
        QuadraticFit      mSaturationPressureNH3;          /**< (--) NH3 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureNH3;       /**< (--) NH3 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationNH3;          /**< (--) NH3 heat of vaporization curve fit */
        QuotientFit       mDensityH2;                      /**< (--) H2 density curve fit */
        LinearFit         mViscosityH2;                    /**< (--) H2 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatH2;                 /**< (--) H2 specific heat curve fit */
        LinearFit         mThermalConductivityH2;          /**< (--) H2 thermal conductivity curve fit */
        LinearFit         mPrandtlNumberH2;                /**< (--) H2 prandtl number curve fit */
        LinearFit         mAdiabaticIndexH2;               /**< (--) H2 adiabatic index curve fit */
        ProductFit        mPressureH2;                     /**< (--) H2 pressure curve fit */
        QuadraticRootFit  mTemperatureH2;                  /**< (--) H2 temperature curve fit */
        QuadraticFit      mSaturationPressureH2;           /**< (--) H2 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureH2;        /**< (--) H2 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationH2;           /**< (--) H2 heat of vaporization curve fit */
        QuotientFit       mDensityCH4;                     /**< (--) CH4 density curve fit */
        LinearFit         mViscosityCH4;                   /**< (--) CH4 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatCH4;                /**< (--) CH4 specific heat curve fit */
        LinearFit         mThermalConductivityCH4;         /**< (--) CH4 thermal conductivity curve fit */
        LinearFit         mPrandtlNumberCH4;               /**< (--) CH4 prandtl number curve fit */
        LinearFit         mAdiabaticIndexCH4;              /**< (--) CH4 adiabatic index curve fit */
        ProductFit        mPressureCH4;                    /**< (--) CH4 pressure curve fit */
        QuadraticRootFit  mTemperatureCH4;                 /**< (--) CH4 temperature curve fit */
        QuadraticFit      mSaturationPressureCH4;          /**< (--) CH4 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureCH4;       /**< (--) CH4 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationCH4;          /**< (--) CH4 heat of vaporization curve fit */
        QuotientFit       mDensityHCL;                     /**< (--) HCL density curve fit */
        LinearFit         mViscosityHCL;                   /**< (--) HCL dynamic viscosity curve fit */
        LinearFit         mSpecificHeatHCL;                /**< (--) HCL specific heat curve fit */
        LinearFit         mThermalConductivityHCL;         /**< (--) HCL thermal conductivity curve fit */
        LinearFit         mPrandtlNumberHCL;               /**< (--) HCL prandtl number curve fit */
        LinearFit         mAdiabaticIndexHCL;              /**< (--) HCL adiabatic index curve fit */
        ProductFit        mPressureHCL;                    /**< (--) HCL pressure curve fit */
        QuadraticRootFit  mTemperatureHCL;                 /**< (--) HCL temperature curve fit */
        QuadraticFit      mSaturationPressureHCL;          /**< (--) HCL saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureHCL;       /**< (--) HCL saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationHCL;          /**< (--) HCL heat of vaporization curve fit */
        QuotientFit       mDensityHCN;                     /**< (--) HCN density curve fit */
        LinearFit         mViscosityHCN;                   /**< (--) HCN dynamic viscosity curve fit */
        LinearFit         mSpecificHeatHCN;                /**< (--) HCN specific heat curve fit */
        LinearFit         mThermalConductivityHCN;         /**< (--) HCN thermal conductivity curve fit */
        LinearFit         mPrandtlNumberHCN;               /**< (--) HCN prandtl number curve fit */
        LinearFit         mAdiabaticIndexHCN;              /**< (--) HCN adiabatic index curve fit */
        ProductFit        mPressureHCN;                    /**< (--) HCN pressure curve fit */
        QuadraticRootFit  mTemperatureHCN;                 /**< (--) HCN temperature curve fit */
        QuadraticFit      mSaturationPressureHCN;          /**< (--) HCN saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureHCN;       /**< (--) HCN saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationHCN;          /**< (--) HCN heat of vaporization curve fit */
        QuotientFit       mDensityHe;                      /**< (--) He density curve fit */
        CubicFit          mViscosityHe;                    /**< (--) He dynamic viscosity curve fit */
        LinearFit         mSpecificHeatHe;                 /**< (--) He specific heat curve fit */
        CubicFit          mThermalConductivityHe;          /**< (--) He thermal conductivity curve fit */
        CubicFit          mPrandtlNumberHe;                /**< (--) He prandtl number curve fit */
        LinearFit         mAdiabaticIndexHe;               /**< (--) He adiabatic index curve fit */
        ProductFit        mPressureHe;                     /**< (--) He pressure curve fit */
        QuadraticRootFit  mTemperatureHe;                  /**< (--) He temperature curve fit */
        QuadraticFit      mSaturationPressureHe;           /**< (--) He saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureHe;        /**< (--) He saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationHe;           /**< (--) He heat of vaporization curve fit */
        TsBilinearInterpolator mDensityHeReal;             /**< (--) He real-gas density table */
        TsBilinearInterpolatorReverse mPressureHeReal;     /**< (--) He real-gas pressure table */
        TsBilinearInterpolator mDensityXe;                 /**< (--) Xe density table */
        LinearFit         mViscosityXe;                    /**< (--) Xe dynamic viscosity curve fit */
        LinearFit         mSpecificHeatXe;                 /**< (--) Xe specific heat curve fit */
        LinearFit         mThermalConductivityXe;          /**< (--) Xe thermal conductivity curve fit */
        LinearFit         mPrandtlNumberXe;                /**< (--) Xe prandtl number curve fit */
        LinearFit         mAdiabaticIndexXe;               /**< (--) Xe adiabatic index curve fit */
        TsBilinearInterpolatorReverse mPressureXe;         /**< (--) Xe pressure table */
        QuadraticRootFit  mTemperatureXe;                  /**< (--) Xe temperature curve fit */
        QuadraticFit      mSaturationPressureXe;           /**< (--) Xe saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureXe;        /**< (--) Xe saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationXe;           /**< (--) Xe heat of vaporization curve fit */
        TsBilinearInterpolator mDensityN2Real;             /**< (--) N2 real-gas density table */
        TsBilinearInterpolatorReverse mPressureN2Real;     /**< (--) N2 real-gas pressure table */
        TsBilinearInterpolator mDensityO2Real;             /**< (--) O2 real-gas density table */
        TsBilinearInterpolatorReverse mPressureO2Real;     /**< (--) O2 real-gas pressure table */
        TsBilinearInterpolator mDensityH2Real;             /**< (--) H2 real-gas density table */
        TsBilinearInterpolatorReverse mPressureH2Real;     /**< (--) H2 real-gas pressure table */
        QuadraticFit      mSaturationPressureN2O4;         /**< (--) N2O4 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureN2O4;      /**< (--) N2O4 saturation temperature curve fit */
        LinearFit         mHeatOfVaporizationN2O4;         /**< (--) N2O4 heat of vaporization curve fit */
        QuadraticFit      mSaturationPressureN2H4;         /**< (--) N2H4 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureN2H4;      /**< (--) N2H4 saturation temperature curve fit */
        LinearFit         mHeatOfVaporizationN2H4;         /**< (--) N2H4 heat of vaporization curve fit */
        QuadraticFit      mSaturationPressureCH3N2H3;      /**< (--) CH3N2H3 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureCH3N2H3;   /**< (--) CH3N2H3 saturation temperature curve fit */
        LinearFit         mHeatOfVaporizationCH3N2H3;      /**< (--) CH3N2H3 heat of vaporization curve fit */
        QuadLinFit        mDensityWATER;                   /**< (--) WATER density curve fit */
        QuinticFit        mViscosityWATER;                 /**< (--) WATER dynamic viscosity curve fit */
        LinearFit         mSpecificHeatWATER;              /**< (--) WATER specific heat curve fit */
        LinearFit         mThermalConductivityWATER;       /**< (--) WATER thermal conductivity curve fit */
        QuinticFit        mPrandtlNumberWATER;             /**< (--) WATER prandtl number curve fit */
        LinearFit         mAdiabaticIndexWATER;            /**< (--) WATER adiabatic index curve fit */
        QuadLinInvFit     mPressureWATER;                  /**< (--) WATER pressure curve fit */
        QuadraticRootFit  mTemperatureWATER;               /**< (--) WATER temperature curve fit */
        QuadLinFit        mDensityHFE7000;                 /**< (--) HFE7000 density curve fit */
        CubicFit          mViscosityHFE7000;               /**< (--) HFE7000 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatHFE7000;            /**< (--) HFE7000 specific heat curve fit */
        LinearFit         mThermalConductivityHFE7000;     /**< (--) HFE7000 thermal conductivity curve fit */
        LinearFit         mPrandtlNumberHFE7000;           /**< (--) HFE7000 prandtl number curve fit */
        LinearFit         mAdiabaticIndexHFE7000;          /**< (--) HFE7000 adiabatic index curve fit */
        QuadLinInvFit     mPressureHFE7000;                /**< (--) HFE7000 pressure curve fit */
        QuadraticRootFit  mTemperatureHFE7000;             /**< (--) HFE7000 temperature curve fit */
        QuadraticFit      mSaturationPressureHFE7000;      /**< (--) HFE7000 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureHFE7000;   /**< (--) HFE7000 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationHFE7000;      /**< (--) HFE7000 heat of vaporization curve fit */
        QuadLinFit        mDensityHFE7100;                 /**< (--) HFE7100 density curve fit */
        CubicFit          mViscosityHFE7100;               /**< (--) HFE7100 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatHFE7100;            /**< (--) HFE7100 specific heat curve fit */
        LinearFit         mThermalConductivityHFE7100;     /**< (--) HFE7100 thermal conductivity curve fit */
        CubicFit          mPrandtlNumberHFE7100;           /**< (--) HFE7100 prandtl number curve fit */
        LinearFit         mAdiabaticIndexHFE7100;          /**< (--) HFE7100 adiabatic index curve fit */
        QuadLinInvFit     mPressureHFE7100;                /**< (--) HFE7100 pressure curve fit */
        QuadraticRootFit  mTemperatureHFE7100;             /**< (--) HFE7100 temperature curve fit */
        QuadraticFit      mSaturationPressureHFE7100;      /**< (--) HFE7100 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureHFE7100;   /**< (--) HFE7100 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationHFE7100;      /**< (--) HFE7100 heat of vaporization curve fit */
        QuadLinFit        mDensityPG30;                    /**< (--) PG30 density curve fit */
        PowerFit          mViscosityPG30;                  /**< (--) PG30 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatPG30;               /**< (--) PG30 specific heat curve fit */
        QuadraticFit      mThermalConductivityPG30;        /**< (--) PG30 thermal conductivity curve fit */
        PowerFit          mPrandtlNumberPG30;              /**< (--) PG30 prandtl number curve fit */
        LinearFit         mAdiabaticIndexPG30;             /**< (--) PG30 adiabatic index curve fit */
        QuadLinInvFit     mPressurePG30;                   /**< (--) PG30 pressure curve fit */
        QuadraticRootFit  mTemperaturePG30;                /**< (--) PG30 temperature curve fit */
        QuadraticFit      mSaturationPressurePG30;         /**< (--) PG30 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperaturePG30;      /**< (--) PG30 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationPG30;         /**< (--) PG30 heat of vaporization curve fit */
        QuadLinFit        mDensityPG40;                    /**< (--) PG40 density curve fit */
        PowerFit          mViscosityPG40;                  /**< (--) PG40 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatPG40;               /**< (--) PG40 specific heat curve fit */
        QuadraticFit      mThermalConductivityPG40;        /**< (--) PG40 thermal conductivity curve fit */
        PowerFit          mPrandtlNumberPG40;              /**< (--) PG40 prandtl number curve fit */
        LinearFit         mAdiabaticIndexPG40;             /**< (--) PG40 adiabatic index curve fit */
        QuadLinInvFit     mPressurePG40;                   /**< (--) PG40 pressure curve fit */
        QuadraticRootFit  mTemperaturePG40;                /**< (--) PG40 temperature curve fit */
        QuadraticFit      mSaturationPressurePG40;         /**< (--) PG40 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperaturePG40;      /**< (--) PG40 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationPG40;         /**< (--) PG40 heat of vaporization curve fit */
        QuadLinFit        mDensityPG50;                    /**< (--) PG50 density curve fit */
        PowerFit          mViscosityPG50;                  /**< (--) PG50 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatPG50;               /**< (--) PG50 specific heat curve fit */
        QuadraticFit      mThermalConductivityPG50;        /**< (--) PG50 thermal conductivity curve fit */
        PowerFit          mPrandtlNumberPG50;              /**< (--) PG50 prandtl number curve fit */
        LinearFit         mAdiabaticIndexPG50;             /**< (--) PG50 adiabatic index curve fit */
        QuadLinInvFit     mPressurePG50;                   /**< (--) PG50 pressure curve fit */
        QuadraticRootFit  mTemperaturePG50;                /**< (--) PG50 temperature curve fit */
        QuadraticFit      mSaturationPressurePG50;         /**< (--) PG50 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperaturePG50;      /**< (--) PG50 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationPG50;         /**< (--) PG50 heat of vaporization curve fit */
        QuadLinFit        mDensityAMMONIA;                 /**< (--) AMMONIA density curve fit */
        CubicFit          mViscosityAMMONIA;               /**< (--) AMMONIA dynamic viscosity curve fit */
        LinearFit         mSpecificHeatAMMONIA;            /**< (--) AMMONIA specific heat curve fit */
        LinearFit         mThermalConductivityAMMONIA;     /**< (--) AMMONIA thermal conductivity curve fit */
        CubicFit          mPrandtlNumberAMMONIA;           /**< (--) AMMONIA prandtl number curve fit */
        LinearFit         mAdiabaticIndexAMMONIA;          /**< (--) AMMONIA adiabatic index curve fit */
        QuadLinInvFit     mPressureAMMONIA;                /**< (--) AMMONIA pressure curve fit */
        QuadraticRootFit  mTemperatureAMMONIA;             /**< (--) AMMONIA temperature curve fit */
        QuadLinFit        mDensityOXYGEN;                  /**< (--) OXYGEN density curve fit */
        CubicFit          mViscosityOXYGEN;                /**< (--) OXYGEN dynamic viscosity curve fit */
        LinearFit         mSpecificHeatOXYGEN;             /**< (--) OXYGEN specific heat curve fit */
        LinearFit         mThermalConductivityOXYGEN;      /**< (--) OXYGEN thermal conductivity curve fit */
        CubicFit          mPrandtlNumberOXYGEN;            /**< (--) OXYGEN prandtl number curve fit */
        LinearFit         mAdiabaticIndexOXYGEN;           /**< (--) OXYGEN adiabatic index curve fit */
        QuadLinInvFit     mPressureOXYGEN;                 /**< (--) OXYGEN pressure curve fit */
        QuadraticRootFit  mTemperatureOXYGEN;              /**< (--) OXYGEN temperature curve fit */
        QuadLinFit        mDensityHYDROGEN;                /**< (--) HYDROGEN density curve fit */
        CubicFit          mViscosityHYDROGEN;              /**< (--) HYDROGEN dynamic viscosity curve fit */
        LinearFit         mSpecificHeatHYDROGEN;           /**< (--) HYDROGEN specific heat curve fit */
        LinearFit         mThermalConductivityHYDROGEN;    /**< (--) HYDROGEN thermal conductivity curve fit */
        CubicFit          mPrandtlNumberHYDROGEN;          /**< (--) HYDROGEN prandtl number curve fit */
        LinearFit         mAdiabaticIndexHYDROGEN;         /**< (--) HYDROGEN adiabatic index curve fit */
        QuadLinInvFit     mPressureHYDROGEN;               /**< (--) HYDROGEN pressure curve fit */
        QuadraticRootFit  mTemperatureHYDROGEN;            /**< (--) HYDROGEN temperature curve fit */
        QuadLinFit        mDensityMETHANE;                 /**< (--) METHANE density curve fit */
        CubicFit          mViscosityMETHANE;               /**< (--) METHANE dynamic viscosity curve fit */
        LinearFit         mSpecificHeatMETHANE;            /**< (--) METHANE specific heat curve fit */
        LinearFit         mThermalConductivityMETHANE;     /**< (--) METHANE thermal conductivity curve fit */
        CubicFit          mPrandtlNumberMETHANE;           /**< (--) METHANE prandtl number curve fit */
        LinearFit         mAdiabaticIndexMETHANE;          /**< (--) METHANE adiabatic index curve fit */
        QuadLinInvFit     mPressureMETHANE;                /**< (--) METHANE pressure curve fit */
        QuadraticRootFit  mTemperatureMETHANE;             /**< (--) METHANE temperature curve fit */
        QuadLinFit        mDensityNAK78;                   /**< (--) NAK78 density curve fit */
        PowerFit          mViscosityNAK78;                 /**< (--) NAK78 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatNAK78;              /**< (--) NAK78 specific heat curve fit */
        QuadraticFit      mThermalConductivityNAK78;       /**< (--) NAK78 thermal conductivity curve fit */
        QuadraticFit      mPrandtlNumberNAK78;             /**< (--) NAK78 prandtl number curve fit */
        QuadLinInvFit     mPressureNAK78;                  /**< (--) NAK78 pressure curve fit */
        QuadraticRootFit  mTemperatureNAK78;               /**< (--) NAK78 temperature curve fit */
        QuadLinFit        mDensityGALDEN170;               /**< (--) GALDEN170 density curve fit */
        PowerFit          mViscosityGALDEN170;             /**< (--) GALDEN170 dynamic viscosity curve fit */
        LinearFit         mSpecificHeatGALDEN170;          /**< (--) GALDEN170 specific heat curve fit */
        LinearFit         mThermalConductivityGALDEN170;   /**< (--) GALDEN170 thermal conductivity curve fit */
        PowerFit          mPrandtlNumberGALDEN170;         /**< (--) GALDEN170 prandtl number curve fit */
        QuadLinInvFit     mPressureGALDEN170;              /**< (--) GALDEN170 pressure curve fit */
        QuadraticRootFit  mTemperatureGALDEN170;           /**< (--) GALDEN170 temperature curve fit */
        QuadraticFit      mSaturationPressureGALDEN170;    /**< (--) GALDEN170 saturation pressure curve fit */
        FluidTsatFit      mSaturationTemperatureGALDEN170; /**< (--) GALDEN170 saturation temperature curve fit */
        FluidHvapFit      mHeatOfVaporizationGALDEN170;    /**< (--) GALDEN170 heat of vaporization curve fit */
        FluidPropertiesDataWaterPvt mDataWATERPVT;         /**< (--) WATER_PVT table data object */
        TsBilinearInterpolator mDensityWATERPVT;           /**< (--) WATER_PVT density table */
        TsBilinearInterpolatorReverse mPressureWATERPVT;   /**< (--) WATER_PVT pressure table */
        QuadLinFit        mDensityNTO;                     /**< (--) NTO density curve fit */
        QuarticFit        mViscosityNTO;                   /**< (--) NTO dynamic viscosity curve fit */
        LinearFit         mSpecificHeatNTO;                /**< (--) NTO specific heat curve fit */
        QuadraticFit      mThermalConductivityNTO;         /**< (--) NTO thermal conductivity curve fit */
        QuarticFit        mPrandtlNumberNTO;               /**< (--) NTO prandtl number curve fit */
        LinearFit         mAdiabaticIndexNTO;              /**< (--) NTO adiabatic index curve fit */
        QuadLinInvFit     mPressureNTO;                    /**< (--) NTO pressure curve fit */
        QuadraticRootFit  mTemperatureNTO;                 /**< (--) NTO temperature curve fit */
        QuadLinFit        mDensityMMH;                     /**< (--) MMH density curve fit */
        QuinticFit        mViscosityMMH;                   /**< (--) MMH dynamic viscosity curve fit */
        LinearFit         mSpecificHeatMMH;                /**< (--) MMH specific heat curve fit */
        QuadraticFit      mThermalConductivityMMH;         /**< (--) MMH thermal conductivity curve fit */
        QuarticFit        mPrandtlNumberMMH;               /**< (--) MMH prandtl number curve fit */
        LinearFit         mAdiabaticIndexMMH;              /**< (--) MMH adiabatic index curve fit */
        QuadLinInvFit     mPressureMMH;                    /**< (--) MMH pressure curve fit */
        QuadraticRootFit  mTemperatureMMH;                 /**< (--) MMH temperature curve fit */
        QuadLinFit        mDensityHYDRAZINE;               /**< (--) HYDRAZINE density curve fit */
        PowerFit          mViscosityHYDRAZINE;             /**< (--) HYDRAZINE dynamic viscosity curve fit */
        LinearFit         mSpecificHeatHYDRAZINE;          /**< (--) HYDRAZINE specific heat curve fit */
        QuadraticFit      mThermalConductivityHYDRAZINE;   /**< (--) HYDRAZINE thermal conductivity curve fit */
        PowerFit          mPrandtlNumberHYDRAZINE;         /**< (--) HYDRAZINE prandtl number curve fit */
        LinearFit         mAdiabaticIndexHYDRAZINE;        /**< (--) HYDRAZINE adiabatic index curve fit */
        QuadLinInvFit     mPressureHYDRAZINE;              /**< (--) HYDRAZINE pressure curve fit */
        QuadraticRootFit  mTemperatureHYDRAZINE;           /**< (--) HYDRAZINE temperature curve fit */
        FluidProperties   mPropertiesCO;                   /**< (--) CO fluid properties (ideal gas) */
        FluidProperties   mPropertiesCO2;                  /**< (--) CO2 fluid properties (ideal gas) */
        FluidProperties   mPropertiesH2O;                  /**< (--) H2O fluid properties (ideal gas) */
        FluidProperties   mPropertiesN2;                   /**< (--) N2 fluid properties (ideal gas) */
        FluidProperties   mPropertiesO2;                   /**< (--) O2 fluid properties (ideal gas) */
        FluidProperties   mPropertiesNH3;                  /**< (--) NH3 fluid properties (ideal gas) */
        FluidProperties   mPropertiesH2;                   /**< (--) H2 fluid properties (ideal gas) */
        FluidProperties   mPropertiesCH4;                  /**< (--) CH4 fluid properties (ideal gas) */
        FluidProperties   mPropertiesHCL;                  /**< (--) HCL fluid properties (ideal gas) */
        FluidProperties   mPropertiesHCN;                  /**< (--) HCN fluid properties (ideal gas) */
        FluidProperties   mPropertiesHe;                   /**< (--) He fluid properties (ideal gas) */
        FluidProperties   mPropertiesHeReal;               /**< (--) He real-gas fluid properties (real-gas compressibility) */
        FluidProperties   mPropertiesXe;                   /**< (--) Xe fluid properties (real-gas compressibility) */
        FluidProperties   mPropertiesN2Real;               /**< (--) N2 real-gas fluid properties (real-gas compressibility) */
        FluidProperties   mPropertiesO2Real;               /**< (--) O2 real-gas fluid properties (real-gas compressibility) */
        FluidProperties   mPropertiesH2Real;               /**< (--) H2 real-gas fluid properties (real-gas compressibility) */
        FluidProperties   mPropertiesWATER;                /**< (--) WATER fluid properties (liquid) */
        FluidProperties   mPropertiesHFE7000 ;             /**< (--) HFE-7000 fluid properties (liquid) */
        FluidProperties   mPropertiesHFE7100 ;             /**< (--) HFE-7100 fluid properties (liquid) */
        FluidProperties   mPropertiesPG30;                 /**< (--) Propylene Glycol 30% fluid properties (liquid) */
        FluidProperties   mPropertiesPG40;                 /**< (--) Propylene Glycol 40% fluid properties (liquid) */
        FluidProperties   mPropertiesPG50;                 /**< (--) Propylene Glycol 50% fluid properties (liquid) */
        FluidProperties   mPropertiesAMMONIA;              /**< (--) AMMONIA fluid properties (liquid) */
        FluidProperties   mPropertiesOXYGEN;               /**< (--) OXYGEN fluid properties (liquid) */
        FluidProperties   mPropertiesHYDROGEN;             /**< (--) HYDROGEN fluid properties (liquid) */
        FluidProperties   mPropertiesMETHANE;              /**< (--) METHANE fluid properties (liquid) */
        FluidProperties   mPropertiesNAK78;                /**< (--) NaK-78 fluid properties (liquid) */
        FluidProperties   mPropertiesGALDEN170;            /**< (--) Galden HT-170 fluid properties (liquid) */
        FluidProperties   mPropertiesWATERPVT;             /**< (--) WATER_PVT fluid properties (liquid with density table lookup) */
        FluidProperties   mPropertiesNTO;                  /**< (--) NTO fluid properties (liquid) */
        FluidProperties   mPropertiesMMH;                  /**< (--) MMH fluid properties (liquid) */
        FluidProperties   mPropertiesHYDRAZINE;            /**< (--) HYDRAZINE fluid properties (liquid) */
        FluidProperties*  mProperties[FluidProperties::NO_FLUID]; /**< (--) Array of pointers to the defined Fluid Properties */
        static const double mHeTemperatureScale[12]; /**< (K)     Helium real-gas density table temperature scale */
        static const double mHePressureScale[12];    /**< (kPa)   Helium real-gas density table pressure scale */
        static const double mHeDensityTable[144];    /**< (kg/m3) Helium real-gas density table */
        static const double mXeTemperatureScale[12]; /**< (K)     Xenon real-gas density table temperature scale */
        static const double mXePressureScale[12];    /**< (kPa)   Xenon real-gas density table pressure scale */
        static const double mXeDensityTable[144];    /**< (kg/m3) Xenon real-gas density table */
        static const double mN2TemperatureScale[12]; /**< (K)     Nitrogen real-gas table temperature scale */
        static const double mN2PressureScale[14];    /**< (kPa)   Nitrogen real-gas table pressure scale */
        static const double mN2DensityTable[168];    /**< (kg/m3) Nitrogen real-gas density table */
        static const double mO2TemperatureScale[12]; /**< (K)     Oxygen real-gas table temperature scale */
        static const double mO2PressureScale[14];    /**< (kPa)   Oxygen real-gas table pressure scale */
        static const double mO2DensityTable[168];    /**< (kg/m3) Oxygen real-gas density table */
        static const double mH2TemperatureScale[13]; /**< (K)     Hydrogen real-gas table temperature scale */
        static const double mH2PressureScale[13];    /**< (kPa)   Hydrogen real-gas table pressure scale */
        static const double mH2DensityTable[169];    /**< (kg/m3) Hydrogen real-gas density table */

    // Trick ICG has had problems before with declaring static const attributes before non-static
    // attributes, so adding a 2nd public block down here...
    public:
        /// @name    Molecular Weight Constants.
        /// @{
        /// @details These constants are public to allow outside models to use them without the
        ///          overhead of declaring a DefinedFluidProperties.
        static const double mMWeightCO;              /**< (1/mol) Molecular weight of CO */
        static const double mMWeightCO2;             /**< (1/mol) Molecular weight of CO2 */
        static const double mMWeightH2O;             /**< (1/mol) Molecular weight of H2O */
        static const double mMWeightN2;              /**< (1/mol) Molecular weight of N2 */
        static const double mMWeightO2;              /**< (1/mol) Molecular weight of O2 */
        static const double mMWeightNH3;             /**< (1/mol) Molecular weight of NH3 */
        static const double mMWeightH2;              /**< (1/mol) Molecular weight of H2 */
        static const double mMWeightCH4;             /**< (1/mol) Molecular weight of CH4 */
        static const double mMWeightHCL;             /**< (1/mol) Molecular weight of HCL */
        static const double mMWeightHCN;             /**< (1/mol) Molecular weight of HCN */
        static const double mMWeightHe;              /**< (1/mol) Molecular weight of He */
        static const double mMWeightXe;              /**< (1/mol) Molecular weight of Xe */
        static const double mMWeightN2O4;            /**< (1/mol) Molecular weight of N2O4 */
        static const double mMWeightN2H4;            /**< (1/mol) Molecular weight of N2H4 */
        static const double mMWeightCH3N2H3;         /**< (1/mol) Molecular weight of CH3N2H3 */
        static const double mMWeightHFE7000;         /**< (1/mol) Molecular weight of HFE7000 */
        static const double mMWeightHFE7100;         /**< (1/mol) Molecular weight of HFE7100 */
        static const double mMWeightPG30;            /**< (1/mol) Molecular weight of propylene glycol 30% */
        static const double mMWeightPG40;            /**< (1/mol) Molecular weight of propylene glycol 40% */
        static const double mMWeightPG50;            /**< (1/mol) Molecular weight of propylene glycol 50% */
        static const double mMWeightNAK78;           /**< (1/mol) Molecular weight of NaK-78 */
        static const double mMWeightGALDEN170;       /**< (1/mol) Molecular weight of Galden HT-170 */
        /// @}

        /// @name    Critical Temperature Constants.
        /// @{
        /// @details These constants are public to allow outside models to use them without the
        ///          overhead of declaring a DefinedFluidProperties.
        static const double mCriticalTemperatureCO;  /**< (K) Critical temperature of CO */
        static const double mCriticalTemperatureCO2; /**< (K) Critical temperature of CO2 */
        static const double mCriticalTemperatureH2O; /**< (K) Critical temperature of H2O */
        static const double mCriticalTemperatureN2;  /**< (K) Critical temperature of N2 */
        static const double mCriticalTemperatureO2;  /**< (K) Critical temperature of O2 */
        static const double mCriticalTemperatureNH3; /**< (K) Critical temperature of NH3 */
        static const double mCriticalTemperatureH2;  /**< (K) Critical temperature of H2 */
        static const double mCriticalTemperatureCH4; /**< (K) Critical temperature of CH4 */
        static const double mCriticalTemperatureHCL; /**< (K) Critical temperature of HCL */
        static const double mCriticalTemperatureHCN; /**< (K) Critical temperature of HCN */
        static const double mCriticalTemperatureHe;  /**< (K) Critical temperature of He */
        static const double mCriticalTemperatureXe;  /**< (K) Critical temperature of Xe */
        static const double mCriticalTemperatureN2O4;    /**< (K) Critical temperature of N2O4 */
        static const double mCriticalTemperatureN2H4;    /**< (K) Critical temperature of N2H4 */
        static const double mCriticalTemperatureCH3N2H3; /**< (K) Critical temperature of CH3N2H3 */
        static const double mCriticalTemperatureHFE7000; /**< (K) Critical temperature of HFE7000 */
        static const double mCriticalTemperatureHFE7100; /**< (K) Critical temperature of HFE7100 */
        static const double mCriticalTemperaturePG30;    /**< (K) Critical temperature of propylene glycol 30% */
        static const double mCriticalTemperaturePG40;    /**< (K) Critical temperature of propylene glycol 40% */
        static const double mCriticalTemperaturePG50;    /**< (K) Critical temperature of propylene glycol 50% */
        static const double mCriticalTemperatureNAK78;   /**< (K) Critical temperature of NaK-78 */
        static const double mCriticalTemperatureGALDEN170; /**< (K) Critical temperature of Galden HT-170 */
        /// @}
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        DefinedFluidProperties(const DefinedFluidProperties&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        DefinedFluidProperties& operator =(const DefinedFluidProperties&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The FluidType (chemical composition or name enumeration) of this Fluid
///
/// @details  Returns the type of this Fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline FluidProperties::FluidType FluidProperties::getType() const
{
    return mType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The FluidPhase (gas, liquid, solid) of this Fluid
///
/// @details  Returns the phase of this Fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline FluidProperties::FluidPhase FluidProperties::getPhase() const
{
    return mPhase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The molecular weight (1/mol) of this Fluid.
///
/// @details  Returns the molecular weight of this Fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getMWeight() const
{
    return mMWeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The critical temperature (K) of this Fluid.
///
/// @details  Returns the critical temperature of this Fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getCriticalTemperature() const
{
    return mCriticalTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)   Temperature of this Fluid
/// @param[in]    pressure     (kPa) Pressure of this Fluid
///
/// @return   The density (kg/m3) of this Fluid at the specified temperature and pressure
///
/// @note     rho = getDensity(t, getPressure(t,rho));
///
/// @note     If the specified temperature or pressure is outside the bounds of the data, then the
///           appropriate boundary value is returned.
///
/// @details  Computes and returns the density of this Fluid at the specified temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getDensity(const double temperature,
                                          const double pressure) const
{
    return mDensity->get(temperature, pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)   Temperature of this Fluid
/// @param[in]    pressure     (kPa) Pressure of this Fluid
///
/// @return   The dynamic viscosity (Pa-s) of this Fluid at the specified temperature and pressure
///
/// @note     If the specified temperature or pressure is outside the bounds of the data, then the
///           appropriate boundary value is returned.
///
/// @details  Computes and returns the dynamic viscosity of this Fluid at the specified
///           temperature and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getViscosity(const double temperature,
                                            const double pressure) const
{
    return mViscosity->get(temperature, pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)   Temperature of this Fluid
/// @param[in]    pressure     (kPa) Pressure of this Fluid
///
/// @return   The specific heat (J/kg/K) of this Fluid at the specified temperature and pressure
///
/// @note     If the specified temperature or pressure is outside the bounds of the data, then the
///           appropriate boundary value is returned.
///
/// @details  Computes and returns the specific heat of this Fluid at the specified temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getSpecificHeat(const double temperature,
                                               const double pressure) const
{
    return mSpecificHeat->get(temperature, pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)   Temperature of this Fluid
/// @param[in]    pressure     (kPa) Pressure of this Fluid
///
/// @return   The specific enthalpy (J/kg) of this Fluid at the specified temperature and pressure
///
/// @note     h = getSpecificEnthalpy(getTemperature(h,p),p).
///
/// @note     If the specified temperature or pressure is outside the bounds of the data, then the
///           appropriate boundary value is returned.
///
/// @details  Computes and returns the specific enthalpy of this Fluid at the specified temperature
///           and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getSpecificEnthalpy(const double temperature,
                                                   const double pressure) const
{
    return mSpecificHeat->get(temperature, pressure) * temperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)   Temperature of this Fluid
/// @param[in]    pressure     (kPa) Pressure of this Fluid
///
/// @return   The thermal conductivity (W/m/K) of this Fluid at the specified temperature and
///           pressure.
///
/// @note     If the specified temperature or pressure is outside the bounds of the data, then the
///           appropriate boundary value is returned.
///
/// @details  Computes and returns the thermal conductivity of this Fluid at the specified
///           temperature and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getThermalConductivity(const double temperature,
                                                      const double pressure) const
{
    return mThermalConductivity->get(temperature, pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)   Input temperature of this Fluid
/// @param[in]    pressure     (kPa) Input pressure of this Fluid
///
/// @return   The PrandtlNumber -- of this Fluid at the specified temperature and pressure.
///
/// @note     If the specified temperature or density is outside the bounds of the data, then the
///           appropriate boundary value is returned.
///
/// @details  Computes and returns the Prandtl Number -- of this Fluid at the specified
///           temperature and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getPrandtlNumber(const double temperature,
                                                const double pressure) const
{
    return mPrandtlNumber->get(temperature, pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)   Temperature of this Fluid
/// @param[in]    pressure     (kPa) Pressure of this Fluid
///
/// @return   The adiabatic index (gamma) of this Fluid at the specified temperature and pressure.
///
/// @note     If the specified temperature or density is outside the bounds of the data, then the
///           appropriate boundary value is returned.
///
/// @details  Computes and returns the adiabatic index (gamma) of this Fluid at the specified
///           temperature and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getAdiabaticIndex(const double temperature,
                                                 const double pressure) const
{
    return mAdiabaticIndex->get(temperature, pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K)     Temperature of this Fluid
/// @param[in]    density      (kg/m3) Density of this Fluid
///
/// @return   The pressure (kPa) of this Fluid at the specified temperature and density.
///
/// @note     Either p = getPressure(t, getDensity(t, p)) or p >= DBL_EPSILON.
///
/// @note     If the specified temperature or density is outside the bounds of the data, then the
///           appropriate boundary value is returned.
///
/// @details  Computes and returns the pressure of this Fluid at the specified temperature and
///           density.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getPressure(const double temperature,
                                           const double density) const
{
    double result = mPressure->get(temperature, density);

    if (result < DBL_EPSILON) {
        result = DBL_EPSILON;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    specificEnthalpy  (J/kg) Specific enthalpy of this Fluid
/// @param[in]    pressure          (kPa)  Pressure of this Fluid
///
/// @return   The temperature (K) of this Fluid at the specified specific enthalpy and pressure
///
/// @note     t = getTemperature(getSpecificEnthalpy(t, p), p)
///
/// @note     If the specified specific enthalpy or pressure is outside the bounds of the data,
///           then the appropriate boundary value is returned.
///
/// @details  Computes and returns the temperature of this Fluid at the specified specific enthalpy
///           and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getTemperature(const double specificEnthalpy,
                                              const double pressure) const
{
    return mTemperature->get(specificEnthalpy, pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K) Temperature to evaluate saturation pressure at
///
/// @return   The saturation pressure (kPa) of this Fluid at the specified temperature
///
/// @note     p = getSaturationPressure(t)
///
/// @note     If the specified temperature is outside the bounds of the data,
///           then the appropriate boundary value is returned.
///
/// @details  Computes and returns the saturation pressure of this Fluid at the specified
///           temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getSaturationPressure(const double temperature) const
{
    return pow(10.0, mSaturationPressure->get(getCriticalTemperature() / std::max(temperature, DBL_EPSILON)));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    pressure  (kPa) Pressure to evaluate saturation temperature at
///
/// @return   The saturation temperature (K) of this Fluid at the specified pressure
///
/// @note     t = getSaturationTemperature(p)
///
/// @note     If the specified pressure is outside the bounds of the data,
///           then the appropriate boundary value is returned.
///
/// @details  Computes and returns the saturation temperature of this Fluid at the specified
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getSaturationTemperature(const double pressure) const
{
    // Divide by zero is protected against in initialization of FluidTsatFit class.
    return getCriticalTemperature() / mSaturationTemperature->get(pressure);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    temperature  (K) Temperature to evaluate heat of vaporization at
///
/// @return   The latent heat of vaporization (kJ/kg) of this Fluid at the specified temperature
///
/// @note     L = getHeatOfVaporization(t)
///
/// @note     If the specified temperature is outside the bounds of the data,
///           then the appropriate boundary value is returned.
///
/// @details  Computes and returns the latent heat of vaporization of this Fluid at the specified
///           temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double FluidProperties::getHeatOfVaporization(const double temperature) const
{
    return mHeatOfVaporization->get(temperature / getCriticalTemperature());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type   Type of Fluid
///
/// @return   A pointer to the properties of the specified fluid or null if undefined.
///
/// @details  Returns a pointer to the properties of the fluid specified by fluid type.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline FluidProperties* DefinedFluidProperties::getProperties(const FluidProperties::FluidType& type) const
{
    FluidProperties* result = 0;

    if (0 <= type && type < FluidProperties::NO_FLUID) {
        result =  mProperties[type];
    }

    return result;
}

#endif
