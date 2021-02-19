/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((aspects/dynamics/GunnsDynSingleRigidStateDeriv.o))
*/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include "software/exceptions/TsNumericalException.hh"
#include "strings/UtResult.hh"
#include <cmath>

#include "UtGunnsDynSingleRigidStateDeriv.hh"

/// @details  Test identification number.
int UtGunnsDynSingleRigidStateDeriv::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Single Rigid-Body EOM State Derivative unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynSingleRigidStateDeriv::UtGunnsDynSingleRigidStateDeriv()
    :
    CppUnit::TestFixture(),
    tRotationalState(),
    tMassProps(),
    tAppliedForces(),
    tAppliedTorques(),
    tArticle(0),
    tName()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Single Rigid-Body EOM State Derivative unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynSingleRigidStateDeriv::~UtGunnsDynSingleRigidStateDeriv()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::setUp()
{
    /// - Define initial state.
    for (int i=0; i<3; ++i) {
        tRotationalState.mVelocity[i] = 0.0;
    }

    /// - Define initial mass properties.
    tMassProps.mMass  =  1.0e6;
    tMassProps.mCM[0] =  0.0;
    tMassProps.mCM[1] =  0.0;
    tMassProps.mCM[2] =  0.0;
    tMassProps.mIxx   =  1.0e7;
    tMassProps.mIxy   = -2.0e4;
    tMassProps.mIxz   = -3.0e4;
    tMassProps.mIyy   =  4.0e7;
    tMassProps.mIyz   = -5.0e4;
    tMassProps.mIzz   =  6.0e7;

    /// - Define initial applied forces.
    tAppliedForces[0].mActive      = false;
    tAppliedForces[0].mForce[0]    = 3000.0;
    tAppliedForces[0].mForce[1]    = -200.0;
    tAppliedForces[0].mForce[2]    =   10.0;
    tAppliedForces[0].mLocation[0] =   -4.0;
    tAppliedForces[0].mLocation[1] =    5.0;
    tAppliedForces[0].mLocation[2] =   -6.0;

    tAppliedForces[1].mActive      = false;
    tAppliedForces[1].mForce[0]    = -900.0;
    tAppliedForces[1].mForce[1]    =   20.0;
    tAppliedForces[1].mForce[2]    = 1000.0;
    tAppliedForces[1].mLocation[0] =    1.0;
    tAppliedForces[1].mLocation[1] =   -0.1;
    tAppliedForces[1].mLocation[2] =    0.2;

    /// - Define initial applied torques.
    tAppliedTorques[0].mActive     = false;
    tAppliedTorques[0].mTorque[0]  =  100.0;
    tAppliedTorques[0].mTorque[1]  =  -20.0;
    tAppliedTorques[0].mTorque[2]  =    3.0;

    tAppliedTorques[1].mActive     = false;
    tAppliedTorques[1].mTorque[0]  = -100.0;
    tAppliedTorques[1].mTorque[1]  =   20.0;
    tAppliedTorques[1].mTorque[2]  =   -3.0;

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsDynSingleRigidStateDeriv(tRotationalState, tMassProps);
    tName    = "tArticle";

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test    Default class data.
    CPPUNIT_ASSERT(""                    ==  tArticle->mName);
    CPPUNIT_ASSERT(&tRotationalState     == &tArticle->mRotationalState);
    CPPUNIT_ASSERT(&tMassProps           == &tArticle->mMassProps);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedForce[0]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedForce[1]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedForce[2]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedTorque[0]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedTorque[1]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedTorque[2]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroForce[0]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroForce[1]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroForce[2]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroTorque[0]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroTorque[1]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroTorque[2]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAngMomentum[0]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAngMomentum[1]);
    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAngMomentum[2]);
    CPPUNIT_ASSERT(7                     ==  tArticle->mDummyNodeList.mNumNodes);
    CPPUNIT_ASSERT(tArticle->mDummyNodes ==  tArticle->mDummyNodeList.mNodes);
    CPPUNIT_ASSERT(0                     ==  tArticle->mLinks.size());
    CPPUNIT_ASSERT(0                     ==  tArticle->mAppliedForces.size());
    CPPUNIT_ASSERT(0                     ==  tArticle->mAppliedTorques.size());

    /// @test    Default construction initialization flag.
    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// @test    Nominal base class init.
    CPPUNIT_ASSERT(tName                     == tArticle->mName);
    CPPUNIT_ASSERT(tArticle->mDummyNodes     == tArticle->mNodes[0]);
    CPPUNIT_ASSERT(&tArticle->mDummyNodeList == tArticle->mNodeList);
    CPPUNIT_ASSERT(6                         == tArticle->mNumPorts);
    CPPUNIT_ASSERT(0                         == tArticle->mNodeMap[0]);
    CPPUNIT_ASSERT(1                         == tArticle->mNodeMap[1]);
    CPPUNIT_ASSERT(2                         == tArticle->mNodeMap[2]);

    /// @test    Nominal state data.
    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedForce[0]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedForce[1]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedForce[2]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedTorque[0]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedTorque[1]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedTorque[2]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroForce[0]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroForce[1]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroForce[2]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroTorque[0]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroTorque[1]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroTorque[2]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAngMomentum[0]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAngMomentum[1]);
    CPPUNIT_ASSERT(0.0                       == tArticle->mAngMomentum[2]);

    /// @test    Nominal initialization flag.
    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testInitializationExceptions()
{
    UT_RESULT;

    // nothing to do

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testRestart()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// - Set attributes to be reset during restart.
    tArticle->mAdmittanceUpdate = true;
    tArticle->mAppliedForce[0]  = 1.0;
    tArticle->mAppliedForce[1]  = 1.0;
    tArticle->mAppliedForce[2]  = 1.0;
    tArticle->mAppliedTorque[0] = 1.0;
    tArticle->mAppliedTorque[1] = 1.0;
    tArticle->mAppliedTorque[2] = 1.0;
    tArticle->mGyroTorque[0]    = 1.0;
    tArticle->mGyroTorque[1]    = 1.0;
    tArticle->mGyroTorque[2]    = 1.0;
    tArticle->mAngMomentum[0]   = 1.0;
    tArticle->mAngMomentum[1]   = 1.0;
    tArticle->mAngMomentum[2]   = 1.0;

    /// @test    Restart method.
    tArticle->restart();
    CPPUNIT_ASSERT(false == tArticle->mAdmittanceUpdate);
    CPPUNIT_ASSERT(0.0   == tArticle->mAppliedForce[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mAppliedForce[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mAppliedForce[2]);
    CPPUNIT_ASSERT(0.0   == tArticle->mAppliedTorque[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mAppliedTorque[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mAppliedTorque[2]);
    CPPUNIT_ASSERT(0.0   == tArticle->mGyroForce[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mGyroForce[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mGyroForce[2]);
    CPPUNIT_ASSERT(0.0   == tArticle->mGyroTorque[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mGyroTorque[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mGyroTorque[2]);
    CPPUNIT_ASSERT(0.0   == tArticle->mAngMomentum[0]);
    CPPUNIT_ASSERT(0.0   == tArticle->mAngMomentum[1]);
    CPPUNIT_ASSERT(0.0   == tArticle->mAngMomentum[2]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative addAppliedForce and
///           addAppliedTorque methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testApplyForcesTorques()
{
    UT_RESULT;

    /// @test addAppliedForce method.
    tArticle->addAppliedForce(&tAppliedForces[0]);
    tArticle->addAppliedForce(&tAppliedForces[1]);
    CPPUNIT_ASSERT(2                              == tArticle->mAppliedForces.size());
    CPPUNIT_ASSERT(tAppliedForces[0].mForce[0]    == tArticle->mAppliedForces[0]->mForce[0]);
    CPPUNIT_ASSERT(tAppliedForces[1].mLocation[2] == tArticle->mAppliedForces[1]->mLocation[2]);

    /// @test addAppliedTorque method.
    tArticle->addAppliedTorque(&tAppliedTorques[0]);
    tArticle->addAppliedTorque(&tAppliedTorques[1]);
    CPPUNIT_ASSERT(2                              == tArticle->mAppliedTorques.size());
    CPPUNIT_ASSERT(tAppliedTorques[0].mTorque[0]  == tArticle->mAppliedTorques[0]->mTorque[0]);
    CPPUNIT_ASSERT(tAppliedTorques[1].mTorque[2]  == tArticle->mAppliedTorques[1]->mTorque[2]);

    UT_PASS;
}

#include "aspects/dynamics/GunnsDynUtils.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testUpdate()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data, and add
    ///   applied force & torque objects.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));
    tArticle->addAppliedForce(&tAppliedForces[0]);
    tArticle->addAppliedForce(&tAppliedForces[1]);
    tArticle->addAppliedTorque(&tAppliedTorques[0]);
    tArticle->addAppliedTorque(&tAppliedTorques[1]);

    /// @test Update with no active forces & torques, no inertial torque.
    CPPUNIT_ASSERT_NO_THROW(tArticle->update());

    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[2]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAngAcceleration[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAngAcceleration[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAngAcceleration[2]);

    /// @test Update with no active forces & torques, but with inertial torque.
    tRotationalState.mVelocity[2] = 0.1;
    CPPUNIT_ASSERT_NO_THROW(tArticle->update());

    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[2]);

    //TODO need some example test cases to validate against.

    /// @test Update with no active forces & active torques that cancel out, with inertial torque.
    tAppliedTorques[0].mActive = true;
    tAppliedTorques[1].mActive = true;
    CPPUNIT_ASSERT_NO_THROW(tArticle->update());

    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[2]);

    /// @test Update with no active forces & net active torques, with inertial torque.
    tAppliedTorques[1].mActive = false;
    CPPUNIT_ASSERT_NO_THROW(tArticle->update());

    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[0]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[1]);
    CPPUNIT_ASSERT(0.0 == tArticle->mAcceleration[2]);

    /// @test Update with net active forces & net active torques, with inertial torque.
    tAppliedForces[0].mActive = true;
    tAppliedForces[1].mActive = true;
    CPPUNIT_ASSERT_NO_THROW(tArticle->update());

    double expectedAx = (tAppliedForces[0].mForce[0] + tAppliedForces[1].mForce[0]) / tMassProps.mMass;
    double expectedAy = (tAppliedForces[0].mForce[1] + tAppliedForces[1].mForce[1]) / tMassProps.mMass;
    double expectedAz = (tAppliedForces[0].mForce[2] + tAppliedForces[1].mForce[2]) / tMassProps.mMass;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAx, tArticle->mAcceleration[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAy, tArticle->mAcceleration[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedAz, tArticle->mAcceleration[2], DBL_EPSILON);

    /// @test F/m = a ==> F = ma.
    double FM[6];
    GunnsDynUtils::multiplyMV(FM, tArticle->mAdmittanceMatrix, tArticle->mPotentialVector, 6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[0], FM[0], 2.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[0]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[1], FM[1], 2.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[1]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[2], FM[2], 2.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[2]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[3], FM[3], 2.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[3]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[4], FM[4], 2.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[4]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[5], FM[5], 2.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[5]));

    // TODO test change to inertia matrix

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative update method with an offset of
///           the center of mass in the body reference frame.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testUpdateCmOffset()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data, and add
    ///   applied force & torque objects.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));
    tArticle->addAppliedForce(&tAppliedForces[0]);
    tArticle->addAppliedForce(&tAppliedForces[1]);
    tArticle->addAppliedTorque(&tAppliedTorques[0]);
    tArticle->addAppliedTorque(&tAppliedTorques[1]);

    /// - Give the CM an offset in the body reference frame.
    tMassProps.mCM[0] = 1.0;
    tRotationalState.mVelocity[1] = 0.01;

    double omegaXcm[3];
    double omegaXomegaXcm[3];
    GunnsDynUtils::crossV3(omegaXcm, tRotationalState.mVelocity, tMassProps.mCM);
    GunnsDynUtils::crossV3(omegaXomegaXcm, tRotationalState.mVelocity, omegaXcm);
    omegaXomegaXcm[0] *= tMassProps.mMass;
    omegaXomegaXcm[1] *= tMassProps.mMass;
    omegaXomegaXcm[2] *= tMassProps.mMass;

    tAppliedForces[0].mActive = true;
    tAppliedForces[1].mActive = true;
    tAppliedTorques[0].mActive = true;

    CPPUNIT_ASSERT_NO_THROW(tArticle->update());

    CPPUNIT_ASSERT_DOUBLES_EQUAL(omegaXomegaXcm[0], tArticle->mGyroForce[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(omegaXomegaXcm[1], tArticle->mGyroForce[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(omegaXomegaXcm[2], tArticle->mGyroForce[2], DBL_EPSILON);

    /// @test F/m = a ==> F = ma.
    double FM[6];
    GunnsDynUtils::multiplyMV(FM, tArticle->mAdmittanceMatrix, tArticle->mPotentialVector, 6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[0], FM[0], 10.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[0]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[1], FM[1], 10.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[1]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[2], FM[2], 10.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[2]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[3], FM[3], 10.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[3]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[4], FM[4], 10.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[4]));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tArticle->mSourceVector[5], FM[5], 10.0 * DBL_EPSILON * fabs(tArticle->mSourceVector[5]));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative update method with an offset of
///           the center of mass in the body reference frame.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testUpdateCmOffset2()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data, and add
    ///   applied force & torque objects.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// - Give the CM an offset in the body reference frame.
    tMassProps.mCM[0] = -10.0;

    CPPUNIT_ASSERT_NO_THROW(tArticle->update());

//    double* a = tArticle->mAdmittanceMatrix;
//    printf("\n [A] = %.0e, %.0e, %.0e, %.0e, %.0e, %.0e",   a[ 0], a[ 1], a[ 2], a[ 3], a[ 4], a[ 5]);
//    printf("\n       %.0e, %.0e, %.0e, %.0e, %.0e, %.0e",   a[ 6], a[ 7], a[ 8], a[ 9], a[10], a[11]);
//    printf("\n       %.0e, %.0e, %.0e, %.0e, %.0e, %.0e",   a[12], a[13], a[14], a[15], a[16], a[17]);
//    printf("\n       %.0e, %.0e, %.0e, %.0e, %.0e, %.0e",   a[18], a[19], a[20], a[21], a[22], a[23]);
//    printf("\n       %.0e, %.0e, %.0e, %.0e, %.0e, %.0e",   a[24], a[25], a[26], a[27], a[28], a[29]);
//    printf("\n       %.0e, %.0e, %.0e, %.0e, %.0e, %.0e\n", a[30], a[31], a[32], a[33], a[34], a[35]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM State Derivative update method exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testUpdateExceptions()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data, and add
    ///   applied force & torque objects.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));
    tArticle->addAppliedForce(&tAppliedForces[0]);
    tArticle->addAppliedForce(&tAppliedForces[1]);
    tArticle->addAppliedTorque(&tAppliedTorques[0]);
    tArticle->addAppliedTorque(&tAppliedTorques[1]);
    tAppliedForces[0].mActive = true;
    tAppliedForces[1].mActive = true;

    /// @test throw from the model given a bad inertia tensor.
    tMassProps.mIxy = 1.0e10;
    CPPUNIT_ASSERT_THROW(tArticle->update(), TsOutOfBoundsException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testCrossV()
{
    UT_RESULT;

    double x[8][3] = { 1,  2,  3,
                      -1,  2,  3,
                       1, -2,  3,
                       1,  2, -3,
                      -1, -2,  3,
                      -1,  2, -3,
                       1, -2, -3,
                      -1, -2, -3};
    double y[8][3] = { 4,  5,  6,
                      -4,  5,  6,
                       4, -5,  6,
                       4,  5, -6,
                      -4, -5,  6,
                      -4,  5, -6,
                       4, -5, -6,
                      -4, -5, -6};
    // expected values from https://www.wolframalpha.com/input...
    double e[8][3] = {-3,  6, -3,
                      -3, -6,  3,
                       3,  6,  3,
                       3, -6, -3,
                       3, -6, -3,
                       3,  6,  3,
                      -3, -6,  3,
                      -3,  6, -3};

    for (int i=0; i<8; ++i) {
        double a[3];
        GunnsDynUtils::crossV3(a, x[i], y[i]);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(a[0], e[i][0], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(a[1], e[i][1], DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(a[2], e[i][2], DBL_EPSILON);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidStateDeriv::testSkew()
{
    UT_RESULT;

    //TODO verify that {w} = {v} X {u} = [v~]{u}:

    double v[3] = {1.0e0, -2.0e-1, 3.0e-2};
    double u[3] = {4.0, 5.0, 6.0};
    double wCross[3];
    double wSkew[3];
    double vSkew[9];
    GunnsDynUtils::skew(vSkew, v);
    GunnsDynUtils::crossV3(wCross, v, u);
    GunnsDynUtils::multiplyMV(wSkew, vSkew, u, 3);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(wCross[0], wSkew[0], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(wCross[1], wSkew[1], DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(wCross[2], wSkew[2], DBL_EPSILON);

    UT_PASS_LAST;
}
