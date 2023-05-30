#ifndef GunnsFluidDistributed2WayBus_EXISTS
#define GunnsFluidDistributed2WayBus_EXISTS

/**
@file     GunnsFluidDistributed2WayBus.hh
@brief    GUNNS Fluid Distributed 2-Way Bus Interface declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_2WAY_BUS    GUNNS Fluid Distributed 2-Way Bus Interface
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Distributed 2-Way Bus Interface.)

REFERENCE:
- ((GUNNS Wiki: https://github.com/nasa/gunns/wiki/Distributed-Bi-Directional-Flow-Fluid-Interface))

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidDistributed2WayBus.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-05) (Initial))

@{
*/

#include "GunnsDistributed2WayBusBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed Fluid Mixture Data
///
/// @details  This describes the intrinsic properties (energy & mixture) of a fluid for
///           communication across a distributed modeling interface.
///
/// @note     This must remain a base class, since it it used in multiple inheritance below, to
///           avoid the diamond inheritance problem.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributedMixtureData // !!! NOTE this must remain a base class, see above !!!
{
    public:
        double  mEnergy;          /**< (1)                     Fluid temperature (K) or specific enthalpy (J/kg). */
        double* mMoleFractions;   /**< (1) trick_chkpnt_io(**) Fluid mole fractions. */
        double* mTcMoleFractions; /**< (1) trick_chkpnt_io(**) Trace compounds mole fractions. */
        /// @brief  Default constructs this Fluid Distributed mixture data.
        GunnsFluidDistributedMixtureData();
        /// @brief  Default destructs this Fluid Distributed mixture data.
        virtual ~GunnsFluidDistributedMixtureData();
        /// @brief  Assignment operator for this Fluid Distributed mixture data.
        GunnsFluidDistributedMixtureData& operator =(const GunnsFluidDistributedMixtureData& that);
        /// @brief  Allocates dynamic arrays for bulk fluid and trace compounds and mole fractions.
        virtual void initialize(const unsigned int nBulk,
                                const unsigned int nTc,
                                const std::string& name = "");
        /// @brief  Sets the bulk fluid mole fractions to the given values.
        void setMoleFractions(const double* fractions, const unsigned int size);
        /// @brief  Sets the trace compound mole fractions to the given values.
        void setTcMoleFractions(const double* fractions, const unsigned int size);
        /// @brief  Sets the given array to the bulk fluid mole fraction values.
        void getMoleFractions(double* fractions, const unsigned int size) const;
        /// @brief  Sets the given array to the trace compound mole fraction values.
        void getTcMoleFractions(double* fractions, const unsigned int size) const;
        /// @brief  Returns the size of this mixture data bulk fluid mole fractions array.
        const unsigned int getNumFluid() const;
        /// @brief  Returns the size of this mixture data trace compounds mole fractions array.
        const unsigned int getNumTc() const;

    protected:
        unsigned int mNumFluid;       /**< *o (1) trick_chkpnt_io(**) Number of primary fluid compounds in this mixture. */
        unsigned int mNumTc;          /**< *o (1) trick_chkpnt_io(**) Number of trace compounds in this mixture. */

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributedMixtureData(const GunnsFluidDistributedMixtureData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed Fluid 2-Way Bus Interface Data
///
/// @details  This holds the data that is transferred between instances of
///           GunnsFluidDistributed2WayBus across the sim-sim interface (HLA, etc.)  The class
///           variables, including the base class variables, map to the HLA FOM.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributed2WayBusInterfaceData : public GunnsFluidDistributedMixtureData, public GunnsDistributed2WayBusBaseInterfaceData
{
    public:
        double mCapacitance; /**< (mol/Pa) Model capacitance. */
        double mSource;      /**< (1)      Fluid pressure (Pa) or molar flow (mol/s). */
        /// @brief  Default constructs this Fluid Distributed 2-Way Bus interface data.
        GunnsFluidDistributed2WayBusInterfaceData();
        /// @brief  Default destructs this Fluid Distributed 2-Way Bus interface data.
        virtual ~GunnsFluidDistributed2WayBusInterfaceData();
        /// @brief  Returns whether this object has received valid data.
        virtual bool hasValidData() const;
        /// @brief Assignment operator for this Fluid Distributed 2-Way Bus interface data.
        GunnsFluidDistributed2WayBusInterfaceData& operator =(const GunnsFluidDistributed2WayBusInterfaceData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBusInterfaceData(const GunnsFluidDistributed2WayBusInterfaceData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed Fluid State Data
///
/// @details  This describes the properties of a fluid state (pressure, energy & mixture) for
///           communication across a distributed modeling interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributed2WayBusFluidState : public GunnsFluidDistributedMixtureData
{
    public:
        double mPressure; /**< (Pa) Fluid pressure. */
        /// @brief Default constructor.
        GunnsFluidDistributed2WayBusFluidState();
        /// @brief Default destructor.
        virtual ~GunnsFluidDistributed2WayBusFluidState();
        /// @brief Assignment operator.
        GunnsFluidDistributed2WayBusFluidState& operator =(const GunnsFluidDistributed2WayBusFluidState& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBusFluidState(const GunnsFluidDistributed2WayBusFluidState&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed Fluid Flow State Data
///
/// @details  This describes the properties of a fluid flow (flow rate, energy & mixture) for
///           communication across a distributed modeling interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributed2WayBusFlowState : public GunnsFluidDistributedMixtureData
{
    public:
        double  mFlowRate; /**< (mol/s) Fluid molar flow rate. */
        /// @brief Default constructor.
        GunnsFluidDistributed2WayBusFlowState();
        /// @brief Default destructor.
        virtual ~GunnsFluidDistributed2WayBusFlowState();
        /// @brief Assignment operator.
        GunnsFluidDistributed2WayBusFlowState& operator =(const GunnsFluidDistributed2WayBusFlowState& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBusFlowState(const GunnsFluidDistributed2WayBusFlowState&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Distributed 2-Way Bus Interface.
///
/// @details  See the GUNNS Wiki link in REFERENCE above.  Main features:
///           - Designed to be generic and reusable outside of GUNNS/Trick.
///           - No dependencies on any other GUNNS code or Trick code.
///           - Implements handshaking and coordination of bi-directional fluid flow between
///             distributed models.
///           - Interfaces with another instance of itself over the data interface (HLA, etc.)
///             - The side of the interface having the larger fluid capacitance is usually the
///               Supply role, supplies the Fluid State (pressure, energy, mixture) in the
///               interface volume to the other side, and receives the other side's demanded Flow
///               State (flow rate, energy, mixture) to be placed as a flow source on this side.
///             - The side of the interface having the smaller fluid capacitance is usually the
///               Demand role, receives the supplied Fluid State from the other side to be placed as
///               a boundary condition on this side, and returns the resulting flow rate between the
///               interface location and the local model as a Flow State demanded to the other side.
///             - The roles flip when the Supply side's capacitance drops to a certain threshold
///               below the Demand side's capacitance.  This helps ensure stability.
///             - The Demand role, which computes the Flow State across the interface, applies
///               limits on the flow rate to ensure stability of the interface.
///           - Depending on the interface Supply or Demand role, the local model receives either a
///             Fluid State or Flow State boundary condition to apply at the interface location.
///           - The local model continuously computes fluid capacitance (the ratio of delta-moles
///             over delta-pressure) and gives this to this interface.
///           - Depending on the interface Supply or Demand role, the local model either supplies
///             the local Fluid State or the interface volume or the Flow State of flows to/from the
///             volume.
///
///           The order of operations between the local model and this object during each model step
///           should be:
///           1.  this->mInData updated with incoming data from the interface data network.
///           2.  Model calls this->processInputs()
///           3.  Model calls this->isInDemandRole(), responds to role swap as needed.
///           4.  Model calls this->getFluidState() or getFlowState() based on role.
///           5.  Model applies the returned fluid or flow state boundary conditions to its interface
///               volume.
///           6.  If in Demand role, model calls this->computeDemandLimit()
///           7.  Model does its main update.  If in Demand role, applies the demand flow rate limit.
///           8.  Model calculates its local capacitance at the interface.
///           9.  Model calls this->setFluidState() or setFlowState() based on role.
///           10. Model calls this->processOutputs()
///           11. Model calls this->isInDemandRole(), responds to role swap as needed.
///           12. Model calls this->popNotification() recursively until no notifications left,
///               transfers notification to the sim's messaging system as desired.
///           13. this->mOutData transmitted across the interface data network to the other side.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributed2WayBus : public GunnsDistributed2WayBusBase
{
    public:
        GunnsFluidDistributed2WayBusInterfaceData mInData;  /**< (1) Input data received from the remote side. */
        GunnsFluidDistributed2WayBusInterfaceData mOutData; /**< (1) Output data to transmit to the remote side. */
        /// @brief Default Constructor.
        GunnsFluidDistributed2WayBus();
        /// @brief Default Destructor.
        virtual ~GunnsFluidDistributed2WayBus();
        /// @brief Initializes this Distributed 2-Way Bus Interface.
        void initialize(const bool         isPairMaster,
                        const unsigned int nIfBulk,
                        const unsigned int nIfTc);
        /// @brief Process received data prior to the model update.
        void processInputs();
        /// @brief Returns the limit on max flow between the Demand-side model and the interface volume.
        double computeDemandLimit(const double timestep, const double demandSidePressure);
        /// @brief Returns the stability filter gain used in the Demand-side flow rate limit.
        double getDemandLimitGain() const;
        /// @brief Special processing of data outputs from the model after the network update.
        void processOutputs(const double capacitance);
        /// @brief Sets the outgoing fluid state of the interface volume when in the Supply role.
        void setFluidState(const GunnsFluidDistributed2WayBusFluidState& fluid);
        /// @brief Gets the incoming fluid state of the interface volume when in the Demand role.
        bool getFluidState(GunnsFluidDistributed2WayBusFluidState& fluid) const;
        /// @brief Sets the outgoing state of flows to/from the interface volume when in the Demand role.
        void setFlowState(const GunnsFluidDistributed2WayBusFlowState& flow);
        /// @brief Gets the incoming state of flows to/from the interface volume when in the Supply role.
        bool getFlowState(GunnsFluidDistributed2WayBusFlowState& flow) const;

    protected:
        double              mDemandLimitGain;        /**<    (1)                         The current Demand-side flow rate limit filter gain. */
        double              mDemandLimitFlowRate;    /**<    (mol/s)                     The current Demand-side flow rate limit. */
        static const double mModingCapacitanceRatio; /**<    (1)     trick_chkpnt_io(**) Supply over Demand capacitance ratio for triggering mode flip. */
        static const double mDemandFilterConstA;     /**< ** (1)     trick_chkpnt_io(**) Demand filter gain constant A. */
        static const double mDemandFilterConstB;     /**< ** (1)     trick_chkpnt_io(**) Demand filter gain constant B. */
        /// @brief Handles several mode flip cases based on input data.
        void flipModesOnInput();
        /// @brief Flips to the Demand mode.
        void flipToDemandMode();
        /// @brief Flips to the Supply mode.
        void flipToSupplyMode();
        /// @brief Flips to the Demand mode based on capacitances.
        void flipModesOnCapacitance();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBus(const GunnsFluidDistributed2WayBus& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBus& operator =(const GunnsFluidDistributed2WayBus& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns the value of mNumFluid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const unsigned int GunnsFluidDistributedMixtureData::getNumFluid() const
{
    return mNumFluid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns the value of mNumTc.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const unsigned int GunnsFluidDistributedMixtureData::getNumTc() const
{
    return mNumTc;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns the value of mDemandLimitGain.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidDistributed2WayBus::getDemandLimitGain() const
{
    return mDemandLimitGain;
}

#endif
