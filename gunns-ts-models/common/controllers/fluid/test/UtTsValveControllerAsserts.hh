#ifndef UtTsValveControllerAsserts_EXISTS
#define UtTsValveControllerAsserts_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_Asserts  Assert Macros
/// @ingroup   UT_TSM_CONTROLLERS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Assertions for the Valve Controller unit tests.
///
///@{
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../TsOpenCloseValveCmd.hh"
#include "../TsOpenCloseValveSensed.hh"
#include "../TsSpeedValveCmd.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that two TsOpenCloseValveSensed are equal by asserting that each of their data
///           members are equal.
///
/// @param[in]   expected   (--)  The expected value.
/// @param[in]   returned   (--)  The returned value.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_OPEN_CLOSE_VALVE_SENSED_EQUAL(expected, returned) \
{ \
    CPPUNIT_ASSERT((expected).mOpen    == (returned).mOpen); \
    CPPUNIT_ASSERT((expected).mClose   == (returned).mClose); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that two TsOpenCloseValveCmds are equal by asserting that each of their data
///           members are equal.
///
/// @param[in]   expected   (--)  The expected value.
/// @param[in]   returned   (--)  The returned value.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_OPEN_CLOSE_VALVE_CMD_EQUAL(expected, returned) \
{ \
    CPPUNIT_ASSERT((expected).mEnable == (returned).mEnable); \
    CPPUNIT_ASSERT((expected).mOpen    == (returned).mOpen); \
    CPPUNIT_ASSERT((expected).mClose   == (returned).mClose); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that two TsSpeedValveCmds are equal by asserting that each of their data
///           members are equal.
///
/// @param[in]   expected   (--)  The expected value.
/// @param[in]   returned   (--)  The returned value.
/// @param[in]   tolerance  (--)  The tolerance for the speed command comparison.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_SPEED_VALVE_CMD_EQUAL(expected, returned, tolerance) \
{ \
    CPPUNIT_ASSERT((expected).mEnable == (returned).mEnable); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL((expected).mSpeed, (returned).mSpeed, tolerance); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that two TsToggleValveCmds are equal by asserting that each of their data
///           members are equal.
///
/// @param[in]   expected   (--)  The expected value.
/// @param[in]   returned   (--)  The returned value.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_TOGGLE_VALVE_CMD_EQUAL(expected, returned) \
{ \
    CPPUNIT_ASSERT((expected).mEnable == (returned).mEnable); \
    CPPUNIT_ASSERT((expected).mToggle == (returned).mToggle); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that two TsPositionValveCmds are equal by asserting that each of their data
///           members are equal.
///
/// @param[in]   expected   (--)  The expected value.
/// @param[in]   returned   (--)  The returned value.
/// @param[in]   tolerance  (--)  The tolerance for the position command comparison.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_POSITION_VALVE_CMD_EQUAL(expected, returned, tolerance) \
{ \
    CPPUNIT_ASSERT((expected).mEnable == (returned).mEnable); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL((expected).mPosition, (returned).mPosition, tolerance); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that a set of attributes are equal to those in a TsOpenCloseValveSensed.
///
/// @param[in]   open       (--)  The expected sensed open flag.
/// @param[in]   close      (--)  The expected sensed closed flag.
/// @param[in]   returned   (--)  The returned value.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_OPEN_CLOSE_VALVE_SENSED_VALUES_EQUAL(open, close, returned) \
{ \
    CPPUNIT_ASSERT(open    == (returned).mOpen); \
    CPPUNIT_ASSERT(close   == (returned).mClose); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that a set of attributes are equal to those in a TsOpenCloseValveCmd.
///
/// @param[in]   enable     (--)  The expected enable command flag.
/// @param[in]   open       (--)  The expected open command flag.
/// @param[in]   close      (--)  The expected close command flag.
/// @param[in]   returned   (--)  The returned value.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_OPEN_CLOSE_VALVE_CMD_VALUES_EQUAL(enable, open, close, returned) \
{ \
    CPPUNIT_ASSERT(enable == (returned).mEnable); \
    CPPUNIT_ASSERT(open   == (returned).mOpen); \
    CPPUNIT_ASSERT(close  == (returned).mClose); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that a set of attributes are equal to those in a TsSpeedValveCmd.
///
/// @param[in]   enable     (--)  The expected enable command flag.
/// @param[in]   speed      (--)  The expected speed command value.
/// @param[in]   returned   (--)  The returned value.
/// @param[in]   tolerance  (--)  The tolerance for the speed command comparison.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_SPEED_VALVE_CMD_VALUES_EQUAL(enable, speed, returned, tolerance) \
{ \
    CPPUNIT_ASSERT(enable == (returned).mEnable); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL(speed, (returned).mSpeed, tolerance); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that a set of attributes are equal to those in a TsToggleValveCmd.
///
/// @param[in]   enable     (--)  The expected enable command flag.
/// @param[in]   speed      (--)  The expected speed command value.
/// @param[in]   returned   (--)  The returned value.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_TOGGLE_VALVE_CMD_VALUES_EQUAL(enable, toggle, returned) \
{ \
    CPPUNIT_ASSERT(enable == (returned).mEnable); \
    CPPUNIT_ASSERT(toggle == (returned).mToggle); \
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Asserts that a set of attributes are equal to those in a TsPositionValveCmd.
///
/// @param[in]   enable     (--)  The expected enable command flag.
/// @param[in]   position   (--)  The expected position command value.
/// @param[in]   returned   (--)  The returned value.
/// @param[in]   tolerance  (--)  The tolerance for the speed command comparison.
//
/// @hideinitializer
////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_POSITION_VALVE_CMD_VALUES_EQUAL(enable, position, returned, tolerance) \
{ \
    CPPUNIT_ASSERT(enable == (returned).mEnable); \
    CPPUNIT_ASSERT_DOUBLES_EQUAL(position, (returned).mPosition, tolerance); \
}

/// @}

#endif
