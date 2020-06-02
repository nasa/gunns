#ifndef UtTsPumpMotorController_EXISTS
#define UtTsPumpMotorController_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup  UT_TSM_CONTROLLERS_TsPumpMotorController    Pump Motor Controller Unit Tests
/// @ingroup   UT_TSM_CONTROLLERS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details   Unit Tests for the Pump Motor Controller model.
///
///@{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include "common/controllers/fluid/TsPumpMotorController.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsPumpMotorController and befriend UtTsPumpMotorController.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsPumpMotorController : public TsPumpMotorController
{
    public:
        FriendlyTsPumpMotorController();
        virtual ~FriendlyTsPumpMotorController();
        friend class UtTsPumpMotorController;
};
inline FriendlyTsPumpMotorController::FriendlyTsPumpMotorController() : TsPumpMotorController() {}
inline FriendlyTsPumpMotorController::~FriendlyTsPumpMotorController() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsDcPwmDynMotor and befriend UtTsPumpMotorController.
///
/// @details  Class derived from the motor inside the unit under test. It just has a constructor
///           with the same arguments as the parent and a default destructor, but it befriends the
///           unit test case driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDcDynPumpMotor : public DcDynPumpMotor
{
    public:
        FriendlyDcDynPumpMotor();
        virtual ~FriendlyDcDynPumpMotor();
        friend class UtTsPumpMotorController;
};
inline FriendlyDcDynPumpMotor::FriendlyDcDynPumpMotor() : DcDynPumpMotor() {}
inline FriendlyDcDynPumpMotor::~FriendlyDcDynPumpMotor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Unit tests for TsPumpMotorController
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsPumpMotorController : public CppUnit::TestFixture {

public:
    UtTsPumpMotorController(void);
    virtual ~UtTsPumpMotorController(void);
    void setUp();
    void tearDown();
    void testConfigData();
    void testInputData();
    void testConstruction();
    void testInitialize();
    void testInitializeExceptions();
    void testAccessors();
    void testTemperatureTrip();
    void testCurrentTrip();
    void testPower();
    void testControlFilterNoise();
    void testControlFilterPulseWidth();
    void testUpdateMotor();
    void testPowerLoad();
    void testStep();
    void testStepWithArgs();

private:
    CPPUNIT_TEST_SUITE(UtTsPumpMotorController);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testConstruction);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testInitializeExceptions);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testTemperatureTrip);
    CPPUNIT_TEST(testCurrentTrip);
    CPPUNIT_TEST(testPower);
    CPPUNIT_TEST(testControlFilterNoise);
    CPPUNIT_TEST(testControlFilterPulseWidth);
    CPPUNIT_TEST(testUpdateMotor);
    CPPUNIT_TEST(testPowerLoad);
    CPPUNIT_TEST(testStep);
    CPPUNIT_TEST(testStepWithArgs);
    CPPUNIT_TEST_SUITE_END();

    std::string                      tName;                 /**< (--)      Nominal article name */
    DcDynPumpMotorConfigData         tMotorConfig;          /**< (--)      Nominal config data */
    double                           tForcingGain;          /**< (--)      Nominal config data */
    double                           tDampingGain;          /**< (--)      Nominal config data */
    double                           tDampingCutoff;        /**< (--)      Nominal config data */
    double                           tTemperatureTripLimit; /**< (K)       Nominal config data */
    double                           tTemperatureTripReset; /**< (K)       Nominal config data */
    double                           tMinVoltage;           /**< (v)       Nominal config data */
    double                           tMaxVoltage;           /**< (v)       Nominal config data */
    double                           tControllerPowerLoad;  /**< (W)       Nominal config data */
    double                           tNoiseAmplitude;       /**< (rev/min) Nominal config data */
    double                           tNoiseFrequency;       /**< (s)       Nominal config data */
    double                           tStartupCurrentLimit;  /**< (amp)     Nominal config data */
    TsPumpMotorControllerConfigData* tNominalConfig;        /**< (--)      Nominal config data */
    double                           tLoadTorques[4];       /**< (--)      Nominal input data  */
    DcDynPumpMotorInputData          tMotorInput;           /**< (--)      Nominal input data  */
    double                           tVoltage;              /**< (v)       Nominal input data  */
    double                           tSensedSpeed;          /**< (rev/min) Nominal input data  */
    double                           tSensedTemperature;    /**< (K)       Nominal input data  */
    bool                             tStartupState;         /**< (--)      Nominal input data  */
    bool                             tCommandEnable;        /**< (--)      Nominal input data  */
    double                           tCommandSpeed;         /**< (rev/min) Nominal input data  */
    double                           tNoisePhase;           /**< (r)       Nominal input data  */
    TsPumpMotorControllerInputData*  tNominalInput;         /**< (--)      Nominal input data  */
    static int                       TEST_ID;               /**< (--)      Test identification number */
    static const double              TWOPI;                 /**< (--)      Units conversion constant */

    /// @brief Copy constructor is not implemented.
    UtTsPumpMotorController(const UtTsPumpMotorController &rhs);

    /// @brief Assignment operator is not implemented.
    UtTsPumpMotorController& operator= (const UtTsPumpMotorController &rhs);
};

/// @}

#endif //UtTsPumpMotorController_EXISTS
