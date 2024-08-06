/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    (
        (math/TS_random.o)
    )
***************************************************************************************************/

#include <cstdlib>
#include <iostream>
#include <vector>
#include "math/TS_random.hh"
#include "UtRandom.hh"

class FriendlyRandom : public TS_random
{
    friend class UtRandom;

private:

    FriendlyRandom();

    virtual ~FriendlyRandom();
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this math unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtRandom::UtRandom() :
    CppUnit::TestFixture(),
    ITERATION_COUNT( 10000 )
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this math unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtRandom::~UtRandom()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::setUp()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::tearDown()
{
    // nothing to do
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests randomDouble()
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::testRandomDouble1()
{
    std::cout << std::endl << "................................................................................";
    std::cout << std::endl << "TS_Random::randomDouble() ..................................................";

    int seed = 0;
    for ( int index = 0; index < ITERATION_COUNT; index++ )
    {
        double val = TS_random::randomDouble( seed );
        CPPUNIT_ASSERT( 0.0 <= val );
        CPPUNIT_ASSERT( 1.0 > val );
    }
    std::cout << "Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests randomDouble(double)
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::testRandomDouble2()
{
    std::cout << std::endl << "TS_Random::randomDouble(double) ............................................";

    int seed = 0;
    for ( int index = 0; index < ITERATION_COUNT; index++ )
    {
        double val = TS_random::randomDouble( seed, 100.1 );
        CPPUNIT_ASSERT( 0.0 <= val );
        CPPUNIT_ASSERT( 100.1 > val );

        val = TS_random::randomDouble( seed, -57.2 );
        CPPUNIT_ASSERT( -57.2 <= val );
        CPPUNIT_ASSERT( 0.0 > val );

        val = TS_random::randomDouble( seed, 1.45 );
        CPPUNIT_ASSERT( 0.0 <= val );
        CPPUNIT_ASSERT( 1.45 > val );
    }
    std::cout << "Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests randomDouble(double,double)
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::testRandomDouble3()
{
    std::cout << std::endl << "TS_Random::randomDouble(double,double) .....................................";

    int seed = 0;
    for ( int index = 0; index < ITERATION_COUNT; index++ )
    {
        double val = TS_random::randomDouble( seed, 1.0, 1.0 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, val, 0.0000001 );

        val = TS_random::randomDouble( seed, 57.2, -5.3 );
        CPPUNIT_ASSERT( -5.3 <= val );
        CPPUNIT_ASSERT( 57.2 > val );

        val = TS_random::randomDouble( seed, 5.3, 5.45 );
        CPPUNIT_ASSERT( 5.3 <= val );
        CPPUNIT_ASSERT( 5.45 > val );
    }
    std::cout << "Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests randomInt()
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::testRandomInt1()
{
    std::cout << std::endl << "TS_Random::randomInt() .....................................................";

    int seed = 0;
    for ( int index = 0; index < ITERATION_COUNT; index++ )
    {
        int val = TS_random::randomInt( seed );
        CPPUNIT_ASSERT( 0 <= val );
        CPPUNIT_ASSERT( RAND_MAX > val );
    }
    std::cout << "Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests randomInt(int,int)
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::testRandomInt2()
{
    std::cout << std::endl << "TS_Random::randomInt(int,int) ..............................................";

    int seed = 0;
    for ( int index = 0; index < ITERATION_COUNT; index++ )
    {
        int val = TS_random::randomInt( seed, 2, 2 );
        CPPUNIT_ASSERT( 2 == val );

        val = TS_random::randomInt( seed, 2, -10 );
        CPPUNIT_ASSERT( -10 <= val );
        CPPUNIT_ASSERT( 2 >= val );

        val = TS_random::randomInt( seed, 2, 10 );
        CPPUNIT_ASSERT( 2 <= val );
        CPPUNIT_ASSERT( 10 >= val );
    }
    std::cout << "Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests uniqueRandomDouble
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::testUniqueRandomDouble()
{
    std::cout << std::endl << "TS_Random::uniqueRandomDouble() ............................................";

    int seed = 0;

    std::vector<double> ignore(10);

    ignore[0] = 5.5;
    ignore[1] = 15.5;
    ignore[2] = 10.2;
    for ( int index = 0; index < ITERATION_COUNT; index++ )
    {
        double val = TS_random::uniqueRandomDouble( seed, 5.0, 16.0, ignore );
        CPPUNIT_ASSERT( 5.5 != val );
        CPPUNIT_ASSERT( 15.5 != val );
        CPPUNIT_ASSERT( 10.2 != val );
        CPPUNIT_ASSERT( 5.0 <= val );
        CPPUNIT_ASSERT( 16.0 > val );
    }
    std::cout << "Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests valueFound
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtRandom::testValueFound()
{
    std::cout << std::endl << "TS_Random::ValueFound() ....................................................";

    std::vector<double> ignore(10);

    ignore[0] = 5.5;
    ignore[1] = 15.5;
    ignore[2] = 10.2;

    CPPUNIT_ASSERT( FriendlyRandom::valueFound(  5.5, ignore ) );
    CPPUNIT_ASSERT( FriendlyRandom::valueFound( 15.5, ignore ) );
    CPPUNIT_ASSERT( FriendlyRandom::valueFound( 10.2, ignore ) );

    CPPUNIT_ASSERT( ! FriendlyRandom::valueFound(  5.500001, ignore ) );
    CPPUNIT_ASSERT( ! FriendlyRandom::valueFound( 15.499999, ignore ) );
    CPPUNIT_ASSERT( ! FriendlyRandom::valueFound( 10.200001, ignore ) );

    std::cout << "Pass";
}
