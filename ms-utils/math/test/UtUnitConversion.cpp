/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (software/exceptions/TsOutOfBoundsException.o)
        (math/UnitConversion.o)
    )
***************************************************************************************************/

#include <iostream>

#include "software/exceptions/TsOutOfBoundsException.hh"
#include "math/UnitConversion.hh"

#include "UtUnitConversion.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this unit conversion unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtUnitConversion::UtUnitConversion()
    :
    CppUnit::TestFixture()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this unit conversio unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtUnitConversion::~UtUnitConversion()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::setUp()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test angle conversion factors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testMiscConversion()
{
    std::cout << "\n................................................................................";
    std::cout << "\n.Unit Conversion 01: Test Miscellaneous Conversion Consistency..............";

    double expected = 1.0;

    // Consistency Checks
    double returned = UnitConversion::MILLI_PER_UNIT * UnitConversion::UNIT_PER_MILLI;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);
    returned = UnitConversion::KILO_PER_UNIT * UnitConversion::UNIT_PER_KILO;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);
    returned = UnitConversion::MEGA_PER_UNIT * UnitConversion::UNIT_PER_MEGA;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);

    // Test conversion methods
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1000000.0, UnitConversion::convertUnitToMicro(1.0),             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.0,    UnitConversion::convertUnitToMilli(1.0),             DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,     UnitConversion::convertUnitToKilo(1.0),              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.000001,  UnitConversion::convertUnitToMega(1.0),              DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0,      UnitConversion::convertFracToPercent(0.1),           DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3600.0,    UnitConversion::convertUnitPerSecToUnitPerHour(1.0), DBL_EPSILON);

    // Test convert method.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1000000.0, UnitConversion::convert(UnitConversion::UNIT_TO_MICRO,             1.0),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.0,    UnitConversion::convert(UnitConversion::UNIT_TO_MILLI,             1.0),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.001,     UnitConversion::convert(UnitConversion::UNIT_TO_KILO,              1.0),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.000001,  UnitConversion::convert(UnitConversion::UNIT_TO_MEGA,              1.0),  DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0,       UnitConversion::convert(UnitConversion::FRAC_TO_PERCENT,           0.02), DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7200.0,    UnitConversion::convert(UnitConversion::UNIT_PER_SEC_TO_UNIT_PER_HOUR, 2.0),  DBL_EPSILON);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test angle conversion factors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testAngleConversion()
{
    std::cout << "\n.Unit Conversion 02: Test Angle Conversion Consistency......................";

    double expected = 1.0;

    // Consistency Checks
    double returned = UnitConversion::RAD_PER_DEG * UnitConversion::DEG_PER_RAD_UTIL;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);
    returned = UnitConversion::MIL_PER_RAD * UnitConversion::RAD_PER_MIL;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);
    returned = UnitConversion::MIL_PER_DEG * UnitConversion::DEG_PER_MIL;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);
    returned = UnitConversion::RAD_PER_REV / UnitConversion::TWO_PI;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);
    returned = UnitConversion::RAD_PER_ARCSEC * UnitConversion::DEG_PER_RAD_UTIL * 3600.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);
    returned = UnitConversion::RAD_PER_ARCMIN * UnitConversion::DEG_PER_RAD_UTIL * 60.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  DBL_EPSILON);

    // Test conversion methods
    CPPUNIT_ASSERT_DOUBLES_EQUAL(360.0,         UnitConversion::convertRadToDeg(6.28318530718),       1.0E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.28318530718, UnitConversion::convertDegToRad(360.0),               1.0E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.54929658552, UnitConversion::convertRadPerSecToRpm(1.0),           1.0E-11);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,           UnitConversion::convertRpmtoRadPerSec(9.54929658552), 1.0E-11);

    // Test convert method.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(360.0,         UnitConversion::convert(UnitConversion::RAD_TO_DEG,       6.28318530718), 1.0E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.28318530718, UnitConversion::convert(UnitConversion::DEG_TO_RAD,       360.0),         1.0E-10);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.54929658552, UnitConversion::convert(UnitConversion::RADPERSEC_TO_RPM, 1.0),           1.0E-11);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0,           UnitConversion::convert(UnitConversion::RPM_TO_RADPERSEC, 9.54929658552), 1.0E-11);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test length conversion factors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testLengthConversion()
{
    std::cout << "\n.Unit Conversion 03: Test Length Conversion Consistency.....................";

    const double expected  = 1.0;
    const double tolerance = 1.0e-09;

    // Consistency Checks
    double returned = UnitConversion::IN_PER_FT_UTIL * UnitConversion::FT_PER_IN;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::FT_PER_YD * UnitConversion::YD_PER_FT;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::CM_PER_IN * UnitConversion::IN_PER_CM;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::M_PER_IN  * UnitConversion::IN_PER_M;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::FT_PER_M  * UnitConversion::M_PER_FT;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::M_PER_KM  * UnitConversion::KM_PER_M;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::IN2_PER_FT2_UTIL * UnitConversion::FT2_PER_IN2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::IN2_PER_M2  * UnitConversion::M2_PER_IN2_UTIL;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::FT2_PER_M2  * UnitConversion::M2_PER_FT2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::IN3_PER_FT3 * UnitConversion::FT3_PER_IN3;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::IN3_PER_M3  * UnitConversion::M3_PER_IN3;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::FT3_PER_M3  * UnitConversion::M3_PER_FT3;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test mass conversion factors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testMassConversion()
{
    std::cout << "\n.Unit Conversion 04: Test Mass Conversion Consistency.......................";

    const double expected  = 1.0;
    const double tolerance = 1.0e-10;

    // Consistency Checks
    double returned = UnitConversion::KG_PER_LBM * UnitConversion::LBM_PER_KG;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::G_PER_LBM * UnitConversion::LBM_PER_G;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::KG_PER_SEC_PER_LBM_PER_HR *
        UnitConversion::LBM_PER_HR_PER_KG_PER_SEC;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::KG_PER_SEC_PER_LBM_PER_SEC  *
        UnitConversion::LBM_PER_SEC_PER_KG_PER_SEC;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    // Test conversion methods
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2380.992274121, UnitConversion::convertKgPerSecToLbmPerHour(0.3), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3, UnitConversion::convertLbmPerHourToKgPerSec(2380.992274121), tolerance);

    // Test convert method.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2380.992274121, UnitConversion::convert(UnitConversion::KG_PER_SEC_TO_LBM_PER_HOUR, 0.3), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3,       UnitConversion::convert(UnitConversion::LBM_PER_HOUR_TO_KG_PER_SEC, 2380.992274121), tolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test pressure conversion factors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testPressureConversion()
{
    std::cout << "\n.Unit Conversion 05: Test Pressure Conversion Consistency...................";

    const double expected  = 1.0;
    const double tolerance = 1.0e-10;

    // Consistency Checks
    double returned = UnitConversion::PA_PER_PSI * UnitConversion::PSI_PER_PA;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::TORR_PER_PSI * UnitConversion::PSI_PER_TORR;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::ATM_PER_PSI * UnitConversion::PSI_PER_ATM;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::ATM_PER_PA * UnitConversion::PA_PER_ATM;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::KPA_PER_PSI * UnitConversion::PSI_PER_KPA;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::TORR_PER_KPA * UnitConversion::KPA_PER_PSI * UnitConversion::PSI_PER_TORR;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::KPA_PER_MMHG * UnitConversion::MMHG_PER_PSI * UnitConversion::PSI_PER_KPA;

    // Test conversion methods
    CPPUNIT_ASSERT_DOUBLES_EQUAL(14.6959494004, UnitConversion::convertKpaToPsi(101.325), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.325,       UnitConversion::convertPsiToKpa(14.6959494004), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(759.999819985, UnitConversion::convertKpaToMmhg(101.325), 1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(759999.819985, UnitConversion::convertKpaToMilliTorr(101.325), 1.0e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(406.7824617,   UnitConversion::convertKpaToInh2o(101.325), 1.0e-6);

    // Test convert method.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(14.6959494004, UnitConversion::convert(UnitConversion::KPA_TO_PSI,       101.325), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101.325,       UnitConversion::convert(UnitConversion::PSI_TO_KPA,       14.6959494004), tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(759.999819985, UnitConversion::convert(UnitConversion::KPA_TO_MMHG,      101.325), 1.0e-9);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(759999.819985, UnitConversion::convert(UnitConversion::KPA_TO_MILLITORR, 101.325), 1.0e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(406.7824617,   UnitConversion::convert(UnitConversion::KPA_TO_INH2O,     101.325), 1.0e-6);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test energy conversion factors.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testEnergyConversion()
{
    std::cout << "\n.Unit Conversion 06: Test Energy Conversion Consistency.....................";

    const double expected  = 1.0;
    const double tolerance = 1.0e-03;

    double returned = UnitConversion::BTU_PER_J         * UnitConversion::J_PER_BTU;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::KJ_PER_BTU        * UnitConversion::BTU_PER_KJ;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::BTU_PER_W_S       * UnitConversion::W_S_PER_BTU;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::BTU_PER_HR_PER_W  * UnitConversion::W_PER_BTU_PER_HR;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);
    returned = UnitConversion::BTU_PER_HR_PER_KW * UnitConversion::KW_PER_BTU_PER_HR;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test percent conversion methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testPercentConversion()
{
    std::cout << "\n.Unit Conversion 07: Test Percent Conversion................................";

    const double tolerance = DBL_EPSILON;

    double returned = UnitConversion::convertToPercent(300.0, 1.0 / 200.0);
    double expected = 150.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertToPercent(50.0, 1.0 / 150.0);
    expected = 100.0 / 3.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertToPercent(-50.0, 1.0 / 150.0);
    expected = -100.0 / 3.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertToPercent(50.0, -1.0 / 150.0);
    expected = -100.0 / 3.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertToPercent(-300.0, 1.0 / 200.0);
    expected = -150.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertFromPercent(150.0, 200.0);
    expected = 300.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertFromPercent(50.0, 150.0);
    expected = 75.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertFromPercent(-50.0, 150.0);
    expected = -75.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertFromPercent(-100.0, 150.0);
    expected = -150.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    returned = UnitConversion::convertFromPercent(-150.0, 200.0);
    expected = -300.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned,  tolerance);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test temperature conversion methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testTemperatureConversion()
{
    std::cout << "\n.Unit Conversion 08: Test Temperature Conversion............................";

    static const double tolerance = 1.0e-12;

    struct Temperatures{
            double kelvin;
            double celsius;
            double rankine;
            double fahrenheit;
    };

    static const Temperatures temperature[8] = {
        { -1.0,  -274.15,   -1.8,   -461.47},
        {  0.0,  -273.15,    0.0,   -459.67},
        {  1.0,  -272.15,    1.8,   -457.87},
        {173.15, -100.0,   311.67,  -148.0},
        {233.15,  -40.0,   419.67,   -40.0},
        {273.15,    0.0,   491.67,    32.0},
        {373.15,  100.0,   671.67,   212.0},
        {500.0,   226.85,  900.0,    440.33}
    };

    for (int i = 0; i < 8; i++) {
        {
            double k = temperature[i].kelvin;
            double c = UnitConversion::convertDegKToDegC(k);
            double f = UnitConversion::convertDegKToDegF(k);
            double r = UnitConversion::convertDegKToDegR(k);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].celsius,    c, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].rankine,    r, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].fahrenheit, f, tolerance);

            double mc = UnitConversion::convert(UnitConversion::K_TO_C, k);
            double mr = UnitConversion::convert(UnitConversion::K_TO_R, k);
            double mf = UnitConversion::convert(UnitConversion::K_TO_F, k);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].celsius,    mc, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].rankine,    mr, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].fahrenheit, mf, tolerance);
        } {
            double c = temperature[i].celsius;
            double f = UnitConversion::convertDegCToDegF(c);
            double r = UnitConversion::convertDegCToDegR(c);
            double k = UnitConversion::convertDegCToDegK(c);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].rankine,    r, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].fahrenheit, f, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].kelvin,     k, tolerance);

            double mf = UnitConversion::convert(UnitConversion::C_TO_F, c);
            double mr = UnitConversion::convert(UnitConversion::C_TO_R, c);
            double mk = UnitConversion::convert(UnitConversion::C_TO_K, c);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].fahrenheit, mf, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].rankine,    mr, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].kelvin,     mk, tolerance);
        } {
            double r = temperature[i].rankine;
            double f = UnitConversion::convertDegRToDegF(r);
            double c = UnitConversion::convertDegRToDegC(r);
            double k = UnitConversion::convertDegRToDegK(r);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].fahrenheit, f, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].celsius,    c, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].kelvin,     k, tolerance);

            double mf = UnitConversion::convert(UnitConversion::R_TO_F, r);
            double mc = UnitConversion::convert(UnitConversion::R_TO_C, r);
            double mk = UnitConversion::convert(UnitConversion::R_TO_K, r);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].fahrenheit, mf, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].celsius,    mc, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].kelvin,     mk, tolerance);
        } {
            double f = temperature[i].fahrenheit;
            double r = UnitConversion::convertDegFToDegR(f);
            double c = UnitConversion::convertDegFToDegC(f);
            double k = UnitConversion::convertDegFToDegK(f);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].rankine,    r, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].celsius,    c, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].kelvin,     k, tolerance);

            double mr = UnitConversion::convert(UnitConversion::F_TO_R, f);
            double mc = UnitConversion::convert(UnitConversion::F_TO_C, f);
            double mk = UnitConversion::convert(UnitConversion::F_TO_K, f);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].rankine,    mr, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].celsius,    mc, tolerance);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i].kelvin,     mk, tolerance);
        }
    }

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test power conversion methods.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testPowerConversion()
{
    std::cout << "\n.Unit Conversion 09: Test Power Conversion..................................";

    static const double tolerance = 1.0e-03;

    /// - Check dB -> Power Ratio -> dB
    for (int i = 0; i < 1000; i++) {
        const double expectedDB = static_cast<double>(i) * 0.25;
        const double expectedPR = pow(10, 0.1 * expectedDB);
        const double returnedPR = UnitConversion::convertDbToPr(expectedDB);
        const double convertPR  = UnitConversion::convert(UnitConversion::DB_TO_PR, expectedDB);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPR, returnedPR, tolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(returnedPR, convertPR,  0.0);
        const double returnedDB = UnitConversion::convertPrToDb(returnedPR);
        const double convertDB  = UnitConversion::convert(UnitConversion::PR_TO_DB, returnedPR);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDB, returnedDB, tolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(returnedDB, convertDB,  0.0);
    }

    /// - Check Watts -> dB Watts
    for (int i = 1; i < 1000; i++) {
        const double expectedW   = static_cast<double>(i) * 0.25;
        const double expectedDBW = 10.0 * log10(expectedW);
        const double returnedDBW = UnitConversion::convertWToDbw(expectedW);
        const double convertDBW  = UnitConversion::convert(UnitConversion::W_TO_DBW, expectedW);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDBW, returnedDBW, tolerance);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(returnedDBW, convertDBW,  0.0);
    }

    /// - Check Watts -> dB Watts with invalid (< 0) argument, expect 0
    const double expectedW   = -1.0;
    const double expectedDBW =  0.0;
    const double returnedDBW =  UnitConversion::convertWToDbw(expectedW);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDBW, returnedDBW, tolerance);

    /// - Check Power Ratio -> dB -> Power Ratio
    for (int i = 1; i < 1000; i++) {
        const double expectedPR = static_cast<double>(i) * 0.25;
        const double expectedDB = 10.0 * log10(expectedPR);
        const double returnedDB = UnitConversion::convertPrToDb(expectedPR);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedDB, returnedDB, tolerance);
        const double returnedPR = UnitConversion::convertDbToPr(returnedDB);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPR, returnedPR, tolerance);
    }

    /// - Check Power Ratio -> dB with invalid (< 0) argument
    CPPUNIT_ASSERT_THROW(UnitConversion::convertPrToDb(-1.0), TsOutOfBoundsException);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test comparison against old values with best tolerance that passes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testCompareOld()
{
    std::cout << "\n.Unit Conversion 10: Test Comparison Against Old Values.....................";

    // Time
    const double TS_10_TO_THE_MINUS_8  = 1E-8;
    const double TS_10_TO_THE_MINUS_12 = 1E-12;
    const double TS_SEC_PER_DAY        = 86400.0;
    const double TS_SEC_PER_HR         = 3600.0;
    const double TS_MIN_PER_HR         =   60.0;
    const double TS_SEC_PER_MIN        =   60.0;
    const double TS_HR_PER_SEC         = 0.00027777778;
    const double TS_HR_PER_MIN         = 0.01666666667;
    const double TS_HR_PER_DAY         = 24.0;
    const double TS_MIN_PER_SEC        = 0.01666666667;
    const int    TS_DAYS_PER_YEAR      = 365;
    const int    TS_MONTHS_PER_YEAR    = 12;
    const int    TS_MAX_YEAR           = 2200;
    const int    TS_MIN_YEAR           = 1970;
    const int    TS_START_DAY_NORM[]   = {0, 0,31,59,90,120,151,181,212,243,273,304,334,365};
    const int    TS_START_DAY_LEAP[]   = {0, 0,31,60,91,121,152,182,213,244,274,305,335,366};
    const double TS_LEAP_YEAR_SEC      = 31622400.0;
    const double TS_COMMON_YEAR_SEC    = 31536000.0;
    const double TS_SEC_PER_MSEC       = 1.0E3;
    const double TS_MSEC_PER_SEC       = 1.0E-3;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_10_TO_THE_MINUS_8, UnitConversion::TEN_TO_THE_MINUS_8, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_10_TO_THE_MINUS_12, UnitConversion::TEN_TO_THE_MINUS_12, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SEC_PER_DAY, UnitConversion::SEC_PER_DAY, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SEC_PER_HR, UnitConversion::SEC_PER_HR, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_MIN_PER_HR, UnitConversion::SEC_PER_MIN, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SEC_PER_MIN, UnitConversion::MIN_PER_HR, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_HR_PER_SEC, UnitConversion::HR_PER_SEC, 1.0e-11);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_HR_PER_MIN, UnitConversion::HR_PER_MIN, 1.0e-11);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_HR_PER_DAY, UnitConversion::HR_PER_DAY, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_MIN_PER_SEC, UnitConversion::MIN_PER_SEC, 1.0e-11);
    CPPUNIT_ASSERT_EQUAL(TS_DAYS_PER_YEAR, UnitConversion::DAYS_PER_YEAR);
    CPPUNIT_ASSERT_EQUAL(TS_MONTHS_PER_YEAR, UnitConversion::MONTHS_PER_YEAR);
    CPPUNIT_ASSERT_EQUAL(TS_MAX_YEAR, UnitConversion::MAX_YEAR);
    CPPUNIT_ASSERT_EQUAL(TS_MIN_YEAR, UnitConversion::MIN_YEAR);
    for (int i = 0; i < 14; i++) {
        CPPUNIT_ASSERT_EQUAL(TS_START_DAY_NORM[i], UnitConversion::START_DAY_NORM[i]);
        CPPUNIT_ASSERT_EQUAL(TS_START_DAY_LEAP[i], UnitConversion::START_DAY_LEAP[i]);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_LEAP_YEAR_SEC, UnitConversion::LEAP_YEAR_SEC, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_COMMON_YEAR_SEC, UnitConversion::COMMON_YEAR_SEC, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SEC_PER_MSEC, UnitConversion::SEC_PER_MSEC, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_MSEC_PER_SEC, UnitConversion::MSEC_PER_SEC, 1.0e-16);

    // Frequency
    const double TS_HZ_PER_MHZ        = 1.0E6;
    const double TS_MHZ_PER_HZ        = 1.0E-6;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_HZ_PER_MHZ, UnitConversion::HZ_PER_MHZ, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_MHZ_PER_HZ, UnitConversion::MHZ_PER_HZ, 1.0e-16);

    // Length
    const double TS_IN_PER_FT         = 12.0;
    const double TS_FT_PER_IN         = 0.08333333333333;
    const double TS_FT_PER_YD         = 3.0;
    const double TS_YD_PER_FT         = 0.33333333333333;
    const double TS_CM_PER_IN         = 2.54;
    const double TS_IN_PER_CM         = 0.3937007874016;
    const double TS_M_PER_IN          = 0.0254;
    const double TS_IN_PER_M          = 39.37007874;
    const double TS_FT_PER_M          = 3.280839895013;
    const double TS_M_PER_FT          = 0.3048;
    const double TS_FT_PER_MI         = 5280.0;      // Mile
    const double TS_MI_PER_FT         = 0.000189394;
    const double TS_FT_PER_NMI        = 6076.115;    // International nautical mile
    const double TS_NMI_PER_FT        = 0.000164579;
    const double TS_M_PER_KM          = 1000.0;
    const double TS_KM_PER_M          = 0.001;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_IN_PER_FT, UnitConversion::IN_PER_FT_UTIL, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_FT_PER_IN, UnitConversion::FT_PER_IN, 1.0e-14);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_FT_PER_YD, UnitConversion::FT_PER_YD, 1.0e-11);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_YD_PER_FT, UnitConversion::YD_PER_FT, 1.0e-13);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_CM_PER_IN, UnitConversion::CM_PER_IN, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_IN_PER_CM, UnitConversion::IN_PER_CM, 1.0e-13);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_M_PER_IN, UnitConversion::M_PER_IN, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_IN_PER_M, UnitConversion::IN_PER_M, 1.0e-09);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_FT_PER_M, UnitConversion::FT_PER_M, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_M_PER_FT, UnitConversion::M_PER_FT, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_M_PER_KM, UnitConversion::M_PER_KM, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_KM_PER_M, UnitConversion::KM_PER_M, 1.0e-16);

    // Area
    const double TS_IN2_PER_FT2       = 144.0;
    const double TS_FT2_PER_IN2       = 0.006944444444445;
    const double TS_IN2_PER_M2        = 1550.003100006;
    const double TS_M2_PER_IN2        = 0.00064516;
    const double TS_FT2_PER_M2        = 10.76391041671;
    const double TS_M2_PER_FT2        = 0.09290304;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_IN2_PER_FT2, UnitConversion::IN2_PER_FT2_UTIL, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_FT2_PER_IN2, UnitConversion::FT2_PER_IN2, 1.0e-15);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_IN2_PER_M2, UnitConversion::IN2_PER_M2, 1.0e-09);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_M2_PER_IN2, UnitConversion::M2_PER_IN2_UTIL, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_FT2_PER_M2, UnitConversion::FT2_PER_M2, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_M2_PER_FT2, UnitConversion::M2_PER_FT2, 1.0e-16);

    // Volume
    const double TS_IN3_PER_FT3     = 1728.0 ;
    const double TS_FT3_PER_IN3     = 0.000578703704;
    const double TS_IN3_PER_M3      = 61023.7441;
    const double TS_M3_PER_IN3      = 1.6387064e-5;
    const double TS_FT3_PER_M3      = 35.3146667;
    const double TS_M3_PER_FT3      = 0.0283168466;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_IN3_PER_FT3, UnitConversion::IN3_PER_FT3, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_FT3_PER_IN3, UnitConversion::FT3_PER_IN3, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_IN3_PER_M3, UnitConversion::IN3_PER_M3, 1.0e-05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_M3_PER_IN3, UnitConversion::M3_PER_IN3, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_FT3_PER_M3, UnitConversion::FT3_PER_M3, 1.0e-07);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_M3_PER_FT3, UnitConversion::M3_PER_FT3, 1.0e-11);

    // Pressure
    const double TS_PA_PER_PSI        = 6894.75;
    const double TS_PSI_PER_PA        = 0.0001450378911491;
    const double TS_TORR_PER_PSI      = 51.71487786825;
    const double TS_PSI_PER_TORR      = 0.01933679515879;
    const double TS_ATM_PER_PSI       = 0.0680458919319;
    const double TS_PSI_PER_ATM       = 14.69596432068;
    const double TS_PA_PER_ATM        = 101325.0;
    const double TS_ATM_PER_PA        = 9.869232667e-6;
    const double TS_MMHG_PER_PSI      = 51.7;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PA_PER_PSI, UnitConversion::PA_PER_PSI, 1.0e-02);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PSI_PER_PA, UnitConversion::PSI_PER_PA, 1.0e-09);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_TORR_PER_PSI, UnitConversion::TORR_PER_PSI, 1.0e-04);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PSI_PER_TORR, UnitConversion::PSI_PER_TORR, 1.0e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_ATM_PER_PSI, UnitConversion::ATM_PER_PSI, 1.0e-07);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PSI_PER_ATM, UnitConversion::PSI_PER_ATM, 1.0e-04);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_ATM_PER_PA, UnitConversion::ATM_PER_PA, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PA_PER_ATM, UnitConversion::PA_PER_ATM, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_MMHG_PER_PSI, UnitConversion::MMHG_PER_PSI, 1.0e-01);

    // Weight, Mass
    const double TS_KG_PER_LBM        = 0.45359237;
    const double TS_G_PER_LBM         = 453.59237;
    const double TS_LBM_PER_KG        = 2.204622621849;
    const double TS_LBM_PER_G         = 0.002204622621849;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_KG_PER_LBM, UnitConversion::KG_PER_LBM, 1.0e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_G_PER_LBM, UnitConversion::G_PER_LBM, 1.0e-04);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_LBM_PER_KG, UnitConversion::LBM_PER_KG, 1.0e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_LBM_PER_G, UnitConversion::LBM_PER_G, 1.0e-09);

    // Mass flow
    const double TS_KG_PER_SEC_PER_LBM_PER_HR = 0.000125998;
    const double TS_LBM_PER_HR_PER_KG_PER_SEC = 7936.633915;
    const double TS_KG_PER_SEC_PER_LBM_PER_SEC = 0.45359237002;
    const double TS_LBM_PER_SEC_PER_KG_PER_SEC = 2.2046226217;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_KG_PER_SEC_PER_LBM_PER_HR, UnitConversion::KG_PER_SEC_PER_LBM_PER_HR, 1.0e-09);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_LBM_PER_HR_PER_KG_PER_SEC, UnitConversion::LBM_PER_HR_PER_KG_PER_SEC, 1.0e-02);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_KG_PER_SEC_PER_LBM_PER_SEC, UnitConversion::KG_PER_SEC_PER_LBM_PER_SEC, 1.0e-07);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_LBM_PER_SEC_PER_KG_PER_SEC, UnitConversion::LBM_PER_SEC_PER_KG_PER_SEC, 1.0e-06);

    // Energy
    const double TS_BTU_PER_J       = 0.00094781712;
    const double TS_J_PER_BTU       = 1055.05585;
    const double TS_KJ_PER_BTU      = 1.05505585;
    const double TS_BTU_PER_KJ      = 0.9478171227;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_BTU_PER_J, UnitConversion::BTU_PER_J, 1.0e-09);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_J_PER_BTU, UnitConversion::J_PER_BTU, 1.0e-03);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_KJ_PER_BTU, UnitConversion::KJ_PER_BTU, 1.0e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_BTU_PER_KJ, UnitConversion::BTU_PER_KJ, 1.0e-06);

    // Power
    const double TS_BTU_PER_W_S       = 0.0009478169879134;
    const double TS_W_S_PER_BTU       = 1055.056;
    const double TS_BTU_PER_HR_PER_W  = 3.4144;
    const double TS_W_PER_BTU_PER_HR  = 0.2928;
    const double TS_BTU_PER_HR_PER_KW = 3414.426;
    const double TS_KW_PER_BTU_PER_HR = 0.00029287;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_BTU_PER_W_S, UnitConversion::BTU_PER_W_S, 1.0e-09);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_W_S_PER_BTU, UnitConversion::W_S_PER_BTU, 1.0e-03);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_BTU_PER_HR_PER_W, UnitConversion::BTU_PER_HR_PER_W, 1.0e-02);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_W_PER_BTU_PER_HR, UnitConversion::W_PER_BTU_PER_HR, 1.0e-03);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_BTU_PER_HR_PER_KW, UnitConversion::BTU_PER_HR_PER_KW, 3.0e+00);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_KW_PER_BTU_PER_HR, UnitConversion::KW_PER_BTU_PER_HR, 1.0e-06);

    // Specific Heat
    const double TS_BTU_OVER_LBMF_PER_KJ_OVER_KGK = 0.23884589663;
    const double TS_KJ_OVER_KGK_PER_BTU_OVER_LBMF = 4.1868;
    const double TS_J_OVER_KGK_PER_BTU_OVER_LBMF  = 4186.8;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_BTU_OVER_LBMF_PER_KJ_OVER_KGK, UnitConversion::BTU_OVER_LBMF_PER_KJ_OVER_KGK, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_KJ_OVER_KGK_PER_BTU_OVER_LBMF, UnitConversion::KJ_OVER_KGK_PER_BTU_OVER_LBMF, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_J_OVER_KGK_PER_BTU_OVER_LBMF,  UnitConversion::J_OVER_KGK_PER_BTU_OVER_LBMF, 1.0e-16);

    // Temperature
    const double TS_F_PER_C         = 1.8;
    const double TS_C_PER_F         = 0.5555555555556;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_F_PER_C, UnitConversion::F_PER_C, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_C_PER_F, UnitConversion::C_PER_F, 1.0e-13);

    // Viscosity
    const double TS_LBF_OVER_FTHR_PER_CP = 2.4190881537;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_LBF_OVER_FTHR_PER_CP, UnitConversion::LBF_OVER_FTHR_PER_CP, 1.0e-16);

    // Percentage
    const double TS_PERCENTAGE = 100.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PERCENTAGE, UnitConversion::PERCENTAGE, 1.0e-16);

    // Angles
    const double TS_DEG_PER_RAD       = 57.2957795130823 ;
    const double TS_RAD_PER_DEG       = 0.0174532925199433 ;
    const double TS_MIL_PER_RAD       = 1018.591635788 ;
    const double TS_RAD_PER_MIL       = 9.81747704247e-4 ;
    const double TS_MIL_PER_DEG       = 17.777777777777777;
    const double TS_DEG_PER_MIL       = 0.05625;
    const double TS_RAD_PER_REV       = 6.28318531;
    const double TS_REV_PER_RAD       = 0.159154943;
    const double TS_RAD_PER_ARCSEC    = 4.84813681e-6;
    const double TS_ARCSEC_PER_RAD    = 206264.806293699;
    const double TS_RAD_PER_ARCMIN    = 2.90888209e-4;
    const double TS_ARCMIN_PER_RAD    = 3437.746766834;
    const double TS_PI                = 3.14159265358979323846;
    const double TS_TWO_PI            = 6.28318530717958647692;
    const double TS_SQRT_PI           = 1.77245385090551602729;
    const double TS_2SQRT_PI          = 3.54490770181103205459;
    const double TS_SQRT_2PI          = 2.50662827463100050241;
    const double TS_PI_OVER_2         = 1.57079632679489661923;
    const double TS_PI_OVER_3         = 1.04719755119659774615;
    const double TS_PI_OVER_4         = 0.78539816339744830962;
    const double TS_PI_OVER_6         = 0.52359877559829887377;
    const double TS_4_PI_OVER_3       = 4.18879020478639098461;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_DEG_PER_RAD, UnitConversion::DEG_PER_RAD_UTIL, 1.0e-13);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_RAD_PER_DEG, UnitConversion::RAD_PER_DEG, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_MIL_PER_RAD, UnitConversion::MIL_PER_RAD, 1.0e-09);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_RAD_PER_MIL, UnitConversion::RAD_PER_MIL, 1.0e-15);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_MIL_PER_DEG, UnitConversion::MIL_PER_DEG, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_DEG_PER_MIL, UnitConversion::DEG_PER_MIL, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_RAD_PER_REV, UnitConversion::RAD_PER_REV, 1.0e-08);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_RAD_PER_ARCSEC, UnitConversion::RAD_PER_ARCSEC, 1.0e-09);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_RAD_PER_ARCMIN, UnitConversion::RAD_PER_ARCMIN, 1.0e-12);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PI, UnitConversion::PI_UTIL, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_TWO_PI, UnitConversion::TWO_PI, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SQRT_PI, UnitConversion::SQRT_PI, 1.0e-15);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_2SQRT_PI, UnitConversion::TWO_SQRT_PI, 1.0e-15);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SQRT_2PI, UnitConversion::SQRT_2PI, 1.0e-15);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PI_OVER_2, UnitConversion::PI_OVER_2, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PI_OVER_3, UnitConversion::PI_OVER_3, 1.0e-15);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PI_OVER_4, UnitConversion::PI_OVER_4, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_PI_OVER_6, UnitConversion::PI_OVER_6, 1.0e-15);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_4_PI_OVER_3, UnitConversion::FOUR_PI_OVER_3, 1.0e-15);

    // Mathematical & physical constants
    const double TS_STEFAN_BOLTZMANN_CONST_SI     = 5.67051e-8;     // W / (M2 K4)
    const double TS_STEFAN_BOLTZMANN_CONST_ENG    = 0.1714e-8;      // BTU / (hr ft2 R4)
    const double TS_BOLTZMANN_CONST               = 1.3806504e-23;  // J/K
    const double TS_BOLTZMANN_CONST_dBW           = -228.599;       // dBW/ (K Hz)
    const double TS_STD_GRAVITY_SI                = 9.80665;        // m / s2
    const double TS_STD_GRAVITY_ENG               = 32.174;         // ft / s2
    const double TS_GC                            = 32.17;          // (ft lbm)/(lbf s2)
    const double TS_UNIV_GAS_CONST_ENG            = 1545.349;       // (lbf ft)/(lbmol R)
    const double TS_UNIV_GAS_CONST_SI             = 8.314472;       //   J/(mol K)
    const double TS_SPEED_LIGHT_SI                = 299792458.0;    //  m/s (exact-NIST)
    const double TS_SPEED_LIGHT_SQ_SI             = 89875517873681764.0;    //  m2 / s2
    const double TS_HP_PER_FTLBF_OVER_MIN         = 33000;          // ft*lbf/min
    const double TS_EARTH_EQUATORIAL_RADIUS       = 6378137.0;      // M
    const double TS_EARTH_POLAR_RADIUS            = 6356752.3;      // M
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_STEFAN_BOLTZMANN_CONST_SI, UnitConversion::STEFAN_BOLTZMANN_CONST_SI, 1.0e-11);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_STEFAN_BOLTZMANN_CONST_ENG, UnitConversion::STEFAN_BOLTZMANN_CONST_ENG, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_BOLTZMANN_CONST, UnitConversion::BOLTZMANN_CONST, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_BOLTZMANN_CONST_dBW, UnitConversion::BOLTZMANN_CONST_DBW, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_STD_GRAVITY_SI, UnitConversion::STD_GRAVITY_SI, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_STD_GRAVITY_ENG, UnitConversion::STD_GRAVITY_ENG, 1.0e-04);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_GC, UnitConversion::GC, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SPEED_LIGHT_SI, UnitConversion::SPEED_LIGHT_SI, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SPEED_LIGHT_SQ_SI, UnitConversion::SPEED_LIGHT_SQ_SI, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_UNIV_GAS_CONST_ENG, UnitConversion::UNIV_GAS_CONST_ENG, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_UNIV_GAS_CONST_SI, UnitConversion::UNIV_GAS_CONST_SI, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_HP_PER_FTLBF_OVER_MIN, UnitConversion::HP_PER_FTLBF_OVER_MIN, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_EARTH_EQUATORIAL_RADIUS, UnitConversion::EARTH_EQUATORIAL_RADIUS, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_EARTH_POLAR_RADIUS, UnitConversion::EARTH_POLAR_RADIUS, 1.0e-01);

    // Square roots
    const double TS_SQRT2      = 1.41421356237309504880168872421; //sqrt(2)
    const double TS_SQRT1_2    = 0.70710678118654752440084436210; //sqrt(1/2)
    const double TS_SQRT3      = 1.73205080756887729352744634151; //sqrt(3)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SQRT2, UnitConversion::SQRT2, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SQRT1_2, UnitConversion::SQRT1_2, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_SQRT3, UnitConversion::SQRT3, 1.0e-16);

    // EPS
    const double TS_ELECTRON_CHARGE = .1592e-18;                      //  Coulomb
    CPPUNIT_ASSERT_DOUBLES_EQUAL(TS_ELECTRON_CHARGE, UnitConversion::ELECTRON_CHARGE, 1.0e-16);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test comparison against Trick values with best tolerance that passes.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testCompareTrick()
{
    std::cout << "\n.Unit Conversion 11: Test Comparison Against Trick Values...................";

    CPPUNIT_ASSERT_DOUBLES_EQUAL(60.0, UnitConversion::SEC_PER_MIN, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3600.0, UnitConversion::SEC_PER_HR, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(86400.0, UnitConversion::SEC_PER_DAY, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(31536000.0, UnitConversion::SEC_PER_YEAR, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3048, UnitConversion::M_PER_FT, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0254, UnitConversion::M_PER_IN, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0174532925199433, UnitConversion::RAD_PER_DEG, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.848136811095362e-06, UnitConversion::RAD_PER_ARCSEC, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0002908882086657216, UnitConversion::RAD_PER_ARCMIN, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.28318530717958647, UnitConversion::RAD_PER_REV, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4535923697760192, UnitConversion::KG_PER_LBM, 1.0e-07);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-273.15, -UnitConversion::ZERO_C_IN_K, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.55555555555555556, UnitConversion::C_PER_F, 1.0e-16);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1055.056, UnitConversion::W_S_PER_BTU, 1.0e-03);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6894.7569999999996, UnitConversion::PA_PER_PSI, 1.0e-03);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(101325.00000, UnitConversion::PA_PER_ATM, 1.0e-16);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test comparison of current and previously recorded values.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testRegression()
{
    std::cout << "\n.Unit Conversion 12: Regression Test........................................";

//    These are the previous (GSL provided) values.
//    A leading * indicates a failed assertion using the new (NIST provided) values, so the change
//    was more than negligible.
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e-08 / UnitConversion::TEN_TO_THE_MINUS_8, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e-12 / UnitConversion::TEN_TO_THE_MINUS_12, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.640000000000000e+04 / UnitConversion::SEC_PER_DAY, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.600000000000000e+03 / UnitConversion::SEC_PER_HR, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.000000000000000e+01 / UnitConversion::SEC_PER_MIN, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.000000000000000e+01 / UnitConversion::MIN_PER_HR, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.777777777777778e-04 / UnitConversion::HR_PER_SEC, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.666666666666667e-02 / UnitConversion::HR_PER_MIN, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.400000000000000e+01 / UnitConversion::HR_PER_DAY, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.666666666666667e-02 / UnitConversion::MIN_PER_SEC, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT(365 == UnitConversion::DAYS_PER_YEAR);
//    CPPUNIT_ASSERT(12 == UnitConversion::MONTHS_PER_YEAR);
//    CPPUNIT_ASSERT(2200 == UnitConversion::MAX_YEAR);
//    CPPUNIT_ASSERT(1970 == UnitConversion::MIN_YEAR);
//    CPPUNIT_ASSERT(0 == UnitConversion::START_DAY_NORM[0]);
//    CPPUNIT_ASSERT(0 == UnitConversion::START_DAY_NORM[1]);
//    CPPUNIT_ASSERT(31 == UnitConversion::START_DAY_NORM[2]);
//    CPPUNIT_ASSERT(59 == UnitConversion::START_DAY_NORM[3]);
//    CPPUNIT_ASSERT(90 == UnitConversion::START_DAY_NORM[4]);
//    CPPUNIT_ASSERT(120 == UnitConversion::START_DAY_NORM[5]);
//    CPPUNIT_ASSERT(151 == UnitConversion::START_DAY_NORM[6]);
//    CPPUNIT_ASSERT(181 == UnitConversion::START_DAY_NORM[7]);
//    CPPUNIT_ASSERT(212 == UnitConversion::START_DAY_NORM[8]);
//    CPPUNIT_ASSERT(243 == UnitConversion::START_DAY_NORM[9]);
//    CPPUNIT_ASSERT(273 == UnitConversion::START_DAY_NORM[10]);
//    CPPUNIT_ASSERT(304 == UnitConversion::START_DAY_NORM[11]);
//    CPPUNIT_ASSERT(334 == UnitConversion::START_DAY_NORM[12]);
//    CPPUNIT_ASSERT(365 == UnitConversion::START_DAY_NORM[13]);
//    CPPUNIT_ASSERT(0 == UnitConversion::START_DAY_LEAP[0]);
//    CPPUNIT_ASSERT(0 == UnitConversion::START_DAY_LEAP[1]);
//    CPPUNIT_ASSERT(31 == UnitConversion::START_DAY_LEAP[2]);
//    CPPUNIT_ASSERT(60 == UnitConversion::START_DAY_LEAP[3]);
//    CPPUNIT_ASSERT(91 == UnitConversion::START_DAY_LEAP[4]);
//    CPPUNIT_ASSERT(121 == UnitConversion::START_DAY_LEAP[5]);
//    CPPUNIT_ASSERT(152 == UnitConversion::START_DAY_LEAP[6]);
//    CPPUNIT_ASSERT(182 == UnitConversion::START_DAY_LEAP[7]);
//    CPPUNIT_ASSERT(213 == UnitConversion::START_DAY_LEAP[8]);
//    CPPUNIT_ASSERT(244 == UnitConversion::START_DAY_LEAP[9]);
//    CPPUNIT_ASSERT(274 == UnitConversion::START_DAY_LEAP[10]);
//    CPPUNIT_ASSERT(305 == UnitConversion::START_DAY_LEAP[11]);
//    CPPUNIT_ASSERT(335 == UnitConversion::START_DAY_LEAP[12]);
//    CPPUNIT_ASSERT(366 == UnitConversion::START_DAY_LEAP[13]);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.162240000000000e+07 / UnitConversion::LEAP_YEAR_SEC, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.153600000000000e+07 / UnitConversion::COMMON_YEAR_SEC, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e+06 / UnitConversion::HZ_PER_MHZ, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e-06 / UnitConversion::MHZ_PER_HZ, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.200000000000000e+01 / UnitConversion::IN_PER_FT_UTIL, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.333333333333333e-02 / UnitConversion::FT_PER_IN, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.000000000000000e+00 / UnitConversion::FT_PER_YD, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.333333333333333e-01 / UnitConversion::YD_PER_FT, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.540000000000000e+00 / UnitConversion::CM_PER_IN, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.937007874015748e-01 / UnitConversion::IN_PER_CM, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.540000000000000e-02 / UnitConversion::M_PER_IN, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.937007874015748e+01 / UnitConversion::IN_PER_M, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.280839895013123e+00 / UnitConversion::FT_PER_M, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.048000000000000e-01 / UnitConversion::M_PER_FT, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e+03 / UnitConversion::M_PER_KM, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e-03 / UnitConversion::KM_PER_M, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.440000000000000e+02 / UnitConversion::IN2_PER_FT2_UTIL, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.944444444444444e-03 / UnitConversion::FT2_PER_IN2, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.550003100006200e+03 / UnitConversion::IN2_PER_M2, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.451600000000000e-04 / UnitConversion::M2_PER_IN2_UTIL, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.076391041670972e+01 / UnitConversion::FT2_PER_M2, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.290304000000001e-02 / UnitConversion::M2_PER_FT2, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.728000000000000e+03 / UnitConversion::IN3_PER_FT3, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.787037037037037e-04 / UnitConversion::FT3_PER_IN3, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.102374409473229e+04 / UnitConversion::IN3_PER_M3, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.638706400000000e-05 / UnitConversion::M3_PER_IN3, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.531466672148859e+01 / UnitConversion::FT3_PER_M3, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.831684659200000e-02 / UnitConversion::M3_PER_FT3, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(6.894757293170000e+03 / UnitConversion::PA_PER_PSI, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(1.450377377301748e-04 / UnitConversion::PSI_PER_PA, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(5.171493257153978e+01 / UnitConversion::TORR_PER_PSI, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(1.933677470461073e-02 / UnitConversion::PSI_PER_TORR, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(6.804596390989391e-02 / UnitConversion::ATM_PER_PSI, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(1.469594877550996e+01 / UnitConversion::PSI_PER_ATM, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.869232667160129e-06 / UnitConversion::ATM_PER_PA, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.013250000000000e+05 / UnitConversion::PA_PER_ATM, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(5.171493257153978e+01 / UnitConversion::MMHG_PER_PSI, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(4.535923700000000e-01 / UnitConversion::KG_PER_LBM, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(4.535923700000000e+02 / UnitConversion::G_PER_LBM, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(2.204622621848776e+00 / UnitConversion::LBM_PER_KG, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(2.204622621848776e-03 / UnitConversion::LBM_PER_G, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(1.259978805555556e-04 / UnitConversion::KG_PER_SEC_PER_LBM_PER_HR, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(7.936641438655593e+03 / UnitConversion::LBM_PER_HR_PER_KG_PER_SEC, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(4.535923700000000e-01 / UnitConversion::KG_PER_SEC_PER_LBM_PER_SEC, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(2.204622621848776e+00 / UnitConversion::LBM_PER_SEC_PER_KG_PER_SEC, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(9.478171203133172e-04 / UnitConversion::BTU_PER_J, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(1.055055852620000e+03 / UnitConversion::J_PER_BTU, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(1.055055852620000e+00 / UnitConversion::KJ_PER_BTU, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(9.478171203133172e-01 / UnitConversion::BTU_PER_KJ, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(9.478171203133172e-04 / UnitConversion::BTU_PER_W_S, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(1.055055852620000e+03 / UnitConversion::W_S_PER_BTU, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(3.412141633127942e+00 / UnitConversion::BTU_PER_HR_PER_W, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(2.930710701722222e-01 / UnitConversion::W_PER_BTU_PER_HR, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(3.412141633127942e+03 / UnitConversion::BTU_PER_HR_PER_KW, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(2.930710701722223e-04 / UnitConversion::KW_PER_BTU_PER_HR, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(2.388458966300000e-01 / UnitConversion::BTU_OVER_LBMF_PER_KJ_OVER_KGK, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(4.186800000000000e+00 / UnitConversion::KJ_OVER_KGK_PER_BTU_OVER_LBMF, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(4.186800000000000e+03 / UnitConversion::J_OVER_KGK_PER_BTU_OVER_LBMF, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.800000000000000e+00 / UnitConversion::F_PER_C, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.555555555555556e-01 / UnitConversion::C_PER_F, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.731500000000000e+02 / UnitConversion::ZERO_C_IN_K, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.596700000000000e+02 / UnitConversion::ZERO_F_IN_R, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.200000000000000e+01 / UnitConversion::ZERO_C_IN_F, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e+01 / UnitConversion::MILLIRAD_TO_MICROGRAY, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e+01 / UnitConversion::MILLIREM_TO_MICROSIEVERT, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.419088153700000e+00 / UnitConversion::LBF_OVER_FTHR_PER_CP, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.000000000000000e+02 / UnitConversion::PERCENTAGE, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.729577951308232e+01 / UnitConversion::DEG_PER_RAD_UTIL, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.745329251994330e-02 / UnitConversion::RAD_PER_DEG, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.018591635788130e+03 / UnitConversion::MIL_PER_RAD, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.817477042468104e-04 / UnitConversion::RAD_PER_MIL, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.777777777777778e+01 / UnitConversion::MIL_PER_DEG, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.624999999999999e-02 / UnitConversion::DEG_PER_MIL, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.283185307179586e+00 / UnitConversion::RAD_PER_REV, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.848136811095360e-06 / UnitConversion::RAD_PER_ARCSEC, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.908882086657216e-04 / UnitConversion::RAD_PER_ARCMIN, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.141592653589793e+00 / UnitConversion::PI_UTIL, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.283185307179586e+00 / UnitConversion::TWO_PI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.772453850905516e+00 / UnitConversion::SQRT_PI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.544907701811032e+00 / UnitConversion::TWO_SQRT_PI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.506628274631000e+00 / UnitConversion::SQRT_2PI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.570796326794897e+00 / UnitConversion::PI_OVER_2, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0471975511965976e+00 / UnitConversion::PI_OVER_3, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.853981633974483e-01 / UnitConversion::PI_OVER_4, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.235987755982988e-01 / UnitConversion::PI_OVER_6, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.188790204786391e+00 / UnitConversion::FOUR_PI_OVER_3, 1.0, DBL_EPSILON);
//   *CPPUNIT_ASSERT_DOUBLES_EQUAL(5.670400473740000e-08 / UnitConversion::STEFAN_BOLTZMANN_CONST_SI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.714000000000000e-09 / UnitConversion::STEFAN_BOLTZMANN_CONST_ENG, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.380650400000000e-23 / UnitConversion::BOLTZMANN_CONST, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.285990000000000e+02 / UnitConversion::BOLTZMANN_CONST_DBW, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.806649999999999e+00 / UnitConversion::STD_GRAVITY_SI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.217404855643044e+01 / UnitConversion::STD_GRAVITY_ENG, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.217000000000000e+01 / UnitConversion::GC, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.997924580000000e+08 / UnitConversion::SPEED_LIGHT_SI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.987551787368176e+16 / UnitConversion::SPEED_LIGHT_SQ_SI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.545349000000000e+03 / UnitConversion::UNIV_GAS_CONST_ENG, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.314472000000000e+00 / UnitConversion::UNIV_GAS_CONST_SI, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.300000000000000e+04 / UnitConversion::HP_PER_FTLBF_OVER_MIN, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.378137000000000e+06 / UnitConversion::EARTH_EQUATORIAL_RADIUS, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.356752314200000e+06 / UnitConversion::EARTH_POLAR_RADIUS, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.602176487000000e-19 / UnitConversion::ELECTRON_CHARGE, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.414213562373095e+00 / UnitConversion::SQRT2, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.071067811865476e-01 / UnitConversion::SQRT1_2, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.732050807568877e+00 / UnitConversion::SQRT3, 1.0, DBL_EPSILON);
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.660254037844386e-01 / UnitConversion::SQRT3_2, 1.0, DBL_EPSILON);

    //    These are the current (NIST provided) values.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e-08 / UnitConversion::TEN_TO_THE_MINUS_8, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.9999999999999998e-13 / UnitConversion::TEN_TO_THE_MINUS_12, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+03 / UnitConversion::MILLI_PER_UNIT, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e-03 / UnitConversion::UNIT_PER_MILLI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e-03 / UnitConversion::KILO_PER_UNIT, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+03 / UnitConversion::UNIT_PER_KILO, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e-06 / UnitConversion::MEGA_PER_UNIT, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+06 / UnitConversion::UNIT_PER_MEGA, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1536000000000000e+07 / UnitConversion::SEC_PER_YEAR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.6400000000000000e+04 / UnitConversion::SEC_PER_DAY, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.6000000000000000e+03 / UnitConversion::SEC_PER_HR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0000000000000000e+01 / UnitConversion::SEC_PER_MIN, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0000000000000000e+01 / UnitConversion::MIN_PER_HR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.7777777777777778e-04 / UnitConversion::HR_PER_SEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6666666666666666e-02 / UnitConversion::HR_PER_MIN, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.4000000000000000e+01 / UnitConversion::HR_PER_DAY, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6666666666666666e-02 / UnitConversion::MIN_PER_SEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT(365 == UnitConversion::DAYS_PER_YEAR);
    CPPUNIT_ASSERT(12 == UnitConversion::MONTHS_PER_YEAR);
    CPPUNIT_ASSERT(2200 == UnitConversion::MAX_YEAR);
    CPPUNIT_ASSERT(1970 == UnitConversion::MIN_YEAR);
    CPPUNIT_ASSERT(0 == UnitConversion::START_DAY_NORM[0]);
    CPPUNIT_ASSERT(0 == UnitConversion::START_DAY_NORM[1]);
    CPPUNIT_ASSERT(31 == UnitConversion::START_DAY_NORM[2]);
    CPPUNIT_ASSERT(59 == UnitConversion::START_DAY_NORM[3]);
    CPPUNIT_ASSERT(90 == UnitConversion::START_DAY_NORM[4]);
    CPPUNIT_ASSERT(120 == UnitConversion::START_DAY_NORM[5]);
    CPPUNIT_ASSERT(151 == UnitConversion::START_DAY_NORM[6]);
    CPPUNIT_ASSERT(181 == UnitConversion::START_DAY_NORM[7]);
    CPPUNIT_ASSERT(212 == UnitConversion::START_DAY_NORM[8]);
    CPPUNIT_ASSERT(243 == UnitConversion::START_DAY_NORM[9]);
    CPPUNIT_ASSERT(273 == UnitConversion::START_DAY_NORM[10]);
    CPPUNIT_ASSERT(304 == UnitConversion::START_DAY_NORM[11]);
    CPPUNIT_ASSERT(334 == UnitConversion::START_DAY_NORM[12]);
    CPPUNIT_ASSERT(365 == UnitConversion::START_DAY_NORM[13]);
    CPPUNIT_ASSERT(0 == UnitConversion::START_DAY_LEAP[0]);
    CPPUNIT_ASSERT(0 == UnitConversion::START_DAY_LEAP[1]);
    CPPUNIT_ASSERT(31 == UnitConversion::START_DAY_LEAP[2]);
    CPPUNIT_ASSERT(60 == UnitConversion::START_DAY_LEAP[3]);
    CPPUNIT_ASSERT(91 == UnitConversion::START_DAY_LEAP[4]);
    CPPUNIT_ASSERT(121 == UnitConversion::START_DAY_LEAP[5]);
    CPPUNIT_ASSERT(152 == UnitConversion::START_DAY_LEAP[6]);
    CPPUNIT_ASSERT(182 == UnitConversion::START_DAY_LEAP[7]);
    CPPUNIT_ASSERT(213 == UnitConversion::START_DAY_LEAP[8]);
    CPPUNIT_ASSERT(244 == UnitConversion::START_DAY_LEAP[9]);
    CPPUNIT_ASSERT(274 == UnitConversion::START_DAY_LEAP[10]);
    CPPUNIT_ASSERT(305 == UnitConversion::START_DAY_LEAP[11]);
    CPPUNIT_ASSERT(335 == UnitConversion::START_DAY_LEAP[12]);
    CPPUNIT_ASSERT(366 == UnitConversion::START_DAY_LEAP[13]);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1622400000000000e+07 / UnitConversion::LEAP_YEAR_SEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1536000000000000e+07 / UnitConversion::COMMON_YEAR_SEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+06 / UnitConversion::HZ_PER_MHZ, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.9999999999999995e-07 / UnitConversion::MHZ_PER_HZ, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+03 / UnitConversion::SEC_PER_MSEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.9999999999999995e-04 / UnitConversion::MSEC_PER_SEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2000000000000000e+01 / UnitConversion::IN_PER_FT_UTIL, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.3333333333333329e-02 / UnitConversion::FT_PER_IN, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0000000000000000e+00 / UnitConversion::FT_PER_YD, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.3333333333333331e-01 / UnitConversion::YD_PER_FT, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5400000000000000e+00 / UnitConversion::CM_PER_IN, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.9370078740157483e-01 / UnitConversion::IN_PER_CM, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5399999999999999e-02 / UnitConversion::M_PER_IN, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.9370078740157481e+01 / UnitConversion::IN_PER_M, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2808398950131230e+00 / UnitConversion::FT_PER_M, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0480000000000002e-01 / UnitConversion::M_PER_FT, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+03 / UnitConversion::M_PER_KM, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e-03 / UnitConversion::KM_PER_M, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.4400000000000000e+02 / UnitConversion::IN2_PER_FT2_UTIL, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.9444444444444441e-03 / UnitConversion::FT2_PER_IN2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5500031000062002e+03 / UnitConversion::IN2_PER_M2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.4515999999999998e-04 / UnitConversion::M2_PER_IN2_UTIL, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0763910416709722e+01 / UnitConversion::FT2_PER_M2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.2903040000000006e-02 / UnitConversion::M2_PER_FT2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.7280000000000000e+03 / UnitConversion::IN3_PER_FT3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.7870370370370367e-04 / UnitConversion::FT3_PER_IN3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.1023744094732290e+04 / UnitConversion::IN3_PER_M3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6387063999999999e-05 / UnitConversion::M3_PER_IN3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.5314666721488585e+01 / UnitConversion::FT3_PER_M3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.8316846592000004e-02 / UnitConversion::M3_PER_FT3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.9999999999999995e-07 / UnitConversion::M3_PER_ML, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.8947569999999996e+03 / UnitConversion::PA_PER_PSI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.8947569999999996e+00 / UnitConversion::KPA_PER_PSI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.4503774389728312e-04 / UnitConversion::PSI_PER_PA, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.4503774389728312e-01 / UnitConversion::PSI_PER_KPA, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.1714918123286111e+01 / UnitConversion::TORR_PER_PSI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5006150504341367e+00 / UnitConversion::TORR_PER_KPA, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.9336780106971138e-02 / UnitConversion::PSI_PER_TORR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.8045961016530959e-02 / UnitConversion::ATM_PER_PSI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.4695949400392212e+01 / UnitConversion::PSI_PER_ATM, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8692326671601285e-06 / UnitConversion::ATM_PER_PA, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0132500000000000e+05 / UnitConversion::PA_PER_ATM, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.1714918123286111e+01 / UnitConversion::MMHG_PER_PSI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.3332240000000000e-01 / UnitConversion::KPA_PER_MMHG, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+03 / UnitConversion::PA_PER_KPA, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8066500000000001e-03 / UnitConversion::KPA_PER_MMH2O, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8066500000000005e+01 / UnitConversion::KPA_PER_KGF_PER_CM2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e-03 / UnitConversion::KPA_PER_PA, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5359240000000001e-01 / UnitConversion::KG_PER_LBM, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5359240000000000e+02 / UnitConversion::G_PER_LBM, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.2046224760379585e+00 / UnitConversion::LBM_PER_KG, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.2046224760379584e-03 / UnitConversion::LBM_PER_G, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2599788888888888e-04 / UnitConversion::KG_PER_SEC_PER_LBM_PER_HR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.9366409137366509e+03 / UnitConversion::LBM_PER_HR_PER_KG_PER_SEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5359240000000001e-01 / UnitConversion::KG_PER_SEC_PER_LBM_PER_SEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.2046224760379585e+00 / UnitConversion::LBM_PER_SEC_PER_KG_PER_SEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.1298482902761670e-01 / UnitConversion::N_M_PER_IN_LBF, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.8507457913271850e+00 / UnitConversion::IN_LBF_PER_N_M, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.4482216000000001e+00 / UnitConversion::N_PER_LBF, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.2480894387096181e-01 / UnitConversion::LBF_PER_N, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.4781698791343778e-04 / UnitConversion::BTU_PER_J, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0550560000000000e+03 / UnitConversion::J_PER_BTU, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0550560000000000e+00 / UnitConversion::KJ_PER_BTU, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.4781698791343783e-01 / UnitConversion::BTU_PER_KJ, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.4781698791343778e-04 / UnitConversion::BTU_PER_W_S, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0550560000000000e+03 / UnitConversion::W_S_PER_BTU, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.4121411564883761e+00 / UnitConversion::BTU_PER_HR_PER_W, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.9307111111111112e-01 / UnitConversion::W_PER_BTU_PER_HR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.4121411564883761e+03 / UnitConversion::BTU_PER_HR_PER_KW, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.9307111111111114e-04 / UnitConversion::KW_PER_BTU_PER_HR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.3884589663000000e-01 / UnitConversion::BTU_OVER_LBMF_PER_KJ_OVER_KGK, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.1867999999999999e+00 / UnitConversion::KJ_OVER_KGK_PER_BTU_OVER_LBMF, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.1868000000000002e+03 / UnitConversion::J_OVER_KGK_PER_BTU_OVER_LBMF, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.8000000000000000e+00 / UnitConversion::F_PER_C, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.5555555555555558e-01 / UnitConversion::C_PER_F, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.7314999999999998e+02 / UnitConversion::ZERO_C_IN_K, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5967000000000002e+02 / UnitConversion::ZERO_F_IN_R, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2000000000000000e+01 / UnitConversion::ZERO_C_IN_F, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+01  / UnitConversion::MILLIRAD_TO_MICROGRAY, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+01  / UnitConversion::MILLIREM_TO_MICROSIEVERT, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.4190881537000002e+00 / UnitConversion::LBF_OVER_FTHR_PER_CP, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+02 / UnitConversion::PERCENTAGE, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.7295779513082323e+01 / UnitConversion::DEG_PER_RAD_UTIL, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.7453292519943295e-02 / UnitConversion::RAD_PER_DEG, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0185916357881301e+03 / UnitConversion::MIL_PER_RAD, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8174770424681044e-04 / UnitConversion::RAD_PER_MIL, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.5492965855137211e+00 / UnitConversion::SEC_PER_MIN_PER_2PI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.7777777777777779e+01 / UnitConversion::MIL_PER_DEG, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6249999999999994e-02 / UnitConversion::DEG_PER_MIL, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.2831853071795862e+00 / UnitConversion::RAD_PER_REV, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.6000000000000000e+02 / UnitConversion::DEG_PER_REV, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5915494309189535e-01 / UnitConversion::REV_PER_RAD, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.7777777777777779e-03 / UnitConversion::REV_PER_DEG, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.8481368110953598e-06 / UnitConversion::RAD_PER_ARCSEC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.9088820866572158e-04 / UnitConversion::RAD_PER_ARCMIN, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.1415926535897931e+00 / UnitConversion::PI_UTIL, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.2831853071795862e+00 / UnitConversion::TWO_PI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.7724538509055159e+00 / UnitConversion::SQRT_PI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.5449077018110318e+00 / UnitConversion::TWO_SQRT_PI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5066282746310002e+00 / UnitConversion::SQRT_2PI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5707963267948966e+00 / UnitConversion::PI_OVER_2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0471975511965976e+00 / UnitConversion::PI_OVER_3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.8539816339744828e-01 / UnitConversion::PI_OVER_4, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.2359877559829882e-01 / UnitConversion::PI_OVER_6, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.1887902047863905e+00 / UnitConversion::FOUR_PI_OVER_3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e+03 / UnitConversion::STD_DENSITY_WATER, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0000000000000000e-03 / UnitConversion::STD_SPECIFIC_VOLUME_WATER, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.2040999999999999e+00 / UnitConversion::STD_DENSITY_AIR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.3049580599617978e-01 / UnitConversion::STD_SPECIFIC_VOLUME_AIR, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(5.6704000000000003e-08 / UnitConversion::STEFAN_BOLTZMANN_CONST_SI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.7140000000000001e-09 / UnitConversion::STEFAN_BOLTZMANN_CONST_ENG, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.3806504000000001e-23 / UnitConversion::BOLTZMANN_CONST, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.2859899999999999e+02 / UnitConversion::BOLTZMANN_CONST_DBW, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.8066499999999994e+00 / UnitConversion::STD_GRAVITY_SI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2174048556430442e+01 / UnitConversion::STD_GRAVITY_ENG, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.2170000000000002e+01 / UnitConversion::GC, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.9979245800000000e+08 / UnitConversion::SPEED_LIGHT_SI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9875517873681760e+16 / UnitConversion::SPEED_LIGHT_SQ_SI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5453489999999999e+03 / UnitConversion::UNIV_GAS_CONST_ENG, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.3144720000000003e+00 / UnitConversion::UNIV_GAS_CONST_SI, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.3000000000000000e+04 / UnitConversion::HP_PER_FTLBF_OVER_MIN, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.3781370000000000e+06 / UnitConversion::EARTH_EQUATORIAL_RADIUS, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(6.3567523141999999e+06 / UnitConversion::EARTH_POLAR_RADIUS, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.6021764870000000e-19 / UnitConversion::ELECTRON_CHARGE, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.4142135623730951e+00 / UnitConversion::SQRT2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0710678118654757e-01 / UnitConversion::SQRT1_2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.7320508075688772e+00 / UnitConversion::SQRT3, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.6602540378443860e-01 / UnitConversion::SQRT3_2, 1.0, DBL_EPSILON);
    CPPUNIT_ASSERT(256                                 == UnitConversion::ONE_BYTE);
    CPPUNIT_ASSERT(65536                               == UnitConversion::TWO_BYTES);

    std::cout << "Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test the convert method and that the convert method function pointer array is
///           completely loaded.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::testConvert()
{
    std::cout << "\n.Unit Conversion 13: Tests Convert Method...................................";

    /// Pass bad values of type to the function and verify exception is thrown.
    CPPUNIT_ASSERT_THROW(UnitConversion::convert(static_cast<UnitConversion::Type>(-1),
                                                 0.0), TsOutOfBoundsException);
    CPPUNIT_ASSERT_THROW(UnitConversion::convert(UnitConversion::NUM_CONVERSIONS,
                                                 0.0), TsOutOfBoundsException);

    /// Test the no conversion option.
    CPPUNIT_ASSERT(42.0 == UnitConversion::convert(UnitConversion::NO_CONVERSION, 42.0));

    /// Loop through the function pointer array and verify that all pointers are set.  Note this
    /// test will fail if any conversion functions are added that ever convert 42.0 to 0.0, but as
    /// 42.0 is a magic number that doesn't mean anything (or does it?), this is unlikely.
    for (int i=0; i<UnitConversion::NUM_CONVERSIONS; ++i) {
        CPPUNIT_ASSERT(0.0 != UnitConversion::convert(static_cast<UnitConversion::Type>(i), 42.0));
    }

    std::cout << "Pass.\n.";
    std::cout << "................................................................................";
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Print the current value of the conversion factors and constants.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtUnitConversion::printValues()
{
    std::cout.setf(std::ios_base::scientific);
    std::cout.precision(16);
    std::cout << "Unit Conversion Values..........................................................\n"
        << UnitConversion::TEN_TO_THE_MINUS_8  << " TEN_TO_THE_MINUS_8" << '\n'
        << UnitConversion::TEN_TO_THE_MINUS_12 << " TEN_TO_THE_MINUS_12" << '\n'
        << UnitConversion::MILLI_PER_UNIT << " MILLI_PER_UNIT" << '\n'
        << UnitConversion::UNIT_PER_MILLI << " UNIT_PER_MILLI" << '\n'
        << UnitConversion::KILO_PER_UNIT << " KILO_PER_UNIT" << '\n'
        << UnitConversion::UNIT_PER_KILO << " UNIT_PER_KILO" << '\n'
        << UnitConversion::MEGA_PER_UNIT << " MEGA_PER_UNIT" << '\n'
        << UnitConversion::UNIT_PER_MEGA << " UNIT_PER_MEGA" << '\n'
        << UnitConversion::SEC_PER_YEAR << " SEC_PER_YEAR" << '\n'
        << UnitConversion::SEC_PER_DAY << " SEC_PER_DAY" << '\n'
        << UnitConversion::SEC_PER_HR  << " SEC_PER_HR" << '\n'
        << UnitConversion::SEC_PER_MIN << " SEC_PER_MIN" << '\n'
        << UnitConversion::MIN_PER_HR  << " MIN_PER_HR" << '\n'
        << UnitConversion::HR_PER_SEC  << " HR_PER_SEC" << '\n'
        << UnitConversion::HR_PER_MIN  << " HR_PER_MIN" << '\n'
        << UnitConversion::HR_PER_DAY  << " HR_PER_DAY" << '\n'
        << UnitConversion::MIN_PER_SEC << " MIN_PER_SEC" << '\n'
        << UnitConversion::DAYS_PER_YEAR   << " DAYS_PER_YEAR" << '\n'
        << UnitConversion::MONTHS_PER_YEAR << " MONTHS_PER_YEAR" << '\n'
        << UnitConversion::MAX_YEAR    << " MAX_YEAR" << '\n'
        << UnitConversion::MIN_YEAR    << " MIN_YEAR" << '\n'
        << UnitConversion::START_DAY_NORM[0]   << " START_DAY_NORM[0]" << '\n'
        << UnitConversion::START_DAY_NORM[1]   << " START_DAY_NORM[1]" << '\n'
        << UnitConversion::START_DAY_NORM[2]   << " START_DAY_NORM[2]" << '\n'
        << UnitConversion::START_DAY_NORM[3]   << " START_DAY_NORM[3]" << '\n'
        << UnitConversion::START_DAY_NORM[4]   << " START_DAY_NORM[4]" << '\n'
        << UnitConversion::START_DAY_NORM[5]   << " START_DAY_NORM[5]" << '\n'
        << UnitConversion::START_DAY_NORM[6]   << " START_DAY_NORM[6]" << '\n'
        << UnitConversion::START_DAY_NORM[7]   << " START_DAY_NORM[7]" << '\n'
        << UnitConversion::START_DAY_NORM[8]   << " START_DAY_NORM[8]" << '\n'
        << UnitConversion::START_DAY_NORM[9]   << " START_DAY_NORM[9]" << '\n'
        << UnitConversion::START_DAY_NORM[10]  << " START_DAY_NORM[10]" << '\n'
        << UnitConversion::START_DAY_NORM[11]  << " START_DAY_NORM[11]" << '\n'
        << UnitConversion::START_DAY_NORM[12]  << " START_DAY_NORM[12]" << '\n'
        << UnitConversion::START_DAY_NORM[13]  << " START_DAY_NORM[13]" << '\n'
        << UnitConversion::START_DAY_LEAP[0]   << " START_DAY_LEAP[0]" << '\n'
        << UnitConversion::START_DAY_LEAP[1]   << " START_DAY_LEAP[1]" << '\n'
        << UnitConversion::START_DAY_LEAP[2]   << " START_DAY_LEAP[2]" << '\n'
        << UnitConversion::START_DAY_LEAP[3]   << " START_DAY_LEAP[3]" << '\n'
        << UnitConversion::START_DAY_LEAP[4]   << " START_DAY_LEAP[4]" << '\n'
        << UnitConversion::START_DAY_LEAP[5]   << " START_DAY_LEAP[5]" << '\n'
        << UnitConversion::START_DAY_LEAP[6]   << " START_DAY_LEAP[6]" << '\n'
        << UnitConversion::START_DAY_LEAP[7]   << " START_DAY_LEAP[7]" << '\n'
        << UnitConversion::START_DAY_LEAP[8]   << " START_DAY_LEAP[8]" << '\n'
        << UnitConversion::START_DAY_LEAP[9]   << " START_DAY_LEAP[9]" << '\n'
        << UnitConversion::START_DAY_LEAP[10]  << " START_DAY_LEAP[10]" << '\n'
        << UnitConversion::START_DAY_LEAP[11]  << " START_DAY_LEAP[11]" << '\n'
        << UnitConversion::START_DAY_LEAP[12]  << " START_DAY_LEAP[12]" << '\n'
        << UnitConversion::START_DAY_LEAP[13]  << " START_DAY_LEAP[13]" << '\n'
        << UnitConversion::LEAP_YEAR_SEC   << " LEAP_YEAR_SEC" << '\n'
        << UnitConversion::COMMON_YEAR_SEC << " COMMON_YEAR_SEC" << '\n'
        << UnitConversion::SEC_PER_MSEC  << " SEC_PER_MSEC" << '\n'
        << UnitConversion::MSEC_PER_SEC  << " MSEC_PER_SEC" << '\n'
        << UnitConversion::HZ_PER_MHZ  << " HZ_PER_MHZ" << '\n'
        << UnitConversion::MHZ_PER_HZ  << " MHZ_PER_HZ" << '\n'
        << UnitConversion::IN_PER_FT_UTIL   << " IN_PER_FT_UTIL" << '\n'
        << UnitConversion::FT_PER_IN   << " FT_PER_IN" << '\n'
        << UnitConversion::FT_PER_YD   << " FT_PER_YD" << '\n'
        << UnitConversion::YD_PER_FT   << " YD_PER_FT" << '\n'
        << UnitConversion::CM_PER_IN   << " CM_PER_IN" << '\n'
        << UnitConversion::IN_PER_CM   << " IN_PER_CM" << '\n'
        << UnitConversion::M_PER_IN    << " M_PER_IN" << '\n'
        << UnitConversion::IN_PER_M    << " IN_PER_M" << '\n'
        << UnitConversion::FT_PER_M    << " FT_PER_M" << '\n'
        << UnitConversion::M_PER_FT    << " M_PER_FT" << '\n'
        << UnitConversion::M_PER_KM    << " M_PER_KM" << '\n'
        << UnitConversion::KM_PER_M    << " KM_PER_M" << '\n'
        << UnitConversion::IN2_PER_FT2_UTIL << " IN2_PER_FT2_UTIL" << '\n'
        << UnitConversion::FT2_PER_IN2 << " FT2_PER_IN2" << '\n'
        << UnitConversion::IN2_PER_M2  << " IN2_PER_M2" << '\n'
        << UnitConversion::M2_PER_IN2_UTIL  << " M2_PER_IN2_UTIL" << '\n'
        << UnitConversion::M2_PER_FT2  << " M2_PER_FT2" << '\n'
        << UnitConversion::FT2_PER_M2  << " FT2_PER_M2" << '\n'
        << UnitConversion::IN3_PER_FT3 << " IN3_PER_FT3" << '\n'
        << UnitConversion::FT3_PER_IN3 << " FT3_PER_IN3" << '\n'
        << UnitConversion::IN3_PER_M3  << " IN3_PER_M3" << '\n'
        << UnitConversion::M3_PER_IN3  << " M3_PER_IN3" << '\n'
        << UnitConversion::FT3_PER_M3  << " FT3_PER_M3" << '\n'
        << UnitConversion::M3_PER_FT3  << " M3_PER_FT3" << '\n'
        << UnitConversion::M3_PER_ML  << " M3_PER_ML" << '\n'
        << UnitConversion::PA_PER_PSI  << " PA_PER_PSI" << '\n'
        << UnitConversion::KPA_PER_PSI << " KPA_PER_PSI" << '\n'
        << UnitConversion::PSI_PER_PA  << " PSI_PER_PA" << '\n'
        << UnitConversion::PSI_PER_KPA  << " PSI_PER_KPA" << '\n'
        << UnitConversion::TORR_PER_PSI    << " TORR_PER_PSI" << '\n'
        << UnitConversion::TORR_PER_KPA    << " TORR_PER_KPA" << '\n'
        << UnitConversion::PSI_PER_TORR    << " PSI_PER_TORR" << '\n'
        << UnitConversion::ATM_PER_PSI << " ATM_PER_PSI" << '\n'
        << UnitConversion::PSI_PER_ATM << " PSI_PER_ATM" << '\n'
        << UnitConversion::ATM_PER_PA  << " ATM_PER_PA" << '\n'
        << UnitConversion::PA_PER_ATM  << " PA_PER_ATM" << '\n'
        << UnitConversion::MMHG_PER_PSI    << " MMHG_PER_PSI" << '\n'
        << UnitConversion::KPA_PER_MMHG    << " KPA_PER_MMHG" << '\n'
        << UnitConversion::PA_PER_KPA    << " PA_PER_KPA" << '\n'
        << UnitConversion::KPA_PER_MMH2O    << " KPA_PER_MMH2O" << '\n'
        << UnitConversion::KPA_PER_KGF_PER_CM2    << " KPA_PER_KGF_PER_CM2" << '\n'
        << UnitConversion::KPA_PER_PA    << " KPA_PER_PA" << '\n'
        << UnitConversion::KG_PER_LBM  << " KG_PER_LBM" << '\n'
        << UnitConversion::G_PER_LBM   << " G_PER_LBM" << '\n'
        << UnitConversion::LBM_PER_KG  << " LBM_PER_KG" << '\n'
        << UnitConversion::LBM_PER_G   << " LBM_PER_G" << '\n'
        << UnitConversion::KG_PER_SEC_PER_LBM_PER_HR   << " KG_PER_SEC_PER_LBM_PER_HR" << '\n'
        << UnitConversion::LBM_PER_HR_PER_KG_PER_SEC   << " LBM_PER_HR_PER_KG_PER_SEC" << '\n'
        << UnitConversion::KG_PER_SEC_PER_LBM_PER_SEC  << " KG_PER_SEC_PER_LBM_PER_SEC" << '\n'
        << UnitConversion::LBM_PER_SEC_PER_KG_PER_SEC  << " LBM_PER_SEC_PER_KG_PER_SEC" << '\n'
        << UnitConversion::N_PER_LBF << " N_PER_LBF" << '\n'
        << UnitConversion::LBF_PER_N << " LBF_PER_N" << '\n'
        << UnitConversion::N_M_PER_IN_LBF << " N_M_PER_IN_LBF" << '\n'
        << UnitConversion::IN_LBF_PER_N_M << " IN_LBF_PER_N_M" << '\n'
        << UnitConversion::BTU_PER_J   << " BTU_PER_J" << '\n'
        << UnitConversion::J_PER_BTU   << " J_PER_BTU" << '\n'
        << UnitConversion::KJ_PER_BTU  << " KJ_PER_BTU" << '\n'
        << UnitConversion::BTU_PER_KJ  << " BTU_PER_KJ" << '\n'
        << UnitConversion::BTU_PER_W_S << " BTU_PER_W_S" << '\n'
        << UnitConversion::W_S_PER_BTU << " W_S_PER_BTU" << '\n'
        << UnitConversion::W_PER_BTU_PER_HR    << " W_PER_BTU_PER_HR" << '\n'
        << UnitConversion::BTU_PER_HR_PER_W    << " BTU_PER_HR_PER_W" << '\n'
        << UnitConversion::BTU_PER_HR_PER_KW   << " BTU_PER_HR_PER_KW" << '\n'
        << UnitConversion::KW_PER_BTU_PER_HR   << " KW_PER_BTU_PER_HR" << '\n'
        << UnitConversion::LBF_OVER_FTHR_PER_CP    << " LBF_OVER_FTHR_PER_CP" << '\n'
        << UnitConversion::BTU_OVER_LBMF_PER_KJ_OVER_KGK   << " BTU_OVER_LBMF_PER_KJ_OVER_KGK" << '\n'
        << UnitConversion::KJ_OVER_KGK_PER_BTU_OVER_LBMF   << " KJ_OVER_KGK_PER_BTU_OVER_LBMF" << '\n'
        << UnitConversion::J_OVER_KGK_PER_BTU_OVER_LBMF    << " J_OVER_KGK_PER_BTU_OVER_LBMF" << '\n'
        << UnitConversion::F_PER_C << " F_PER_C" << '\n'
        << UnitConversion::C_PER_F << " C_PER_F" << '\n'
        << UnitConversion::ZERO_C_IN_K << " ZERO_C_IN_K" << '\n'
        << UnitConversion::ZERO_F_IN_R << " ZERO_F_IN_R" << '\n'
        << UnitConversion::ZERO_C_IN_F << " ZERO_C_IN_F" << '\n'
        << UnitConversion::MILLIRAD_TO_MICROGRAY   << " MILLIRAD_TO_MICROGRAY" << '\n'
        << UnitConversion::MILLIREM_TO_MICROSIEVERT   << " MILLIREM_TO_MICROSIEVERT" << '\n'
        << UnitConversion::PERCENTAGE  << " PERCENTAGE" << '\n'
        << UnitConversion::DEG_PER_RAD_UTIL << " DEG_PER_RAD_UTIL" << '\n'
        << UnitConversion::RAD_PER_DEG << " RAD_PER_DEG" << '\n'
        << UnitConversion::MIL_PER_RAD << " MIL_PER_RAD" << '\n'
        << UnitConversion::RAD_PER_MIL << " RAD_PER_MIL" << '\n'
        << UnitConversion::SEC_PER_MIN_PER_2PI << " SEC_PER_MIN_PER_2PI" << '\n'
        << UnitConversion::MIL_PER_DEG << " MIL_PER_DEG" << '\n'
        << UnitConversion::DEG_PER_MIL << " DEG_PER_MIL" << '\n'
        << UnitConversion::RAD_PER_REV << " RAD_PER_REV" << '\n'
        << UnitConversion::DEG_PER_REV << " DEG_PER_REV" << '\n'
        << UnitConversion::REV_PER_RAD << " REV_PER_RAD" << '\n'
        << UnitConversion::REV_PER_DEG << " REV_PER_DEG" << '\n'
        << UnitConversion::RAD_PER_ARCSEC  << " RAD_PER_ARCSEC" << '\n'
        << UnitConversion::RAD_PER_ARCMIN  << " RAD_PER_ARCMIN" << '\n'
        << UnitConversion::PI_UTIL  << " PI" << '\n'
        << UnitConversion::TWO_PI  << " TWO_PI" << '\n'
        << UnitConversion::SQRT_PI << " SQRT_PI" << '\n'
        << UnitConversion::TWO_SQRT_PI << " TWO_SQRT_PI" << '\n'
        << UnitConversion::SQRT_2PI    << " SQRT_2PI" << '\n'
        << UnitConversion::PI_OVER_2   << " PI_OVER_2" << '\n'
        << UnitConversion::PI_OVER_3   << " PI_OVER_3" << '\n'
        << UnitConversion::PI_OVER_4   << " PI_OVER_4" << '\n'
        << UnitConversion::PI_OVER_6   << " PI_OVER_6" << '\n'
        << UnitConversion::FOUR_PI_OVER_3  << " FOUR_PI_OVER_3" << '\n'
        << UnitConversion::STD_DENSITY_WATER   << " STD_DENSITY_WATER" << '\n'
        << UnitConversion::STD_SPECIFIC_VOLUME_WATER   << " STD_SPECIFIC_VOLUME_WATER" << '\n'
        << UnitConversion::STD_DENSITY_AIR   << " STD_DENSITY_AIR" << '\n'
        << UnitConversion::STD_SPECIFIC_VOLUME_AIR   << " STD_SPECIFIC_VOLUME_AIR" << '\n'
        << UnitConversion::STEFAN_BOLTZMANN_CONST_SI   << " STEFAN_BOLTZMANN_CONST_SI" << '\n'
        << UnitConversion::STEFAN_BOLTZMANN_CONST_ENG  << " STEFAN_BOLTZMANN_CONST_ENG" << '\n'
        << UnitConversion::BOLTZMANN_CONST << " BOLTZMANN_CONST" << '\n'
        << UnitConversion::BOLTZMANN_CONST_DBW << " BOLTZMANN_CONST_DBW" << '\n'
        << UnitConversion::STD_GRAVITY_SI  << " STD_GRAVITY_SI" << '\n'
        << UnitConversion::STD_GRAVITY_ENG << " STD_GRAVITY_ENG" << '\n'
        << UnitConversion::GC  << " GC" << '\n'
        << UnitConversion::SPEED_LIGHT_SI  << " SPEED_LIGHT_SI" << '\n'
        << UnitConversion::SPEED_LIGHT_SQ_SI   << " SPEED_LIGHT_SQ_SI" << '\n'
        << UnitConversion::UNIV_GAS_CONST_ENG  << " UNIV_GAS_CONST_ENG" << '\n'
        << UnitConversion::UNIV_GAS_CONST_SI   << " UNIV_GAS_CONST_SI" << '\n'
        << UnitConversion::HP_PER_FTLBF_OVER_MIN   << " HP_PER_FTLBF_OVER_MIN" << '\n'
        << UnitConversion::EARTH_EQUATORIAL_RADIUS << " EARTH_EQUATORIAL_RADIUS" << '\n'
        << UnitConversion::EARTH_POLAR_RADIUS  << " EARTH_POLAR_RADIUS" << '\n'
        << UnitConversion::ELECTRON_CHARGE << " ELECTRON_CHARGE" << '\n'
        << UnitConversion::SQRT2   << " SQRT2" << '\n'
        << UnitConversion::SQRT1_2 << " SQRT1_2" << '\n'
        << UnitConversion::SQRT3   << " SQRT3" << '\n'
        << UnitConversion::SQRT3_2 << " SQRT3_2" << '\n'
        << UnitConversion::ONE_BYTE << " ONE_BYTE" << '\n'
        << UnitConversion::TWO_BYTES << " TWO_BYTES" << '\n'
        << std::endl;
}
