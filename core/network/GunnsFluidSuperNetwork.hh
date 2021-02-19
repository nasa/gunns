#ifndef GunnsFluidSuperNetwork_EXISTS
#define GunnsFluidSuperNetwork_EXISTS

/**
@file
@brief     GUNNS Fluid Super-Network declarations

@defgroup  TSM_GUNNS_CORE_FLUID_SUPER_NETWORK    GUNNS Fluid Super-Network Class
@ingroup   TSM_GUNNS_CORE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Fluid Super-Network.)

@details
REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidSuperNetwork.o))

PROGRAMMERS:
- ((Jason Harvey) (L3) (2016-10) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/network/GunnsSuperNetworkBase.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Super-Network Class.
///
/// @details  This is the super-network for fluid aspect networks.  Please see the details for the
///           base class for more info.
///
///           There are a few extra restrictions on the fluid networks added as sub-networks:
///           - All sub-networks must have identical internal network fluid configuration: number of
///             types, and the exact same types in the same order.
///           - Trace Compounds (TC's) are optional, but all networks must contain identical TC
///             config: number of types and order.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSuperNetwork : public GunnsSuperNetworkBase
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSuperNetwork);
    public:
        GunnsFluidNode* netNodes;                            /**< (--) Network fluid nodes array. */
        /// @brief  Default constructor.
        GunnsFluidSuperNetwork(const std::string& name);
        /// @brief  Default destructor.
        virtual ~GunnsFluidSuperNetwork();

    protected:
        /// @brief  Allocates dynamic array of fluid nodes for the super-network.
        virtual void allocateNodes();
        /// @brief  Deletes the dynamic array of fluid nodes.
        virtual void freeNodes();
        /// @brief  Returns whether the nodes array has been allocated.
        virtual bool isNullNodes() const;
        /// @brief  Initializes the super-network fluid Ground node.
        virtual void initGroundNode();
        /// @brief  Initializes the super-network solver.
        virtual void initSolver();
        /// @brief  Creates a derived joint network type.
        virtual GunnsNetworkBase* createJointNetwork(const int type);
        /// @brief  Allocate an array of the derived joint network type.
        virtual void registerJoints();
        /// @brief  Returns whether the given fluid configurations are identical.
        bool compareFluidConfigs(const PolyFluidConfigData* config1,
                                 const PolyFluidConfigData* config2) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSuperNetwork(const GunnsFluidSuperNetwork& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSuperNetwork& operator =(const GunnsFluidSuperNetwork& that);
};

/// @}

#endif
