/**
@file
@brief    GUNNS Dynamics Euler Angles Factory implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    ((aspects/dynamics/GunnsDynEuler123.o)
     (aspects/dynamics/GunnsDynEuler132.o)
     (aspects/dynamics/GunnsDynEuler231.o)
     (aspects/dynamics/GunnsDynEuler213.o)
     (aspects/dynamics/GunnsDynEuler312.o)
     (aspects/dynamics/GunnsDynEuler321.o)
     (software/exceptions/TsOutOfBoundsException.o))
*/

#include "GunnsDynEulerFactory.hh"
#include "aspects/dynamics/GunnsDynEuler123.hh"
#include "aspects/dynamics/GunnsDynEuler132.hh"
#include "aspects/dynamics/GunnsDynEuler231.hh"
#include "aspects/dynamics/GunnsDynEuler213.hh"
#include "aspects/dynamics/GunnsDynEuler312.hh"
#include "aspects/dynamics/GunnsDynEuler321.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Dynamics Euler Angles Factory.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynEulerFactory::GunnsDynEulerFactory()
    :
    mCreatedObjects()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Dynamics Euler Angles Factory.  Also deletes all objects
///           that this factory has created.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynEulerFactory::~GunnsDynEulerFactory()
{
    for (unsigned int i=0; i<mCreatedObjects.size(); ++i) {
        delete mCreatedObjects[i];
        mCreatedObjects[i] = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  sequence  (--)  The desired Euler rotation sequence, either as an enumeration (XYZ)
///                             or as an integer (123).
///
/// @throws   TsOutOfBoundsException
///
/// @returns  GunnsDynEulerBase* (--) Base class pointer to the desired Euler sequence type.
///
/// @details  Creates an Euler sequence object of the given sequence, returns a pointer to that
///           object, and stores the pointer in a vector for later use in the destructor.  Throws an
///           exception if an invalid sequence is given.
///
/// @note  The caller should not delete the actual object being pointed to, as this factory will
///        delete the actual object during its destruction.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynEulerBase* GunnsDynEulerFactory::create(const GunnsDynEulerFactory::Sequences sequence)
{
    GunnsDynEulerBase* object = 0;

    switch (sequence) {
        case GunnsDynEulerFactory::XYZ:
            object = new GunnsDynEuler123();
            break;

        case GunnsDynEulerFactory::XZY:
            object = new GunnsDynEuler132();
            break;

        case GunnsDynEulerFactory::YZX:
            object = new GunnsDynEuler231();
            break;

        case GunnsDynEulerFactory::YXZ:
            object = new GunnsDynEuler213();
            break;

        case GunnsDynEulerFactory::ZXY:
            object = new GunnsDynEuler312();
            break;

        case GunnsDynEulerFactory::ZYX:
            object = new GunnsDynEuler321();
            break;

        default:            // any invalid sequence
            throw TsOutOfBoundsException();
    }
    mCreatedObjects.push_back(object);
    return object;
}
