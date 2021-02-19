#ifndef PredefinedMaterialProperties_EXISTS
#define PredefinedMaterialProperties_EXISTS
/**
@defgroup TSM_UTILITIES_PROPERTIES_PREDEFINED_MATERIAL Predefined Material Properties
@ingroup  TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- ( Provide access to the predefined material properties )

 REFERENCE:
 - (none)

 ASSUMPTIONS AND LIMITATIONS:
 - ( )

 LIBRARY DEPENDENCY:
 -((PredefinedMaterialProperties.o))

 PROGRAMMERS:
 -(
   ((Chuck Sorensen) (LZT) (Mar 2011) (Initial Version))
  )
@{
*/

#include <map>
#include <string>

class MaterialProperties;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Provides the predefined material properties.
///
/// @details
/// The material properties are accessed by the name (string) of the material.
///
/// <b>For the list of predefined material properties see getMaterialProperties(name).</b>
/// @see getMaterialProperties(std::string name)
////////////////////////////////////////////////////////////////////////////////////////////////////
class PredefinedMaterialProperties {
public:
    static MaterialProperties *getMaterialProperties(std::string name);
    static int size();

private:
    /// @brief keep private
    PredefinedMaterialProperties();

    /// @brief keep private
    ~PredefinedMaterialProperties() {};

    /// @brief keep private, never used
    PredefinedMaterialProperties(const PredefinedMaterialProperties &rhs);

    /// @brief keep private, never used
    PredefinedMaterialProperties operator= (const PredefinedMaterialProperties &rhs);


    typedef std::map<std::string, MaterialProperties* > MaterialsMap;
    typedef MaterialsMap::iterator iterator;

    /// @brief map of material name vs material properties
    MaterialsMap materialsMap;                         // -- materials map

    /// @brief singleton instance of predefine material properties
    static PredefinedMaterialProperties theMaterials; //  -- instance


};

/// @}

#endif
