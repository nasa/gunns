/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((properties/SolidProperties.o))
***************************************************************************************************/

#include <iostream>

#include "strings/UtResult.hh"

#include "UtSolidProperties.hh"

/// @details  Test identification number.
int UtSolidProperties::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Solid Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSolidProperties::UtSolidProperties()
    :
    CppUnit::TestFixture(),
    mType(SolidProperties::NO_SOLID),
    mDensity(0.0),
    mSpecificHeat(0.0),
    mThermalConductivity(0.0),
    mRoughness(0.0),
    mArticle(0),
    mDefined(0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Solid Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtSolidProperties::~UtSolidProperties()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolidProperties::setUp()
{
    /// - Construct a Solid Properties test article.
    mType                = SolidProperties::ALUMINUM_6061;
    mDensity             = 1.0;
    mSpecificHeat        = 2.0;
    mThermalConductivity = 3.0;
    mRoughness           = 4.0;
    mArticle             = new FriendlySolidProperties(mType,
                                                       mDensity,
                                                       mSpecificHeat,
                                                       mThermalConductivity,
                                                       mRoughness);

    /// - Default construct the Defined Solid Properties.
    mDefined             = new DefinedSolidProperties();


    /// - Define the nominal comparison tolerance.
    mTolerance           = 0.001;

    /// - Increment the test identification number.
    ++TEST_ID;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolidProperties::tearDown()
{
    /// - Deletes for news (in reverse order) in setUp.
    delete mDefined;
    delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Solid Properties model default construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolidProperties::testPropertyDefaultConstruction()
{
    UT_RESULT_FIRST;

    /// - Default construct a test article.
    FriendlySolidProperties article;

    /// @test    Individual default properties.
    CPPUNIT_ASSERT(SolidProperties::NO_SOLID == article.mType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mDensity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mSpecificHeat, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mThermalConductivity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, article.mRoughness, mTolerance);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Solid Properties model construction with arguments.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolidProperties::testPropertyConstruction()
{
    UT_RESULT;

    /// @test    Individual argument properties.
    CPPUNIT_ASSERT(mType == mArticle->mType);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mDensity, mArticle->mDensity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSpecificHeat, mArticle->mSpecificHeat, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalConductivity, mArticle->mThermalConductivity, mTolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRoughness, mArticle->mRoughness, mTolerance);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Solid Properties model accessors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolidProperties::testPropertyAccessors()
{
    UT_RESULT;

    /// @test    Get type.
    CPPUNIT_ASSERT(mType == mArticle->getType());

    /// @test    Get density.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mDensity,
                                 mArticle->getDensity(),
                                 mTolerance);

    /// @test    Get specific heat.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mSpecificHeat,
                                 mArticle->getSpecificHeat(),
                                 mTolerance);

    /// @test    Get thermal conductivity.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mThermalConductivity,
                                 mArticle->getThermalConductivity(),
                                 mTolerance);

    /// @test    Get roughness.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(mRoughness,
                                 mArticle->getRoughness(),
                                 mTolerance);

    UT_PASS;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for Defined Solid Properties model.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtSolidProperties::testDefined()
{
    UT_RESULT;

    /// @test    Steel 304.
    CPPUNIT_ASSERT(SolidProperties::STEEL_304     ==
                   mDefined->getProperties(SolidProperties::STEEL_304)->getType());

    /// @test    Aluminum 6061.
    CPPUNIT_ASSERT(SolidProperties::ALUMINUM_6061 ==
                   mDefined->getProperties(SolidProperties::ALUMINUM_6061)->getType());

    /// @test    Undefined type.
    CPPUNIT_ASSERT(0 == mDefined->getProperties(SolidProperties::NO_SOLID));

    /// @test    Invalid type.
    CPPUNIT_ASSERT(0 == mDefined->getProperties(static_cast<SolidProperties::SolidType>(-1)));

    UT_PASS_LAST;
}
