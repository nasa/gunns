#ifndef GunnsElectConstantPowerUserLoad_EXISTS
#define GunnsElectConstantPowerUserLoad_EXISTS

/**
@file
@brief    GUNNS Electrical Constant-Power User Load Spotter declarations

@defgroup TSM_GUNNS_ELECTRICAL_USERLOADBASE__RESISTIVE_SPOTTER GUNNS Electrical Constant-Power User Load Spotter
@ingroup  TSM_GUNNS_ELECTRICAL_USERLOADBASE

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for the GUNNS Electrical Constant-Power User Load Spotter.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectConstantPowerUserLoad.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-05) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsElectUserLoad.hh"
#include "ConstantPowerLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Constant-Power User Load Spotter Configuration Data
///
/// @details  This provides a data structure for the GUNNS Electrical Constant-Power User Load
///           Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectConstantPowerUserLoadConfigData : public GunnsElectUserLoadConfigData
{
    public:
        double mPowerNormal;                 /**< (W) trick_chkpnt_io(**) Power for normal mode. */
        double mPowerStandby;                /**< (W) trick_chkpnt_io(**) Power for standby mode. */
        /// @brief Default constructs this GUNNS Electrical Constant-Power User Load Spotter
        ///        configuration data.
        GunnsElectConstantPowerUserLoadConfigData(const std::string& name              = "",
                                                  const double       underVoltageLimit = 0.0,
                                                  const double       powerNormal       = 0.0,
                                                  const double       powerStandby      = 0.0,
                                                  const double       fuseCurrentLimit  = 0.0,
                                                  const double       dutyCycleFraction = 0.0,
                                                  const double       dutyCyclePeriod   = 0.0);
        /// @brief Default destructs this GUNNS Electrical Constant-Power User Load Spotter
        ///        configuration data.
        virtual ~GunnsElectConstantPowerUserLoadConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectConstantPowerUserLoadConfigData(const GunnsElectConstantPowerUserLoadConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectConstantPowerUserLoadConfigData& operator =(const GunnsElectConstantPowerUserLoadConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Constant-Power User Load Spotter Input Data
///
/// @details  This provides a data structure for the GUNNS Electrical Constant-Power User Load
///           Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectConstantPowerUserLoadInputData : public GunnsElectUserLoadInputData
{
    public:
        /// @brief Default constructs this GUNNS Electrical Constant-Power User Load Spotter input
        ///        data.
        GunnsElectConstantPowerUserLoadInputData(const int    initialMode    = 0.0,
                                                 const double initialVoltage = 0.0,
                                                 const double dutyCycleTimer = 0.0);
        /// @brief Default destructs this GUNNS Electrical Constant-Power User Load Spotter input
        ///        data.
        virtual ~GunnsElectConstantPowerUserLoadInputData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectConstantPowerUserLoadInputData(const GunnsElectConstantPowerUserLoadInputData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectConstantPowerUserLoadInputData& operator =(const GunnsElectConstantPowerUserLoadInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Constant-Power User Load Spotter Class.
///
/// @details  This wraps a resistive User Load object in a GUNNS spotter.  This allows the User Load
///           to easily be added to GunnShow network drawings, or it can also be added to the sim in
///           another class.  An interface to the User Load is provided for external models, such as
///           a GunnsElectUserLoadSwitch link, to initialize and update the load in run-time.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectConstantPowerUserLoad : public GunnsElectUserLoad
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectConstantPowerUserLoad);
    public:
        /// @brief Default Constructor.
        GunnsElectConstantPowerUserLoad(GunnsElectConstantPowerUserLoadConfigData* configData = 0,
                                        GunnsElectConstantPowerUserLoadInputData*  inputData  = 0);
        /// @brief Default destructor.
        virtual     ~GunnsElectConstantPowerUserLoad();
        /// @brief Loads the config & input data.
        virtual void         initialize(const GunnsNetworkSpotterConfigData* configData,
                                         const GunnsNetworkSpotterInputData*  inputData);
        /// @brief Initializes this spotter and the contained User Load.
        virtual void          initLoad();
        /// @brief Provides interface to the contained User Load.
        virtual UserLoadBase* getLoad();

    protected:
        const GunnsElectConstantPowerUserLoadConfigData* mConfig; /**< ** (--) trick_chkpnt_io(**) The resistive User Load's config data. */
        const GunnsElectConstantPowerUserLoadInputData*  mInput;  /**< ** (--) trick_chkpnt_io(**) The resistive User Load's input data. */
        ConstantPowerLoad                                mLoad;   /**<    (--)                     The resistive User Load. */
        /// @brief Validates the supplied configuration data.
        const GunnsElectConstantPowerUserLoadConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief Validates the supplied input data.
        const GunnsElectConstantPowerUserLoadInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectConstantPowerUserLoad(const GunnsElectConstantPowerUserLoad& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectConstantPowerUserLoad& operator =(const GunnsElectConstantPowerUserLoad& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  UserLoadBase* (--) Base class pointer to the resistive User Load.
///
/// @details  Returns the resistive User Load, as a base-class pointer type.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline UserLoadBase* GunnsElectConstantPowerUserLoad::getLoad()
{
    return &mLoad;
}

#endif
