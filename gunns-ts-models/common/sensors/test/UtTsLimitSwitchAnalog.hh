#ifndef UtTsLimitSwitchAnalog_EXISTS
#define UtTsLimitSwitchAnalog_EXISTS

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


/// @brief unit tests for TsLimitSwitchAnalog
class UtTsLimitSwitchAnalog : public CppUnit::TestFixture {

public:

    UtTsLimitSwitchAnalog(void);
    virtual ~UtTsLimitSwitchAnalog( void );
    void setUp();
    void tearDown();
    void testUpdate();

protected:

private:

    CPPUNIT_TEST_SUITE(UtTsLimitSwitchAnalog);
    CPPUNIT_TEST(testUpdate);
    CPPUNIT_TEST_SUITE_END();

    /// @brief keep private, never used
    UtTsLimitSwitchAnalog(const UtTsLimitSwitchAnalog &rhs);

    /// @brief keep private, never used
    UtTsLimitSwitchAnalog& operator= (const UtTsLimitSwitchAnalog &rhs);

};


/// @}


#endif //UtTsLimitSwitchAnalog_EXISTS
