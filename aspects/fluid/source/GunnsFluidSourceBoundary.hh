#ifndef GunnsFluidSourceBoundary_EXISTS
#define GunnsFluidSourceBoundary_EXISTS

/**
@file
@brief    GUNNS Fluid Source Boundary Link declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_BOUNDARY    GUNNS Fluid Source Boundary Link
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Source Boundary link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidSourceBoundary.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2016-01) (Initial))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Source Boundary Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Source
///           Boundary configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSourceBoundaryConfigData : public GunnsFluidLinkConfigData
{
    public:
        bool mFlipFlowSign;       /**< (--) trick_chkpnt_io(**) True makes positive flow rate go out of the node. */
        bool mTraceCompoundsOnly; /**< (--) trick_chkpnt_io(**) True only flows trace compounds, not bulk fluid. */
        /// @brief Default constructs this Fluid Source Boundary configuration data.
        GunnsFluidSourceBoundaryConfigData(const std::string& name               = "",
                                           GunnsNodeList*     nodes              = 0,
                                           const bool         flipFlowSign       = false,
                                           const bool         traceCompoundsOnly = false);
        /// @brief Default destructs this Fluid Source Boundary configuration data.
        virtual ~GunnsFluidSourceBoundaryConfigData();
        /// @brief Copy constructs this Fluid Source Boundary configuration data.
        GunnsFluidSourceBoundaryConfigData(const GunnsFluidSourceBoundaryConfigData& that);

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSourceBoundaryConfigData& operator =(const GunnsFluidSourceBoundaryConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Source Boundary Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Source
///           Boundary input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSourceBoundaryInputData : public GunnsFluidLinkInputData
{
    public:
        double              mFlowDemand;    /**< (kg/s) trick_chkpnt_io(**) Initial demanded mass flow rate of the link. */
        PolyFluidInputData* mInternalFluid; /**< (--)   trick_chkpnt_io(**) Initial fluid properties of the link flow. */
        /// @brief    Default constructs this Fluid Source Boundary input data.
        GunnsFluidSourceBoundaryInputData(const bool          malfBlockageFlag  = false,
                                          const double        malfBlockageValue = 0.0,
                                          const double        flowDemand        = 0.0,
                                          PolyFluidInputData* internalFluid     = 0);
        /// @brief    Default destructs this Fluid Source Boundary input data.
        virtual ~GunnsFluidSourceBoundaryInputData();
        /// @brief    Copy constructs this Fluid Source Boundary input data.
        GunnsFluidSourceBoundaryInputData(const GunnsFluidSourceBoundaryInputData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSourceBoundaryInputData& operator =(const GunnsFluidSourceBoundaryInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Source Boundary class
///
/// @details  This link forces fluid with specified properties into or out of a single fluid node.
///           This can be used as a flow boundary condition at the attached node.  This link
///           controls the flow fluid mixture and temperature, allowing for specific fluid
///           components to flow independently of each other or the attached node properties.
///
///           This link is similar to GunnsFluidSource except for these key differences:
///           - This link is a 1-port link.
///           - It allows the sign convention of the mFlowDemand input to be reversed.
///           - It defines the internal fluid, whereas GunnsFluidSource doesn't.
///           - The internal fluid can contain trace compounds, making this link a source of trace
///             compounds to/from the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSourceBoundary : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSourceBoundary);

    public:
        /// @brief Default Constructor.
        GunnsFluidSourceBoundary();
        /// @brief Default Destructor.
        virtual ~GunnsFluidSourceBoundary();
        /// @brief Initialize
        void initialize(const GunnsFluidSourceBoundaryConfigData& configData,
                        const GunnsFluidSourceBoundaryInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0);
        /// @brief Step method for updating the link.
        virtual void   step(const double dt);
        /// @brief Method for computing the flows across the link.
        virtual void   computeFlows(const double dt);
        /// @brief Method for transporting the flows across the link.
        virtual void   transportFlows(const double dt);
        /// @brief Sets the flow demand for the link.
        virtual void   setFlowDemand(const double toFlowDemand);
        /// @brief Sets the flow state of the link.
        virtual void   setFlowState(PolyFluid* withState);
        /// @brief Returns the flow demand of the link.
        virtual double getFlowDemand() const;
        /// @brief Moves the internal fluid to or from the attached node.
        virtual void   transportFluid(const bool forcedOutflow,
                                      const int  fromPort = 0,
                                      const int  toPort   = 1);

    protected:
        bool    mFlipFlowSign;       /**< (--)   trick_chkpnt_io(**) True makes positive flow rate go out of the node. */
        bool    mTraceCompoundsOnly; /**< (--)   trick_chkpnt_io(**) True only flows trace compounds, not bulk fluid. */
        double  mFlowDemand;         /**< (kg/s)                     User demanded mass flow rate through the link. */
        double* mTraceCompoundRates; /**< (--)   trick_chkpnt_io(**) Source flow rate of the trace compounds relative to mFlowDemand. */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void   restartModel();
        /// @brief Builds the source vector terms of the links contribution to the network.
        virtual void   buildSource();
        /// @brief Validates the initialization of this GUNNS Fluid Source Boundary.
        void           validate(const GunnsFluidSourceBoundaryConfigData& configData,
                                const GunnsFluidSourceBoundaryInputData&  inputData) const;
        /// @brief Computes mPower for the aspect-specific implementation.
        virtual void   computePower();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSourceBoundary(const GunnsFluidSourceBoundary& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSourceBoundary& operator =(const GunnsFluidSourceBoundary& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double (kg/s) Returns the flow demand.
///
/// @details Returns the flow demand.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSourceBoundary::getFlowDemand() const
{
    return mFlowDemand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] toFlowDemand (kg/s) flow demand.
///
/// @details Sets the state of the fluid inside the fluid source link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSourceBoundary::setFlowDemand(const double toFlowDemand)
{
    mFlowDemand = toFlowDemand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] withState (--) Fluid State information.
///
/// @details Sets the state of the fluid inside the fluid source link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSourceBoundary::setFlowState(PolyFluid* withState)
{
    mInternalFluid->setState(withState);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Builds the source vector terms of the link's contribution to the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSourceBoundary::buildSource()
{
    mSourceVector[0] = mFlux;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Hydraulic power in the link is volumetric flow * potential drop.  Note that this is
///           positive for flow going into the node, since the link is adding power to the network.
///           Since potential is in units of kPa we must convert it to Pa to express power in Watts.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSourceBoundary::computePower()
{
    mPower = UnitConversion::PA_PER_KPA * mVolFlowRate * mPotentialVector[0];
}

#endif
