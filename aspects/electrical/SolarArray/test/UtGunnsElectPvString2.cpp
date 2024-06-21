/**
@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include "software/exceptions/TsInitializationException.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"
#include <iostream>
#include "strings/UtResult.hh"
#include "UtGunnsElectPvString2.hh"
#include "math/MsMath.hh"

/// @details  Test identification number.
int UtGunnsElectPvString2::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default constructor for the UtGunnsElectPvString2 class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvString2::UtGunnsElectPvString2()
    :
    tName(""),
    tConfigData(0),
    tInputData(0),
    tArticle(0),
    tCellSurfaceArea(0.0),
    tCellEfficiency(0.0),
    tCellSeriesResistance(0.0),
    tCellShuntResistance(0.0),
    tCellOpenCircuitVoltage(0.0),
    tCellRefTemperature(0.0),
    tCellTemperatureVoltageCoeff(0.0),
    tCellTemperatureCurrentCoeff(0.0),
    tCellShortCircuitCurrent(0.0),
    tCellMppVoltage(0.0),
    tCellMppCurrent(0.0),
    tCellPhotoFlux(0.0),
    tCellIdeality(0.0),
    tBlockingDiodeVoltageDrop(0.0),
    tBypassDiodeVoltageDrop(0.0),
    tBypassDiodeInterval(0),
    tNumCells(0),
    tPhotoFlux(0.0),
    tSourceExposedFraction(0.0),
    tTemperature(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This is the default destructor for the UtGunnsElectPvString2 class.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtGunnsElectPvString2::~UtGunnsElectPvString2()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::setUp()
{
    tName = "tArticle";

    /// - Define the nominal configuration data using the version 2 constructor.
    tCellOpenCircuitVoltage      =    0.6;
    tCellShortCircuitCurrent     =    2.6;
    tCellMppVoltage              =    0.5;
    tCellMppCurrent              =    2.4;
    tCellPhotoFlux               = 1361.0;
    tCellRefTemperature          =  300.0;
    tCellTemperatureVoltageCoeff =   -0.002;
    tCellTemperatureCurrentCoeff =    0.0014;
    tCellIdeality                =    1.0;
    tCellSurfaceArea             =    0.003;
    tBlockingDiodeVoltageDrop    =    0.7;
    tBypassDiodeVoltageDrop      =    0.5;
    tBypassDiodeInterval         =    5;
    tNumCells                    =   20;
    tConfigData                  = new GunnsElectPvStringConfigData(tBlockingDiodeVoltageDrop,
                                                                    tBypassDiodeVoltageDrop,
                                                                    tBypassDiodeInterval,
                                                                    tNumCells,
                                                                    tCellOpenCircuitVoltage,
                                                                    tCellShortCircuitCurrent,
                                                                    tCellMppVoltage,
                                                                    tCellMppCurrent,
                                                                    tCellPhotoFlux,
                                                                    tCellRefTemperature,
                                                                    tCellTemperatureVoltageCoeff,
                                                                    tCellTemperatureCurrentCoeff,
                                                                    tCellIdeality,
                                                                    tCellSurfaceArea);

    /// - Define the nominal input data.
    tPhotoFlux             = 1361.0;
    tSourceExposedFraction = 1.0;
    tTemperature           = 300.0;
    tInputData             = new GunnsElectPvStringInputData(tPhotoFlux,
                                                             tSourceExposedFraction,
                                                             tTemperature);

    /// - Default construct the nominal test article.
    tArticle = new FriendlyGunnsElectPvString2(tConfigData, tInputData);

    /// - Increment the test identification number.
    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::tearDown()
{
    /// - Deletes for news in setUp
    delete tArticle;
    delete tInputData;
    delete tConfigData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell version 2 configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testCellConfig()
{
    UT_RESULT_FIRST;

    /// @test    Configuration nominal construction.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             tConfigData->mCellConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellEfficiency,              0.0,                                               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSeriesResistance,        0.0,                                               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShuntResistance,         0.0,                                               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      tConfigData->mCellConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          tConfigData->mCellConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, tConfigData->mCellConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, tConfigData->mCellConfig.mTemperatureCurrentCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShortCircuitCurrent,     tConfigData->mCellConfig.mShortCircuitCurrent,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellMppVoltage,              tConfigData->mCellConfig.mMppVoltage,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellMppCurrent,              tConfigData->mCellConfig.mMppCurrent,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellPhotoFlux,               tConfigData->mCellConfig.mPhotoFlux,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellIdeality,                tConfigData->mCellConfig.mIdeality,                0.0);
    CPPUNIT_ASSERT(tConfigData->mCellConfig.isVersion2());

    /// @test    Configuration data default construction.
    GunnsElectPvCellConfigData defaultConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mTemperatureCurrentCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mShortCircuitCurrent,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mMppVoltage,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mMppCurrent,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mPhotoFlux,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, defaultConfig.mIdeality,                0.0);
    CPPUNIT_ASSERT(not defaultConfig.isVersion2());

    /// @test    Configuration data assignment operator.
    GunnsElectPvCellConfigData assignConfig;
    assignConfig = tConfigData->mCellConfig;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea,             assignConfig.mSurfaceArea,             0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellEfficiency,              assignConfig.mEfficiency,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSeriesResistance,        assignConfig.mSeriesResistance,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShuntResistance,         assignConfig.mShuntResistance,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellOpenCircuitVoltage,      assignConfig.mOpenCircuitVoltage,      0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellRefTemperature,          assignConfig.mRefTemperature,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureVoltageCoeff, assignConfig.mTemperatureVoltageCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellTemperatureCurrentCoeff, assignConfig.mTemperatureCurrentCoeff, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellShortCircuitCurrent,     assignConfig.mShortCircuitCurrent,     0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellMppVoltage,              assignConfig.mMppVoltage,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellMppCurrent,              assignConfig.mMppCurrent,              0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellPhotoFlux,               assignConfig.mPhotoFlux,               0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellIdeality,                assignConfig.mIdeality,                0.0);
    CPPUNIT_ASSERT(assignConfig.isVersion2());

    /// @test    Configuration data self assign.
    GunnsElectPvCellConfigData* assignConfig2 = &assignConfig;
    assignConfig = *assignConfig2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellSurfaceArea, assignConfig.mSurfaceArea, 0.0);

    /// @test    New/delete for code coverage.
    GunnsElectPvCellConfigData* testArticle = new GunnsElectPvCellConfigData();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell equivalent circuit version 2 model construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testCellEquivProps()
{
    UT_RESULT;

    /// @test    Cell equivalent data default construction.
    GunnsElectPvCellEquivCircuit2 cellEqCirc;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIL,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRsh,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRs,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mNVt,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mI0,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mFillFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mEfficiency,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVoc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIsc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVmp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mImp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mPhotoFlux,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIdeality,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDVocDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDIscDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mSurfaceArea, 0.0);

    /// @test    Clear method.
    cellEqCirc.mIL          = 1.0;
    cellEqCirc.mRsh         = 1.0;
    cellEqCirc.mRs          = 1.0;
    cellEqCirc.mNVt         = 1.0;
    cellEqCirc.mI0          = 1.0;
    cellEqCirc.mFillFactor  = 1.0;
    cellEqCirc.mEfficiency  = 1.0;
    cellEqCirc.mVoc         = 1.0;
    cellEqCirc.mIsc         = 1.0;
    cellEqCirc.mVmp         = 1.0;
    cellEqCirc.mImp         = 1.0;
    cellEqCirc.mPhotoFlux   = 1.0;
    cellEqCirc.mIdeality    = 1.0;
    cellEqCirc.mTemperature = 1.0;
    cellEqCirc.mCoeffDVocDT = 1.0;
    cellEqCirc.mCoeffDIscDT = 1.0;
    cellEqCirc.mSurfaceArea = 1.0;
    cellEqCirc.clear();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIL,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRsh,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRs,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mNVt,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mI0,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mFillFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mEfficiency,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVoc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIsc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVmp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mImp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mPhotoFlux,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIdeality,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDVocDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDIscDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mSurfaceArea, 0.0);

    /// @test    New/delete for code coverage.
    GunnsElectPvCellEquivCircuit2* testArticle = new GunnsElectPvCellEquivCircuit2();
    delete testArticle;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell equivalent circuit version 2 model nominal initialization.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testCellEquivInit()
{
    UT_RESULT;

    GunnsElectPvCellEquivCircuit2 cellEqCirc;
    CPPUNIT_ASSERT_NO_THROW(cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc"));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mOpenCircuitVoltage,      cellEqCirc.mVoc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mShortCircuitCurrent,     cellEqCirc.mIsc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mMppVoltage,              cellEqCirc.mVmp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mMppCurrent,              cellEqCirc.mImp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mPhotoFlux,               cellEqCirc.mPhotoFlux,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mIdeality,                cellEqCirc.mIdeality,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mRefTemperature,          cellEqCirc.mTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mTemperatureVoltageCoeff, cellEqCirc.mCoeffDVocDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mTemperatureCurrentCoeff, cellEqCirc.mCoeffDIscDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tConfigData->mCellConfig.mSurfaceArea,             cellEqCirc.mSurfaceArea, 0.0);

    /// @test outputs of the derive() function.  These expected values are copied from the
    ///       tuning help spreadsheet for this configuration.
    /// @note the spreadsheet uses exact solutions for Lambert W, whereas the models uses faster
    ///       but less accurate approximations, generally accurate to within 1% error.
    const double expectedNVt = 2.5852028372702e-2;
    const double expectedRs  = 9.43247059606149e-3;
    const double expectedRsh = 6.74413002445867;
    const double expectedI0  = 2.09380292531632e-10;
    const double expectedIL  = 2.60363641025022;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNVt, cellEqCirc.mNVt, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRs,  cellEqCirc.mRs,  0.01 * expectedRs);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRsh, cellEqCirc.mRsh, 0.01 * expectedRsh);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI0,  cellEqCirc.mI0,  0.01 * expectedI0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIL,  cellEqCirc.mIL,  0.01 * expectedIL);

    /// @test outputs of the computeEfficiency() function.  These expected values are copied from the
    ///       tuning help spreadsheet for this configuration.
    const double expectedFF  = 0.769;
    const double expectedEff = 0.294;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFF,  cellEqCirc.mFillFactor, 0.001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff, cellEqCirc.mEfficiency, 0.001);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell equivalent circuit version 2 model nominal initialization
///           with error handling.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testCellEquivInitErrors()
{
    UT_RESULT;

    GunnsElectPvCellEquivCircuit2 cellEqCirc;

    // - Test values to force failures were computed from the tuning helper spreadsheet.

    /// @test Operand to exponent for C out of bounds.
    tConfigData->mCellConfig.mOpenCircuitVoltage = 14.0;
    CPPUNIT_ASSERT_THROW(cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc"), TsInitializationException);
    tConfigData->mCellConfig.mOpenCircuitVoltage = tCellOpenCircuitVoltage;

    /// @test Exception handling from Lambert W throw.
    tConfigData->mCellConfig.mMppCurrent = 1.3;
    CPPUNIT_ASSERT_THROW(cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc"), TsInitializationException);
    tConfigData->mCellConfig.mMppCurrent = tCellMppCurrent;

    /// @test Negative series resistance.
    tConfigData->mCellConfig.mIdeality = 1.5;
    CPPUNIT_ASSERT_THROW(cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc"), TsInitializationException);
    tConfigData->mCellConfig.mIdeality = tCellIdeality;

    /// @test Negative shunt resistance.
    tConfigData->mCellConfig.mOpenCircuitVoltage = 0.8;
    CPPUNIT_ASSERT_THROW(cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc"), TsInitializationException);
    tConfigData->mCellConfig.mOpenCircuitVoltage = tCellOpenCircuitVoltage;

    /// @test Operand to exponent for saturation current out of bounds.
    tConfigData->mCellConfig.mOpenCircuitVoltage = 0.9;
    tConfigData->mCellConfig.mIdeality           = 1.5;
    tConfigData->mCellConfig.mRefTemperature     = 5.0;
    CPPUNIT_ASSERT_THROW(cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc"), TsInitializationException);
    tConfigData->mCellConfig.mOpenCircuitVoltage = tCellOpenCircuitVoltage;
    tConfigData->mCellConfig.mIdeality           = tCellIdeality;
    tConfigData->mCellConfig.mRefTemperature     = tCellRefTemperature;

    /// @test Negative saturation current.
    //TODO haven't been able to find a set of inputs that causes this failure without first causing
    //     prior failures.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell equivalent circuit version 2 model update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testCellEquivUpdate()
{
    UT_RESULT;

    GunnsElectPvCellEquivCircuit2 cellEqCirc;
    GunnsElectPvCellEquivCircuit2 cellRef;
    cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc");
    cellRef.initialize(&tConfigData->mCellConfig, "cellRef");

    /// @test normal illumination and reference temperature.
    double photoRatio  = 0.5;
    double temperature = tCellRefTemperature;
    double photoFlux   = tCellPhotoFlux * photoRatio;
    double expectedIsc = tCellShortCircuitCurrent * photoRatio;
    double expectedImp = tCellMppCurrent * photoRatio;
    double expectedRs  = cellRef.mRs / photoRatio;
    double expectedRsh = cellRef.mRsh / photoRatio;
    double expectedI0  = cellRef.mI0 * photoRatio;
    double expectedIL  = cellRef.mIL * photoRatio;
    double expectedFF  = cellRef.mVmp * expectedImp / tCellOpenCircuitVoltage / expectedIsc;
    double expectedEff = cellRef.mVmp * expectedImp / tCellSurfaceArea / photoFlux;
    cellEqCirc.clear();
    CPPUNIT_ASSERT_NO_THROW(cellEqCirc.update(&cellRef, temperature, photoFlux));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIL,           cellEqCirc.mIL,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRsh,          cellEqCirc.mRsh,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRs,           cellEqCirc.mRs,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mNVt,         cellEqCirc.mNVt,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI0,           cellEqCirc.mI0,          DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFF,           cellEqCirc.mFillFactor,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff,          cellEqCirc.mEfficiency,  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mVoc,         cellEqCirc.mVoc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,          cellEqCirc.mIsc,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mVmp,         cellEqCirc.mVmp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImp,          cellEqCirc.mImp,         DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(photoFlux,            cellEqCirc.mPhotoFlux,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mIdeality,    cellEqCirc.mIdeality,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature,          cellEqCirc.mTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mCoeffDVocDT, cellEqCirc.mCoeffDVocDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mCoeffDIscDT, cellEqCirc.mCoeffDIscDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mSurfaceArea, cellEqCirc.mSurfaceArea, 0.0);

    /// @test no illumination.
    photoFlux = 0.0;
    CPPUNIT_ASSERT_NO_THROW(cellEqCirc.update(&cellRef, temperature, photoFlux));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIL,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRsh,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRs,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mNVt,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mI0,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mFillFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mEfficiency,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVoc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIsc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVmp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mImp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mPhotoFlux,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIdeality,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDVocDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDIscDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mSurfaceArea, 0.0);

    /// @test normal illumination at different temperature.
    photoRatio  = 1.03;
    double dT   = 20.0;
    temperature = tCellRefTemperature + dT;
    photoFlux   = tCellPhotoFlux * photoRatio;
    double expectedNVt = 2.7575e-2;
    double expectedVoc = 0.56;
    double expectedVmp = 0.466667;
    expectedIsc = 2.706;
    expectedImp = 2.4978;
    expectedRs  = 0.00562;
    expectedRsh = 7.8232;
    expectedI0  = 3.9939e-9;
    expectedIL  = 2.7079;
    expectedFF  = expectedVmp * expectedImp / expectedVoc / expectedIsc;
    expectedEff = expectedVmp * expectedImp / tCellSurfaceArea / photoFlux;
    CPPUNIT_ASSERT_NO_THROW(cellEqCirc.update(&cellRef, temperature, photoFlux));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIL,           cellEqCirc.mIL,          0.05 * expectedIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRsh,          cellEqCirc.mRsh,         0.05 * expectedRsh);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRs,           cellEqCirc.mRs,          0.06 * expectedRs);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNVt,          cellEqCirc.mNVt,         0.05 * expectedNVt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI0,           cellEqCirc.mI0,          0.05 * expectedI0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFF,           cellEqCirc.mFillFactor,  0.05 * expectedFF);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff,          cellEqCirc.mEfficiency,  0.05 * expectedEff);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,          cellEqCirc.mVoc,         0.05 * expectedVoc);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,          cellEqCirc.mIsc,         0.05 * expectedIsc);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmp,          cellEqCirc.mVmp,         0.05 * expectedVmp);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImp,          cellEqCirc.mImp,         0.05 * expectedImp);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(photoFlux,            cellEqCirc.mPhotoFlux,   DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mIdeality,    cellEqCirc.mIdeality,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature,          cellEqCirc.mTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mCoeffDVocDT, cellEqCirc.mCoeffDVocDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mCoeffDIscDT, cellEqCirc.mCoeffDIscDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cellRef.mSurfaceArea, cellEqCirc.mSurfaceArea, 0.0);

    /// @test extreme temperature causes Voc <= DBL_EPSILON.
    temperature = 600.0;
    CPPUNIT_ASSERT_NO_THROW(cellEqCirc.update(&cellRef, temperature, photoFlux));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIL,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRsh,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRs,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mNVt,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mI0,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mFillFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mEfficiency,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVoc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIsc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVmp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mImp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mPhotoFlux,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIdeality,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDVocDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDIscDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mSurfaceArea, 0.0);

    /// @test extreme temperature causes Isc <= DBL_EPSILON.  We have to enlarge the
    ///       reference cell's Isc temperature coefficient to trigger this case.
    temperature = 300.0;
    CPPUNIT_ASSERT_NO_THROW(cellEqCirc.update(&cellRef, temperature, photoFlux));
    temperature = 240.0;
    cellRef.mCoeffDIscDT = 0.05;
    CPPUNIT_ASSERT_NO_THROW(cellEqCirc.update(&cellRef, temperature, photoFlux));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIL,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRsh,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mRs,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mNVt,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mI0,          0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mFillFactor,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mEfficiency,  0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVoc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIsc,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mVmp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mImp,         0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mPhotoFlux,   0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mIdeality,    0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mTemperature, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDVocDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mCoeffDIscDT, 0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, cellEqCirc.mSurfaceArea, 0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell equivalent circuit version 2 model computeCurrent method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testCellEquivCurrent()
{
    UT_RESULT;

    GunnsElectPvCellEquivCircuit2 cellEqCirc;
    cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc");

    /// @test voltage <= DBL_EPSILON is expected to return exactly short-circuit current.
    double expectedI = tCellShortCircuitCurrent;
    double result = cellEqCirc.computeCurrent(DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, result, 0.0);
    result = cellEqCirc.computeCurrent(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, result, 0.0);
    result = cellEqCirc.computeCurrent(-1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, result, 0.0);

    /// @test nominal voltage < open-circuit voltage.  These expected values are computed from the
    ///       tuning helper spreadsheet, and the model is expected to be within 1% error.
    expectedI = 2.5298;
    result = cellEqCirc.computeCurrent(0.425);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, result, 0.01 * expectedI);
    expectedI = tCellMppCurrent;
    result = cellEqCirc.computeCurrent(tCellMppVoltage);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, result, 0.01 * expectedI);
    expectedI = 1.8147;
    result = cellEqCirc.computeCurrent(0.55);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, result, 0.01 * expectedI);

    /// @test voltage >= open-circuit voltage is expected to return exactly zero.
    expectedI = 0.0;
    result = cellEqCirc.computeCurrent(tCellOpenCircuitVoltage);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, result, 0.0);
    result = cellEqCirc.computeCurrent(tCellOpenCircuitVoltage + 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, result, 0.0);

    /// @test bad operand to the exponent in the input to the Lambert W function.  This requires
    ///       some pretty extreme and unrealistic configuration for the cell, found from the tuning
    ///       helper spreadsheet.
    cellEqCirc.mVoc      = 0.99;
    cellEqCirc.mIsc      = 5.54;
    cellEqCirc.mVmp      = 0.495;
    cellEqCirc.mImp      = 2.25;
    cellEqCirc.mIdeality = 0.09;
    cellEqCirc.mNVt      = 0.09 * 2.59e-2;
    cellEqCirc.mRs       = 0.22;
    cellEqCirc.mRsh      = 2.6512e-9;
    cellEqCirc.mI0       = 1.3936e-177;
    cellEqCirc.mIL       = 4.5971e8;
    CPPUNIT_ASSERT_THROW(cellEqCirc.computeCurrent(0.6), TsOutOfBoundsException);

    /// @test handling exception from the Lambert W function.
    //TODO haven't been able to find a set of inputs that causes this failure without first causing
    //     prior failures.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic Cell equivalent circuit version 2 model computeVoltage method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testCellEquivVoltage()
{
    UT_RESULT;

    GunnsElectPvCellEquivCircuit2 cellEqCirc;
    cellEqCirc.initialize(&tConfigData->mCellConfig, "cellEqCirc");

    /// @test current <= DBL_EPSILON is expected to return exactly open-circuit voltage.
    double expectedV = tCellOpenCircuitVoltage;
    double result = cellEqCirc.computeVoltage(DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, result, 0.0);
    result = cellEqCirc.computeVoltage(0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, result, 0.0);
    result = cellEqCirc.computeVoltage(-1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, result, 0.0);

    /// @test nominal current < short-circuit current.  These are the same expected values as
    ///       testCellEquivCurrent() above, in reverse order of operation.
    expectedV = 0.425;
    result = cellEqCirc.computeVoltage(2.5298);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, result, 0.01 * expectedV);
    expectedV = tCellMppVoltage;
    result = cellEqCirc.computeVoltage(tCellMppCurrent);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, result, 0.01 * expectedV);
    expectedV = 0.55;
    result = cellEqCirc.computeVoltage(1.8147);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, result, 0.05 * expectedV);

    /// @test current >= short-circuit current is expected to return exactly zero.
    expectedV = 0.0;
    result = cellEqCirc.computeVoltage(tCellShortCircuitCurrent);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, result, 0.0);
    result = cellEqCirc.computeVoltage(tCellShortCircuitCurrent + 1.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, result, 0.0);

    /// @test bad operand to the exponent in the input to the Lambert W function.
    /// @note we see here that the model, given a realistically-configured cell,
    ///       can't always compute voltage for the full range of nominal currents.
    ///       This is currently a model limitation.
    CPPUNIT_ASSERT_THROW(cellEqCirc.computeVoltage(0.5), TsOutOfBoundsException);

    /// @test handling exception from the Lambert W function.
    //TODO haven't been able to find a set of inputs that causes this failure without first causing
    //     prior failures.

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the constructor of the GunnsElectPvString2 class.  This class has no member
///           variables, so this test is mainly checking the base class constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testConstruction()
{
    UT_RESULT;

    /// @test    Nominal construction.
    CPPUNIT_ASSERT(""          == tArticle->mName);
    CPPUNIT_ASSERT(tConfigData == tArticle->mConfig);
    CPPUNIT_ASSERT(tInputData  == tArticle->mInput);

    /// @test    Default construction.
    FriendlyGunnsElectPvString2 defaultString;
    CPPUNIT_ASSERT("" == defaultString.mName);
    CPPUNIT_ASSERT(0  == defaultString.mConfig);
    CPPUNIT_ASSERT(0  == defaultString.mInput);

    /// @test    New/delete for code coverage.
    GunnsElectPvString2* article = new GunnsElectPvString2;
    delete article;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String version 2 model nominal initialization without
///           exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testNominalInitialization()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.  Input data
    ///   temperature and illumination are changed from the reference to help verify the initial
    ///   mEqProps state.
    double photoRatio  = 1.03;
    double dT          = 20.0;
    double temperature = tCellRefTemperature + dT;
    double photoFlux   = tCellPhotoFlux * photoRatio;
    tInputData->mTemperature = temperature;
    tInputData->mPhotoFlux   = photoFlux;
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// @test Nominal state data.
    CPPUNIT_ASSERT(tName == tArticle->mName);
    CPPUNIT_ASSERT(0.0   == tArticle->mShortCircuitCurrent);
    CPPUNIT_ASSERT(0.0   == tArticle->mOpenCircuitVoltage);
    CPPUNIT_ASSERT(0.0   == tArticle->mMpp.mConductance);
    CPPUNIT_ASSERT(0.0   == tArticle->mTerminal.mConductance);
    CPPUNIT_ASSERT(0     == tArticle->mNumBypassedGroups);
    CPPUNIT_ASSERT(0     == tArticle->mNumActiveCells);
    CPPUNIT_ASSERT(false == tArticle->mShunted);

    /// @test Initialization of mRefCell.  This repeats some of the checks in
    ///       testCellEquivInit() to verify its initialize function was called.
    double expectedNVt = 2.5852028372702e-2;
    double expectedIL  = 2.60363641025022;
    double expectedEff = 0.294;
    CPPUNIT_ASSERT(0     != tArticle->mRefCell);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellMppVoltage, tArticle->mRefCell->mVmp,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tCellMppCurrent, tArticle->mRefCell->mImp,        0.0);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNVt,     tArticle->mRefCell->mNVt,        1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIL,      tArticle->mRefCell->mIL,         0.01 * expectedIL);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff,     tArticle->mRefCell->mEfficiency, 0.01 * expectedEff);

    /// @test Initial update of mEqProps.  This repeats some of the checks in testCellEquivUpdate()
    ///       to verify its initialize and update functions were called.
    double expectedVmp = 0.466667;
    double expectedImp = 2.4978;
    expectedNVt        = 2.7575e-2;
    expectedEff        = expectedVmp * expectedImp / tCellSurfaceArea / photoFlux;
    CPPUNIT_ASSERT(0     != tArticle->mEqProps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNVt, tArticle->mEqProps->mNVt,        0.05 * expectedNVt);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmp, tArticle->mEqProps->mVmp,        0.05 * expectedVmp);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImp, tArticle->mEqProps->mImp,        0.05 * expectedImp);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff, tArticle->mEqProps->mEfficiency, 0.05 * expectedEff);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String model nominal initialization with exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testInitializationErrors()
{
    UT_RESULT;

    /// @test    Exception thrown for empty name.
    CPPUNIT_ASSERT_THROW(tArticle->initialize(""), TsInitializationException);

    /// @test    Exception thrown for missing config data.
    tArticle->mConfig = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tArticle->mConfig = tConfigData;

    /// @test    Exception thrown for bad open circuit voltage.
    tConfigData->mCellConfig.mOpenCircuitVoltage = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mOpenCircuitVoltage = tCellOpenCircuitVoltage;

    /// @test    Exception thrown for bad short circuit current.
    tConfigData->mCellConfig.mShortCircuitCurrent = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mShortCircuitCurrent = tCellShortCircuitCurrent;

    /// @test    Exception thrown for bad MPP voltage.
    tConfigData->mCellConfig.mMppVoltage = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mMppVoltage = tCellMppVoltage;

    /// @test    Exception thrown for bad MPP current.
    tConfigData->mCellConfig.mMppCurrent = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mMppCurrent = tCellMppCurrent;

    /// @test    Exception thrown for bad reference photo flux.
    tConfigData->mCellConfig.mPhotoFlux = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mPhotoFlux = tCellPhotoFlux;

    /// @test    Exception thrown for bad reference temperature.
    tConfigData->mCellConfig.mRefTemperature = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mRefTemperature = tCellRefTemperature;

    /// @test    Exception thrown for bad diode ideality.
    tConfigData->mCellConfig.mIdeality = 0.9999;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mIdeality = 1.5001;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mIdeality = tCellIdeality;

    /// @test    Exception thrown for bad surface area.
    tConfigData->mCellConfig.mSurfaceArea = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mCellConfig.mSurfaceArea = tCellSurfaceArea;

    /// @test    Exception thrown for bad # cells.
    tConfigData->mNumCells = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mNumCells = tNumCells;

    /// @test    Exception thrown for bad bypass diode interval.
    tConfigData->mBypassDiodeInterval = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mBypassDiodeInterval = 3;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tConfigData->mBypassDiodeInterval = tBypassDiodeInterval;

    /// @test    Exception thrown for missing input data.
    tArticle->mInput = 0;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tArticle->mInput = tInputData;

    /// @test    Exception thrown for bad photo flux magnitude.
    tInputData->mPhotoFlux = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tInputData->mPhotoFlux = tPhotoFlux;

    /// @test    Exception thrown for bad source exposed fraction.
    tInputData->mSourceExposedFraction = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tInputData->mSourceExposedFraction = 1.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tInputData->mSourceExposedFraction = tSourceExposedFraction;

    /// @test    Exception thrown for bad temperature.
    tInputData->mTemperature = -0.01;
    CPPUNIT_ASSERT_THROW(tArticle->initialize(tName), TsInitializationException);
    tInputData->mTemperature = tTemperature;

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String model version 2 determination of number of active cells
///           and bypassed groups.  This repeats some of the base class
///           UtGunnsElectPvString::testActiveCells() to verify that
///           GunnsElectPvString::updateBypassedGroups() is called.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testActiveCells()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// @test    Partial shading of one group.
    tInputData->mSourceExposedFraction = 0.9;
    const int expectedBypGroup = 1;
    const int expectedNumCells = 15;
    tArticle->update();
    CPPUNIT_ASSERT(expectedBypGroup == tArticle->mNumBypassedGroups);
    CPPUNIT_ASSERT(expectedNumCells == tArticle->mNumActiveCells);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests for Photovoltaic String version 2 model update method.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testStep()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    {
        /// @test Update outputs with one cell group bypassed and the string is 20% degraded.  These
        ///       expected values are copied from the tuning help spreadsheet for this configuration.
        /// @note the spreadsheet uses exact solutions for Lambert W, whereas the models uses faster
        ///       but less accurate approximations, generally accurate to within 1% error.
        tArticle->mMalfCellGroupValue = 1;
        tArticle->mMalfCellGroupFlag  = true;
        tArticle->mMalfDegradeValue   = 0.2;
        tArticle->mMalfDegradeFlag    = true;
        tInputData->mTemperature      = 320.0;
        tInputData->mPhotoFlux        = 1400.0;
        const unsigned int numCells   = tNumCells - tBypassDiodeInterval;
        const double expectedVoc      = 0.56;
        const double expectedIsc      = 2.16760323291697;
        const double expectedVmp      = 0.466666666666667;
        const double expectedImp      = 2.00086452269259;
        const double expectedNVt      = 2.75754664717318e-2;
        const double expectedRs       = 7.01631078770962e-3;
        const double expectedRsh      = 9.76631204019056;
        const double expectedI0       = 3.19922799011596e-09;
        const double expectedIL       = 2.16916048174171;
        const double expectedEff      = expectedVmp * expectedImp / 1400.0 / tCellSurfaceArea;
        const double expectedFF       = expectedVmp * expectedImp / expectedVoc / expectedIsc;
        const double expectedVsDrop   = tBlockingDiodeVoltageDrop + tBypassDiodeVoltageDrop;
        const double expectedVocStr   = numCells * expectedVoc - expectedVsDrop;
        const double expectedVmpStr   = numCells * expectedVmp - expectedVsDrop;
        const double expectedPmpStr   = expectedVmpStr * expectedImp;
        const double expectedGmpStr   = expectedImp / expectedVmpStr;
        CPPUNIT_ASSERT_NO_THROW(tArticle->update());
        CPPUNIT_ASSERT_EQUAL(numCells, tArticle->mNumActiveCells);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVsDrop, tArticle->mSeriesVoltageDrop,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(320.0,          tArticle->mEqProps->mTemperature, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1400.0,         tArticle->mEqProps->mPhotoFlux,   DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVoc,    tArticle->mEqProps->mVoc,         0.01 * expectedVoc);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,    tArticle->mEqProps->mIsc,         0.01 * expectedIsc);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmp,    tArticle->mEqProps->mVmp,         0.01 * expectedVmp);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImp,    tArticle->mEqProps->mImp,         0.01 * expectedImp);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedNVt,    tArticle->mEqProps->mNVt,         0.01 * expectedNVt);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRs,     tArticle->mEqProps->mRs,          0.05 * expectedRs);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedRsh,    tArticle->mEqProps->mRsh,         0.05 * expectedRsh);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI0,     tArticle->mEqProps->mI0,          0.05 * expectedI0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIL,     tArticle->mEqProps->mIL,          0.05 * expectedIL);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedEff,    tArticle->mEqProps->mEfficiency,  0.01 * expectedIL);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedFF,     tArticle->mEqProps->mFillFactor,  0.01 * expectedIL);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedIsc,    tArticle->mShortCircuitCurrent,   0.01 * expectedIsc);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVocStr, tArticle->mOpenCircuitVoltage,    0.01 * expectedVocStr);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVmpStr, tArticle->mMpp.mVoltage,          0.01 * expectedVmpStr);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedImp,    tArticle->mMpp.mCurrent,          0.01 * expectedImp);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPmpStr, tArticle->mMpp.mPower,            0.01 * expectedPmpStr);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedGmpStr, tArticle->mMpp.mConductance,      0.01 * expectedGmpStr);
    } {
        /// @test Update outputs with all cell group bypassed so the string makes no power.
        tArticle->mMalfCellGroupValue = 4;
        const double expectedVsDrop   = tBlockingDiodeVoltageDrop + 4 * tBypassDiodeVoltageDrop;
        CPPUNIT_ASSERT_NO_THROW(tArticle->update());
        CPPUNIT_ASSERT(0 == tArticle->mNumActiveCells);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedVsDrop, tArticle->mSeriesVoltageDrop,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mTemperature, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mPhotoFlux,   0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mVoc,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mIsc,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mVmp,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mImp,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mNVt,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mRs,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mRsh,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mI0,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mIL,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mEfficiency,  0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mFillFactor,  0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mShortCircuitCurrent,   0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mOpenCircuitVoltage,    0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mMpp.mVoltage,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mMpp.mCurrent,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mMpp.mPower,            0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mMpp.mConductance,      0.0);
    } {
        /// @test Update outputs zero panel temperature so the string makes no power.
        tArticle->mMalfCellGroupValue = 0;
        tInputData->mTemperature = 0.0;
        CPPUNIT_ASSERT_NO_THROW(tArticle->update());
        CPPUNIT_ASSERT_EQUAL(tNumCells, static_cast<int>(tArticle->mNumActiveCells));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mTemperature, 0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mPhotoFlux,   0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mVoc,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mIsc,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mVmp,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mImp,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mNVt,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mRs,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mRsh,         0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mI0,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mIL,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mEfficiency,  0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mEqProps->mFillFactor,  0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mShortCircuitCurrent,   0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mOpenCircuitVoltage,    0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mMpp.mVoltage,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mMpp.mCurrent,          0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mMpp.mPower,            0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,            tArticle->mMpp.mConductance,      0.0);
    } {
        /// @test Update outputs with MPP voltage = 0, for coverage.  We force this by manipulating
        ///       the blocking diode voltage to be higher than the entire output of all the cells.
        tArticle->mMalfCellGroupValue = 0;
        tArticle->mMalfCellGroupFlag  = false;
        tArticle->mMalfDegradeValue   = 0.0;
        tArticle->mMalfDegradeFlag    = false;
        tConfigData->mBlockingDiodeVoltageDrop = 10.0;
        CPPUNIT_ASSERT_NO_THROW(tArticle->update());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMpp.mConductance,      0.0);
    } {
        /// @test updateMpp in the zero voltage case for code coverage.
        tArticle->mEqProps->mVmp = 0.0;
        tArticle->mEqProps->mImp = 1.0;
        tArticle->updateMpp();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMpp.mVoltage,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, tArticle->mMpp.mCurrent,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMpp.mPower,       0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, tArticle->mMpp.mConductance, 0.0);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the loadAtPower method.  Since the string vesrion 2 model has a limitation that
///           it can't be loaded at an arbitrary power, it can only load at either Voc, Isc, or MPP.
///           So we just test those 3 options.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testLoadAtPower()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// - Update the cell with nominal state.
    tArticle->update();

    {
        /// @test    Terminal outputs at MPP for any power load > 0.
        const double expectedP = tArticle->mMpp.mPower;
        const double expectedV = tArticle->mMpp.mVoltage;
        const double expectedI = tArticle->mMpp.mCurrent;
        const double expectedG = tArticle->mMpp.mConductance;
        const bool   shortSide = false;

        tArticle->loadAtPower(1.0, shortSide);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, 0.0);
    } {
        /// @test    Terminal outputs given zero power load, on short-circuit side.
        const double expectedP = 0.0;
        const double expectedV = 0.0;
        const double expectedI = tArticle->mShortCircuitCurrent;
        const double expectedG = tArticle->mShortCircuitCurrent / DBL_EPSILON;
        const bool   shortSide = true;

        tArticle->loadAtPower(expectedP, shortSide);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     DBL_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, DBL_EPSILON);
    } {
        /// @test    Terminal outputs given zero power load, on open-circuit side.
        const double expectedP = 0.0;
        const double expectedV = tArticle->mOpenCircuitVoltage;
        const double expectedI = 0.0;
        const double expectedG = 0.0;
        const bool   shortSide = false;

        tArticle->loadAtPower(expectedP, shortSide);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, 0.0);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the loadAtVoltage and predictCurrentAtVoltage methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testLoadAtVoltage()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// - Update the cell with nominal state.
    tArticle->update();

    {
        /// @test Terminal outputs on open-circuit side of the I-V curve.  These values are copied
        ///       from the tuning helper spreadsheet.
        /// @note The spreadsheet uses exact solutions for Lambert W, whereas the models uses
        ///       faster but less accurate approximations, generally accurate to within 1% error.
        const double cellV     = 0.505;
        const double expectedV = cellV * tNumCells - tBlockingDiodeVoltageDrop;
        const double expectedI = 2.3738;
        const double expectedP = expectedV * expectedI;
        const double expectedG = expectedI / expectedV;

        tArticle->loadAtVoltage(expectedV);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     0.01 * expectedV);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     0.01 * expectedI);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       0.01 * expectedP);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, 0.01 * expectedG);
    } {
        /// @test Terminal outputs on short-circuit side of the I-V curve.  These values are copied
        ///       from the tuning helper spreadsheet.
        /// @note the spreadsheet uses exact solutions for Lambert W, whereas the models uses
        ///       faster but less accurate approximations, generally accurate to within 1% error.
        const double cellV     = 0.475;
        const double expectedV = cellV * tNumCells - tBlockingDiodeVoltageDrop;
        const double expectedI = 2.4803;
        const double expectedP = expectedV * expectedI;
        const double expectedG = expectedI / expectedV;

        tArticle->loadAtVoltage(expectedV);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     0.01 * expectedV);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     0.01 * expectedI);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       0.01 * expectedP);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, 0.01 * expectedG);
    } {
        /// @test    Terminal outputs given zero voltage.
        const double expectedV = 0.0;
        const double expectedI = tArticle->predictCurrentAtVoltage(expectedV);
        const double expectedP = 0.0;
        const double expectedG = 0.0;

        tArticle->loadAtVoltage(expectedV);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     0.01 * expectedI);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, 0.0);
    } {
        /// @test    Terminal outputs given voltage greater than maximum.
        const double expectedV = tArticle->mOpenCircuitVoltage + 1.0;
        const double expectedI = 0.0;
        const double expectedP = 0.0;
        const double expectedG = 0.0;

        tArticle->loadAtVoltage(expectedV);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, 0.0);
    } {
        /// @test    Terminal outputs with no active cells.
        tArticle->mMalfCellGroupFlag  = true;
        tArticle->mMalfCellGroupValue = 4;
        tArticle->update();

        const double expectedV  = tArticle->mOpenCircuitVoltage + 1.0;
        const double expectedI = 0.0;
        const double expectedP = 0.0;
        const double expectedG = 0.0;

        tArticle->loadAtVoltage(expectedV);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       0.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, 0.0);
    }

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests the loadAtConductance method.  Since the string vesrion 2 model has a limitation
///           that it can't be loaded at an arbitrary conductance, it can only load at either near
///           Voc or Isc.  So we just test those 2 options.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtGunnsElectPvString2::testLoadAtConductance()
{
    UT_RESULT;

    /// - Initialize default constructed test article with nominal initialization data.
    CPPUNIT_ASSERT_NO_THROW(tArticle->initialize(tName));

    /// - Update the cell with nominal state.
    tArticle->update();

    {
        /// @test Terminal outputs on open-circuit side of the I-V curve.  We expect 99.99% Voc.
        const double expectedV = 0.9999 * tArticle->mOpenCircuitVoltage;
        const double expectedI = tArticle->predictCurrentAtVoltage(expectedV);
        const double expectedP = expectedV * expectedI;
        const double expectedG = expectedI / expectedV;

        tArticle->loadAtConductance(0.0);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
    } {
        /// @test Terminal outputs on short-circuit side of the I-V curve.  We expect 0.01% Voc.
        const double expectedV = 0.0001 * tArticle->mOpenCircuitVoltage;
        const double expectedI = tArticle->predictCurrentAtVoltage(expectedV);
        const double expectedP = expectedV * expectedI;
        const double expectedG = expectedI / expectedV;

        tArticle->loadAtConductance(1.0e10);

        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedV, tArticle->mTerminal.mVoltage,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedI, tArticle->mTerminal.mCurrent,     FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, tArticle->mTerminal.mPower,       FLT_EPSILON);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedG, tArticle->mTerminal.mConductance, FLT_EPSILON);
    }

    UT_PASS_LAST;
}
