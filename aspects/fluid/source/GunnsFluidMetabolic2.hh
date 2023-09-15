#ifndef GunnsFluidMetabolic2_EXISTS
#define GunnsFluidMetabolic2_EXISTS

/**
@file
@brief    GUNNS Improved Human Metabolics Model declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_METABOLICS_2  GUNNS Improved Human Metabolics Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling human metabolics within a GUNNS fluid network.  This is a
   variant of GunnsFluidMetabolic, with several improvements:
   - More up-to-date baseline data for constituent metabolic rates in various crew activity states,
   - Adds more crew activity states,
   - Adds new fluid constituents: methane, hydrogen, ammonia and carbon monoxide,
   - Adds all trace compounds given in NASA/TP-2015-218570,
   - All trace compounds are optional in the network.)

REQUIREMENTS:
- (NASA/TP-2015-218570 "Life Support Baseline Values and Assumptions Document",
   M.S. Anderson, et al., March 2015)

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidMetabolic2.o))

 PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Initial) (2015-10))

@{
*/

#include "core/GunnsFluidSource.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Metabolic Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Metabolic
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMetabolic2ConfigData : public GunnsFluidSourceConfigData
{
    public:
        /// @brief    Default constructs this Metabolic configuration data with arguments.
        GunnsFluidMetabolic2ConfigData(const std::string& name  = "",
                                       GunnsNodeList*     nodes = 0);
        /// @brief    Default destructs this Metabolic configuration data.
        virtual ~GunnsFluidMetabolic2ConfigData();
        /// @brief    Copy constructs this Metabolic configuration data.
        GunnsFluidMetabolic2ConfigData(const GunnsFluidMetabolic2ConfigData& that);
        double mO2ConsumptionRate_Nominal;                              /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in nominal state. */
        double mO2ConsumptionRate_Sleep;                                /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in sleep state. */
        double mO2ConsumptionRate_Recovery0;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in recovery0 state. */
        double mO2ConsumptionRate_Recovery1;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in recovery1 state. */
        double mO2ConsumptionRate_Recovery2;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in recovery2 state. */
        double mO2ConsumptionRate_Recovery3;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in recovery3 state. */
        double mO2ConsumptionRate_Exercise0;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in exercise0 state. */
        double mO2ConsumptionRate_Exercise1;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in exercise1 state. */
        double mCO2ProductionRate_Nominal;                              /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in nominal state. */
        double mCO2ProductionRate_Sleep;                                /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in sleep state. */
        double mCO2ProductionRate_Recovery0;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in recovery0 state. */
        double mCO2ProductionRate_Recovery1;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in recovery1 state. */
        double mCO2ProductionRate_Recovery2;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in recovery2 state. */
        double mCO2ProductionRate_Recovery3;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in recovery3 state. */
        double mCO2ProductionRate_Exercise0;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in exercise0 state. */
        double mCO2ProductionRate_Exercise1;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in exercise1 state. */
        double mH2OProductionRate_Nominal;                              /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in nominal state. */
        double mH2OProductionRate_Sleep;                                /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in sleep state. */
        double mH2OProductionRate_Recovery0;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in recovery0 state. */
        double mH2OProductionRate_Recovery1;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in recovery1 state. */
        double mH2OProductionRate_Recovery2;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in recovery2 state. */
        double mH2OProductionRate_Recovery3;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in recovery3 state. */
        double mH2OProductionRate_Exercise0;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in exercise0 state. */
        double mH2OProductionRate_Exercise1;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in exercise1 state. */
        double mHeatProductionRate_Nominal;                             /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in nominal state. */
        double mHeatProductionRate_Sleep;                               /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in sleep state. */
        double mHeatProductionRate_Recovery0;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in recovery0 state. */
        double mHeatProductionRate_Recovery1;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in recovery1 state. */
        double mHeatProductionRate_Recovery2;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in recovery2 state. */
        double mHeatProductionRate_Recovery3;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in recovery3 state. */
        double mHeatProductionRate_Exercise0;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in exercise0 state. */
        double mHeatProductionRate_Exercise1;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in exercise1 state. */
        double mCH4OProductionRate;                                     /**<    (kg/s) trick_chkpnt_io(**) Methanol production rate for a crew member in all states. */
        double mC2H6OProductionRate;                                    /**<    (kg/s) trick_chkpnt_io(**) Ethanol production rate for a crew member in all states. */
        double mC4H10OProductionRate;                                   /**<    (kg/s) trick_chkpnt_io(**) 1-Butanol production rate for a crew member in all states. */
        double mCH2OProductionRate;                                     /**<    (kg/s) trick_chkpnt_io(**) Formaldehyde (Methanal) production rate for a crew member in all states. */
        double mC2H4OProductionRate;                                    /**<    (kg/s) trick_chkpnt_io(**) Acetaldehyde (Ethanal) production rate for a crew member in all states. */
        double mC6H6ProductionRate;                                     /**<    (kg/s) trick_chkpnt_io(**) Benzene production rate for a crew member in all states. */
        double mC7H8ProductionRate;                                     /**<    (kg/s) trick_chkpnt_io(**) Toluene (Methylbenzene) production rate for a crew member in all states. */
        double mC8H10ProductionRate;                                    /**<    (kg/s) trick_chkpnt_io(**) o-Xylene (Dymethylbenzenes) production rate for a crew member in all states. */
        double mCH2CL2ProductionRate;                                   /**<    (kg/s) trick_chkpnt_io(**) Dichloromethane production rate for a crew member in all states. */
        double mC3H6OProductionRate;                                    /**<    (kg/s) trick_chkpnt_io(**) Acetone (2-propanol) production rate for a crew member in all states. */
        double mNH3ProductionRate;                                      /**<    (kg/s) trick_chkpnt_io(**) Ammonia production rate for a crew member in all states. */
        double mCOProductionRate;                                       /**<    (kg/s) trick_chkpnt_io(**) Carbon monoxide production rate for a crew member in all states. */
        double mH2ProductionRate;                                       /**<    (kg/s) trick_chkpnt_io(**) Hydrogen production rate for a crew member in all states. */
        double mCH4ProductionRate;                                      /**<    (kg/s) trick_chkpnt_io(**) Methane production rate for a crew member in all states. */

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolic2ConfigData& operator =(const GunnsFluidMetabolic2ConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Metabolic Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Metabolic
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMetabolic2InputData : public GunnsFluidSourceInputData
{
    public:
        double mNNominal;   /**< (--) Number of crew members in NOMINAL state. */
        double mNSleep;     /**< (--) Number of crew members in SLEEP state. */
        double mNRecovery0; /**< (--) Number of crew members in RECOVERY_0 state. */
        double mNRecovery1; /**< (--) Number of crew members in RECOVERY_1 state. */
        double mNRecovery2; /**< (--) Number of crew members in RECOVERY_2 state. */
        double mNRecovery3; /**< (--) Number of crew members in RECOVERY_3 state. */
        double mNExercise0; /**< (--) Number of crew members in EXERCISE_0 state. */
        double mNExercise1; /**< (--) Number of crew members in EXERCISE_1 state. */
        /// @brief    Default constructs this Metabolic input data with arguments.
        GunnsFluidMetabolic2InputData(const bool   malfBlockageFlag  = false,
                                      const double malfBlockageValue = 0.0,
                                      const double flowDemand        = 0.0,
                                      const double nNominal          = 0.0,
                                      const double nSleep            = 0.0,
                                      const double nRecovery0        = 0.0,
                                      const double nRecovery1        = 0.0,
                                      const double nRecovery2        = 0.0,
                                      const double nRecovery3        = 0.0,
                                      const double nExercise0        = 0.0,
                                      const double nExercise1        = 0.0);
        /// @brief    Copy constructs this Metabolic input data.
        GunnsFluidMetabolic2InputData(const GunnsFluidMetabolic2InputData& that);
        /// @brief    Default destructs this Metabolic input data.
        ~GunnsFluidMetabolic2InputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolic2InputData& operator=(const GunnsFluidMetabolic2InputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Metabolic
///
/// @details  Class for modeling human metabolic consumption and production of atmospheric
///           constituents and heat.  This improves upon the GunnsFluidMetabolic link by adding
///           finer control over the variances of the constituents to crew state, and also models
///           human output of various trace contaminants.  This treats all trace contaminants as
///           optional and only affects those that are present in the network.  Some trace
///           contaminants, such as NH3, can either be fluid constituents or trace compounds, or
///           both, in the network.  If a compound is present in the network as both a fluid and a
///           trace compound, this model updates both types.
///
///           Fluid constituent types GUNNS_O2, GUNNS_H2O, GUNNS_CO2 are required to be in the
///           network.  All other fluid and trace compound types are optional.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMetabolic2 : public GunnsFluidSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidMetabolic2);
    public:
        /// @brief    Enumeration of the types of metabolic states.
        enum MetabolicType {
            NOMINAL      = 0, ///< Nominal (14.5 hr).
            SLEEP        = 1, ///< Sleep (8 hr).
            RECOVERY_0   = 2, ///< Recovery  0-15 min post 75% VO2max (1/4 hr).
            RECOVERY_1   = 3, ///< Recovery 15-30 min post 75% VO2max (1/4 hr).
            RECOVERY_2   = 4, ///< Recovery 30-45 min post 75% VO2max (1/4 hr).
            RECOVERY_3   = 5, ///< Recovery 45-60 min post 75% VO2max (1/4 hr).
            EXERCISE_0   = 6, ///< Exercise  0-15 min @ 75% VO2max (1/4 hr).
            EXERCISE_1   = 7, ///< Exercise 15-30 min @ 75% VO2max (1/4 hr).
            NO_METABOLIC = 8  ///< Invalid or number of metabolic states - Keep this last!
        };
        double mNCrew[GunnsFluidMetabolic2::NO_METABOLIC];              /**<    (--)                       Array of number of crew members in each metabolic state. */
        /// @brief    Default constructs this Metabolic model.
        GunnsFluidMetabolic2();
        /// @brief    Default destructs this Metabolic model.
        virtual ~GunnsFluidMetabolic2();
        /// @brief    Initializes this Metabolic model with configuration and input data.
        void initialize(const GunnsFluidMetabolic2ConfigData& configData,
                        const GunnsFluidMetabolic2InputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0,
                        const int                             port1);
        /// @brief    Updates the state of this Metabolic.
        virtual void updateState(const double dt);
        /// @brief    Updates the internal fluid of this Metabolic model.
        virtual void updateFluid(const double dt, const double flowrate);
        /// @brief    Transitions crew members between metabolic states.
        void transition(const double        number,
                        const MetabolicType fromState,
                        const MetabolicType toState);
        /// @brief    Returns a pointer to the number of crew members array.
        const double* getNCrew() const;
        /// @brief    Returns O2 consumption rate for this cycle.
        double getConsumedO2() const;
        /// @brief    Returns CO2 production rate for this cycle.
        double getProducedCO2() const;
        /// @brief    Returns H2O production rate for this cycle.
        double getProducedH2O() const;
        /// @brief    Returns heat production rate for this cycle.
        double getProducedHeat() const;
        /// @brief    Returns NH3 production rate for this cycle.
        double getProducedNH3() const;
        /// @brief    Returns CO production rate for this cycle.
        double getProducedCO() const;
        /// @brief    Returns H2 production rate for this cycle.
        double getProducedH2() const;
        /// @brief    Returns CH4 production rate for this cycle.
        double getProducedCH4() const;
    protected:
        double mO2ConsumptionRate[GunnsFluidMetabolic2::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of Oxygen consumption rate for a crew member in each metabolic state. */
        double mCO2ProductionRate[GunnsFluidMetabolic2::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of Carbon dioxide production rate for a crew member in each metabolic state. */
        double mH2OProductionRate[GunnsFluidMetabolic2::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of Water production rate for a crew member in each metabolic state. */
        double mHeatProductionRate[GunnsFluidMetabolic2::NO_METABOLIC]; /**<    (W)    trick_chkpnt_io(**) Array of Heat production rate for a crew member in each metabolic state. */
        double mCH4OProductionRate;                                     /**<    (kg/s) trick_chkpnt_io(**) Methanol production rate for a crew member in all states. */
        double mC2H6OProductionRate;                                    /**<    (kg/s) trick_chkpnt_io(**) Ethanol production rate for a crew member in all states. */
        double mC4H10OProductionRate;                                   /**<    (kg/s) trick_chkpnt_io(**) 1-Butanol production rate for a crew member in all states. */
        double mCH2OProductionRate;                                     /**<    (kg/s) trick_chkpnt_io(**) Formaldehyde (Methanal) production rate for a crew member in all states. */
        double mC2H4OProductionRate;                                    /**<    (kg/s) trick_chkpnt_io(**) Acetaldehyde (Ethanal) production rate for a crew member in all states. */
        double mC6H6ProductionRate;                                     /**<    (kg/s) trick_chkpnt_io(**) Benzene production rate for a crew member in all states. */
        double mC7H8ProductionRate;                                     /**<    (kg/s) trick_chkpnt_io(**) Toluene (Methylbenzene) production rate for a crew member in all states. */
        double mC8H10ProductionRate;                                    /**<    (kg/s) trick_chkpnt_io(**) o-Xylene (Dymethylbenzenes) production rate for a crew member in all states. */
        double mCH2CL2ProductionRate;                                   /**<    (kg/s) trick_chkpnt_io(**) Dichloromethane production rate for a crew member in all states. */
        double mC3H6OProductionRate;                                    /**<    (kg/s) trick_chkpnt_io(**) Acetone (2-propanol) production rate for a crew member in all states. */
        double mNH3ProductionRate;                                      /**<    (kg/s) trick_chkpnt_io(**) Ammonia production rate for a crew member in all states. */
        double mCOProductionRate;                                       /**<    (kg/s) trick_chkpnt_io(**) Carbon monoxide production rate for a crew member in all states. */
        double mH2ProductionRate;                                       /**<    (kg/s) trick_chkpnt_io(**) Hydrogen production rate for a crew member in all states. */
        double mCH4ProductionRate;                                      /**<    (kg/s) trick_chkpnt_io(**) Methane production rate for a crew member in all states. */
        double mConsumedO2;                                             /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate this cycle. */
        double mProducedCO2;                                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate this cycle. */
        double mProducedH2O;                                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate this cycle. */
        double mProducedHeat;                                           /**<    (W)    trick_chkpnt_io(**) Heat production rate this cycle. */
        double mProducedCH4O;                                           /**<    (kg/s) trick_chkpnt_io(**) Methanol production rate this cycle. */
        double mProducedC2H6O;                                          /**<    (kg/s) trick_chkpnt_io(**) Ethanol production rate this cycle. */
        double mProducedC4H10O;                                         /**<    (kg/s) trick_chkpnt_io(**) 1-Butanol production rate this cycle. */
        double mProducedCH2O;                                           /**<    (kg/s) trick_chkpnt_io(**) Formaldehyde (Methanal) production rate this cycle. */
        double mProducedC2H4O;                                          /**<    (kg/s) trick_chkpnt_io(**) Acetaldehyde (Ethanal) production rate this cycle. */
        double mProducedC6H6;                                           /**<    (kg/s) trick_chkpnt_io(**) Benzene production rate this cycle. */
        double mProducedC7H8;                                           /**<    (kg/s) trick_chkpnt_io(**) Toluene (Methylbenzene) production rate this cycle. */
        double mProducedC8H10;                                          /**<    (kg/s) trick_chkpnt_io(**) o-Xylene (Dymethylbenzenes) production rate this cycle. */
        double mProducedCH2CL2;                                         /**<    (kg/s) trick_chkpnt_io(**) Dichloromethane production rate this cycle. */
        double mProducedC3H6O;                                          /**<    (kg/s) trick_chkpnt_io(**) Acetone (2-propanol) production rate this cycle. */
        double mProducedNH3;                                            /**<    (kg/s) trick_chkpnt_io(**) Ammonia production rate this cycle. */
        double mProducedCO;                                             /**<    (kg/s) trick_chkpnt_io(**) Carbon monoxide production rate this cycle. */
        double mProducedH2;                                             /**<    (kg/s) trick_chkpnt_io(**) Hydrogen production rate this cycle. */
        double mProducedCH4;                                            /**<    (kg/s) trick_chkpnt_io(**) Methane production rate this cycle. */
        int    mO2;                                                     /**< *o (--)   trick_chkpnt_io(**) Index of Oxygen constituent in fluid atmosphere. */
        int    mCO2;                                                    /**< *o (--)   trick_chkpnt_io(**) Index of Carbon dioxide constituent in fluid atmosphere. */
        int    mH2O;                                                    /**< *o (--)   trick_chkpnt_io(**) Index of Water constituent in fluid atmosphere. */
        int    mNH3;                                                    /**< *o (--)   trick_chkpnt_io(**) Index of Ammonia constituent in fluid atmosphere. */
        int    mCO;                                                     /**< *o (--)   trick_chkpnt_io(**) Index of Carbon monoxide constituent in fluid atmosphere. */
        int    mH2;                                                     /**< *o (--)   trick_chkpnt_io(**) Index of Hydrogen constituent in fluid atmosphere. */
        int    mCH4;                                                    /**< *o (--)   trick_chkpnt_io(**) Index of Methane constituent in fluid atmosphere. */
        int    mTcCH4O;                                                 /**< *o (--)   trick_chkpnt_io(**) Index of Methanol in trace compounds. */
        int    mTcC2H6O;                                                /**< *o (--)   trick_chkpnt_io(**) Index of Ethanol in trace compounds. */
        int    mTcC4H10O;                                               /**< *o (--)   trick_chkpnt_io(**) Index of 1-Butanol in trace compounds. */
        int    mTcCH2O;                                                 /**< *o (--)   trick_chkpnt_io(**) Index of Formaldehyde (Methanal) in trace compounds. */
        int    mTcC2H4O;                                                /**< *o (--)   trick_chkpnt_io(**) Index of Acetaldehyde (Ethanal) in trace compounds. */
        int    mTcC6H6;                                                 /**< *o (--)   trick_chkpnt_io(**) Index of Benzene in trace compounds. */
        int    mTcC7H8;                                                 /**< *o (--)   trick_chkpnt_io(**) Index of Toluene (Methylbenzene) in trace compounds. */
        int    mTcC8H10;                                                /**< *o (--)   trick_chkpnt_io(**) Index of o-Xylene (Dymethylbenzenes) in trace compounds. */
        int    mTcCH2CL2;                                               /**< *o (--)   trick_chkpnt_io(**) Index of Dichloromethane in trace compounds. */
        int    mTcC3H6O;                                                /**< *o (--)   trick_chkpnt_io(**) Index of Acetone (2-propanol) in trace compounds. */
        int    mTcNH3;                                                  /**< *o (--)   trick_chkpnt_io(**) Index of Ammonia in trace compounds. */
        int    mTcCO;                                                   /**< *o (--)   trick_chkpnt_io(**) Index of Carbon monoxide in trace compounds. */
        int    mTcH2;                                                   /**< *o (--)   trick_chkpnt_io(**) Index of Hydrogen in trace compounds. */
        int    mTcCH4;                                                  /**< *o (--)   trick_chkpnt_io(**) Index of Methane in trace compounds. */
        bool   mShouldApplyBias;                                        /**<    (--)   trick_chkpnt_io(**) Flag to apply biases onto consumption/production calculations when true. */
        double mO2ConsumptionBias;                                      /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate bias, set externally. */
        double mCO2ProductionBias;                                      /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate bias, set externally. */
        double mH2OProductionBias;                                      /**<    (kg/s) trick_chkpnt_io(**) Water production rate bias, set externally. */
        double mHeatProductionBias;                                     /**<    (W)    trick_chkpnt_io(**) Heat production rate bias, set externally. */
        /// @brief    Validates the initialization inputs of this Metabolic model.
        void   validate(const GunnsFluidMetabolic2InputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Initializes the derived attributes of this Metabolic model.
        void   derive(const GunnsFluidMetabolic2ConfigData& configData,
                      const GunnsFluidMetabolic2InputData&  inputData);
        /// @brief    Looks up the index of the given fluid type in the network.
        int    findFluidIndex(const FluidProperties::FluidType type);
        /// @brief    Looks up the index of the given trace compound type in the network.
        int    findTraceCompoundIndex(const ChemicalCompound::Type type);
        /// @brief    Returns the total rate of a fluid/compound produced by all crew.
        double computeProductionRate(const int indexFluid, const int indexTc,
                                     const double crew, const double rate) const;
        /// @brief    Zeroes individual fluid constituent rates.
        void   resetRates();
        /// @brief    Prepares the trace compounds contents for flow into the node.
        void   updateTraceCompounds();
        /// @brief    Updates the mass of a trace compound in the trace compounds object.
        void   setTraceMass(GunnsFluidTraceCompounds* tc, const int index, const double rate);
        /// @brief    Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolic2(const GunnsFluidMetabolic2&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolic2& operator=(const GunnsFluidMetabolic2&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The O2 consumption rate.
///
/// @details  Getter method returns the number of crew array
////////////////////////////////////////////////////////////////////////////////////////////
inline const double* GunnsFluidMetabolic2::getNCrew() const
{
    return mNCrew;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The O2 consumption rate.
///
/// @details  Getter method returns O2 consumption rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::getConsumedO2() const
{
    return mConsumedO2;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The CO2 production rate.
///
/// @details  Getter method returns CO2 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::getProducedCO2() const
{
    return mProducedCO2;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The H2O production rate.
///
/// @details  Getter method returns H2O production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::getProducedH2O() const
{
    return mProducedH2O;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) The heat production rate.
///
/// @details  Getter method returns heat production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::getProducedHeat() const
{
    return mProducedHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The NH3 production rate.
///
/// @details  Getter method returns NH3 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::getProducedNH3() const
{
    return mProducedNH3;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The CO production rate.
///
/// @details  Getter method returns CO production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::getProducedCO() const
{
    return mProducedCO;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The H2 production rate.
///
/// @details  Getter method returns H2 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::getProducedH2() const
{
    return mProducedH2;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The CH4 production rate.
///
/// @details  Getter method returns CH4 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::getProducedCH4() const
{
    return mProducedCH4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes individual fluid constituent rates.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidMetabolic2::resetRates()
{
    mConsumedO2     = 0.0;
    mProducedCO2    = 0.0;
    mProducedH2O    = 0.0;
    mProducedHeat   = 0.0;
    mProducedNH3    = 0.0;
    mProducedCO     = 0.0;
    mProducedH2     = 0.0;
    mProducedCH4    = 0.0;
    mProducedCH4O   = 0.0;
    mProducedC2H6O  = 0.0;
    mProducedC4H10O = 0.0;
    mProducedCH2O   = 0.0;
    mProducedC2H4O  = 0.0;
    mProducedC6H6   = 0.0;
    mProducedC7H8   = 0.0;
    mProducedC8H10  = 0.0;
    mProducedCH2CL2 = 0.0;
    mProducedC3H6O  = 0.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  indexFluid  (--)    Index of the compound in the network fluids.
/// @param[in]  indexTc     (--)    Index of the compound in the network trace compounds.
/// @param[in]  crew        (--)    Number of crew to apply the given rate to.
/// @param[in]  rate        (kg/s)  Compound production rate per crew.
///
/// @returns  double  (kg/s)  Total compound production rate from all crew.
///
/// @details  Checks if the given fluid or trace compound is in the network, and if so, returns the
///           the product of the given number of crew and the compound's rate per crew, otherwise
///           returns zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic2::computeProductionRate(const int indexFluid, const int indexTc,
                                                          const double crew, const double rate) const
{
    double result = 0.0;
    if (indexFluid > -1 or indexTc > -1) {
        result = crew * rate;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  tc     (--)    Pointer to the internal fluid's trace compounds object.
/// @param[in]  index  (--)    Index of the compound in the trace compounds object.
/// @param[in]  rate   (kg/s)  Produced flow rate of the compound.
///
/// @details  Sets the mass flow rate of the given trace compound index into the given trace
///           compound object.
///
/// @note     The tc pointer must not be NULL, as this method doesn't check it.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidMetabolic2::setTraceMass(GunnsFluidTraceCompounds* tc, const int index,
                                               const double rate)
{
    if (index > -1) {
        tc->setMass(index, rate);
    }
}

#endif
