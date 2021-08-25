#ifndef GunnsElectResistiveUserLoad_EXISTS
#define GunnsElectResistiveUserLoad_EXISTS

/**
@file
@brief    GUNNS Electrical Resistive User Load Spotter declarations

@defgroup TSM_GUNNS_ELECTRICAL_USERLOADBASE__RESISTIVE_SPOTTER GUNNS Electrical Resistive User Load Spotter
@ingroup  TSM_GUNNS_ELECTRICAL_USERLOADBASE

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Provides the classes for the GUNNS Electrical Resistive User Load Spotter.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectResistiveUserLoad.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-05) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsElectUserLoad.hh"
#include "ResistiveLoad.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Resistive User Load Spotter Configuration Data
///
/// @details  This provides a data structure for the GUNNS Electrical Resistive User Load Spotter
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectResistiveUserLoadConfigData : public GunnsElectUserLoadConfigData
{
    public:
        double mResistanceNormal;     /**< (ohm) trick_chkpnt_io(**) Resistance for normal mode. */
        double mResistanceStandby;    /**< (ohm) trick_chkpnt_io(**) Resistance for standby mode. */
        /// @brief Default constructs this GUNNS Electrical Resistive User Load Spotter
        ///        configuration data.
        GunnsElectResistiveUserLoadConfigData(const std::string& name              = "",
                                              const double       underVoltageLimit = 0.0,
                                              const double       resistanceNormal  = 0.0,
                                              const double       resistanceStandby = 0.0,
                                              const double       fuseCurrentLimit  = 0.0,
                                              const double       dutyCycleFraction = 0.0,
                                              const double       dutyCyclePeriod   = 0.0);
        /// @brief Default destructs this GUNNS Electrical Resistive User Load Spotter
        ///        configuration data.
        virtual ~GunnsElectResistiveUserLoadConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectResistiveUserLoadConfigData(const GunnsElectResistiveUserLoadConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectResistiveUserLoadConfigData& operator =(const GunnsElectResistiveUserLoadConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Resistive User Load Spotter Input Data
///
/// @details  This provides a data structure for the GUNNS Electrical Resistive User Load Spotter
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectResistiveUserLoadInputData : public GunnsElectUserLoadInputData
{
    public:
        /// @brief Default constructs this GUNNS Electrical Resistive User Load Spotter input data.
        GunnsElectResistiveUserLoadInputData(const int    initialMode    = 0,
                                             const double initialVoltage = 0.0,
                                             const double dutyCycleTimer = 0.0);
        /// @brief Default destructs this GUNNS Electrical Resistive User Load Spotter input data.
        virtual ~GunnsElectResistiveUserLoadInputData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectResistiveUserLoadInputData(const GunnsElectResistiveUserLoadInputData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectResistiveUserLoadInputData& operator =(const GunnsElectResistiveUserLoadInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Electrical Resistive User Load Spotter Class.
///
/// @details  This wraps a resistive User Load object in a GUNNS spotter.  This allows the User Load
///           to easily be added to GunnShow network drawings, or it can also be added to the sim in
///           another class.  An interface to the User Load is provided for external models, such as
///           a GunnsElectUserLoadSwitch link, to initialize and update the load in run-time.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectResistiveUserLoad : public GunnsElectUserLoad
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectResistiveUserLoad);
    public:
        /// @brief Default Constructor.
        GunnsElectResistiveUserLoad(GunnsElectResistiveUserLoadConfigData* configData = 0,
                                    GunnsElectResistiveUserLoadInputData*  inputData  = 0);
        /// @brief Default destructor.
        virtual     ~GunnsElectResistiveUserLoad();
        /// @brief Loads the config & input data.
        virtual void          initialize(GunnsNetworkSpotterConfigData* configData,
                                         GunnsNetworkSpotterInputData*  inputData);
        /// @brief Initializes this spotter and the contained resistive User Load.
        virtual void          initLoad();
        /// @brief Provides interface to the contained resistive User Load.
        virtual UserLoadBase* getLoad();

    protected:
        GunnsElectResistiveUserLoadConfigData* mConfig; /**< ** (--) trick_chkpnt_io(**) The resistive User Load's config data. */
        GunnsElectResistiveUserLoadInputData*  mInput;  /**< ** (--) trick_chkpnt_io(**) The resistive User Load's input data. */
        ResistiveLoad                          mLoad;   /**<    (--)                     The resistive User Load. */
        /// @brief Validates the supplied configuration data.
        GunnsElectResistiveUserLoadConfigData* validateConfig(GunnsNetworkSpotterConfigData* config);
        /// @brief Validates the supplied input data.
        GunnsElectResistiveUserLoadInputData*  validateInput (GunnsNetworkSpotterInputData* input);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectResistiveUserLoad(const GunnsElectResistiveUserLoad& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectResistiveUserLoad& operator =(const GunnsElectResistiveUserLoad& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  UserLoadBase* (--) Base class pointer to the resistive User Load.
///
/// @details  Returns the resistive User Load, as a base-class pointer type.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline UserLoadBase* GunnsElectResistiveUserLoad::getLoad()
{
    return &mLoad;
}

#endif
