#ifndef GunnsFluidMetabolic_EXISTS
#define GunnsFluidMetabolic_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_METABOLICS  Human Metabolics Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for modeling human metabolics within a GUNNS fluid network.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidMetabolic.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Prototype) (2011-06))

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
class GunnsFluidMetabolicConfigData : public GunnsFluidSourceConfigData
{
    public:
        double mConsumptionRateO2;   /**< (kg/s) trick_chkpnt_io(**) Nominal O2 consumption rate for a crew member. */
        double mProductionRateCO2;   /**< (kg/s) trick_chkpnt_io(**) Nominal CO2 production rate for a crew member. */
        double mProductionRateH2O;   /**< (kg/s) trick_chkpnt_io(**) Nominal H2O production rate for a crew member. */
        double mProductionRateHeat;  /**< (W)    trick_chkpnt_io(**) Nominal heat production rate for a crew member. */
        double mSleepScaleFactor;    /**< (--)   trick_chkpnt_io(**) Sleep production/consumption scale factor. */
        double mRecoveryScaleFactor; /**< (--)   trick_chkpnt_io(**) Recovery production/consumption scale factor. */
        double mExerciseScaleFactor; /**< (--)   trick_chkpnt_io(**) Exercise production/consumption scale factor. */
        /// @brief    Default constructs this Metabolic configuration data with arguments.
        GunnsFluidMetabolicConfigData(const std::string& name                = "",
                                      GunnsNodeList*     nodes               = 0,
                                      const double       consumptionRateO2   = 0.0,
                                      const double       productionRateCO2   = 0.0,
                                      const double       productionRateH2O   = 0.0,
                                      const double       productionRateHeat  = 0.0,
                                      const double       sleepScaleFactor    = 0.0,
                                      const double       recoveryScaleFactor = 0.0,
                                      const double       exerciseScaleFactor = 0.0);
        /// @brief    Default destructs this Metabolic configuration data.
        virtual ~GunnsFluidMetabolicConfigData();
        /// @brief    Copy constructs this Metabolic configuration data.
        GunnsFluidMetabolicConfigData(const GunnsFluidMetabolicConfigData& that);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolicConfigData& operator =(const GunnsFluidMetabolicConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Metabolic Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Metabolic
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMetabolicInputData : public GunnsFluidSourceInputData
{
    public:
        double mNNominal;  /**< (--) Number of crew members in nominal state. */
        double mNSleep;    /**< (--) Number of crew members in sleep state. */
        double mNRecovery; /**< (--) Number of crew members in recovery state. */
        double mNExercise; /**< (--) Number of crew members in exercise state. */
        /// @brief    Default constructs this Metabolic input data with arguments.
        GunnsFluidMetabolicInputData(const bool   malfBlockageFlag  = false,
                                const double malfBlockageValue = 0.0,
                                const double flowDemand        = 0.0,
                                const double nNominal          = 0.0,
                                const double nSleep            = 0.0,
                                const double nRecovery         = 0.0,
                                const double nExercise         = 0.0);
        /// @brief    Copy constructs this Metabolic input data.
        GunnsFluidMetabolicInputData(const GunnsFluidMetabolicInputData& that);
        /// @brief    Default destructs this Metabolic input data.
        ~GunnsFluidMetabolicInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolicInputData& operator=(const GunnsFluidMetabolicInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Metabolic
///
/// @details  Class for modeling human metabolic comsumption and production of atmosheric
///           constituents and heat.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMetabolic : public GunnsFluidSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidMetabolic);
    public:
        /// @brief    Enumeration of the types of metabolic states.
        enum MetabolicType {
            NOMINAL      = 0,
            SLEEP        = 1,
            RECOVERY     = 2,
            EXERCISE     = 3,
            NO_METABOLIC = 4  // Invalid or number of metabolic states - Keep this last!
        };
        /// @brief    Default constructs this Metabolic model.
        GunnsFluidMetabolic();
        /// @brief    Default destructs this Metabolic model.
        virtual ~GunnsFluidMetabolic();
        /// @brief    Initializes this Metabolic model with configuration and input data.
        void initialize(const GunnsFluidMetabolicConfigData& configData,
                        const GunnsFluidMetabolicInputData&  inputData,
                        std::vector<GunnsBasicLink*>&   networkLinks,
                        const int                       port0,
                        const int                       port1);
        /// @brief    Updates the state of this Metabolic.
        virtual void updateState(const double dt);
        /// @brief    Updates the internal fluid of this Metabolic model.
        virtual void updateFluid(const double dt, const double flowrate);
        /// @brief    Transitions a crew member between metabolic states.
        void transition(const double        number,
                        const MetabolicType fromState,
                        const MetabolicType toState);
        /// @brief    Returns a pointer to the number of crew members array
        const double* getNCrew() const;
        /// @brief    Returns O2 consumption rate for this cycle
        double getConsumedO2() const;
        /// @brief    Returns CO2 production rate for this cycle
        double getProducedCO2() const;
        /// @brief    Returns H2O production rate for this cycle
        double getProducedH2O() const;
        /// @brief    Returns heat production rate for this cycle
        double getProducedHeat() const;
    protected:
        double          mNCrew[GunnsFluidMetabolic::NO_METABOLIC];              /**<    (--)                       Array of number of crew members in each metabolic state. */
        double          mO2ConsumptionRate[GunnsFluidMetabolic::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of O2 consumption rate for a crew member in each metabolic state. */
        double          mCO2ProductionRate[GunnsFluidMetabolic::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of CO2 production rate for a crew member in each metabolic state. */
        double          mH2OProductionRate[GunnsFluidMetabolic::NO_METABOLIC];  /**<    (kg/s) trick_chkpnt_io(**) Array of H2O production rate for a crew member in each metabolic state. */
        double          mHeatProductionRate[GunnsFluidMetabolic::NO_METABOLIC]; /**<    (W)    trick_chkpnt_io(**) Array of Heat production rate for a crew member in each metabolic state. */
        double          mConsumedO2;                                            /**<    (kg/s) trick_chkpnt_io(**) O2 consumption rate this cycle. */
        double          mProducedCO2;                                           /**<    (kg/s) trick_chkpnt_io(**) CO2 production rate this cycle. */
        double          mProducedH2O;                                           /**<    (kg/s) trick_chkpnt_io(**) H2O production rate this cycle. */
        double          mProducedHeat;                                          /**<    (W)    trick_chkpnt_io(**) Heat production rate this cycle. */
        int             mO2;                                                    /**< *o (--)   trick_chkpnt_io(**) Index of O2 constituent in atmosphere. */
        int             mCO2;                                                   /**< *o (--)   trick_chkpnt_io(**) Index of CO2 constituent in atmosphere. */
        int             mH2O;                                                   /**< *o (--)   trick_chkpnt_io(**) Index of H2O constituent in atmosphere. */
        /// @brief    Validates the initialization inputs of this Metabolic model.
        void validate(const GunnsFluidMetabolicConfigData& configData,
                      const GunnsFluidMetabolicInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Initializes the derived attributes of this Metabolic model.
        void derive(const GunnsFluidMetabolicConfigData& configData,
                    const GunnsFluidMetabolicInputData&  inputData);
        /// @brief    Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolic(const GunnsFluidMetabolic&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidMetabolic& operator=(const GunnsFluidMetabolic&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The O2 consumption rate.
///
/// @details    Getter method returns the number of crew array
////////////////////////////////////////////////////////////////////////////////////////////
inline const double* GunnsFluidMetabolic::getNCrew() const
{
    return mNCrew;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The O2 consumption rate.
///
/// @details    Getter method returns O2 consumption rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic::getConsumedO2() const
{
    return mConsumedO2;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The CO2 production rate.
///
/// @details    Getter method returns CO2 production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic::getProducedCO2() const
{
    return mProducedCO2;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (kg/s) The H2O production rate.
///
/// @details    Getter method returns H2O production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic::getProducedH2O() const
{
    return mProducedH2O;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (W) The heat production rate.
///
/// @details    Getter method returns heat production rate for this cycle
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMetabolic::getProducedHeat() const
{
    return mProducedHeat;
}

#endif
