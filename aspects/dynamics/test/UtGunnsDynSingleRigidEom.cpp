/**
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
 ((aspects/dynamics/GunnsDynSingleRigidEom.o))
*/

//#include "software/exceptions/TsInitializationException.hh"
//#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"
#include <cmath>

#include "UtGunnsDynSingleRigidEom.hh"

/// @details  Test identification number.
int UtGunnsDynSingleRigidEom::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Single Rigid-Body EOM unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynSingleRigidEom::UtGunnsDynSingleRigidEom()
    :
    CppUnit::TestFixture(),
    tAppliedForce(),
    tInputData(0),
    tArticle(0),
    tName("")
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Single Rigid-Body EOM unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynSingleRigidEom::~UtGunnsDynSingleRigidEom()
{
    // nothing to do
}

double gState[13];        /**< Global state values for global integrator functions. */
double gDeriv[13];        /**< Global state derivative values for global integrator functions. */
double gTimeStep = 0.001; /**< Global integration time step for global integrator functions. */
int    gIntStep  = 0;     /**< Global integration step counter for global integrator functions. */

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] p0 (--) Pointer to position state 0th element.
/// @param[in] p1 (--) Pointer to position state 1st element.
/// @param[in] p2 (--) Pointer to position state 2nd element.
/// @param[in] v0 (--) Pointer to velocity state 0th element.
/// @param[in] v1 (--) Pointer to velocity state 1st element.
/// @param[in] v2 (--) Pointer to velocity state 2nd element.
/// @param[in] q0 (--) Pointer to quaternion state 0th element.
/// @param[in] q1 (--) Pointer to quaternion state 1st element.
/// @param[in] q2 (--) Pointer to quaternion state 2nd element.
/// @param[in] q3 (--) Pointer to quaternion state 3rd element.
/// @param[in] w0 (--) Pointer to angular velocity state 0th element.
/// @param[in] w1 (--) Pointer to angular velocity state 1st element.
/// @param[in] w2 (--) Pointer to angular velocity state 2nd element.
///
/// @brief    Integrator load state global method.
///
/// @details  This takes the place of Trick infrastructure since we don't use Trick in our tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
void load_state(double* p0, double* p1, double* p2,
                double* v0, double* v1, double* v2,
                double* q0, double* q1, double* q2, double* q3,
                double* w0, double* w1, double* w2, ...)
{
    gState[ 0] = *p0;
    gState[ 1] = *p1;
    gState[ 2] = *p2;
    gState[ 3] = *v0;
    gState[ 4] = *v1;
    gState[ 5] = *v2;
    gState[ 6] = *q0;
    gState[ 7] = *q1;
    gState[ 8] = *q2;
    gState[ 9] = *q3;
    gState[10] = *w0;
    gState[11] = *w1;
    gState[12] = *w2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] p0 (--) Pointer to position state derivative 0th element.
/// @param[in] p1 (--) Pointer to position state derivative 1st element.
/// @param[in] p2 (--) Pointer to position state derivative 2nd element.
/// @param[in] v0 (--) Pointer to velocity state derivative 0th element.
/// @param[in] v1 (--) Pointer to velocity state derivative 1st element.
/// @param[in] v2 (--) Pointer to velocity state derivative 2nd element.
/// @param[in] q0 (--) Pointer to quaternion state derivative 0th element.
/// @param[in] q1 (--) Pointer to quaternion state derivative 1st element.
/// @param[in] q2 (--) Pointer to quaternion state derivative 2nd element.
/// @param[in] q3 (--) Pointer to quaternion state derivative 3rd element.
/// @param[in] w0 (--) Pointer to angular velocity state derivative 0th element.
/// @param[in] w1 (--) Pointer to angular velocity state derivative 1st element.
/// @param[in] w2 (--) Pointer to angular velocity state derivative 2nd element.
///
/// @brief    Integrator load state derivative global method.
///
/// @details  This takes the place of Trick infrastructure since we don't use Trick in our tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
void load_deriv(double* p0, double* p1, double* p2,
                double* v0, double* v1, double* v2,
                double* q0, double* q1, double* q2, double* q3,
                double* w0, double* w1, double* w2, ...)
{
    gDeriv[ 0] = *p0;
    gDeriv[ 1] = *p1;
    gDeriv[ 2] = *p2;
    gDeriv[ 3] = *v0;
    gDeriv[ 4] = *v1;
    gDeriv[ 5] = *v2;
    gDeriv[ 6] = *q0;
    gDeriv[ 7] = *q1;
    gDeriv[ 8] = *q2;
    gDeriv[ 9] = *q3;
    gDeriv[10] = *w0;
    gDeriv[11] = *w1;
    gDeriv[12] = *w2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Integrator integrate global method.
///
/// @returns  int (--) Integration step counter.
///
/// @details  This takes the place of Trick infrastructure since we don't use Trick in our tests.
///           This implements an Euler integration.
////////////////////////////////////////////////////////////////////////////////////////////////////
int integrate()
{
    gIntStep += 1;
    for (unsigned int i=0; i<13; ++i) {
        gState[i] += gDeriv[i] * gTimeStep;
    }
    return gIntStep;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[out] p0 (--) Pointer to position state 0th element.
/// @param[out] p1 (--) Pointer to position state 1st element.
/// @param[out] p2 (--) Pointer to position state 2nd element.
/// @param[out] v0 (--) Pointer to velocity state 0th element.
/// @param[out] v1 (--) Pointer to velocity state 1st element.
/// @param[out] v2 (--) Pointer to velocity state 2nd element.
/// @param[out] q0 (--) Pointer to quaternion state 0th element.
/// @param[out] q1 (--) Pointer to quaternion state 1st element.
/// @param[out] q2 (--) Pointer to quaternion state 2nd element.
/// @param[out] q3 (--) Pointer to quaternion state 3rd element.
/// @param[out] w0 (--) Pointer to angular velocity state 0th element.
/// @param[out] w1 (--) Pointer to angular velocity state 1st element.
/// @param[out] w2 (--) Pointer to angular velocity state 2nd element.
///
/// @brief    Integrator unload state global method.
///
/// @details  This takes the place of Trick infrastructure since we don't use Trick in our tests.
////////////////////////////////////////////////////////////////////////////////////////////////////
void unload_state(double* p0, double* p1, double* p2,
                  double* v0, double* v1, double* v2,
                  double* q0, double* q1, double* q2, double* q3,
                  double* w0, double* w1, double* w2, ...)
{
    *p0 = gState[ 0];
    *p1 = gState[ 1];
    *p2 = gState[ 2];
    *v0 = gState[ 3];
    *v1 = gState[ 4];
    *v2 = gState[ 5];
    *q0 = gState[ 6];
    *q1 = gState[ 7];
    *q2 = gState[ 8];
    *q3 = gState[ 9];
    *w0 = gState[10];
    *w1 = gState[11];
    *w2 = gState[12];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::setUp()
{
    tInputData = new GunnsDynSingleRigidEomInputData();

    /// - Define initial state.
    tInputData->mPosition[0]    =  0.0;
    tInputData->mPosition[1]    =  0.0;
    tInputData->mPosition[2]    =  0.0;
    tInputData->mVelocity[0]    =  0.0;
    tInputData->mVelocity[1]    =  0.0;
    tInputData->mVelocity[2]    =  0.0;
    tInputData->mEulerSequence  =  GunnsDynEulerFactory::XYZ;
    tInputData->mEulerAngles[0] =  0.0;
    tInputData->mEulerAngles[1] =  0.0;
    tInputData->mEulerAngles[2] =  0.0;
    tInputData->mAngVelocity[0] =  0.0;
    tInputData->mAngVelocity[1] =  0.0;
    tInputData->mAngVelocity[2] =  0.0;

    /// - Define initial mass properties.
    tInputData->mMassProps.mMass  =  1.0e6;
    tInputData->mMassProps.mCM[0] =  0.0;
    tInputData->mMassProps.mCM[1] =  0.0;
    tInputData->mMassProps.mCM[2] =  0.0;
    tInputData->mMassProps.mIxx   =  1.0e7;
    tInputData->mMassProps.mIxy   = -2.0e4;
    tInputData->mMassProps.mIxz   = -3.0e4;
    tInputData->mMassProps.mIyy   =  4.0e7;
    tInputData->mMassProps.mIyz   = -5.0e4;
    tInputData->mMassProps.mIzz   =  6.0e7;

    /// - Define initial applied force.
    tAppliedForce.mActive      = true;
    tAppliedForce.mForce[0]    =    0.0;
    tAppliedForce.mForce[1]    = 3000.0;
    tAppliedForce.mForce[2]    =    0.0;
    tAppliedForce.mLocation[0] =    0.0;
    tAppliedForce.mLocation[1] =    0.0;
    tAppliedForce.mLocation[2] =    0.0;

    /// - Default construct the nominal test article.
    tArticle = new GunnsDynSingleRigidEom();
    tName    = "tArticle";

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
    delete tInputData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testDefaultConstruction()
{
    UT_RESULT_FIRST;

//    /// @test    Default class data.
//    CPPUNIT_ASSERT(""                    ==  tArticle->mName);
//    CPPUNIT_ASSERT(&tRotationalState     == &tArticle->mRotationalState);
//    CPPUNIT_ASSERT(&tMassProps           == &tArticle->mMassProps);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedForce[0]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedForce[1]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedForce[2]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedTorque[0]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedTorque[1]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAppliedTorque[2]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroForce[0]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroForce[1]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroForce[2]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroTorque[0]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroTorque[1]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mGyroTorque[2]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAngMomentum[0]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAngMomentum[1]);
//    CPPUNIT_ASSERT(0.0                   ==  tArticle->mAngMomentum[2]);
//    CPPUNIT_ASSERT(7                     ==  tArticle->mDummyNodeList.mNumNodes);
//    CPPUNIT_ASSERT(tArticle->mDummyNodes ==  tArticle->mDummyNodeList.mNodes);
//    CPPUNIT_ASSERT(0                     ==  tArticle->mLinks.size());
//    CPPUNIT_ASSERT(0                     ==  tArticle->mAppliedForces.size());
//    CPPUNIT_ASSERT(0                     ==  tArticle->mAppliedTorques.size());
//
//    /// @test    Default construction initialization flag.
//    CPPUNIT_ASSERT(!tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName, *tInputData));

//    /// @test    Nominal base class init.
//    CPPUNIT_ASSERT(tName                     == tArticle->mName);
//    CPPUNIT_ASSERT(tArticle->mDummyNodes     == tArticle->mNodes[0]);
//    CPPUNIT_ASSERT(&tArticle->mDummyNodeList == tArticle->mNodeList);
//    CPPUNIT_ASSERT(6                         == tArticle->mNumPorts);
//    CPPUNIT_ASSERT(0                         == tArticle->mNodeMap[0]);
//    CPPUNIT_ASSERT(1                         == tArticle->mNodeMap[1]);
//    CPPUNIT_ASSERT(2                         == tArticle->mNodeMap[2]);
//
//    /// @test    Nominal state data.
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedForce[0]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedForce[1]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedForce[2]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedTorque[0]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedTorque[1]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAppliedTorque[2]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroForce[0]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroForce[1]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroForce[2]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroTorque[0]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroTorque[1]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mGyroTorque[2]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAngMomentum[0]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAngMomentum[1]);
//    CPPUNIT_ASSERT(0.0                       == tArticle->mAngMomentum[2]);
//
//    /// @test    Nominal initialization flag.
//    CPPUNIT_ASSERT(tArticle->mInitFlag);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM initialization exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testInitializationExceptions()
{
    UT_RESULT;

    // nothing to do

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM restart method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testRestart()
{
    UT_RESULT;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM addAppliedForce and
///           addAppliedTorque methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testApplyForcesTorques()
{
    UT_RESULT;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testUpdate()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName, *tInputData));
    tArticle->mStateDeriv.addAppliedForce(&tAppliedForce);

    CPPUNIT_ASSERT(true == tArticle->mStateDeriv.isInitialized());

    for (int i=0; i<100; ++i) {
        tArticle->update();
        tArticle->integ();
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM update method with an offset of
///           the center of mass in the body reference frame.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testUpdateCmOffset()
{
    UT_RESULT;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body EOM update method exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testUpdateExceptions()
{
    UT_RESULT;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testCrossV()
{
    UT_RESULT;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidEom::testSkew()
{
    UT_RESULT;

    UT_PASS_LAST;
}
