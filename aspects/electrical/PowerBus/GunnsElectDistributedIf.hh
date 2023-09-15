#ifndef GunnsElectDistributedIf_EXISTS
#define GunnsElectDistributedIf_EXISTS

/**
@file      GunnsElectDistributedIf.hh
@brief     GUNNS Electrical Distributed Bi-Directional Interface Link declarations.

@defgroup  TSM_GUNNS_ELECTRICAL_2WAY_LINK    Distributed Bi-Directional Interface Link
@ingroup   TSM_GUNNS_ELECTRICAL

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Distributed 2-Way Bus Interface Link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectDistributedIf.o))

PROGRAMMERS:
- (
   (Jason Harvey) (CACI) (2023-03) (Initial)
  )

@{
*/

#include "core/GunnsBasicLink.hh"
#include "GunnsElectDistributed2WayBus.hh"
#include "aspects/electrical/Converter/GunnsElectConverterOutput.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed Bi-Directional Interface Link Configuration Data
///
/// @details  This provides a data structure for the Distributed Bi-Directional Interface Link
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectDistributedIfConfigData : public GunnsBasicLinkConfigData
{
    public:
        bool                                          mIsPairPrimary;     /**<    (1)     trick_chkpnt_io(**) This is the primary side of the interface, defaults to Supply role. */
        double                                        mConductance;       /**<    (1/ohm) trick_chkpnt_io(**) Output channel conductance of the internal voltage source. */
        double                                        mNetCapDvThreshold; /**<    (V)     trick_chkpnt_io(**) Network capacitance delta-voltage threshold. */
        std::vector<const GunnsElectConverterOutput*> mSupplies;          /**< ** (1)     trick_chkpnt_io(**) Pointers to the voltage supplies in the network. */
        /// @brief Default constructs this Distributed Bi-Directional Interface configuration data.
        GunnsElectDistributedIfConfigData(const std::string& name              = "",
                                          GunnsNodeList*     nodes             = 0,
                                          const bool         isPairPrimary     = false,
                                          const double       conductance       = 0.0,
                                          const double       netCapDvThreshold = 0.0);
        /// @brief Copy constructs this Distributed Bi-Directional Interface configuration data.
        GunnsElectDistributedIfConfigData(const GunnsElectDistributedIfConfigData& that);
        /// @brief Default destructs this Distributed Bi-Directional Interface configuration data.
        virtual ~GunnsElectDistributedIfConfigData();
        /// @brief Registers the given converter link as a voltage supplier to the interface.
        void addSupply(const GunnsElectConverterOutput* supply);

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectDistributedIfConfigData& operator =(const GunnsElectDistributedIfConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed Bi-Directional Interface Link Input Data
///
/// @details  This provides a data structure for the Distributed Bi-Directional Interface Link
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectDistributedIfInputData : public GunnsBasicLinkInputData
{
    public:
        bool mForceDemandMode; /**< (--) trick_chkpnt_io(**) Forces the interface to always be in Demand mode. */
        bool mForceSupplyMode; /**< (--) trick_chkpnt_io(**) Forces the interface to always be in Supply mode. */
        /// @brief Default constructs this Distributed Bi-Directional Interface input data.
        GunnsElectDistributedIfInputData(const bool forceDemandMode = false,
                                         const bool forceSupplyMode = false);
        /// @brief Copy constructs this Distributed Bi-Directional Interface input data.
        GunnsElectDistributedIfInputData(const GunnsElectDistributedIfInputData& that);
        /// @brief Default destructs this Distributed Bi-Directional Interface input data.
        virtual ~GunnsElectDistributedIfInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectDistributedIfInputData& operator =(const GunnsElectDistributedIfInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Distributed Bi-Directional Interface Link Supply Data
///
/// @details  This provides a data structure to hold, for each registered voltage supply, its
///           interface data and additional GUNNS-specific data for debugging.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsElectDistributedIfSupplyData
{
    public:
        GunnsElectDistributed2WayBusSupplyData* mSupplyData; /**< (--) trick_chkpnt_io(**) The interface's supply data. */
        const GunnsElectConverterOutput*        mLink;       /**< (--) trick_chkpnt_io(**) Pointer to the supply link. */
        double                                  mNetCapDV;   /**< (V)  trick_chkpnt_io(**) The network capacitance delta-potential. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Distributed Bi-Directional Interface Link
///
/// @details  This wraps a GunnsElectDistributed2WayBus object in a network and provides all
///           interfaces between it and the network.  This contains a ConverterOutput and Input, for
///           applying the voltage source and power load effects on the network node, respectively,
///           and these are driven depending on the state of the interface.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectDistributedIf : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectDistributedIf);
    public:
        /// @name    Public objects and malfunction terms.
        /// @{
        /// @details The interface logic object and malfunction targets are public to allow access
        ///          from the Trick events processor.
        GunnsElectDistributed2WayBus mInterface;         /**< (1) The interface logic. */
        GunnsElectConverterInput     mPowerLoad;         /**< (1) Constant power load on the local network. */
        GunnsElectConverterOutput    mVoltageSource;     /**< (1) Voltage source to the local network. */
        bool                         mMalfPowerLoad;     /**< (1) Disables the power load draw from the local network. */
        bool                         mMalfVoltageSource; /**< (1) Disables the voltage source supply to the local network. */
        /// @}
        /// @brief Default constructor.
        GunnsElectDistributedIf();
        /// @brief Default destructor.
        virtual ~GunnsElectDistributedIf();
        /// @brief Initialize method.
        void initialize(const GunnsElectDistributedIfConfigData& configData,
                        const GunnsElectDistributedIfInputData&  inputData,
                        std::vector<GunnsBasicLink*>&            networkLinks,
                        const int                                port0);
        /// @brief Steps the link.
        virtual void step(const double dt);
        /// @brief Minor step method for non-linear iterations.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief Returns the link's assessment of the network solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief Computes and transports flows across the link.
        virtual void computeFlows(const double dt);
        /// @brief Returns true, this link is always non-linear.
        virtual bool isNonLinear();

    protected:
        double                                         mNetCapDvThreshold;  /**<    (V) trick_chkpnt_io(**) Network capacitance delta-voltage threshold. */
        std::vector<GunnsElectDistributedIfSupplyData> mSupplies;           /**< ** (1) trick_chkpnt_io(**) Local voltage supply descriptions to the interface utility. */
        unsigned int                                   mNumSupplies;        /**< *o (1) trick_chkpnt_io(**) Number of supplies. */
        unsigned int                                   mSupplyMonitorIndex; /**<    (1) trick_chkpnt_io(**) Index of the supply data to monitor. */
        GunnsElectDistributedIfSupplyData*             mSupplyMonitor;      /**<    (1) trick_chkpnt_io(**) Pointer to the monitored supply data, for visbility. */
        /// @brief Validates initialization data.
        void validate() const;
        /// @brief Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Updates the local voltage supply availability for the interface.
        void updateSupplyData();
        /// @brief Points the supply data monitor to the desired supply data object.
        void updateSupplyMonitor();
        /// @brief Updates the interface utility.
        bool updateInterface();
        /// @brief Relays notifications from the interface to H&S.
        void processIfNotifications(const bool isInit);

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectDistributedIf(const GunnsElectDistributedIf& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectDistributedIf& operator =(const GunnsElectDistributedIf& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] supply (--) Pointer to the converter link to register as a supply to the interface.
///
/// @details  Pushes the given converter link pointer onto mSupplies.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectDistributedIfConfigData::addSupply(const GunnsElectConverterOutput* supply)
{
    mSupplies.push_back(supply);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool (--) Always returns true.
///
/// @details  This link is non-linear because it can reject the network solution when the interface
///           role changes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectDistributedIf::isNonLinear()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets the supply data monitor object pointer to the supply data object at the given
///           index.
///
/// @note  Since this feature is just for debugging we don't ensure that mSupplyMonitorIndex is in
///        bounds.  User beware!
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectDistributedIf::updateSupplyMonitor()
{
    if (mSupplyMonitorIndex < mSupplies.size()) {
        mSupplyMonitor = &mSupplies.at(mSupplyMonitorIndex);
    }
}

#endif
