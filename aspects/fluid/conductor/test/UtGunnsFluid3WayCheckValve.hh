#ifndef UtGunnsFluid3WayCheckValve_EXISTS
#define UtGunnsFluid3WayCheckValve_EXISTS

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

#include "aspects/fluid/conductor/GunnsFluid3WayCheckValve.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluid3WayCheckValve and befriend UtGunnsFluid3WayCheckValve.
///
/// @details  Class derived from the unit under test. It just has a constructor with the same
///           arguments as the parent and a default destructor, but it befriends the unit test case
///           driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluid3WayCheckValve : public GunnsFluid3WayCheckValve
{
    public:
        FriendlyGunnsFluid3WayCheckValve();
        virtual ~FriendlyGunnsFluid3WayCheckValve();
        friend class UtGunnsFluid3WayCheckValve;
};
inline FriendlyGunnsFluid3WayCheckValve::FriendlyGunnsFluid3WayCheckValve() : GunnsFluid3WayCheckValve() {};
inline FriendlyGunnsFluid3WayCheckValve::~FriendlyGunnsFluid3WayCheckValve() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Inherit from GunnsFluidValve and befriend UtGunnsFluid3WayCheckValve.
///
/// @details  Class derived from an object embedded in the unit under test. It befriends the unit
///           test case driver class to allow it access to protected data members.
////////////////////////////////////////////////////////////////////////////////////////////////////
class FriendlyGunnsFluidValveFor3WayCheck : public GunnsFluidValve
{
    public:
        FriendlyGunnsFluidValveFor3WayCheck();
        virtual ~FriendlyGunnsFluidValveFor3WayCheck();
        friend class UtGunnsFluid3WayCheckValve;
};
inline FriendlyGunnsFluidValveFor3WayCheck::FriendlyGunnsFluidValveFor3WayCheck() : GunnsFluidValve() {};
inline FriendlyGunnsFluidValveFor3WayCheck::~FriendlyGunnsFluidValveFor3WayCheck() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Three Way Valve  unit tests.
////
/// @details  This class provides the unit tests for the Three Way Valve within the CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluid3WayCheckValve: public CppUnit::TestFixture
{
    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluid3WayCheckValve(const UtGunnsFluid3WayCheckValve& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluid3WayCheckValve& operator =(const UtGunnsFluid3WayCheckValve& that);

        CPPUNIT_TEST_SUITE(UtGunnsFluid3WayCheckValve);
        CPPUNIT_TEST(testConfig);
        CPPUNIT_TEST(testInput);
        CPPUNIT_TEST(testDefaultConstruction);
        CPPUNIT_TEST(testAccessors);
        CPPUNIT_TEST(testNominalInitialization);
        CPPUNIT_TEST(testInitializationExceptions);
        CPPUNIT_TEST(testUpdateState);
        CPPUNIT_TEST_SUITE_END();

        FriendlyGunnsFluid3WayCheckValve*    tArticle;          /**< (--) Test article          */
        std::string                     tLinkName;              /**< (--) Nominal init data     */
        GunnsFluid3WayCheckValveConfigData*  tConfigData;       /**< (--) Nominal config data   */
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
        double                          tRateLimit;             /**< (--) Nominal config data   */
        double                          tOpenPressureA;         /**< (--) Nominal config data   */
        double                          tOpenPressureB;         /**< (--) Nominal config data   */
        double                          tMinPressDiff;          /**< (--) Nominal config data   */
        GunnsFluid3WayCheckValveInputData*   tInputData;        /**< (--) Nominal input data    */
        double                          tPosition;              /**< (--) Nominal input data    */
        double                          tWallTemperatureA;      /**< (--) Nominal input data    */
        double                          tWallTemperatureB;      /**< (--) Nominal input data    */
        bool                            tMalfStuckFlag;         /**< (--) Nominal input data    */
        bool                            tMalfFailToFlag;        /**< (--) Nominal input data    */
        double                          tMalfFailToValue;       /**< (--) Nominal input data    */
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
        FriendlyGunnsFluidValveFor3WayCheck* tPathA;            /**< (--) Embedded path A valve */
        FriendlyGunnsFluidValveFor3WayCheck* tPathB;            /**< (--) Embedded path B valve */
        static int                      TEST_ID;                /**< (--) Test identification number. */

    public:
        UtGunnsFluid3WayCheckValve();
        virtual ~UtGunnsFluid3WayCheckValve();
        void tearDown();
        void setUp();
        void testConfig();
        void testInput();
        void testDefaultConstruction();
        void testAccessors();
        void testNominalInitialization();
        void testInitializationExceptions();
        void testUpdateState();
};

///@}

#endif
