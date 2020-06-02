#ifndef UtGunnsThermoelectricEffect_EXISTS
#define UtGunnsThermoelectricEffect_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup    UT_GUNNS_THERMAL_RADIATION  GUNNS Thermoelectric Effect Unit test
/// @ingroup     UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details     Unit Tests for GUNNS Thermoelectric Effect
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "aspects/thermal/GunnsThermoelectricEffect.hh"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsThermoelectricEffect and befriend UtGunnsThermoelectricEffect.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsThermoelectricEffect : public GunnsThermoelectricEffect
{
    public:
        FriendlyGunnsThermoelectricEffect();
        virtual ~FriendlyGunnsThermoelectricEffect();
        friend class UtGunnsThermoelectricEffect;
        friend class UtGunnsThermoelectricDevice;
};
inline FriendlyGunnsThermoelectricEffect::FriendlyGunnsThermoelectricEffect() : GunnsThermoelectricEffect() {};
inline FriendlyGunnsThermoelectricEffect::~FriendlyGunnsThermoelectricEffect() {};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Thermoelectric Effect unit tests.
///
/// @details  This class provides unit tests for the Thermoelectric Effect within the CPPUnit
///           framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsThermoelectricEffect: public CppUnit::TestFixture
{
    public:
        /// @brief    Default Constructor.
        UtGunnsThermoelectricEffect();
        /// @brief    Default Destructor.
        virtual ~UtGunnsThermoelectricEffect();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        /// @brief    Tests ConfigData construction.
        void testConfig();
        /// @brief    Tests InputData  construction.
        void testInput();
        /// @brief    Tests Default Construction.
        void testDefaultConstruction();
        /// @brief    Tests initialize method without exceptions.
        void testInitialization();
        /// @brief    Tests initialize method with exceptions.
        void testInitializationExceptions();
        /// @brief    Tests restart method.
        void testRestart();
        /// @brief    Tests update method.
        void testUpdate();
        /// @brief    Tests setter & getter methods.
        void testAccessors();

    private:
        CPPUNIT_TEST_SUITE(UtGunnsThermoelectricEffect);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testUpdate);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST_SUITE_END();
        GunnsThermoelectricEffectConfigData* tConfigData;                      /**< (--)    Pointer to nominal configuration data. */
        GunnsThermoelectricEffectInputData*  tInputData;                       /**< (--)    Pointer to nominal input data. */
        FriendlyGunnsThermoelectricEffect*   tArticle;                         /**< (--)    Pointer to the test article. */
        std::string                          tName;                            /**< (--)    Test article name. */
        double                               tNumThermocouples;                /**< (--)    Nominal config data. */
        double                               tGeometryFactor;                  /**< (m)     Nominal config data. */
        double                               tElectricalResistivityCoeff[2];   /**< (ohm*m) Nominal config data. */
        double                               tSeebeckCoeff[3];                 /**< (v/K)   Nominal config data. */
        double                               tThermalConductivityCoeff[3];     /**< (W/m/K) Nominal config data. */
        double                               tEndPlateThermalConductance;      /**< (W/K)   Nominal config data. */
        double                               tMinTemperature;                  /**< (K)     Nominal config data. */
        double                               tMaxTemperature;                  /**< (K)     Nominal config data. */
        bool                                 tMalfThermoelectricEffectsFlag;   /**< (--)    Nominal input data. */
        double                               tMalfThermoelectricEffectsScalar; /**< (--)    Nominal input data. */
        double                               tCurrent;                         /**< (amp)   Nominal input data. */
        double                               tTemperatureHot;                  /**< (K)     Nominal input data. */
        double                               tTemperatureCold;                 /**< (K)     Nominal input data. */
        static int                           TEST_ID;                          /**< (--)    Test identification number. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsThermoelectricEffect(const UtGunnsThermoelectricEffect& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsThermoelectricEffect& operator =(const UtGunnsThermoelectricEffect& that);
};

///@}

#endif
