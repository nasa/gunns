#ifndef GunnsFluidFlowIntegrator_EXISTS
#define GunnsFluidFlowIntegrator_EXISTS

/**
@file
@brief     GUNNS Fluid Flow Integrator Spotter declarations

@defgroup  TSM_GUNNS_CORE_FLUID_FLOW_INTEGRATOR   GUNNS Fluid Flow Integrator Spotter
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Fluid Flow Integrator Spotter.
            This spotter is used to integrate flows through a link within a GUNNS network. It
            can be useful for finding total mass flown through a given link.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidFlowIntegrator.o))

PROGRAMMERS:
- ((Mike Moore) (L3) (2015-08) (Initial))

@{
*/

#include <vector>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsNetworkSpotter.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Flow Integrator Spotter Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Flow Integrator Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFlowIntegratorConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        /// @brief  Default constructs this GUNNS Fluid Flow Integrator Spotter configuration data.
        GunnsFluidFlowIntegratorConfigData(const std::string& name);
        /// @brief  Default destructs this GUNNS Fluid Flow Integrator Spotter configuration data.
        virtual ~GunnsFluidFlowIntegratorConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidFlowIntegratorConfigData(const GunnsFluidFlowIntegratorConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidFlowIntegratorConfigData& operator =(const GunnsFluidFlowIntegratorConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Flow Integrator Spotter Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Flow Integrator Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFlowIntegratorInputData : public GunnsNetworkSpotterInputData
{
    public:
        /// @brief  Default constructs this GUNNS Fluid Flow Integrator Spotter input data.
        GunnsFluidFlowIntegratorInputData();
        /// @brief  Default destructs this GUNNS Fluid Flow Integrator Spotter input data.
        virtual ~GunnsFluidFlowIntegratorInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidFlowIntegratorInputData(const GunnsFluidFlowIntegratorInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidFlowIntegratorInputData& operator =(const GunnsFluidFlowIntegratorInputData& that);
};

// Forward-declare the basic link class.
class GunnsFluidLink;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Flow Integrator Spotter Class.
///
/// @details  This spotter is used to integrate flows through a link within a GUNNS network. It
///           can be useful for finding total mass flown through a given link.
///
/// @note     This should only be used on links that meet these criteria:
///           - they do not change the mixture of the fluid passing through it,
///           - they update their mFlowRate as positive values flowing from their port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidFlowIntegrator : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidFlowIntegrator);
    public:
        /// @brief  Default Constructor
        GunnsFluidFlowIntegrator(const GunnsFluidLink& link);
        /// @brief   Default destructor.
        virtual     ~GunnsFluidFlowIntegrator();
        /// @brief   Initializes the GUNNS Fluid Flow Integrator Spotter with configuration and
        ///          input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief   Steps the GUNNS Fluid Flow Integrator Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief   Steps the GUNNS Fluid Flow Integrator Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief   Gets the integrated mass of the specified constituent index.
        double       getIntegratedMass(const int index) const;

    protected:
        const GunnsFluidLink&  mLink;        /**< *o (--) trick_chkpnt_io(**) Reference to the link of interest. */
        int      mNumFluidConstituents;      /**< *o (--) trick_chkpnt_io(**) Number of fluid constituents in the network. */
        double*  mMassFractionsPort0;        /**< *o (--) trick_chkpnt_io(**) Mass fractions of the fluid at port zero. */
        double*  mMassFractionsPort1;        /**< *o (--) trick_chkpnt_io(**) Mass fractions of the fluid at port one. */
        double*  mIntegratedMasses;          /**< *o (--) trick_chkpnt_io(**) Accumulated masses that have flown through the link. This is flow rate
                                                                              sign sensitive. Flow rate is positive from port 0 to 1. */

        /// @brief   Validates the supplied configuration data.
        const GunnsFluidFlowIntegratorConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsFluidFlowIntegratorInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief Deletion of allocated objects
        void         cleanup();

        /// @brief Helper method to grab the mass fractions out of the link's port0 and port1 nodes.
        void collectMassFractions();

        /// @brief Method that integrates the flow rate in order to compute the total fluid masses that have flown
        ///        through the link.
        void performIntegration(double* fractionsToUse, double flowRate, double dt);

        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidFlowIntegrator(const GunnsFluidFlowIntegrator& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidFlowIntegrator& operator =(const GunnsFluidFlowIntegrator& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  index  (--)  Index of the fluid constituent to get the mass of.
///
/// @returns  double  (kg)  Integrated mass of the specified constituent flowed through the link.
///
/// @details  Returns the total mass of the specified fluid constituent that has flowed through the
///           link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidFlowIntegrator::getIntegratedMass(const int index) const
{
    return mIntegratedMasses[index];
}

#endif
