#ifndef CombustCH4_EXISTS
#define CombustCH4_EXISTS

/**
@file
@brief    Methane Combustion Model declarations

@defgroup  TSM_UTILITIES_PROPERTIES_COMBUSTCH4 Methane Chemical Combsution Model
@ingroup   TSM_UTILITIES_PROPERTIES

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the class for modeling Chemical Combustion of Methane and Oxygen, with option for
   Helium purge gas.)

REQUIREMENTS:
- ()

REFERENCE:
- ((Implementation of the Interactive Program STANJAM, W. C. Reynolds, 1986)
   (Methane/Oxygen/Inert Gas Flammability Diagram))

ASSUMPTIONS AND LIMITATIONS:
- ((The only inlet fluids are Methane, Oxygen, and Helium)
   (No combustion will occur if the fuel to oxidizer ratio is outside the flammability limits.)
   (Possible dissociation products are OH, CO, O, H2, and H.)
   (Helium is inert, and does not contribute to the reaction. The presence of Helium does contribute
    to final temperature, ratio of specific heats, and molecular weight))

 LIBRARY DEPENDENCY:
- ((CombustCH4.o))

 PROGRAMMERS:
- ((Christopher Brownschidle) (CACI) (Initial) (2016-07))

@{
 */
#include "Combust.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
///@brief       Methane Combustion Model
///
///@details     This model calculates the products of high temperature Methane + Oxygen combustion
///             reactions. It receives as inputs temperature, pressure, and relative concentrations
///             of each propellant compound, and outputs the post-combustion temperature, molecular
///             weight, and adiabatic index (ratio of specific heats).
///
///             This model assumes that the reaction products are O2, CH4, H2O, CO2, OH, CO, O, H2
///             and H. Any other products are assumed to be in small enogh quantity to be rejected.
///
///             This model assumes flammability limits according to the Methane/Oxygen/Inert Gas
///             Flammability Diagram. These are limits on the fuel to oxidizer to inert gas ratio that
///             allow combustion to occur. These limits state that combustion will not occur if the
///             mixture is more than 95% O2, or if the mixture's fuel concentration is more than
///             2.5*%O2 - 39.28%. When outside these limits, the program does not initiate combustion,
///             and the exit conditions are kept identical to the inlet conditions.
///
///             This model cannot accurately solve for mixtures that have an oxidizer to fuel mass
///             ratio between 3.31 and 3.99. If the oxidizer ratio is within these limits, the
///             program uses linear interpolation between the nearest stable points to calculate the
///             solution.
///
///             This model inherits from its base class Combust, and is specific to the reaction
///             identified above.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CombustCH4: public Combust
{
    public:
        TS_MAKE_SIM_COMPATIBLE(CombustCH4);
        /// @brief Default constructs this Methane combustion model
        CombustCH4();
        /// @brief Default destructs this Methane combustion model
        virtual ~CombustCH4();

    protected:
        enum Compound {
            O2          = 0,    ///< Oxygen
            CH4         = 1,    ///< Methane
            H2O         = 2,    ///< Water
            CO2         = 3,    ///< Carbon Dioxide
            OH          = 4,    ///< Hydroxyl
            CO          = 5,    ///< Carbon Monoxide
            O           = 6,    ///< Singular Oxygen
            H2          = 7,    ///< Hydrogen
            H           = 8,    ///< Singular Hydrogen
            He          = 9,    ///< Helium
            NCompounds  = 10,   ///< Number of compounds. Keep this last
        };
        double                 mDampCoeff;              /**< (--)     trick_chkpnt_io(**)     Dampening coefficient, used to ensure convergence of updateCompound() method. */
        double                 mInitialOxidizer;        /**< (mol)    trick_chkpnt_io(**)     Number of moles of oxidizer at inlet. */
        double                 mInitialFuel;            /**< (mol)    trick_chkpnt_io(**)     Number of moles of fuel at inlet. */
        double                 mEnt1;                   /**< (kJ/g/K) trick_chkpnt_io(**)     Entropy of the first solution used in unstable interpolation. */
        double                 mEnt2;                   /**< (kJ/g/K) trick_chkpnt_io(**)     Entropy of the second solution used in unstable interpolation. */
        static const int       mMaxItEquilCH4 = 10;     /**< (--)     trick_chkpnt_io(**)     Maximum iterations for solveElemPotEqns method. */
        static const int       mMaxItCombustCH4 = 400;  /**< (--)     trick_chkpnt_io(**)     Maximum iterations for solveCombustion method. */
        static const double    mMinErrorEquilCH4;       /**< (mol)    trick_chkpnt_io(**)     Minimum error for solveElemPotEqns method. */
        static const double    mTestTempStepCH4;        /**< (K)      trick_chkpnt_io(**)     Initial temperature step for solveCombustion iteration. */
        static const int       mMaxCombustLoopsCH4 = 3; /**< (--)     trick_chkpnt_io(**)     Number of times solveCombust will refine its temperature step. */
        /// @brief Updates members mInitialOxidizer, mInitialFuel, and mMoles to represent current inlet conditions
        virtual void updateCompound(double*                     moleRatios,
                                    FluidProperties::FluidType* fluidTypes,
                                    int                         nFluidTypes);
        /// @brief Provides the first guess for the solveElemPotEqns() method. The first guess assumes
        ///        that no dissociation occurs.
        virtual void makeFirstGuessAtEquil();
        /// @brief This applies the reaction specific elemental potential equations to solve for the
        ///        equilibrium conditions at the specified temperature.
        virtual void solveElemPotEqns(double* ratios,
                                      double* moles,
                                      double  temp);
        /// @brief This method calculates the exponent, but protects against underflow errors
        double       calcExpSafe(double value);
        /// @brief Check is final result for molar concentrations are negative, and if so, calculate
        ///        mole fractions without satisfying conservation of atomic species.
        void         checkNegativeMoles(double* ratios,
                                        double* moles,
                                        CombustCH4::Compound compound);
        /// @brief Calculates combustion solution when input is in the unstable range
        virtual void solveUnstableCombustion() ;
        /// @brief Calculates recombination solution when input is in the unstable range
        virtual void solveUnstableRecombination();
        /// @brief Linear interpolation method
        double       interpolate(double x,
                                 double x1,
                                 double x2,
                                 double y1,
                                 double y2);

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CombustCH4(const CombustCH4&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        CombustCH4& operator =(const CombustCH4&);
};

/// @}
#endif /* CombustCH4_EXISTS */
