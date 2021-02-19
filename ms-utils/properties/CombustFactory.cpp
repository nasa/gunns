/*
@file
@brief Combustion Factory Implementation.

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
     ((properties/CombustFactory.o)
      (properties/Combust.o)
      (properties/CombustCH4.o))
 **************************************************************************************************/

#include <iostream>
#include "CombustFactory.hh"
#include "Combust.hh"
#include "CombustCH4.hh"

CombustFactory::CombustFactory()
:
mCreatedCombustors(0)
{
    // Do Nothing
}

CombustFactory::~CombustFactory()
{
    for(unsigned int i = 0; i < mCreatedCombustors.size(); i++){
        delete mCreatedCombustors[i] ;
        mCreatedCombustors[i] = 0 ;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      CombustFactory::CombustorType   (--)  Desired combustion model type.
///
/// @return         Combust*                        (--)  Pointer to combustion model.
///
/// @details  Creates a combustion model, and returns a pointer to that combustion model.
////////////////////////////////////////////////////////////////////////////////////////////////////

Combust* CombustFactory::createCombustor(CombustFactory::CombustorType Type){
    switch(Type){
    case CombustFactory::NONE :
        return 0 ;
        break ;
    case CombustFactory::CH4 :
        CombustCH4* combustor = new CombustCH4() ;
        mCreatedCombustors.push_back(combustor) ;
        return combustor ;
        break ;
    }
    return 0 ;
}
