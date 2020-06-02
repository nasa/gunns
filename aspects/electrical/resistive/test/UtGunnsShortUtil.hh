#ifndef UtGunnsShortUtil_EXISTS
#define UtGunnsShortUtil_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_ELECTRICAL_RESISTIVE_SHORT_UTIL    GUNNS Electrical Short Utility Unit Test
/// @ingroup  UT_GUNNS_ELECTRICAL_RESISTIVE
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Electrical Short Utility.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>
#include "aspects/electrical/resistive/GunnsShortUtil.hh"
#include "core/GunnsBasicNode.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsShortUtil and befriend UtGunnsShortUtil.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsShortUtil : public GunnsShortUtil
{
    public:
        FriendlyGunnsShortUtil();
        virtual ~FriendlyGunnsShortUtil();
        friend class UtGunnsShortUtil;
};
inline FriendlyGunnsShortUtil::FriendlyGunnsShortUtil() : GunnsShortUtil() {};
inline FriendlyGunnsShortUtil::~FriendlyGunnsShortUtil() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Short Utility unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Electrical Short Utility within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsShortUtil: public CppUnit::TestFixture
{
    public:
        UtGunnsShortUtil();
        virtual ~UtGunnsShortUtil();
        void tearDown();
        void setUp();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testComputeShort();
        void testAccessors();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsShortUtil(const UtGunnsShortUtil& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsShortUtil& operator =(const UtGunnsShortUtil& that);

        CPPUNIT_TEST_SUITE(UtGunnsShortUtil);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testComputeShort);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsShortUtil*  tArticle;                  /**< (--)  Test article. */
        double                   tMalfMinimumVoltage;       /**< (V)   Nominal input data. */
        bool                     tMalfConstantPowerFlag;    /**< (--)  Nominal input data. */
        double                   tMalfConstantPowerValue;   /**< (W)   Nominal input data. */
        bool                     tMalfConstantCurrentFlag;  /**< (--)  Nominal input data. */
        double                   tMalfConstantCurrentValue; /**< (amp) Nominal input data. */
        bool                     tMalfInitialPowerFlag;     /**< (--)  Nominal input data. */
        double                   tMalfInitialPowerValue;    /**< (W)   Nominal input data. */
        bool                     tMalfInitialCurrentFlag;   /**< (--)  Nominal input data. */
        double                   tMalfInitialCurrentValue;  /**< (amp) Nominal input data. */
        bool                     tMalfResistanceFlag;       /**< (--)  Nominal input data. */
        double                   tMalfResistanceValue;      /**< (ohm) Nominal input data. */
        GunnsShortUtilInputData* tInputData;                /**< (--)  Pointer to nominal input data. */
};

///@}

#endif
