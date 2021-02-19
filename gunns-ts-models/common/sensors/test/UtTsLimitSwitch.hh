#ifndef UtTsLimitSwitch_EXISTS
#define UtTsLimitSwitch_EXISTS

/**
@defgroup  <classname>  some text goes here
@ingroup   <ingroup>

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 - ()

 REFERENCE:
 - ()

 ASSUMPTIONS AND LIMITATIONS:
 - ()

 LIBRARY DEPENDENCY:
 - ()

 PROGRAMMERS:
 - ((Chuck Sorensen) (LZT) (Dec 2, 2011) (TS21) (initial))
@{
*/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>


/// @brief unit test for TsLimitSwitch
class UtTsLimitSwitch : public CppUnit::TestFixture {

public:

    UtTsLimitSwitch(void);
    virtual ~UtTsLimitSwitch( void );
    void setUp();
    void tearDown();
    void testInit();
    void testUpdate();

protected:

private:
    CPPUNIT_TEST_SUITE(UtTsLimitSwitch);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testUpdate);
    CPPUNIT_TEST_SUITE_END();

    /// @brief keep private, never used
    UtTsLimitSwitch(const UtTsLimitSwitch &rhs);

    /// @brief keep private, never used
    UtTsLimitSwitch& operator= (const UtTsLimitSwitch &rhs);

};


/// @}


#endif //UtTsLimitSwitch_EXISTS
