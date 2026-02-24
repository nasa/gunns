#ifndef UtDistributed2WayBusBase_EXISTS
#define UtDistributed2WayBusBase_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_UTILS_INTEROP_DISTR_2WAYBUS_BASE    Distributed 2-Way Bus Base Interface Unit Test
/// @ingroup  UT_UTILS_INTEROP
///
/// @copyright Copyright 2025 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Distributed 2-Way Bus Base Interface
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "interop/Distributed2WayBusBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from Distributed2WayBusBase and befriend UtDistributed2WayBusBase.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDistributed2WayBusBase : public Distributed2WayBusBase
{
    public:
        FriendlyDistributed2WayBusBase(Distributed2WayBusBaseInterfaceData* inDataPtr,
                                       Distributed2WayBusBaseInterfaceData* outDataPtr)
            : Distributed2WayBusBase(inDataPtr, outDataPtr) {;}
        virtual ~FriendlyDistributed2WayBusBase() {;}
        friend class UtDistributed2WayBusBase;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test interface data class
///
/// @details  Inherits the pure virtual Distributed2WayBusBaseInterfaceData and implements the
///           required pure virtual functions for testing.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtDistributed2WayBusBaseInterfaceData : public Distributed2WayBusBaseInterfaceData
{
    public:
        UtDistributed2WayBusBaseInterfaceData() {;}
        virtual ~UtDistributed2WayBusBaseInterfaceData() {;}
        virtual bool hasValidData() const {return true;}
        UtDistributed2WayBusBaseInterfaceData& operator =(const UtDistributed2WayBusBaseInterfaceData& that) {
            if (this != &that) {
                Distributed2WayBusBaseInterfaceData::operator = (that);
            }
            return *this;
        }
    private:
        UtDistributed2WayBusBaseInterfaceData(const UtDistributed2WayBusBaseInterfaceData&);

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed 2-Way Bus Base Interface unit tests.
////
/// @details  This class provides the unit tests for the Distributed2WayBusBase class within
///           the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtDistributed2WayBusBase: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtDistributed2WayBusBase(const UtDistributed2WayBusBase& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtDistributed2WayBusBase& operator =(const UtDistributed2WayBusBase& that);

        CPPUNIT_TEST_SUITE(UtDistributed2WayBusBase);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testFrameCounts);
        CPPUNIT_TEST(testNotifications);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();

        FriendlyDistributed2WayBusBase*       tArticle;      /**< (--) Article under test. */
        bool                                  tIsPairMaster; /**< (--) Nominal initialization data. */
        UtDistributed2WayBusBaseInterfaceData tInData;       /**< (--) Incoming interface data. */
        UtDistributed2WayBusBaseInterfaceData tOutData;      /**< (--) Incoming interface data. */
        static int                            TEST_ID;       /**< (--) Test identification number. */

    public:
        UtDistributed2WayBusBase();
        virtual ~UtDistributed2WayBusBase();
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
