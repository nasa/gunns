#ifndef CombustFactory_EXISTS
#define CombustFactory_EXISTS
/**
@defgroup  TSM_UTILITIES_PROPERTIES_COMBUSTFACTORY Combustion Model Factory
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the class to construct Combust class.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- (A combustion model type must be in the CombustorType list.)

 LIBRARY DEPENDENCY:
- ((CombustFactory.o)
   (Combust.o))

 PROGRAMMERS:
- ((Christopher Brownschidle) (CACI) (Initial) (2016-08))

@{
 */
#include "Combust.hh"
#include <vector>
////////////////////////////////////////////////////////////////////////////////////////////////////
///@brief       Combustion Model Factory
///
///@details     This model creates Combustion models. The program using these models calls
///             CombustFactory()'s sole method, and receives a pointer to the desired combustion model
////////////////////////////////////////////////////////////////////////////////////////////////////

class CombustFactory {
public:
    /// @brief Defined combustion model types.
    enum CombustorType{
        CH4     = 0,        ///< Methane + Oxygen (Helium purge gas) Combustion Model
        NONE    = 1,        ///< No dissociation combustion model used
    };
    /// @brief Default constructor.
    CombustFactory() ;
    /// @brief Default destructor
    virtual ~CombustFactory() ;
    /// @brief Method to create new combustion model, and return it's pointer
    Combust* createCombustor(CombustFactory::CombustorType Type) ;

private:
    std::vector<Combust*> mCreatedCombustors ; /**< (--)   trick_chkpnt_io(**)     Vector containing pointers to all created combustors, used for clearing memory */
};
/// @}
#endif /* CombustFactory_EXISTS */
