#ifndef Combust_EXISTS
#define Combust_EXISTS

/**
@file
@brief    Combustion Model declarations

@defgroup  TSM_UTILITIES_PROPERTIES_COMBUST Chemical Combustion Model Base Class
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the base class for modeling Chemical Combustion, including dissociation.)

REQUIREMENTS:
- (Develop a combustion chamber model. )

REFERENCE:
- ((Implementation of the Interactive Program STANJAM, W. C. Reynolds, 1986)
   (1993 Nasa Thermo Database))

ASSUMPTIONS AND LIMITATIONS:
- ((All propellants are single phase, ideal gases upon entering the combustion Chamber.)
   (The combustion chamber experiences no heat losses during combustion.)
   (Combustion occurs at constant pressure and enthalpy, while recombination occurs at constant
    pressure and entropy.)
   (All reactions occur at constant pressure, and either constant enthalpy or entropy.)
   (Thermodynamic coefficients are used outside of their provided bounds in some cases.))

 LIBRARY DEPENDENCY:
- ((Combust.o))

 PROGRAMMERS:
- ((Christopher Brownschidle) (CACI) (Initial) (2016-06))

@{
 */
#include "properties/ChemicalCompound.hh"
#include "properties/FluidProperties.hh"
#include <cstring>

////////////////////////////////////////////////////////////////////////////////////////////////////
///@brief       Combustion Model
///
///@details     This is a model to calculate the products of high temperature combustion reactions,
///             as found in rocket combustion chambers. It receives temperature, pressure, and
///             relative concentrations of each propellant compound, and outputs the post-combustion
///             temperature, molecular weight, enthalpy, entropy, and adiabatic index (ratio of
///             specific heats). This model can also calculate the results of post-combustion
///             recombination of dissociation products.
///
///             This model uses the method of Elemental Potentials to account for high temperature
///             dissociation reactions. It also accounts for the effect of non-reacting compounds.
///
///             This model serves as a base class, and performs tasks associated with general
///             reactions. The specifics of the reaction at hand, such as compound types and
///             constraint equations, need to be provided in a separate derived class.
///
////////////////////////////////////////////////////////////////////////////////////////////////////

class Combust
{
    TS_MAKE_SIM_COMPATIBLE(Combust);
    public:
        enum Property {
            S = 0 , ///< Entropy
            H = 1   ///< Enthalpy
        };
        /// @brief Default destructs this combustion mixture
        virtual ~Combust() ;
        /// @brief Updates mixture, assuming constant enthalpy combustion process
        void updateCombustionMixture(double tempGuess,
                                     double  press,
                                     double  tempIn,
                                     double* molesIn = 0,
                                     FluidProperties::FluidType* fluidTypesIn = 0,
                                     int nFluidTypes = 0) ;
        /// @brief Updates mixture, assuming constant entropy recombination process
        void    updateRecombinationMixture(double tempGuess,
                                          double press) ;
        /// @brief Return mixture temperature.
        double  getTemp() const;
        /// @brief Return mixture pressure.
        double  getPress() const;
        /// @brief Return mixture enthalpy.
        double  getEnth() const;
        /// @brief Return mixture entropy.
        double  getEnt() const;
        /// @brief Return the reactant's ratio of specific heats.
        double  getGamma() const;
        /// @brief Return mixture specific heat
        double  getCp() const;
        /// @brief Return mixture molecular weight
        double  getMolecWeight() const;
        /// @brief Return pointer to array of mixture molar concentrations.
        double* getMoles() const;
        /// @brief Return pointer to array of mixture mole fractions.
        double* getMoleFractions() const;
        /// @brief Sets The maximum iterations for the equilibrium solver.
        void    setMaxItEquil(int It);
        /// @brief Sets The maximum iterations for the combustion solving method.
        void    setMaxItCombust(int It);
        /// @brief Sets the number of times the combustion solving method will refine it's step.
        void    setMaxCombustLoops(int Loops);
        /// @brief Sets the minimum error necessary to break out of the equilibrium solver
        void    setMinErrorEquil(double Error);

    protected:
        /// @brief Default constructs this combustion mixture
        Combust(int     nCompounds      =   0,
                double  testTempStep    =   30,
                int     maxItEquil      =   10,
                int     maxItCombust    =   1000,
                int     maxCombustLoops =   2,
                double  minErrorEquil   =   1.0E-6);

        Combust::Property   mConstantProperty;     /**< (--)       trick_chkpnt_io(**)     Enumeration to determine whether to hold mixture at constant S or H. */
        DefinedChemicalCompounds mCompoundsDefined;/**< ** (--)    trick_chkpnt_io(**)     Defined chemical compounds. */
        const ChemicalCompound**  mCompounds;      /**< ** (--)    trick_chkpnt_io(**)     Array of combustion Compounds. */
        int                 mNCompounds;           /**< (--)       trick_chkpnt_io(**)     Number of compounds in reaction. */
        int                 mWarningCountEquil;    /**< (--)       trick_chkpnt_io(**)     Counter of how many times solveEquilibrium method reached the max iterations. */
        int                 mWarningCountCombust;  /**< (--)       trick_chkpnt_io(**)     Counter of how many times solveCombustion method reached the max iterations. */
        int                 mMaxItEquil;           /**< (--)       trick_chkpnt_io(**)     Max iterations for solveEquilibrium(). */
        int                 mMaxItCombust;         /**< (--)       trick_chkpnt_io(**)     Max iterations for solveCombustion(). */
        int                 mCombustLoops;         /**< (--)       trick_chkpnt_io(**)     Number of times solveCombust will refine its temperature step. */
        double              mMinErrorEquil;        /**< (--)       trick_chkpnt_io(**)     Error minimum for solveEquilibrium(). */
        double              mTestTempStep;         /**< (K)        trick_chkpnt_io(**)     Initial temperature step for solveCombustion iteration. */
        bool                mCombustionOccurs;     /**< (--)       trick_chkpnt_io(**)     Flag to determine if combustion will occur. */
        bool                mUnstable;             /**< (--)       trick_chkpnt_io(**)     Flag to determine if the program must solve an unstable solution. */
        double              mTemp;                 /**< (K)        trick_chkpnt_io(**)     Mixture temperature. */
        double              mTempGuess;            /**< (K)        trick_chkpnt_io(**)     Guess for final temperature (usually the last pass result). */
        double              mPress;                /**< (Pa)       trick_chkpnt_io(**)     Mixture pressure. */
        double              mEnth;                 /**< (kJ/g)     trick_chkpnt_io(**)     Mixture enthalpy. */
        double              mReactants;            /**< (kJ/g)     trick_chkpnt_io(**)     Enthalpy/entropy of reactants. */
        double*             mRatio;                /**< (--)       trick_chkpnt_io(**)     Mixture mole fraction array. */
        double*             mMoles;                /**< (mol)      trick_chkpnt_io(**)     Mixture molar concentration array. */
        double              mGamma;                /**< (--)       trick_chkpnt_io(**)     Mixture ratio of specific heats */
        double              mCp;                   /**< (kJ/mol/K) trick_chkpnt_io(**)     Mixture specific heat. */
        double              mMW;                   /**< (1/mol)    trick_chkpnt_io(**)     Mixture molecular weight. */
        double              mEnt;                  /**< (kJ/K/g)   trick_chkpnt_io(**)     Mixture entropy. */

        /// @brief Perform the combustion/recombination solving loop.
        void          solveCombustion() ;
        /// @brief Calculate the sum of an array.
        double        sumArray(double* array,
                               int     arraySize) ;
        /// @brief Calculate mole fractions based on the moles of each constituent.
        void          recalculateRatios();
        /// @brief This method searches the inputted array of FluidTypes, so that the starting moles
        ///        of each component can be matched to it's correct type
        int           findFluidType(FluidProperties::FluidType* fluidTypes,
                                    FluidProperties::FluidType  type,
                                    int                         nFluidTypes) const;
        /// @brief Reaction specific function to set inlet moles for each compound
        virtual void  updateCompound(double*                     moleRatios,
                                     FluidProperties::FluidType* fluidTypes,
                                     int                         nFluidTypes) = 0;
        /// @brief Calculate mole fractions for the first pass of solveElemPotEqns().
        virtual void  makeFirstGuessAtEquil() = 0;
        /// @brief Calculate mole fractions using elemental potential method.
        virtual void  solveElemPotEqns(double* productRatios,
                                       double* productMoles,
                                       double  temp) = 0;
        /// @brief Method to calculate equilibrium mole fractions at the given temperature
        void          solveEquilibrium(double temp) ;
        /// @brief returns pointer to array of thermodynamic coefficients for the specified compound
        const double* lookUpThermoCoeff(const ChemicalCompound* compound,
                                        double                  temp) const;
        /// @brief Calculates Gibbs free energy of specified compound at specified temperature
        double        calcGibbs(const ChemicalCompound* compound,
                                double                  temp) const;
        /// @brief Calculates enthalpy of specified compound at specified temperature
        double        calcEnth(const ChemicalCompound* compound,
                               double                  temp) const;
        /// @brief Calculates entropy of specified compound at specified temperature and partial
        ///        pressure
        double        calcEnt(const ChemicalCompound* compound,
                              double                  temp,
                              double                  partPress) const;
        /// @brief Calculates specific heat of specified compound at specified temperature
        double        calcCp(const ChemicalCompound* compound,
                             double                  temp) const;
        /// @brief Throws TsInitializationException
        void          throwError(std::string error1,
                                 std::string error2) ;
        /// @brief solves combustion and calculates thermodynamic properties
        void          calculateProperties() ;
        /// @brief Calculates the product of combustion (enthalpy or entropy)
        double        calculateCombustionProduct(double temp);
        /// @brief virtual function to solve combustion at unstable points, if they exist.
        virtual void  solveUnstableCombustion();
        /// @brief virtual function to solve recombination at unstable points, if they exist.
        virtual void  solveUnstableRecombination();

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared protected and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        Combust(const Combust&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared protected and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        Combust& operator =(const Combust&);

};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Solves combustion for unstable inlet conditions. Must be defined by derived class
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Combust::solveUnstableCombustion(){
    calculateProperties();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Solves recombination for unstable inlet conditions. Must be defined by derived class
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Combust::solveUnstableRecombination(){
    calculateProperties();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      double    (--)  Number of iterations
///
/// @details  Sets the Equilibrium Solver's max iterations
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Combust::setMaxItEquil(int it){
    mMaxItEquil = it;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      double    (--)  Number of iterations
///
/// @details  Sets the Combustion Solver's max iterations
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Combust::setMaxItCombust(int it){
    mMaxItCombust = it;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      double    (--)  Number of loops
///
/// @details  Sets the Combustion Solver's number of refinement loops
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Combust::setMaxCombustLoops(int it){
    mCombustLoops = it;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]      double    (--)  Error value
///
/// @details  Sets the Equilibrium Solver's minimum error to break out of the loop
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void Combust::setMinErrorEquil(double error){
    mMinErrorEquil = error;
}

/// @}
#endif
