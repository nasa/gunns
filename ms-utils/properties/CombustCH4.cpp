/**
@file
@brief  Methane Combustion Model implementation

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

LIBRARY DEPENDENCY:
   ((properties/Combust.o)
    (properties/ChemicalCompound.o)
    (simulation/hs/TsHsMsg.o)
    (software/exceptions/TsInitializationException.o))
 **************************************************************************************************/

#include "CombustCH4.hh"
#include "ChemicalCompound.hh"
#include "GenericMacros.hh"
#include "software/exceptions/TsInitializationException.hh"

#include <cmath>

/// @brief Minimum error for solveElemPotEqns method.
const double CombustCH4::mMinErrorEquilCH4 = 5.0E-2;
/// @brief Initial temperature step for solveCombustion iteration.
const double CombustCH4::mTestTempStepCH4 = 100.0;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Methane Combustion Model. Initializes the model. Creates global
///           members for this simulation. Memory is allocated for the arrays mRatio, mMoles, and
///           mCompounds. The ten compounds that take place in this reaction are also created.
////////////////////////////////////////////////////////////////////////////////////////////////////

CombustCH4::CombustCH4()
:
Combust(CombustCH4::NCompounds, mTestTempStepCH4, mMaxItEquilCH4, mMaxItCombustCH4,
        mMaxCombustLoopsCH4, mMinErrorEquilCH4),
mDampCoeff(0.25),
mInitialOxidizer(0.0),
mInitialFuel(0.0),
mEnt1(0.0),
mEnt2(0.0)
{
    /// - Allocate memory
    mRatio     = new double[CombustCH4::NCompounds];
    mMoles     = new double[CombustCH4::NCompounds];
    mCompounds = const_cast<const ChemicalCompound**>(new ChemicalCompound* [CombustCH4::NCompounds]);

    /// - Set each compound in the array mCompounds to its correct compound type
    mCompounds[CombustCH4::O2]  = mCompoundsDefined.getCompound(ChemicalCompound::O2);
    mCompounds[CombustCH4::CH4] = mCompoundsDefined.getCompound(ChemicalCompound::CH4);
    mCompounds[CombustCH4::H2O] = mCompoundsDefined.getCompound(ChemicalCompound::H2O);
    mCompounds[CombustCH4::CO2] = mCompoundsDefined.getCompound(ChemicalCompound::CO2);
    mCompounds[CombustCH4::OH]  = mCompoundsDefined.getCompound(ChemicalCompound::OH);
    mCompounds[CombustCH4::CO]  = mCompoundsDefined.getCompound(ChemicalCompound::CO);
    mCompounds[CombustCH4::O]   = mCompoundsDefined.getCompound(ChemicalCompound::O);
    mCompounds[CombustCH4::H2]  = mCompoundsDefined.getCompound(ChemicalCompound::H2);
    mCompounds[CombustCH4::H]   = mCompoundsDefined.getCompound(ChemicalCompound::H);
    mCompounds[CombustCH4::He]  = mCompoundsDefined.getCompound(ChemicalCompound::He);


}

#define O2 CombustCH4::O2
#define CH4 CombustCH4::CH4
#define H2O CombustCH4::H2O
#define CO2 CombustCH4::CO2
#define OH CombustCH4::OH
#define CO CombustCH4::CO
#define O CombustCH4::O
#define H2 CombustCH4::H2
#define H CombustCH4::H
#define HE CombustCH4::He

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this Methane combustion model. Frees memory allocated for arrays.
////////////////////////////////////////////////////////////////////////////////////////////////////

CombustCH4::~CombustCH4(){
    delete[] mCompounds;
    delete[] mMoles;
    delete[] mRatio;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      Value       (--)  value to take exp() of
///
/// @return         double      (--)  exp(value).
///
/// @details  Safely calculates exp(value). If value is low enough to cause an underflow error, this
///           method will return exp(-8) , preventing run time errors.
////////////////////////////////////////////////////////////////////////////////////////////////////

double CombustCH4::calcExpSafe(double value){
    if(value <= -8){
        return 0.0;
    }
    else{
        return exp(value);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      MoleRatios    (--)  Array of mole fractions for inlet fluids.
/// @param[in]      FluidTypes    (--)  Array of fluid types that describes inlet mixture.
/// @param[in]      NFluidTypes   (--)  Number of entries in array FluidTypes.
///
/// @throws   TsInitializationException
///
/// @details  Sets the initial mixture mole quantities to the inlet condition. This method also
///           checks if the inlet quantities are outside of this reaction's explosive limit, and
///           halts combustion if this criteria is not reached. If the inlet conditions are within
///           unstable limits, this method will call the unstable method, and halt normal
///           combustion/recombination.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombustCH4::updateCompound(double* moleRatios, FluidProperties::FluidType* fluidTypes,
                                int nFluidTypes){
    for(int i = 0; i < nFluidTypes; i++){
        if( moleRatios[i] < -DBL_EPSILON or moleRatios[i] > 1.0 + DBL_EPSILON){
            throwError( "Initialization Error",
            "invalid input data. Mole fractions must be non-negative and less than 1.");
        }
    }

    /// - Reset arrays to zero.
    for(int i = 0; i < mNCompounds; i++){
        mMoles[i] = 0.0;
    }

    /// - Set initial oxidizer and fuel, based on inlet conditions
    int OxInt = findFluidType(fluidTypes, FluidProperties::GUNNS_O2, nFluidTypes);
    int MethInt = findFluidType(fluidTypes, FluidProperties::GUNNS_CH4, nFluidTypes);
    if(-1 == OxInt or -1 == MethInt){
        throwError( "Initialization Error",
                "inlet initialization error. Fluid index must contain O2 and CH4.");
    }
    mInitialOxidizer = moleRatios[OxInt];
    mInitialFuel = moleRatios[MethInt];

    /// - If there is Helium present at the inlet, it is added to the model.
    if(findFluidType(fluidTypes, FluidProperties::GUNNS_HE, nFluidTypes) != -1){
        mMoles[He] = moleRatios[findFluidType(fluidTypes, FluidProperties::GUNNS_HE, nFluidTypes)];
    }

    /// - If there is no fuel and/or oxidizer, stop combustion.
    if(mInitialOxidizer == 0.0 or mInitialFuel == 0.0){
        mCombustionOccurs = false;
    }

    /// - Check that the inlet fuel to oxidizer ratio is within the explosive limit.
    ///   If not, no combustion occurs and the mixture exits the combustor at the same state as the
    ///   inlet. See header description for explanation of explosive limits.
    if(mInitialFuel < 0.05 or mInitialFuel > 2.5 * mInitialOxidizer - 0.3928){
        mCombustionOccurs = false;
    }

    /// - Check if the mixture is within the unstable region.
    if(mInitialOxidizer / mInitialFuel > 1.65957 and mInitialOxidizer / mInitialFuel < 2.0){
        mUnstable = true;
    }

    mMoles[O2] = mInitialOxidizer;
    mMoles[CH4] = mInitialFuel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Gives an initial guess for the solveEquilibrium() method. From that first guess,
///           solveEquilibrium() iterates until the correct result is met. For each subsequent call
///           to solveEquilibrium(), the previous equilibrium concentration are used as a first guess.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombustCH4::makeFirstGuessAtEquil(){
    double Sum = sumArray(mMoles, mNCompounds);

    for(int i = 0; i < mNCompounds - 1; i++){
        mMoles[i] = Sum / (mNCompounds - 1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]          ratios      (--)  Pointer to array of mole fractions.
/// @param[in]          moles       (--)  Pointer to array of moles.
/// @param[in]          compound    (--)  Compound type
///
/// @details  If the molar amount for a compound is negative after applying the conservation of atomic
///           species constraint, this method will set the the amount to what was calculated using
///           the elemental potential equations. This prevents numerical exceptions.
///
/// @note     If this method is called on the last pass of the solveElemPotEqns method, an incorrect
///           solution will result.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombustCH4::checkNegativeMoles(double* ratios, double* moles, CombustCH4::Compound compound){
    if(moles[compound] <= DBL_EPSILON){
        moles[compound] = (1e-8 + ratios[compound]) * sumArray(ratios, mNCompounds);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in,out]      productRatios   (--)  Array of Mole Fractions from current solveEquilibrium() iteration
/// @param[in,out]      productMoles    (--)  Array of Moles from current solveEquilibrium() iteration
/// @param[in]          temp            (K)   Temperature to calculate equilibrium at
/// @param[in]          it              (--)  Current solveEquilibrium() iteration
///
/// @details  Uses the Methane-Oxygen combustion elemental potential equations to calculate the
///           equilibrium concentrations. This method calculates each elemental Lagrange constraint
///           based on the concentrations of O2, CO2, and H2O. These constraints are then used to
///           calculate the mole fractions of all other compounds. The conservation of elemental
///           composition equations are then used to recalculate the concentrations of O2, CO2, and
///           H2O. This method can easily diverge and generate non-physical results, therefore a
///           dampened iterative method is used to reach the converged solution.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombustCH4::solveElemPotEqns(double* ratios, double* moles, double temp){
    double lagrangeO;
    double lagrangeC;
    double lagrangeH;
    double ratioHold[mNCompounds - 1];

    /// - Calculate Lagrange multipliers
        /// - Fuel rich case
    if(2.0 * mInitialFuel > mInitialOxidizer){
        lagrangeH = 0.5 * (log(ratios[H2]) + calcGibbs(mCompounds[H2], temp));
        lagrangeO = log(ratios[H2O]) - 2.0 * lagrangeH + calcGibbs(mCompounds[H2O], temp) ;
        lagrangeC = log(ratios[CO]) - 1.0 * lagrangeO + calcGibbs(mCompounds[CO], temp);
    }
         /// - Fuel lean case
    else{
        lagrangeO = 0.5 * (log(ratios[O2]) + calcGibbs(mCompounds[O2], temp));
        lagrangeC = log(ratios[CO2]) - 2.0 * lagrangeO + calcGibbs(mCompounds[CO2], temp);
        lagrangeH = 0.5*(log(ratios[H2O]) - lagrangeO + calcGibbs(mCompounds[H2O], temp));
    }
    /// - Calculate mole fractions of dissociation products using elemental potential equations
    ratioHold[O2] = calcExpSafe(2.0 * lagrangeO - calcGibbs(mCompounds[O2], temp));
    ratioHold[CH4] = calcExpSafe(lagrangeC + 4.0 * lagrangeH - calcGibbs(mCompounds[CH4], temp));
    ratioHold[H2O] = calcExpSafe(lagrangeO + 2.0 * lagrangeH - calcGibbs(mCompounds[H2O], temp));
    ratioHold[CO2] = calcExpSafe(lagrangeC + 2.0 * lagrangeO - calcGibbs(mCompounds[CO2], temp));
    ratioHold[OH] = calcExpSafe(lagrangeO +  lagrangeH - calcGibbs(mCompounds[OH], temp));
    ratioHold[CO] = calcExpSafe(lagrangeC + lagrangeO - calcGibbs(mCompounds[CO], temp));
    ratioHold[O] = calcExpSafe(lagrangeO - calcGibbs(mCompounds[O], temp));
    ratioHold[H2] = calcExpSafe(2.0 * lagrangeH - calcGibbs(mCompounds[H2], temp));
    ratioHold[H] = calcExpSafe(lagrangeH - calcGibbs(mCompounds[H], temp));

    /// - Use low pass filter to ensure stability
    for(int i = 0; i < mNCompounds - 1; i++){
        ratios[i] = ratios[i] + mDampCoeff * (ratioHold[i] - ratios[i]);
    }

    /// - Recalculate total number of moles, based on new mole ratios.
    if(2.0 * mInitialFuel > mInitialOxidizer){
        double Sum = sumArray(ratios, mNCompounds);
        for(int i = 0; i < mNCompounds; i++){
            ratios[i] = ratios[i] / Sum;
        }
    }
    double Sum = sumArray(moles,mNCompounds);
    for(int i = 0; i < mNCompounds; i++){
        moles[i] = ratios[i] * Sum;
    }

    /// - Apply conservation of elemental composition equations.
    /// - The product moles must all be positive. If a negative value is calculated, switch to the
    ///   result obtained using the elemental potential equations
        /// - Fuel rich case
    if(2.0 * mInitialFuel > mInitialOxidizer){
        moles[CO] = mInitialFuel - moles[CH4] - moles[CO2];
        checkNegativeMoles(ratios, moles, CO);

        moles[H2O] = 2.0 * (mInitialOxidizer - moles[CO2] - moles[O2]) - moles[CO] - moles[OH] - moles[O];
        checkNegativeMoles(ratios, moles, H2O);

        moles[H2] = 2.0 * (mInitialFuel - moles[CH4]) - moles[H2O] - 0.5 * (moles[H] + moles[OH]);
        checkNegativeMoles(ratios, moles, H2);
    }
        /// - Fuel lean case
    else{
        moles[CO2] = mInitialFuel - moles[CH4] - moles[CO];
        checkNegativeMoles(ratios, moles, CO2);

        moles[H2O] = 2.0 * (mInitialFuel - moles[CH4]) - moles[H2] - 0.5 * (moles[H] + moles[OH]);
        checkNegativeMoles(ratios, moles, H2O);

        moles[O2] = mInitialOxidizer - moles[CO2] - 0.5 * (moles[H2O] + moles[CO] + moles[OH] + moles[O]);
        checkNegativeMoles(ratios, moles, O2);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method is called when the oxidizer to fuel ratio is within the unstable range.
///           This method calculates the solution at each end of this range, then uses linear
///           interpolation to estimate the solution at the desired oxidizer ratio.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombustCH4::solveUnstableCombustion(){
    /// - Store the inlet conditions in temporary variables
    double fuel = mInitialFuel;
    double ox = mInitialOxidizer;
    double temp = mTemp;
    double helium = mMoles[He];

    /// - Calculate fuel lean solution
    mInitialFuel = 0.333 * (fuel + ox);
    /// - Store to use in interpolation function
    double fuel1 = mInitialFuel;
    mInitialOxidizer = 0.667 * (fuel + ox);
    for(int i = 0; i < mNCompounds - 1; i++){
        mMoles[i] = 0.0;
    }
    mMoles[O2] = mInitialOxidizer;
    mMoles[CH4] = mInitialFuel;

    recalculateRatios();

    mEnth = 0.0;
    mMW = 0.0;
    for(int i = 0; i < mNCompounds; i++){
        mEnth += mRatio[i] * calcEnth(mCompounds[i], mTemp);
        mMW += mRatio[i] * mCompounds[i]->mMWeight;
    }
    mEnth = mEnth / mMW;

    makeFirstGuessAtEquil();

    calculateProperties();
    /// - Store to use in interpolation function
    double temp1     = mTemp;
    double gamma1    = mGamma;
    double MW1       = mMW;
    double enth1     = mEnth;
    mEnt1            = mEnt;

    /// - Reset Test quantities
    mTestTempStep = mTestTempStepCH4;
    mMoles[He] = helium;
    mTemp = temp;
    for(int i = 0; i < mNCompounds - 1; i++){
        mMoles[i] = 0.0;
    }

    /// - Calculate fuel rich solution
    mInitialFuel = 0.376 * (fuel + ox);
    /// - Store to use in interpolation function
    double fuel2 = mInitialFuel ;
    mInitialOxidizer = 0.624 * (fuel + ox);
    mMoles[O2] = mInitialOxidizer;
    mMoles[CH4] = mInitialFuel;

    recalculateRatios();

    mEnth = 0.0;
    mMW = 0.0;
    for(int i = 0; i < mNCompounds; i++){
        mEnth += mRatio[i] * calcEnth(mCompounds[i], mTemp);
        mMW += mRatio[i] * mCompounds[i]->mMWeight;
    }
    mEnth = mEnth / mMW;
    makeFirstGuessAtEquil();

    calculateProperties();
    double temp2     = mTemp;
    double gamma2    = mGamma;
    double MW2       = mMW;
    double enth2     = mEnth;
    mEnt2            = mEnt;

    /// - Interpolate
    mTemp = interpolate(fuel, fuel1, fuel2, temp1, temp2) ;
    mMW = interpolate(fuel, fuel1, fuel2, MW1, MW2) ;
    mGamma = interpolate(fuel, fuel1, fuel2, gamma1, gamma2) ;
    mEnth = interpolate(fuel, fuel1, fuel2, enth1, enth2) ;
    mEnt = interpolate(fuel, fuel1, fuel2, mEnt1, mEnt2);
    mInitialFuel = fuel;
    mInitialOxidizer = ox;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This method is called when the oxidizer to fuel ratio is within the unstable range.
///           This method calculates the solution at each end of this range, then uses linear
///           interpolation to estimate the solution at the desired oxidizer ratio.
////////////////////////////////////////////////////////////////////////////////////////////////////

void CombustCH4::solveUnstableRecombination(){
    /// - Store the inlet conditions in temporary variables
    double fuel = mInitialFuel ;
    double ox = mInitialOxidizer ;
    double temp = mTemp ;

    /// - Calculate fuel lean solution
    mInitialFuel = 0.333 * (fuel + ox) ;
    /// - Store to use in interpolation function
    double fuel1 = mInitialFuel ;
    mInitialOxidizer = 0.667 * (fuel + ox) ;
    mEnt = mEnt1 ;

    calculateProperties() ;
    /// - Store to use in interpolation function
    double temp1     = mTemp ;
    double gamma1    = mGamma ;
    double MW1       = mMW ;
    double enth1     = mEnth ;
    double ent1      = mEnt ;

    /// - Reset Test quantities
    mTestTempStep = mTestTempStepCH4 ;
    mTemp = temp ;

    /// - Calculate rich lean solution
    mInitialFuel = 0.376 * (fuel + ox) ;
    /// - Store to use in interpolation function
    double fuel2 = mInitialFuel ;
    mInitialOxidizer = 0.624 * (fuel + ox) ;
    mEnt = mEnt2;

    calculateProperties() ;
    double temp2     = mTemp ;
    double gamma2    = mGamma ;
    double MW2       = mMW ;
    double enth2     = mEnth ;
    double ent2      = mEnt ;

    /// - Interpolate
    mTemp = interpolate(fuel, fuel1, fuel2, temp1, temp2) ;
    mMW = interpolate(fuel, fuel1, fuel2, MW1, MW2) ;
    mGamma = interpolate(fuel, fuel1, fuel2, gamma1, gamma2) ;
    mEnth = interpolate(fuel, fuel1, fuel2, enth1, enth2) ;
    mEnt = interpolate(fuel, fuel1, fuel2, ent1, ent2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      x            (--)  Independent variable of interest.
/// @param[in]      x1           (--)  Independent variable at lower edge of range.
/// @param[in]      x2           (--)  Independent variable at higher edge of range.
/// @param[in]      y1           (--)  Dependent variable at lower edge of range.
/// @param[in]      y2           (--)  Dependent variable at higher edge of range.
///
/// @return         double       (--)  Dependent variable of interest.
///
/// @details Uses linear interpolation to estimate f(X).
////////////////////////////////////////////////////////////////////////////////////////////////////

double CombustCH4::interpolate(double x, double x1, double x2, double y1, double y2){
    return y1 + (y2 - y1) * ((x - x1) / (x2 - x1)) ;
}

