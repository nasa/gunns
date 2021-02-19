#ifndef UtTsValveController_EXISTS
#define UtTsValveController_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_TsValveController  Valve Controller Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Valve Controller model.
///
///@{
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsValveController and befriend UtTsValveController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsValveController : public TsValveController
{
    public:
        FriendlyTsValveController();
        virtual ~FriendlyTsValveController();
        friend class UtTsValveController;
};
inline FriendlyTsValveController::FriendlyTsValveController() : TsValveController() {};
inline FriendlyTsValveController::~FriendlyTsValveController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve Controller unit tests.
///
/// @details  This class provides the unit tests for the Valve Controller model within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsValveController : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Valve Controller unit test.
        UtTsValveController();
        /// @brief    Default destructs this Valve Controller unit test.
        virtual ~UtTsValveController();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests config and input data.
        void testConfigAndInput();
        /// @brief    Tests default construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method.
        void testNominalInitialization();
        /// @brief    Tests accessor methods.
        void testAccessors();
        /// @brief    Tests modifier methods.
        void testModifiers();
        /// @brief    Tests update method (nominal).
        void testUpdateNominal();
        /// @brief    Tests update method (nominal).
        void testUpdateOffNominal();
        /// @brief    Tests update method (malfunctions).
        void testUpdateMalfunctions();
        /// @brief    Tests initialize method exceptions.
        void testInitializationExceptions();
    private:
        CPPUNIT_TEST_SUITE(UtTsValveController);
        CPPUNIT_TEST(testConfigAndInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateNominal);
        CPPUNIT_TEST(testUpdateOffNominal);
        CPPUNIT_TEST(testUpdateMalfunctions);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        double                        mMinCmdPosition;          /**< (--)  Minimum valid valve position. */
        double                        mMaxCmdPosition;          /**< (--)  Maximum valid valve  position. */
        double                        mMinFluidPosition;        /**< (--)  Minimum valid valve flow area fraction. */
        double                        mMaxFluidPosition;        /**< (--)  Maximum valid valve  flow area fraction. */
        double                        mFluidBias;               /**< (--)  Bias for linear conversion of valve position to valve flow area fraction. */
        double                        mFluidScale;              /**< (--)  Scale factor for linear conversion of valve position to valve flow area fraction. */
        TsValveControllerConfigData*  mConfig;                  /**< (--)  Pointer to nominal configuration data. */
        double                        mCmdPosition;             /**< (--)  Valve position. */
        bool                          mMalfValveStuckFlag;      /**< (--)  Valve stuck malfunction flag. */
        bool                          mMalfValveFailToFlag;     /**< (--)  Valve fail-to position malfunction flag. */
        double                        mMalfValveFailToValue;    /**< (--)  Valve fail-to position malfunction value. */
        bool                          mManualPositionFlag;      /**< (--)  Manual override position flag. */
        double                        mManualPositionValue;     /**< (--)  Manual override position value. */
        bool                          mMalfManualFlag;          /**< (--)  Manual override malfunction flag. */
        TsValveControllerInputData*   mInput;                   /**< (--)  Pointer to nominal input data. */
        std::string                   mName;                    /**< (--)  Object name. */
        double                        mFluidPosition;           /**< (--)  Valve flow area fraction (simbus output to fluid aspect). */
        FriendlyTsValveController*    mArticle;                 /**< (--)  Pointer to article under test. */
        double                        mTimeStep;                /**< (s)   Nominal time step. */
        double                        mTolerance;               /**< (--)  Nominal tolerance for comparison of expected and returned values. */
        static int                    TEST_ID;                  /**< (--)  Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsValveController(const UtTsValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsValveController& operator =(const UtTsValveController&);
};

/// @}

#endif
