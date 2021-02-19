#ifndef UtGunnsDynSingleRigidMassProps_EXISTS
#define UtGunnsDynSingleRigidMassProps_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_DYNAMICS_SINGLE_RIGID_MASS_PROPS Single Rigid-Body Mass Properties Unit Tests
/// @ingroup  UT_TSM_GUNNS_DYNAMICS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Single Rigid-Body Mass Properties class.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/dynamics/GunnsDynSingleRigidMassProps.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Single Rigid-Body Mass Properties unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Single Rigid-Body Mass Properties
///           class within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsDynSingleRigidMassProps: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this unit test.
        UtGunnsDynSingleRigidMassProps();
        /// @brief    Default destructs this unit test.
        virtual ~UtGunnsDynSingleRigidMassProps();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests copy construction.
        void testCopyConstruction();
        /// @brief    Tests the assignment operator.
        void testAssignmentOperator();
        /// @brief    Tests the equals operator.
        void testEqualsOperator();
        /// @brief    Tests validate method.
        void testValidate();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsDynSingleRigidMassProps);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testCopyConstruction);
        CPPUNIT_TEST(testAssignmentOperator);
        CPPUNIT_TEST(testEqualsOperator);
        CPPUNIT_TEST(testValidate);
        CPPUNIT_TEST_SUITE_END();
        double                        tMass;    /**< (kg)    Body mass. */
        double                        tCM[3];   /**< (m)     Center of mass location in the body reference frame. */
        double                        tIxx;     /**< (kg*m2) Body X moment of inertia in the body reference frame. */
        double                        tIxy;     /**< (kg*m2) Body XY product of inertia in the body reference frame. */
        double                        tIxz;     /**< (kg*m2) Body XZ product of inertia in the body reference frame. */
        double                        tIyy;     /**< (kg*m2) Body Y moment of inertia in the body reference frame. */
        double                        tIyz;     /**< (kg*m2) Body YZ product of inertia in the body reference frame. */
        double                        tIzz;     /**< (kg*m2) Body Z moment of inertia in the body reference frame. */
        GunnsDynSingleRigidMassProps* tArticle; /**< (--)    Pointer to the article under test. */
        static int                    TEST_ID;  /**< (--)    Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynSingleRigidMassProps(const UtGunnsDynSingleRigidMassProps&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynSingleRigidMassProps& operator =(const UtGunnsDynSingleRigidMassProps&);
};

///@}

#endif
