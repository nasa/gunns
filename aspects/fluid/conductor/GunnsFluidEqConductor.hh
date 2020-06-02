#ifndef GunnsFluidEqConductor_EXISTS
#define GunnsFluidEqConductor_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_EQUIVALENT_CONDUCTOR    Equivalent-Circuit Conductor Link
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Equivalent-Circuit Conductor.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidEqConductor.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2013-01) (Initial Prototype))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Equivalent-Circuit Conductor Configuration Data
///
/// @details  This provides a data structure for the Fluid Equivalent-Circuit Conductor config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidEqConductorConfigData : public GunnsFluidLinkConfigData
{
    public:
        bool mUseNetworkCapacitance; /**< (--) trick_chkpnt_io(**) Causes network capacitances to be available for output to external demands */
        /// @brief Default constructs this Fluid Equivalent-Circuit Conductor configuration data.
        GunnsFluidEqConductorConfigData(const std::string& name                  = "",
                                        GunnsNodeList*     nodes                 = 0,
                                        const bool         useNetworkCapacitance = false);
        /// @brief Copy constructs this GUNNS Fluid Equivalent-Circuit Conductor configuration data.
        GunnsFluidEqConductorConfigData(const GunnsFluidEqConductorConfigData& that);
        /// @brief Default destructs this Fluid Equivalent-Circuit Conductor configuration data.
        virtual ~GunnsFluidEqConductorConfigData();
    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidEqConductorConfigData& operator =(const GunnsFluidEqConductorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Equivalent-Circuit Conductor Input Data
///
/// @details  This provides a data structure for the Fluid Equivalent-Circuit Conductor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidEqConductorInputData : public GunnsFluidLinkInputData
{
    public:
        /// @brief Default constructs this Fluid Equivalent-Circuit Conductor input data.
        GunnsFluidEqConductorInputData(const bool   malfBlockageFlag  = false,
                                       const double malfBlockageValue = 0.0);
        /// @brief Copy constructs this Fluid Equivalent-Circuit Conductor input data.
        GunnsFluidEqConductorInputData(const GunnsFluidEqConductorInputData& that);
        /// @brief Default destructs this Fluid Equivalent-Circuit Conductor input data.
        virtual ~GunnsFluidEqConductorInputData();
    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidEqConductorInputData& operator =(const GunnsFluidEqConductorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Equivalent-Circuit Conductor
///
/// @details  This class represents a portion of an external network as a single conductor within
///           the local network.  This is intended to allow fluid loops to be split across two
///           networks, with the pump/fan in the local network and a portion of the loop path in the
///           external network.  This link interfaces with two GunnsFluidExternalDemand links in the
///           external network, which are at the end points of the represented loop section.  This
///           link acts like the two GunnsFluidExternalSupply links that the Demand links would
///           normally interface with.  This link supplies pressure and fluid properties of the
///           local nodes to the ExternalDemands, and receives demanded flux from them.
///
///           This creates the same flow and pressure drop across this link that the resulting
///           external network section would experience if it were in place of this link.  The
///           external section can be either gas or liquid (but not both), can leak out, and can
///           change the fluid temperature.  The external section can contain any number of nodes
///           and a mix of parallel and series flow paths.  Pressure & flow can go in either
///           direction.  This link assumes that any difference between inlet & outlet flux demands
///           are due to leaks from the external section.  An equal leak mass is removed from the
///           local network, as if it were leaking out through the external section.
///
/// @note     There are several limitations on the external network section:
///           - The external section must have no active capacitance, potential sources, or flow
///             sources.
///           - The external section must not change the fluid mixture.
///           - The external section must not have flow enter into it from other links besides the
///             two demand links.
///           - The external section must only connect one set of demand links at a time.
///           - This link does not support demanded mass flow (kg/s), so the external demand links
///             must send demanded flux as kg*mol/s.
///           - The external network must update at the same frequency as the local network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidEqConductor : public GunnsFluidLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsFluidEqConductor);

    public:
        /// @brief Default Constructor
        GunnsFluidEqConductor();
        /// @brief Default constructor
        virtual ~GunnsFluidEqConductor();
        /// @brief Initialize method
        void initialize(const GunnsFluidEqConductorConfigData& configData,
                        const GunnsFluidEqConductorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&          networkLinks,
                        const int                              port0,
                        const int                              port1);
        /// @brief Default constructor
        virtual void step(const double dt);
        /// @brief Compute Flows
        virtual void computeFlows(const double dt);
        /// @brief Transport Flows
        virtual void transportFlows(const double dt);
        /// @brief Special processing of data outputs from the model after the network update
        virtual void processOutputs();

    protected:
        bool    mUseNetworkCapacitance;    /**< (--)           trick_chkpnt_io(**) Causes network capacitances to be available for output to external demands */
        double  mPort0DemandFlux;          /**< (kg*mol/s)                         input from simbus */
        double  mPort0DemandTemperature;   /**< (K)                                input from simbus */
        double  mPort1DemandFlux;          /**< (kg*mol/s)                         input from simbus */
        double  mPort1DemandTemperature;   /**< (K)                                input from simbus */
        double  mPort0SupplyCapacitance;   /**< (kg*mol/kPa)                       output to simbus */
        double  mPort0SupplyPressure;      /**< (kPa)                              output to simbus */
        double  mPort0SupplyTemperature;   /**< (K)                                output to simbus */
        double* mPort0SupplyMassFractions; /**< (--)           trick_chkpnt_io(**) output to simbus */
        double  mPort1SupplyCapacitance;   /**< (kg*mol/kPa)                       output to simbus */
        double  mPort1SupplyPressure;      /**< (kPa)                              output to simbus */
        double  mPort1SupplyTemperature;   /**< (K)                                output to simbus */
        double* mPort1SupplyMassFractions; /**< (--)           trick_chkpnt_io(**) output to simbus */
        double  mEquivalentConductance;    /**< (kg*mol/kPa/s) trick_chkpnt_io(**) Estimated conductance of the external network */
        double  mPort0LeakConductance;     /**< (kg*mol/kPa/s) trick_chkpnt_io(**) Conductance to mimic external network leak */
        double  mPort1LeakConductance;     /**< (kg*mol/kPa/s) trick_chkpnt_io(**) Conductance to mimic external network leak */
        double  mLastPotentialDrop;        /**< (kPa)                              Last-pass value of link potential drop */
        static const double mNetworkCapacitanceFlux; /**< (kg*mol/s) Flux value to use in network node capacitance calculations */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Updates the admittance and source terms of the link
        virtual void buildConductance();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidEqConductor(const GunnsFluidEqConductor& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidEqConductor& operator =(const GunnsFluidEqConductor& that);
};

/// @}

#endif
