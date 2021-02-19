#ifndef ChemicalCompound_EXISTS
#define ChemicalCompound_EXISTS
/**
@defgroup  TSM_UTILITIES_PROPERTIES_CHEMICAL_COMPOUND  Chemical Compounds
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling Chemical Compounds.)

REQUIREMENTS:
- ()

REFERENCE:
- (
  )

ASSUMPTIONS AND LIMITATIONS:
- (Only those  Chemical Compounds in the Defined Chemical Compounds class are available.)

 CLASS:
- ()

 LIBRARY DEPENDENCY:
- ((ChemicalCompound.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-05))

@{
 */

#include "properties/FluidProperties.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Class for a Chemical Compound.
///
/// @details  Provides a class for a Chemical Compound: compound type, corresponding fluid type,
///           and molecular weight.
////////////////////////////////////////////////////////////////////////////////////////////////////
class ChemicalCompound {
    TS_MAKE_SIM_COMPATIBLE(ChemicalCompound);
public:
    friend class DefinedChemicalCompounds;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief    Enumeration of the types of Chemical Compounds.
    ///
    /// @details  This enumeration is used to index the array of Defined Chemical Compounds.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    enum Type {
        CH2O        = 0,  ///<  Formaldehyde
        CH2CL2      = 1,  ///<  Methylene chloride (Dichloromethane)
        CH4         = 2,  ///<  Methane
        CH4O        = 3,  ///<  Methanol (Methyl alcohol)
        C2H4O       = 4,  ///<  Acetaldehyde
        C2H6O       = 5,  ///<  Ethanol
        C3H6O       = 6,  ///<  Acetone
        C3H10OSI    = 7,  ///<  Trimethylsilanol
        C4H4O       = 8,  ///<  Furan
        C4H10O      = 9,  ///<  1-Butanol
        C6H6        = 10, ///<  Benzene
        C6H18O3SI3  = 11, ///<  Hexamethylcyclotrisiloxane
        C7H8        = 12, ///<  Toluene
        C8H10       = 13, ///<  o-Xylene
        CO          = 14, ///<  Carbon monoxide
        CO2         = 15, ///<  Carbon dioxide
        H2          = 16, ///<  Hydrogen
        H2O         = 17, ///<  Water
        H3PO4       = 18, ///<  Phosphoric Acid
        H           = 19, ///<  Singular Hydrogen
        LIOH        = 20, ///<  Lithium hydroxide
        LIOHH2O     = 21, ///<  Lithium hydroxide monohydrate
        LI2CO3      = 22, ///<  Lithium carbonate
        NH3         = 23, ///<  Ammonia
        NH42HPO4    = 24, ///<  Di-Ammonium hydrogen phosphate
        O2          = 25, ///<  Oxygen
        OH          = 26, ///<  Hydroxyl
        O           = 27, ///<  Singular Oxygen
        He          = 28, ///<  Helium
        NO_COMPOUND = 29, ///<  Invalid or number of compounds - Keep this last!
    };
    const ChemicalCompound::Type     mType;      /**< (--)    Type of this Chemical Compound. */
    const std::string                mName;      /**< (--)    Compound name. */
    const FluidProperties::FluidType mFluidType; /**< (--)    Type of corresponding Fluid. */
    const double                     mMWeight;   /**< (1/mol) Molecular weight. */
    // Thermodynamic Coefficients used to calculate Enthalpy, Entropy, Gibbs Free Energy, and Specific Heat Capacity:
    const double* mThermoCoeffHighTemp;          /**< (--)    Thermodynamic Coefficients for temperatures higher than 1000 K. */
    const double* mThermoCoeffLowTemp ;          /**< (--)    Thermodynamic Coefficients for temperatures lower than 1000 K. */
    /// @brief    Default constructs this Chemical Compound with arguments.
    ChemicalCompound(const ChemicalCompound::Type     type                = ChemicalCompound::NO_COMPOUND,
                     const std::string                name                = "NO_COMPOUND",
                     const FluidProperties::FluidType fluidType           = FluidProperties::NO_FLUID,
                     const double                     mWeight             = 0,
                     const double*                    thermoCoeffHighTemp = 0,
                     const double*                    thermoCoeffLowTemp  = 0) ;
    /// @brief    Default destructs this Chemical Compound.
    virtual ~ChemicalCompound();
    /// @brief    Inequality operator for this Chemical Compound.
    bool operator !=(const ChemicalCompound& that) const;

private:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Copy constructor unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ChemicalCompound(const ChemicalCompound&);
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ChemicalCompound& operator =(const ChemicalCompound&);
};

/////////////////////////////////////////----///////////////////////////////////////////////////////
/// @brief    Class for Defined Chemical Compounds.
///
/// @details  Provides the Defined Chemical Compounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DefinedChemicalCompounds {
    TS_MAKE_SIM_COMPATIBLE(DefinedChemicalCompounds);
public:
    /// @brief    Default constructs this Defined Chemical Compounds.
    DefinedChemicalCompounds();
    /// @brief    Default destructs this Defined Chemical Compounds.
    virtual ~DefinedChemicalCompounds();
    /// @brief    Returns a pointer to the specified Chemical Compound.
    const ChemicalCompound* getCompound(const ChemicalCompound::Type& type) const;
    static const int mNThermoCoeff = 7;              /**< (--) Number of Thermodynamic Coefficients. */

protected:
    ChemicalCompound mCompoundCH2O;                  /**< (--) CH2O chemical compound */
    ChemicalCompound mCompoundCH2CL2;                /**< (--) CH2CL2 chemical compound */
    ChemicalCompound mCompoundCH4;                   /**< (--) CH4 chemical compound */
    ChemicalCompound mCompoundCH4O;                  /**< (--) CH4O chemical compound */
    ChemicalCompound mCompoundC2H4O;                 /**< (--) C2H4O chemical compound */
    ChemicalCompound mCompoundC2H6O;                 /**< (--) C2H6O chemical compound */
    ChemicalCompound mCompoundC3H6O;                 /**< (--) C3H6O chemical compound */
    ChemicalCompound mCompoundC3H10OSI;              /**< (--) C3H10OSI chemical compound */
    ChemicalCompound mCompoundC4H4O;                 /**< (--) C4H4O chemical compound */
    ChemicalCompound mCompoundC4H10O;                /**< (--) C4H10O chemical compound */
    ChemicalCompound mCompoundC6H6;                  /**< (--) C6H6 chemical compound */
    ChemicalCompound mCompoundC6H18O3SI3;            /**< (--) C6H18O3SI3 chemical compound */
    ChemicalCompound mCompoundC7H8;                  /**< (--) C7H8 chemical compound */
    ChemicalCompound mCompoundC8H10;                 /**< (--) C8H10 chemical compound */
    ChemicalCompound mCompoundCO;                    /**< (--) CO  chemical compound */
    ChemicalCompound mCompoundCO2;                   /**< (--) CO2 chemical compound */
    ChemicalCompound mCompoundH2;                    /**< (--) H2 chemical compound */
    ChemicalCompound mCompoundH2O;                   /**< (--) H2O chemical compound */
    ChemicalCompound mCompoundH3PO4;                 /**< (--) H3PO4 chemical compound */
    ChemicalCompound mCompoundH;                     /**< (--) H chemical compound */
    ChemicalCompound mCompoundLIOH;                  /**< (--) LIOH chemical compound */
    ChemicalCompound mCompoundLIOHH2O;               /**< (--) LIOHH2O chemical compound */
    ChemicalCompound mCompoundLI2CO3;                /**< (--) LI2CO3 chemical compound */
    ChemicalCompound mCompoundNH3;                   /**< (--) NH3 chemical compound */
    ChemicalCompound mCompoundNH42HPO4;              /**< (--) (NH4)2(HPO4) chemical compound */
    ChemicalCompound mCompoundO2;                    /**< (--) O2 chemical compound */
    ChemicalCompound mCompoundOH;                    /**< (--) OH chemical compound */
    ChemicalCompound mCompoundO;                     /**< (--) O chemical compound */
    ChemicalCompound mCompoundHe;                    /**< (--) He chemical compound */
    ChemicalCompound* mCompounds[ChemicalCompound::NO_COMPOUND];  /**< (--) Compounds pointer array */

    /// --        Arrays to Hold the Thermodynamic Coefficients for each Compound
    static const double ThermoCoeffHighTempScaleO2[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleO2[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleCO2[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleCO2[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleH2O[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleH2O[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleCH4[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleCH4[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleCO[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleCO[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleH2[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleH2[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleOH[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleOH[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleH[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleH[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleO[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleO[mNThermoCoeff] ;
    static const double ThermoCoeffHighTempScaleHe[mNThermoCoeff] ;
    static const double ThermoCoeffLowTempScaleHe[mNThermoCoeff] ;
    // default coefficients
    static const double ThermoCoeffDefaultScale[mNThermoCoeff] ;
private:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Copy constructor unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    DefinedChemicalCompounds(const DefinedChemicalCompounds&);
    ////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Assignment operator unavailable since declared private and not implemented.
    ////////////////////////////////////////////////////////////////////////////////////////////////
    DefinedChemicalCompounds& operator =(const DefinedChemicalCompounds&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  type   Type of Chemical Compound
///
/// @return   A pointer to the specified Chemical Compound or null if undefined.
///
/// @details  Returns a pointer to the Chemical Compound specified by Chemical Compound type.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const ChemicalCompound* DefinedChemicalCompounds::getCompound(const ChemicalCompound::Type& type) const
{
    ChemicalCompound* result = 0;

    if (0 <= type && type < ChemicalCompound::NO_COMPOUND) {
        result =  mCompounds[type];
    }

    return result;
}

#endif
