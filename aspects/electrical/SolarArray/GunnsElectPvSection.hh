#ifndef GunnsElectPvSection_EXISTS
#define GunnsElectPvSection_EXISTS

/**
@file
@brief    GUNNS Electrical Photovoltaic Section Model declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_SECTION    Photovoltaic Section Model
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the Photovoltaic Section Model.  Refer to the GunnsElectPvSection class details
   below.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectPvSection.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2017-10) (Initial))

@{
*/

#include "GunnsElectPvString.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Section Model Configuration Data.
///
/// @details  This provides a data structure for the Photovoltaic Section Model config data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvSectionConfigData
{
    public:
        double                       mSourceAngleExponent;    /**< (--)   trick_chkpnt_io(**) Exponent on trig function of light source incident angle. */
        double                       mBacksideReduction;      /**< (--)   trick_chkpnt_io(**) Reduction fraction (0-1) when lit from back side. */
        bool                         mSourceAngleEdgeOn;      /**< (--)   trick_chkpnt_io(**) Angle of light source to surface is edge-on instead of normal. */
        double                       mRefSourceFluxMagnitude; /**< (W/m2) trick_chkpnt_io(**) Reference ambient flux magnitude of light source at the surface. */
        GunnsElectPvStringConfigData mStringConfig;           /**< (--)   trick_chkpnt_io(**) Config data for the strings. */
        /// @brief Default constructs this Photovoltaic Section Model config data.
        GunnsElectPvSectionConfigData(const double       sectionSourceAngleExponent     = 0.0,
                                      const double       sectionBacksideReduction       = 0.0,
                                      const bool         sectionSourceAngleEdgeOn       = 0.0,
                                      const double       sectionRefSourceFluxMagnitude  = 0.0,
                                      const double       stringBlockingDiodeVoltageDrop = 0.0,
                                      const double       stringBypassDiodeVoltageDrop   = 0.0,
                                      const unsigned int stringBypassDiodeInterval      = 0,
                                      const unsigned int stringNumCells                 = 0,
                                      const double       cellSurfaceArea                = 0.0,
                                      const double       cellEfficiency                 = 0.0,
                                      const double       cellSeriesResistance           = 0.0,
                                      const double       cellShuntResistance            = 0.0,
                                      const double       cellOpenCircuitVoltage         = 0.0,
                                      const double       cellRefTemperature             = 0.0,
                                      const double       cellTemperatureVoltageCoeff    = 0.0,
                                      const double       cellTemperatureCurrentCoeff    = 0.0);
        /// @brief Default destructs this Photovoltaic Section Model config data.
        virtual ~GunnsElectPvSectionConfigData();
        /// @brief Assignment operator for this Photovoltaic Section Model config data.
        GunnsElectPvSectionConfigData& operator =(const GunnsElectPvSectionConfigData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvSectionConfigData(const GunnsElectPvSectionConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Section Model Input Data.
///
/// @details  This provides a data structure for the Photovoltaic Section Model input data, and is
///           used as a section's interface to its environment models.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvSectionInputData
{
    public:
        double mSourceFluxMagnitude;   /**< (W/m2) trick_chkpnt_io(**) Ambient flux magnitude of light source at the surface. */
        double mSourceAngle;           /**< (rad)  trick_chkpnt_io(**) Angle of light source to surface. */
        double mSourceExposedFraction; /**< (--)   trick_chkpnt_io(**) Surface area fraction exposed to light source (0-1). */
        double mTemperature;           /**< (K)    trick_chkpnt_io(**) Temperature of the section. */
        /// @brief Default constructs this Photovoltaic Section Model input data.
        GunnsElectPvSectionInputData(const double sectionSourceFluxMagnitude   = 0.0,
                                     const double sectionSourceAngle           = 0.0,
                                     const double sectionSourceExposedFraction = 0.0,
                                     const double sectionTemperature           = 0.0);
        /// @brief Default destructs this Photovoltaic Section Model input data.
        virtual ~GunnsElectPvSectionInputData();
        /// @brief Assignment operator for this Photovoltaic Section Model input data.
        GunnsElectPvSectionInputData& operator =(const GunnsElectPvSectionInputData& that);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvSectionInputData(const GunnsElectPvSectionInputData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Section Model.
///
/// @details  A Photovoltaic Section is a group of Photovoltaic Strings wired in parallel that have
///           the same environment interface, including pointing, shading, ambient light level, and
///           structural temperature.  This Section provides an interface between environment models
///           and the strings contained within.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvSection
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectPvSection);
    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsElectPvString*                  mStrings;            /**<    (--) trick_chkpnt_io(**) Array of strings in this section. */
        GunnsElectPvStringInputData          mStringsInput;       /**<    (--) trick_chkpnt_io(**) Input data for the strings. */
        GunnsElectPvSectionInputData         mInput;              /**<    (--)                     This section's unique input data and environment model interface. */
        /// @}
        /// @brief Default constructor for this Photovoltaic Section.
        GunnsElectPvSection();
        /// @brief Normal constructor for this Photovoltaic Section.
        GunnsElectPvSection(const GunnsElectPvSectionConfigData* configData);
        /// @brief Destructs this Photovoltaic Section.
        ~GunnsElectPvSection();
        /// @brief Initializes this Photovoltaic Section.
        void   initialize(const std::string&                  name,
                          const GunnsElectPvSectionInputData& inputData,
                          const unsigned int                  numStrings);
        /// @brief Updates this Photovoltaic Section state.
        void   update(const double dt);
        /// @brief Gets the number of strings in this Photovoltaic Section.
        unsigned int getNumStrings() const;
        /// @brief Gets the percentage of actual light incident on string relative to reference.
        double getPercentInsolation() const;
        /// @brief Sets this Photovoltaic Section's ambient light flux magnitude input data.
        void   setSourceFluxMagnitude(const double sourceFluxMagnitude);
        /// @brief Sets this Photovoltaic Section's ambient light source angle input data.
        void   setSourceAngle(const double sourceAngle);
        /// @brief Sets this Photovoltaic Section's surface light exposed fraction input data.
        void   setSourceExposedFraction(const double sourceExposedFraction);
        /// @brief Sets this Photovoltaic Section's temperature input data.
        void   setTemperature(const double temperature);

    protected:
        std::string                          mName;               /**<    (--) trick_chkpnt_io(**) Instance name for H&S messages. */
        const GunnsElectPvSectionConfigData* mConfig;             /**< ** (--) trick_chkpnt_io(**) Pointer to common section config data. */
        unsigned int                         mNumStrings;         /**< *o (--) trick_chkpnt_io(**) The number of strings in this section. */
        double                               mPercentInsolation;  /**<    (--) trick_chkpnt_io(**) Percentage of actual light incident on string relative to reference source flux magnitude. */
        double                               mTerminalPower;      /**<    (W)  trick_chkpnt_io(**) Total electrical power output of all strings (negative). */
        static const double                  mMaxAngleExponent;   /**< ** (--) trick_chkpnt_io(**) Upper limit on light source angle exponent. */
        /// @brief Validates the initialization of this Photovoltaic Section.
        void validate() const;
        /// @brief Updates the environment inputs to the strings.
        void updateEnvironment(const double dt);

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvSection(const GunnsElectPvSection& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvSection& operator =(const GunnsElectPvSection& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  unsigned int  (--)  Number of strings in this section.
///
/// @details  Returns the mNumStrings attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline unsigned int GunnsElectPvSection::getNumStrings() const
{
    return mNumStrings;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (--)  Percentage of actual light incident on string relative to reference.
///
/// @details  Returns the mPercentInsolation attribute.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectPvSection::getPercentInsolation() const
{
    return mPercentInsolation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] sourceFluxMagnitude (W/m2) Ambient flux magnitude of light source at the surface.
///
/// @details  Sets this object's mSourceFluxMagnitude input data to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvSection::setSourceFluxMagnitude(const double sourceFluxMagnitude)
{
    mInput.mSourceFluxMagnitude = sourceFluxMagnitude;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] sourceAngle (rad) Angle of light source to surface.
///
/// @details  Sets this object's mSourceAngle input data to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvSection::setSourceAngle(const double sourceAngle)
{
    mInput.mSourceAngle = sourceAngle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] sourceExposedFraction (--) Surface area fraction exposed to light source (0-1).
///
/// @details  Sets this object's mSourceExposedFraction input data to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvSection::setSourceExposedFraction(const double sourceExposedFraction)
{
    mInput.mSourceExposedFraction = sourceExposedFraction;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] temperature (K) Temperature of the section.
///
/// @details  Sets this object's mTemperature input data to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectPvSection::setTemperature(const double temperature)
{
    mInput.mTemperature = temperature;
}

#endif
