#ifndef GunnsFluidMetabolic3_EXISTS
#define GunnsFluidMetabolic3_EXISTS

/**
@file
@brief    GUNNS Improved Human Metabolics Model declarations (Updated BVAD)

@defgroup  TSM_GUNNS_FLUID_SOURCE_METABOLICS_3  GUNNS Improved Human Metabolics Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
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
- (NASA/TP-2015-218570/REV2 "Life Support Baseline Values and Assumptions Document",
   Michael K. Ewert, et al., February 2022)

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidMetabolic3.o))

 PROGRAMMERS:
- (
    (Jason Harvey) (L-3 Communications) (Initial) (2015-10)
    (Kyle Fondon) (Axiom Space) (Update) (2023-08)
  )

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
class GunnsFluidMetabolic3ConfigData : public GunnsFluidSourceConfigData
{
    public:
        /// @brief    Default constructs this Metabolic configuration data with arguments.
        GunnsFluidMetabolic3ConfigData(const std::string& name  = "",
                                       GunnsNodeList*     nodes = 0);
        /// @brief    Default destructs this Metabolic configuration data.
        virtual ~GunnsFluidMetabolic3ConfigData();
        /// @brief    Copy constructs this Metabolic configuration data.
        GunnsFluidMetabolic3ConfigData(const GunnsFluidMetabolic3ConfigData& that);
        double mO2ConsumptionRate_Nominal;                              /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in nominal state. */
        double mO2ConsumptionRate_Sleep;                                /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in sleep state. */
        double mO2ConsumptionRate_Recovery0;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in recovery0 state. */
        double mO2ConsumptionRate_Recovery1;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in recovery1 state. */
        double mO2ConsumptionRate_Recovery2;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in recovery2 state. */
        double mO2ConsumptionRate_Recovery3;                            /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in recovery3 state. */
        double mO2ConsumptionRate_Aerobic0;                             /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in aerobic0 state. */
        double mO2ConsumptionRate_Aerobic1;                             /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in aerobic1 state. */
        double mO2ConsumptionRate_Resistive0;                           /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in resistive0 state. */
        double mO2ConsumptionRate_Resistive1;                           /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in resistive1 state. */
        double mO2ConsumptionRate_Resistive2;                           /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in resistive2 state. */
        double mO2ConsumptionRate_Resistive3;                           /**<    (kg/s) trick_chkpnt_io(**) Oxygen consumption rate for a crew member in resistive3 state. */
        double mCO2ProductionRate_Nominal;                              /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in nominal state. */
        double mCO2ProductionRate_Sleep;                                /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in sleep state. */
        double mCO2ProductionRate_Recovery0;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in recovery0 state. */
        double mCO2ProductionRate_Recovery1;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in recovery1 state. */
        double mCO2ProductionRate_Recovery2;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in recovery2 state. */
        double mCO2ProductionRate_Recovery3;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in recovery3 state. */
        double mCO2ProductionRate_Aerobic0;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in aerobic0 state. */
        double mCO2ProductionRate_Aerobic1;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in aerobic1 state. */
        double mCO2ProductionRate_Resistive0;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in resistive0 state. */
        double mCO2ProductionRate_Resistive1;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in resistive1 state. */
        double mCO2ProductionRate_Resistive2;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in resistive2 state. */
        double mCO2ProductionRate_Resistive3;                            /**<    (kg/s) trick_chkpnt_io(**) Carbon dioxide production rate for a crew member in resistive3 state. */
        double mH2OProductionRate_Nominal;                              /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in nominal state. */
        double mH2OProductionRate_Sleep;                                /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in sleep state. */
        double mH2OProductionRate_Recovery0;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in recovery0 state. */
        double mH2OProductionRate_Recovery1;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in recovery1 state. */
        double mH2OProductionRate_Recovery2;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in recovery2 state. */
        double mH2OProductionRate_Recovery3;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in recovery3 state. */
        double mH2OProductionRate_Aerobic0;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in aerobic0 state. */
        double mH2OProductionRate_Aerobic1;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in aerobic1 state. */
        double mH2OProductionRate_Resistive0;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in resistive0 state. */
        double mH2OProductionRate_Resistive1;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in resistive1 state. */
        double mH2OProductionRate_Resistive2;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in resistive2 state. */
        double mH2OProductionRate_Resistive3;                            /**<    (kg/s) trick_chkpnt_io(**) Water production rate for a crew member in resistive3 state. */
        double mHeatProductionRate_Nominal;                             /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in nominal state. */
        double mHeatProductionRate_Sleep;                               /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in sleep state. */
        double mHeatProductionRate_Recovery0;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in recovery0 state. */
        double mHeatProductionRate_Recovery1;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in recovery1 state. */
        double mHeatProductionRate_Recovery2;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in recovery2 state. */
        double mHeatProductionRate_Recovery3;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in recovery3 state. */
        double mHeatProductionRate_Aerobic0;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in aerobic0 state. */
        double mHeatProductionRate_Aerobic1;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in aerobic1 state. */
        double mHeatProductionRate_Resistive0;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in resistive0 state. */
        double mHeatProductionRate_Resistive1;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in resistive1 state. */
        double mHeatProductionRate_Resistive2;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in resistive2 state. */
        double mHeatProductionRate_Resistive3;                           /**<    (W)    trick_chkpnt_io(**) Heat production rate for a crew member in resistive3 state. */
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
        GunnsFluidMetabolic3ConfigData& operator =(const GunnsFluidMetabolic3ConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Metabolic Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Metabolic
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMetabolic3InputData : public GunnsFluidSourceInputData
{
    public:
        double mNNominal;    /**< (--) Number of crew members in NOMINAL state. */
        double mNSleep;      /**< (--) Number of crew members in SLEEP state. */
        double mNRecovery0;  /**< (--) Number of crew members in RECOVERY_0 state. */
        double mNRecovery1;  /**< (--) Number of crew members in RECOVERY_1 state. */
        double mNRecovery2;  /**< (--) Number of crew members in RECOVERY_2 state. */
        double mNRecovery3;  /**< (--) Number of crew members in RECOVERY_3 state. */
        double mNAerobic0;   /**< (--) Number of crew members in AEROBIC_0 state. */
        double mNAerobic1;   /**< (--) Number of crew members in AEROBIC_1 state. */
        double mNResistive0; /**< (--) Number of crew members in RESISTIVE_0 state. */
        double mNResistive1; /**< (--) Number of crew members in RESISTIVE_1 state. */
        double mNResistive2; /**< (--) Number of crew members in RESISTIVE_2 state. */
        double mNResistive3; /**< (--) Number of crew members in RESISTIVE_3 state. */
        /// @brief    Default constructs this Metabolic input data with arguments.
        GunnsFluidMetabolic3InputData(const bool   malfBlockageFlag  = false,
                                      const double malfBlockageValue = 0.0,
                                      const double flowDemand        = 0.0,
                                      const double nNominal          = 0.0,
                                      const double nSleep            = 0.0,
                                      const double nRecovery0        = 0.0,
                                      const double nRecovery1        = 0.0,
                                      const double nRecovery2        = 0.0,
                                      const double nRecovery3        = 0.0,
                                      const double nAerobic0         = 0.0,
                                      const double nAerobic1         = 0.0,
                                      const double nResistive0       = 0.0,
                                      const double nResistive1       = 0.0,
                                      const double nResistive2       = 0.0,
                                      const double nResistive3       = 0.0);
        /// @brief    Copy constructs this Metabolic input data.
        GunnsFluidMetabolic3InputData(const GunnsFluidMetabolic3InputData& that);
        /// @brief    Default destructs this Metabolic input data.
        ~GunnsFluidMetabolic3InputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolic3InputData& operator=(const GunnsFluidMetabolic3InputData&);
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
class GunnsFluidMetabolic3 : public GunnsFluidSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidMetabolic3);
    public:
        /// @brief    Enumeration of the types of metabolic states.
        enum MetabolicType {
            NOMINAL      = 0,  ///< Nominal (14.5 hr).
            SLEEP        = 1,  ///< Sleep (8 hr).
            RECOVERY_0   = 2,  ///< Recovery   0-15 min
            RECOVERY_1   = 3,  ///< Recovery  15-30 min
            RECOVERY_2   = 4,  ///< Recovery  30-45 min
            RECOVERY_3   = 5,  ///< Recovery  45-60 min
            AEROBIC_0    = 6,  ///< Aerobic    0-15 min
            AEROBIC_1    = 7,  ///< Aerobic   15-30 min
            RESISTIVE_0  = 8,  ///< Resistive  0-15 min
            RESISTIVE_1  = 9,  ///< Resistive 15-30 min
            RESISTIVE_2  = 10, ///< Resistive 30-45 min
            RESISTIVE_3  = 11, ///< Resistive 45-60 min
            NO_METABOLIC = 12  ///< Invalid or number of metabolic states - Keep this last!
        };
        double mNCrew[GunnsFluidMetabolic3::NO_METABOLIC];              /**<    (--)                       Array of number of crew members in each metabolic state. */
        /// @brief    Default constructs this Metabolic model.
        GunnsFluidMetabolic3();
        /// @brief    Default destructs this Metabolic model.
        virtual ~GunnsFluidMetabolic3();
        /// @brief    Initializes this Metabolic model with configuration and input data.
        void initialize(const GunnsFluidMetabolic3ConfigData& configData,
                        const GunnsFluidMetabolic3InputData&  inputData,
                        std::vector<GunnsBasicLink*>&         networkLinks,
                        const int                             port0,
                        const int                             port1);
        /// @brief    Step method for updating this Metabolic.
        virtual void step(const double dt);
        /// @brief    Method for computing the flows across the link.
        virtual void computeFlows(const double dt);
        /// @brief    Method for transporting the flows across the link.
        virtual void transportFlows(const double dt);
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
        double mO2ConsumptionRate[GunnsFluidMetabolic3::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of Oxygen consumption rate for a crew member in each metabolic state. */
        double mCO2ProductionRate[GunnsFluidMetabolic3::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of Carbon dioxide production rate for a crew member in each metabolic state. */
        double mH2OProductionRate[GunnsFluidMetabolic3::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of Water production rate for a crew member in each metabolic state. */
        double mHeatProductionRate[GunnsFluidMetabolic3::NO_METABOLIC]; /**<    (W)    trick_chkpnt_io(**) Array of Heat production rate for a crew member in each metabolic state. */
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
        /// @brief    Validates the initialization inputs of this Metabolic model.
        void   validate(const GunnsFluidMetabolic3InputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Initializes the derived attributes of this Metabolic model.
        void   derive(const GunnsFluidMetabolic3ConfigData& configData,
                      const GunnsFluidMetabolic3InputData&  inputData);
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
        GunnsFluidMetabolic3(const GunnsFluidMetabolic3&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolic3& operator=(const GunnsFluidMetabolic3&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The O2 consumption rate.
///
/// @details  Getter method returns the number of crew array
////////////////////////////////////////////////////////////////////////////////////////////
inline const double* GunnsFluidMetabolic3::getNCrew() const
{
    return mNCrew;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The O2 consumption rate.
///
/// @details  Getter method returns O2 consumption rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic3::getConsumedO2() const
{
    return mConsumedO2;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The CO2 production rate.
///
/// @details  Getter method returns CO2 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic3::getProducedCO2() const
{
    return mProducedCO2;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The H2O production rate.
///
/// @details  Getter method returns H2O production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic3::getProducedH2O() const
{
    return mProducedH2O;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) The heat production rate.
///
/// @details  Getter method returns heat production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic3::getProducedHeat() const
{
    return mProducedHeat;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The NH3 production rate.
///
/// @details  Getter method returns NH3 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic3::getProducedNH3() const
{
    return mProducedNH3;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The CO production rate.
///
/// @details  Getter method returns CO production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic3::getProducedCO() const
{
    return mProducedCO;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The H2 production rate.
///
/// @details  Getter method returns H2 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic3::getProducedH2() const
{
    return mProducedH2;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The CH4 production rate.
///
/// @details  Getter method returns CH4 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic3::getProducedCH4() const
{
    return mProducedCH4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Zeroes individual fluid constituent rates.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidMetabolic3::resetRates()
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
inline double GunnsFluidMetabolic3::computeProductionRate(const int indexFluid, const int indexTc,
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
inline void GunnsFluidMetabolic3::setTraceMass(GunnsFluidTraceCompounds* tc, const int index,
                                               const double rate)
{
    if (index > -1) {
        tc->setMass(index, rate);
    }
}

#endif
