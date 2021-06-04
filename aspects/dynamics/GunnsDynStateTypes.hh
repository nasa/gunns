#ifndef GunnsDynStateTypes_EXISTS
#define GunnsDynStateTypes_EXISTS

/**
@file
@brief    GUNNS Dynamics State Types declarations

@defgroup  TSM_GUNNS_DYNAMICS_STATE_TYPES  GUNNS Dynamics State Types
@ingroup   TSM_GUNNS_DYNAMICS

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (This file defines various structures for holding dynamics states, forces & torques.)

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ()

LIBRARY_DEPENDENCY:
  ()

PROGRAMMERS:
  (
   ((Jason Harvey) (CACI) (January 2017) (Initial))
  )
@{
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Single Rigid-Body EOM Translational State.
///
/// @details  This struct contains position and velocity vectors of a single rigid-body
///           translational state.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsDynTranslationalState
{
        double mPosition[3];     /**< (m)     Position vector of body frame in inertial frame, expressed in inertial frame. */
        double mVelocity[3];     /**< (m/s)   Velocity vector of body frame in inertial frame, expressed in inertial frame. */
};

// Forward declaration for pointer type below:
class GunnsDynEulerBase;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Single Rigid-Body EOM Rotational State.
///
/// @details  This struct contains attitude and attitude rate properties of a single rigid-body
///           rotational state.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsDynRotationalState
{
        GunnsDynEulerBase* mEuler;           /**< (--)    Pointer to an Euler sequence object. */
        double             mQuaternion[4];   /**< (--)    Inertial to body frame quaternion. */
        double             mTransMatrix[9];  /**< (--)    Inertial to body frame transformation matrix. */
        double             mVelocityMag;     /**< (rad/s) Angular velocity magnitude. */
        double             mVelocity[3];     /**< (rad/s) Angular velocity vector of body frame in inertial frame, expressed in body frame. */
        double             mVelocityUnit[3]; /**< (--)    Angular velocity unit vector of body frame in inertial frame, expressed in body frame. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Single Rigid-Body EOM State.
///
/// @details  This struct contains a translation and rotation state for a single rigid-body EOM.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsDynSingleRigidState
{
        GunnsDynTranslationalState mTranslational; /**< (--) Translational state. */
        GunnsDynRotationalState    mRotational;    /**< (--) Rotational state. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Single Rigid-Body EOM Applied Force.
///
/// @details  This struct contains force & location vectors and an activation flag.  This is the
///           user interface for applying forces to the body.  Instances of this type can be created
///           by the user and registered with the GunnsDynSingleRigidStateDeriv.  Then the user can
///           update the force and its active flag during runtime as desired.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsDynAppliedForce
{
        bool   mActive;      /**< (--) Force is included in EOM if true. */
        double mForce[3];    /**< (N)  Force vector, expressed in body frame. */
        double mLocation[3]; /**< (m)  Position of force application in body frame, expressed in body frame. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Single Rigid-Body EOM Applied Torque.
///
/// @details  This struct contains a torque vector and an activation flag.  This is the user
///           interface for applying torques to the body.  Instances of this type can be created by
///           the user and registered with the GunnsDynSingleRigidStateDeriv.  Then the user can
///           update the torque and its active flag during runtime as desired.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsDynAppliedTorque
{
        bool   mActive;    /**< (--)  Torque is included in EOM if true. */
        double mTorque[3]; /**< (N*m) Torque vector, expressed in body frame. */
};

/// @}

#endif
