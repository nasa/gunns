#ifndef Distributed2WayBusElect_EXISTS
#define Distributed2WayBusElect_EXISTS

/**
@file     Distributed2WayBusElect.hh
@brief    Electrical Distributed 2-Way Bus Interface declarations

@defgroup  TSM_INTEROP_DISTR_2WAY_BUS_ELECTRICAL    Electrical Distributed 2-Way Bus Interface
@ingroup   TSM_INTEROP

@copyright Copyright 2025 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Electrical Distributed 2-Way Bus Interface.)

REFERENCE:
- ((https://github.com/nasa/gunns/wiki/Distributed-Bi-Directional-Flow-Electrical-Interface))

ASSUMPTIONS AND LIMITATIONS:
- ((Only one side of the interface regulates the shared bus voltage at any given time.)
   (Role switching logic does not account for voltage drop from the supplies to this interface.)
   (Switching of flow direction and voltage control across the interface is limited by round-trip
    data transport time, and is generally much slower than real-world circuits.))

LIBRARY DEPENDENCY:
- ((Distributed2WayBusElect.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2023-03) (Initial))

@{
*/

#include "Distributed2WayBusBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Distributed 2-Way Bus voltage supply data.
///
/// @details  This is a data structure to describe the ability of a voltage supply in the local
///           model to supply power to the interface location.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Distributed2WayBusElectSupplyData
{
    public:
        bool  mAvailable;      /**< (1) True if the voltage regulation is available if needed. */
        float mMaximumVoltage; /**< (V) Maximum voltage that can be supplied, or ideal regulated voltage setpoint. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Distributed 2-Way Bus voltage interface data.
///
/// @details  This holds the data that is transferred between instances of
///           Distributed2WayBusElect across the sim-sim interface.  The class variables,
///           including the base class variables, map to the HLA FOM.
////////////////////////////////////////////////////////////////////////////////////////////////////
class Distributed2WayBusElectInterfaceData : public Distributed2WayBusBaseInterfaceData
{
    public:
        float mDemandPower;   /**< (W) Demanded power load. */
        float mSupplyVoltage; /**< (V) Supplied voltage. */
        /// @brief  Default constructs this Electrical Distributed 2-Way Bus interface data.
        Distributed2WayBusElectInterfaceData();
        /// @brief  Default destructs this Electrical Distributed 2-Way Bus interface data.
        virtual ~Distributed2WayBusElectInterfaceData();
        /// @brief  Returns whether this object has received valid data.
        virtual bool hasValidData() const;
        /// @brief Assignment operator for this Electrical Distributed 2-Way Bus interface data.
        Distributed2WayBusElectInterfaceData& operator =(const Distributed2WayBusElectInterfaceData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        Distributed2WayBusElectInterfaceData(const Distributed2WayBusElectInterfaceData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Distributed 2-Way Bus Interface.
///
/// @details  See the reference above.  Main features:
///           - Designed to be generic and reusable.
///           - No dependencies on any 3rd-party code.
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
class Distributed2WayBusElect : public Distributed2WayBusBase
{
    public:
        Distributed2WayBusElectInterfaceData mInData;  /**< (1) Input data received from the remote side. */
        Distributed2WayBusElectInterfaceData mOutData; /**< (1) Output data to transmit to the remote side. */
        /// @brief  Default constructor.
        Distributed2WayBusElect();
        /// @brief  Default destructor.
        virtual ~Distributed2WayBusElect();
        /// @brief  Creates an interface for a voltage supply in the local model.
        Distributed2WayBusElectSupplyData* createSupplyData();
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
        std::vector<Distributed2WayBusElectSupplyData*> mSupplyDatas; /**< ** (1) trick_chkpnt_io(**) Data objects for the local voltage supplies. */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        Distributed2WayBusElect(const Distributed2WayBusElect& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        Distributed2WayBusElect& operator =(const Distributed2WayBusElect& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  (--)  True if all data validation checks passed.
///
/// @details  Checks for all of the following conditions to be met:  Frame count > 0.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Distributed2WayBusElectInterfaceData::hasValidData() const
{
    return (mFrameCount > 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  Distributed2WayBusElectSupplyData* (--) Address of the created supply data object.
///
/// @details  Creates a new Distributed2WayBusElectSupplyData and returns its address to the
///           caller.  Users should call this for each voltage supply in the local model that can
///           supply power to this interface location, and then drive the object to describe the
///           ability of the voltage supply to supply power during runtime.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline Distributed2WayBusElectSupplyData* Distributed2WayBusElect::createSupplyData()
{
    Distributed2WayBusElectSupplyData* newSupplyData = new Distributed2WayBusElectSupplyData();
    mSupplyDatas.push_back(newSupplyData);
    return newSupplyData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  float (W) Power demand on the local model from the remote model.
///
/// @details  If in the Supply role, returns the demanded power from the remote model, otherwise
///           returns zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline float Distributed2WayBusElect::getRemoteLoad() const
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
inline float Distributed2WayBusElect::getRemoteSupply() const
{
    return mInData.mSupplyVoltage;
}

#endif
