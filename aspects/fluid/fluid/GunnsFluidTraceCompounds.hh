#ifndef GunnsFluidTraceCompounds_EXISTS
#define GunnsFluidTraceCompounds_EXISTS

/**
@file
@brief    GUNNS Fluid Trace Compounds Model declarations

@defgroup  TSM_GUNNS_FLUID_FLUID_TRACE_COMPOUNDS  GUNNS Fluid Trace Compounds Model
@ingroup   TSM_GUNNS_FLUID_FLUID

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling trace compounds in a GUNNS fluid network.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidTraceCompounds.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2016-01))

@{
*/
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "properties/ChemicalCompound.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Trace Compounds Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Trace Compounds
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTraceCompoundsConfigData
{
    public:
        int                            mNTypes;    /**<    (--) trick_chkpnt_io(**) Number of compound types added via Gunnshow. */
        std::vector<ChemicalCompound*> mCompounds; /**< ** (--) trick_chkpnt_io(**) Vector of compounds. */
        /// @brief  Default constructs this Fluid Trace Compounds configuration data with arguments.
        GunnsFluidTraceCompoundsConfigData(const ChemicalCompound::Type*   types     = 0,
                                           const int                       nTypes    = 0,
                                           const std::string&              name      = "Unnamed GunnsFluidTraceCompoundsConfigData");
        /// @brief  Default destructs this Fluid Trace Compounds configuration data.
        virtual ~GunnsFluidTraceCompoundsConfigData();
        /// @brief  Add an additional new compound describing its properties.
        void addCompound(const double                     molecWeight,
                         const std::string                name,
                         const FluidProperties::FluidType fluidType = FluidProperties::NO_FLUID,
                         ChemicalCompound::Type           type      = ChemicalCompound::NO_COMPOUND);
        /// @brief  Add an additional compound from the defined types.
        void addCompound(ChemicalCompound::Type type);

    private:
        std::string mName;                  /**< ** (--) trick_chkpnt_io(**) Name of this object. */
        /// @brief validate constructor data
        void validate(const ChemicalCompound::Type* types,
                      const int                     nTypes);
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidTraceCompoundsConfigData(const GunnsFluidTraceCompoundsConfigData&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidTraceCompoundsConfigData& operator =(const GunnsFluidTraceCompoundsConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Trace Compounds Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Trace Compounds
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTraceCompoundsInputData
{
    public:
        double* mState; /**< (--) trick_chkpnt_io(**) Array of trace compound states. */
        /// @brief  Default constructs this Fluid Trace Compounds input data with arguments.
        GunnsFluidTraceCompoundsInputData(double* state = 0);
        /// @brief  Default destructs this Fluid Trace Compounds input data.
        ~GunnsFluidTraceCompoundsInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidTraceCompoundsInputData(const GunnsFluidTraceCompoundsInputData&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidTraceCompoundsInputData& operator=(const GunnsFluidTraceCompoundsInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Trace Compounds
///
/// @details  This class models the fluid mole fractions of trace chemical compounds in a parent
///           fluid, such as the fluid contents of a GUNNS network node or link.  This allows the
///           transport and mixing of these compounds between parent fluids, for modeling the flow
///           of trace compounds around and between fluid networks.  The advantage of tracking the
///           trace compounds in this class is that it avoids needing to model all of the fluid
///           properties of these compounds in the network's PolyFluid objects -- since we assume
///           that trace amounts of compounds have no affect on the bulk fluid properties.  This
///           reduces the number of fluids in the network's fluid configuration, saving memory and
///           reducing the CPU cost of running the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTraceCompounds
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidTraceCompounds);
    public:
        /// @brief  TODO NEVER CALL THIS, see details.
        GunnsFluidTraceCompounds();
        /// @brief  Default constructs this Fluid Trace Compounds model.
        GunnsFluidTraceCompounds(const double& fluidMoles);
        /// @brief  Copy constructs this Fluid Trace Compounds model.
        GunnsFluidTraceCompounds(const GunnsFluidTraceCompounds& that,
                                 const double&                   fluidMoles,
                                 const std::string&              name);
        /// @brief  Default destructs this Fluid Trace Compounds model.
        virtual ~GunnsFluidTraceCompounds();
        /// @brief  Initializes this Fluid Trace Compounds model with configuration and input data.
        void    initialize(const GunnsFluidTraceCompoundsConfigData* configData,
                           const GunnsFluidTraceCompoundsInputData*  inputData,
                           const std::string&                        name);
        /// @brief  Restart job method to be run after a checkpoint load.
        void    restart();
        /// @brief  Returns the pointer to the Fluid Trace Compounds config data.
        const GunnsFluidTraceCompoundsConfigData* getConfig() const;
        /// @brief  Returns the compound type at the specified index.
        ChemicalCompound::Type getType(const int i) const;
        /// @brief  Returns the index of the compound at the specified index.
        int     find(const ChemicalCompound::Type& type, const std::string name = "NO_NAME") const;
        /// @brief  Returns the index of the given chemical compound.
        int     findCompound(const ChemicalCompound::Type compound) const;
        /// @brief  Returns this Fluid Trace Compounds array of compound mass values.
        double* getMasses() const;
        /// @brief  Returns the current mass of the given compound type.
        double  getMass(const ChemicalCompound::Type& type, const std::string name = "NO_NAME") const;
        /// @brief  Returns this Fluid Trace Compounds array of compound mole fraction values.
        double* getMoleFractions() const;
        /// @brief  Returns the current mole fraction of the given compound type.
        double  getMoleFraction(const ChemicalCompound::Type& type, const std::string name = "NO_NAME") const;
        /// @brief  Returns whether this Trace Compounds has been successfully initialized.
        bool    isInitialized() const;
        /// @brief  Sets all of the masses in this Trace Compounds to the given values.
        void    setMasses(const double* masses = 0);
        /// @brief  Sets the mass of the given compound type in this Trace Compounds.
        void    setMass(const ChemicalCompound::Type& type, const double mass, const std::string name = "NO_NAME");
        /// @brief  Sets all of the mole fractions in this Trace Compounds to the given values.
        void    setMoleFractions(const double* moleFractions = 0);
        /// @brief  Sets the mole fraction of the given compound type in this Trace Compounds.
        void    setMoleFraction(const ChemicalCompound::Type& type, const double moleFraction, const std::string name = "NO_NAME");
#ifndef SWIG
        /// @brief  Sets the mass of the given index in this Trace Compounds.
        /// @note   This overloaded function is hidden from the Trick input file, as SWIG can't
        ///         handle the ambiguity between C++ int and enum types in python.  Instead, input
        ///         files should use the version with the Type argument.
        void    setMass(const int index, const double mass);
        /// @brief  Sets the mole fraction of the given index in this Trace Compounds.
        /// @note   This overloaded function is hidden from the Trick input file, as SWIG can't
        ///         handle the ambiguity between C++ int and enum types in python.  Instead, input
        ///         files should use the version with the Type argument.
        void    setMoleFraction(const int index, const double moleFraction);
#endif
        /// @brief  Recomputes all compound masses from their mole fractions in the parent fluid.
        void    updateMasses();
        /// @brief  Recomputes all compound mole fractions from their masses in the parent fluid.
        void    updateMoleFractions();
        /// @brief  Mixes trace compounds of bulk flow of another fluid into the parent fluid.
        void    flowIn(const GunnsFluidTraceCompounds& source,
                       const double                    totalMolesIn);
        /// @brief  Flows trace compounds at given rates into the parent fluid.
        void    flowIn(const double* rates, const double dt);
        /// @brief  Removes trace compounds due to bulk flow out of the parent fluid.
        void    flowOut(const double totalMolesOut);
        /// @brief  Zeroes all negative masses and mole fractions.
        void    limitPositive();

    protected:
        std::string                               mName;          /**< *o (--)     trick_chkpnt_io(**) Name of the instance for messaging. */
        const GunnsFluidTraceCompoundsConfigData* mConfig;        /**< *o (--)     trick_chkpnt_io(**) Definition of compound types. */
        double*                                   mMass;          /**<    (kg)     trick_chkpnt_io(**) Array of compound masses. */
        double*                                   mMoleFraction;  /**<    (--)     trick_chkpnt_io(**) Array of compound mole fractions in the fluid. */
        const double&                             mFluidMoles;    /**< ** (kg*mol) trick_chkpnt_io(**) Moles of the parent fluid, not including moles of these trace compounds. */
        bool                                      mInitFlag;      /**< *o (--)     trick_chkpnt_io(**) Object initialized flag. */
        /// @brief  Deletes dynamically allocated memory.
        void cleanup();
        /// @brief  Validates the initialization inputs of this Fluid Trace Compounds model.
        void validate(const GunnsFluidTraceCompoundsConfigData* configData,
                      const GunnsFluidTraceCompoundsInputData*  inputData) const;

    private:
        static const double                       mNoRef;         /**< ** (--)     trick_chkpnt_io(**) Dummy placeholder to catch wrong constructor during init. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidTraceCompounds(const GunnsFluidTraceCompounds&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidTraceCompounds& operator=(const GunnsFluidTraceCompounds&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsFluidTraceCompoundsConfigData* (--) Pointer to this Trace Compound's config data.
///
/// @details  Returns a pointer to this Trace Compounds configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsFluidTraceCompoundsConfigData* GunnsFluidTraceCompounds::getConfig() const
{
    return mConfig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double*  (kg)  Array of trace compound masses.
///
/// @details  Returns the array of this Trace Compounds mass values.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsFluidTraceCompounds::getMasses() const
{
    return mMass;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double*  (kg)  Array of trace compound mole fractions.
///
/// @details  Returns the array of this Trace Compounds mole fraction values.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsFluidTraceCompounds::getMoleFractions() const
{
    return mMoleFraction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   Initialization status of this Trace Compounds.
///
/// @details  Returns the current initialization status of this Trace Compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidTraceCompounds::isInitialized() const
{
    return mInitFlag;
}

#endif
