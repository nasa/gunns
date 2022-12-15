/*
@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.
*/

#include <iostream>

#include "math/approximation/LinearFit.hh"

#include "UtFluidProperties.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Fluid Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtFluidProperties::UtFluidProperties()
    :
    CppUnit::TestFixture(),
    mArticle(0),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Fluid Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtFluidProperties::~UtFluidProperties()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::setUp()
{
    mArticle   = new FriendlyDefinedFluidProperties();
    mTolerance = 0.001;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::tearDown()
{
   delete mArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for default construction without exceptions.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testConstruction()
{
    std::cout << '\n';
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n Fluid Properties 01: Default Constructor Test                          ";

    CPPUNIT_ASSERT(0 != mArticle->mProperties);
    for (int i = 0; i < FluidProperties::NO_FLUID; i++) {
         CPPUNIT_ASSERT(0 != mArticle->mProperties[i]);
    }

    std::cout << "... Pass";
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for consistency between density and pressure..
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testAccessors()
{
    std::cout << "\n Fluid Properties 02: Test Accessors                                    ";

    /// @test for each fluid type (just for existence)
    for (int j = 0; j  < FluidProperties::NO_FLUID; j++) {
        const FluidProperties::FluidType i = static_cast<const FluidProperties::FluidType>(j);
        const double t = 300.0;
        const double p = 100000.0;
        const double h = 10000.0;
        const double d = 30.0;
        const FluidProperties::FluidType type   =
            mArticle->getProperties(i)->getType();
        /// @test getType
        const FluidProperties::FluidPhase phase =
            mArticle->getProperties(i)->getPhase();
        /// @test getMWeight
        const double mWeight                    =
            mArticle->getProperties(i)->getMWeight();
        /// @test getCriticalTemperature
        const double criticalTemperature        =
            mArticle->getProperties(i)->getCriticalTemperature();
        /// @test getDensity
        const double density                    =
            mArticle->getProperties(i)->getDensity(t, p);
        /// @test getViscosity
        const double viscosity                  =
            mArticle->getProperties(i)->getViscosity(t, p);
        /// @test getSpecificHeat
        const double specificHeat               =
            mArticle->getProperties(i)->getSpecificHeat(t, p);
        /// @test getSpecificEnthalpy
        const double specificEnthalpy           =
            mArticle->getProperties(i)->getSpecificEnthalpy(t, p);
        /// @test getAdiabaticIndex
        const double adiabaticIndex             =
            mArticle->getProperties(i)->getAdiabaticIndex(t, p);
        /// @test getAdiabaticIndex
        const double thermalConductivity        =
            mArticle->getProperties(i)->getThermalConductivity(t, p);
        /// @test getAdiabaticIndex
        const double prandtlNumber              =
            mArticle->getProperties(i)->getPrandtlNumber(t, p);
        /// @test getPressure
        const double pressure                   =
            mArticle->getProperties(i)->getPressure(t, d);
        /// @test getTemperature
        const double temperature                =
            mArticle->getProperties(i)->getTemperature(h, p);
        /// @test getSaturationPressure
        const double saturationPressure         =
            mArticle->getProperties(i)->getSaturationPressure(t);
        /// @test getSaturationTemperature
        const double saturationTemperature      =
            mArticle->getProperties(i)->getSaturationTemperature(p);
        /// @test getSaturationTemperature
        const double heatOfVaporization         =
            mArticle->getProperties(i)->getHeatOfVaporization(t);
    }

    std::cout << "... Pass";
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for consistency between density and pressure..
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testDensityPressureConsistency()
{
    std::cout << "\n Fluid Properties 03: Test for Density/Pressure Consistency             ";

    for (int i = 0; i  < FluidProperties::NO_FLUID; i++) {
        FluidProperties::FluidType type = static_cast<FluidProperties::FluidType>(i);
        const double temperature = 278.15;
        const double expectedP   = 1013.25;
        const double expectedD   = mArticle->getProperties(type)->
            getDensity(temperature, expectedP);
        const double returnedP   = mArticle->getProperties(type)->
            getPressure(temperature, expectedD);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, 5.0e-04);
        const double returnedD   = mArticle->getProperties(type)->
            getDensity(temperature, returnedP);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedD, returnedD, 5.0e-04);
    }

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for consistency between specific enthalpy and temperature.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testSpecificEnthalpyTemperatureConsistency()
{
    std::cout << "\n Fluid Properties 04: Test for SpecificEnthalpy/Temperature Consistency ";

    for (int i = 0; i  < FluidProperties::GUNNS_WATER; i++) {
        FluidProperties::FluidType type = static_cast<FluidProperties::FluidType>(i);
        const double pressure           = 1000.0;
        const double expected           = 400.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_WATER;
        const double pressure           = 600.0;
        const double expected           = 300.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_HFE7000;
        const double pressure           = 600.0;
        const double expected           = 250.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_HFE7100;
        const double pressure           = 600.0;
        const double expected           = 250.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_PG30;
        const double pressure           = 600.0;
        const double expected           = 300.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_PG40;
        const double pressure           = 600.0;
        const double expected           = 300.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_PG50;
        const double pressure           = 600.0;
        const double expected           = 300.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_AMMONIA;
        const double pressure           = 600.0;
        const double expected           = 220.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_OXYGEN;
        const double pressure           = 600.0;
        const double expected           = 100.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_HYDROGEN;
        const double pressure           = 500.0;
        const double expected           = 20.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_METHANE;
        const double pressure           = 600.0;
        const double expected           = 100.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_NAK78;
        const double pressure           = 600.0;
        const double expected           = 900.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_GALDEN170;
        const double pressure           = 600.0;
        const double expected           = 400.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_NTO;
        const double pressure           = 600.0;
        const double expected           = 400.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_MMH;
        const double pressure           = 600.0;
        const double expected           = 400.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    } {
        FluidProperties::FluidType type = FluidProperties::GUNNS_HYDRAZINE;
        const double pressure           = 600.0;
        const double expected           = 300.0;
        const double specificEnthalpy   = mArticle->getProperties(type)->
            getSpecificEnthalpy(expected, pressure);
        const double returned           = mArticle->getProperties(type)->
            getTemperature(specificEnthalpy, pressure);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, 1.0e-10);
    }

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for a return value of zero within valid range of input.  All fluid properties
///           should be positive across the entire input range.  For now, we test the min & max
///           range points, which suffices for linear fits.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testForPositive()
{
    std::cout << "\n Fluid Properties 05: Test for Positive                                 ";

    for (int i = 0; i < FluidProperties::NO_FLUID; i++) {
        std::stringstream ss;
        ss << "Fluid type: " << i;
        FluidProperties::FluidType type = static_cast<FluidProperties::FluidType>(i);
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getMWeight());

        /// - Test minimum range.
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <= mArticle->getProperties(type)->getDensity(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getViscosity(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getSpecificHeat(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 == mArticle->getProperties(type)->getSpecificEnthalpy(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getThermalConductivity(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getPrandtlNumber(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getAdiabaticIndex(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getPressure(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getTemperature(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getSaturationPressure(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getSaturationTemperature(0.0));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getHeatOfVaporization(0.0));

        /// - Test maximum range.
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <= mArticle->getProperties(type)->getDensity(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getViscosity(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getSpecificHeat(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getSpecificEnthalpy(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getThermalConductivity(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getPrandtlNumber(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getAdiabaticIndex(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getPressure(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getTemperature(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getSaturationPressure(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <  mArticle->getProperties(type)->getSaturationTemperature(1.0E15));
        CPPUNIT_ASSERT_MESSAGE(ss.str(), 0.0 <= mArticle->getProperties(type)->getHeatOfVaporization(1.0E15));
}

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for consistency between density and pressure..
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testLowP()
{
    std::cout << "\n Fluid Properties 06: Test for Density/Pressure Consistency  at Low P   ";

    for (int i = 0; i  < FluidProperties::GUNNS_WATER; i++) {
        FluidProperties::FluidType type = static_cast<FluidProperties::FluidType>(i);
        for (int j = 0; j < 10; ++j) {
            const double temperature = 270.0;
            const double expectedP   = FLT_EPSILON + FLT_EPSILON * 0.1 * j;
            const double expectedD   = mArticle->getProperties(type)->
                getDensity(temperature, expectedP);
            const double returnedP   = mArticle->getProperties(type)->
                getPressure(temperature, expectedD);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, 1.0e-13);
            const double returnedD   = mArticle->getProperties(type)->
                getDensity(temperature, returnedP);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedD, returnedD, 1.0e-13);
//            std::cout << "\n expected pressure " << expectedP << " returned pressure " << returnedP
//                      << " expected density " << expectedD << " returned density " << returnedD;
        }
    }
//    std::cout << '\n';
    for (int i = FluidProperties::GUNNS_WATER; i  < FluidProperties::NO_FLUID; i++) {
        FluidProperties::FluidType type = static_cast<FluidProperties::FluidType>(i);
        for (int j = 0; j < 10; ++j) {
            const double temperature = 270.0;
            const double expectedP   = FLT_EPSILON + FLT_EPSILON * 0.1 * j;
            const double expectedD   = mArticle->getProperties(type)->
                getDensity(temperature, expectedP);
            const double returnedP   = mArticle->getProperties(type)->
                getPressure(temperature, expectedD);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedP, returnedP, 1.0e-09);
            const double returnedD   = mArticle->getProperties(type)->
                getDensity(temperature, returnedP);
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedD, returnedD, 1.0e-09);
//            std::cout << "\n expected pressure " << expectedP << " returned pressure " << returnedP
//                      << " expected density " << expectedD << " returned density " << returnedD;
        }
    }
//    std::cout << '\n';
    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test invalid range bounds.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testInvalidRangeBounds()
{
    std::cout << "\n Fluid Properties 07: Invalid Range Bounds                              ";

    // Note that it suffices to test upper and lower bounds on each method for any fluid.
    {
        /// @test getDensity lower temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_WATER]->
            getDensity(100.0, 1000000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_WATER]->
            getDensity(199.0, 1000000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getDensity upper temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_WATER]->
            getDensity(470.0, 1000000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_WATER]->
            getDensity(471.0, 1000000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getDensity lower pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_N2]->
            getDensity(300.0, 1.0e-06);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_N2]->
            getDensity(300.0, 1.0e-07);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getDensity upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_N2]->
            getDensity(300.0, 1.0e+06);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_N2]->
            getDensity(300.0, 1.0e+07);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getViscosity lower temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_CO]->
            getViscosity(100.0, 1000000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_CO]->
            getViscosity(99.0, 1000000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getViscosity upper temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_CO]->
            getViscosity(2000.0, 1000000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_CO]->
            getViscosity(2001.0, 1000000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getViscosity lower pressure limit
//        const double expected =
//            mArticle->mProperties[FluidProperties::GUNNS_CO]->
//            getViscosity(300.0, 1.0e-12);
//        const double returned =
//            mArticle->mProperties[FluidProperties::GUNNS_CO]->
//            getViscosity(300.0, 1.0e-13);
//        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getViscosity upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_CO]->
            getViscosity(300.0, 1.0e+12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_CO]->
            getViscosity(300.0, 1.0e+13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        ///@test getSpecificHeat lower temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_HFE7000]->
            getSpecificHeat(100.0, 1000000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_HFE7000]->
            getSpecificHeat(99.0, 1000000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSpecificHeat upper temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_HFE7000]->
            getSpecificHeat(470.0, 1000000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_HFE7000]->
            getSpecificHeat(471.0, 1000000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        ///@test getSpecificHeat lower pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_HFE7000]->
            getSpecificHeat(300.0, 1.0e-12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_HFE7000]->
            getSpecificHeat(300.0, 1.0e-13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSpecificHeat upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_HFE7000]->
            getSpecificHeat(300.0, 1.0e+12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_HFE7000]->
            getSpecificHeat(300.0, 1.0e+13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        ///@test getSpecificEnthalpy lower temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_CO2]->
            getSpecificHeat(100.0, 1000.0) * 99.0;
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_CO2]->
            getSpecificEnthalpy(99.0, 1000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSpecificEnthalpy upper temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_CO2]->
            getSpecificHeat(2000.0, 1000.0) * 2001.0;
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_CO2]->
            getSpecificEnthalpy(2001.0, 1000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        ///@test getSpecificEnthalpy lower pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_CO2]->
            getSpecificEnthalpy(300.0, 1.0e-12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_CO2]->
            getSpecificEnthalpy(300.0, 1.0e-13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSpecificEnthalpy upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_CO2]->
            getSpecificEnthalpy(300.0, 1.0e+12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_CO2]->
            getSpecificEnthalpy(300.0, 1.0e+13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getAdiabaticIndex lower temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_NH3]->
            getAdiabaticIndex(100.0, 1000000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_NH3]->
            getAdiabaticIndex(99.0, 1000000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getAdiabaticIndex upper temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_NH3]->
            getAdiabaticIndex(2000.0, 1000000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_NH3]->
            getAdiabaticIndex(2001.0, 1000000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getAdiabaticIndex lower pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_NH3]->
            getAdiabaticIndex(300.0, 1.0e-12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_NH3]->
            getAdiabaticIndex(300.0, 1.0e-13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getAdiabaticIndex upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_NH3]->
            getAdiabaticIndex(300.0, 1.0e+12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_NH3]->
            getAdiabaticIndex(300.0, 1.0e+13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getPressure lower temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_O2]->
            getPressure(50.0, 1.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_O2]->
            getPressure(49.0, 1.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getPressure upper temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_O2]->
            getPressure(2000.0, 1.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_O2]->
            getPressure(2001.0, 1.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getPressure lower density limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_O2]->
            getPressure(300.0, 1.0e-12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_O2]->
            getPressure(300.0, 1.0e-13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getPressure upper density limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_O2]->
            getPressure(300.0, 1.0e+12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_O2]->
            getPressure(300.0, 1.0e+13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getTemperature lower specific enthalpy limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_H2]->
            getTemperature(1.0e-06, 100000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_H2]->
            getTemperature(1.0e-07, 100000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getTemperature upper specific enthalpy limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_H2]->
            getTemperature(1.0e+12, 100000.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_H2]->
            getTemperature(1.0e+13, 100000.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getTemperature lower pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_H2]->
            getTemperature( 1.0, 1.0e-12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_H2]->
            getTemperature(1.0, 1.0e-13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getTemperature upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_H2]->
            getTemperature(1.0, 1.0e+12);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_H2]->
            getTemperature(1.0, 1.0e+13);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSaturationPressure lower temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_HE]->
            getSaturationPressure(1.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_HE]->
            getSaturationPressure(0.5);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSaturationPressure upper temperature limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_HE]->
            getSaturationPressure(6.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_HE]->
            getSaturationPressure(7.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSaturationTemperature lower pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_METHANE]->
            getSaturationTemperature(12.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_METHANE]->
            getSaturationTemperature(11.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSaturationTemperature upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_NTO]->
            getSaturationTemperature(9929.85);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_NTO]->
            getSaturationTemperature(9930.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSaturationTemperature upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_MMH]->
            getSaturationTemperature(8030.46);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_MMH]->
            getSaturationTemperature(8031.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    } {
        /// @test getSaturationTemperature upper pressure limit
        const double expected =
            mArticle->mProperties[FluidProperties::GUNNS_HYDRAZINE]->
            getSaturationTemperature(14784.0);
        const double returned =
            mArticle->mProperties[FluidProperties::GUNNS_HYDRAZINE]->
            getSaturationTemperature(14785.0);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test table look-ups for GUNNS_HE_REAL_GAS
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testHeTable()
{
    std::cout << "\n Fluid Properties 8: He Density Table                                  ";

    /// @test A few specific points for good table data.  Table corners:
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_HE_REAL_GAS)->getDensity(2.1768, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(303.4472,
                                 mArticle->getProperties(FluidProperties::GUNNS_HE_REAL_GAS)->getDensity(2.1768, 60000.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_HE_REAL_GAS)->getDensity(1000.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(27.06089,
                                 mArticle->getProperties(FluidProperties::GUNNS_HE_REAL_GAS)->getDensity(1000.0, 60000.0),
                                 FLT_EPSILON);

    /// @test Critical point:
    CPPUNIT_ASSERT_DOUBLES_EQUAL(66.78098,
                                 mArticle->getProperties(FluidProperties::GUNNS_HE_REAL_GAS)->getDensity(5.1953, 228.32),
                                 DBL_EPSILON);

    /// @test Loop across the entire table and check for good inverse between pressure and density
    ///       at all points.
    const double maxP = 60000.0;
    const double minP = 0.0;
    const double maxT = 1000.0;
    const double minT = 2.1768;
    double maxE  = 0.0;
    double maxEP = 0.0;
    double maxET = 0.0;
    int    maxI  = 0;
    int    maxJ  = 0;
    for (int i=0; i<1000; ++i) {
        for (int j=0; j<1000; ++j) {
            const double pressure    = minP + (maxP - minP) * i / 1000.0;
            const double temperature = minT + (maxT - minT) * j / 1000.0;
            const double density = mArticle->getProperties(FluidProperties::GUNNS_HE_REAL_GAS)->getDensity(temperature, pressure);
            const double error = pressure - mArticle->getProperties(FluidProperties::GUNNS_HE_REAL_GAS)->getPressure(temperature, density);
            if (fabs(error) > maxE) {
                maxEP = pressure;
                maxET = temperature;
                maxI  = i;
                maxJ  = j;
                maxE  = error;
            }
        }
    }
//    std::cout << "\n     max inverting error =" << maxE << " @ (" << maxI << "," << maxJ << ") P=" << maxEP << ", T=" << maxET;
//    std::cout << "\n                                                                        ";
    CPPUNIT_ASSERT(maxE < 1.0E-9);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test table look-ups for GUNNS_XE_REAL_GAS
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testXeTable()
{
    std::cout << "\n Fluid Properties 9: Xe Density Table                                  ";

    /// @test A few specific points for good table data.  Table corners:
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_XE_REAL_GAS)->getDensity(170.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3052.36,
                                 mArticle->getProperties(FluidProperties::GUNNS_XE_REAL_GAS)->getDensity(170.0, 34473.8),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_XE_REAL_GAS)->getDensity(750.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(692.003,
                                 mArticle->getProperties(FluidProperties::GUNNS_XE_REAL_GAS)->getDensity(750.0, 34473.8),
                                 FLT_EPSILON);

    /// @test Critical point:
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1118.36,
                                 mArticle->getProperties(FluidProperties::GUNNS_XE_REAL_GAS)->getDensity(289.733, 5841.97),
                                 DBL_EPSILON);

    /// @test Loop across the entire table and check for good inverse between pressure and density
    ///       at all points.
    const double maxP = 34473.8;
    const double minP = 0.0;
    const double maxT = 750.0;
    const double minT = 170.0;
    double maxE  = 0.0;
    double maxEP = 0.0;
    double maxET = 0.0;
    int    maxI  = 0;
    int    maxJ  = 0;
    for (int i=0; i<1000; ++i) {
        for (int j=0; j<1000; ++j) {
            const double pressure    = minP + (maxP - minP) * i / 1000.0;
            const double temperature = minT + (maxT - minT) * j / 1000.0;
            const double density = mArticle->getProperties(FluidProperties::GUNNS_XE_REAL_GAS)->getDensity(temperature, pressure);
            const double error = pressure - mArticle->getProperties(FluidProperties::GUNNS_XE_REAL_GAS)->getPressure(temperature, density);
            if (fabs(error) > maxE) {
                maxEP = pressure;
                maxET = temperature;
                maxI  = i;
                maxJ  = j;
                maxE  = error;
            }
        }
    }
//    std::cout << "\n     max inverting error =" << maxE << " @ (" << maxI << "," << maxJ << ") P=" << maxEP << ", T=" << maxET;
//    std::cout << "\n                                                                        ";
    CPPUNIT_ASSERT(maxE < 1.0E-9);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test table look-ups for GUNNS_N2_REAL_GAS
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testN2Table()
{
    std::cout << "\n Fluid Properties 10: N2 Density Table                                  ";

    /// @test A few specific points for good table data.  Table corners:
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_N2_REAL_GAS)->getDensity(160.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(693.263,
                                 mArticle->getProperties(FluidProperties::GUNNS_N2_REAL_GAS)->getDensity(160.0, 59090.9),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_N2_REAL_GAS)->getDensity(750.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(209.91,
                                 mArticle->getProperties(FluidProperties::GUNNS_N2_REAL_GAS)->getDensity(750.0, 59090.9),
                                 FLT_EPSILON);

    /// @test Loop across the entire table and check for good inverse between pressure and density
    ///       at all points.
    const double maxP = 59090.9;
    const double minP = 0.0;
    const double maxT = 750.0;
    const double minT = 160.0;
    double maxE  = 0.0;
    double maxEP = 0.0;
    double maxET = 0.0;
    int    maxI  = 0;
    int    maxJ  = 0;
    for (int i=0; i<1000; ++i) {
        for (int j=0; j<1000; ++j) {
            const double pressure    = minP + (maxP - minP) * i / 1000.0;
            const double temperature = minT + (maxT - minT) * j / 1000.0;
            const double density = mArticle->getProperties(FluidProperties::GUNNS_N2_REAL_GAS)->getDensity(temperature, pressure);
            const double error = pressure - mArticle->getProperties(FluidProperties::GUNNS_N2_REAL_GAS)->getPressure(temperature, density);
            if (fabs(error) > maxE) {
                maxEP = pressure;
                maxET = temperature;
                maxI  = i;
                maxJ  = j;
                maxE  = error;
            }
        }
    }
//    std::cout << "\n     max inverting error =" << maxE << " @ (" << maxI << "," << maxJ << ") P=" << maxEP << ", T=" << maxET;
//    std::cout << "\n                                                                        ";
    CPPUNIT_ASSERT(maxE < 1.0E-9);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test table look-ups for GUNNS_O2_REAL_GAS
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testO2Table()
{
    std::cout << "\n Fluid Properties 11: O2 Density Table                                  ";

    /// @test A few specific points for good table data.  Table corners:
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_O2_REAL_GAS)->getDensity(160.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1003.26,
                                 mArticle->getProperties(FluidProperties::GUNNS_O2_REAL_GAS)->getDensity(160.0, 59090.9),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_O2_REAL_GAS)->getDensity(750.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(254.064,
                                 mArticle->getProperties(FluidProperties::GUNNS_O2_REAL_GAS)->getDensity(750.0, 59090.9),
                                 FLT_EPSILON);

    /// @test Loop across the entire table and check for good inverse between pressure and density
    ///       at all points.
    const double maxP = 59090.9;
    const double minP = 0.0;
    const double maxT = 750.0;
    const double minT = 160.0;
    double maxE  = 0.0;
    double maxEP = 0.0;
    double maxET = 0.0;
    int    maxI  = 0;
    int    maxJ  = 0;
    for (int i=0; i<1000; ++i) {
        for (int j=0; j<1000; ++j) {
            const double pressure    = minP + (maxP - minP) * i / 1000.0;
            const double temperature = minT + (maxT - minT) * j / 1000.0;
            const double density = mArticle->getProperties(FluidProperties::GUNNS_O2_REAL_GAS)->getDensity(temperature, pressure);
            const double error = pressure - mArticle->getProperties(FluidProperties::GUNNS_O2_REAL_GAS)->getPressure(temperature, density);
            if (fabs(error) > maxE) {
                maxEP = pressure;
                maxET = temperature;
                maxI  = i;
                maxJ  = j;
                maxE  = error;
            }
        }
    }
//    std::cout << "\n     max inverting error =" << maxE << " @ (" << maxI << "," << maxJ << ") P=" << maxEP << ", T=" << maxET;
//    std::cout << "\n                                                                        ";
    CPPUNIT_ASSERT(maxE < 1.0E-9);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test table look-ups for GUNNS_H2_REAL_GAS
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testH2Table()
{
    std::cout << "\n Fluid Properties 12: H2 Density Table                                  ";

    /// @test A few specific points for good table data.  Table corners:
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_H2_REAL_GAS)->getDensity(64.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(88.3871,
                                 mArticle->getProperties(FluidProperties::GUNNS_H2_REAL_GAS)->getDensity(64.0, 80000.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
                                 mArticle->getProperties(FluidProperties::GUNNS_H2_REAL_GAS)->getDensity(1000.0, 0.0),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(16.8613,
                                 mArticle->getProperties(FluidProperties::GUNNS_H2_REAL_GAS)->getDensity(1000.0, 80000.0),
                                 FLT_EPSILON);

    /// @test Loop across the entire table and check for good inverse between pressure and density
    ///       at all points.
    const double maxP = 80000.0;
    const double minP = 0.0;
    const double maxT = 1000.0;
    const double minT = 64.0;
    double maxE  = 0.0;
    double maxEP = 0.0;
    double maxET = 0.0;
    int    maxI  = 0;
    int    maxJ  = 0;
    for (int i=0; i<1000; ++i) {
        for (int j=0; j<1000; ++j) {
            const double pressure    = minP + (maxP - minP) * i / 1000.0;
            const double temperature = minT + (maxT - minT) * j / 1000.0;
            const double density = mArticle->getProperties(FluidProperties::GUNNS_H2_REAL_GAS)->getDensity(temperature, pressure);
            const double error = pressure - mArticle->getProperties(FluidProperties::GUNNS_H2_REAL_GAS)->getPressure(temperature, density);
            if (fabs(error) > maxE) {
                maxEP = pressure;
                maxET = temperature;
                maxI  = i;
                maxJ  = j;
                maxE  = error;
            }
        }
    }
//    std::cout << "\n     max inverting error =" << maxE << " @ (" << maxI << "," << maxJ << ") P=" << maxEP << ", T=" << maxET;
//    std::cout << "\n                                                                        ";
    CPPUNIT_ASSERT(maxE < 1.0E-9);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test table look-ups for GUNNS_WATER_PVT
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testWaterPvtTable()
{
    std::cout << "\n Fluid Properties 13: Water PVT Density Table                           ";

    /// @test A few specific points for good table data.  Table corners:
    CPPUNIT_ASSERT_DOUBLES_EQUAL(999.792208924,
                                 mArticle->getProperties(FluidProperties::GUNNS_WATER_PVT)->getDensity(273.16, 1.0e-10),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1000.3387535,
                                 mArticle->getProperties(FluidProperties::GUNNS_WATER_PVT)->getDensity(273.16, 1075.4274162),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(999.792208924,
                                 mArticle->getProperties(FluidProperties::GUNNS_WATER_PVT)->getDensity(373.506467, 1.0e-10),
                                 FLT_EPSILON);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(958.549732485,
                                 mArticle->getProperties(FluidProperties::GUNNS_WATER_PVT)->getDensity(373.506467, 1075.4274162),
                                 FLT_EPSILON);

    /// @test Loop across the entire table and check for good inverse between pressure and density
    ///       at all points.
    const double maxP = 1075.4274162;
    const double minP = 1.0e-10;
    const double maxT = 373.506467;
    const double minT = 273.16;
    double maxE  = 0.0;
    double maxEP = 0.0;
    double maxET = 0.0;
    int    maxI  = 0;
    int    maxJ  = 0;
    for (int i=0; i<1000; ++i) {
        for (int j=0; j<1000; ++j) {
            const double pressure    = minP + (maxP - minP) * i / 1000.0;
            const double temperature = minT + (maxT - minT) * j / 1000.0;
            const double density = mArticle->getProperties(FluidProperties::GUNNS_WATER_PVT)->getDensity(temperature, pressure);
            const double error = pressure - mArticle->getProperties(FluidProperties::GUNNS_WATER_PVT)->getPressure(temperature, density);
            if (fabs(error) > maxE) {
                maxEP = pressure;
                maxET = temperature;
                maxI  = i;
                maxJ  = j;
                maxE  = error;
            }
        }
    }
//    std::cout << "\n     max inverting error =" << maxE << " @ (" << maxI << "," << maxJ << ") P=" << maxEP << ", T=" << maxET;
//    std::cout << "\n                                                                        ";
    CPPUNIT_ASSERT(maxE < 1.0E-9);

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Tests reversibility of the saturation curve.  Ts = f(Ps = f(Ts)).
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtFluidProperties::testSaturationCurveConsistency()
{
    std::cout << "\n Fluid Properties 14: Saturation Curve Consistency                      ";

    // Pick test Ts points for each fluid type in between triple & critical points.  Use all
    // unique values so we can find which fluid type fails.
    const double temperature[] = {100.0, 200.0, 300.0, 101.0, 102.0,
                                  202.0,  20.0, 103.0, 203.0, 204.0,
                                    4.0,   4.0, 205.0, 106.0, 107.0,
                                   21.0, 207.0, 308.0, 309.0, 400.0,
                                  401.0, 402.0, 310.0,  60.0,  20.0,
                                  150.0, 311.0, 312.0, 313.0, 314.0,
                                  315.0, 316.0};

    for (int i = 0; i  < FluidProperties::NO_FLUID; i++) {
        FluidProperties::FluidType type = static_cast<FluidProperties::FluidType>(i);
        const double Ps   = mArticle->getProperties(type)->getSaturationPressure(temperature[i]);
        const double Ts   = mArticle->getProperties(type)->getSaturationTemperature(Ps);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(temperature[i], Ts, FLT_EPSILON);
    }

    std::cout << "... Pass" << std::endl;
}
