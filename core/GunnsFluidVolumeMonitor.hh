#ifndef GunnsFluidVolumeMonitor_EXISTS
#define GunnsFluidVolumeMonitor_EXISTS

/**
@file
@brief     GUNNS Fluid Volume Monitor Spotter declarations

@defgroup  TSM_GUNNS_CORE_FLUID_VOLUME_MONITOR   GUNNS Fluid Volume Monitor Spotter
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:  (Provides the classes for the GUNNS Fluid Volume Monitor Spotter.
           This spotter is used to peek at the non-sense-able fluid parameters of a node, such as
           volume, total mass, partial masses & moles.  This spotter attaches to a link and looks
           at the node attached at the designated link's port.  This allows the spotter to keep
           monitoring the link's attached node when the link changes nodes in the network.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidVolumeMonitor.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2016-11) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsNetworkSpotter.hh"
#include "math/MsMath.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Volume Monitor Spotter Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Volume Monitor Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidVolumeMonitorConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        /// @brief  Default constructs this GUNNS Fluid Volume Monitor Spotter configuration data.
        GunnsFluidVolumeMonitorConfigData(const std::string& name);
        /// @brief  Default destructs this GUNNS Fluid Volume Monitor Spotter configuration data.
        virtual ~GunnsFluidVolumeMonitorConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidVolumeMonitorConfigData(const GunnsFluidVolumeMonitorConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidVolumeMonitorConfigData& operator =(const GunnsFluidVolumeMonitorConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Volume Monitor Spotter Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Volume Monitor Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidVolumeMonitorInputData : public GunnsNetworkSpotterInputData
{
    public:
        int mLinkPort;   /**< (--) trick_chkpnt_io(**) Which of the link's port nodes to monitor. */
        /// @brief  Default constructs this GUNNS Fluid Volume Monitor Spotter input data.
        GunnsFluidVolumeMonitorInputData(const int linkPort = 0);
        /// @brief  Default destructs this GUNNS Fluid Volume Monitor Spotter input data.
        virtual ~GunnsFluidVolumeMonitorInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidVolumeMonitorInputData(const GunnsFluidVolumeMonitorInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidVolumeMonitorInputData& operator =(const GunnsFluidVolumeMonitorInputData& that);
};

// Forward-declare referenced types.
struct GunnsNodeList;
class GunnsFluidLink;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Volume Monitor Spotter Class.
///
/// @details  This spotter is used to peek at the non-sense-able fluid parameters of a node, such as
///           volume, total mass, partial masses & moles.  This spotter attaches to a link and looks
///           at the node attached at the designated link's port.  This allows the spotter to keep
///           monitoring the link's attached node when the link changes nodes in the network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidVolumeMonitor : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidVolumeMonitor);
    public:
        /// @brief  Default Constructor
        GunnsFluidVolumeMonitor(const GunnsNodeList&  nodeList,
                                const GunnsFluidLink& link);
        /// @brief   Default destructor.
        virtual     ~GunnsFluidVolumeMonitor();
        /// @brief   Initializes the GUNNS Fluid Volume Monitor Spotter with configuration and
        ///          input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief   Steps the GUNNS Fluid Volume Monitor Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief   Steps the GUNNS Fluid Volume Monitor Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief   Gets the total mass of the node contents.
        double       getMass() const;
        /// @brief   Gets the mass fraction of the specified constituent index in the node contents.
        double       getMassFraction(const int index) const;
        /// @brief   Gets the mole fraction of the specified constituent index in the node contents.
        double       getMoleFraction(const int index) const;
        /// @brief   Gets the total volume of the node.
        double       getVolume() const;

    protected:
        const GunnsNodeList&  mNodeList;             /**< *o (--) trick_chkpnt_io(**) Reference to the network node list. */
        const GunnsFluidLink& mLink;                 /**< *o (--) trick_chkpnt_io(**) Reference to the link of interest. */
        int                   mLinkPort;             /**<    (--)                     Which of the link's port nodes to monitor. */
        int                   mNumFluidConstituents; /**< *o (--) trick_chkpnt_io(**) Number of fluid constituents in the network. */
        double                mNodeMass;             /**< *o (kg) trick_chkpnt_io(**) Total mass of the node contents. */
        double*               mNodeMassFractions;    /**< *o (--) trick_chkpnt_io(**) Mass fractions of the node contents. */
        double*               mNodeMoleFractions;    /**< *o (--) trick_chkpnt_io(**) Mole fractions of the node contents. */
        double*               mNodeTcMoleFractions;  /**< *o (--) trick_chkpnt_io(**) Mole fractions of the node trace compounds contents. */
        double                mNodeVolume;           /**< *o (m3) trick_chkpnt_io(**) Total volume of the node. */
        /// @brief   Validates the supplied configuration data.
        const GunnsFluidVolumeMonitorConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsFluidVolumeMonitorInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief   Deletes dynamic memory.
        void         cleanup();
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidVolumeMonitor(const GunnsFluidVolumeMonitor& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidVolumeMonitor& operator =(const GunnsFluidVolumeMonitor& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (kg) Total mass of all fluid in the node contents.
///
/// @details  Returns the total mass of all fluid in the node contents.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidVolumeMonitor::getMass() const
{
    return mNodeMass;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  index (--) Index of the fluid constituent to get the mass fraction of.
///
/// @returns  double (--) Mass fraction (0-1) of the specified constituent in the node contents.
///
/// @details  Returns the mass fraction (0-1) of the specified fluid constituent in the node
///           contents.  The given index is limited to the valid range of fluid constituents.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidVolumeMonitor::getMassFraction(const int index) const
{
    return mNodeMassFractions[MsMath::limitRange(0, index, mNumFluidConstituents-1)];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  index (--) Index of the fluid constituent to get the mole fraction of.
///
/// @returns  double (--) Mole fraction (0-1) of the specified constituent in the node contents.
///
/// @details  Returns the mole fraction (0-1) of the specified fluid constituent in the node
///           contents.  The given index is limited to the valid range of fluid constituents.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidVolumeMonitor::getMoleFraction(const int index) const
{
    return mNodeMoleFractions[MsMath::limitRange(0, index, mNumFluidConstituents-1)];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (m3) Total volume of the node.
///
/// @details  Returns the total volume of the node.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidVolumeMonitor::getVolume() const
{
    return mNodeVolume;
}

#endif
