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

#include "core/GunnsDistributed2WayBusBase.hh"

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
///           GunnsElectDistributed2WayBus across the sim-sim interface.  The class variables,
///           including the base class variables, map to the HLA FOM.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectDistributed2WayBusInterfaceData : public GunnsDistributed2WayBusBaseInterfaceData
{
    public:
        float mDemandPower;   /**< (W) Demanded power load. */
        float mSupplyVoltage; /**< (V) Supplied voltage. */
        /// @brief  Default constructs this Electrical Distributed 2-Way Bus interface data.
        GunnsElectDistributed2WayBusInterfaceData();
        /// @brief  Default destructs this Electrical Distributed 2-Way Bus interface data.
        virtual ~GunnsElectDistributed2WayBusInterfaceData();
        /// @brief  Returns whether this object has received valid data.
        virtual bool hasValidData() const;
        /// @brief Assignment operator for this Electrical Distributed 2-Way Bus interface data.
        GunnsElectDistributed2WayBusInterfaceData& operator =(const GunnsElectDistributed2WayBusInterfaceData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectDistributed2WayBusInterfaceData(const GunnsElectDistributed2WayBusInterfaceData&);
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
class GunnsElectDistributed2WayBus : public GunnsDistributed2WayBusBase
{
    public:
        GunnsElectDistributed2WayBusInterfaceData mInData;  /**< (1) Input data received from the remote side. */
        GunnsElectDistributed2WayBusInterfaceData mOutData; /**< (1) Output data to transmit to the remote side. */
        /// @brief  Default constructor.
        GunnsElectDistributed2WayBus();
        /// @brief  Default destructor.
        virtual ~GunnsElectDistributed2WayBus();
        /// @brief  Creates an interface for a voltage supply in the local model.
        GunnsElectDistributed2WayBusSupplyData* createSupplyData();
        /// @brief  Initializes this Distributed 2-Way Bus Interface.
        void initialize(const bool isPrimarySide = false, const float voltage = 0.0);
        /// @brief  Updates frame counters, should be called once per main model step.
        void processInputs();
        /// @brief  Updates the interface logic.
        void update(const float localVoltage, const float localPowerDemand);
        /// @brief  Returns the power demand from the remote model to apply to the local model.
        float getRemoteLoad() const;
        /// @brief  Returns the voltage supply from the remote model to apply to the local model.
        float getRemoteSupply() const;

    protected:
        std::vector<GunnsElectDistributed2WayBusSupplyData*> mSupplyDatas; /**< ** (1) trick_chkpnt_io(**) Data objects for the local voltage supplies. */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsElectDistributed2WayBus(const GunnsElectDistributed2WayBus& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectDistributed2WayBus& operator =(const GunnsElectDistributed2WayBus& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if all data validation checks passed.
///
/// @details  Checks for all of the following conditions to be met:  Frame count > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectDistributed2WayBusInterfaceData::hasValidData() const
{
    return (mFrameCount > 1);
}

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

#endif
