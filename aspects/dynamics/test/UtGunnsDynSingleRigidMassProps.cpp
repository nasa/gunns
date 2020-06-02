/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include "software/exceptions/TsOutOfBoundsException.hh"
#include "strings/UtResult.hh"
#include <cmath>

#include "UtGunnsDynSingleRigidMassProps.hh"

/// @details  Test identification number.
int UtGunnsDynSingleRigidMassProps::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this GUNNS Single Rigid-Body Mass Properties unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynSingleRigidMassProps::UtGunnsDynSingleRigidMassProps()
    :
    CppUnit::TestFixture(),
    tMass(),
    tCM(),
    tIxx(),
    tIxy(),
    tIxz(),
    tIyy(),
    tIyz(),
    tIzz(),
    tArticle(0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this GUNNS Single Rigid-Body Mass Properties unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsDynSingleRigidMassProps::~UtGunnsDynSingleRigidMassProps()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidMassProps::setUp()
{
    /// - Define a nominal state.
    tMass  =  1.0e6;
    tCM[0] = -1.0;
    tCM[1] =  2.0;
    tCM[2] = -3.0;
    tIxx   =  1.0e7;
    tIxy   = -2.0e4;
    tIxz   = -3.0e4;
    tIyy   =  4.0e7;
    tIyz   = -5.0e4;
    tIzz   =  6.0e7;

    /// - Construct the test article with nominal state.
    tArticle = new GunnsDynSingleRigidMassProps(tMass, tCM, tIxx, tIxy, tIxz, tIyy, tIyz, tIzz);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidMassProps::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body Mass Properties default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidMassProps::testDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// @test Default constructed class data.
    GunnsDynSingleRigidMassProps article;
    CPPUNIT_ASSERT(0.0    == article.mMass);
    CPPUNIT_ASSERT(0.0    == article.mCM[0]);
    CPPUNIT_ASSERT(0.0    == article.mCM[1]);
    CPPUNIT_ASSERT(0.0    == article.mCM[2]);
    CPPUNIT_ASSERT(0.0    == article.mIxx);
    CPPUNIT_ASSERT(0.0    == article.mIxy);
    CPPUNIT_ASSERT(0.0    == article.mIxz);
    CPPUNIT_ASSERT(0.0    == article.mIyy);
    CPPUNIT_ASSERT(0.0    == article.mIyz);
    CPPUNIT_ASSERT(0.0    == article.mIzz);

    /// @test Nominally constructed class data.
    CPPUNIT_ASSERT(tMass  == tArticle->mMass);
    CPPUNIT_ASSERT(tCM[0] == tArticle->mCM[0]);
    CPPUNIT_ASSERT(tCM[1] == tArticle->mCM[1]);
    CPPUNIT_ASSERT(tCM[2] == tArticle->mCM[2]);
    CPPUNIT_ASSERT(tIxx   == tArticle->mIxx);
    CPPUNIT_ASSERT(tIxy   == tArticle->mIxy);
    CPPUNIT_ASSERT(tIxz   == tArticle->mIxz);
    CPPUNIT_ASSERT(tIyy   == tArticle->mIyy);
    CPPUNIT_ASSERT(tIyz   == tArticle->mIyz);
    CPPUNIT_ASSERT(tIzz   == tArticle->mIzz);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body Mass Properties copy construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidMassProps::testCopyConstruction()
{
    UT_RESULT;

    /// @test Copy constructed class data.
    GunnsDynSingleRigidMassProps copy(*tArticle);
    CPPUNIT_ASSERT(tMass  == copy.mMass);
    CPPUNIT_ASSERT(tCM[0] == copy.mCM[0]);
    CPPUNIT_ASSERT(tCM[1] == copy.mCM[1]);
    CPPUNIT_ASSERT(tCM[2] == copy.mCM[2]);
    CPPUNIT_ASSERT(tIxx   == copy.mIxx);
    CPPUNIT_ASSERT(tIxy   == copy.mIxy);
    CPPUNIT_ASSERT(tIxz   == copy.mIxz);
    CPPUNIT_ASSERT(tIyy   == copy.mIyy);
    CPPUNIT_ASSERT(tIyz   == copy.mIyz);
    CPPUNIT_ASSERT(tIzz   == copy.mIzz);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body Mass Properties assignment operator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidMassProps::testAssignmentOperator()
{
    UT_RESULT;

    /// - Default construct an empty test article.
    GunnsDynSingleRigidMassProps article;

    /// @test Assignment of the test article by assignment operator.
    article = *tArticle;
    CPPUNIT_ASSERT(tMass  == article.mMass);
    CPPUNIT_ASSERT(tCM[0] == article.mCM[0]);
    CPPUNIT_ASSERT(tCM[1] == article.mCM[1]);
    CPPUNIT_ASSERT(tCM[2] == article.mCM[2]);
    CPPUNIT_ASSERT(tIxx   == article.mIxx);
    CPPUNIT_ASSERT(tIxy   == article.mIxy);
    CPPUNIT_ASSERT(tIxz   == article.mIxz);
    CPPUNIT_ASSERT(tIyy   == article.mIyy);
    CPPUNIT_ASSERT(tIyz   == article.mIyz);
    CPPUNIT_ASSERT(tIzz   == article.mIzz);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body Mass Properties equals operator.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidMassProps::testEqualsOperator()
{
    UT_RESULT;

    /// - Construct two unequal test articles.
    GunnsDynSingleRigidMassProps article1(*tArticle);
    GunnsDynSingleRigidMassProps article2;

    /// @test Equals operator for all terms unequal.
    CPPUNIT_ASSERT(!(article1 == article2));

    /// @test Equals operator for various but not all terms unequal.
    article2.mMass  = article1.mMass;
    CPPUNIT_ASSERT(!(article1 == article2));

    article2.mCM[0] = article1.mCM[0];
    CPPUNIT_ASSERT(!(article1 == article2));

    article2.mCM[1] = article1.mCM[1];
    CPPUNIT_ASSERT(!(article1 == article2));

    article2.mCM[2] = article1.mCM[2];
    CPPUNIT_ASSERT(!(article1 == article2));

    article2.mIxx  = article1.mIxx;
    CPPUNIT_ASSERT(!(article1 == article2));

    article2.mIxy  = article1.mIxy;
    CPPUNIT_ASSERT(!(article1 == article2));

    article2.mIxz  = article1.mIxz;
    CPPUNIT_ASSERT(!(article1 == article2));

    article2.mIyy  = article1.mIyy;
    CPPUNIT_ASSERT(!(article1 == article2));

    article2.mIyz  = article1.mIyz;
    CPPUNIT_ASSERT(!(article1 == article2));

    /// @test Equals operator for all terms equal.
    article2.mIzz  = article1.mIzz;
    CPPUNIT_ASSERT(article1 == article2);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for GUNNS Single Rigid-Body Mass Properties validate method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsDynSingleRigidMassProps::testValidate()
{
    UT_RESULT;

    /// @test Validate method for a nominal article.
    CPPUNIT_ASSERT_NO_THROW(tArticle->validate());

    /// @test Article throws exception for mass < DBL_EPSILON.
    tArticle->mMass = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsOutOfBoundsException);
    tArticle->mMass = DBL_EPSILON;

    /// @test Article throws exception for Ixx < DBL_EPSILON.
    tArticle->mIxx  = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsOutOfBoundsException);
    tArticle->mIxx  = DBL_EPSILON;

    /// @test Article throws exception for Iyy < DBL_EPSILON.
    tArticle->mIyy  = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsOutOfBoundsException);
    tArticle->mIyy  = DBL_EPSILON;

    /// @test Article throws exception for Izz < DBL_EPSILON.
    tArticle->mIzz  = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsOutOfBoundsException);
    tArticle->mIzz  = DBL_EPSILON;

    /// @test Article throws exception for Ixy > 0.
    tArticle->mIxy  = 1.0e-100;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsOutOfBoundsException);
    tArticle->mIxy  = 0.0;

    /// @test Article throws exception for Ixz > 0.
    tArticle->mIxz  = 1.0e-100;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsOutOfBoundsException);
    tArticle->mIxz  = 0.0;

    /// @test Article throws exception for Iyz > 0.
    tArticle->mIyz  = 1.0e-100;
    CPPUNIT_ASSERT_THROW(tArticle->validate(), TsOutOfBoundsException);
    tArticle->mIyz  = 0.0;

    /// @test No throw for all terms at valid limits.
    CPPUNIT_ASSERT_NO_THROW(tArticle->validate());

    UT_PASS_LAST;
}
