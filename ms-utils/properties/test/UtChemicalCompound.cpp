/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
    ((properties/ChemicalCompound.o))
***************************************************************************************************/

#include <iostream>

#include "properties/ChemicalCompound.hh"

#include "UtChemicalCompound.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Chemical Compound Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtChemicalCompound::UtChemicalCompound()
    :
    CppUnit::TestFixture(),
    mTolerance(0.0)
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Chemical Compound Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtChemicalCompound::~UtChemicalCompound()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalCompound::setUp()
{
    mTolerance = 1.0e-16;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalCompound::tearDown()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for construction.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalCompound::testConstruction()
{
    std::cout << "--------------------------------------------------------------------------------";
    std::cout << "\n Chemical Compound Properties 01: Nominal Construction Test             ";

    {
        /// @test object for creation of properties for each compound type.
        DefinedChemicalCompounds compounds;
        for (int i = 0; i < ChemicalCompound::NO_COMPOUND; i++) {
            const ChemicalCompound::Type expected = static_cast<ChemicalCompound::Type>(i);
            const ChemicalCompound::Type returned = compounds.getCompound(expected)->mType;
            CPPUNIT_ASSERT(expected == returned);
        }
    } {
        /// @test pointer for creation of properties for each compound type.
        DefinedChemicalCompounds* compounds;
        TS_NEW_CLASS_OBJECT(compounds, DefinedChemicalCompounds, ());
        for (int i = 0; i < ChemicalCompound::NO_COMPOUND; i++) {
            const ChemicalCompound::Type expected = static_cast<ChemicalCompound::Type>(i);
            const ChemicalCompound::Type returned = compounds->getCompound(expected)->mType;
            CPPUNIT_ASSERT(expected == returned);
        }
        TS_DELETE_OBJECT(compounds);
    }

    std::cout << "... Pass";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test for chemical compound properties consistency with fluid properties..
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtChemicalCompound::testConsistency()
{
    std::cout << "\n Chemical Compound Properties 02: Consistency Test                      ";

    DefinedChemicalCompounds compounds;

    /// @test for molecular weight consistency between compound and corresponding fluid.
    for (int i = 0; i < ChemicalCompound::NO_COMPOUND; i++) {
        const ChemicalCompound::Type type = static_cast<ChemicalCompound::Type>(i);
        const double returned = compounds.getCompound(type)->mMWeight;
        const FluidProperties::FluidType fluidType = compounds.getCompound(type)->mFluidType;
        if (FluidProperties::NO_FLUID != fluidType) {
            DefinedFluidProperties properties;
            const double expected = properties.getProperties(fluidType)->getMWeight();
            CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, returned, mTolerance);
        }
    }

    std::cout << "... Pass" << std::endl;
}

