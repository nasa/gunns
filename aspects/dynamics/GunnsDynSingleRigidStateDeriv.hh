#ifndef GunnsDynSingleRigidStateDeriv_EXISTS
#define GunnsDynSingleRigidStateDeriv_EXISTS

/**
@file
@brief    GUNNS Dynamics Single Rigid-Body EOM State Derivative declarations

@defgroup  TSM_GUNNS_DYNAMICS_SINGLE_RIGID_STATE_DERIV  GUNNS Dynamics Single Rigid-Body EOM State Derivative
@ingroup   TSM_GUNNS_DYNAMICS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (This solves the 6-DOF equations of motion of a single rigid body for translational and rotational
   accelerations of the body reference frame. All vectors are expressed in the body reference frame.

   We are solving Eqn. 2.5.5 of the reference noted below for the accelerations.  This linear system
   of equations is of the same form as that solvable by GUNNS; hence this is a GUNNS implementation.
   This is a special GUNNS link that embeds its own GUNNS solver and nodes, so it is also its own
   GUNNS "network".

   This only performs the state derivative, and doesn't integrate the body state or do coordinate
   transformations to/from other reference frames.  We leave that to other models that may wrap
   this model as part of a total dynamics simulation.  In Trick simulations, this model's update
   function would be called inside a derivative job.

   The inputs to this are the body mass properties, angular velocity vector, and applied forces &
   torques.  This solution allows for an offset of the center of mass (CM) from the body reference
   frame origin.  All inputs can be dynamic and change during run-time, including the CM offset.)

REFERENCE:
  ("Adv. Dynamics", Tushar K. Ghosh, June 2016)

ASSUMPTIONS AND LIMITATIONS:
  ((All vectors are expressed in the body reference frame.))

LIBRARY_DEPENDENCY:
  (GunnsDynSingleRigidStateDeriv.o)

PROGRAMMERS:
  (
   ((Jason Harvey) (CACI) (December 2016) (Initial))
  )
@{
*/

#include "core/GunnsBasicLink.hh"
#include "core/Gunns.hh"
#include "aspects/dynamics/GunnsDynStateTypes.hh"
#include "aspects/dynamics/GunnsDynSingleRigidMassProps.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Single Rigid-Body EOM State Derivative Link.
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDynSingleRigidStateDeriv : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsDynSingleRigidStateDeriv);

    public:
        /// @brief Default Constructor.
        GunnsDynSingleRigidStateDeriv(GunnsDynRotationalState&            state,
                                      const GunnsDynSingleRigidMassProps& massProps);
        /// @brief Default Destructor.
        virtual ~GunnsDynSingleRigidStateDeriv();
        /// @brief Initialize method.
        void         initialize(const std::string& name);
        /// @brief Computes the state derivative.
        void         update();
        /// @brief Registers a force object with this state derivative.
        void         addAppliedForce(GunnsDynAppliedForce* force);
        /// @brief Registers a torque object with this state derivative.
        void         addAppliedTorque(GunnsDynAppliedTorque* torque);
        /// @brief Returns the acceleration vector.
        double*      getAcceleration();
        /// @brief Returns the angular acceleration vector.
        double*      getAngAcceleration();
        /// @brief Returns the quaternion derivative.
        double*      getQdot();

    protected:
        GunnsDynRotationalState&            mRotationalState;    /**< *o (--)      trick_chkpnt_io(**) Reference to body state. */
        const GunnsDynSingleRigidMassProps& mMassProps;          /**< *o (--)      trick_chkpnt_io(**) Reference to body mass properties. */
        double                              mAcceleration[3];    /**<    (m/s2)                        Output acceleration, expressed in body frame. */
        double                              mAngAcceleration[3]; /**<    (rad/s2)                      Output angular acceleration, expressed in body frame. */
        double                              mQdot[4];            /**<    (--)                          Output time derivative of inertial to body frame quaternion. */
        double                              mAppliedForce[3];    /**< *o (N)       trick_chkpnt_io(**) Total applied force, expressed in body frame. */
        double                              mAppliedTorque[3];   /**< *o (N*m)     trick_chkpnt_io(**) Total applied torques, expressed in body frame. */
        double                              mGyroForce[3];       /**< *o (N)       trick_chkpnt_io(**) Gyroscopic psuedo-force due to CM offset, expressed in body frame. */
        double                              mGyroTorque[3];      /**< *o (N*m)     trick_chkpnt_io(**) Gyroscopic torque, expressed in body frame. */
        double                              mAngMomentum[3];     /**< *o (kg*m2/s) trick_chkpnt_io(**) Angular momentum, expressed in body frame. */
        GunnsDynSingleRigidMassProps        mLastMassProps;      /**< *o (--)      trick_chkpnt_io(**) Last-pass body mass properties. */
        Gunns                               mSolver;             /**<    (--)                          GUNNS system of equations solver. */
        GunnsBasicNode                      mDummyNodes[7];      /**< ** (--)      trick_chkpnt_io(**) Dummy basic nodes for the solver. */
        GunnsNodeList                       mDummyNodeList;      /**< ** (--)      trick_chkpnt_io(**) Dummy nodes list for the solver. */
        std::vector<GunnsBasicLink*>        mLinks;              /**< ** (--)      trick_chkpnt_io(**) Link pointers vector for the solver. */
        std::vector<GunnsDynAppliedForce*>  mAppliedForces;      /**< ** (--)      trick_chkpnt_io(**) Vector of pointers to applied force objects. */
        std::vector<GunnsDynAppliedTorque*> mAppliedTorques;     /**< ** (--)      trick_chkpnt_io(**) Vector of pointers to applied torque objects. */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Zeroes various internal state arrays.
        void         clearInternalState();
        /// @brief Computes the total active applied force.
        void         sumAppliedForces();
        /// @brief Computes the total active applied torque.
        void         sumAppliedTorques();
        /// @brief Updates the link admittance matrix for network solution.
        void         buildAdmittanceMatrix();
        /// @brief Updates the link source vector for network solution.
        void         buildSourceVector();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 6};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsDynSingleRigidStateDeriv(const GunnsDynSingleRigidStateDeriv& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsDynSingleRigidStateDeriv& operator =(const GunnsDynSingleRigidStateDeriv& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] force (--) Pointer to the force object to add to this state derivative.
///
/// @details Pushes the given force object pointer onto this state derivative's container.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsDynSingleRigidStateDeriv::addAppliedForce(GunnsDynAppliedForce* force)
{
    mAppliedForces.push_back(force);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] torque (--) Pointer to the torque object to add to this state derivative.
///
/// @details Pushes the given torque object pointer onto this state derivative's container.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsDynSingleRigidStateDeriv::addAppliedTorque(GunnsDynAppliedTorque* torque)
{
    mAppliedTorques.push_back(torque);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double* (--) Pointer to this state's acceleration vector.
///
/// @details Returns a pointer to this state's acceleration vector expressed in body frame.  This
///          should be called after update().
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsDynSingleRigidStateDeriv::getAcceleration()
{
    return mAcceleration;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double* (--) Pointer to this state's angular acceleration vector.
///
/// @details Returns a pointer to this state derivative's angular acceleration vector expressed in
///          body frame.  This should be called after update().
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsDynSingleRigidStateDeriv::getAngAcceleration()
{
    return mAngAcceleration;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double* (--) Pointer to this state's quaternion derivative.
///
/// @details Returns a pointer to this state derivative's time derivative of inertial to body frame
///          quaternion.  This should be called after update().
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double* GunnsDynSingleRigidStateDeriv::getQdot()
{
    return mQdot;
}

#endif
