#ifndef GunnsFluidHeater_EXISTS
#define GunnsFluidHeater_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_Heater  Fluid Heater Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Heater link model.)

REQUIREMENTS:
- (TBD)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidHeater.o))

 PROGRAMMERS:
- ((Jason Harvey) (L3) (Initial) (2012-03))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heater Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Heater link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHeaterConfigData : public GunnsFluidLinkConfigData
{
    public:
        /// @details  This is the fraction of consumed heater power that is actually transported
        ///           between the nodes.  It is mainly intended as a tuning parameter.
        double mHeaterEfficiency;     /**< (--)  trick_chkpnt_io(**) (0-1) Heater efficiency */
        GunnsFluidHeaterConfigData(const std::string& name             = "",
                                   GunnsNodeList*     nodes            = 0,
                                   const double       heaterEfficiency = 1.0);
        /// @brief Default destructs this Heater model configuration data.
        virtual ~GunnsFluidHeaterConfigData();
        /// @brief Copy constructs this Heater model configuration data.
        GunnsFluidHeaterConfigData(const GunnsFluidHeaterConfigData& that);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHeaterConfigData& operator =(const GunnsFluidHeaterConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heater Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Heater link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHeaterInputData : public GunnsFluidLinkInputData
{
    public:
        /// @details  This is the input to control the total heat consumed, prior to it being
        ///           scaled by the efficiency.  This would be a heater coil's electrical power
        ///           load, for example.
        double mHeaterPower;          /**< (W)   Initial heater power consumed */
        /// @brief Default constructs this Heater model input data with arguments.
        GunnsFluidHeaterInputData(const bool   malfBlockageFlag  = false,
                                  const double malfBlockageValue = 0.0,
                                  const double heaterPower       = 0.0);
        /// @brief Copy constructs this Heater model input data.
        GunnsFluidHeaterInputData(const GunnsFluidHeaterInputData& that);
        /// @brief Default destructs this Heater model input data.
        ~GunnsFluidHeaterInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHeaterInputData& operator=(const GunnsFluidHeaterInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Heater Fluid Aspect Model
///
/// @details  This link transfers heat directly between the attached node fluid contents, without an
///           associated mass flow.  It can be used to simulate real fluid heaters that heat the
///           fluid directly, bypassing any shell metal mass, etc.  It can also be used as a
///           convenient way to bias heat flux into or out of a node.
///
///           Positive heat flux goes from port 0 to port 1.  Normally you will connect port 0 to
///           the vacuum node and port 1 to the node you wish to heat.  However, you can also place
///           this link between two normal nodes, in which case it will remove the heat from the
///           port 0 node and add it to the port 1 node.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHeater : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHeater);
    public:
        /// @brief Default constructs this Heater model.
        GunnsFluidHeater();
        /// @brief Default destructs this Heater model.
        virtual ~GunnsFluidHeater();
        /// @brief Initializes this Heater model with configuration and input data.
        void initialize(const GunnsFluidHeaterConfigData& configData,
                        const GunnsFluidHeaterInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);
        /// @brief Step method for updating the link
        virtual void step(const double dt);
        /// @brief Computes the flows through the link
        virtual void computeFlows(const double dt);
        /// @brief transports the flows heat flux through the link
        virtual void transportFlows(const double dt);
        /// @brief Returns the heat flux
        double getHeatFlux() const;
        /// @brief sets the heater power
        void setHeaterPower(const double heaterPower);

    protected:
        /// @details  Model output, mHeatFlux = mHeaterPower * mEfficiency *
        ///           (1 - mMalfBlockageValue (if active)).  TODO replace with term from FluidLink
        ///           if it ever gets added there.
        double mHeatFlux;             /**< (J/s)                     Actual transported link heat flux */
        /// @details  This is the input to control the total heat consumed, prior to it being
        ///           scaled by the efficiency.  This would be a heater coil's electrical power
        ///           load, for example.
        double mHeaterPower;          /**< (W)                       Heater power consumed */
        /// @details  This is the fraction of mHeaterPower that is actually transported between the
        ///           nodes.  It is mainly intended as a tuning parameter.
        double mHeaterEfficiency;     /**< (--)  trick_chkpnt_io(**) (0-1) Heater efficiency */
        /// @brief Validates the initialization inputs of this Heater model.
        void validate(const GunnsFluidHeaterConfigData& configData,
                      const GunnsFluidHeaterInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHeater(const GunnsFluidHeater&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidHeater& operator=(const GunnsFluidHeater&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double (J/s) The heat flux.
///
/// @details  Getter method returns the heat flux.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidHeater::getHeatFlux() const
{
    return mHeatFlux;
}

#endif
