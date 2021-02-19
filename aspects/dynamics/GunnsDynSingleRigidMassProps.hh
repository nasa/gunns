#ifndef GunnsDynSingleRigidMassProps_EXISTS
#define GunnsDynSingleRigidMassProps_EXISTS

/**
@file
@brief    GUNNS Dynamics Single Rigid-Body EOM Mass Properties declarations

@defgroup  TSM_GUNNS_DYNAMICS_SINGLE_RIGID_MASS_PROPS  GUNNS Dynamics Single Rigid-Body EOM Mass Properties
@ingroup   TSM_GUNNS_DYNAMICS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (This defines the mass properties for a single rigid body, including mass, inertia tensor (as
   individual scalars), and a center of mass position vector in the body reference frame.)

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ((All vectors are expressed in the body reference frame.))

LIBRARY_DEPENDENCY:
  (GunnsDynSingleRigidMassProps.o)

PROGRAMMERS:
  (
   ((Jason Harvey) (CACI) (December 2016) (Initial))
  )
@{
*/

#include "aspects/dynamics/GunnsDynUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Dynamics Single Rigid-Body EOM Mass Properties.
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDynSingleRigidMassProps
{
    public:
        double mMass;  /**< (kg)    Body mass. */
        double mCM[3]; /**< (m)     Center of mass location in the body frame. */
        double mIxx;   /**< (kg*m2) Body X moment of inertia about the center of mass in the body frame. */
        double mIxy;   /**< (kg*m2) Body XY product of inertia about the center of mass in the body frame. */
        double mIxz;   /**< (kg*m2) Body XZ product of inertia about the center of mass in the body frame. */
        double mIyy;   /**< (kg*m2) Body Y moment of inertia about the center of mass in the body frame. */
        double mIyz;   /**< (kg*m2) Body YZ product of inertia about the center of mass in the body frame. */
        double mIzz;   /**< (kg*m2) Body Z moment of inertia about the center of mass in the body frame. */
        /// @brief Default constructor.
        GunnsDynSingleRigidMassProps(const double  mass = 0.0,
                                     const double* cm   = 0,
                                     const double  ixx  = 0.0,
                                     const double  ixy  = 0.0,
                                     const double  ixz  = 0.0,
                                     const double  iyy  = 0.0,
                                     const double  iyz  = 0.0,
                                     const double  izz  = 0.0);
        /// @brief Default destructor.
        virtual ~GunnsDynSingleRigidMassProps();
        /// @brief Copy constructor.
        GunnsDynSingleRigidMassProps(const GunnsDynSingleRigidMassProps& that);
        /// @brief Assignment operator.
        GunnsDynSingleRigidMassProps& operator =(const GunnsDynSingleRigidMassProps& that);
        /// @brief Custom == operator.
        bool operator ==(const GunnsDynSingleRigidMassProps& that) const;
        /// @brief Checks mass properties values for valid ranges.
        void validate() const;
        /// @brief Returns whether there is a CM offset in the body reference frame.
        bool isCmOffset() const;
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) True if the CM offset vector is non-zero, false otherwise.
///
/// @details  Returns true if the CM offset vector is non-zero, false otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsDynSingleRigidMassProps::isCmOffset() const
{
    return GunnsDynUtils::isNonZeroV(mCM, 3);
}

#endif
