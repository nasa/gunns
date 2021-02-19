#ifndef UtGunnsFluidUtils_EXISTS
#define UtGunnsFluidUtils_EXISTS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @defgroup UT_GUNNS_FLUID_UTILS    Utilities Unit Tests
/// @ingroup  UT_GUNNS
///
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
///
/// @details  Unit Tests for the GUNNS Fluid Utilities.
/// @{
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <iostream>

#include "core/GunnsFluidUtils.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Utilities unit tests.
////
/// @details  This class provides the unit tests for the GUNNS Fluid Utilities class within the
///           CPPUnit framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
class UtGunnsFluidUtils : public CppUnit::TestFixture
{
    public:
        /// @brief    Default constructs this Utilities unit test.
        UtGunnsFluidUtils();
        /// @brief    Default destructs this Utilities unit test.
        virtual ~UtGunnsFluidUtils();
        /// @brief    Executes before each test.
        void setUp();
        /// @brief    Executes after each test.
        void tearDown();
        void testAdmittance();
        void testLowDpAdmittance();
        void testComputeCapacitance();
        void testLowPressureCapacitance();
        void testZeroVolumeCapacitance();
        void testComputeIsentropicTemperature();
        void testPredictConductivity();
        void testPredictExpansionScaleFactor();
        void testBuildTransformMap();
        void testTransformState();
        void testConvectiveCoefficient();
        void testConductiveHeatFlux();
        void testConvectiveHeatFlux();
        void testGasDiffusion();
        void testRelativeHumidityH2O();
        void testMoleToMassFraction();
        void testMassToMoleFraction();
        void testPpToMoleFraction();
        void testMoleToPpFraction();
    private:
        CPPUNIT_TEST_SUITE(UtGunnsFluidUtils);
        CPPUNIT_TEST(testAdmittance);
        CPPUNIT_TEST(testLowDpAdmittance);
        CPPUNIT_TEST(testComputeCapacitance);
        CPPUNIT_TEST(testLowPressureCapacitance);
        CPPUNIT_TEST(testZeroVolumeCapacitance);
        CPPUNIT_TEST(testComputeIsentropicTemperature);
        CPPUNIT_TEST(testPredictConductivity);
        CPPUNIT_TEST(testPredictExpansionScaleFactor);
        CPPUNIT_TEST(testBuildTransformMap);
        CPPUNIT_TEST(testTransformState);
        CPPUNIT_TEST(testConvectiveCoefficient);
        CPPUNIT_TEST(testConvectiveHeatFlux);
        CPPUNIT_TEST(testConductiveHeatFlux);
        CPPUNIT_TEST(testGasDiffusion);
        CPPUNIT_TEST(testRelativeHumidityH2O);
        CPPUNIT_TEST(testMoleToMassFraction);
        CPPUNIT_TEST(testMassToMoleFraction);
        CPPUNIT_TEST(testPpToMoleFraction);
        CPPUNIT_TEST(testMoleToPpFraction);
        CPPUNIT_TEST_SUITE_END();
        DefinedFluidProperties*             mFluidProperties; /**< (--) Pointer to predefined fluid properties. */
        DefinedChemicalCompounds*           mTcProperties;    /**< (--) Pointer to predefined chemical compounds properties. */
        ChemicalCompound::Type              mTcTypes[4];      /**< (--) Array of trace compound types. */
        GunnsFluidTraceCompoundsConfigData* mFluidTcConfig1;  /**< (--) Pointer to Fluid 1 trace compounds config data. */
        PolyFluidConfigData*                mFluidConfig1;    /**< (--) Pointer to Fluid 1 config data. */
        PolyFluidConfigData*                mFluidConfig2;    /**< (--) Pointer to Fluid 2 config data. */
        double*                             fractions;        /**< (--) Pointer to Fluid Fractions. */
        PolyFluidInputData*                 mFluidInput1;     /**< (--) Pointer to Fluid 1 input data. */
        PolyFluidInputData*                 mFluidInput2;     /**< (--) Pointer to Fluid 2 input data. */
        PolyFluidInputData*                 mFluidInput3;     /**< (--) Pointer to Fluid 3 input data. */
        PolyFluid*                          tFluid1;          /**< (--) Pointer to Pointer to fluid 1. */
        PolyFluid*                          tFluid2;          /**< (--) Pointer to Pointer to fluid 2. */
        PolyFluid*                          tFluid3;          /**< (--) Pointer to Pointer to fluid 3. */
        /// @brief Copy constructor unavailable since declared private and not implemented.
        UtGunnsFluidUtils(const UtGunnsFluidUtils& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        UtGunnsFluidUtils& operator =(const UtGunnsFluidUtils& that);
};

///@}

#endif
