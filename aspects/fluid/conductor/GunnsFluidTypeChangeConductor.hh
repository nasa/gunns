#ifndef GunnsFluidTypeChangeConductor_EXISTS
#define GunnsFluidTypeChangeConductor_EXISTS

/**
@file     GunnsFluidTypeChangeConductor.hh
@brief    GUNNS Fluid Type Change Conductor Model declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_PHASE_CHANGE    GUNNS Fluid Type Change Conductor Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Type Change Conductor link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidTypeChangeConductor.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2021-10))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Type Change Conductor Model Configuration Data
///
/// @details  This class provides a data structure for the GUNNS Fluid Type Change Conductor link
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTypeChangeConductorConfigData : public GunnsFluidConductorConfigData
{
    public:
        std::vector<FluidProperties::FluidType> mFluidTypesPort0; /**< (1)  trick_chkpnt_io(**) Fluid types to be converted at the port 0 node. */
        std::vector<FluidProperties::FluidType> mFluidTypesPort1; /**< (1)  trick_chkpnt_io(**) Fluid types to be converted at the port 1 node. */
        /// @brief  Default constructs this Type Change Conductor configuration data.
        GunnsFluidTypeChangeConductorConfigData(
                const std::string&                       name            = "",
                GunnsNodeList*                           nodes           = 0,
                const double                             maxConductivity = 0.0,
                std::vector<FluidProperties::FluidType>* fluidTypesPort0 = 0,
                std::vector<FluidProperties::FluidType>* fluidTypesPort1 = 0);
        /// @brief  Copy constructs this Type Change Conductor configuration data.
        GunnsFluidTypeChangeConductorConfigData(const GunnsFluidTypeChangeConductorConfigData& that);
        /// @brief  Default destructs this Type Change Conductor configuration data.
        virtual ~GunnsFluidTypeChangeConductorConfigData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidTypeChangeConductorConfigData& operator =(const GunnsFluidTypeChangeConductorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Type Change Conductor Model Input Data
///
/// @details  This class provides a data structure for the GUNNS Fluid Type Change Conductor link
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTypeChangeConductorInputData : public GunnsFluidConductorInputData
{
    public:
        /// @brief  Default constructs this Type Change Conductor input data.
        GunnsFluidTypeChangeConductorInputData(const bool   malfBlockageFlag  = false,
                                               const double malfBlockageValue = 0.0);
        /// @brief  Copy constructs this Type Change Conductor input data.
        GunnsFluidTypeChangeConductorInputData(const GunnsFluidTypeChangeConductorInputData& that);
        /// @brief  Default destructs this Type Change Conductor input data.
        virtual ~GunnsFluidTypeChangeConductorInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidTypeChangeConductorInputData& operator =(const GunnsFluidTypeChangeConductorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Type Change Conductor Model
///
/// @details  This is like a normal GunnsFluidConductor, except it can convert the fluids flowing
///           through it from one type to another.  Zero or more type conversions can be done.
///           Each conversion defines a different type for the port 0 node and the port 1 node.
///           Fluid leaving a node has its moles converted to the same number of moles of the
///           corresponding type in the other node.
///
///           This is intended to be used to convert different fluid properties types of the same
///           chemical compound, such as between GUNNS_O2 and GUNNS_O2_REAL_GAS.  But any conversion
///           is allowed as long as the phase of the compound matches the node's phase.
///
///           Note that this conserves the total number of moles, not mass.  When converting between
///           different state equation fluids of the same compound, such as between GUNNS_O2 and
///           GUNNS_O2_REAL_GAS, this distinction makes no difference, and both maass and moles are
///           conserved.  However when converting between different compounds, such as N2 and O2,
///           the moles is conserved, but mass is not, due to their differing molecular weights.  We
///           choose to conserve moles, not mass, so that this conversion won't cause state errors
///           in the nodes for ideal gas mixtures.
///
///           This can be used to do phase changes like a GunnsFluidPhaseChangeConverter; however
///           this link won't calculate the heat of phase change.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidTypeChangeConductor : public  GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidTypeChangeConductor);
    public:
        /// @brief  Default constructs this Type Change Conductor.
        GunnsFluidTypeChangeConductor();
        /// @brief  Default destructs this Type Change Conductor.
        virtual ~GunnsFluidTypeChangeConductor();
        /// @brief  Initializes this Type Change Conductor with configuration and input data.
        void initialize(const GunnsFluidTypeChangeConductorConfigData& configData,
                        const GunnsFluidTypeChangeConductorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&                  networkLinks,
                        const int                                      port0,
                        const int                                      port1);
        /// @brief  Moves the fluid from one port to the other.
        virtual void transportFluid(const bool forcedOutflow,
                                    const int  fromPort = 0,
                                    const int  toPort   = 1);

    protected:
        unsigned int mNumTypes;          /**< *o (1) trick_chkpnt_io(**) Number of fluid types being converted. */
        int*         mIndexPort0;        /**<    (1) trick_chkpnt_io(**) Index of each conversion fluid type in the port 0 node. */
        int*         mIndexPort1;        /**<    (1) trick_chkpnt_io(**) Index of each conversion fluid type in the port 1 node. */
        double*      mWorkMoleFractions; /**<    (1) trick_chkpnt_io(**) Working array for intermediate mole fraction values. */
        /// @brief  Validates the initialization inputs of this Type Change Conductor.
        void validate(const GunnsFluidTypeChangeConductorConfigData& configData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidTypeChangeConductor(const GunnsFluidTypeChangeConductor&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidTypeChangeConductor& operator =(const GunnsFluidTypeChangeConductor&);
};

/// @}

#endif
