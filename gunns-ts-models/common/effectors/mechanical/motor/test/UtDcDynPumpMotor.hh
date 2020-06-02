#ifndef UtDcDynPumpMotor_EXISTS
#define UtDcDynPumpMotor_EXISTS

/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include "common/effectors/mechanical/motor/DcDynPumpMotor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from DcDynPumpMotor and befriend UtDcDynPumpMotor.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyDcDynPumpMotor : public DcDynPumpMotor
{
    public:
        FriendlyDcDynPumpMotor();
        virtual ~FriendlyDcDynPumpMotor();
        friend class UtDcDynPumpMotor;
};
inline FriendlyDcDynPumpMotor::FriendlyDcDynPumpMotor() : DcDynPumpMotor() {}
inline FriendlyDcDynPumpMotor::~FriendlyDcDynPumpMotor() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Unit tests for DcDynPumpMotor
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtDcDynPumpMotor : public CppUnit::TestFixture {

public:
    UtDcDynPumpMotor(void);
    virtual ~UtDcDynPumpMotor(void);
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
    CPPUNIT_TEST_SUITE(UtDcDynPumpMotor);
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

    std::string               tName;              /**< (--)          Nominal article name */
    double                    tWindingResistance; /**< (ohm)         Nominal config data  */
    double                    tTorqueConstant;    /**< (N*m/amp)     Nominal config data  */
    double                    tFrictionConstant;  /**< (N*m*min/rev) Nominal config data  */
    double                    tFrictionMinSpeed;  /**< (rev/min)     Nominal config data  */
    double                    tInertia;           /**< (kg*m2)       Nominal config data  */
    DcDynPumpMotorConfigData* tNominalConfig;     /**< (--)          Nominal config data  */
    double                    tVoltage;           /**< (v)           Nominal input data   */
    double                    tCurrentLimit;      /**< (amp)         Nominal input data   */
    double                    tMotorSpeed;        /**< (rev/min)     Nominal input data   */
    double                    tLoadTorque1;       /**< (N*m)         Nominal input data   */
    double                    tLoadTorque2;       /**< (N*m)         Nominal input data   */
    double                    tLoadTorque3;       /**< (N*m)         Nominal input data   */
    double                    tLoadTorque4;       /**< (N*m)         Nominal input data   */
    DcDynPumpMotorInputData*  tNominalInput;      /**< (--)          Nominal input data   */
    static const double       RPMTORADS;          /**< (--)          Units conversion constant */

    /// @brief Copy constructor is not implemented.
    UtDcDynPumpMotor(const UtDcDynPumpMotor &rhs);

    /// @brief Assignment operator is not implemented.
    UtDcDynPumpMotor& operator= (const UtDcDynPumpMotor &rhs);
};

#endif //UtDcDynPumpMotor_EXISTS
