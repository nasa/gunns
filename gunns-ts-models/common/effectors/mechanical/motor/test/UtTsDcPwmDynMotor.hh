#ifndef UtTsDcPwmDynMotor_EXISTS
#define UtTsDcPwmDynMotor_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "common/effectors/mechanical/motor/TsDcPwmDynMotor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from TsDcPwmDynMotor and befriend UtTsDcPwmDynMotor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyTsDcPwmDynMotor : public TsDcPwmDynMotor
{
    public:
        FriendlyTsDcPwmDynMotor();
        virtual ~FriendlyTsDcPwmDynMotor();
        friend class UtTsDcPwmDynMotor;
};
inline FriendlyTsDcPwmDynMotor::FriendlyTsDcPwmDynMotor() : TsDcPwmDynMotor() {}
inline FriendlyTsDcPwmDynMotor::~FriendlyTsDcPwmDynMotor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Unit tests for TsDcPwmDynMotor
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtTsDcPwmDynMotor : public CppUnit::TestFixture {

public:
    UtTsDcPwmDynMotor(void);
    virtual ~UtTsDcPwmDynMotor(void);
    void setUp();
    void tearDown();
    void testConfigData();
    void testInputData();
    void testConstruction();
    void testInitialize();
    void testInitializeExceptions();
    void testAccessors();
    void testMotorTorque();
    void testExternalLoads();
    void testMotorSpeed();
    void testElectricalOutputs();
    void testWasteHeat();
    void testStep();

private:
    CPPUNIT_TEST_SUITE(UtTsDcPwmDynMotor);
    CPPUNIT_TEST(testConfigData);
    CPPUNIT_TEST(testInputData);
    CPPUNIT_TEST(testConstruction);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testInitializeExceptions);
    CPPUNIT_TEST(testAccessors);
    CPPUNIT_TEST(testMotorTorque);
    CPPUNIT_TEST(testExternalLoads);
    CPPUNIT_TEST(testMotorSpeed);
    CPPUNIT_TEST(testElectricalOutputs);
    CPPUNIT_TEST(testWasteHeat);
    CPPUNIT_TEST(testStep);
    CPPUNIT_TEST_SUITE_END();

    std::string                tName;               /**< (--)          Nominal article name */
    double                     tPulseWasteFraction; /**< (--)          Nominal config data */
    double                     tStallTorqueCoeff1;  /**< (N*m/v)       Nominal config data */
    double                     tStallTorqueCoeff2;  /**< (N*m/v)       Nominal config data */
    double                     tFrictionTorque;     /**< (N*m)         Nominal config data */
    double                     tBemfConstant;       /**< (v*min/rev)   Nominal config data */
    double                     tArmatureResistance; /**< (ohm)         Nominal config data */
    double                     tInertia;            /**< (kg*m2)       Nominal config data */
    double                     tSpeedLoadRatio;     /**< (rev/min/N/m) Nominal config data */
    TsDcPwmDynMotorConfigData* tNominalConfig;      /**< (--)          Nominal config data */
    double                     tVoltage;            /**< (v)           Nominal input data */
    double                     tPulseWidth;         /**< (--)          Nominal input data */
    double                     tMotorSpeed;         /**< (rev/min)     Nominal input data */
    double                     tLoadTorque1;        /**< (N*m)         Nominal input data */
    double                     tLoadTorque2;        /**< (N*m)         Nominal input data */
    double                     tLoadTorque3;        /**< (N*m)         Nominal input data */
    double                     tLoadTorque4;        /**< (N*m)         Nominal input data */
    TsDcPwmDynMotorInputData*  tNominalInput;       /**< (--)          Nominal input data */
    static const double        RPMTORADS;           /**< (--)          Units conversion constant */

    /// @brief Copy constructor is not implemented.
    UtTsDcPwmDynMotor(const UtTsDcPwmDynMotor &rhs);

    /// @brief Assignment operator is not implemented.
    UtTsDcPwmDynMotor& operator= (const UtTsDcPwmDynMotor &rhs);
};

#endif //UtTsDcPwmDynMotor_EXISTS
