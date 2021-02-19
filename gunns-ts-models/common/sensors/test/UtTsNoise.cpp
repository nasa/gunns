/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((common/sensors/TsNoise.o))
 ***************************************************************************************************/

#include "UtTsNoise.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsNoise::UtTsNoise()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructor
////////////////////////////////////////////////////////////////////////////////////////////////////
UtTsNoise::~UtTsNoise()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsNoise::setUp()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  unit test infrastructure
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsNoise::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the distribution range.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsNoise::testRange()
{
    std::cout << "\n--------------------------------------------------------------------------------";
    std::cout << "\n UtTsNoise ............ 01: testRange..................................";

    // Produce 1,000,000 samples of noise and show that they all fall within [-1,1] with no rejections.
    TsNoise::initialize(314159265);
    for (int i = 0; i <= 1000000; ++i) {
        const double noise = TsNoise::getNoise();
        CPPUNIT_ASSERT(-1.0 <= noise && noise <= 1.0);
        CPPUNIT_ASSERT(noise != 0.0); // on rejection noise = 0.0
    }

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the distribution mean.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsNoise::testMean()
{
    std::cout << "\n UtTsNoise ............ 02: testMean...................................";

    // Mean of 1,000,000 samples of noise is close to zero.
    TsNoise::initialize(23);
    double sum = 0.0;
    for (int i = 0; i <= 1000000; ++i) {
        sum += TsNoise::getNoise();
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, sum * 1.0e-06, 0.001);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the repeatability.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsNoise::testRepeatability()
{
    std::cout << "\n UtTsNoise ............ 03: testRepeatability..........................";

    TsNoise::initialize(123456789);
    double expected;
    for (int i = 0; i < 10000; ++i) {
        expected = TsNoise::getNoise();
    }

    TsNoise::initialize(123456789);
    double returned;
    for (int i = 0; i < 10000; ++i) {
        returned = TsNoise::getNoise();
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 0.0);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the randomness.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsNoise::testRandom()
{
    std::cout << "\n UtTsNoise ............ 04: testRandom.................................";

    TsNoise::initialize(0);
    const double expected = TsNoise::getNoise();
    const double returned = TsNoise::getNoise();

    // Yes, the test is that the exact same value is not returned twice in a row.
    CPPUNIT_ASSERT(expected != returned);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Tests the accessor functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtTsNoise::testAccessors()
{
    std::cout << "\n UtTsNoise ............ 05: testAccessors..............................";

    typedef double (*f_ptr)();
    f_ptr ptr = TsNoise::getNoiseFunction();
    CPPUNIT_ASSERT(ptr == TsNoise::getNoise);

    std::cout << "... Pass";
}
