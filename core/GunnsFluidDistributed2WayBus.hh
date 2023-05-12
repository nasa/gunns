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

#include "GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed Fluid Mixture Data
///
/// @details  This describes the intrinsic properties (energy & mixture) of a fluid for
///           communication across a distributed modeling interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributedMixtureData
{
    public:
        double  mEnergy;          /**< (1)                     Fluid temperature (K) or specific enthalpy (J/kg). */
        double* mMoleFractions;   /**< (1) trick_chkpnt_io(**) Fluid mole fractions. */
        double* mTcMoleFractions; /**< (1) trick_chkpnt_io(**) Trace compounds mole fractions. */
        /// @brief  Default constructs this Fluid Distributed mixture data.
        GunnsFluidDistributedMixtureData();
        /// @brief  Default destructs this Fluid Distributed mixture data.
        virtual ~GunnsFluidDistributedMixtureData();
        /// @brief  Allocates dynamic arrays for bulk fluid and trace compounds and mole fractions.
        void initialize(const unsigned int nBulk,
                        const unsigned int nTc,
                        const unsigned int nIfBulk,
                        const unsigned int nIfTc);
        /// @brief  Sets the bulk fluid mole fractions to the given values.
        void setMoleFractions(const double* fractions);
        /// @brief  Sets the trace compound mole fractions to the given values.
        void setTcMoleFractions(const double* fractions);
        /// @brief  Sets the given array to the bulk fluid mole fraction values.
        void getMoleFractions(double* fractions) const;
        /// @brief  Sets the given array to the trace compound mole fraction values.
        void getTcMoleFractions(double* fractions) const;

    protected:
        unsigned int mNumFluid;       /**< *o (1) trick_chkpnt_io(**) Number of primary fluid compounds in model. */
        unsigned int mNumTc;          /**< *o (1) trick_chkpnt_io(**) Number of trace compounds in model. */
        unsigned int mNumFluidIf;     /**< *o (1) trick_chkpnt_io(**) Number of primary fluid compounds in interface. */
        unsigned int mNumTcIf;        /**< *o (1) trick_chkpnt_io(**) Number of trace compounds in interface. */
        unsigned int mNumFluidCommon; /**< *o (1) trick_chkpnt_io(**) Number of primary fluid compounds in common between model and interface. */
        unsigned int mNumTcCommon;    /**< *o (1) trick_chkpnt_io(**) Number of trace compounds in common between model and interface. */

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributedMixtureData& operator =(const GunnsFluidDistributedMixtureData& that);
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
class GunnsFluidDistributed2WayBusInterfaceData : public GunnsFluidDistributedMixtureData
{
    public:
        unsigned int mFrameCount;      /**< (1)      Frame count driven by this side. */
        unsigned int mFrameLoopback;   /**< (1)      Frame count driven by other side, echoed back. */
        bool         mDemandMode;      /**< (1)      Demand mode flag. */
        double       mCapacitance;     /**< (mol/Pa) Model capacitance. */
        double       mSource;          /**< (1)      Fluid pressure (Pa) or molar flow (mol/s). */
        /// @brief  Default constructs this Fluid Distributed 2-Way Bus interface data.
        GunnsFluidDistributed2WayBusInterfaceData();
        /// @brief  Default destructs this Fluid Distributed 2-Way Bus interface data.
        virtual ~GunnsFluidDistributed2WayBusInterfaceData();
        /// @brief  Returns whether this object has received valid data.
        bool hasValidData() const;
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
        /// @brief  Default destructor.
        virtual ~GunnsFluidDistributed2WayBusFluidState();

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBusFluidState& operator =(const GunnsFluidDistributed2WayBusFluidState& that);
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
        /// @brief  Default destructor.
        virtual ~GunnsFluidDistributed2WayBusFlowState();

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBusFlowState& operator =(const GunnsFluidDistributed2WayBusFlowState& that);
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBusFlowState(const GunnsFluidDistributed2WayBusFlowState&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Distributed 2-Way Bus notification message.
///
/// @details  This describes a notification message from the GunnsFluidDistributed2WayBus to the
///           outside, including severity level and message string.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributed2WayBusNotification
{
    public:
        typedef enum {
            INFO = 0, ///< Information.
            WARN = 1, ///< Warning.
            ERR  = 2, ///< Error.
            NONE = 3, ///< No notification, empty queue.
        } NotificationLevel;
        NotificationLevel mLevel;   /**< (1) The severity level of the notification. */
        std::string       mMessage; /**< (1) the notification message. */
        /// @brief  Default constructor.
        GunnsFluidDistributed2WayBusNotification(const NotificationLevel level = NONE, const std::string& message = "");
        /// @brief  Default destructor.
        virtual ~GunnsFluidDistributed2WayBusNotification();
        /// @brief  Copy constructor.
        GunnsFluidDistributed2WayBusNotification(const GunnsFluidDistributed2WayBusNotification& that);
        /// @brief  Assignment operator.
        GunnsFluidDistributed2WayBusNotification& operator =(const GunnsFluidDistributed2WayBusNotification& that);
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
class GunnsFluidDistributed2WayBus
{
    public:
        /// @brief  Enumeration of interface roles. */
        typedef enum {
            NONE   = 0, /**< No role. */
            SUPPLY = 1, /**< Supply role. */
            DEMAND = 2  /**< Demand role. */
        } Roles;
        GunnsFluidDistributed2WayBusInterfaceData mInData;  /**< (1) Input data received from the remote side. */
        GunnsFluidDistributed2WayBusInterfaceData mOutData; /**< (1) Output data to transmit to the remote side. */
        /// @brief Default Constructor.
        GunnsFluidDistributed2WayBus();
        /// @brief Default Destructor.
        virtual ~GunnsFluidDistributed2WayBus();
        /// @brief Initializes this Distributed 2-Way Bus Interface.
        void initialize(const bool         isPairMaster,
                        const unsigned int nBulk,
                        const unsigned int nTc,
                        const unsigned int nIfBulk,
                        const unsigned int nIfTc);
        /// @brief Process received data prior to the model update.
        virtual void processInputs();
        /// @brief Returns the limit on max flow between the Demand-side model and the interface volume.
        double computeDemandLimit(const double timestep, const double demandSidePressure);
        /// @brief Returns the stability filter gain used in the Demand-side flow rate limit.
        double getDemandLimitGain() const;
        /// @brief Special processing of data outputs from the model after the network update.
        virtual void processOutputs(const double capacitance);
        /// @brief Forces this interface to remain in Demand role.
        void forceDemandRole();
        /// @brief Forces this interface to remain in Supply role.
        void forceSupplyRole();
        /// @brief Resets the forced role and lets the interface logic determine role normally.
        void resetForceRole();
        /// @brief Returns whether this Distributed 2-Way Bus Interface is in the Demand role.
        bool isInDemandRole() const;
        /// @brief Sets the outgoing fluid state of the interface volume when in the Supply role.
        void setFluidState(const GunnsFluidDistributed2WayBusFluidState& fluid);
        /// @brief Gets the incoming fluid state of the interface volume when in the Demand role.
        bool getFluidState(GunnsFluidDistributed2WayBusFluidState& fluid) const;
        /// @brief Sets the outgoing state of flows to/from the interface volume when in the Demand role.
        void setFlowState(const GunnsFluidDistributed2WayBusFlowState& flow);
        /// @brief Gets the incoming state of flows to/from the interface volume when in the Supply role.
        bool getFlowState(GunnsFluidDistributed2WayBusFlowState& flow) const;
        /// @brief Pops a notification message off of the queue and returns the remaining queue size.
        unsigned int popNotification(GunnsFluidDistributed2WayBusNotification& notification);

    protected:
        bool                                                  mIsPairMaster;           /**<    (1)     trick_chkpnt_io(**) This is the master side of the interface. */
        bool                                                  mInDataLastDemandMode;   /**<    (1)                         Last-pass demand mode from the other paired link. */
        int                                                   mFramesSinceFlip;        /**<    (1)                         Number of frames since the last mode flip. */
        int                                                   mLoopLatency;            /**<    (1)                         Round-trip loop data lag measurement. */
        double                                                mDemandLimitGain;        /**<    (1)                         The current Demand-side flow rate limit filter gain. */
        double                                                mDemandLimitFlowRate;    /**<    (mol/s)                     The current Demand-side flow rate limit. */
        Roles                                                 mForcedRole;             /**<    (1)                         The role this interface is forced to be in, if any. */
        std::vector<GunnsFluidDistributed2WayBusNotification> mNotifications;          /**< ** (1)     trick_chkpnt_io(**) Notifications queue. */
        static const double                                   mModingCapacitanceRatio; /**<    (1)     trick_chkpnt_io(**) Supply over Demand capacitance ratio for triggering mode flip. */
        static const double                                   mDemandFilterConstA;     /**< ** (1)     trick_chkpnt_io(**) Demand filter gain constant A. */
        static const double                                   mDemandFilterConstB;     /**< ** (1)     trick_chkpnt_io(**) Demand filter gain constant B. */
        /// @brief Handles several mode flip cases based on input data.
        void flipModesOnInput();
        /// @brief Flips to the Demand mode.
        void flipToDemandMode();
        /// @brief Flips to the Supply mode.
        void flipToSupplyMode();
        /// @brief Flips to the Demand mode based on capacitances.
        void flipModesOnCapacitance();
        /// @brief  Adds a notification message to the queue.
        void pushNotification(const GunnsFluidDistributed2WayBusNotification::NotificationLevel level,
                              const std::string&                                                message);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBus(const GunnsFluidDistributed2WayBus& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributed2WayBus& operator =(const GunnsFluidDistributed2WayBus& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the mForcedRole attribute to DEMAND.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidDistributed2WayBus::forceDemandRole()
{
    mForcedRole = DEMAND;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the mForcedRole attribute to SUPPLY.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidDistributed2WayBus::forceSupplyRole()
{
    mForcedRole = SUPPLY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the mForcedRole attribute to NONE.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidDistributed2WayBus::resetForceRole()
{
    mForcedRole = NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (1) True if this interface is currently in the Demand role.
///
/// @details  Returns the value of mOutData.mDemandMode.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsFluidDistributed2WayBus::isInDemandRole() const
{
    return mOutData.mDemandMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Returns the value of mDemandLimitGain.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidDistributed2WayBus::getDemandLimitGain() const
{
    return mDemandLimitGain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] level   (--) Severity level of the message.
/// @param[in] message (--) Detailed message string.
///
/// @details  Adds a new notification message object to the message queue.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidDistributed2WayBus::pushNotification(
        const GunnsFluidDistributed2WayBusNotification::NotificationLevel level,
        const std::string&                                                message)
{
    mNotifications.push_back(GunnsFluidDistributed2WayBusNotification(level, message));
}

#endif
