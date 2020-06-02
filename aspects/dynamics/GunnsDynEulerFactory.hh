#ifndef GunnsDynEulerFactory_EXISTS
#define GunnsDynEulerFactory_EXISTS

/**
@file
@brief    GUNNS Dynamics Euler Angles Factory declarations

@defgroup  TSM_GUNNS_DYNAMICS_EULER_FACTORY  GUNNS Dynamics Euler Angles Factory Class
@ingroup   TSM_GUNNS_DYNAMICS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (Refer to class details.)

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ()

LIBRARY_DEPENDENCY:
  (GunnsDynEulerFactory.o)

PROGRAMMERS:
  ((Jason Harvey) (CACI) (December 2016) (Initial))
@{
*/

#include <vector>

// Forward class declarations for pointer types.
class GunnsDynEulerBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  GUNNS Dynamics Euler Angles Factory
///
/// @details  This is the factory class for creating Euler sequence objects of a given type.  It
///           creates new objects of the desired type and returns a pointer to the new object.  It
///           also holds pointers to the objects so it can delete them during destruction.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDynEulerFactory
{
    public:
        /// @brief Defined euler rotation sequences.
        enum Sequences{
            NONE = 0,
            XYZ  = 123,        ///< Roll, Pitch, Yaw
            XZY  = 132,        ///< Roll, Yaw, Pitch
            YXZ  = 213,        ///< Pitch, Roll, Yaw
            YZX  = 231,        ///< Pitch, Yaw, Roll
            ZXY  = 312,        ///< Yaw, Roll, Pitch
            ZYX  = 321         ///< Yaw, Pitch, Roll
        };
        /// @brief Default constructor.
        GunnsDynEulerFactory();
        /// @brief Default destructor.
        virtual ~GunnsDynEulerFactory();
        /// @brief Returns a pointer to a new Euler sequence object with the given Euler sequence.
        GunnsDynEulerBase* create(const GunnsDynEulerFactory::Sequences sequence);

    private:
        std::vector<GunnsDynEulerBase*> mCreatedObjects; /**< ** (--) trick_chkpnt_io(**) Container of created objects for later deletion. */
        /// @brief Copy constructor is unavailable since private and not implemented.
        GunnsDynEulerFactory(const GunnsDynEulerFactory& that);
        /// @brief Assignment operator is unavailable since private and not implemented.
        GunnsDynEulerFactory& operator =(const GunnsDynEulerFactory& that);
};

/// @}

#endif
