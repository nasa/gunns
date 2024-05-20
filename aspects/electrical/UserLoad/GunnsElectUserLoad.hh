#ifndef GunnsElectUserLoad_EXISTS
#define GunnsElectUserLoad_EXISTS

/**
@file
@brief    GUNNS Electrical User Load Base Spotter declarations

@defgroup TSM_GUNNS_ELECTRICAL_USERLOADBASE_SPOTTER GUNNS Electrical User Load Base Spotter
@ingroup  TSM_GUNNS_ELECTRICAL_USERLOADBASE

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for the GUNNS Electrical User Load Base Spotter.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectUserLoad.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-05) (Initial))

@{
*/

#include "core/GunnsNetworkSpotter.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical User Load Base Spotter Configuration Data
///
/// @details  This provides a data structure for the GUNNS Electrical User Load Base Spotter
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoadConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        double mUnderVoltageLimit;  /**< (V)   trick_chkpnt_io(**) Low voltage limit for operation. */
        double mFuseCurrentLimit;   /**< (amp) trick_chkpnt_io(**) Current above which the fuse blows. */
        double mDutyCycleFraction;  /**< (1)   trick_chkpnt_io(**) Fraction of time the load spends in the on state in its duty cycle. */
        double mDutyCyclePeriod;    /**< (s)   trick_chkpnt_io(**) Duration of each duty cycle. */
        /// @brief Default constructs this GUNNS Electrical User Load Base Spotter configuration
        ///        data.
        GunnsElectUserLoadConfigData(const std::string& name,
                                     const double       underVoltageLimit,
                                     const double       fuseCurrentLimit,
                                     const double       dutyCycleFraction,
                                     const double       dutyCyclePeriod);
        /// @brief Default destructs this GUNNS Electrical User Load Base Spotter configuration
        ///        data.
        virtual ~GunnsElectUserLoadConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectUserLoadConfigData(const GunnsElectUserLoadConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoadConfigData& operator =(const GunnsElectUserLoadConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical User Load Base Spotter Input Data
///
/// @details  This provides a data structure for the GUNNS Electrical User Load Base Spotter
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoadInputData : public GunnsNetworkSpotterInputData
{
    public:
        int    mInitialMode;        /**< (1) trick_chkpnt_io(**) Initial mode. */
        double mInitialVoltage;     /**< (V) trick_chkpnt_io(**) Initial input voltage. */
        double mDutyCycleTimer;     /**< (s) trick_chkpnt_io(**) Initial duty cycle elapsed time. */
        /// @brief Default constructs this GUNNS Electrical User Load Base Spotter input data.
        GunnsElectUserLoadInputData(const int    initialMode,
                                    const double initialVoltage,
                                    const double dutyCycleTimer);
        /// @brief Default destructs this GUNNS Electrical User Load Base Spotter input data.
        virtual ~GunnsElectUserLoadInputData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectUserLoadInputData(const GunnsElectUserLoadInputData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoadInputData& operator =(const GunnsElectUserLoadInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical User Load Base Spotter Class.
///
/// @details  This provides an interface to a User Load object contained within a derived class.
///
/// @note     This is a pure-virtual class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectUserLoad : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectUserLoad);
    public:
        /// @brief Default Constructor
        GunnsElectUserLoad();
        /// @brief Default destructor.
        virtual     ~GunnsElectUserLoad();
        /// @brief Loads the derived class config & input data.
        virtual void          initialize(const GunnsNetworkSpotterConfigData* configData,
                                         const GunnsNetworkSpotterInputData*  inputData);
        /// @brief Initializes the derived class and its contained User Load.
        virtual void          initLoad() = 0;
        /// @brief Provides interface to the User Load contained in a derived class.
        virtual UserLoadBase* getLoad() = 0;
        /// @brief Steps the GUNNS Electrical User Load Base Spotter prior to the GUNNS solver step.
        virtual void          stepPreSolver(const double dt);
        /// @brief Steps the GUNNS Electrical User Load Base Spotter after the GUNNS solver step.
        virtual void          stepPostSolver(const double dt);
        /// @brief Sets the supply voltage of this GUNNS Electrical User Load Base Spotter.
        void                  setSupplyVoltage(const double voltage);
        /// @brief Updates the user load object and updates the supply load.
        void                  step();
        /// @brief Updates the user load duty cycle function.
        void                  stepDutyCycle(const double dt);

    protected:
        double mDutyCycleFraction;  /**< (1) trick_chkpnt_io(**) Fraction of time the load spends in the on state in its duty cycle. */
        double mDutyCyclePeriod;    /**< (s) trick_chkpnt_io(**) Duration of each duty cycle. */
        double mDutyCycleTimer;     /**< (s)                     Duty cycle elapsed time. */
        double mSupplyVoltage;      /**< (V)                     Voltage input from the power supply. */
        /// @brief Validates the supplied configuration data.
        const GunnsElectUserLoadConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief Validates the supplied input data.
        const GunnsElectUserLoadInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectUserLoad(const GunnsElectUserLoad& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectUserLoad& operator =(const GunnsElectUserLoad& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  voltage  (V)  Given value to set the supply voltage to.
///
/// @details  Sets the mSupplyVoltage attribute to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectUserLoad::setSupplyVoltage(const double voltage)
{
    mSupplyVoltage = voltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Steps the contained user load class, giving it the mSupplyVoltage attribute to use as
///           its input voltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectUserLoad::step()
{
    getLoad()->step(mSupplyVoltage);
}

#endif
