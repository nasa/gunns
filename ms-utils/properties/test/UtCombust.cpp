/************************** TRICK HEADER ***********************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
   ((properties/CombustCH4.o)
    (properties/Combust.o)
    (properties/ChemicalCompound.o)
    (properties/FluidProperties.o)
    (properties/CombustFactory.o)
    (simulation/hs/TsHsMsg.o)
    (software/exceptions/TsInitializationException.o))
 ***************************************************************************************************/

#include <iostream>
#include <fstream>

#include "properties/CombustCH4.hh"
#include "properties/CombustFactory.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "strings/UtResult.hh"

#include "UtCombust.hh"

int UtCombust::TEST_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Chemical Compound Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtCombust::UtCombust()
:
CppUnit::TestFixture(),
tNStep(5),
tTempMin(54.5),
tTempMax(190.0),
tPressMin(101),
tPressMax(20684.27),
tHeMin(0.0),
tHeMax(0.8),
tCH4Min(0.0),
tCH4Max(1.0),
tTolerance(0.0),
tArticle(0),
tTempIn(0.0),
tTempGuess(0.0),
tPress(0.0),
tGamma(0.0),
tMW(0.0)
{
    /// - Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default destructs this Chemical Compound Properties model unit test.
////////////////////////////////////////////////////////////////////////////////////////////////////
UtCombust::~UtCombust()
{
    /// - Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed before each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCombust::setUp()
{
    tTempIn = 106.0;
    tTempGuess = 2000.0;
    tPress = 1207.0;

    tTolerance = 1.0e-3;
    tArticle = new FriendlyCombustCH4();

    tTypes[0] = FluidProperties::GUNNS_O2;
    tTypes[1] = FluidProperties::GUNNS_CH4;
    tTypes[2] = FluidProperties::GUNNS_HE;

    tMoles[0] = 0.6;
    tMoles[1] = 0.4;
    tMoles[2] = 0.0;

    ++TEST_ID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Executed after each unit test as part of the CPPUNIT framework.
////////////////////////////////////////////////////////////////////////////////////////////////////
void UtCombust::tearDown()
{
    delete tArticle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test that the default values are set correctly.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust::testDefault(){
    UT_RESULT_FIRST;

    CPPUNIT_ASSERT(tArticle->mTemp                  ==   -1.0);
    CPPUNIT_ASSERT(tArticle->mPress                 ==   -1.0);
    CPPUNIT_ASSERT(tArticle->mEnth                  ==    0.0);
    CPPUNIT_ASSERT(tArticle->mReactants             ==    0.0);
    CPPUNIT_ASSERT(tArticle->mGamma                 ==   -1.0);
    CPPUNIT_ASSERT(tArticle->mCp                    ==   -1.0);
    CPPUNIT_ASSERT(tArticle->mMW                    ==    0.0);
    CPPUNIT_ASSERT(tArticle->mEnt                   ==    0.0);
    CPPUNIT_ASSERT(tArticle->mWarningCountEquil     ==      0);
    CPPUNIT_ASSERT(tArticle->mWarningCountCombust   ==      0);
    CPPUNIT_ASSERT(tArticle->mMaxItEquil            ==     10);
    CPPUNIT_ASSERT(tArticle->mMaxItCombust          ==    400);
    CPPUNIT_ASSERT(tArticle->mCombustLoops          ==      3);
    CPPUNIT_ASSERT(tArticle->mMinErrorEquil         == 5.0E-2);
    CPPUNIT_ASSERT(tArticle->mTestTempStep          ==  100.0);
    CPPUNIT_ASSERT(tArticle->mCombustionOccurs               );
    CPPUNIT_ASSERT(!tArticle->mUnstable                      );
    CPPUNIT_ASSERT(tArticle->mNCompounds            ==     10);
    CPPUNIT_ASSERT(tArticle->mConstantProperty  == Combust::S);
    CPPUNIT_ASSERT(tArticle->mDampCoeff             ==   0.25);
    CPPUNIT_ASSERT(tArticle->mInitialOxidizer       ==    0.0);
    CPPUNIT_ASSERT(tArticle->mInitialFuel           ==    0.0);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This test ensures that none of the possible input conditions will result in any exceptions.
///           The temperature bounds were determined by the limits on the fluid properties class.
///           The pressure bounds were based on the expected operating conditions for this class
///           (it was determined unlikely that this class would be used for reactions at a chamber
///           pressure above 3,000 psi or below 1 atm.
///
/// @note     WARNING! This test takes a very long time. Consider commenting it out of the test suite
///           if you don't want to wait.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust::testFull(){
    UT_RESULT;

    double tempStep = (tTempMax - tTempMin) / tNStep;
    double pressStep = (tPressMax - tPressMin) / tNStep;
    double HeStep = (tHeMax - tHeMin) / tNStep;
    double CH4Step = (tCH4Max - tCH4Min) / tNStep;

    /// - Begin 4th degree loop
    for(double press = tPressMin; press <= tPressMax + 1; press += pressStep){
        for(double temp = tTempMin; temp <= tTempMax + 1.0; temp += tempStep){
            for(double He = tHeMin; He <= tHeMax + 0.01; He += HeStep){
                for(double CH4 = tCH4Min; CH4 <= tCH4Max - He + 0.01; CH4 += CH4Step){
                    tPress = press;
                    tTempIn = temp;
                    tMoles[0] = 1.0 - He - CH4; tMoles[1] = CH4; tMoles[2] = He;
                    try{
                    tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes);
                    CPPUNIT_ASSERT_DOUBLES_EQUAL((tArticle->mReactants - tArticle->mEnth)
                            / tArticle->mReactants, 0.0 , 0.15);
                    tPress = 1;
                    tTempIn = 500;
                    tArticle->updateRecombinationMixture(tTempIn, tPress);
                    }
                    catch(TsInitializationException& e){
                        std::cout << "Exception caught." << std::endl;
                        std::cout << "P = " << press << ", T = " << temp << ", He = " << He << ", CH4 = " << CH4 << std::endl;
                        CPPUNIT_ASSERT(false);
                    }
                }
                double CH4 = 0.35 * (1 - He);
                tPress = press;
                tTempIn = temp;
                tMoles[0] = 1.0 - He - CH4; tMoles[1] = CH4; tMoles[2] = He;
                try{
                    tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes);
                    tPress = 1;
                    tTempIn = 500;
                    tArticle->updateRecombinationMixture(tTempIn, tPress);
                }
                catch(TsInitializationException& e){
                    std::cout << "Exception caught." << std::endl;
                    std::cout << "P = " << press << ", T = " << temp << ", He = " << He << ", CH4 = " << CH4 << std::endl;
                }
            }
        }
    }
    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Test's the model's update method using last past mole fractions as inlet conditions.
///           This test checks inlet conditions at a variety of pressures and temperatures.
///
/// @note     WARNING! This test takes a very long time. Consider commenting it out of the test suite
///           if you don't want to wait.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust::testRecombine(){
    UT_RESULT;

    tMoles[0] = 0.666;
    tMoles[1] = 0.337;
    tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes);

    double tempStart = tTempIn;
    double tempEnd   = 1000.0;
    double tempStep  = (tempStart - tempEnd) / tNStep;

    double pressStart = tPress;
    double pressEnd   = 1.0;
    double pressStep  = (pressStart - pressEnd) / tNStep;

    for(double press = pressStart; press < pressEnd; press -= pressStep){
        for(double temp = tempStart; temp < tempEnd; temp -= tempStep){
            tTempIn = temp;
            tPress = press;
            try{
                tArticle->updateRecombinationMixture(tTempIn, tPress);
                CPPUNIT_ASSERT_DOUBLES_EQUAL((tArticle->mReactants-tArticle->mEnt)
                        / tArticle->mReactants, 0.0 , 0.05);
            }
            catch(TsInitializationException& e){
                std::cout << "Exception caught." << std::endl;
                std::cout << "P = " << press << ", T = " << temp  << std::endl;
                CPPUNIT_ASSERT(false);
            }
        }
    }
    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This test ensure that all get methods work correctly, and return the correct value.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust:: testGet(){
    UT_RESULT;

    tMoles[0] = 0.65;
    tMoles[1] = 0.35;
    tMoles[2] = 0.0;

    tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes);

    CPPUNIT_ASSERT(tArticle->mTemp == tArticle->getTemp());
    CPPUNIT_ASSERT(tArticle->mPress * UnitConversion::KPA_PER_PA == tArticle->getPress());
    CPPUNIT_ASSERT(tArticle->mEnth== tArticle->getEnth());
    CPPUNIT_ASSERT(tArticle->mEnt== tArticle->getEnt());
    CPPUNIT_ASSERT(tArticle->mGamma== tArticle->getGamma());
    CPPUNIT_ASSERT(tArticle->mCp== tArticle->getCp());
    CPPUNIT_ASSERT(tArticle->mMW== tArticle->getMolecWeight());
    CPPUNIT_ASSERT( tArticle->mMoles[1] == tArticle->getMoles()[1]);
    CPPUNIT_ASSERT( tArticle->mRatio[1] == tArticle->getMoleFractions()[1]);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This test ensure that all set methods work correctly, and return the correct value.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust:: testSet(){
    UT_RESULT;

    tArticle->setMaxItEquil(20);
    CPPUNIT_ASSERT(tArticle->mMaxItEquil == 20);

    tArticle->setMaxItCombust(20);
    CPPUNIT_ASSERT(tArticle->mMaxItCombust == 20);

    tArticle->setMaxCombustLoops(4);
    CPPUNIT_ASSERT(tArticle->mCombustLoops == 4);

    tArticle->setMinErrorEquil(4.0E-2);
    CPPUNIT_ASSERT(tArticle->mMinErrorEquil == 4.0E-2);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This test tries all possible non-physical input types, and ensures that initialization
///           errors are thrown in each case.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust::testBadInit(){
    UT_RESULT;
    /// - try negative pressure.
    tPress = -1.0;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);
    tPress = -1.0;
    CPPUNIT_ASSERT_THROW(tArticle->updateRecombinationMixture(tTempGuess, tPress),
            TsInitializationException);
    /// - try negative temperature.
    tTempIn = -100.0;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);
    /// - try oxygen mole fraction greater than 1.
    tTempIn = 100.0;
    tMoles[0] = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);
    /// - try methane mole fraction greater than 1.
    tTempIn = 100.0;
    tMoles[0] = 0.5;
    tMoles[1] = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);
    /// - try helium mole fraction greater than 1.
    tTempIn = 100.0;
    tMoles[1] = 0.5;
    tMoles[2] = 1.1;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);
    /// - try oxygen mole fraction less than 0.
    tTempIn = 100.0;
    tMoles[2] = 0.0;
    tMoles[0] = -0.5;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);
    /// - try methane mole fraction less than 0.
    tTempIn = 100.0;
    tMoles[0] = 0.5;
    tMoles[1] = -0.5;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);
    /// - try Helium mole fraction less than 0.
    tTempIn = 100.0;
    tMoles[1] = 0.25;
    tMoles[2] = -0.25;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);

    /// - try all mole fractions equal to 0.
    tTempIn = 100.0;
    tMoles[0] = 0.0;
    tMoles[1] = 0.0;
    tMoles[2] = 0.0;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This test tries updating the combustion model with fluid config data that lacks the
///           required fluid types (CH4 and O2), and ensures that initialization error are thrown.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust::testBadFluid(){
    UT_RESULT;
    /// - try updating with no O2
    tTypes[0] = FluidProperties::NO_FLUID;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);
    /// - try updating with no CH4
    tTypes[1] = FluidProperties::GUNNS_O2;
    CPPUNIT_ASSERT_THROW(tArticle->updateCombustionMixture(tTempGuess, tPress, tTempIn, tMoles, tTypes, tNFluidTypes),
            TsInitializationException);

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This test ensures that the combustion factory can correctly produce all possible model
///           types.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust::testFactory(){
    UT_RESULT;

    CombustFactory factory;
    CPPUNIT_ASSERT(0 == factory.createCombustor(CombustFactory::NONE));
    CPPUNIT_ASSERT(0 != factory.createCombustor(CombustFactory::CH4));

    UT_PASS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This test validates the model. It calculates the solution to all possible inlet conditions,
///           and generates a text file of these results. This text file can then be compared to the
///           output generated from a tool such as STANJAM or Cantera. The effect of Helium at the
///           was not included in this test, since many of the validation tools used for this model
///           cannot correctly handle the presence of Helium.
///
/// @note     WARNING! This test takes a very long time. Consider commenting it out of the test suite
///           if you don't want to wait.
////////////////////////////////////////////////////////////////////////////////////////////////////

void UtCombust::validate(){
    UT_RESULT;

    int i = 1;
    tNStep = 20;
    tCH4Min = 0.1;
    tCH4Max = 0.602;
    double guess1 = 1000;
    double guess2 = 750;
    double He = 0.0;
    std::ofstream out;
    out.open("/users/cbrownsc/Documents/out.txt");

    double tempStep = (tTempMax-tTempMin) / tNStep;
    double pressStep = (tPressMax - tPressMin) / tNStep;
    double CH4Step = (tCH4Max - tCH4Min) / tNStep;

    /// - Begin 3 degree loop
    for(double press = tPressMin; press <= tPressMax; press += pressStep){
        for(double temp = tTempMin; temp <= tTempMax + tempStep; temp += tempStep){
            for(double CH4 = tCH4Min; CH4 <= tCH4Max + 0.005; CH4 += CH4Step){
                tPress = press;
                tTempIn = temp;
                tMoles[1] = (1 - He) * CH4; tMoles[0] = 1.0  - tMoles[1] - He; tMoles[2] = He;
                try{
                    tArticle->updateCombustionMixture(guess1, tPress, tTempIn, tMoles, tTypes, tNFluidTypes);
                    guess1 = tArticle->mTemp;
                    tPress = 14;
                    tArticle->updateRecombinationMixture(guess2, tPress);
                    if(tArticle->mTemp <= 750.0){
                        guess2 = 750;
                    }else{
                        guess2 = tArticle->mTemp;
                    }
                    i++;
                }
                catch(TsInitializationException& e){
                    std::cout << "Exception caught." << std::endl;
                    std::cout << "P = " << press << ", T = " << temp << ", CH4 = " << CH4 <<  ", T_g = " << guess1 << ", T_g2 = " << guess2 << std::endl;
                }
                out << tArticle->mEnth<< std::endl;
            }
        }
    }
    out.close();
    UT_PASS;
}
