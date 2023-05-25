#ifndef UtGunnsDistributed2WayBusBase_EXISTS
#define UtGunnsDistributed2WayBusBase_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_DISTRIBUTED_2WAYBUS_BASE    GUNNS Distributed 2-Way Bus Base Interface Unit Test
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2023 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Distributed 2-Way Bus Base Interface
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsDistributed2WayBusBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsDistributed2WayBusBase and befriend UtGunnsDistributed2WayBusBase.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsDistributed2WayBusBase : public GunnsDistributed2WayBusBase
{
    public:
        FriendlyGunnsDistributed2WayBusBase(GunnsDistributed2WayBusBaseInterfaceData* inDataPtr,
                                            GunnsDistributed2WayBusBaseInterfaceData* outDataPtr)
            : GunnsDistributed2WayBusBase(inDataPtr, outDataPtr) {;}
        virtual ~FriendlyGunnsDistributed2WayBusBase() {;}
        friend class UtGunnsDistributed2WayBusBase;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test interface data class
///
/// @details  Inherits the pure virtual GunnsDistributed2WayBusBaseInterfaceData and implements the
///           required pure virtual functiosn for testing.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsDistributed2WayBusBaseInterfaceData : public GunnsDistributed2WayBusBaseInterfaceData
{
    public:
        UtGunnsDistributed2WayBusBaseInterfaceData() {;}
        virtual ~UtGunnsDistributed2WayBusBaseInterfaceData() {;}
        virtual bool hasValidData() const {return true;}
        UtGunnsDistributed2WayBusBaseInterfaceData& operator =(const UtGunnsDistributed2WayBusBaseInterfaceData& that) {
            if (this != &that) {
                GunnsDistributed2WayBusBaseInterfaceData::operator = (that);
            }
            return *this;
        }
    private:
        UtGunnsDistributed2WayBusBaseInterfaceData(const UtGunnsDistributed2WayBusBaseInterfaceData&);

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Distributed 2-Way Bus Base Interface unit tests.
////
/// @details  This class provides the unit tests for the GunnsDistributed2WayBusBase class within
///           the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsDistributed2WayBusBase: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsDistributed2WayBusBase(const UtGunnsDistributed2WayBusBase& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsDistributed2WayBusBase& operator =(const UtGunnsDistributed2WayBusBase& that);

        CPPUNIT_TEST_SUITE(UtGunnsDistributed2WayBusBase);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testFrameCounts);
        CPPUNIT_TEST(testNotifications);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsDistributed2WayBusBase*       tArticle;      /**< (--) Article under test. */
        bool                                       tIsPairMaster; /**< (--) Nominal initialization data. */
        UtGunnsDistributed2WayBusBaseInterfaceData tInData;       /**< (--) Incoming interface data. */
        UtGunnsDistributed2WayBusBaseInterfaceData tOutData;      /**< (--) Incoming interface data. */

    public:
        UtGunnsDistributed2WayBusBase();
        virtual ~UtGunnsDistributed2WayBusBase();
        void tearDown();
        void setUp();
        void testDefaultConstruction();
        void testInitialization();
        void testFrameCounts();
        void testNotifications();
        void testAccessors();
};

///@}

#endif
