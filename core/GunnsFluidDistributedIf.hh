#ifndef GunnsFluidDistributedIf_EXISTS
#define GunnsFluidDistributedIf_EXISTS

/**
@file
@brief    GUNNS Fluid Distributed Interface Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_DISTRIBUTED_IF    GUNNS Fluid Distributed Interface Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2022 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Distributed Interface link.)

REFERENCE:
- ("Distributed Fluid Simulation Interface Standard", J.Harvey, Draft, June 2019)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidDistributedIf.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2019-03) (Initial))

@{
*/

#include "GunnsFluidLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Distributed Interface Data
///
/// @details  This class provides a data structure for the data shared between a pair of Fluid
///           Distributed Interface links that allows flow between separate fluid networks.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributedIfData
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidDistributedIfData);
    public:
        unsigned int mFrameCount;      /**< (--)                     Frame count driven by this side. */
        unsigned int mFrameLoopback;   /**< (--)                     Frame count driven by other side, echoed back. */
        bool         mDemandMode;      /**< (--)                     Demand mode flag. */
        double       mCapacitance;     /**< (mol/Pa)                 Network capacitance. */
        double       mSource;          /**< (--)                     Fluid pressure (Pa) or molar flow (mol/s). */
        double       mEnergy;          /**< (--)                     Fluid temperature (K) or specific enthalpy (J/kg). */
        double*      mMoleFractions;   /**< (--) trick_chkpnt_io(**) Fluid mole fractions. */
        double*      mTcMoleFractions; /**< (--) trick_chkpnt_io(**) Trace compounds mole fractions. */
        /// @brief  Default constructs this Fluid Distributed Interface interface data.
        GunnsFluidDistributedIfData();
        /// @brief  Default destructs this Fluid Distributed Interface interface data.
        virtual ~GunnsFluidDistributedIfData();
        /// @brief  Allocates dynamic arrays for mass and mole fractions.
        void initialize(const std::string& name,
                        const unsigned int nFluids,
                        const unsigned int nTc,
                        const bool         fluidSizesOverride,
                        const unsigned int nIfFluids,
                        const unsigned int nIfTc);
        /// @brief  Returns whether this object has received valid data.
        bool hasValidData();
        /// @brief  Sets the interface bulk fluid mole fractions to the given values.
        void setMoleFractions(const double* fractions);
        /// @brief  Sets the interface trace compound mole fractions to the given values.
        void setTcMoleFractions(const double* fractions);
        /// @brief  Sets the given array to the interface bulk fluid mole fraction values.
        void getMoleFractions(double* fractions);
        /// @brief  Sets the given array to the interface trace compound mole fraction values.
        void getTcMoleFractions(double* fractions);
        /// @brief Assignment operator for this Fluid Distributed Interface interface data.
        GunnsFluidDistributedIfData& operator =(const GunnsFluidDistributedIfData& that);

    protected:
        unsigned int mNumFluid;       /**< *o (--) trick_chkpnt_io(**) Number of primary fluid compounds in model. */
        unsigned int mNumTc;          /**< *o (--) trick_chkpnt_io(**) Number of trace compounds in model. */
        unsigned int mNumFluidIf;     /**< *o (--) trick_chkpnt_io(**) Number of primary fluid compounds in interface. */
        unsigned int mNumTcIf;        /**< *o (--) trick_chkpnt_io(**) Number of trace compounds in interface. */
        unsigned int mNumFluidCommon; /**< *o (--) trick_chkpnt_io(**) Number of primary fluid compounds in common between model and interface. */
        unsigned int mNumTcCommon;    /**< *o (--) trick_chkpnt_io(**) Number of trace compounds in common between model and interface. */

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributedIfData(const GunnsFluidDistributedIfData&);
};

// Forward declarations for pointer types
class GunnsFluidCapacitor;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Distributed Interface Configuration Data
///
/// @details  This class provides a data structure for the Fluid Distributed Interface link
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributedIfConfigData : public GunnsFluidLinkConfigData
{
    public:
        bool                 mIsPairMaster;           /**< (--) trick_chkpnt_io(**) This is the master of the pair. */
        bool                 mUseEnthalpy;            /**< (--) trick_chkpnt_io(**) Transport energy as specific enthalpy instead of temperature. */
        bool                 mDemandOption;           /**< (--) trick_chkpnt_io(**) Demand mode option to trade stability for less restriction on flow rate. */
        GunnsFluidCapacitor* mCapacitorLink;          /**< (--) trick_chkpnt_io(**) Pointer to the node capacitor link. */
        double               mModingCapacitanceRatio; /**< (--) trick_chkpnt_io(**) Supply over Demand capacitance ratio for triggering mode flip. */
        double               mDemandFilterConstA;     /**< (--) trick_chkpnt_io(**) Demand filter gain constant A. */
        double               mDemandFilterConstB;     /**< (--) trick_chkpnt_io(**) Demand filter gain constant B. */
        bool                 mFluidSizesOverride;     /**< (--) trick_chkpnt_io(**) Override of fluid mixture sizes is active. */
        unsigned int         mNumFluidOverride;       /**< (--) trick_chkpnt_io(**) Number of primary fluid compounds override value. */
        unsigned int         mNumTcOverride;          /**< (--) trick_chkpnt_io(**) Number of trace compounds override value. */
        /// @brief Default constructs this Fluid Distributed Interface configuration data.
        GunnsFluidDistributedIfConfigData(
                const std::string&   name           = "",
                GunnsNodeList*       nodes          = 0,
                const bool           isPairMaster   = false,
                const bool           useEnthalpy    = false,
                const bool           demandOption   = false,
                GunnsFluidCapacitor* capacitorLink  = 0);
        /// @brief Default destructs this Fluid Distributed Interface configuration data.
        virtual ~GunnsFluidDistributedIfConfigData();
        /// @brief Override the interface fluid mixture sizes to the given values.
        void overrideInterfaceMixtureSizes(const unsigned int nFluids, const unsigned int nTc);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributedIfConfigData(const GunnsFluidDistributedIfConfigData&);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributedIfConfigData& operator =(const GunnsFluidDistributedIfConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Distributed Interface Input Data
///
/// @details  This class provides a data structure for the Fluid Distributed Interface link input
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributedIfInputData : public GunnsFluidLinkInputData
{
    public:
        bool mForceDemandMode; /**< (--) trick_chkpnt_io(**) Forces the link to always be in Demand mode. */
        bool mForceSupplyMode; /**< (--) trick_chkpnt_io(**) Forces the link to always be in Supply mode. */
        /// @brief Default constructs this Fluid Distributed Interface input data.
        GunnsFluidDistributedIfInputData(const bool   malfBlockageFlag  = false,
                                         const double malfBlockageValue = 0.0,
                                         const bool   forceDemandMode   = false,
                                         const bool   forceSupplyMode   = false);
        /// @brief Default destructs this Fluid Distributed Interface input data.
        virtual ~GunnsFluidDistributedIfInputData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributedIfInputData(const GunnsFluidDistributedIfInputData&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributedIfInputData& operator =(const GunnsFluidDistributedIfInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Distributed Interface class
///
/// @details  This combines the functions of GunnsFluidExternalSupply and GunnsFluidExternalDemand
///           into one class.  Two of these, in separate networks, interface with each other.
///           One link in the pair takes the Demand role and the other Supply.  The links swap
///           these roles automatically to keep the Supply role on the side with the higher network
///           capacitance.  This promotes stability in high-latency, tightly coupled interfaces.
///
///           Because the Demand effect cannot be applied to a node with capacitance, this link
///           edits the node to zero volume, via the node's attached capacitor link, when entering
///           Demand mode, and restores the node volume when returning to Supply mode.
///
///           The nodes in both networks should default to the same volume.
///
///           Mass is not conserved when the Supply/Demand sides flip.  The mass error is
///           proportional to the loop data lag, flow rate through the path, and rate of mixture
///           & temperature change in the flow during the flip.
///
///           Because of the mass conservation error that occurs during the flip, this should only
///           be used in especially challenging stability interfaces, such as the hatch/IMV
///           connection between cabin atmospheres in distributed sims.
///
///           Because this link has limited use, it isn't a replacement for ExternalSupply & Demand.
///
///           This is a one-port link and we do away with the assumed Ground node.
///
///           In both Supply and Demand roles, this link treats positive flow direction as flow into
///           the node.  So a negative flow out of the supply node will match a positive flow value
///           into the demand node.  This differs from GunnsFluidExternalSupply, which treats flow
///           out of the supply node as positive.
///
///           These links need to know about other similar links in the same network to avoid
///           interfering with each other's network capacitance.  Use the addOtherIf method to
///           register links with each other.
///
///           Note that in interfaces with a large conductance, such as hatches, this works best
///           with a minLinearizationPotential of 1e-8 or less.
///           \verbatim
///
///              Master-Side Network                                          Slave-Side Network
///                                                     |
///                   ****                                                             ****
///                ***    ***                           |                           ***    ***
///               *          *                                                     *          *
///              *            *                         |                         *            *
///              *    NODE    *<----------->                          <---------->*    NODE    *
///              *            * GunnsFluidDistributedIf | GunnsFluidDistributedIf *            *
///               *          *           Link                       Link           *          *
///                ***    ***                           |                           ***    ***
///                   ****                                                             ****
///                                                     |
///           \endverbatim
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidDistributedIf : public GunnsFluidLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidDistributedIf);
    public:
        GunnsFluidDistributedIfData mInData;  /**< (--) Data from the other paired link input from the interface. */
        GunnsFluidDistributedIfData mOutData; /**< (--) Data to the other paired link output to the interface. */
        /// @brief Default Constructor.
        GunnsFluidDistributedIf();
        /// @brief Default Destructor.
        virtual ~GunnsFluidDistributedIf();
        /// @brief Initializes the link, with input data.
        void initialize(const GunnsFluidDistributedIfConfigData& configData,
                        const GunnsFluidDistributedIfInputData&  inputData,
                        std::vector<GunnsBasicLink*>&            networkLinks,
                        const int                                port0);
        /// @brief Step method for updating the link.
        virtual void step(const double dt);
        /// @brief Method for computing the flows across the link.
        virtual void computeFlows(const double dt);
        /// @brief Transports the flows to and from the node.
        virtual void transportFlows(const double dt);
        /// @brief Special processing of data inputs to the model prior to the network update.
        virtual void processInputs();
        /// @brief Special processing of data outputs from the model after the network update.
        virtual void processOutputs();
        /// @brief Registers another GunnsFluidDistributedIf link with this one.
        void addOtherIf(GunnsFluidDistributedIf* otherIf);
        /// @brief Returns the capacitance this link adds to the node in Demand mode.
        double getSuppliedCapacitance() const;
        /// @brief Returns the node's network capacitance delta-potentials array.
        const double* getNetCapDeltaPotential() const;

    protected:
        bool                        mIsPairMaster;           /**<    (--)       trick_chkpnt_io(**) This is the master of the link pair. */
        bool                        mUseEnthalpy;            /**<    (--)       trick_chkpnt_io(**) Transport energy as specific enthalpy instead of temperature. */
        bool                        mDemandOption;           /**<    (--)       trick_chkpnt_io(**) Demand mode option to trade stability for less restriction on flow rate. */
        GunnsFluidCapacitor*        mCapacitorLink;          /**< ** (--)       trick_chkpnt_io(**) Pointer to the node capacitor link. */
        double                      mModingCapacitanceRatio; /**<    (--)       trick_chkpnt_io(**) Supply over Demand capacitance ratio for triggering mode flip. */
        double                      mDemandFilterConstA;     /**<    (--)       trick_chkpnt_io(**) Demand filter gain constant A. */
        double                      mDemandFilterConstB;     /**<    (--)       trick_chkpnt_io(**) Demand filter gain constant B. */
        bool                        mForceDemandMode;        /**<    (--)                           Forces the link to always be in Demand mode. */
        bool                        mForceSupplyMode;        /**<    (--)                           Forces the link to always be in Supply mode. */
        bool                        mInDataLastDemandMode;   /**<    (--)                           Last-pass demand mode from the other paired link. */
        int                         mFramesSinceFlip;        /**<    (--)                           Number of frames since the last mode flip. */
        double                      mSupplyVolume;           /**<    (m3)                           Stored volume of the node when in Demand mode. */
        double                      mEffectiveConductivity;  /**<    (m2)       trick_chkpnt_io(**) Effective conductivity of the link in Demand mode. */
        double                      mSourcePressure;         /**<    (kPa)      trick_chkpnt_io(**) Source pressure created in the node in Demand mode. */
        double                      mDemandFlux;             /**<    (kg*mol/s) trick_chkpnt_io(**) Source molar flow added to the node in Supply mode. */
        int                         mLoopLatency;            /**<    (--)       trick_chkpnt_io(**) Round-trip loop data lag measured by the Master of the pair. */
        double                      mDemandFluxGain;         /**<    (--)       trick_chkpnt_io(**) Demand mode flow factor due to lag frames. */
        double                      mSuppliedCapacitance;    /**<    (mol/Pa)   trick_chkpnt_io(**) Network capacitance applied to the Demand node from the Supply side. */
        double*                     mTempMassFractions;      /**< ** (--)       trick_chkpnt_io(**) Scratch array for mass-mole mixture conversions. */
        double*                     mTempMoleFractions;      /**< ** (--)       trick_chkpnt_io(**) Scratch array for bulk fluid mole fraction adjustments. */
        double*                     mTempTcMoleFractions;    /**< ** (--)       trick_chkpnt_io(**) Scratch array for trace compound mole fraction adjustments. */
        std::vector<GunnsFluidDistributedIf*> mOtherIfs;     /**< ** (--)       trick_chkpnt_io(**) Vector of other similar links to avoid capacitance interference with. */
        PolyFluid                   mFluidState;             /**<    (--)       trick_chkpnt_io(**) Fluid state of the interface volume, for sensors. */
        static const double         mNetworkCapacitanceFlux; /**< ** (kg*mol/s) trick_chkpnt_io(**) Flux value to use in network node capacitance calculations. */
        /// @brief Validates the initialization of this Gunns Fluid Distributed Interface.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief Computes molar flux rate through the link.
        void computeFlux();
        /// @brief Special processing of Supply mode data output.
        void processOutputsSupply();
        /// @brief Special processing of Demand mode data output.
        void processOutputsDemand();
        /// @brief Special processing of Supply mode data input.
        void processInputsSupply();
        /// @brief Special processing of Demand mode data input.
        void processInputsDemand();
        /// @brief Copies incoming fluid state from the interface to the given fluid object.
        double inputFluid(const double pressure, PolyFluid* fluid);
        /// @brief Copies the given fluid object state to the outgoing interface.
        double outputFluid(PolyFluid* fluid);
        /// @brief Handles several mode flip cases based on input data.
        void flipModesOnInput();
        /// @brief Flips to the Demand mode based on capacitances.
        void flipModesOnCapacitance();
        /// @brief Flips to the Demand mode.
        void flipToDemandMode();
        /// @brief Flips to the Supply mode.
        void flipToSupplyMode();
        /// @brief Computes and outputs capacitance.
        void outputCapacitance();
        /// @brief Checks if any of the given fluid's mixture fractions are negative.
        bool checkNegativeFluidFractions(const PolyFluid* fluid) const;

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidDistributedIf(const GunnsFluidDistributedIf& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidDistributedIf& operator =(const GunnsFluidDistributedIf& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the molar flux rate through the link.  Units:
///           Flux (kgmol/s) = Admittance (kgmol/kPa/s) * dP (kPa)
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidDistributedIf::computeFlux()
{
    mFlux = mPotentialDrop * mAdmittanceMatrix[0] + mSourceVector[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (mol/Pa)  Effective capacitance added to the node in Demand mode.
///
/// @details  Returns the value of mSuppliedCapacitance.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidDistributedIf::getSuppliedCapacitance() const
{
    return mSuppliedCapacitance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  const double*  (--)  The node's network capacitance delta-potentials array.
///
/// @details  Returns the node's network capacitance delta-potentials array.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const double* GunnsFluidDistributedIf::getNetCapDeltaPotential() const
{
    return mNodes[0]->getNetCapDeltaPotential();
}

#endif
