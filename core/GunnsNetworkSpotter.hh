#ifndef GunnsNetworkSpotter_EXISTS
#define GunnsNetworkSpotter_EXISTS

/**
@file
@brief     GUNNS Network Spotter declarations

@defgroup  TSM_GUNNS_CORE_NETWORK_SPOTTER    GUNNS Network Spotter
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Network Spotter.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsNetworkSpotter.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2013-08) (Initial))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Network Spotter Configuration Data Class.
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Network
///           Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsNetworkSpotterConfigData
{
    public:
        std::string mName;      /**< (--) trick_chkpnt_io(**) Instance name for self-identification in messages */
        /// @brief  Default constructor.
        GunnsNetworkSpotterConfigData(const std::string& name);
        /// @brief  Default destructor.
        virtual ~GunnsNetworkSpotterConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsNetworkSpotterConfigData(const GunnsNetworkSpotterConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsNetworkSpotterConfigData& operator =(const GunnsNetworkSpotterConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Network Spotter Input Data Class.
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Network
///           Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsNetworkSpotterInputData
{
    public:
        /// @brief  Default constructor.
        GunnsNetworkSpotterInputData();
        /// @brief  Default destructor.
        virtual ~GunnsNetworkSpotterInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsNetworkSpotterInputData(const GunnsNetworkSpotterInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsNetworkSpotterInputData& operator =(const GunnsNetworkSpotterInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Network Spotter Class.
///
/// @details  This is a base interface class used to insert custom class objects into a GUNNS
///           network class, and have them initialized and updated during run-time.  These are not
///           links, and are not stepped or accessed by the GUNNS solver.  Instead, these are
///           updated by the network class and are meant to implement user interfaces to groups of
///           network links, or collate network-wide data for output, etc.  A derived class will add
///           references to external links as protected members, and set them via their default
///           constructors.
///
/// @note     This class has pure virtual methods.
///
/// @note     See the test/ExampleDerivedSpotter class for an example of how to derive from this
////          class and use references to external network links, etc.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsNetworkSpotter);
    public:
        /// @brief  Default constructor.
        GunnsNetworkSpotter();
        /// @brief  Default destructor.
        virtual     ~GunnsNetworkSpotter();
        /// @brief  Initializes the GUNNS Network Spotter with configuration and input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief  GUNNS Network Spotter processing performed before the GUNNS solver step.
        virtual void stepPreSolver(const double dt)  = 0;
        /// @brief  GUNNS Network Spotter processing performed after the GUNNS solver step.
        virtual void stepPostSolver(const double dt) = 0;
        /// @brief  Returns whether the GUNNS Network Spotter has been successfully initialized & validated.
        bool         isInitialized() const;
        /// @brief  Returns the instance name of this GUNNS Network Spotter.
        const std::string& getName() const;

    protected:
        std::string  mName;     /**< *o (--) trick_chkpnt_io(**) Instance name for self-identification in messages */
        bool         mInitFlag; /**< *o (--) trick_chkpnt_io(**) Initialization status flag (T is good) */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsNetworkSpotter(const GunnsNetworkSpotter& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsNetworkSpotter& operator =(const GunnsNetworkSpotter& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Returns true if initialization & validation were successful.
///
/// @details  Returns whether the object has been successfully initialized & validated.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsNetworkSpotter::isInitialized() const
{
    return mInitFlag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  string  (--)  Name of this instance as a string reference.
///
/// @details  This will be empty until the spotter is initialized.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const std::string& GunnsNetworkSpotter::getName() const
{
    return mName;
}

#endif
