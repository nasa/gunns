#ifndef GunnsFluidSeparatorLiquid_EXISTS
#define GunnsFluidSeparatorLiquid_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_SOURCE_SEPARATOR_LIQUID  Separator Liquid Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Separator Liquid link model.)

REQUIREMENTS:
- ()

REFERENCE:
- (
   -()
  )

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidSeparatorLiquid.o))

 PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Initial) (2011-11))

@{
*/

#include "core/GunnsFluidPotential.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Separator Liquid Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Separator Liquid link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSeparatorLiquidConfigData : public GunnsFluidPotentialConfigData
{
    public:
        FluidProperties::FluidType  mLiquidType;           /**< (--) trick_chkpnt_io(**) Liquid phase fluid type */
        /// @brief    Default constructs this Separator Liquid model configuration data with
        ///           arguments.
        GunnsFluidSeparatorLiquidConfigData(
                                const std::string&         name                 = "",
                                GunnsNodeList*             nodes                = 0,
                                const double               maxConductivity      = 0.0,
                                const double               expansionScaleFactor = 0.0,
                                FluidProperties::FluidType liquidType = FluidProperties::NO_FLUID);
        /// @brief    Default destructs this Separator Liquid model configuration data.
        virtual ~GunnsFluidSeparatorLiquidConfigData();
        /// @brief    Copy constructs this Separator Liquid model configuration data.
        GunnsFluidSeparatorLiquidConfigData(const GunnsFluidSeparatorLiquidConfigData& that);
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSeparatorLiquidConfigData& operator =(const GunnsFluidSeparatorLiquidConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Separator Liquid  Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Separator Liquid link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSeparatorLiquidInputData : public GunnsFluidPotentialInputData
{
    public:
        double mTransferTemperature;                /**< (K) Temperature of incoming liquid. */
        /// @brief    Default constructs this Separator Liquid model input data with arguments.
        GunnsFluidSeparatorLiquidInputData(const bool   malfBlockageFlag    = false,
                                           const double malfBlockageValue   = 0.0,
                                           const double sourcePressure      = 0.0,
                                           const double transferTemperature = 0.0);
        /// @brief    Copy constructs this Separator Liquid model input data.
        GunnsFluidSeparatorLiquidInputData(const GunnsFluidSeparatorLiquidInputData& that);
        /// @brief    Default destructs this Separator Liquid model input data.
        ~GunnsFluidSeparatorLiquidInputData();
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSeparatorLiquidInputData& operator=(const GunnsFluidSeparatorLiquidInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Separator Liquid Model
///
/// @details  The GUNNS Fluid Separator Liquid link model provides the liquid network side link for
///           a two phase separator. It receives a liquid pressure and temperature from the gas side
///           (GunnsFluidSeparatorGas) and returns a liquid demand flow rate to the gas side.  The
///           transferred liquid is added to the system using an internal fluid. The port 0 node
///           must be the vacuum node.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSeparatorLiquid : public GunnsFluidPotential
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSeparatorLiquid);
    public:
        /// @brief    Default constructs this Separator Liquid model.
        GunnsFluidSeparatorLiquid();
        /// @brief    Default destructs this Separator Liquid model.
        virtual ~GunnsFluidSeparatorLiquid();
        /// @brief    Initializes this Separator Liquid model with configuration and input data.
        void initialize(const GunnsFluidSeparatorLiquidConfigData& configData,
                        const GunnsFluidSeparatorLiquidInputData&  inputData,
                        std::vector<GunnsBasicLink*>&              networkLinks,
                        const int                                  port0,
                        const int                                  port1);
        /// @brief    Updates the internal fluid of this Separator Liquid model.
        virtual void updateFluid(const double dt, const double flowrate);
        /// @brief    Special processing of data outputs from the model after to the network update
        virtual void processOutputs();
    protected:
        FluidProperties::FluidType mLiquidType;          /**< *o (--) trick_chkpnt_io(**) Liquid phase fluid type */
        double                     mTransferTemperature; /**<    (K)                      Incoming liquid temperature */
        int                        mLiquidIndex;         /**< *o (--) trick_chkpnt_io(**) Index of liquid fluid type */
        /// @brief    Validates the initialization inputs of this Separator Liquid model.
        void validate(const GunnsFluidSeparatorLiquidConfigData& configData,
                      const GunnsFluidSeparatorLiquidInputData&  inputData) const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Derives the initialization data of this Separator Liquid model.
        void derive(const GunnsFluidSeparatorLiquidConfigData& configData,
                    const GunnsFluidSeparatorLiquidInputData&  inputData);
        /// @brief    Checks for valid Separator Liquid model specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSeparatorLiquid(const GunnsFluidSeparatorLiquid&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidSeparatorLiquid& operator=(const GunnsFluidSeparatorLiquid&);
};

/// @}

#endif
