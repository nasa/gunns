#ifndef GunnsThermoelectricDevice_EXISTS
#define GunnsThermoelectricDevice_EXISTS

/**
@file
@brief    GUNNS Thermoelectric Device Link declarations

@defgroup  TSM_GUNNS_THERMAL_ELECTRIC_DEVICE    GUNNS Thermoelectric Device Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (Classes for the GUNNS Thermoelectric Device Link.)

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  ()

LIBRARY_DEPENDENCY:
  (GunnsThermoelectricDevice.o)

PROGRAMMERS:
  (
   ((Jason Harvey) (L-3 Communications) (September 2015) (Initial))
  )
@{
*/

#include "core/GunnsBasicConductor.hh"
#include "GunnsThermoelectricEffect.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermoelectric Device link Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermoelectric
///           Device link configuration data.  Refer to the GunnsThermoelectricEffect class for
///           details on its configuration parameters.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermoelectricDeviceConfigData : public GunnsBasicConductorConfigData
{
    public:
        GunnsThermoelectricEffectConfigData mThermoelectricEffect; /**< (--) The thermoelectric effect config data. */
        /// @brief Default constructs this Thermoelectric Device link configuration data.
        GunnsThermoelectricDeviceConfigData(const std::string& name                        = "",
                                            GunnsNodeList*     nodes                       = 0,
                                            const double       numThermocouples            = 0.0,
                                            const double       geometryFactor              = 0.0,
                                            const double       electricalResistivityCoeff0 = 0.0,
                                            const double       electricalResistivityCoeff1 = 0.0,
                                            const double       seebeckCoeff0               = 0.0,
                                            const double       seebeckCoeff1               = 0.0,
                                            const double       seebeckCoeff2               = 0.0,
                                            const double       thermalConductivityCoeff0   = 0.0,
                                            const double       thermalConductivityCoeff1   = 0.0,
                                            const double       thermalConductivityCoeff2   = 0.0,
                                            const double       endPlateThermalConductance  = 0.0,
                                            const double       minTemperature              = 0.0,
                                            const double       maxTemperature              = 0.0);
        /// @brief Default destructs this Thermoelectric Device link configuration data.
        virtual ~GunnsThermoelectricDeviceConfigData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsThermoelectricDeviceConfigData(const GunnsThermoelectricDeviceConfigData& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsThermoelectricDeviceConfigData& operator =(const GunnsThermoelectricDeviceConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermoelectric Device link Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermoelectric
///           Device link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermoelectricDeviceInputData : public GunnsBasicConductorInputData
{
    public:
        GunnsThermoelectricEffectInputData mThermoelectricEffect; /**< (--) The thermoelectric effect input data. */
        /// @brief    Default constructs this Thermoelectric Device link input data.
        GunnsThermoelectricDeviceInputData(const bool   malfBlockageFlag                = false,
                                           const double malfBlockageValue               = 0.0,
                                           const bool   malfThermoelectricEffectsFlag   = false,
                                           const double malfThermoelectricEffectsScalar = 0.0);
        /// @brief    Default destructs this Thermoelectric Device link input data.
        virtual ~GunnsThermoelectricDeviceInputData();

    private:
        /// @details  Copy constructor unavailable since declared private and not implemented.
        GunnsThermoelectricDeviceInputData(const GunnsThermoelectricDeviceInputData& that);
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermoelectricDeviceInputData& operator =(const GunnsThermoelectricDeviceInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Thermoelectric Device Link
///
/// @details  This link is used to model the thermal aspect of a thermoelectric device (TED) in a
///           GUNNS thermal network.  It wraps a GunnsThermoelectricEffect utility model and
///           provides the interface between the model and the network.  This link's Port 0 is the
///           thermoelectric effect's "hot" plate side, so that positive thru-flux goes from hot to
///           cold and from Port 0 to Port 1.
///
///           This link acts like both a conductor and a source, with heat sources added directly to
///           the nodes.  Therefore you should be careful about using this link with non-capacitive
///           nodes and ensure there's always a place for heat to go to or come from in the network.
///           It is recommended that both nodes be capacitive and have connections to other nodes in
///           a thermal circuit.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermoelectricDevice : public GunnsBasicConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermoelectricDevice);

    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsThermoelectricEffect mThermoelectricEffect;     /**< (--) The thermoelectric effect. */
        /// @}
        /// @brief Default Constructor
        GunnsThermoelectricDevice();
        /// @brief Default Destructor
        virtual ~GunnsThermoelectricDevice();
        /// @brief Initialize method
        void initialize(const GunnsThermoelectricDeviceConfigData& configData,
                              GunnsThermoelectricDeviceInputData&  inputData,
                        std::vector<GunnsBasicLink*>&              networkLinks,
                        const int                                  port0,
                        const int                                  port1);
        /// @brief step method for updating the link
        virtual void step(const double dt);

    protected:
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Updates the Admittance and Source terms for the link.
        virtual void buildPotential();
        /// @brief Computes power across the link
        virtual void computePower();
        /// @brief Adds fluxes to nodes.
        virtual void transportFlux(const int fromPort = 0, const int toPort = 1);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsThermoelectricDevice(const GunnsThermoelectricDevice& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsThermoelectricDevice& operator =(const GunnsThermoelectricDevice& that);
};

/// @}

#endif
