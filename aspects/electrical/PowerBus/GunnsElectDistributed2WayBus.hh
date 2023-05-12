#ifndef GunnsElectDistributed2WayBus_EXISTS
#define GunnsElectDistributed2WayBus_EXISTS

/**
@file     GunnsElectDistributed2WayBus.hh
@brief    GUNNS Electrical Distributed 2-Way Bus Interface declarations

@defgroup  TSM_GUNNS_ELECTRICAL_2WAY_BUS    GUNNS Electrical Distributed 2-Way Bus Interface
@ingroup   TSM_GUNNS_ELECTRICAL

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Distributed 2-Way Bus Interface.)

REFERENCE:
- ((GUNNS Wiki: https://github.com/nasa/gunns/wiki/Distributed-Bi-Directional-Flow-Electrical-Interface))

ASSUMPTIONS AND LIMITATIONS:
- ((Only one side of the interface regulates the shared bus voltage at any given time.)
   (Role switching logic does not account for voltage drop from the supplies to this interface.)
   (Switching of flow direction and voltage control across the interface is limited by round-trip
    data transport time, and is generally much slower than real-world circuits.))

LIBRARY DEPENDENCY:
- ((GunnsElectDistributed2WayBus.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-03) (Initial))

@{
*/

#include <vector>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Distributed 2-Way Bus voltage supply data.
///
/// @details  This is a data structure to describe the ability of a voltage supply in the local
///           model to supply power to the interface location.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsElectDistributed2WayBusSupplyData
{
    public:
        bool  mAvailable;      /**< (1) True if the voltage regulation is available if needed. */
        float mMaximumVoltage; /**< (V) Maximum voltage that can be supplied, or ideal regulated voltage setpoint. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Distributed 2-Way Bus voltage interface data.
///
/// @details  This holds the data that is transferred between instances of
///           GunnsElectDistributed2WayBus across the sim-sim interface.  These terms map to the
///           HLA FOM.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsElectDistributed2WayBusInterfaceData
{
    public:
        unsigned int mFrameCount;    /**< (1) Frame count driven by this side. */
        unsigned int mFrameLoopback; /**< (1) Frame count driven by other side, echoed back. */
        bool         mDemandMode;    /**< (1) Demand mode flag. */
        float        mDemandPower;   /**< (W) Demanded power load. */
        float        mSupplyVoltage; /**< (V) Supplied voltage. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Distributed 2-Way Bus notification message.
///
/// @details  This describes a notification message from the GunnsElectDistributed2WayBus to the
///           outside, including severity level and message string.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectDistributed2WayBusNotification
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
        GunnsElectDistributed2WayBusNotification(const NotificationLevel level = NONE, const std::string& message = "");
        /// @brief  Default destructor.
        virtual ~GunnsElectDistributed2WayBusNotification();
        /// @brief  Copy constructor.
        GunnsElectDistributed2WayBusNotification(const GunnsElectDistributed2WayBusNotification& that);
        /// @brief  Assignment operator.
        GunnsElectDistributed2WayBusNotification& operator =(const GunnsElectDistributed2WayBusNotification& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Distributed 2-Way Bus Interface.
///
/// @details  See the GUNNS Wiki link in REFERENCE above.  Main features:
///           - Designed to be generic and reusable outside of GUNNS/Trick.
///           - No dependencies on any other GUNNS code or Trick code.
///           - Implements handshaking and coordination of bi-directional power flow between
///             distributed models.
///           - Interfaces with another instance of itself over the data interface (HLA, etc.)
///             - The side of the interface maintaining the bus voltage is the Supply role, supplies
///               voltage to the other side, and receives the other side's demanded power to be
///               placed as a load on this side.
///             - The side of the interface not maintaining the bus voltage is the Demand role,
///               receives the supplied voltage from the other side to be placed as a voltage
///               boundary condition on this side, and returns the resulting power load in the
///               voltage supply to the other side.
///             - The Supply role is determined as the side with the highest connected and available
///               voltage regulation.
///           - The local model registers 0 or more voltage supplies with this.  Each interface to
///             the local voltage supplies is a simple data container with values for:
///             - Regulated voltage,
///             - Available flag, meaning the regulator is enabled and has a flow path to this
///               interface.
///             - The local model drives these during runtime from the current state of each
///               respective voltage regulator model.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectDistributed2WayBus
{
    public:
        /// @brief  Enumeration of interface roles. */
        typedef enum {
            NONE   = 0, /**< No role. */
            SUPPLY = 1, /**< Supply role. */
            DEMAND = 2  /**< Demand role. */
        } Roles;
        /// @ this is the HLA-like interface, not the model, etc.
        GunnsElectDistributed2WayBusInterfaceData mInData;  /**< (1) Input data received from the remote side. */
        GunnsElectDistributed2WayBusInterfaceData mOutData; /**< (1) Output data to transmit to the remote side. */
        /// @brief  Default constructor.
        GunnsElectDistributed2WayBus();
        /// @brief  Default destructor.
        virtual ~GunnsElectDistributed2WayBus();
        //MODEL INTERFACE:
        /// @brief  Creates an interface for a voltage supply in the local model.
        GunnsElectDistributed2WayBusSupplyData* createSupplyData();
        /// @brief  Initializes this Distributed 2-Way Bus Interface.
        void initialize(const bool isPrimarySide = false, const float voltage = 0.0);
        /// @brief  Updates frame counters, should be called once per main model step.
        void updateFrameCounts();
        /// @brief  Updates the interface logic.
        void update(const float localVoltage, const float localPowerDemand);
        /// @brief  Forces this interface to remain in Demand role.
        void forceDemandRole();
        /// @brief  Forces this interface to remain in Supply role.
        void forceSupplyRole();
        /// @brief  Resets the forced role and lets the interface logic determine role normally.
        void resetForceRole();
        /// @brief  Returns whether this Distributed 2-Way Bus Interface is in the Demand role.
        bool isInDemandRole() const;
        /// @brief  Returns the power demand from the remote model to apply to the local model.
        float getRemoteLoad() const;
        /// @brief  Returns the voltage supply from the remote model to apply to the local model.
        float getRemoteSupply() const;
        /// @brief  Pops a notification message off of the queue and returns the remaining queue size.
        unsigned int popNotification(GunnsElectDistributed2WayBusNotification& notification);

    protected:
        bool                                                  mIsPrimarySide;   /**<    (1) trick_chkpnt_io(**) This is the primary side of the interface if true. */
        Roles                                                 mForcedRole;      /**<    (1)                     The role this interface is forced to be in, if any. */
        std::vector<GunnsElectDistributed2WayBusSupplyData*>  mSupplyDatas;     /**< ** (1) trick_chkpnt_io(**) Data objects for the local voltage supplies. */
        int                                                   mLoopLatency;     /**<    (1)                     Measured round-trip data loop latency, in number of model update steps. */
        int                                                   mFramesSinceFlip; /**< (1) Count of main model frames since our last mode flip. */
        std::vector<GunnsElectDistributed2WayBusNotification> mNotifications;   /**< ** (1) trick_chkpnt_io(**) Notifications queue. */
        /// @brief  Adds a notification message to the queue.
        void pushNotification(const GunnsElectDistributed2WayBusNotification::NotificationLevel level,
                              const std::string&                                                message);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsElectDistributed2WayBus(const GunnsElectDistributed2WayBus& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectDistributed2WayBus& operator =(const GunnsElectDistributed2WayBus& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsElectDistributed2WayBusSupplyData* (--) Address of the created supply data object.
///
/// @details  Creates a new GunnsElectDistributed2WayBusSupplyData and returns its address to the
///           caller.  Users should call this for each voltage supply in the local model that can
///           supply power to this interface location, and then drive the object to describe the
///           ability of the voltage supply to supply power during runtime.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsElectDistributed2WayBusSupplyData* GunnsElectDistributed2WayBus::createSupplyData()
{
    GunnsElectDistributed2WayBusSupplyData* newSupplyData = new GunnsElectDistributed2WayBusSupplyData();
    mSupplyDatas.push_back(newSupplyData);
    return newSupplyData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the mForcedRole attribute to DEMAND.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectDistributed2WayBus::forceDemandRole()
{
    mForcedRole = DEMAND;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the mForcedRole attribute to SUPPLY.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectDistributed2WayBus::forceSupplyRole()
{
    mForcedRole = SUPPLY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the mForcedRole attribute to NONE.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectDistributed2WayBus::resetForceRole()
{
    mForcedRole = NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (1) True if this is currently in the Demand role.
///
/// @details  Returns the value of mOutData.mDemandMode.  When in Demand role (returned value = 1),
///           this side of the interface acts as a constant-power load on the local model, and the
///           local model should call getRemoteLoad() for the load amount.  When in Supply role,
///           this side of the interface acts as a voltage supply to the local model, which can be
///           characterized by calling getRemoteSupply().
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectDistributed2WayBus::isInDemandRole() const
{
    return mOutData.mDemandMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  float (W) Power demand on the local model from the remote model.
///
/// @details  If in the Supply role, returns the demanded power from the remote model, otherwise
///           returns zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline float GunnsElectDistributed2WayBus::getRemoteLoad() const
{
    if (mOutData.mDemandMode) {
        return 0.0;
    }
    return mInData.mDemandPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  float (V) Voltage supplied to the local model from the remote model.
///
/// @details  Returns the supplied voltage from the remote model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline float GunnsElectDistributed2WayBus::getRemoteSupply() const
{
    return mInData.mSupplyVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] level   (--) Severity level of the message.
/// @param[in] message (--) Detailed message string.
///
/// @details  Adds a new notification message object to the message queue.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectDistributed2WayBus::pushNotification(
        const GunnsElectDistributed2WayBusNotification::NotificationLevel level,
        const std::string&                                                message)
{
    mNotifications.push_back(GunnsElectDistributed2WayBusNotification(level, message));
}

#endif
