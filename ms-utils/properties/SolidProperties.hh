#ifndef SolidProperties_EXISTS
#define SolidProperties_EXISTS
/**
@defgroup  TSM_UTILITIES_PROPERTIES_SOLID  Solid Properties
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling the properties of a solid.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- (The constructors are protected, so only those Solid Properties objects in its friend,
   the Predefined Solid Properties class are available.)

 CLASS:
- ()

 LIBRARY DEPENDENCY:
- ((SolidProperties.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Update) (2011-09))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Solid Properties.
///
/// @details  The Solid Properties Model provides the properties of a solid: type, density,
///           specific heat, thermal conductivity and roughness.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SolidProperties {
    TS_MAKE_SIM_COMPATIBLE(SolidProperties);
    public:
        friend class DefinedSolidProperties;
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief    Enumeration of the types of Solids.
        ///
        /// @note     The last value is used to both define the length of the array and provide an
        ///           undefined Solid type.
        ///
        /// @details  This enumeration is used to index the array of Solid Properties.
        ////////////////////////////////////////////////////////////////////////////////////////////
        enum SolidType {
            STEEL_304         = 0,  ///<  steel 304
            ALUMINUM_6061     = 1,  ///<  aluminum 6061
            NO_SOLID          = 2   ///<  Invalid or number of solids - Keep this last!
        };
        /// @brief    Default constructs a Solid Properties.
        SolidProperties(const SolidProperties::SolidType  type = SolidProperties::NO_SOLID,
                        const double       density             = 0.0,
                        const double       specificHeat        = 0.0,
                        const double       thermalConductivity = 0.0,
                        const double       roughness           = 0.0);
        /// @brief    Default destructs a Solid Properties.
        virtual ~SolidProperties();
        /// @brief    Returns the type of this Solid.
        SolidProperties::SolidType getType() const;
        /// @brief    Returns the density (kg/m3) of this Solid.
        double getDensity() const;
        /// @brief    Returns the specific heat (J/kg/K) of this Solid.
        double getSpecificHeat() const;
        /// @brief    Returns the thermal conductivity (W/m/K) of this Solid.
        double getThermalConductivity() const;
        /// @brief    Returns the roughness (m) of this Solid.
        double getRoughness() const;
    protected:
        /// @brief    --       Type of this Solid.
        const SolidProperties::SolidType  mType;                // --       type
        /// @brief    (kg/m3)  Density of this Solid.
        const double                      mDensity;             // (kg/m3)  density
        /// @brief    (J/kg/K) Specific heat of this Solid.
        const double                      mSpecificHeat;        // (J/kg/K) specific heat
        /// @brief    (W/m/K)  Thermal conductivity of this Solid.
        const double                      mThermalConductivity; // (W/m/K)  thermal conductivity
        /// @brief    (m)      Roughness of this Solid.
        const double                      mRoughness;           // (m)      roughness
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        SolidProperties(const SolidProperties&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        SolidProperties& operator =(const SolidProperties&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Defined Solid Properties.
///
/// @details  The Defined Solid Properties model defines the Solid Properties for a set of solids:
///           Stainless Steel 304 and Aluminum 6061.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DefinedSolidProperties {
    TS_MAKE_SIM_COMPATIBLE(DefinedSolidProperties);
    public:
        /// @brief    Default constructs a Solid Properties.
        DefinedSolidProperties();
        /// @brief    Default destructs a Solid Properties.
        virtual ~DefinedSolidProperties();
        /// @brief    Returns a pointer to the properties of the specified Solid Type.
        SolidProperties* getProperties(const SolidProperties::SolidType type) const;
    protected:
        /// @brief    --  Steel 304 solid properties.
        SolidProperties   mPropertiesSteel304;                  // -- Steel 304 solid properties
        /// @brief    --  Aluminum 6061 solid properties.
        SolidProperties   mPropertiesAluminum6061;              // -- Aluminum 6061 solid properties
        /// @brief    --  Array of pointers to the defined Solid Properties.
        SolidProperties*  mProperties[SolidProperties::NO_SOLID]; // -- Solid properties array
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        DefinedSolidProperties(const DefinedSolidProperties&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        DefinedSolidProperties& operator =(const DefinedSolidProperties&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The SolidType (chemical composition or name enumeration) of this Solid
///
/// @details  Returns the type of this Solid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline SolidProperties::SolidType SolidProperties::getType() const
{
    return mType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The density (kg/m3) of this Solid
///
/// @details  Returns the density of this Solid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SolidProperties::getDensity() const
{
    return mDensity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The specific heat (J/kg/K) of this Solid
///
/// @details  Returns the specific heat of this Solid .
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SolidProperties::getSpecificHeat() const
{
    return mSpecificHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The thermal conductivity (W/m/K) of this Solid.
///
/// @details  Returns the thermal conductivity of this Solid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SolidProperties::getThermalConductivity() const
{
    return mThermalConductivity;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   The roughness (m) of this Solid.
///
/// @details  Returns the roughness of this Solid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double SolidProperties::getRoughness() const
{
    return mRoughness;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]    type   Type of Solid
///
/// @return   A pointer to the properties of the specified solid or null if undefined.
///
/// @details  Returns a pointer to the properties of the solid specified by solid type.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline SolidProperties* DefinedSolidProperties::getProperties(
                                                        const SolidProperties::SolidType type) const
{
    SolidProperties* result = 0;

    if (0 <= type && type < SolidProperties::NO_SOLID) {
        result =  mProperties[type];
    }

    return result;
}

#endif
