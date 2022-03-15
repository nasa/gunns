#ifndef GunnsFluidSorptionBed_EXISTS
#define GunnsFluidSorptionBed_EXISTS

/**
@file     GunnsFluidSorptionBed.hh
@brief    GUNNS Sorption Bed Link declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_SORPTION_BED  Sorption Bed Link
@ingroup   TSM_GUNNS_FLUID_SOURCE

@details
PURPOSE:
- (Provides the classes for the GUNNS Fluid Sorption Bed link.)

REQUIREMENTS:
- ()

REFERENCE:
- (("Full System Modeling and Validation of the Carbon Dioxide Removal Assembly", Robert Coker, et. al., 44th ICES, July 2014)
   ("A 1-D Model of the 4 Bed Molecular Sieve of the Carbon Dioxide Removal Assembly", Robert Coker, et. al.))

ASSUMPTIONS AND LIMITATIONS:
- ()

 LIBRARY DEPENDENCY:
- ((GunnsFluidSorptionBed.o))

 PROGRAMMERS:
- ((Jason Harvey) (CACI) (Initial) (2022-03))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "properties/SorbantProperties.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed Segment Sorbate Model
///
/// @details  This models the state of a single sorbate in a segment in a Sorption Bed model.
///           It holds the properties and loading states for this sorbate in the sorbant segment.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSorptionBedSorbate
{
    public:
        double mLoading;         /**< (kg*mol/m3)                       Current loading of this sorbate in the sorbant segment. */
        double mLoadingEquil;    /**< (kg*mol/m3)   trick_chkpnt_io(**) Equilibrium loading of this sorbate in the sorbant segment. */
        double mLoadingRate;     /**< (kg*mol/m3/s) trick_chkpnt_io(**) Loading rate of this sorbate into the sorbant segment. */
        double mLoadingFraction; /**< (1)           trick_chkpnt_io(**) Ratio of current to equilibrium loading. */
        double mAdsorptionRate;  /**< (kg*mol/s)    trick_chkpnt_io(**) Adsorption rate of this sorbate into the sorbant segment. */
        double mLoadedMass;      /**< (kg)          trick_chkpnt_io(**) Adsorbed mass of this sorbate in the sorbant segment. */
        /// @brief  Default constructs this Sorption Bed Sorbate.
        GunnsFluidSorptionBedSorbate();
        /// @brief  Default destructs this Sorption Bed Sorbate.
        virtual ~GunnsFluidSorptionBedSorbate();
        //TODO
        void init(const SorbateProperties* properties, const PolyFluidConfigData* fluidConfig,
                  const double loading);
        //TODO
        void registerInteractions(GunnsFluidSorptionBedSorbate* sorbates, const unsigned int nSorbates);
        /// @brief  Computes the equilibrium sorbant-sorbate loading at current conditions.
        void updateLoadingEquil(const double pp, const double temperature);
        /// @brief  Updates the sorbate loading and rate in the sorbant segment.
        void updateLoading(const double timestep, const double inFlux, const double desorbLimit);
        /// @brief  Updates the sorbed mass of this sorbate in the sorbant segment.
        void updateLoadedMass(const double volume);
        //TODO
        const SorbateProperties* getProperties() const;
        //TODO
        int getFluidIndex() const;
        //TODO
        double getMoleFraction(const PolyFluid* fluid) const;

    protected:
        const SorbateProperties* mProperties; /**<    (1) trick_chkpnt_io(**) Properties of the sorbate. */
        int                      mFluidIndex; /**< *o (1) trick_chkpnt_io(**) Index of the compound in the network bulk fluids. */
        std::vector<GunnsFluidSorptionBedSorbate*> mBlockingStates; /**< TODO */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSorptionBedSorbate(const GunnsFluidSorptionBedSorbate&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSorptionBedSorbate& operator =(const GunnsFluidSorptionBedSorbate&);
};

//TODO
class GunnsFluidSorptionBedSegmentConfigData
{
    public:
        const std::string        mName;       /**< (1) TODO */
        const SorbantProperties* mProperties; /**< (1)   trick_chkpnt_io(**) Properties of the sorbant & sorbates in this segment. */
        const double             mVolume;     /**< (m3)  trick_chkpnt_io(**) Total volume of this segment including sorbant material and voids. */
        const double             mHtc;        /**< (W/K) trick_chkpnt_io(**) Convective heat transfer coefficient between sorbant and fluid. */
        //TODO rule of 3 stuff
        //TODO
        GunnsFluidSorptionBedSegmentConfigData(const std::string& name, const SorbantProperties* sorbant, const double volume, const double htc);
        //TODO
        ~GunnsFluidSorptionBedSegmentConfigData();
        /// @brief  Copy constructs this TODO
        GunnsFluidSorptionBedSegmentConfigData(const GunnsFluidSorptionBedSegmentConfigData& that);
        /// @brief  Assigns this TODO
        GunnsFluidSorptionBedSegmentConfigData& operator =(const GunnsFluidSorptionBedSegmentConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed Segment Model
///
/// @details  This models the state of a single segment in a Sorption Bed model.  It holds the
///           states of the H2O & CO2 sorbates within, and various physical configuration & state
///           values of the segment.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSorptionBedSegment
{
    public:
        GunnsFluidSorptionBedSorbate* mSorbates;    /**< (1)   trick_chkpnt_io(**) States of the sorbates in this segment. */
        double                        mHtc;         /**< (W/K) trick_chkpnt_io(**) Convective heat transfer coefficient between sorbant and fluid. */
        double                        mVolume;      /**< (m3)  trick_chkpnt_io(**) Total volume of this segment including sorbant material and voids. */
        double                        mVolSorbant;  /**< (m3)  trick_chkpnt_io(**) Volume of the sorbant in this segment, not including voids. */
        double                        mTemperature; /**< (K)                       Average temperature of the sorbant material. */
        double                        mHeatFlux;    /**< (W)                       Heat flux from fluid to the sorbant material. */
        double                        mThermCap;    /**< (W/K)                     Total thermal capacity of sorbant + sorbates. */
        PolyFluid*                    mFluid;       /**< (1)   trick_chkpnt_io(**) Pointer to the link's internal fluid. */
        /// @brief  Default constructs this Sorption Bed Segment.
        GunnsFluidSorptionBedSegment();
        /// @brief  Default destructs this Sorption Bed Segment.
        virtual ~GunnsFluidSorptionBedSegment();
        /// @brief  Initializes this segment with the given properties.
        void init(const GunnsFluidSorptionBedSegmentConfigData& configData,
                  const PolyFluidConfigData*                    fluidConfig);
        /// @brief  Updates the sorption and propagates the state of this segment over the timestep.
        void update(double& flowIn, const double pIn, const double pOut, const double timestep);
        //TODO
        unsigned int getNSorbates() const;

    protected:
        std::string   mName; /**< *o (1) TODO */
        unsigned int mNSorbates; /**< *o (1) TODO */
        const SorbantProperties* mProperties; /**< (1) trick_chkpnt_io(**) Properties of the sorbant & sorbates in this segment. */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSorptionBedSegment(const GunnsFluidSorptionBedSegment&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSorptionBedSegment& operator =(const GunnsFluidSorptionBedSegment&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed Configuration Data
///
/// @details  This provides a data structure for the GUNNS Fluid Sorption Bed link
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO
// - change mSegments to a vector, not pointer.
// - add addSegment method to add a segment to the bed.  Args will be sorbant type/custom, not pointer
//   to properties.
//   - If the type is a pre-defined type, then the segment properites are now fully defined
//     by the default type proerties of the sorbant/sorbates, and we're done.
//   - If the type is custome, then the user must call functions in the returned custom sorbant
///    properties to define it and its sorbates.
//   - So, addSegment returns a pointer to the segment's SorbantProperties.
class GunnsFluidSorptionBedConfigData : public GunnsFluidConductorConfigData
{
    public:
        std::vector<GunnsFluidSorptionBedSegmentConfigData> mSegments; /**< (1) trick_chkpnt_io(**) Configuration data for each of the bed segments. */
        /// @brief  Default constructs this Sorption Bed configuration data.
        GunnsFluidSorptionBedConfigData(const std::string& name            = "",
                                        GunnsNodeList*     nodes           = 0,
                                        const double       maxConductivity = 0.0);
        /// @brief  Default destructs this Sorption Bed configuration data.
        virtual ~GunnsFluidSorptionBedConfigData();
        //TODO
        SorbantProperties* addCustomSorbant(const double       density  = 0.0,
                                            const double       porosity = 0.0,
                                            const double       cp       = 0.0);
        /// @brief  Adds configuration data for a new bed segment with defined sorbant type.
        void addSegment(const SorbantProperties::Type definedType, const double volume, const double htc);
        /// @brief  Adds configuration data for a new bed segment with custom sorbant type.
        void addSegment(const SorbantProperties* customType, const double volume, const double htc);

    protected:
        DefinedSorbantProperties*      mDefinedSorbants; /**< ** (1) trick_chkpnt_io(**) Properties of defined sorbant types. */
        std::vector<SorbantProperties> mCustomSorbants;  /**< ** (1) trick_chkpnt_io(**) Properties of custom sorbant types. */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSorptionBedConfigData(const GunnsFluidSorptionBedConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSorptionBedConfigData& operator =(const GunnsFluidSorptionBedConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Sorption Bed link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSorptionBedInputData : public GunnsFluidConductorInputData
{
    public:
        // TODO initial loading of sorbates in each bed?
        double mWallTemperature; /**< (K) trick_chkpnt_io(**) Initial tube wall temperature for thermal convection. */
        /// @brief  Default constructs this Sorption Bed input data with arguments.
        GunnsFluidSorptionBedInputData(const bool   malfBlockageFlag  = false,
                                       const double malfBlockageValue = 0.0,
                                       const double wallTemperature   = 0.0);
        /// @brief  Default destructs this Sorption Bed input data.
        ~GunnsFluidSorptionBedInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSorptionBedInputData(const GunnsFluidSorptionBedInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSorptionBedInputData& operator=(const GunnsFluidSorptionBedInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed
///
/// @details  This link implementation the model described in references as a GUNNS link, with some
///           extra limitations & simplifications.  The sorbant bed is modeled in 10 segments, each
///           of which can be customized with a different sorbant type, volume, fluid heat transfer
///           coefficient.  Each segment is intended to be interfaced with its own thermal aspect,
///           similar to GunnsFluidHeatExchanger.  Heat of reaction and thermal convection is
///           modeled and should affect the thermal aspect temperature, which in turn affects
///           adsorption.  Adsorption is modeled with Toth isotherm equations for each CO2 & H2O
///           sorbate paired with the sorbant type.  Co-adsorption and interaction between H2O & CO2
///           is modeled.
///
///           Limitations & simplifications of this link relative to the reference model:
///           - we don't model the axial dispersion of sorbate along the bed.
///           - many other higher-order terms in the reference model PDE's are omitted.
///           - we simplify the pressure drop between segments as a linear interpolation of the port
///             pressures, weighted by segment volume.
///           - wall heat transfer via convection is modeled with a constant heat transfer
///             coefficient.
////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO:
//TODO replace all references to CDRA, CdraAdsorber
// - do not absorb/desorb TC's, only do bulk compounds.  TC's not really needed because trace amounts
///  won't need this much accuracy, and Toth isotherm probably not a good model for them anyway.
// - this link will ABSORB all other absorber links and replace them!  well... not really...
// - Re-think init and definition of config data, bed properties, etc:
//   - do not want to make users define vectors in Python (is that even possible anyway?), and don't
//     pass in pointers to vectors.
//   - instead, each GunnsFluidSorptionBedConfigData will hold the vectors, that the user fills out
//     by calls from the input file to add segments & sorbates, etc... Link init copies that in from
//     the config data during init.
class GunnsFluidSorptionBed : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSorptionBed);
    public:
        GunnsFluidSorptionBedSegment* mSegments; /**< (1) Sorbant segment models in this bed. */
        /// @brief  Default constructs this Sorption Bed.
        GunnsFluidSorptionBed();
        /// @brief  Default destructs this Sorption Bed.
        virtual ~GunnsFluidSorptionBed();
        /// @brief  Initializes this Sorption Bed with configuration and input data.
        void initialize(const GunnsFluidSorptionBedConfigData& configData,
                        const GunnsFluidSorptionBedInputData&  inputData,
                        std::vector<GunnsBasicLink*>&          networkLinks,
                        const int                              port0,
                        const int                              port1);
        /// @brief  Updates the sorption and transports flows.
        virtual void computeFlows(const double dt);
//        /// @brief    Returns H2O adsorbtion mass for the CdraAdsorber.
//        double getAdsorbedMassH2O() const;
//        /// @brief    Returns CO2 adsorbtion mass for the CdraAdsorber.
//        double getAdsorbedMassCO2() const;
        //TODO
        double getAdsorbedMass(FluidProperties::FluidType type) const;

    protected:
        unsigned int mNSegments;       /**< *o (1)    trick_chkpnt_io(**) Constant number of segments in this link. */
        double       mVolume;          /**<    (m3)   trick_chkpnt_io(**) Combined volume of all sorbant segments. */
        double*      mAdsorptionRates; /**<    (kg/s) trick_chkpnt_io(**) Total adsorption rate of each fluid constituent over all segments. */
        double*      mAdsorbedMasses;  /**<    (kg)   trick_chkpnt_io(**) Total adsorbed mass of each fluid constituent over all segments. */
        /// @brief  Validates the initialization inputs of this Sorption Bed.
        void validate(const GunnsFluidSorptionBedConfigData& configData,
                      const GunnsFluidSorptionBedInputData&  inputData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSorptionBed(const GunnsFluidSorptionBed&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSorptionBed& operator=(const GunnsFluidSorptionBed&);
};

/// @}

//TODO
inline const SorbateProperties* GunnsFluidSorptionBedSorbate::getProperties() const
{
    return mProperties;
}

//TODO
inline int GunnsFluidSorptionBedSorbate::getFluidIndex() const
{
    return mFluidIndex;
}

//TODO
inline double GunnsFluidSorptionBedSorbate::getMoleFraction(const PolyFluid* fluid) const
{
    return fluid->getMoleFraction(mFluidIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in] type (--) The fluid type to return the adsorbed mass of.
///
/// @return  double (kg) The total adsorbed mass of the given fluid type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Getter method returns the total adsorbed mass of the given fluid type.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSorptionBed::getAdsorbedMass(FluidProperties::FluidType type) const
{
    return mAdsorbedMasses[mInternalFluid->find(type)];
}

#endif
