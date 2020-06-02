#ifndef GunnsFluidSource_EXISTS
#define GunnsFluidSource_EXISTS

/**
@file
@brief    GUNNS Fluid Source Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_SOURCE    GUNNS Fluid Source Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Gunns Fluid Source link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidSource.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-02) (Initial Prototype)
-  (Kevin Supak)  (L-3 Communications) (2011-02) (Updated to Coding Standards))

@{
*/

#include "GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Source Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Source
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSourceConfigData : public GunnsFluidLinkConfigData
{
    public:
        /// @brief Default constructs this Fluid Source configuration data.
        GunnsFluidSourceConfigData(const std::string& name  = "",
                                   GunnsNodeList*     nodes = 0);

        /// @brief Default destructs this Fluid Source configuration data.
        virtual ~GunnsFluidSourceConfigData();

        /// @brief Copy constructs this Fluid Source configuration data.
        GunnsFluidSourceConfigData(const GunnsFluidSourceConfigData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSourceConfigData& operator =(const GunnsFluidSourceConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Source Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Source
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSourceInputData : public GunnsFluidLinkInputData
{
    public:
        double mFlowDemand;        /**< (kg/s) Initial demanded mass flow rate of the link */

        /// @brief    Default constructs this Fluid Source input data.
        GunnsFluidSourceInputData(const bool   malfBlockageFlag  = false,
                                  const double malfBlockageValue = 0.0,
                                  const double flowDemand        = 0.0);

        /// @brief    Default destructs this Fluid Source input data.
        virtual ~GunnsFluidSourceInputData();

        /// @brief    Copy constructs this Fluid Source input data.
        GunnsFluidSourceInputData(const GunnsFluidSourceInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSourceInputData& operator =(const GunnsFluidSourceInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Source class
///
/// @details  This class forces flow in a fluid network, for simulating any kind of flow boundary
///           condition.  Flow can be forced between a regular node and vacuum, or between two
///           regular nodes.  This link controls the flow fluid mixture and temperature, allowing
///           for specific fluid components to flow independently of each other or the attached node
///           properties.  This allows simulation of chemical reactions, fires, crew metabolics,
///           diffusion, osmosis, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSource : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSource);

    public:
        /// @brief Default Constructor
        GunnsFluidSource();

        /// @brief Default Destructor
        virtual ~GunnsFluidSource();

        /// @brief Initialize
        void initialize(const GunnsFluidSourceConfigData& configData,
                        const GunnsFluidSourceInputData&  inputData,
                        std::vector<GunnsBasicLink*>&     networkLinks,
                        const int                         port0,
                        const int                         port1);

        /// @brief Step method for updating the link
        virtual void step(const double dt);

        /// @brief Method for computing the flows across the link
        virtual void computeFlows(const double dt);

        /// @brief Transports the flows through the link between the nodes
        virtual void transportFlows(const double dt);

        /// @brief Sets the flow demand for the link
        virtual void setFlowDemand(const double toFlowDemand);

        /// @brief Sets the flow state of the link
        virtual void setFlowState(PolyFluid* withState);

        /// @brief Returns the flow demand of the link
        virtual double getFlowDemand() const;

    protected:
        double mFlowDemand; /**< (kg/s) User demanded mass flow rate through the link */

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Builds the source vector terms of the links contribution to the network
        virtual void buildSource();

        /// @brief Validates the initialization of this Gunns Fluid Source
        void validate() const;

        /// @brief Computes mPower for the aspect-specific implementation
        virtual void computePower();

        /// @brief Transports flow between nodes specifying source node mixture option
        virtual void transportFluidMixtureOption();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSource(const GunnsFluidSource& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSource& operator =(const GunnsFluidSource& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double (kg/s) Returns the flow demand
///
/// @details Returns the flow demand
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSource::getFlowDemand() const
{
    return mFlowDemand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] toFlowDemand (kg/s) flow demand
///
/// @details Sets the state of the fluid inside the fluid source link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSource::setFlowDemand(const double toFlowDemand)
{
    mFlowDemand = toFlowDemand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] withState (--) Fluid State information
///
/// @details Sets the state of the fluid inside the fluid source link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSource::setFlowState(PolyFluid* withState)
{
    mInternalFluid->setState(withState);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Builds the source vector terms of the link's contribution to the network.  This sign
///          convention creates positive flow from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSource::buildSource()
{
    mSourceVector[0] = -mFlux;
    mSourceVector[1] =  mFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Hydraulic power in the link is -volumetric flow * potential drop.  Note that this is
///           defined as power gain, such as from a pump.  Pressure-producing links such as pumps
///           and fans will have positive power gain, and the power in dissipating links such as
///           pipes, etc. will be negative.  Since potential is in units of kPa, we must convert it
///           to Pa to express power in Watts.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSource::computePower()
{
    mPower = -UnitConversion::PA_PER_KPA * mVolFlowRate * getDeltaPotential();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Calls the base class to transport the fluid between the nodes.  The true bool argument
///           indicates that this class can force a mixture change in the supplying node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSource::transportFluidMixtureOption()
{
    transportFluid(true);
}

#endif
