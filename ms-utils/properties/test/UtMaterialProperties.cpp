/********************************* TRICK HEADER *******************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
(
    (properties/MaterialProperties.o)
    (properties/PredefinedMaterialProperties.o)
)
*******************************************************************************/

#include <iostream>

#include "UtMaterialProperties.hh"
#include "properties/MaterialProperties.hh"
#include "properties/PredefinedMaterialProperties.hh"
#include "software/exceptions/TsOutOfBoundsException.hh"

using std::cout;
using std::endl;

static int material_properties_tested = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Material Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtMaterialProperties::UtMaterialProperties()
    :
    CppUnit::TestFixture(),
    minValue(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Material Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtMaterialProperties::~UtMaterialProperties()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMaterialProperties::setUp()
{
    minValue = 1.0e-10;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtMaterialProperties::tearDown()
{
    // nothing to do
}

/// @brief check minimum values
void UtMaterialProperties::checkMinimums(const MaterialProperties &props) {
    if(props.thermalConductivity < minValue) {
        CPPUNIT_ASSERT(false);
    }
    if(props.roughness < minValue) {
        CPPUNIT_ASSERT(false);
    }
    if(props.specificHeatCp < minValue) {
        CPPUNIT_ASSERT(false);
    }
    if(props.density < minValue) {
        CPPUNIT_ASSERT(false);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// This is a test method for steel304.
////////////////////////////////////////////////////////////////////////////////////////////
void UtMaterialProperties::Teststeel304()
{
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n MaterialProperties 01: steel304 Test                                   ";

    MaterialProperties *props = PredefinedMaterialProperties::getMaterialProperties("Steel 304");
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.13360E-06,
            props->roughness, 0.00000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(7910.0,
            props->density, 0.000000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(490.0,
            props->specificHeatCp, 0.000000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(14.75,
            props->thermalConductivity, 0.00000001);

    checkMinimums(*props);

    material_properties_tested++;

    std::cout << "... Pass";
}


////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// This is a test method for Aluminum_60601.
////////////////////////////////////////////////////////////////////////////////////////////
void UtMaterialProperties::Testaluminum6061()
{
    std::cout << "\n MaterialProperties 02: aluminum6061 Test                               ";

    MaterialProperties *props =
            PredefinedMaterialProperties::getMaterialProperties("Aluminum 6061");

/*           CPPUNIT_ASSERT_DOUBLES_EQUAL(3.81E-07,
            MaterialProperties::aluminum6061.roughness, 0.00000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2712.55219,
            MaterialProperties::aluminum6061.density, 0.000000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(879.249,
            MaterialProperties::aluminum6061.specificHeatCp, 0.000000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(173.10461,
            MaterialProperties::aluminum6061.thermalConductivity, 0.00000001);*/


    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.81E-07, props->roughness, 0.00000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2712.55219, props->density, 0.000000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(879.249, props->specificHeatCp, 0.000000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(173.10461, props->thermalConductivity, 0.00000001);

//            checkMinimums(MaterialProperties::aluminum6061);
    checkMinimums(*props);

    material_properties_tested++;

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @details
/// This is a test method for the default constructor.
////////////////////////////////////////////////////////////////////////////////////////////
void UtMaterialProperties::TestDefaultConstructor() {
    class MyMaterialProperties : public MaterialProperties {

    };

    std::cout << "\n MaterialProperties 03: Default Construction Test                       ";

    MyMaterialProperties foo;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            foo.roughness, 0.00000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            foo.density, 0.000000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            foo.specificHeatCp, 0.000000001);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,
            foo.thermalConductivity, 0.00000001);

    std::cout << "... Pass";
}

void UtMaterialProperties::GetKnownProperty() {

    std::cout << "\n MaterialProperties 04: Get Known Property Test                         ";

    CPPUNIT_ASSERT_NO_THROW(PredefinedMaterialProperties::getMaterialProperties("Steel 304"));

    std::cout << "... Pass";
}

void UtMaterialProperties::GetUnknownProperty() {
    std::cout << "\n MaterialProperties 05: Get Unknown Property Test                       ";

    CPPUNIT_ASSERT_THROW(PredefinedMaterialProperties::getMaterialProperties("foo bar"),
            TsOutOfBoundsException);

    std::cout << "... Pass";
}

void UtMaterialProperties::AllProertiesTested() {
    std::cout << "\n MaterialProperties 06: All Properties Checked Test                     ";

    CPPUNIT_ASSERT( (material_properties_tested == PredefinedMaterialProperties::size() ));

    std::cout << "... Pass";
}
