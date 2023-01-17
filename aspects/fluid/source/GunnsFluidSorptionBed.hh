#ifndef GunnsFluidSorptionBed_EXISTS
#define GunnsFluidSorptionBed_EXISTS

/**
@file     GunnsFluidSorptionBed.hh
@brief    GUNNS Sorption Bed Link declarations

@defgroup  TSM_GUNNS_FLUID_SOURCE_SORPTION_BED  Sorption Bed Link
@ingroup   TSM_GUNNS_FLUID_SOURCE

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

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
/// @brief    Sorption Bed Fluid Indexes
///
/// @details  This holds the indexes of a chemical compound in the bulk fluid constituents and the
///           trace compounds in a network's fluids.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct GunnsFluidSorptionBedFluidIndex
{
    int mFluid; /**< *o (1) trick_chkpnt_io(**) Index of the compound in the network bulk fluid constituents. */
    int mTc;    /**< *o (1) trick_chkpnt_io(**) Index of the compound in the network trace compounds. */
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed Segment Sorbate Model
///
/// @details  This models the state of a single sorbate in a segment in a Sorption Bed model.
///           It holds the properties and loading states for this sorbate within the sorbant.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSorptionBedSorbate
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSorptionBedSorbate);
    public:
        bool   mMalfLoadingEquilFlag;  /**< (1)                               Activates the equilibrium loading factor malfunction. */
        double mMalfLoadingEquilValue; /**< (1)                               Sets the equilibrium loading malfunction factor. */
        double mLoading;               /**< (kg*mol/m3)                       Current loading of this sorbate in the sorbant segment. */
        double mLoadingEquil;          /**< (kg*mol/m3)   trick_chkpnt_io(**) Equilibrium loading of this sorbate in the sorbant segment. */
        double mLoadingRate;           /**< (kg*mol/m3/s) trick_chkpnt_io(**) Loading rate of this sorbate into the sorbant segment. */
        double mLoadingFraction;       /**< (1)           trick_chkpnt_io(**) Ratio of current to equilibrium loading. */
        double mAdsorptionRate;        /**< (kg*mol/s)    trick_chkpnt_io(**) Adsorption rate of this sorbate into the sorbant segment. */
        double mLoadedMass;            /**< (kg)          trick_chkpnt_io(**) Adsorbed mass of this sorbate in the sorbant segment. */
        /// @brief  Default constructs this Sorption Bed Sorbate.
        GunnsFluidSorptionBedSorbate();
        /// @brief  Default destructs this Sorption Bed Sorbate.
        virtual ~GunnsFluidSorptionBedSorbate();
        /// @brief  Initializes this Sorption Bed Sorbate.
        void init(const SorbateProperties* properties, const int fluidIndex, const int tcIndex,
                  const double loading, const double volume, const PolyFluid* fluid);
        /// @brief  Stores pointers to interacting sorbates in the sorption bed.
        virtual void registerInteractions(GunnsFluidSorptionBedSorbate* sorbates, const unsigned int nSorbates);
        /// @brief  Computes the equilibrium sorbant-sorbate loading at current conditions.
        void updateLoadingEquil(const double pp, const double temperature);
        /// @brief  Updates the sorbate loading and rate in the sorbant segment.
        void updateLoading(const double timestep, const double inFlux, const double desorbLimit);
        /// @brief  Updates the sorbed mass of this sorbate in the sorbant segment.
        void updateLoadedMass(const double volume);
        /// @brief  Returns the heat flux to the sorbant caused by sorption of this sorbate.
        double computeHeatFlux() const;
        /// @brief  Returns the Sorbate Properties of this Sorption Bed Sorbate.
        const SorbateProperties* getProperties() const;
        /// @brief  Returns the fluid indexes of this sorbate compound.
        const GunnsFluidSorptionBedFluidIndex* getFluidIndexes() const;
        /// @brief  Returns the fluid indexes of the offgasing compounds.
        const std::vector<GunnsFluidSorptionBedFluidIndex>* getOffgasIndexes() const;
        /// @brief  Computes and returns the mole fraction of this sorbate in the stream.
        double getMoleFraction(const PolyFluid* fluid) const;
        /// @brief  Resets model values after checkpoint load.
        void restartModel();

    protected:
        const SorbateProperties*                     mProperties;            /**< *o (1) trick_chkpnt_io(**) Properties of the sorbate. */
        GunnsFluidSorptionBedFluidIndex              mFluidIndexes;          /**< *o (1) trick_chkpnt_io(**) Indexes of the sorbate compound in the network fluids. */
        std::vector<GunnsFluidSorptionBedSorbate*>   mBlockingStates;        /**< ** (1) trick_chkpnt_io(**) Pointers to the Sorption Bed Sorbate states of the blocking compounds. */
        std::vector<unsigned int>                    mBlockingCompoundIndex; /**< ** (1) trick_chkpnt_io(**) Index of the blocking compounds interaction data in the Sorbate Properties. */
        std::vector<GunnsFluidSorptionBedFluidIndex> mOffgasIndexes;         /**< ** (1) trick_chkpnt_io(**) Fluid indexes of the offgasing compounds. */
        static const float                           mLimitAdsorbFraction;   /**< ** (1) trick_chkpnt_io(**) Limit on fraction of influx that can be adsorbed. */

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSorptionBedSorbate(const GunnsFluidSorptionBedSorbate&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSorptionBedSorbate& operator =(const GunnsFluidSorptionBedSorbate&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed Segment Configuration Data
///
/// @details  This provides a data structure for the GUNNS Fluid Sorption Bed Segment configuration
///           data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSorptionBedSegmentConfigData
{
    public:
        std::string              mName;       /**< (1)   trick_chkpnt_io(**) Name of the sorption bed segment. */
        const SorbantProperties* mProperties; /**< (1)   trick_chkpnt_io(**) Properties of the sorbant & sorbates in this segment. */
        double                   mVolume;     /**< (m3)  trick_chkpnt_io(**) Total volume of this segment including sorbant material and voids. */
        double                   mHtc;        /**< (W/K) trick_chkpnt_io(**) Convective heat transfer coefficient between sorbant and fluid. */
        /// @brief  Constructs this Sorption Bed Segment Configuration Data.
        GunnsFluidSorptionBedSegmentConfigData(const std::string& name, const SorbantProperties* sorbant,
                                               const double volume, const double htc);
        /// @brief  Destructs this Sorption Bed Segment Configuration Data.
        ~GunnsFluidSorptionBedSegmentConfigData();
        /// @brief  Copy constructs this Sorption Bed Segment Configuration Data.
        GunnsFluidSorptionBedSegmentConfigData(const GunnsFluidSorptionBedSegmentConfigData& that);
        /// @brief  Assigns this Sorption Bed Segment Configuration Data.
        GunnsFluidSorptionBedSegmentConfigData& operator =(const GunnsFluidSorptionBedSegmentConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed Segment Input Data
///
/// @details  This provides a data structure for the GUNNS Fluid Sorption Bed Segment input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSorptionBedSegmentInputData
{
    public:
        unsigned int           mSegment; /**< (1)         trick_chkpnt_io(**) The segment number to load. */
        ChemicalCompound::Type mSorbate; /**< (1)         trick_chkpnt_io(**) The sorbate compound type to load. */
        double                 mLoading; /**< (kg*mol/m3) trick_chkpnt_io(**) Initial loading amount. */
        /// @brief  Constructs this Sorption Bed Segment Input Data.
        GunnsFluidSorptionBedSegmentInputData(const unsigned int segment, const ChemicalCompound::Type sorbate,
                                              const double loading);
        /// @brief  Destructs this Sorption Bed Segment Input Data.
        ~GunnsFluidSorptionBedSegmentInputData();
        /// @brief  Copy constructs this Sorption Bed Segment Input Data.
        GunnsFluidSorptionBedSegmentInputData(const GunnsFluidSorptionBedSegmentInputData& that);
        /// @brief  Assigns this Sorption Bed Segment Input Data.
        GunnsFluidSorptionBedSegmentInputData& operator =(const GunnsFluidSorptionBedSegmentInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Sorption Bed Segment Model
///
/// @details  This models the state of a single segment in a Sorption Bed model.  It holds the
///           states of the sorbant & sorbates within, and various physical configuration & state
///           values of the segment.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidSorptionBedSegment
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidSorptionBedSegment);
    public:
        bool                          mMalfDegradeFlag;  /**< (1)                       Activates the sorbant degradation malfunction. */
        double                        mMalfDegradeValue; /**< (1)                       Fraction of sorbant performance lost due to degradation. */
        GunnsFluidSorptionBedSorbate* mSorbates;         /**< (1)   trick_chkpnt_io(**) States of the sorbates in this segment. */
        double                        mVolume;           /**< (m3)  trick_chkpnt_io(**) Total volume of this segment including sorbant material and voids. */
        double                        mTemperature;      /**< (K)                       Average temperature of the sorbant material. */
        double                        mHeatFlux;         /**< (W)                       Heat flux from fluid to the sorbant material. */
        double                        mThermCap;         /**< (W/K)                     Total thermal capacity of sorbant + sorbates. */
        PolyFluid*                    mFluid;            /**< (1)   trick_chkpnt_io(**) Pointer to the link's internal fluid. */
        /// @brief  Default constructs this Sorption Bed Segment.
        GunnsFluidSorptionBedSegment();
        /// @brief  Default destructs this Sorption Bed Segment.
        virtual ~GunnsFluidSorptionBedSegment();
        /// @brief  Initializes this segment with the given properties.
        void init(const GunnsFluidSorptionBedSegmentConfigData&             configData,
                  const PolyFluidConfigData*                                fluidConfig,
                  const std::vector<GunnsFluidSorptionBedSegmentInputData>& loading);
        /// @brief  Updates the sorption and propagates the state of this segment over the timestep.
        void update(double& flowIn, const double pIn, const double pOut, const double timestep);
        /// @brief  Returns the number of sorbates in this Sorption Bed Segment.
        unsigned int getNSorbates() const;
        /// @brief  Sets and resets the degradation malfunction controls for this segment.
        void setMalfDegrade(const bool flag = false, const double value = 0.0);

    protected:
        std::string              mName;       /**< *o (1)   trick_chkpnt_io(**) Name of this instance for messaging. */
        unsigned int             mNSorbates;  /**< *o (1)   trick_chkpnt_io(**) Number of Sorption Bed Sorbates in this segment. */
        const SorbantProperties* mProperties; /**< *o (1)   trick_chkpnt_io(**) Properties of the sorbant & sorbates in this segment. */
        double                   mHtc;        /**<    (W/K) trick_chkpnt_io(**) Convective heat transfer coefficient between sorbant and fluid. */
        double                   mVolSorbant; /**<    (m3)  trick_chkpnt_io(**) Volume of the sorbant in this segment, not including voids. */
        /// @brief  Updates fluid masses in the exit stream.
        double exchangeFluid(const int fluidIndex, const int tcIndex, const double mdotInBulk,
                             const double ndotInTc, const double dndot, const double molWeight);
        /// @brief  Returns the effective sorbant volume including degradation malfunction.
        double computeEffectiveSorbantVolume() const;

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
        /// @brief  Adds configuration data for a new custom sorbant and returns its address.
        SorbantProperties* addCustomSorbant(const double density  = 0.0,
                                            const double porosity = 0.0,
                                            const double cp       = 0.0);
        /// @brief  Adds configuration data for a new bed segment with defined sorbant type.
        void addSegment(const SorbantProperties::Type definedType, const double volume, const double htc);
        /// @brief  Adds configuration data for a new bed segment with custom sorbant type.
        void addSegment(SorbantProperties* customType, const double volume, const double htc);

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
        double                                             mWallTemperature; /**< (K) trick_chkpnt_io(**) Initial tube wall temperature for thermal convection. */
        std::vector<GunnsFluidSorptionBedSegmentInputData> mLoading;         /**< (1) trick_chkpnt_io(**) Initial sorbate loadings. */
        /// @brief  Default constructs this Sorption Bed input data with arguments.
        GunnsFluidSorptionBedInputData(const bool   malfBlockageFlag  = false,
                                       const double malfBlockageValue = 0.0,
                                       const double wallTemperature   = 0.0);
        /// @brief  Default destructs this Sorption Bed input data.
        ~GunnsFluidSorptionBedInputData();
        /// @brief  Adds an initial loading value for the given sorbate type in the given segment.
        void addSegmentSorbateLoading(const unsigned int segment, const ChemicalCompound::Type sorbate,
                                      const double loading);

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
///           extra limitations & simplifications.  The sorbant bed is modeled in one or more
///           segments, each of which can be customized with a different sorbant type, sorbates and
///           other properties.  Each segment is intended to be interfaced with its own thermal
///           aspect similar to GunnsFluidHeatExchanger.  Heat of reaction and thermal convection is
///           modeled and should affect the thermal aspect temperature, which in turn affects
///           adsorption.  Adsorption is modeled by the SorbateProperties within the sorbant's
///           SorbantProperties.  Co-adsorption and interaction between sorbates is modeled.
///
///           Limitations & simplifications of this link relative to the reference model:
///           - we don't model the axial dispersion of sorbate along the bed.
///           - many other higher-order terms in the reference model PDE's are omitted.
///           - we simplify the pressure drop between segments as a linear interpolation of the port
///             pressures, weighted by segment volume.
///           - wall heat transfer via convection is modeled with a constant heat transfer
///             coefficient.
////////////////////////////////////////////////////////////////////////////////////////////////////
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
        virtual void transportFlows(const double dt);
        /// @brief  Returns the total absorbed mass of the given fluid type.
        double getAdsorbedFluidMass(FluidProperties::FluidType type) const;
        /// @brief  Returns the total absorbed mass of the given trace compound type.
        double getAdsorbedTcMass(ChemicalCompound::Type type) const;
        /// @brief  Returns the total absorption rate of the given fluid type.
        double getAdsorptionFluidRate(FluidProperties::FluidType type) const;
        /// @brief  Returns the total absorption rate of the given trace compound type.
        double getAdsorptionTcRate(ChemicalCompound::Type type) const;

    protected:
        unsigned int mNSegments;            /**< *o (1)    trick_chkpnt_io(**) Constant number of segments in this link. */
        double       mVolume;               /**<    (m3)   trick_chkpnt_io(**) Combined volume of all sorbant segments. */
        double*      mAdsorptionFluidRates; /**<    (kg/s) trick_chkpnt_io(**) Total adsorption rate of each bulk fluid constituent over all segments. */
        double*      mAdsorptionTcRates;    /**<    (kg/s) trick_chkpnt_io(**) Total adsorption rate of each trace compound over all segments. */
        double*      mAdsorbedFluidMasses;  /**<    (kg)   trick_chkpnt_io(**) Total adsorbed mass of each bulk fluid constituent over all segments. */
        double*      mAdsorbedTcMasses;     /**<    (kg)   trick_chkpnt_io(**) Total adsorbed mass of each trace compound over all segments. */
        /// @brief  Validates the initialization inputs of this Sorption Bed.
        void validate(const GunnsFluidSorptionBedConfigData& configData,
                      const GunnsFluidSorptionBedInputData&  inputData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief  Zeroes the adsorbed mass and adsorption rate output arrays.
        void zeroAdsorbOutputs();
        /// @brief  Zeroes the adsorbed mass and adsorption rate temporary working arrays.
        void zeroAdsorbWork();
        /// @brief  Computes the adsorbed mass and adsorption rate output values.
        void computeAdsorbOutputs();
        /// @brief  Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;
        /// @brief  Update bed segments for sorption and exchange with the through-fluid.
        double updateSegments(const double dt, const double sourceDensity, const unsigned int sourcePort);

    private:
        double* mWorkFluidRates;  /**< ** (kg*mol/s) trick_chkpnt_io(**) Temporary work array for total adsorption rate of each bulk fluid constituent over all segments. */
        double* mWorkTcRates;     /**< ** (kg*mol/s) trick_chkpnt_io(**) Temporary work array for total adsorption rate of each trace compound over all segments. */
        double* mWorkFluidMasses; /**< ** (kg)       trick_chkpnt_io(**) Temporary work array for total adsorbed mass of each bulk fluid constituent over all segments. */
        double* mWorkTcMasses;    /**< ** (kg)       trick_chkpnt_io(**) Temporary work array for total adsorbed mass of each trace compound over all segments. */
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidSorptionBed(const GunnsFluidSorptionBed&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidSorptionBed& operator=(const GunnsFluidSorptionBed&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  SorbateProperties* (--) Pointer to the sorbate properties of this sorbate state.
///
/// @details  Returns mProperties.
////////////////////////////////////////////////////////////////////////////////////////////
inline const SorbateProperties* GunnsFluidSorptionBedSorbate::getProperties() const
{
    return mProperties;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsFluidSorptionBedFluidIndex* (--) Pointer to the sorbate comopund fluid indexes.
///
/// @details  Returns the address of mFluidIndexes.
////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsFluidSorptionBedFluidIndex* GunnsFluidSorptionBedSorbate::getFluidIndexes() const
{
    return &mFluidIndexes;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  std::vector<GunnsFluidSorptionBedFluidIndex>* (--) Pointer to the offgas fluid indexes vector.
///
/// @details  Returns the address of mOffgasIndexes.
////////////////////////////////////////////////////////////////////////////////////////////
inline const std::vector<GunnsFluidSorptionBedFluidIndex>* GunnsFluidSorptionBedSorbate::getOffgasIndexes() const
{
    return &mOffgasIndexes;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (W) Heat flux into the sorbant due to sorption.
///
/// @details  Calls the sorbate properties to get the heat of sorption given the current
///           adsorption rate, and returns the returned value to the caller.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSorptionBedSorbate::computeHeatFlux() const
{
    return mProperties->computeHeatFlux(mAdsorptionRate);
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  int (--) The number of sorbates in this sorption bed segment.
///
/// @details  Returns mNSorbates.
////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int GunnsFluidSorptionBedSegment::getNSorbates() const
{
    return mNSorbates;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag  (--) True activates the malfunction.
/// @param[in] value (--) Fraction (0-1) of degradation, fraction of sorbant that cannot sorb.
///
/// @details  Calling this with default arguments deactivates the malfunction.
////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSorptionBedSegment::setMalfDegrade(const bool flag, const double value)
{
    mMalfDegradeFlag  = flag;
    mMalfDegradeValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns double (m3) Effective sorbant volume with degradation malfunction applied.
///
/// @details  Computes and returns the effective sorbant volume considering degradation malfunction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSorptionBedSegment::computeEffectiveSorbantVolume() const
{
    if (mMalfDegradeFlag) {
        return mVolSorbant * MsMath::limitRange(0.0, 1.0 - mMalfDegradeValue, 1.0);
    }
    return mVolSorbant;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] segment (--)        The segment number to load.
/// @param[in] sorbate (--)        The sorbate compound type to load.
/// @param[in] loading (kg*mol/m3) The initial loading amount.
///
/// @details  Adds a new GunnsFluidSorptionBedSegmentInputData to mLoading, with the given
///           values.
////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidSorptionBedInputData::addSegmentSorbateLoading(const unsigned int           segment,
                                                                     const ChemicalCompound::Type sorbate,
                                                                     const double                 loading)
{
    mLoading.push_back(GunnsFluidSorptionBedSegmentInputData(segment, sorbate, loading));
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in] type (--) The fluid type to return the adsorbed mass of.
///
/// @return  double (kg) The total adsorbed mass of the given fluid type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the total adsorbed mass of the given fluid type.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSorptionBed::getAdsorbedFluidMass(FluidProperties::FluidType type) const
{
    return mAdsorbedFluidMasses[mInternalFluid->find(type)];
}

////////////////////////////////////////////////////////////////////////////////////////////
/// @param [in] type (--) The fluid type to return the adsorption rate of.
///
/// @return  double (kg/s) The total adsorption rate of the given fluid type.
///
/// @throws   TsOutOfBoundsException
///
/// @details  Returns the total adsorption rate of the given fluid type.
////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidSorptionBed::getAdsorptionFluidRate(FluidProperties::FluidType type) const
{
    return mAdsorptionFluidRates[mInternalFluid->find(type)];
}

#endif
