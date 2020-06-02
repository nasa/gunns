#ifndef MaterialProperties_EXISTS
#define MaterialProperties_EXISTS
/**
@defgroup  TSM_UTILITIES_PROPERTIES_MATERIAL Material Properties
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- ( Properties of a material )

 REFERENCE:
 - (none)

 ASSUMPTIONS AND LIMITATIONS:
 - ( SI units only )

 LIBRARY DEPENDENCY:
 -((MaterialProperties.o))

 PROGRAMMERS:
 -(
   ((Kevin Supak) (L-3 Titan) (Dec 2009) (Initial prototype))
   ((Chuck Sorensen) (LZT) (Jan 2011) (Convert to C++))
  )

@{
*/


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Provides the properties of a (solid) material.
///
/// @details
/// Properties include the Specific Heat Cp, Density, roughness and conductivity
/// An object exists for those materials used
/// Ex: steel304.specificHeatCp  is the Cp for the material
///
////////////////////////////////////////////////////////////////////////////////////////////////////
class MaterialProperties {

public:

    /// @brief Specific Heat capacity (Constant Pressure) (J/kg/K)
    const double specificHeatCp;       //  (J/kg/K)   Specific Heat Cp

    /// @brief thermal conductivity (W/m/K)
    const double thermalConductivity;  //  (W/m/K)    Conductivity

    /// @brief roughness (m)
    const double roughness;            //  (m)        Roughness

    /// @brief density (kg/M3)
    const double density;              //  (kg/m3)  Density

    /// @brief  standard constructor. Requires values for all attributes
    MaterialProperties(double cp = 0.0, double k = 0.0,
            double rough = 0.0, double rho = 0.0) :
                specificHeatCp(cp), thermalConductivity(k),
                roughness(rough), density(rho) {}

    /// @brief  standard destructor
    virtual ~MaterialProperties() {}

private:
    /// @brief keep private, never used
    MaterialProperties(const MaterialProperties &rhs);

    /// @brief keep private, never used
    MaterialProperties operator= (const MaterialProperties &rhs);
};

/// @}

#endif /* MaterialProperties_EXISTS */
