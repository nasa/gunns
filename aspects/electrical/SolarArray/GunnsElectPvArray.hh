#ifndef GunnsElectPvArray_EXISTS
#define GunnsElectPvArray_EXISTS

/**
@file     GunnsElectPvArray.hh
@brief    GUNNS Electrical Photovoltaic Array Link declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_ARRAY    Photovoltaic Array Link
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Photovoltaic Array Link.  Refer to the GunnsElectPvArray class details
   below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectPvArray.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-10) (Initial))

@{
*/

#include "GunnsElectPvSection.hh"
#include "core/GunnsBasicLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Array Link Configuration Data.
///
/// @details  This provides a data structure for the Photovoltaic Array Link config data.  This has
///           overloaded constructors that determine which version of the underlying String & Cell
///           models are to be used.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvArrayConfigData : public GunnsBasicLinkConfigData
{
    public:
        unsigned int                  mNumSections;         /**< (1) trick_chkpnt_io(**) Number of sections in the array. */
        unsigned int                  mNumStrings;          /**< (1) trick_chkpnt_io(**) Default total number of strings in the array. */
        unsigned int*                 mNumStringsBySection; /**< (1) trick_chkpnt_io(**) Optional list of number of strings in each section. */
        GunnsElectPvSectionConfigData mSectionConfig;       /**< (1) trick_chkpnt_io(**) Config data for the sections. */
        /// @brief Default constructs this Photovoltaic Array Link config data.
        GunnsElectPvArrayConfigData(const std::string& name = "",
                                    GunnsNodeList*     nodes = 0);
        /// @brief Constructs this Photovoltaic Array Link config data for the original version strings.
        GunnsElectPvArrayConfigData(const std::string& name,
                                    GunnsNodeList*     nodes,
                                    const unsigned int arrayNumSections,
                                    const unsigned int arrayNumStrings,
                                    const double       sectionSourceAngleExponent,
                                    const double       sectionBacksideReduction,
                                    const bool         sectionSourceAngleEdgeOn,
                                    const double       sectionRefSourceFluxMagnitude,
                                    const double       stringBlockingDiodeVoltageDrop,
                                    const double       stringBypassDiodeVoltageDrop,
                                    const unsigned int stringBypassDiodeInterval,
                                    const unsigned int stringNumCells,
                                    const double       cellSurfaceArea,
                                    const double       cellEfficiency,
                                    const double       cellSeriesResistance,
                                    const double       cellShuntResistance,
                                    const double       cellOpenCircuitVoltage,
                                    const double       cellRefTemperature,
                                    const double       cellTemperatureVoltageCoeff,
                                    const double       cellTemperatureCurrentCoeff);
        /// @brief Constructs this Photovoltaic Array Link config data for the version 2 strings.
        GunnsElectPvArrayConfigData(const std::string& name,
                                    GunnsNodeList*     nodes,
                                    const double       cellRefVoc,
                                    const double       cellRefIsc,
                                    const double       cellRefVmp,
                                    const double       cellRefImp,
                                    const double       cellRefTemperature,
                                    const double       cellCoeffDVocDT,
                                    const double       cellCoeffDIscDT,
                                    const double       cellIdeality,
                                    const double       cellArea,
                                    const double       sectionSourceAngleExponent,
                                    const double       sectionBacksideReduction,
                                    const bool         sectionSourceAngleEdgeOn,
                                    const double       sectionRefSourceFluxMagnitude,
                                    const double       stringBlockingDiodeVoltageDrop,
                                    const double       stringBypassDiodeVoltageDrop,
                                    const unsigned int stringBypassDiodeInterval,
                                    const unsigned int stringNumCells,
                                    const unsigned int arrayNumSections,
                                    const unsigned int arrayNumStrings);
        /// @brief Default destructs this Photovoltaic Array Link config data.
        virtual ~GunnsElectPvArrayConfigData();
        /// @brief Assignment operator for this Photovoltaic Array Link config data.
        GunnsElectPvArrayConfigData& operator =(const GunnsElectPvArrayConfigData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvArrayConfigData(const GunnsElectPvArrayConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Array Link Input Data.
///
/// @details  This provides a data structure for the Photovoltaic Array Link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvArrayInputData : public GunnsElectPvSectionInputData
{
    public:
        /// @brief Default constructs this Photovoltaic Array Link input data.
        GunnsElectPvArrayInputData(const double sectionSourceFluxMagnitude   = 0.0,
                                   const double sectionSourceAngle           = 0.0,
                                   const double sectionSourceExposedFraction = 0.0,
                                   const double sectionTemperature           = 0.0);
        /// @brief Default destructs this Photovoltaic Array Link input data.
        virtual ~GunnsElectPvArrayInputData();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvArrayInputData(const GunnsElectPvArrayInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvArrayInputData& operator =(const GunnsElectPvArrayInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Array Link.
///
/// @details  Models a Photovoltaic array containing parallel sections, each containing parallel
///           photovoltaic strings.  The entire set of strings is modeled as an equivalent circuit
///           supplying power to the terminal node.
///
///           This link supports two modes of string output: either the strings are all tied to a
///           common output (the terminal node), or they can be independent of the terminal node and
///           loaded individually by an external model, such as a downstream voltage regulator.
///
///           When the strings all have identical lighting & performance, this array's equivalent
///           circuit model performs identically to any individual string, scaled by the number of
///           strings (multiplies electrical current & power output for a given voltage).
///
///           When the strings have dissimilar performance (due to uneven lighting or malfunctions,
///           etc.), this array approximates their aggregate performance, including the effects of
///           strings dropping out due to their blocking diodes, etc.  This approximation is not
///           exact, but saves computation time and improves stability in a GUNNS network.
///
///           This is a 1-port GUNNS link, with the port attached to the output terminal node.
///           Connections to Ground are built-in internally.
///
///           See the detailed descriptions for the contained GunnsElectPvSection and
///           GunnsElectPvString classes.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvArray : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectPvArray);
    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsElectPvSection* mSections; /**< (1) trick_chkpnt_io(**) Array of sections in this array. */
        /// @}
        /// @brief Default Constructor.
        GunnsElectPvArray();
        /// @brief Default constructor.
        virtual ~GunnsElectPvArray();
        /// @brief Initialize method.
        void initialize(const GunnsElectPvArrayConfigData&  configData,
                        const GunnsElectPvSectionInputData& inputData,
                        std::vector<GunnsBasicLink*>&       networkLinks,
                        const int                           port0);
        /// @brief Updates the link in run-time and first minor step in non-linear systems.
        virtual void step(const double dt);
        /// @brief Minor Step method for non-linear systems.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief Compute Flows.
        virtual void computeFlows(const double dt);
        /// @brief Boolean for telling the solver this is a non-linear link.
        virtual bool isNonLinear();
        /// @brief Returns the link's assessment of the network solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief Returns the number of sections in this array.
        unsigned int getNumSections() const;
        /// @brief Returns the total number of strings in this array.
        unsigned int getNumStrings() const;
        /// @brief Gets the Maximum Power Point state of this array.
        const GunnsElectPvLoadState& getMpp() const;
        /// @brief Gets the Terminal load state of this array.
        const GunnsElectPvLoadState& getTerminal() const;
        /// @brief Gets the short-circuit current at terminal node (max load).
        double getShortCircuitCurrent() const;
        /// @brief Gets the open-circuit voltage at terminal node (no load).
        double getOpenCircuitVoltage() const;
        /// @brief Gets the average array I-V curve corner voltage.
        double getIvCornerVoltage() const;
        /// @brief Gets the average array I-V curve corner current.
        double getIvCornerCurrent() const;
        /// @brief Returns power & conductance that loads the array at the given terminal voltage.
        void predictLoadAtVoltage(double& power, double& conductance, const double voltage) const;
        /// @brief Loads the array output terminal at the given power load.
        void loadAtPower(const double power, const bool shortSide);
        /// @brief Ties the array strings to a common output or isolates them.
        void setCommonStringsOutput(const bool flag);

    protected:
        GunnsElectPvArrayConfigData mConfig;              /**< (1)   trick_chkpnt_io(**) Array config data. */
        bool                        mOpenCircuitSide;     /**< (1)   trick_chkpnt_io(**) Array is operating on the open-circuit side of its I-V curve. */
        bool                        mCommonStringsOutput; /**< (1)                        String terminals are tied to a common output. */
        double                      mPercentInsolation;   /**< (1)   trick_chkpnt_io(**) Percentage of actual light incident on array relative to reference source flux magnitude. */
        double                      mShortCircuitCurrent; /**< (amp) trick_chkpnt_io(**) Short-circuit current at terminal node (max load). */
        double                      mOpenCircuitVoltage;  /**< (V)   trick_chkpnt_io(**) Open-circuit voltage at terminal node (no load). */
        GunnsElectPvLoadState       mMpp;                 /**< (1)   trick_chkpnt_io(**) Maximum Power Point load state. */
        GunnsElectPvLoadState       mTerminal;            /**< (1)   trick_chkpnt_io(**) Terminal output load state. */
        double                      mIvCornerVoltage;     /**< (V)   trick_chkpnt_io(**) Average array I-V curve corner voltage. */
        double                      mIvCornerCurrent;     /**< (amp) trick_chkpnt_io(**) Average array I-V curve corner current. */
        /// @brief Validates the initialization of this Gunns Photovoltaic Array.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Updates the link admittance matrix.
        void buildAdmittanceMatrix();
        /// @brief Updates the link source vector.
        void buildSourceVector();
        /// @brief Updates the array state for current input conditions.
        void updateArray(const double dt);

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvArray(const GunnsElectPvArray& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvArray& operator =(const GunnsElectPvArray& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  Always true.
///
/// @details  This is always a non-linear link, as it contains diodes.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectPvArray::isNonLinear()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  unsigned int  (--)  The number of sections in this array.
///
/// @details  This returns the mConfig.mNumSections value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int GunnsElectPvArray::getNumSections() const
{
    return mConfig.mNumSections;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  unsigned int  (--)  The total number of strings in this array.
///
/// @details  This returns the mConfig.mNumStrings value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int GunnsElectPvArray::getNumStrings() const
{
    return mConfig.mNumStrings;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsElectPvLoadState& (--) A reference to the Maximum Power Point state object.
///
/// @details  Returns a const reference to the mMpp attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsElectPvLoadState& GunnsElectPvArray::getMpp() const
{
    return mMpp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsElectPvLoadState& (--) A reference to the terminal load state object.
///
/// @details  Returns a const reference to the mTerminal attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline const GunnsElectPvLoadState& GunnsElectPvArray::getTerminal() const
{
    return mTerminal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (amp)  Short-circuit current at terminal node (max load).
///
/// @details  This returns the mShortCircuitCurrent value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvArray::getShortCircuitCurrent() const
{
    return mShortCircuitCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  Open-circuit voltage at terminal node (no load).
///
/// @details  This returns the mOpenCircuitVoltage value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvArray::getOpenCircuitVoltage() const
{
    return mOpenCircuitVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  Average array I-V curve corner voltage.
///
/// @details  This returns the mIvCornerVoltage value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvArray::getIvCornerVoltage() const
{
    return mIvCornerVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (amp)  Average array I-V curve corner current.
///
/// @details  This returns the mIvCornerCurrent value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvArray::getIvCornerCurrent() const
{
    return mIvCornerCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] flag (--) True if the strings are tied to a common output voltage, false to isolate
///                      them from each other and load them separately.
///
/// @details  Sets the mCommonStringsOutput flag to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvArray::setCommonStringsOutput(const bool flag)
{
    mCommonStringsOutput = flag;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Builds the source vector for the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvArray::buildSourceVector()
{
    if (mOpenCircuitSide) {
        mSourceVector[0] = mAdmittanceMatrix[0] * mOpenCircuitVoltage;
    } else {
        mSourceVector[0] = mAdmittanceMatrix[0] * mIvCornerVoltage;
    }
}

#endif
