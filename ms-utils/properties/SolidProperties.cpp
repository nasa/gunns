/************************** TRICK HEADER **********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
     (Provides the class for modeling the properties of liquids and gases.)

 REQUIREMENTS:
     ()

 REFERENCE:
     ()

 ASSUMPTIONS AND LIMITATIONS:
     (The constructor is protected, so only those static Solid Properties objects in its friend,
      the Defined Solid Properties class are available.)

 LIBRARY DEPENDENCY:
     ()

 PROGRAMMERS:
     ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-05))
 **************************************************************************************************/

#include "SolidProperties.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    type                 Type of this Solid (from SolidType enum)
/// @param[in]    density              Fensity of this Solid
/// @param[in]    specificHeat         Specific heat of this Solid
/// @param[in]    thermalConductivity  Thermal conductivity of this Solid
/// @param[in]    roughness            Roughness of this Solid
///
/// @details  Constructs this Solid Properties by specifying values for each attribute. Also serves
///           as the default constructor since a default value is specified for each argument.
////////////////////////////////////////////////////////////////////////////////////////////////////
SolidProperties::SolidProperties(const SolidProperties::SolidType type,
                                 const double                     density,
                                 const double                     specificHeat,
                                 const double                     thermalConductivity,
                                 const double                     roughness)
    :
    mType(type),
    mDensity(density),
    mSpecificHeat(specificHeat),
    mThermalConductivity(thermalConductivity),
    mRoughness(roughness)
{
    // nothing left to do
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs a fluid properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
SolidProperties::~SolidProperties()
{
    // nothing to do
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Predefined Solid Properties. Initializes the array of solid
///           properties, indexed by solid type, specifying:
///           - Type of solid (enumeration of alloy types)
///           - Density (kg/m3)
///           - Specific heat (J/kg/K)
///           - Thermal conductivity (W/m/K)
///           - Roughness (m)
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedSolidProperties::DefinedSolidProperties()
    :
    mPropertiesSteel304(SolidProperties::STEEL_304,
                        7910.0,
                        490.0,
                        14.75,
                        2.13360E-06),
    mPropertiesAluminum6061(SolidProperties::ALUMINUM_6061,
                            2712.55219,
                            879.249,
                            173.10461,
                            3.81E-07),
    mProperties()
{
    /// - Load this Solid properties array with the pointer to the properties for each solid type
    mProperties[SolidProperties::STEEL_304]     = &mPropertiesSteel304;
    mProperties[SolidProperties::ALUMINUM_6061] = &mPropertiesAluminum6061;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Defined Solid Properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
DefinedSolidProperties::~DefinedSolidProperties()
{
    // nothing to do
}

