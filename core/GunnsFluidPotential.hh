#ifndef GunnsFluidPotential_EXISTS
#define GunnsFluidPotential_EXISTS

/**
@file
@brief    GUNNS Fluid Potential Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_FLUID_POTENTIAL    GUNNS Fluid Potential Link
@ingroup   TSM_GUNNS_CORE_LINK_FLUID

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Potential.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidPotential.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (2011-02) (Initial Prototype)
-  (Kevin Supak)  (L-3 Communications) (2011-02) (Updated to Coding Standards))

@{
*/

#include "GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Potential Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Fluid Potential
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPotentialConfigData : public GunnsFluidConductorConfigData
{
    public:
        /// @brief Default constructs this Fluid Potential configuration data.
        GunnsFluidPotentialConfigData(const std::string& name                 = "",
                                      GunnsNodeList*     nodes                = 0,
                                      const double       maxConductivity      = 0.0,
                                      const double       expansionScaleFactor = 0.0);

        /// @brief Default destructs this Fluid Potential configuration data.
        virtual ~GunnsFluidPotentialConfigData();

        /// @brief Copy constructs this Fluid Potential configuration data.
        GunnsFluidPotentialConfigData(const GunnsFluidPotentialConfigData& that);

    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPotentialConfigData& operator =(const GunnsFluidPotentialConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fluid Potential Input Data
///
/// @details  This provides a data structure for the Fluid Potential input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPotentialInputData : public GunnsFluidConductorInputData
{
    public:
        double mSourcePressure;                   /**< (kPa) Initial pressure rise of the link */

        /// @brief Default constructs this Fluid Potential input data.
        GunnsFluidPotentialInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0,
                                     const double sourcePressure    = 0.0);

        /// @brief Copy constructs this Fluid Potential input data.
        GunnsFluidPotentialInputData(const GunnsFluidPotentialInputData& that);

        /// @brief Default destructs this Fluid Potential input data.
        virtual ~GunnsFluidPotentialInputData();

    protected:

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPotentialInputData& operator =(const GunnsFluidPotentialInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Fluid Potential class
///
/// @details  This class inherits GunnsFluidConductor, but also creates a pressure source effect in
///           fluid networks, which can be used to simulate a pump/fan, or a pressure boundary
///           condition.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidPotential : public GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidPotential);

    public:

        /// @brief Default Constructor
        GunnsFluidPotential();

        /// @brief Default destructor
        virtual ~GunnsFluidPotential();

        /// @brief Initializes the link
        void initialize(const GunnsFluidPotentialConfigData& configData,
                        const GunnsFluidPotentialInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1);

        /// @brief Updates link for the frame
        virtual void step(const double dt);

        /// @brief Sets the Source Pressure
        void setSourcePressure(const double toPressure);

        /// @brief Gets the Source Pressure
        double getSourcePressure() const;

    protected:
        double mSourcePressure;        /**< (kPa) Pressure rise created by the link */

        /// @brief Validates the initialization of this Gunns Fluid Potential
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Updates the Admittance and Source terms for the potential source
        virtual void buildPotential();

        /// @brief Computes molar flux rate through the link
        virtual void computeFlux();

    private:

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsFluidPotential(const GunnsFluidPotential& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsFluidPotential& operator =(const GunnsFluidPotential& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] toPressure (kPa) Desired pressure rise
///
/// @details  Sets the source pressure
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidPotential::setSourcePressure(const double toPressure)
{
    mSourcePressure = toPressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return double (kPa) Returns the source pressure
///
/// @details  Gets the source pressure
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidPotential::getSourcePressure() const
{
    return mSourcePressure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Computes the molar flux rate through the link.  Units:
///           Flux (kgmol/s) = Admittance (kgmol/kPa/s) * dP (kPa)
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidPotential::computeFlux()
{
    mFlux = mPotentialDrop * mAdmittanceMatrix[0] - mSourceVector[0];
}

#endif
