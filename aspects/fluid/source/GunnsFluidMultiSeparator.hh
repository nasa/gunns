#ifndef GunnsFluidMultiSeparator_EXISTS
#define GunnsFluidMultiSeparator_EXISTS

/**
@file     GunnsFluidMultiSeparator.hh
@brief    GUNNS Fluid Multi-Separator implementation

@defgroup  TSM_GUNNS_FLUID_SOURCE_MULTI_SEPARATOR  Fluid Multi-Separator Model
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Multi-Separator link model.)

REQUIREMENTS:
- ()

REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidMultiSeparator.o))

 PROGRAMMERS:
- ((Jason Harvey) (CACI) (2021-09) (Initial))

@{
*/

#include "core/GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Multi-Separator Configuration Data
///
/// @details  This provides a data structure for the GUNNS Fluid Multi-Separator link configuration
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMultiSeparatorConfigData : public GunnsFluidLinkConfigData
{
    public:
        double                                  mMaxConductance; /**< (m2) trick_chkpnt_io(**) Max conductance of the flow-thru path. */
        std::vector<FluidProperties::FluidType> mFluidTypes;     /**< (1)  trick_chkpnt_io(**) Fluid types to be separated. */
        std::vector<int>                        mFluidPorts;     /**< (1)  trick_chkpnt_io(**) Port assignments for each separated fluid. */
        std::vector<ChemicalCompound::Type>     mTcTypes;        /**< (1)  trick_chkpnt_io(**) Trace compound types to be separated. */
        std::vector<int>                        mTcPorts;        /**< (1)  trick_chkpnt_io(**) Port assignments for each separated trace compound. */
        /// @brief  Default constructs this Multi-Separator model configuration data with arguments.
        GunnsFluidMultiSeparatorConfigData(
                const std::string&                       name           = "",
                GunnsNodeList*                           nodes          = 0,
                const double                             maxConductance = 0.0,
                std::vector<FluidProperties::FluidType>* fluidTypes     = 0,
                std::vector<int>*                        fluidPorts     = 0,
                std::vector<ChemicalCompound::Type>*     tcTypes        = 0,
                std::vector<int>*                        tcPorts        = 0);
        /// @brief  Default destructs this Multi-Separator model configuration data.
        virtual ~GunnsFluidMultiSeparatorConfigData();
        /// @brief  Copy constructs this Multi-Separator model configuration data.
        GunnsFluidMultiSeparatorConfigData(const GunnsFluidMultiSeparatorConfigData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidMultiSeparatorConfigData& operator =(const GunnsFluidMultiSeparatorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Multi-Separator  Input Data
///
/// @details  This provides a data structure for the GUNNS Fluid Multi-Separator link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMultiSeparatorInputData : public GunnsFluidLinkInputData
{
    public:
        std::vector<double> mFluidFractions; /**< (1) trick_chkpnt_io(**) For each separated fluid type, initial thru-flow fraction that is removed. */
        std::vector<double> mTcFractions;    /**< (1) trick_chkpnt_io(**) For each separated trace compound, initial thru-flow fraction that is removed. */
        /// @brief  Default constructs this Multi-Separator model input data with arguments.
        GunnsFluidMultiSeparatorInputData(const bool           malfBlockageFlag  = false,
                                          const double         malfBlockageValue = 0.0,
                                          std::vector<double>* fluidFractions    = 0,
                                          std::vector<double>* tcFractions       = 0);
        /// @brief  Copy constructs this Multi-Separator model input data.
        GunnsFluidMultiSeparatorInputData(const GunnsFluidMultiSeparatorInputData& that);
        /// @brief  Default destructs this Multi-Separator model input data.
        ~GunnsFluidMultiSeparatorInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidMultiSeparatorInputData& operator=(const GunnsFluidMultiSeparatorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Multi-Separator Model
///
/// @details  This link is like a fluid conductor, but it also separates multiple compounds from the
///           bulk conductor flow and diverts them to separate exit ports.
///           Rather than calculating the fraction of each compound to remove, this relies on the
///           fraction to be supplied from outside.  This can remove up to 100% of the compound from
///           the incoming flow.  Each separation compound is assigned a separate port in this link
///           to exit to; however these ports can be connected to the same or different network
///           nodes.  This doesn't model phase change, and doesn't add or remove heat to the fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidMultiSeparator : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidMultiSeparator);
    public:
        /// @brief  Default constructs this Multi-Separator model.
        GunnsFluidMultiSeparator();
        /// @brief  Default destructs this Multi-Separator model.
        virtual ~GunnsFluidMultiSeparator();
        /// @brief  Initializes this Multi-Separator model with configuration and input data.
        void initialize(const GunnsFluidMultiSeparatorConfigData& configData,
                        const GunnsFluidMultiSeparatorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        std::vector<int>*                         portsVector);
        /// @brief  Updates the contributions to the network system of equations.
        virtual void step(const double dt);
        /// @brief  Computes flow rates to be transported.
        virtual void computeFlows(const double dt);
        /// @brief  Transports the flows through the link between the nodes.
        virtual void transportFlows(const double dt);
        /// @brief  Sets the separation mass fraction for the given fluid type.
        void setSeparationFraction(const FluidProperties::FluidType, const double fraction);
        /// @brief  Returns the separation mass fraction for the given fluid type.
        double getSeparationFraction(const FluidProperties::FluidType) const;
        /// @brief  Sets the separation mass fraction for the given trace compound type.
        void setTcFraction(const ChemicalCompound::Type, const double fraction);
        /// @brief  Returns the separation mass fraction for the given trace compound type.
        double getTcFraction(const ChemicalCompound::Type) const;

    protected:
        double     mMaxConductance;       /**<    (m2)           trick_chkpnt_io(**) Max conductance of the flow-thru path. */
        int        mNumSepTypes;          /**< *o (1)            trick_chkpnt_io(**) Number of separation bulk fluid compounds. */
        int        mNumTcTypes;           /**< *o (1)            trick_chkpnt_io(**) Number of separation trace compounds. */
        int*       mSepIndex;             /**<    (1)            trick_chkpnt_io(**) Index of each separation bulk fluid compound in the network fluids array. */
        int*       mTcIndex;              /**<    (1)            trick_chkpnt_io(**) Index of each separation trace compound in the network trace compounds array. */
        int*       mSepPort;              /**<    (1)            trick_chkpnt_io(**) Exit port assignment of each separation bulk fluid compound. */
        int*       mTcPort;               /**<    (1)            trick_chkpnt_io(**) Exit port assignment of each separation trace compound. */
        double*    mSepFraction;          /**<    (1)            trick_chkpnt_io(**) For each separated bulk fluid compound, the thru-flow fraction that is removed. */
        double*    mTcFraction;           /**<    (1)            trick_chkpnt_io(**) For each separated trace compound, the thru-flow fraction that is removed. */
        double     mEffectiveConductance; /**< *o (m2)           trick_chkpnt_io(**) Effective conductance of the flow-thru path. */
        double     mSystemConductance;    /**< *o (kg*mol/s/kPa) trick_chkpnt_io(**) Limited molar conductance of the flow-thru path. */
        double*    mSepBufferThru;        /**<    (kg*mol/s)     trick_chkpnt_io(**) Removal flow rate buffer for the flow-thru path, this pass. */
        double*    mSepBufferExit;        /**<    (kg*mol/s)     trick_chkpnt_io(**) Removal flow rate added to exit ports, next pass. */
        PolyFluid* mSepFluid;             /**<    (1)            trick_chkpnt_io(**) Fluid objects for each separated compound. */
        double*    mWorkTcMassFlowRates;  /**<    (kg/s)         trick_chkpnt_io(**) Working array for intermediate trace compounds mass flow rate values. */
        double*    mWorkMoleFractions;    /**<    (1)            trick_chkpnt_io(**) Working array for intermediate mole fraction values. */
        /// @brief  Validates the initialization inputs of this Multi-Separator model.
        void validate(const GunnsFluidMultiSeparatorConfigData& configData,
                      const GunnsFluidMultiSeparatorInputData&  inputData) const;
        /// @brief  Validates the initialization inputs of the separation fluid types.
        void validateSep(const GunnsFluidMultiSeparatorConfigData& configData,
                         const GunnsFluidMultiSeparatorInputData&  inputData) const;
        /// @brief  Validates the initialization inputs of the trace compuond types.
        void validateTc(const GunnsFluidMultiSeparatorConfigData& configData,
                        const GunnsFluidMultiSeparatorInputData&  inputData) const;
        /// @brief  Frees allocated memory.
        void cleanupMemory();
        /// @breif  Allocates and initializes dynamic memory.
        void allocateMemory(const GunnsFluidMultiSeparatorConfigData& configData,
                            const GunnsFluidMultiSeparatorInputData&  inputData);
        /// @brief  Initializes internal fluid objects.
        void initializeFluids();
        /// @brief  Linearizes the fluid conductance for the admittance matrix.
        double linearizeConductance();
        /// @brief Computes molar flow rate through the through-flow path of the link.
        void   computeFlux();
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Returns the index of the given type in our separated fluid types array.
        int findIndexOfType(const FluidProperties::FluidType type) const;
        /// @brief  Returns the index of the given type in our separated trace compound types array.
        int findIndexOfTc(const ChemicalCompound::Type type) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidMultiSeparator(const GunnsFluidMultiSeparator&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidMultiSeparator& operator=(const GunnsFluidMultiSeparator&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] type     (--) The fluid type to set the separation fraction for.
/// @param[in] fraction (--) The new mass fraction (0-1) value to use for the fluid.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets mSeparationFraction for the given fluid type to the given fraction.  The fraction
///           is quietly limited to be in (0-1).  Throws exception if the given type is not in the
///           network or not one of our separated fluid types.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidMultiSeparator::setSeparationFraction(const FluidProperties::FluidType type, const double fraction)
{
    mSepFraction[findIndexOfType(type)] = MsMath::limitRange(0.0, fraction, 1.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (--) The separation fraction (0-1) of the given fluid type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the separation fraction of the given fluid type.  Throws exception if the
///           given type is not in the network or not one of our separated fluid types.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMultiSeparator::getSeparationFraction(const FluidProperties::FluidType type) const
{
    return mSepFraction[findIndexOfType(type)];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] type     (--) The trace compound type to set the separation fraction for.
/// @param[in] fraction (--) The new mass fraction (0-1) value to use for the trace compound.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Sets mSeparationFraction for the given trace compound type to the given fraction.  The
///           fraction is quietly limited to be in (0-1).  Throws exception if the given type is not
///           in the network or not one of our separated trace compound types.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidMultiSeparator::setTcFraction(const ChemicalCompound::Type type, const double fraction)
{
    mTcFraction[findIndexOfTc(type)] = MsMath::limitRange(0.0, fraction, 1.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (--) The separation fraction (0-1) of the given trace compound type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the separation fraction of the given trace compound type.  Throws exception if
///           the given type is not in the network or not one of our separated trace compound types.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidMultiSeparator::getTcFraction(const ChemicalCompound::Type type) const
{
    return mTcFraction[findIndexOfTc(type)];
}

#endif
