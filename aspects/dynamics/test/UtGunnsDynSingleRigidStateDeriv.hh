#ifndef UtGunnsDynSingleRigidStateDeriv_EXISTS
#define UtGunnsDynSingleRigidStateDeriv_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_DYNAMICS_SINGLE_RIGID_STATE_DERIV Single Rigid-Body EOM State Derivative Unit Tests
/// @ingroup  UT_TSM_GUNNS_DYNAMICS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Single Rigid-Body EOM State Derivative link model.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "aspects/dynamics/GunnsDynSingleRigidStateDeriv.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsDynSingleRigidStateDeriv and befriend UtGunnsDynSingleRigidStateDeriv.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsDynSingleRigidStateDeriv : public GunnsDynSingleRigidStateDeriv
{
    public:
        FriendlyGunnsDynSingleRigidStateDeriv(GunnsDynRotationalState&            state,
                                              const GunnsDynSingleRigidMassProps& massProps);
        virtual ~FriendlyGunnsDynSingleRigidStateDeriv();
        friend class UtGunnsDynSingleRigidStateDeriv;
};
inline FriendlyGunnsDynSingleRigidStateDeriv::FriendlyGunnsDynSingleRigidStateDeriv(
        GunnsDynRotationalState&            state,
        const GunnsDynSingleRigidMassProps& massProps)
    :
    GunnsDynSingleRigidStateDeriv(state, massProps) {;}
inline FriendlyGunnsDynSingleRigidStateDeriv::~FriendlyGunnsDynSingleRigidStateDeriv() {;}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Single Rigid-Body EOM State Derivative unit tests.
///
/// @details  This class provides the unit tests for the GUNNS Single Rigid-Body EOM State
///           Derivative link model within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsDynSingleRigidStateDeriv: public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this unit test.
        UtGunnsDynSingleRigidStateDeriv();
        /// @brief    Default destructs this unit test.
        virtual ~UtGunnsDynSingleRigidStateDeriv();
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
        void testUpdateCmOffset2();
        /// @brief    Tests exceptions from the update method.
        void testUpdateExceptions();
        //TODO
        void testCrossV();
        //TODO
        void testSkew();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsDynSingleRigidStateDeriv);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testApplyForcesTorques);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testUpdateCmOffset);
        CPPUNIT_TEST(testUpdateCmOffset2);
        CPPUNIT_TEST(testUpdateExceptions);
        CPPUNIT_TEST(testCrossV);
        CPPUNIT_TEST(testSkew);
        CPPUNIT_TEST_SUITE_END();
        GunnsDynRotationalState                tRotationalState;   /**< (--) Body rotational state object. */
        GunnsDynSingleRigidMassProps           tMassProps;         /**< (--) Body mass properties object. */
        GunnsDynAppliedForce                   tAppliedForces[2];  /**< (--) Body applied force objects. */
        GunnsDynAppliedTorque                  tAppliedTorques[2]; /**< (--) Body applied torque objects. */
        FriendlyGunnsDynSingleRigidStateDeriv* tArticle;           /**< (--) Pointer to the article under test. */
        std::string                            tName;              /**< (--) Test article name. */
        static int                             TEST_ID;            /**< (--) Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynSingleRigidStateDeriv(const UtGunnsDynSingleRigidStateDeriv&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtGunnsDynSingleRigidStateDeriv& operator =(const UtGunnsDynSingleRigidStateDeriv&);
};

///@}

#endif
