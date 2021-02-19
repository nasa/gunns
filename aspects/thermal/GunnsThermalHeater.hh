#ifndef GunnsThermalHeater_EXISTS
#define GunnsThermalHeater_EXISTS
/********************************** TRICK HEADER ***************************************************
@defgroup  TSM_GUNNS_THERMAL_HEATER    GUNNS Thermal Heater Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
 (The GunnsThermalHeater will model all heaters and be compatible within a GUNNS Thermal
 Network. It may or may not be tied to a card. If so, its power is set every Network::update()
 call by a HeaterCardInterface object. Otherwise, its electircal power value in Watts will
 come directly from Electrical via the simbus.

 GunnsThermalHeater will multiply the electrical power by an tuning scalar, which represents how
 well the heater converts electrical power into thermal heatflux. The resulting heatflux, with
 units still in Watts, is applied to a node in the GUNNS Thermal Network. The heater's sole
 malfunction in the thermal aspect is a blockage, in which the heater still consumes power
 but its heat is not transferred to the appropriate node. This capability is already provided
 by the BasicLink's blockage attribute, set in the InputData.)

REQUIREMENTS:
 (R.TS228-0005: Spacecraft thermal models shall simulate the functionality of heaters.)
 (R.TS228-0031: Spacecraft thermal models shall provide heater malfunctions.)

REFERENCE:
 ()

ASSUMPTIONS AND LIMITATIONS:
 (A constant resistive load with no thermal capacitance of its own.)

LIBRARY DEPENDENCY:
 ((GunnsThermalHeater.o))

PROGRAMMERS:
 ((Joe Valerioti) (L3) (Aug 2011) (derived from GunnsBasicSource)
                       (Nov 2011) (significant updates, unit-test and code review)
                       (Mar 2012) (converted to multi-port)
                       (Jan 2013) (change to inherit from new GunnsThermalSource)
 )
@{
***************************************************************************************************/
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsThermalSource.hh"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Thermal Heater Configuration Data
///
/// @details  This class provides a data structure for the ThermalHeater ConfigData.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalHeaterConfigData: public GunnsThermalSourceConfigData
{
    public:
        /// @brief  Default constructs this Thermal Heater configuration data.
        GunnsThermalHeaterConfigData(const std::string& name = "unnamed heater",
                                        GunnsNodeList* nodes =   0,
                                   const double tuningScalar = 1.0,
              std::vector<double>* fluxDistributionFractions =   0);

        /// @brief  Default destructs this Thermal Heater configuration data.
        virtual ~GunnsThermalHeaterConfigData();
        /// @brief  Copy constructs this Thermal Heater configuration data.
        GunnsThermalHeaterConfigData(const GunnsThermalHeaterConfigData& that);

    protected:
        // nothing
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalHeaterConfigData& operator=( const GunnsThermalHeaterConfigData&);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Thermal Heater Input Data
///
/// @details  This class provides a data structure for the ThermalHeater InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalHeaterInputData: public GunnsThermalSourceInputData
{
    public:
        /// @brief  Default constructs this Thermal Heater input data.
        GunnsThermalHeaterInputData(const bool malfBlockageFlag        = false,
                                    const double malfBlockageValue     = 0.0,
                                    const double demandedFlux          = 0.0,
                                    const bool malfFluxOverrideFlag    = false,
                                    const double malfFluxOverrideValue = 0.0);

        /// @brief  Default destructs this Thermal Heater input data.
        virtual ~GunnsThermalHeaterInputData();
        /// @brief  Copy constructs this Thermal Heater input data.
        GunnsThermalHeaterInputData(const GunnsThermalHeaterInputData& that);

    protected:
        // nothing
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalHeaterInputData& operator=(const GunnsThermalHeaterInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief   Heater class compatible in a GUNNS network.
///
/// @details The GunnsThermalHeater takes a power input from the electrical network and converts it
///          to a thermal heatflux to apply to a node in a thermal network.
///          The class, per its inheritance from GunnsThermalSource, creates a fixed flux onto one
///          or more nodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalHeater: public GunnsThermalSource
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermalHeater);

    public:
        /// @brief   Default Constructor.
        GunnsThermalHeater();
        /// @brief   Default Destructor.
        virtual ~GunnsThermalHeater();

        /// @brief   Initializes the link with Config and Input data
        void initialize(const GunnsThermalHeaterConfigData& configData,
                        const GunnsThermalHeaterInputData& inputData,
                        std::vector<GunnsBasicLink*>& networkLinks,
                        std::vector<int>* portsVector);

        /// @brief  A setter for the Thermal Heater's electrical power.
        void setPowerElectrical(const double inputElectricalPower);
        /// @brief  A getter for the Thermal Heater's mPowerElectrical.
        double getPowerElectrical() const;
        /// @brief  Updates the state of the link.
        virtual void updateState(const double dt);

    protected:
        /**< (W)  Electrical power, set by the electrical aspect via the simbus. */
        double mPowerElectrical; /**< (W)  Electrical power actually consumed by the heater. */

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsThermalHeater(const GunnsThermalHeater& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalHeater& operator=(const GunnsThermalHeater& that);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param    inputElectricalPower   (W)   value to which mPowerElectrical will be set
///
/// @details  A setter for the Thermal Heater's electrical power.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsThermalHeater::setPowerElectrical(const double inputElectricalPower)
{
    /// - set heater's electrical power
    mPowerElectrical = inputElectricalPower;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   (W)  Requested electrical power of the heater.
///
/// @details  A getter for the Thermal Heater's mPowerElectrical.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermalHeater::getPowerElectrical() const
{
    return mPowerElectrical;
}

#endif
