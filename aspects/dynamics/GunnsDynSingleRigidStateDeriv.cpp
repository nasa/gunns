/**
@file
@brief    GUNNS Dynamics Single Rigid-Body EOM State Derivative implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
    (
     (core/Gunns.o)
     (core/GunnsBasicLink.o)
     (aspects/dynamics/GunnsDynSingleRigidMassProps.o)
     (aspects/dynamics/GunnsDynUtils.o)
     (software/exceptions/TsInitializationException.o)
    )
*/

#include "GunnsDynSingleRigidStateDeriv.hh"
#include "aspects/dynamics/GunnsDynUtils.hh"
#include "software/exceptions/TsInitializationException.hh"
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] state     (--) Reference to body rotational state.
/// @param[in] massProps (--) Reference to body mass properties.
///
/// @details  Default constructs this Single Rigid-Body EOM State Derivative link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidStateDeriv::GunnsDynSingleRigidStateDeriv(
        GunnsDynRotationalState&            state,
        const GunnsDynSingleRigidMassProps& massProps)
    :
    GunnsBasicLink(NPORTS),
    mRotationalState(state),
    mMassProps(massProps),
    mAcceleration(),
    mAngAcceleration(),
    mQdot(),
    mAppliedForce(),
    mAppliedTorque(),
    mGyroForce(),
    mGyroTorque(),
    mAngMomentum(),
    mLastMassProps(),
    mSolver(),
    mDummyNodes(),
    mDummyNodeList(7, mDummyNodes),
    mLinks(),
    mAppliedForces(),
    mAppliedTorques()
{
    clearInternalState();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Destructs this Single Rigid-Body EOM State Derivative link.
////////////////////////////////////////////////////////////////////////////////////////////////////
GunnsDynSingleRigidStateDeriv::~GunnsDynSingleRigidStateDeriv()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] name (--) Reference to instance name for output messages.
///
/// @throws   TsInitializationException
///
/// @details  Initializes this Single Rigid-Body EOM State Derivative link.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidStateDeriv::initialize(const std::string& name)
{
    /// - Initialize dummy nodes for the base link class & solver.
    mDummyNodes[0].initialize(mName + ".mDummyNodes_0", 0.0);
    mDummyNodes[1].initialize(mName + ".mDummyNodes_1", 0.0);
    mDummyNodes[2].initialize(mName + ".mDummyNodes_2", 0.0);
    mDummyNodes[3].initialize(mName + ".mDummyNodes_3", 0.0);
    mDummyNodes[4].initialize(mName + ".mDummyNodes_4", 0.0);
    mDummyNodes[5].initialize(mName + ".mDummyNodes_5", 0.0);
    mDummyNodes[6].initialize(mName + ".mDummyNodes_6", 0.0);

    /// - Initialize the base link class.
    int ports[NPORTS] = {0, 1, 2, 3, 4, 5};
    GunnsBasicLinkConfigData configData(name, &mDummyNodeList);
    GunnsBasicLink::initialize(configData, GunnsBasicLinkInputData(), mLinks, ports);

    /// - Reset init flag.
    mInitFlag = false;

    /// - Initialize state data.
    clearInternalState();

    /// - Set init flag on successful initialization of internal solver.
    mSolver.initializeNodes(mDummyNodeList);
    GunnsConfigData solverConfig(mName + ".mSolver", 1.0, 1.0, 1, 1);
    try {
        mInitFlag = true;
        mSolver.initialize(solverConfig, mLinks);

        /// - Note that for our 6x6 mass matrix, the solver island mode should be SOLVE.  This is
        ///   because the solver conditions the admittance matrix differently between SOLVE &
        ///   non-SOLVE modes.  The non-SOLVE conditioning assumes that all off-diagonals are
        ///   negative, which is true for flow system aspects but not true for our the m[cm~]
        ///   sub-matrix of our mass matrix.  The SOLVE conditioning allows for positive or negative
        ///   values in the off-diagonals.
        /// - Being in SOLVE mode also makes it solve faster when some of the {cm} terms or products
        ///   of inertia are zero.
        mSolver.setIslandMode(Gunns::SOLVE);
    } catch (...) {
        mInitFlag = false;
        GUNNS_ERROR(TsInitializationException, "Invalid Initialization Data",
                "mSolver failed to initialize.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsOutOfBoundsException, TsNumericalException, TsInitializationException
///
/// @details  Validates the mass properties, computes total active forces & torques, uses the GUNNS
///           solver to compute new translational & angular accelerations, and updates the rate
///           quaternion.
///
///           This will throw exceptions if this object hasn't been initialized, or if given bad
///           mass properties.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidStateDeriv::update()
{
    mMassProps.validate();
    buildAdmittanceMatrix();
    buildSourceVector();

    /// - Call GUNNS solver to decompose the mass matrix and solve for new accelerations.
    ///   A fake timestep of 1 is given because solver wants > 0, otherwise doesn't matter.
    mSolver.step(1.0);
    GunnsDynUtils::setV(mAcceleration,    &mPotentialVector[0], 3);
    GunnsDynUtils::setV(mAngAcceleration, &mPotentialVector[3], 3);

    /// - Compute time derivative of rotational state quaternion.
    GunnsDynUtils::Qdot(mQdot, mRotationalState.mQuaternion, mRotationalState.mVelocity);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the link admittance matrix [A] for the GUNNS solver.  This is the 6x6 mass
///           matrix from Eqn. 2.5.5, with coupling between the translational and rotational DOF's
///           due to the CM offset in the body reference frame {cm}:
///           [A] = [  m[I]        -m[cm~]      ]   <-- 3 rows translational
///                 [ m[cm~]  [Icm]-m[cm~][cm~] ]   <-- 3 rows rotational
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidStateDeriv::buildAdmittanceMatrix()
{
    if (not (mMassProps == mLastMassProps)) {
        mLastMassProps    = mMassProps;
        mAdmittanceUpdate = true;
        GunnsDynUtils::zeroV(mAdmittanceMatrix, 36);

        // m*[I] 3x3:
        mAdmittanceMatrix[0]  = mMassProps.mMass;
        mAdmittanceMatrix[7]  = mMassProps.mMass;
        mAdmittanceMatrix[14] = mMassProps.mMass;

        // [Icm] 3x3:
        mAdmittanceMatrix[21] = mMassProps.mIxx;
        mAdmittanceMatrix[22] = mMassProps.mIxy;
        mAdmittanceMatrix[23] = mMassProps.mIxz;
        mAdmittanceMatrix[27] = mMassProps.mIxy;
        mAdmittanceMatrix[28] = mMassProps.mIyy;
        mAdmittanceMatrix[29] = mMassProps.mIyz;
        mAdmittanceMatrix[33] = mMassProps.mIxz;
        mAdmittanceMatrix[34] = mMassProps.mIyz;
        mAdmittanceMatrix[35] = mMassProps.mIzz;

        /// - When CM offset {cm} is non-zero, adjust the mass matrix by its effects.
        if (mMassProps.isCmOffset()) {
            double cmSkew[9], massCmSkew[9], massCmSkew2[9];
            GunnsDynUtils::skew(cmSkew, mMassProps.mCM);
            GunnsDynUtils::scaleV(massCmSkew, cmSkew, mMassProps.mMass, 9);
            GunnsDynUtils::multiplyMM(massCmSkew2, massCmSkew, cmSkew, 3);

            // m[cm~]~ 3x3:
            mAdmittanceMatrix[19] =  massCmSkew[1];
            mAdmittanceMatrix[20] =  massCmSkew[2];
            mAdmittanceMatrix[24] =  massCmSkew[3];
            mAdmittanceMatrix[26] =  massCmSkew[5];
            mAdmittanceMatrix[30] =  massCmSkew[6];
            mAdmittanceMatrix[31] =  massCmSkew[7];

            //                                T
            // -m[cm~] 3x3, and -[cm~] = [cm~] :
            mAdmittanceMatrix[4]  = -massCmSkew[1];
            mAdmittanceMatrix[5]  = -massCmSkew[2];
            mAdmittanceMatrix[9]  = -massCmSkew[3];
            mAdmittanceMatrix[11] = -massCmSkew[5];
            mAdmittanceMatrix[15] = -massCmSkew[6];
            mAdmittanceMatrix[16] = -massCmSkew[7];

            // [Icm] 3x3 shifted to body frame origin: Io = Icm - m[cm~][cm~]:
            mAdmittanceMatrix[21] -= massCmSkew2[0];
            mAdmittanceMatrix[22] -= massCmSkew2[1];
            mAdmittanceMatrix[23] -= massCmSkew2[2];
            mAdmittanceMatrix[27] -= massCmSkew2[3];
            mAdmittanceMatrix[28] -= massCmSkew2[4];
            mAdmittanceMatrix[29] -= massCmSkew2[5];
            mAdmittanceMatrix[33] -= massCmSkew2[6];
            mAdmittanceMatrix[34] -= massCmSkew2[7];
            mAdmittanceMatrix[35] -= massCmSkew2[8];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the link source vector for the GUNNS solver.  This includes all of the vector
///           terms in Eqn. 2.5.5, including applied & gyroscopic forces & torques.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidStateDeriv::buildSourceVector()
{
    sumAppliedForces();
    sumAppliedTorques();

    /// - Compute angular momentum & gyroscopic torque:
    ///   {angular_momentum} = [Inertia] * {angular_velocity}
    ///   {gyro_torque}      = {angular_velocity} X {angular_momentum}
    mAngMomentum[0] = mMassProps.mIxx * mRotationalState.mVelocity[0]
                    + mMassProps.mIxy * mRotationalState.mVelocity[1]
                    + mMassProps.mIxz * mRotationalState.mVelocity[2];
    mAngMomentum[1] = mMassProps.mIxy * mRotationalState.mVelocity[0]
                    + mMassProps.mIyy * mRotationalState.mVelocity[1]
                    + mMassProps.mIyz * mRotationalState.mVelocity[2];
    mAngMomentum[2] = mMassProps.mIxz * mRotationalState.mVelocity[0]
                    + mMassProps.mIyz * mRotationalState.mVelocity[1]
                    + mMassProps.mIzz * mRotationalState.mVelocity[2];
    GunnsDynUtils::crossV3(mGyroTorque, mRotationalState.mVelocity, mAngMomentum);

    /// - Build GUNNS source vector {w}:
    mSourceVector[0] = mAppliedForce[0];
    mSourceVector[1] = mAppliedForce[1];
    mSourceVector[2] = mAppliedForce[2];
    mSourceVector[3] = mAppliedTorque[0] - mGyroTorque[0];
    mSourceVector[4] = mAppliedTorque[1] - mGyroTorque[1];
    mSourceVector[5] = mAppliedTorque[2] - mGyroTorque[2];

    if (mMassProps.isCmOffset()) {
        /// - Compute gyroscopic pseudo-force due CM offset from the center of body reference frame:
        ///   {gyro_force} = m{o} X ({o} X {cm})
        GunnsDynUtils::crossV3(mGyroForce, mRotationalState.mVelocity, mMassProps.mCM);
        GunnsDynUtils::crossV3(mGyroForce, mRotationalState.mVelocity, mGyroForce);
        GunnsDynUtils::scaleV (mGyroForce, mGyroForce, mMassProps.mMass, 3);

        mSourceVector[0] -= mGyroForce[0];
        mSourceVector[1] -= mGyroForce[1];
        mSourceVector[2] -= mGyroForce[2];
    } else {
        GunnsDynUtils::zeroV(mGyroForce, 3);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Loops over all of the applied force objects, and for those that are active, adds their
///           force into the total mAppliedForce.  This does not include the torque from the forces'
///           offset from the center of mass.  That is included in sumAppliedTorques().
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidStateDeriv::sumAppliedForces()
{
    GunnsDynUtils::zeroV(mAppliedForce, 3);
    for (unsigned int i=0; i<mAppliedForces.size(); ++i) {
        if (mAppliedForces[i]->mActive) {
            GunnsDynUtils::addV(mAppliedForce, mAppliedForce, mAppliedForces[i]->mForce, 3);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Loops over all the applied torque objects, and for those that are active, adds their
///           torque into the total mAppliedTorque.  Also loops over active applied force objects
///           and adds their (location X force) torque.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidStateDeriv::sumAppliedTorques()
{
    GunnsDynUtils::zeroV(mAppliedTorque, 3);
    const bool isCmOffset = mMassProps.isCmOffset();
    for (unsigned int i=0; i<mAppliedForces.size(); ++i) {
        if (mAppliedForces[i]->mActive) {
            /// - Add applied force location crossed with the force.  The force locations are in the
            ///   body frame so we must move them to relative to the CM when there's a CM offset.
            double location[3];
            double torque[3];
            GunnsDynUtils::setV(location, mAppliedForces[i]->mLocation, 3);
            if (isCmOffset) {
                GunnsDynUtils::subtractV(location, location, mMassProps.mCM, 3);
            }
            GunnsDynUtils::crossV3(torque, location, mAppliedForces[i]->mForce);
            GunnsDynUtils::addV(mAppliedTorque, mAppliedTorque, torque, 3);
        }
    }

    /// - Add pure applied torques.
    for (unsigned int i=0; i<mAppliedTorques.size(); ++i) {
        if (mAppliedTorques[i]->mActive) {
            GunnsDynUtils::addV(mAppliedTorque, mAppliedTorque, mAppliedTorques[i]->mTorque, 3);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Derived classes should call their base class implementation too.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidStateDeriv::restartModel()
{
    /// - Note we cannot call the solver's restart method because it would re-call this method,
    ///   causing an infinite loop.  This is a side effect of us abusing the original GUNNS design.
    GunnsBasicLink::restartModel();
    clearInternalState();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes various internal arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////
void GunnsDynSingleRigidStateDeriv::clearInternalState()
{
    GunnsDynUtils::zeroV(mAcceleration,    3);
    GunnsDynUtils::zeroV(mAngAcceleration, 3);
    GunnsDynUtils::zeroV(mAppliedForce,    3);
    GunnsDynUtils::zeroV(mAppliedTorque,   3);
    GunnsDynUtils::zeroV(mGyroForce,       3);
    GunnsDynUtils::zeroV(mGyroTorque,      3);
    GunnsDynUtils::zeroV(mAngMomentum,     3);
    GunnsDynUtils::zeroV(mQdot,            4);
}
