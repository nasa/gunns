#ifndef GunnsBasicPotential_EXISTS
#define GunnsBasicPotential_EXISTS

/**
@file
@brief    GUNNS Basic Potential Link declarations

@defgroup  TSM_GUNNS_CORE_LINK_BASIC_POTENTIAL    GUNNS Basic Potential Link
@ingroup   TSM_GUNNS_CORE_LINK_BASIC

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Basic Potential Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- (
   (GunnsBasicPotential.o)
  )

PROGRAMMERS:
- (
   (Jason Harvey) (L3) (2011-02) (Initial Prototype))
   (Kevin Supak)  (L3) (2011-02) (Updated to Coding Standards))
  )

@{
*/

#include "GunnsBasicConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Potential Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Potential
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicPotentialConfigData : public GunnsBasicConductorConfigData
{
    public:
        /// @brief Default constructs this Basic Potential configuration data.
        GunnsBasicPotentialConfigData(const std::string& name                = "",
                                      GunnsNodeList*     nodes               = 0,
                                      const double       defaultConductivity = 0.0);

        /// @brief Default destructs this Basic Potential configuration data.
        virtual ~GunnsBasicPotentialConfigData();

        /// @brief Copy constructs this Basic Potential configuration data.
        GunnsBasicPotentialConfigData(const GunnsBasicPotentialConfigData& that);
    protected:

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicPotentialConfigData& operator =(const GunnsBasicPotentialConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Basic Potential Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Basic Potential
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicPotentialInputData : public GunnsBasicConductorInputData
{
    public:
        double mSourcePotential;                  /**< (--) Initial potential rise of the link */

        /// @brief    Default constructs this Basic Potential input data.
        GunnsBasicPotentialInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0,
                                     const double sourcePotential   = 0.0);

        /// @brief    Default destructs this Basic Potential input data.
        virtual ~GunnsBasicPotentialInputData();

        /// @brief    Copy constructs this Basic Potential input data.
        GunnsBasicPotentialInputData(const GunnsBasicPotentialInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsBasicPotentialInputData& operator =(const GunnsBasicPotentialInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Basic Potential Model
///
/// @details  The GUNNS basic potential models a potential difference within a network.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsBasicPotential : public GunnsBasicConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsBasicPotential);

    public:
        /// @brief Default Constructor
        GunnsBasicPotential();

        /// @brief Default Destructor
        virtual ~GunnsBasicPotential();

        /// @brief Initialize method
        void initialize(const GunnsBasicPotentialConfigData& configData,
                        const GunnsBasicPotentialInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1);

        /// @brief step method for updating the link
        virtual void step(const double dt);

        /// @brief Sets the potential rise created by the link
        void         setSourcePotential(const double toPotential);

        /// @brief Returns the potential rise created by the link
        double       getSourcePotential() const;

    protected:
        double mSourcePotential;        /**< (--) Generic potential rise created by the link */

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Updates the Admittance and Source terms for the link
        virtual void buildPotential();

        /// @brief Validates the link
        void validate() const;

        /// @brief Computes flux across the link
        virtual void computeFlux();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsBasicPotential(const GunnsBasicPotential& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsBasicPotential& operator =(const GunnsBasicPotential& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] toPotential (--) Desired potential across the link
///
/// @details  Sets the source potential term that controls the potential rise.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicPotential::setSourcePotential(const double toPotential)
{
    mSourcePotential = toPotential;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns mSourcePotential  -- Generic potential rise created by the link
///
/// @details Returns the source potential term that controls the potential rise.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsBasicPotential::getSourcePotential() const
{
    return mSourcePotential;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Protected method for building the admittance and potential terms
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicPotential::buildPotential()
{
    /// - The source potential term is defined as the potential rise created by the potential
    ///   source.  So that a positive source potential creates a flow in the positive direction
    ///   (port 0 to port 1), we reverse the sign in the source vector.
    mSourceVector[0] = -mSourcePotential * mAdmittanceMatrix[0];
    mSourceVector[1] = -mSourceVector[0];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Compute flux across the link, defined positive from port 0 to port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsBasicPotential::computeFlux()
{
    mFlux = mPotentialDrop * mAdmittanceMatrix[0] - mSourceVector[0];
}

#endif
