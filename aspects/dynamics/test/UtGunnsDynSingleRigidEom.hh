#ifndef UtGunnsDynSingleRigidEom_EXISTS
#define UtGunnsDynSingleRigidEom_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_DYNAMICS_SINGLE_RIGID_STATE_DERIV Single Rigid-Body EOM Unit Tests
/// @ingroup  UT_TSM_GUNNS_DYNAMICS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Single Rigid-Body EOM model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/dynamics/GunnsDynSingleRigidEom.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Single Rigid-Body EOM unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Single Rigid-Body EOM model within
///           the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsDynSingleRigidEom: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this unit test.
        UtGunnsDynSingleRigidEom();
        /// @brief    Default destructs this unit test.
        virtual ~UtGunnsDynSingleRigidEom();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests addAppliedForce and addAppliedTorque methods.
        void testApplyForcesTorques();
        /// @brief    Tests nominal update method.
        void testUpdate();
        /// @brief    Tests nominal update method with a CM offset.
        void testUpdateCmOffset();
        /// @brief    Tests exceptions from the update method.
        void testUpdateExceptions();
        //TODO
        void testCrossV();
        //TODO
        void testSkew();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsDynSingleRigidEom);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testApplyForcesTorques);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testUpdateCmOffset);
        CPPUNIT_TEST(testUpdateExceptions);
        CPPUNIT_TEST(testCrossV);
        CPPUNIT_TEST(testSkew);
        CPPUNIT_TEST_SUITE_END();
        GunnsDynAppliedForce             tAppliedForce; /**< (--) Body applied force object. */
        GunnsDynSingleRigidEomInputData* tInputData;    /**< (--) Pointer to nominal input data. */
        GunnsDynSingleRigidEom*          tArticle;      /**< (--) Pointer to the article under test. */
        std::string                      tName;         /**< (--) Test article name. */
        static int                       TEST_ID;       /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynSingleRigidEom(const UtGunnsDynSingleRigidEom&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynSingleRigidEom& operator =(const UtGunnsDynSingleRigidEom&);
};

///@}

#endif
