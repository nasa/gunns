#ifndef UtGunnsFluid3WayValve_EXISTS
#define UtGunnsFluid3WayValve_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_TSM_GUNNS_FLUID_CONDUCTOR_THREE_WAY_VALVE Three Way Valve Unit Tests
/// @ingroup  UT_TSM_GUNNS_FLUID_CONDUCTOR
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the Gunns Three Way Valve
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "aspects/fluid/conductor/GunnsFluid3WayValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluid3WayValve and befriend UtGunnsFluid3WayValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluid3WayValve : public GunnsFluid3WayValve
{
    public:
        FriendlyGunnsFluid3WayValve();
        virtual ~FriendlyGunnsFluid3WayValve();
        friend class UtGunnsFluid3WayValve;
};
inline FriendlyGunnsFluid3WayValve::FriendlyGunnsFluid3WayValve() : GunnsFluid3WayValve() {};
inline FriendlyGunnsFluid3WayValve::~FriendlyGunnsFluid3WayValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidValve and befriend UtGunnsFluid3WayValve.
///
/// @details  Class derived from an object embedded in the unit under test. It befriends the unit
///           test case driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidValveFor3Way : public GunnsFluidValve
{
    public:
        FriendlyGunnsFluidValveFor3Way();
        virtual ~FriendlyGunnsFluidValveFor3Way();
        friend class UtGunnsFluid3WayValve;
};
inline FriendlyGunnsFluidValveFor3Way::FriendlyGunnsFluidValveFor3Way() : GunnsFluidValve() {};
inline FriendlyGunnsFluidValveFor3Way::~FriendlyGunnsFluidValveFor3Way() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Three Way Valve  unit tests.
////
/// @details  This class provides the unit tests for the Three Way Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluid3WayValve: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluid3WayValve(const UtGunnsFluid3WayValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluid3WayValve& operator =(const UtGunnsFluid3WayValve& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluid3WayValve);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testRestart);
        CPPUNIT_TEST(testStep);
        CPPUNIT_TEST(testStepNonMixing);
        CPPUNIT_TEST(testDynamicPortMapping);
        CPPUNIT_TEST(testComputeFlow);
        CPPUNIT_TEST(testAccessMethods);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluid3WayValve*    tArticle;               /**< (--) Test article          */
        std::string                     tLinkName;              /**< (--) Nominal init data     */
        GunnsFluid3WayValveConfigData*  tConfigData;            /**< (--) Nominal config data   */
        bool                            tPositionPathB;         /**< (--) Nominal config data   */
        double                          tMaxConductivityA;      /**< (--) Nominal config data   */
        double                          tMaxConductivityB;      /**< (--) Nominal config data   */
        double                          tExpansionScaleFactorA; /**< (--) Nominal config data   */
        double                          tExpansionScaleFactorB; /**< (--) Nominal config data   */
        double                          tThermalLengthA;        /**< (--) Nominal config data   */
        double                          tThermalLengthB;        /**< (--) Nominal config data   */
        double                          tThermalDiameterA;      /**< (--) Nominal config data   */
        double                          tThermalDiameterB;      /**< (--) Nominal config data   */
        double                          tSurfaceRoughnessA;     /**< (--) Nominal config data   */
        double                          tSurfaceRoughnessB;     /**< (--) Nominal config data   */
        double                          tNonMixingBand;         /**< (--) Nominal config data   */
        GunnsFluid3WayValveInputData*   tInputData;             /**< (--) Nominal input data    */
        double                          tPosition;              /**< (--) Nominal input data    */
        double                          tWallTemperatureA;      /**< (--) Nominal input data    */
        double                          tWallTemperatureB;      /**< (--) Nominal input data    */
        GunnsFluidNode                  tNodes[3];              /**< (--) Test nodes            */
        GunnsNodeList                   tNodeList;              /**< (--) Test node list        */
        std::vector<GunnsBasicLink*>    tLinks;                 /**< (--) Test links vector     */
        int                             tPort0;                 /**< (--) Nominal init data     */
        int                             tPort1;                 /**< (--) Nominal init data     */
        int                             tPort2;                 /**< (--) Nominal init data     */
        double                          tTimeStep;              /**< (--) Test time step        */
        DefinedFluidProperties*         tFluidProperties;       /**< (--) Nominal init data     */
        PolyFluidConfigData*            tFluidConfig;           /**< (--) Nominal init data     */
        PolyFluidInputData*             tFluidInput1;           /**< (--) Nominal init data     */
        PolyFluidInputData*             tFluidInput2;           /**< (--) Nominal init data     */
        PolyFluidInputData*             tFluidInput3;           /**< (--) Nominal init data     */
        FriendlyGunnsFluidValveFor3Way* tPathA;                 /**< (--) Embedded path A valve */
        FriendlyGunnsFluidValveFor3Way* tPathB;                 /**< (--) Embedded path B valve */
        static int                      TEST_ID;                /**< (--) Test identification number. */

    public:
        UtGunnsFluid3WayValve();
        virtual ~UtGunnsFluid3WayValve();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testRestart();
        void testStep();
        void testStepNonMixing();
        void testDynamicPortMapping();
        void testComputeFlow();
        void testAccessMethods();
};

///@}

#endif
