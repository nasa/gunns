#ifndef PolyFluid_EXISTS
#define PolyFluid_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_FLUID_POLY_FLUID PolyFluid Model
@ingroup   TSM_GUNNS_FLUID_FLUID

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling PolyFluids. A PolyFluid is a composite fluid made up of
   constituent MonoFluids. A PolyFluid has state: temperature, pressure, mass  flow rate, mass and
   moles as well as properties: molecular weight, density, viscosity, specific heat, specific
   enthalpy and adiabatic index. The properties of a PolyFluid are the mole or mass fraction
   weighted averages of the constituent properties.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ((The array of mass fractions specified in the input data must be of the same length and in the
    same order as the array of types specified in the configuration data.)
   (The user of this class is responsible for checking that it has been successfully initialized
    before making any access, modify, update or compute calls.)
  )

 LIBRARY DEPENDENCY:
- ((PolyFluid.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-04))

@{
*/

#include "aspects/fluid/fluid/MonoFluid.hh"
#include "aspects/fluid/fluid/GunnsFluidTraceCompounds.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PolyFluid Config Data
///
/// @details  The sole purpose of this class is to provide a data structure for the PolyFluid
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class PolyFluidConfigData
{
    public:
        const DefinedFluidProperties*             mProperties;     /**<    (--) trick_chkpnt_io(**) Pointer to the defined fluid properties */
        FluidProperties::FluidType*               mTypes;          /**< ** (--) trick_chkpnt_io(**) Pointer to the array of constituent types */
        int                                       mNTypes;         /**<    (--) trick_chkpnt_io(**) Number of constituent types */
        const GunnsFluidTraceCompoundsConfigData* mTraceCompounds; /**<    (--) trick_chkpnt_io(**) Optional pointer to the trace compounds config data. */
        /// @brief Constructs this PolyFluid configuration data.
        PolyFluidConfigData(const DefinedFluidProperties*             properties     = 0,
                            const FluidProperties::FluidType*         types          = 0,
                            const int                                 nTypes         = 0,
                            const GunnsFluidTraceCompoundsConfigData* traceCompounds = 0);
        /// @brief Copy constructs this PolyFluid configuration data.
        PolyFluidConfigData(const PolyFluidConfigData& that);
        /// @brief Default destructs this PolyFluid configuration data.
        virtual ~PolyFluidConfigData();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details Default constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
//        PolyFluidConfigData();
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        PolyFluidConfigData& operator=(const PolyFluidConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    PolyFluid Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the PolyFluid input
///           data. Once constructed, the number of mass fractions should not be changed.
////////////////////////////////////////////////////////////////////////////////////////////////////
class PolyFluidInputData : public MonoFluidInputData
{
    public:
        double*                                  mMassFraction;   /**< (--) trick_chkpnt_io(**) Array of constituent mass fractions. */
        const GunnsFluidTraceCompoundsInputData* mTraceCompounds; /**< (--) trick_chkpnt_io(**) Optional pointer to the trace compounds input data. */
        /// @brief Default constructs this PolyFluid input data.
        PolyFluidInputData(const double                             temperature    = 0.0,
                           const double                             pressure       = 0.0,
                           const double                             flowRate       = 0.0,
                           const double                             mass           = 0.0,
                           double*                                  massFraction   = 0,
                           const GunnsFluidTraceCompoundsInputData* traceCompounds = 0);
        /// @brief Copy constructs this PolyFluid input data.
        PolyFluidInputData(const PolyFluidInputData& that);
        /// @brief Default destructs this PolyFluid input data.
        virtual ~PolyFluidInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details Default constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
//        PolyFluidInputData();
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        PolyFluidInputData& operator=(const PolyFluidInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    A typedef for a struct containing the constituent fluid data that is mapped
///           by type.
////////////////////////////////////////////////////////////////////////////////////////////
class Constituent {
    TS_MAKE_SIM_COMPATIBLE(Constituent);
    public:
        /// @brief Default constructs this Constituent
        Constituent();
        /// @brief Default destructs this Constituent
        virtual ~Constituent();
        FluidProperties::FluidType  mType;           /**< *o (--) trick_chkpnt_io(**) The type of this constituent fluid */
        MonoFluid                   mFluid;          /**<    (--)                     The constituent fluid */
        double                      mMassFraction;   /**<    (--)                     Mass fraction of this constituent fluid */
        double                      mMoleFraction;   /**<    (--)                     Mole fraction of this constituent fluid */

    private:
        /// @details The assignment operator is unavailable since it is declared private and not
        ///          implemented.
        Constituent& operator =(const Constituent&);
        /// @details The default copy constructor is unavailable since it is declared private and
        ///          not implemented.
        Constituent(const Constituent& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Class for PolyFluid, a multiple species fluid.
///
/// @details
/// Provides the class for a multiple species fluid containing the state and properties for the
/// composite fluid as well as the constituent fluids and their mass and mole fractions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class PolyFluid
{
    TS_MAKE_SIM_COMPATIBLE(PolyFluid);
    public:
        /// @brief Default constructs this PolyFluid.
        PolyFluid();
        /// @brief Default constructs this PolyFluid.
        PolyFluid(const PolyFluidConfigData& configData, const PolyFluidInputData& inputData);
        /// @brief Copy constructs this PolyFluid.
        PolyFluid(const PolyFluid& that, const std::string& name, const bool copyTc = true);
        /// @brief Default destructs this PolyFluid.
        virtual ~PolyFluid();
        /// @brief Initializes this PolyFluid Model with configuration and input data.
        void initialize(const PolyFluidConfigData& configData, const PolyFluidInputData& inputData);
        /// @brief Initializes this PolyFuid Model's name attribute.
        void initializeName(const std::string& name);
        /// @brief Returns the initialization status of this PolyFluid.
        bool isInitialized() const;
        /// @brief Returns the temperature of this PolyFluid.
        double getTemperature() const;
        /// @brief Returns the pressure of this PolyFluid.
        double getPressure() const;
        /// @brief Returns the mass flow rate of this PolyFluid.
        double getFlowRate() const;
        /// @brief Returns the mass of this PolyFluid.
        double getMass() const;
        /// @brief Returns the moles of this PolyFluid.
        double getMole() const;
        /// @brief Returns the mole fraction of the specified constituent fluid.
        double getMoleFraction(const FluidProperties::FluidType& type) const;
        /// @brief Returns the mass fraction of the specified constituent fluid.
        double getMassFraction(const FluidProperties::FluidType& type) const;
#ifndef SWIG
        /// @brief Returns the mole fraction of the specified constituent fluid.
        /// @note  This overloaded function is hidden from the Trick input file, as SWIG can't
        ///        handle the ambiguity between C++ int and enum types in python.  Instead, input
        ///        files should use the version with the FluidType argument.
        double getMoleFraction(const int index) const;
        /// @brief Returns the mass fraction of the specified constituent fluid.
        /// @note  This overloaded function is hidden from the Trick input file, as SWIG can't
        ///        handle the ambiguity between C++ int and enum types in python.  Instead, input
        ///        files should use the version with the FluidType argument.
        double getMassFraction(const int index) const;
#endif
        /// @brief Returns the partial pressure of the specified constituent fluid.
        double getPartialPressure(const FluidProperties::FluidType& type) const;
        /// @brief Returns the number of constituent fluids
        int getNConstituents() const;
        /// @brief Returns a pointer to the constituent fluid of the specified type
        MonoFluid* getConstituent(const FluidProperties::FluidType& type);
        /// @brief Returns a pointer to the constituent fluid properties of the specified type
        const FluidProperties* getProperties(const FluidProperties::FluidType& type) const;
        /// @brief  Returns the index of the constituent fluid of the specified type
        FluidProperties::FluidType getType(const int i) const;
        /// @brief  Returns the index of the constituent fluid of the specified type
        int find(const FluidProperties::FluidType& type) const;
        /// @brief  Returns the constituent and trace compound indexes of the specific chemical compound.
        void findCompound(int&                    fluidIndex,
                          int&                    tcIndex,
                          const ChemicalCompound* compound) const;
        /// @brief Returns the phase of this PolyFluid.
        FluidProperties::FluidPhase getPhase() const;
        /// @brief Returns the molecular weight of this PolyFluid.
        double getMWeight() const;
        /// @brief Returns the density of this PolyFluid.
        double getDensity() const;
        /// @brief Returns the viscosity of this PolyFluid.
        double getViscosity() const;
        /// @brief Returns the specific heat of this PolyFluid.
        double getSpecificHeat() const;
        /// @brief Returns the specific enthalpy of this PolyFluid.
        double getSpecificEnthalpy() const;
        /// @brief Returns the thermal conductivity (W/m/K) of this PolyFluid.
        double getThermalConductivity() const;
        /// @brief Returns the Prandtl number -- of this PolyFluid.
        double getPrandtlNumber() const;
        /// @brief Returns the adiabatic index (gamma) of this PolyFluid.
        double getAdiabaticIndex() const;
        /// @brief Returns a pointer to the trace compounds contained in this PolyFluid.
        GunnsFluidTraceCompounds* getTraceCompounds() const;
        /// @brief Sets the mass flow rate of this PolyFluid.
        void setFlowRate(const double flowRate);
        /// @brief Sets the mass and moles of this PolyFluid.
        void setMass(const double mass);
        /// @brief Sets the moles and mass of this PolyFluid.
        void setMole(const double mole);
        /// @brief Sets the pressure of this PolyFluid.
        void setPressure(const double pressure);
        /// @brief Sets the temperature of this PolyFluid.
        void setTemperature(const double temperature);
        /// @brief Sets the composite mass and mass fraction in composite of this constituent fluid.
        void setMassAndMassFractions(const double                      mass,
                                     double*                           massFraction);
        /// @brief Sets the composite mole and mole fraction in composite of this constituent fluid.
        void setMoleAndMoleFractions(const double                      mole,
                                     double*                           moleFraction);
        /// @brief Sets the moles of the constituent fluid of the specified type
        void setMass(const int index, const double mass);
        /// @brief Updates composite mass and moles and fractions from the constituent masses
        void updateMass();
        /// @brief Resets (zeros) the state of this PolyFluid
        void resetState();
        /// @brief Updates the state of this PolyFluid by copying from the specified fluid state
        void setState(const PolyFluid* that);
        /// @brief Updates the state of this PolyFluid by adding in the specified fluid state
        void addState(const PolyFluid* that, const double flowRate = 0.0);
        /// @brief Updates the state of this PolyFluid to the specified temperature and pressure.
        void edit(const double temperature, const double pressure);
        /// @brief Updates the state of this PolyFluid to the specified temperature and partial
        //         pressures.
        void edit(const double temperature, double* partialPressure);
        /// @brief Returns the temperature at the specified specific enthalpy
        double computeTemperature(const double specificEnthalpy) const;
        /// @brief Returns the specific enthalpy at the specified temperature and pressure
        double computeSpecificEnthalpy(const double temperature, const double pressure = 0.0) const;
        /// @brief Returns the pressure at the specified temperature and density
        double computePressure(const double temperature, const double density) const;
        /// @brief Returns the density at the specified temperature and pressure
        double computeDensity(const double temperature, const double pressure = 0.0) const;
        /// @brief Deletes dynamic memory allocated by this PolyFluid
        void cleanup();

    protected:
        static const double FRACTION_TOLERANCE;   /**<    (--)                     Fraction normalization threshold */
        static const double MOLE_INNER_LIMIT;     /**<    (--)                     Inner limit for various molar parameters */
        std::string         mName;                /**< *o (--) trick_chkpnt_io(**) Name of the instance for messaging */
        double              mTemperature;         /**<    (K)                      Temperature of the fluid */
        double              mPressure;            /**<    (kPa)                    Pressure of the fluid */
        double              mFlowRate;            /**<    (kg/s)                   Mass flow rate of the fluid */
        double              mMass;                /**<    (kg)                     Mass of the fluid */
        double              mMole;                /**<    (kg*mol)                 Moles of the fluid */
        Constituent*        mConstituents;        /**<    (--) trick_chkpnt_io(**) Array of constituents (type, fluid, mass fraction and mole fraction) */
        int                 mNConstituents;       /**<    (--) trick_chkpnt_io(**) Number of constituents */
        FluidProperties::FluidPhase mPhase;       /**<    (--)                     Phase of this PolyFluid */
        double              mMWeight;             /**<    (1/mol)                  Molecular weight of the fluid*/
        double              mDensity;             /**<    (kg/m3)                  Density at current T&P */
        double              mViscosity;           /**<    (Pa*s)                   Viscosity of the fluid */
        double              mSpecificHeat;        /**<    (J/kg/K)                 Specific heat of the fluid */
        double              mSpecificEnthalpy;    /**<    (J/kg)                   Specific enthalpy of the fluid */
        double              mThermalConductivity; /**<    (W/m/K)                  Thermal conductivity of the fluid */
        double              mPrandtlNumber;       /**<    (--)                     Prandtl number of the fluid */
        double              mAdiabaticIndex;      /**<    (--)                     Adiabatic index (gamma) of the fluid */
        GunnsFluidTraceCompounds* mTraceCompounds; /**<   (--)                     Trace compounds in the fluid. */
        bool                mInitFlag;            /**< *o (--) trick_chkpnt_io(**) Init status: T- if initialized and valid */
        /// @brief Validates this PolyFluid Model initial state.
        void validate();
        /// @brief Computes this PolyFluid derived state and properties.
        void derive();
    private:
        /// @details The assignment operator is unavailable since it is declared private and not
        ///          implemented.
        PolyFluid& operator =(const PolyFluid&);
        /// @details The default copy constructor is unavailable since it is declared private and
        ///          not implemented.
        PolyFluid(const PolyFluid& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Initialization status of this PolyFluid
///
/// @details  Returns the current initialization status of this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool PolyFluid::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Temperature (K) of this PolyFluid
///
/// @details  Returns the current temperature of this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getTemperature() const
{
    return mTemperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Pressure (kPa) of this PolyFluid
///
/// @details  Returns the current pressure of this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getPressure() const
{
    return mPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Mass Flow Rate (kg/m3) of this PolyFluid
///
/// @details  Returns the current mass flow rate (kg/s) of this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getFlowRate() const
{
    return mFlowRate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Mass (kg) of this PolyFluid
///
/// @details  Returns the current mass of this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getMass() const
{
    return mMass;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Moles (kg*mol) of this PolyFluid
///
/// @details  Returns the current moles of this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getMole() const
{
    return mMole;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The phase (gas, liquid, solid) of this PolyFluid.
///
/// @details  Returns the phase of this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline FluidProperties::FluidPhase PolyFluid::getPhase() const
{
    return mPhase;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The molecular weight (1/mol) of this PolyFluid.
///
/// @details  Returns the molecular weight of this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getMWeight() const
{
    return mMWeight;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The density (kg/m3) of this PolyFluid
///
/// @details  Returns the density of this PolyFluid at the current temperature and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getDensity() const
{
    return mDensity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The dynamic viscosity (Pa s) of this PolyFluid
///
/// @details  Returns the dynamic viscosity of this PolyFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getViscosity() const
{
    return mViscosity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The specific heat (J/kg/K) of this PolyFluid
///
/// @details  Returns the specific heat of this PolyFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getSpecificHeat() const
{
    return mSpecificHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The specific enthalpy (J/kg) of this PolyFluid
///
/// @details  Returns the specific enthalp of this PolyFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getSpecificEnthalpy() const
{
    return mSpecificEnthalpy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The thermal conductivity of this PolyFluid
///
/// @details  Returns the thermal conductivity (W/m/K) of this PolyFluid at the current temperature
///           and pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getThermalConductivity() const
{
    return mThermalConductivity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The Prandtl number of this PolyFluid
///
/// @details  Returns the Prandtl number of this PolyFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getPrandtlNumber() const
{
    return mPrandtlNumber;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The adiabatic index of this PolyFluid
///
/// @details  Returns the adiabatic index (gamma) of this PolyFluid at the current temperature and
///           pressure.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getAdiabaticIndex() const
{
    return mAdiabaticIndex;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   type  --  Type of fluid
///
/// @return   --  Mole fraction of the specified constituent fluid in this PolyFluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the mole fraction of the specified constituent fluid in this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getMoleFraction(const FluidProperties::FluidType& type) const
{
    return mConstituents[find(type)].mMoleFraction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   type  --  Type of fluid
///
/// @return   --  Mass fraction of the specified constituent fluid in this PolyFluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the mass fraction of the specified constituent fluid in this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getMassFraction(const FluidProperties::FluidType& type) const
{
    return mConstituents[find(type)].mMassFraction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   type  --  Type of fluid
///
/// @return   (kPa)  Partial pressure of the specified constituent fluid in this PolyFluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the partial pressure of the specified constituent fluid in this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double PolyFluid::getPartialPressure(const FluidProperties::FluidType& type) const
{
    return mConstituents[find(type)].mFluid.getPressure();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   --  Number of constituents
///
/// @details  Returns the number of constituent fluids in this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline int PolyFluid::getNConstituents() const
{
    return mNConstituents;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   type  --  Type of fluid
///
/// @return   --  Pointer to the specified constituent fluid of this PolyFluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns a pointer to the constituent fluid of the specified type in this
///           PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline MonoFluid* PolyFluid::getConstituent(const FluidProperties::FluidType& type)
{
    return &mConstituents[find(type)].mFluid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   type  --  Type of fluid
///
/// @return   --  Pointer to the specified constituent fluid properties of this PolyFluid
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns a const pointer to the constituent fluid properties of the specified type
///           in this PolyFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const FluidProperties* PolyFluid::getProperties(const FluidProperties::FluidType& type) const
{
    return mConstituents[find(type)].mFluid.getProperties();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   i  --  Index of constituent fluid
///
/// @return   --  Type of the specified constituent fluid of this PolyFluid
///
/// @details  Returns the type of the constituent fluid with the specified index in this PolyFluid
///           or NO_FLUID if not present.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline FluidProperties::FluidType PolyFluid::getType(const int i) const
{
    FluidProperties::FluidType result = FluidProperties::NO_FLUID;

    if (0 <= i && i < mNConstituents) {
        result = mConstituents[i].mType;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return  GunnsFluidTraceCompounds* (--) Pointer to the trace compounds object.
///
/// @details Returns a pointer to this PolyFluid's trace compounds object, or NULL if this fluid
///          does not have a trace compounds object.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsFluidTraceCompounds* PolyFluid::getTraceCompounds() const
{
    return mTraceCompounds;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   flowRate  (kg/s)  Mass flow rate
///
/// @return   void
///
/// @details  Sets the mass flow rate (kg/s) of this PolyFluid and its constituent fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void PolyFluid::setFlowRate(const double flowRate)
{
    /// - Set the composite mass flow rate.
    mFlowRate = flowRate;

    /// - Apportion the composite flow rate to the constituents according to their mass fractions.
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mFluid.setFlowRate(mConstituents[i].mMassFraction * mFlowRate);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]   mole  (kg*mol) Moles of the PolyFluid
///
/// @return   void
///
/// @details  Sets the moles and updates the mass of this PolyFluid and its constituent fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void PolyFluid::setMole(const double mole)
{
    /// - Set the composite moles and update the composite mass.
    mMole = mole;
    mMass = mMole * mMWeight;

    /// - Apportion the composite moles and mass to the constituents according to their mole and
    ///   mass fractions.
    for (int i = 0; i < mNConstituents; ++i) {
        mConstituents[i].mFluid.setMole(mConstituents[i].mMoleFraction * mMole);
    }
}

#endif
