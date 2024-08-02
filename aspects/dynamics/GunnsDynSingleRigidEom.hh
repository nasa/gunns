#ifndef GunnsDynSingleRigidEom_EXISTS
#define GunnsDynSingleRigidEom_EXISTS

/**
@file
@brief    GUNNS Dynamics Single Rigid-Body Equations of Motion declarations

@defgroup  TSM_GUNNS_DYNAMICS_SINGLE_RIGID_EOM  GUNNS Dynamics Single Rigid-Body Equations of Motion
@ingroup   TSM_GUNNS_DYNAMICS

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (This implements the 6-DOF EOM for a single rigid body, as described in Section 2 of the reference
   noted below.  Applied forces & torques, as well as body mass properties including a center of
   mass offset from the body reference frame, can be dynamic during run-time.

   This model owns the body state, state derivative, and mass properties objects.  All are public to
   allow interfacing as needed, such as a dynamic mass properties computation.

   This has two main functions that the simulation should call during run:
   - update(), called in a Trick derivative job or equivalent.
   - integ(),  called in a Trick integration job or equivalent.  This should be called after update
               in the cycle.  Note that a non-Trick simulation must implement its own integrator
               (Runga-Kutta, etc).
  )

REFERENCE:
  ("Adv. Dynamics", Tushar K. Ghosh, June 2016)

ASSUMPTIONS AND LIMITATIONS:
  ()

LIBRARY_DEPENDENCY:
  (GunnsDynSingleRigidEom.o)

PROGRAMMERS:
  (
   ((Jason Harvey) (CACI) (December 2016) (Initial))
  )
@{
*/

#include "aspects/dynamics/GunnsDynSingleRigidStateDeriv.hh"
#include "aspects/dynamics/GunnsDynEulerFactory.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Dynamics Single Rigid-Body Equations of Motion Input Data
///
/// @details  This class defines the input data needed to initialize a GUNNS Dynamics Single
///           Rigid-Body Equations of Motion.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDynSingleRigidEomInputData
{
    public:
        double                          mPosition[3];    /**< (m)     trick_chkpnt_io(**) Initial position vector of body frame in inertial frame, expressed in inertial frame. */
        double                          mVelocity[3];    /**< (m/s)   trick_chkpnt_io(**) Initial velocity vector of body frame in inertial frame, expressed in inertial frame. */
        GunnsDynEulerFactory::Sequences mEulerSequence;  /**< (--)    trick_chkpnt_io(**) Euler rotation sequence to use. */
        double                          mEulerAngles[3]; /**< (rad)   trick_chkpnt_io(**) Initial Euler rotations of the body frame in the inertial frame, in the given sequence. */
        double                          mAngVelocity[3]; /**< (rad/s) trick_chkpnt_io(**) Initial angular velocity vector of body frame in inertial frame, expressed in body frame. */
        GunnsDynSingleRigidMassProps    mMassProps;      /**< (--)    trick_chkpnt_io(**) Initial body mass properties. */
        /// @brief Default constructor.
        GunnsDynSingleRigidEomInputData(const double*                         position      = 0,
                                        const double*                         velocity      = 0,
                                        const GunnsDynEulerFactory::Sequences eulerSequence = GunnsDynEulerFactory::NONE,
                                        const double*                         eulerAngles   = 0,
                                        const double*                         angVelocity   = 0,
                                        const GunnsDynSingleRigidMassProps*   massProps     = 0);
        /// @brief Default destructor.
        virtual ~GunnsDynSingleRigidEomInputData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsDynSingleRigidEomInputData(const GunnsDynSingleRigidEomInputData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsDynSingleRigidEomInputData& operator =(const GunnsDynSingleRigidEomInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Dynamics Single Rigid-Body Equations of Motion.
///
/// @details  Refer to class Purpose.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDynSingleRigidEom
{
    TS_MAKE_SIM_COMPATIBLE(GunnsDynSingleRigidEom);

    public:
        GunnsDynSingleRigidState      mState;        /**<    (--)                     Body state. */
        GunnsDynSingleRigidMassProps  mMassProps;    /**<    (--)                     Body mass properties. */
        GunnsDynSingleRigidStateDeriv mStateDeriv;   /**<    (--)                     Body state derivatives. */
        /// @brief Default constructor.
        GunnsDynSingleRigidEom();
        /// @brief Default destructor.
        virtual ~GunnsDynSingleRigidEom();
        /// @brief Initializes this Dynamics Single Rigid-Body Equations of Motion with input data.
        void initialize(const std::string&                     name,
                        const GunnsDynSingleRigidEomInputData& inputData);
        /// @brief Updates the state derivative.
        void update();
        /// @brief Updates the state.
        int  integ();

    protected:
        std::string                   mName;         /**< *o (--) trick_chkpnt_io(**) Instance name for error messaging. */
        bool                          mInitFlag;     /**< *o (--) trick_chkpnt_io(**) Initialization complete flag. */
        GunnsDynEulerFactory          mEulerFactory; /**< ** (--) trick_chkpnt_io(**) Factory to create the Euler sequence object. */
        /// @brief Validates the input data.
        void validate(const GunnsDynSingleRigidEomInputData& inputData) const;

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsDynSingleRigidEom(const GunnsDynSingleRigidEom& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsDynSingleRigidEom& operator =(const GunnsDynSingleRigidEom& that);
};

/// @}

#endif
