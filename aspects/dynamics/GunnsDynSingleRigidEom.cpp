/**
@file
@brief    GUNNS Dynamics Single Rigid-Body Equations of Motion implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (aspects/dynamics/GunnsDynUtils.o)
     (aspects/dynamics/GunnsDynEulerBase.o)
     (software/exceptions/TsInitializationException.o)
    )
*/

#include "GunnsDynSingleRigidEom.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"
#include "aspects/dynamics/GunnsDynEulerBase.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] position      (m)     Pointer to initial position vector of body frame in inertial frame, expressed in inertial frame.
/// @param[in] velocity      (m/s)   Pointer to initial velocity vector of body frame in inertial frame, expressed in inertial frame.
/// @param[in] eulerSequence (--)    Euler rotation sequence to use.
/// @param[in] eulerAngles   (rad)   Pointer to initial Euler rotations of the body frame in the inertial frame, in the given sequence.
/// @param[in] angVelocity   (rad/s) Pointer to initial angular velocity vector of body frame in inertial frame, expressed in body frame.
/// @param[in] massProps     (--)    Pointer to initial body mass properties.
///
/// @details  Constructs this input data with optional arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidEomInputData::GunnsDynSingleRigidEomInputData(
        const double*                         position,
        const double*                         velocity,
        const GunnsDynEulerFactory::Sequences eulerSequence,
        const double*                         eulerAngles,
        const double*                         angVelocity,
        const GunnsDynSingleRigidMassProps*   massProps)
    :
    mPosition(),
    mVelocity(),
    mEulerSequence(eulerSequence),
    mEulerAngles(),
    mAngVelocity(),
    mMassProps()
{
    if (position) {
        GunnsDynUtils::setV(mPosition,    position,    3);
    } else {
        GunnsDynUtils::zeroV(mPosition, 3);
    }
    if (velocity) {
        GunnsDynUtils::setV(mVelocity,    velocity,    3);
    } else {
        GunnsDynUtils::zeroV(mVelocity, 3);
    }
    if (eulerAngles) {
        GunnsDynUtils::setV(mEulerAngles, eulerAngles, 3);
    } else {
        GunnsDynUtils::zeroV(mEulerAngles, 3);
    }
    if (angVelocity) {
        GunnsDynUtils::setV(mAngVelocity, angVelocity, 3);
    } else {
        GunnsDynUtils::zeroV(mAngVelocity, 3);
    }
    if (massProps) {
        mMassProps = *massProps;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidEomInputData::~GunnsDynSingleRigidEomInputData()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Dynamics Single Rigid-Body Equations of Motion.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidEom::GunnsDynSingleRigidEom()
    :
    mState(),
    mMassProps(),
    mStateDeriv(mState.mRotational, mMassProps),
    mName(""),
    mInitFlag(false),
    mEulerFactory()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Dynamics Single Rigid-Body Equations of Motion.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidEom::~GunnsDynSingleRigidEom()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name      (--) Instance name for error messaging.
/// @param[in] inputData (--) Input data.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this GUNNS Dynamics Single Rigid-Body Equations of Motion.  This should be
///           called with input data before calling any other functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidEom::initialize(const std::string&                     name,
                                        const GunnsDynSingleRigidEomInputData& inputData)
{
    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize instance name.
    GUNNS_NAME_ERREX("GunnsDynSingleRigidEom", name);

    /// - Initialize state from input data.
    validate(inputData);
    mMassProps = inputData.mMassProps;
    GunnsDynUtils::setV(mState.mTranslational.mPosition, inputData.mPosition,    3);
    GunnsDynUtils::setV(mState.mTranslational.mVelocity, inputData.mVelocity,    3);
    try {
        mState.mRotational.mEuler = mEulerFactory.create(inputData.mEulerSequence);
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Error in Euler sequence creation.");
    }
    /// - Load initial Euler angles into the rotational state and initialize the quaternion.
    mState.mRotational.mEuler->computeQuat(mState.mRotational.mQuaternion, inputData.mEulerAngles);
    GunnsDynUtils::setV(mState.mRotational.mVelocity,    inputData.mAngVelocity, 3);

    /// - Initialize the transformation matrix from the quaternion.
    GunnsDynUtils::QtoM(mState.mRotational.mTransMatrix, mState.mRotational.mQuaternion);

    /// - Initialize the state derivative.
    mStateDeriv.initialize(mName + ".mStateDeriv");

    /// - Set init flag on successful completion.
    mInitFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] inputData (--) Input data.
///
/// @throws   TsInitializationException
///
/// @details  Validates the input data and throws a H&S error message and an exception on any
///           failures.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidEom::validate(const GunnsDynSingleRigidEomInputData& inputData) const
{
    /// - Catch & re-throw an exception if the mass properties fail validation.
    try {
        inputData.mMassProps.validate();
    } catch (...) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Error in initial mass properties.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Updates the state derivative.  This should be called from a Trick derivative job or
///           equivalent, before integ() in each cycle.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidEom::update()
{
    if (!mInitFlag) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Failed to update the uninitialized object.");
    }

    /// - Compute state derivative accelerations & Qdot.
    mStateDeriv.update();
}

#ifndef no_TRICK_ENV       // Trick
#include "sim_services/Integrator/include/integrator_c_intf.h"
#else                      // non-Trick
// Declare methods as externs, to be implemented elsewhere (e.g. a unit test program or a non-Trick
// sim environment).  Note that in C++ the 'extern' keyword is superflous since un-scoped methods
// default to the global namespace anyway.  This is just to make it more obvious what we're doing.
extern void load_state(double* p0, double* p1, double* p2,
                       double* v0, double* v1, double* v2,
                       double* q0, double* q1, double* q2, double* q3,
                       double* w0, double* w1, double* w2, ...);
extern void load_deriv(double* p0, double* p1, double* p2,
                       double* v0, double* v1, double* v2,
                       double* q0, double* q1, double* q2, double* q3,
                       double* w0, double* w1, double* w2, ...);
extern int  integrate();
extern void unload_state(double* p0, double* p1, double* p2,
                         double* v0, double* v1, double* v2,
                         double* q0, double* q1, double* q2, double* q3,
                         double* w0, double* w1, double* w2, ...);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Updates the state.  This should be called from a Trick integration job or equivalent,
///           after update() in each cycle.
////////////////////////////////////////////////////////////////////////////////////////////////////
int GunnsDynSingleRigidEom::integ()
{
    if (!mInitFlag) {
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "Failed to integrate the uninitialized object.");
    }

    /// - Load current states for the integrator.
    load_state(
            &mState.mTranslational.mPosition[0],
            &mState.mTranslational.mPosition[1],
            &mState.mTranslational.mPosition[2],
            &mState.mTranslational.mVelocity[0],
            &mState.mTranslational.mVelocity[1],
            &mState.mTranslational.mVelocity[2],
            &mState.mRotational.mQuaternion[0],
            &mState.mRotational.mQuaternion[1],
            &mState.mRotational.mQuaternion[2],
            &mState.mRotational.mQuaternion[3],
            &mState.mRotational.mVelocity[0],
            &mState.mRotational.mVelocity[1],
            &mState.mRotational.mVelocity[2],
            NULL);

    /// - Transform translational acceleration from body to inertial frame for integration.
    double inertialAccel[3];
    GunnsDynUtils::multiplyMtV(inertialAccel, mState.mRotational.mTransMatrix, mStateDeriv.getAcceleration(), 3);

    /// - Load current state derivatives for the integrator.
    double* qdot = mStateDeriv.getQdot();
    double* wdot = mStateDeriv.getAngAcceleration();
    load_deriv(
            &mState.mTranslational.mVelocity[0],
            &mState.mTranslational.mVelocity[1],
            &mState.mTranslational.mVelocity[2],
            &inertialAccel[0],
            &inertialAccel[1],
            &inertialAccel[2],
            &qdot[0],
            &qdot[1],
            &qdot[2],
            &qdot[3],
            &wdot[0],
            &wdot[1],
            &wdot[2],
            NULL);

    /// - Call the integrator function.
    int ipass = integrate();

    /// - Unload the integrated state from the integrator.
    unload_state(
            &mState.mTranslational.mPosition[0],
            &mState.mTranslational.mPosition[1],
            &mState.mTranslational.mPosition[2],
            &mState.mTranslational.mVelocity[0],
            &mState.mTranslational.mVelocity[1],
            &mState.mTranslational.mVelocity[2],
            &mState.mRotational.mQuaternion[0],
            &mState.mRotational.mQuaternion[1],
            &mState.mRotational.mQuaternion[2],
            &mState.mRotational.mQuaternion[3],
            &mState.mRotational.mVelocity[0],
            &mState.mRotational.mVelocity[1],
            &mState.mRotational.mVelocity[2],
            NULL);

    /// - Normalize the state quaternion and update the transformation matrix.
    GunnsDynUtils::normalizeQ(mState.mRotational.mQuaternion);
    GunnsDynUtils::QtoM(mState.mRotational.mTransMatrix, mState.mRotational.mQuaternion);

    /// - Update the state angular velocity magnitude & unit vector.
    mState.mRotational.mVelocityMag = GunnsDynUtils::magV(mState.mRotational.mVelocity, 3);
    if (mState.mRotational.mVelocityMag > DBL_EPSILON) {
        GunnsDynUtils::scaleV(mState.mRotational.mVelocityUnit, mState.mRotational.mVelocity, 1.0/mState.mRotational.mVelocityMag, 3);
    }

    /// - Update Euler angles from the quaternion.
    if (mState.mRotational.mEuler) {
        mState.mRotational.mEuler->update(mState.mRotational.mQuaternion);
    }

    return ipass;
}
