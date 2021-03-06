#ifndef UtTsOpenCloseValveController_EXISTS
#define UtTsOpenCloseValveController_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_TsOpenCloseValveController  Open/Close Valve Controller Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Open/Close Valve Controller model.
///
///@{
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "../TsOpenCloseValveController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsOpenCloseValveController and befriend UtTsOpenCloseValveController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsOpenCloseValveController : public TsOpenCloseValveController
{
    public:
        FriendlyTsOpenCloseValveController();
        virtual ~FriendlyTsOpenCloseValveController();
        friend class UtTsOpenCloseValveController;
};
inline FriendlyTsOpenCloseValveController::FriendlyTsOpenCloseValveController() : TsOpenCloseValveController() {};
inline FriendlyTsOpenCloseValveController::~FriendlyTsOpenCloseValveController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Valve Controller unit tests.
///
/// @details  This class provides the unit tests for the Open/Close Valve Controller model within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsOpenCloseValveController : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Open/Close Valve Controller unit test.
        UtTsOpenCloseValveController();
        /// @brief    Default destructs this Open/Close Valve Controller unit test.
        virtual ~UtTsOpenCloseValveController();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
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
        CPPUNIT_TEST_SUITE(UtTsOpenCloseValveController);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testModifiers);
        CPPUNIT_TEST(testUpdateNominal);
        CPPUNIT_TEST(testUpdateOffNominal);
        CPPUNIT_TEST(testUpdateMalfunctions);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST_SUITE_END();
        double                              mMinCmdPosition;          /**< (--)     Minimum valid valve position. */
        double                              mMaxCmdPosition;          /**< (--)     Maximum valid valve  position. */
        double                              mMinFluidPosition;        /**< (--)     Minimum valid valve flow area fraction. */
        double                              mMaxFluidPosition;        /**< (--)     Maximum valid valve  flow area fraction. */
        double                              mFluidBias;               /**< (--)     Bias for linear conversion of valve position to valve flow area fraction. */
        double                              mFluidScale;              /**< (--)     Scale factor for linear conversion of valve position to valve flow area fraction. */
        double                              mTransitTime;             /**< (s)      Reference transit time. */
        double                              mRefCmd;                  /**< (--)     Reference speed command. */
        double                              mCmdScale;                /**< (one/s)  Speed command normalization scale factor. */
        double                              mHoldPower;               /**< (W)      Power consumption when valve is holding. */
        double                              mMovePower;               /**< (W)      Power consumption when valve is moving. */
        double                              mStuckPower;              /**< (W)      Power consumption when valve is stuck. */
        double                              mRefVoltage;              /**< (v)      Reference voltage for power consumption. */
        double                              mHoldResistance;          /**< (ohm)    Resistance when valve is holding. */
        double                              mMoveResistance;          /**< (ohm)    Resistance when valve is moving. */
        double                              mStuckResistance;         /**< (ohm)    Resistance when valve is stuck. */
        TsPoweredValveController::LatchType mLatch;                   /**< (--)     Latching type. */
        bool                                mEotInterrupt;            /**< (--)     End of travel interrupting flag. */
        TsPoweredValveControllerConfigData* mConfig;                  /**< (--)     Pointer to nominal configuration data. */
        double                              mCmdPosition;             /**< (--)     Valve position. */
        bool                                mMalfValveStuckFlag;      /**< (--)     Valve stuck malfunction flag. */
        bool                                mMalfValveFailToFlag;     /**< (--)     Valve fail-to position malfunction flag. */
        double                              mMalfValveFailToValue;    /**< (--)     Valve fail-to malfunction value. */
        bool                                mManualPositionFlag;      /**< (--)     Manual override position flag. */
        double                              mManualPositionValue;     /**< (--)     Manual override position value. */
        bool                                mMalfManualFlag;          /**< (--)     Manual override malfunction flag. */
        bool                                mMalfPowerGoodFlag;       /**< (--)     Ignore voltage failure flag. */
        bool                                mMalfDriveStuckFlag;      /**< (--)     Stuck motor/solenoid malfunction flag. */
        bool                                mMalfDriveFailToFlag;     /**< (--)     Motor/solenoid fail-to position malfunction flag. */
        double                              mMalfDriveFailToValue;    /**< (--)     Motor/solenoid fail-to position malfunction value. */
        bool                                mSupplyVoltageFlag;       /**< (--)     Supply voltage good flag. */
        bool                                mEnabledFlag;             /**< (--)     Controller enabled flag. */
        double                              mCmd;                     /**< (--)     Speed command. */
        TsOpenCloseValveCmd                 mCommand;                 /**< (--)     Composite command.  */
        TsOpenCloseValveSensed              mSensed;                  /**< (--)     Composite sensed position.  */
        TsPoweredValveControllerInputData*  mInput;                   /**< (--)     Pointer to nominal input data. */
        std::string                         mName;                    /**< (--)     Object name. */
        double                              mFluidPosition;           /**< (--)     Valve flow area fraction (simbus output to fluid aspect). */
        double                              mPower;                   /**< (W)      Power consumption. */
        FriendlyTsOpenCloseValveController* mArticle;                 /**< (--)     Pointer to article under test. */
        double                              mTimeStep;                /**< (s)      Nominal time step. */
        double                              mTolerance;               /**< (--)     Nominal tolerance for comparison of expected and returned values. */
        static int                          TEST_ID;                  /**< (--)     Test identification number. */
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsOpenCloseValveController(const UtTsOpenCloseValveController&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        UtTsOpenCloseValveController& operator =(const UtTsOpenCloseValveController&);
};

/// @}

#endif
