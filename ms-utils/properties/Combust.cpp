/*
@file
@brief Combustion Model Implementation.

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 LIBRARY DEPENDENCY:
     ((properties/FluidProperties.o)
      (properties/ChemicalCompound.o)
      (math/UnitConversion.o)
      (simulation/hs/TsHsMsg.o)
      (software/exceptions/TsInitializationException.o))
 **************************************************************************************************/

#include "Combust.hh"
#include "ChemicalCompound.hh"
#include "math/UnitConversion.hh"
#include "GenericMacros.hh"
#include "software/exceptions/TsHsException.hh"
#include "software/exceptions/TsInitializationException.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      nCompounds      (--)  Number of compounds.
/// @param[in]      testTempStep    (K)   Initial temperature step for solveCombustion iteration.
/// @param[in]      maxItEquil      (--)  Max iterations for solveEquilibrium().
/// @param[in]      maxItCombust    (--)  Max iterations for solveCombustion().
/// @param[in]      maxCombustLoops (--)  Number of times solveCombust will refine its temperature step.
/// @param[in]      minErrorEquil   (--)  Error minimum For solveEquilibrium().
///
/// @details  Default constructs this Combustion Model.
///
/// @note     Constructor is declared protected, and is only ever called by derived classes
////////////////////////////////////////////////////////////////////////////////////////////////////
Combust::Combust(int nCompounds, double testTempStep, int maxItEquil, int maxItCombust,
        int maxCombustLoops, double  minErrorEquil)
    :
    mConstantProperty(Combust::S),
    mCompoundsDefined(),
    mCompounds(),
    mNCompounds(nCompounds),
    mWarningCountEquil(0),
    mWarningCountCombust(0),
    mMaxItEquil(maxItEquil),
    mMaxItCombust(maxItCombust),
    mCombustLoops(maxCombustLoops),
    mMinErrorEquil(minErrorEquil),
    mTestTempStep(testTempStep),
    mCombustionOccurs(true),
    mUnstable(false),
    mTemp(-1.0),
    mTempGuess(0.0),
    mPress(-1.0),
    mEnth(0.0),
    mReactants(0.0),
    mRatio(),
    mMoles(),
    mGamma(-1.0),
    mCp(-1.0),
    mMW(0.0),
    mEnt(0.0)
{
    /// - Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Default constructs this combustion model.
////////////////////////////////////////////////////////////////////////////////////////////////////

Combust::~Combust()
{
    /// - Nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      tempGuess     (K)   Guess for final mixture temperature
/// @param[in]      press         (kPa) Mixture pressure
/// @param[in]      tempIn        (K)   Inlet mixture temperature.
/// @param[in]      molesIn       (--)  Pointer to array of inlet mole fractions.
/// @param[in]      fluidTypesIn  (--)  Pointer to array of GUNNS fluid types in inlet mixture.
/// @param[in]      nFluidtypes   (--)  Number of inlet fluids.
///
/// @throws   TsInitializationException
///
/// @details  Updates this combustion model with inlet conditions, then calculates outputs. Calling
///           this method will solve for the equilibrium conditions assuming constant pressure and
///           enthalpy, as well as the inputted inlet ratios.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Combust::updateCombustionMixture(double tempGuess, double press, double tempIn, double* molesIn,
                                      FluidProperties::FluidType* fluidTypesIn, int nFluidTypes){
    mCombustionOccurs = true;
    mUnstable = false;
    mTemp = tempIn;
    mPress = press * UnitConversion::PA_PER_KPA;

    /// - Reaction specific function to initialize inlet mole fractions
    updateCompound(molesIn, fluidTypesIn, nFluidTypes);
    recalculateRatios();

    /// - Check for non-physical inputs
    if(mTemp <= DBL_EPSILON or mPress <= DBL_EPSILON){
        throwError( "Initialization Error",
        "invalid input data. Non physical input values. Pressure and temperature must be greater than 0.");
    }

    /// - Calculate starting enthalpy and entropy (overrides previous mixture conditions)
    mEnth = mEnt = mMW = 0.0;
    for(int i = 0; i < mNCompounds; i++){
        mEnth += mRatio[i] * calcEnth(mCompounds[i], mTemp);
        mEnt  += mRatio[i] * calcEnt(mCompounds[i], mTemp, mPress * mRatio[i]);
        mMW  += mRatio[i] * mCompounds[i]->mMWeight;
    }
    mEnth = mEnth / mMW;
    mEnt  = mEnt / mMW;

    mConstantProperty = Combust::H;
    updateRecombinationMixture(tempGuess, press);
    mConstantProperty = Combust::S;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  tempGuess     (K)   Guess at mixture temperature
/// @param[in]  press         (kPa) Mixture pressure
///
/// @throws   TsInitializationException
///
/// @details  Calculates solution at the given pressure, using the stored variables for all other
///            inlet conditions.
///
/// @note     tempGuess should be above a certain minimum to maintain model accuracy. This limit is
///           reaction dependent.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Combust::updateRecombinationMixture(double tempGuess, double press){
    mTempGuess = tempGuess;
    mPress = press * UnitConversion::PA_PER_KPA;
    /// - Check for non-physical inputs
    if(mPress <= DBL_EPSILON){
        throwError( "Initialization Error",
 "invalid input data. Non physical input values. Pressure and temperature must be greater than 0.");
    }

    if(mCombustionOccurs){
        makeFirstGuessAtEquil();
    }

    /// - If the reaction is unstable (decided in updateCompound), calculate combustion using
    ///   unstable method
    if(mUnstable){
        switch(mConstantProperty){
        case Combust::H :
            solveUnstableCombustion();
            break;
        case Combust::S :
            solveUnstableRecombination();
            break;
        }
    }
    else{
        calculateProperties();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws   TsInitializationException
///
/// @details  Calls solveCombustion method, and calculates thermodynamic properties.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Combust::calculateProperties(){
    /// - Calculate mole fractions
    recalculateRatios();

    /// - Calculate initial property of interest
    switch(mConstantProperty){
    case Combust::H :
        mReactants = mEnth;
        break;
    case Combust::S :
        mReactants = mEnt;
        break;
    }

    /// - Call combustion function
    if(mCombustionOccurs){
        solveCombustion();
    }
    recalculateRatios();
    /// - Update all thermodynamic quantities
    mEnth = mEnt = mCp = mMW = 0;
    for(int i = 0; i < mNCompounds; i++){
        mEnth+= mRatio[i] * calcEnth(mCompounds[i], mTemp);
        mEnt += mRatio[i] * calcEnt(mCompounds[i], mTemp, mPress * mRatio[i]);
        mMW  += mRatio[i] * mCompounds[i]->mMWeight;
        mCp  += mRatio[i] * mCompounds[i]->mMWeight * calcCp(mCompounds[i], mTemp);
    }
    mEnth = mEnth / mMW;
    mEnt = mEnt / mMW;
    mCp = mCp / mMW;
    const double Cv = mCp - UnitConversion::UNIV_GAS_CONST_SI * UnitConversion::KILO_PER_UNIT;
    if(Cv > DBL_EPSILON){
        mGamma = mCp / Cv;
    }
    else{
        throwError( "Initialization Error", "error calculating Cv. Cv cannot be negative.");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      error1        (--)  Error subtype.
/// @param[in]      error2        (--)  Error cause.
///
/// @throws         TsInitializationException
///
/// @details  Throws a TsInizialitationException
////////////////////////////////////////////////////////////////////////////////////////////////////

void Combust::throwError(std::string error1, std::string error2 ){
    TsHsMsg msg(TS_HS_ERROR, TS_HS_GENERIC);
    msg << "throwing " << " " << error1 << "-" << error2 << '\n' << tsStackTrace();
    hsSendMsg(msg);
    throw TsInitializationException(error1, "no name", error2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      array         (--)  Pointer to array being summed
/// @param[in]      arraySize     (--)  Size of array
///
/// @return         double      (--)  Sum of input array.
///
/// @throws         TsInitializationException
///
/// @details  Calculate the sum of the inputted array.
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::sumArray(double* array, int arraySize) {
    double arraySum = 0.0;
    for(int i = 0; i < arraySize; i++){
        arraySum += array[i];
    }
    if(arraySum > DBL_EPSILON){
        return arraySum;
    }
    else{
        throwError("Negative sum.",
                "sumArray should only be used on non-empty arrays with only positive values.");
        return -1;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Recalculate mole ratios based on new new mole values
////////////////////////////////////////////////////////////////////////////////////////////////////

void Combust::recalculateRatios(){
    double Sum = sumArray(mMoles, mNCompounds);
    for(int i = 0; i < mNCompounds; i++){
        mRatio[i] = mMoles[i] / Sum;
        if (mRatio[i] < DBL_EPSILON) {
            mRatio[i] = 0.0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      fluidTypes    (--)  Array of fluid types in inlet mixture.
/// @param[in]      type          (--)  Fluid type we wish to know the index of.
/// @param[in]      nFluidTypes   (--)  Number of entries in array FluidTypes.
///
/// @returns        int           (--)  Index of desired fluid type.
///
/// @details  Receives an array of fluid types at the inlet, and returns the index of the desired
///           fluid type. This index is then used to determine the initial moles of that fluid.
////////////////////////////////////////////////////////////////////////////////////////////////////

int Combust::findFluidType(FluidProperties::FluidType* fluidTypes, FluidProperties::FluidType type,
                           int nFluidTypes) const{
    int index = -1;
    for(int j = 0; j < nFluidTypes; j++){
        if(fluidTypes[j] == type){
            index = j;
        }
    }
    return index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      compound      (--)  Chemical compound to look up data for.
/// @param[in]      temp          (K)   Temperature of compound
///
/// @returns        double*       (--)  Pointer to the correct array of thermodynamic coefficients.
///
/// @details  This method provides the correct array for the calcEnth, calcEnt, and calcCp methods.
////////////////////////////////////////////////////////////////////////////////////////////////////

const double* Combust::lookUpThermoCoeff(const ChemicalCompound* compound, double temp) const{
    /// - Choose correct coefficients. The nasa database provides two sets of coefficients, one for
    ///   temperatures between 300 K and 1000 K, and one for temperatures between 1000 K and 5000 K.
    ///   This program assumes that these coefficients retain most of their accuracy below 300 K or
    ///   above 5000 K.
    if(temp < 1000.0){
        return compound->mThermoCoeffLowTemp ;
    }
    else{
        return compound->mThermoCoeffHighTemp ;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      compound      (--)  Pointer to chemical compound of interest
/// @param[in]      temp          (K)   Temperature at which to evaluate thermodynamic property
///
/// @returns        double        (--)  Gibbs Free Energy
///
/// @details  Calculates Gibbs free energy using the NASA thermodynamic coefficients for the specified
///           compound.
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::calcGibbs(const ChemicalCompound* compound, double temp) const{
    /// - calculate enthalpy
    const double enth = calcEnth(compound, temp) / (temp * UnitConversion::UNIV_GAS_CONST_SI
                      * UnitConversion::KILO_PER_UNIT);
    /// - calculate entropy
    const double ent = calcEnt(compound, temp, mPress) / (UnitConversion::UNIV_GAS_CONST_SI
                     * UnitConversion::KILO_PER_UNIT);

    return enth - ent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      Compound      (--)  Pointer to chemical compound of interest
/// @param[in]      Temp          (K)   Temperature at which to evaluate thermodynamic property
///
/// @returns        double        (kJ/mol) Enthalpy
///
/// @details  Calculates enthalpy using the NASA thermodynamic coefficients for the specified
///           compound.
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::calcEnth(const ChemicalCompound* compound, double temp) const{
    const double* thermoCoeff = lookUpThermoCoeff(compound, temp);
    const double enth = thermoCoeff[0] + 0.5 * thermoCoeff[1] * temp + thermoCoeff[2] * temp * temp
                      / 3.0 + thermoCoeff[3] * temp * temp * temp / 4.0 + thermoCoeff[4] * temp *
                      temp * temp * temp / 5.0 + thermoCoeff[5] / temp;
    return enth * temp * UnitConversion::UNIV_GAS_CONST_SI * UnitConversion::KILO_PER_UNIT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      compound      (--)  Pointer to chemical compound of interest
/// @param[in]      temp          (K)   Temperature at which to evaluate thermodynamic property
/// @param[in]      partPress     (Pa)  Partial Pressure of compound
///
/// @returns        double        (kJ/mol/K) Entropy
///
/// @details  Calculates entropy using the NASA thermodynamic coefficients for the specified
///           compound.
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::calcEnt(const ChemicalCompound* compound, double temp, double partPress) const{
    const double* thermoCoeff = lookUpThermoCoeff(compound, temp);
    double ent = thermoCoeff[0] * log(temp) + thermoCoeff[1] * temp + thermoCoeff[2] * temp * temp /
               2.0 + thermoCoeff[3] * temp * temp * temp / 3.0 + thermoCoeff[4] * temp * temp * temp
               * temp / 4.0 + thermoCoeff[6];
    /// - adjust for non-standard pressures
    partPress = std::max(partPress, DBL_EPSILON);
    ent = ent - log(partPress / UnitConversion::PA_PER_ATM );
    return ent * UnitConversion::UNIV_GAS_CONST_SI * UnitConversion::KILO_PER_UNIT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      compound      (--)  Pointer to chemical compound of interest
/// @param[in]      temp          (K)   Temperature at which to evaluate thermodynamic property
///
/// @returns        double        (kJ/mol/K) Specific Heat
///
/// @details  Calculates specific heat using the NASA thermodynamic coefficients for the specified
///           compound.
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::calcCp(const ChemicalCompound* compound, double temp) const{
    const double* thermoCoeff = lookUpThermoCoeff(compound, temp);
    const double Cp = thermoCoeff[0] + thermoCoeff[1] * temp + thermoCoeff[2] * temp * temp +
                    thermoCoeff[3] * temp * temp * temp + thermoCoeff[4] * temp * temp * temp * temp;
    return Cp * UnitConversion::UNIV_GAS_CONST_SI * UnitConversion::KILO_PER_UNIT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns      double    (K)  Mixture temperature
///
/// @details  Returns mixture temperature
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::getTemp() const{
    return mTemp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns     double    (kPa)  Mixture pressure
///
/// @details  Returns mixture pressure
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::getPress() const{
    return mPress * UnitConversion::KPA_PER_PA;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns      double    (kJ/g)  Mixture enthalpy
///
/// @details  Returns mixture enthalpy
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::getEnth() const{
    return mEnth;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns      double    (kJ/g/K)  Mixture entropy
///
/// @details  Returns mixture entropy
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::getEnt() const{
    return mEnt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns     double    (--)  Mixture ratio of specific heats
///
/// @details  Returns mixture ratio of specific heats
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::getGamma() const{
    return mGamma;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns     double    (kJ/mol/K)  Mixture specific heat
///
/// @details  Returns mixture specific heat
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::getCp() const{
    return mCp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns     double    (g/mol)  Mixture molecular weight
///
/// @details  Returns mixture molecular weight
////////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::getMolecWeight() const{
    return mMW;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns     double*    (mol)  Array of mixture moles
///
///@details  Return a pointer to the mixture's moles array.
////////////////////////////////////////////////////////////////////////////////////////////////////

double* Combust::getMoles() const{
        return mMoles;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns     double*    (--)  Array of mixture mole fractions
///
///@details  Return a pointer to the mixture's mole fractions array.
////////////////////////////////////////////////////////////////////////////////////////////////////

double* Combust::getMoleFractions() const{
        return mRatio;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      temp    (K)   Temperature to calculate equilibrium at
///
/// @throws         TsInitializationException
///
/// @details  Calculates and updates the equilibrium concentrations of all product compounds at the
///           given temperature. Iterations continue until the solution converges or the maximum
///           iterations are reached.
////////////////////////////////////////////////////////////////////////////////////////////////////

void Combust::solveEquilibrium(double temp){
    /// - Create arrays to hold values until output
    double productMoles[mNCompounds];
    double productRatios[mNCompounds];
    double productRatiosHold[mNCompounds];
    double changeSum = 10.0;
    int it = 1;

    /// - Update mole fractions
    recalculateRatios();

    for(int i = 0; i < mNCompounds; i++){
        productRatios[i] = mRatio[i];
        productRatiosHold[i] = mRatio[i];
        productMoles[i]  = mMoles[i];
    }
    /// - Begin loop
    while(it <= mMaxItEquil and changeSum > mMinErrorEquil){
        changeSum = 0.0;
        /// - Call reaction specific function to perform elemental potential calculations
        solveElemPotEqns(productRatios, productMoles, temp);
        /// - Update mole ratios
        double Sum = sumArray(productMoles, mNCompounds);
        for(int i = 0; i < mNCompounds; i++){
            productRatios[i] = productMoles[i] / Sum;
        }
        it++;

        /// - Check for convergance
        for(int i = 0; i < mNCompounds; i++){
            changeSum += fabs(productRatios[i] - productRatiosHold[i]);
            productRatiosHold[i] = productRatios[i];
        }

        /// - Check for errors
        for(int i = 0; i < mNCompounds; i++){
            if(productRatios[i] < 0.0 or std::isnan(productRatios[i]) or productRatios[i] > 1.0){
                throwError( "solveEquilibrium Error",
                        "solveEquilibrium. Could not solve Elemental Potential Equations.");
            }
        }
    }

    /// - Output
    for(int i = 0; i < mNCompounds; i++){
        mRatio[i] = productRatios[i];
        mMoles[i] = productMoles[i];
    }
    if(changeSum > mMinErrorEquil){
        mWarningCountEquil++;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @throws         TsInitializationException
///
/// @details  Performs combust and recombination calculation. This algorithm first calculates the
///           enthalpy or entropy of the inlet mixture. The process is assumed complete when the
///           enthalpy/entropy of the products equals the enthalpy/entropy of the reactants.
///           Starting at the inlet temperature, the program steps through temperature at varying
///           intervals, calling the solveEquilibrium() function and calculating enthalpy/entropy at
///           every step. When the program overshoots, it will decrease its temperature step and
///           march in the opposite direction. If the program cannot reached the desired enthalpy/
///           entropy, the program will cease calculation and output the inlet conditions.
/////////////////////////////////////////////////////////////////////////////////////////////////

void Combust::solveCombustion(){
    /// - Create variable to hold enthalpy/entropy until output
    int It = 1;
    double testTempStepHold = mTestTempStep;
    double molesHold[mNCompounds];
    for(int i = 0; i < mNCompounds; i++){
        molesHold[i] = mMoles[i];
    }

    /// - Start the loop at the inlet temperature
    double testTemp = mTempGuess;
    /// - Calculate equilibrium molar values
    solveEquilibrium(testTemp);
    recalculateRatios();
    /// - Calculate enthalpy/entropy at equilibrium
    double product = calculateCombustionProduct(testTemp);
    /// - Decide which directions loop should start
    int sign;
    if(product < mReactants){
        sign = 1;
    } else {
        sign = -1;
    }

    /// - Begin loop
    for(int j = 0; j < mCombustLoops; j++){
        while(sign * (mReactants - product) > 0 and It <= mMaxItCombust){
            testTemp += sign * mTestTempStep;
            /// - Solutions should never be below 10 K. This is used as an indicator that the
            ///   calculation has failed.
            if(testTemp > 10.0){
                /// - Calculate equilibrium molar values
                solveEquilibrium(testTemp);
                for(int i = 0; i < mNCompounds; i++){
                    mRatio[i] = mMoles[i] / sumArray(mMoles, mNCompounds);
                }
                /// - Calculate enthalpy/entropy at equilibrium
                product = calculateCombustionProduct(testTemp);
            } else{
                It = mMaxItCombust;
            }
            It ++ ;
        }
        sign = -sign ;
        mTestTempStep = mTestTempStep / 10;
    }

    /// - Output
    if(testTemp > 10.0){
        mTemp = testTemp;
    }
    else{
        for(int i = 0; i < mNCompounds; i++){
            mMoles[i] = molesHold[i];
        }
    }

    /// - Check if maximum iterations were met or if the final enthalpy/entropy is not with 5% of the
    ///   initial value.
    if(It >= mMaxItCombust or fabs(product - mReactants) > mReactants / 1000){
        mWarningCountCombust++;
    }
    mTestTempStep = testTempStepHold;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  temp    (K)     Temperature of mixture.
///
/// @returns    double      (kJ/kg)     product
///
/// @details  Calculate the product of combustion (either enthalpy or entropy). Molecular weight is
///           also calculated, because it is needed to convert the entropy/enthalpy from kJ/mol to
///           kJ/g.
/////////////////////////////////////////////////////////////////////////////////////////////////

double Combust::calculateCombustionProduct(double temp){
    double product = 0.0;
    mMW = 0.0;

    switch(mConstantProperty){
    case Combust::H :
        for(int i = 0; i < mNCompounds; i++){
            product += mRatio[i] * calcEnth(mCompounds[i], temp);
            mMW += mRatio[i] * mCompounds[i]->mMWeight;
        }
        break;
    case Combust::S :
        for(int i = 0; i < mNCompounds; i++){
            product += mRatio[i] * calcEnt(mCompounds[i], temp, mPress * mRatio[i]);
            mMW += mRatio[i] * mCompounds[i]->mMWeight;
        }
        break;
    }
    return product / mMW;
}
