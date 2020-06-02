#ifndef GunnsElectRealDiode_EXISTS
#define GunnsElectRealDiode_EXISTS

/**
@file
@brief    GUNNS Real Diode Link declarations

@defgroup  GUNNS_ELECTRICAL_DIODE_REAL    GUNNS Real Diode Link
@ingroup   GUNNS_ELECTRICAL_DIODE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Real Diode Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- (
   (GunnsElectRealDiode.o)
  )

PROGRAMMERS:
- (
   ((Jason Harvey) (CACI) (2017-10) (Initial))
  )

@{
*/

#include "core/GunnsBasicPotential.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Real Diode Configuration Data
///
/// @details  This class provides a structure for the Real Diode configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectRealDiodeConfigData : public GunnsBasicPotentialConfigData
{
    public:
        double mReverseConductivity; /**< (1/ohm) trick_chkpnt_io(**) Diode conductance for reverse bias. */
        double mVoltageDrop;         /**< (V)     trick_chkpnt_io(**) Diode junction voltage drop in forward bias. */
        /// @brief Default constructs this Real Diode configuration data.
        GunnsElectRealDiodeConfigData(const std::string& name               = "",
                                      GunnsNodeList*     nodes              = 0,
                                      const double       forwardConductance = 0.0,
                                      const double       reverseConductance = 0.0,
                                      const double       voltageDrop        = 0.0);
        /// @brief Default destructs this Real Diode configuration data.
        virtual ~GunnsElectRealDiodeConfigData();
        /// @brief Copy constructs this Real Diode configuration data.
        GunnsElectRealDiodeConfigData(const GunnsElectRealDiodeConfigData& that);

    private:
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectRealDiodeConfigData& operator =(const GunnsElectRealDiodeConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Real Diode Input Data
///
/// @details  This class provides a structure for the Real Diode input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectRealDiodeInputData : public GunnsBasicPotentialInputData
{
    public:
        double mReverseBias;                /**< (--) trick_chkpnt_io(**) Initial bias direction. */
        /// @brief    Default constructs this Real Diode input data.
        GunnsElectRealDiodeInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0,
                                     const bool   reverseBias       = false);
        /// @brief    Default destructs this Real Diode input data.
        virtual ~GunnsElectRealDiodeInputData();
        /// @brief    Copy constructs this Real Diode input data.
        GunnsElectRealDiodeInputData(const GunnsElectRealDiodeInputData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsElectRealDiodeInputData& operator =(const GunnsElectRealDiodeInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Real Diode Model
///
/// @details  This models a junction diode with voltage drop.  It allows forward current when the
///           forward voltage is greater than the voltage drop of the junction.  This state is
///           called forward bias.  If the forward voltage is less than the voltage drop, then this
///           state is called reverse bias and the diode switches to a lower (reverse) conductance,
///           thereby limiting reverse current.
///
///           The base class mDefaultConductivity term is used as the forward conductance, and the
///           base class mSourcePotential is used as the negative of the voltage drop.  For example
///           silicon diodes, which typically have a built-in potential of 0.7 V, would have
///           mSourcePotential = -0.7.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectRealDiode : public GunnsBasicPotential
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectRealDiode);

    public:
        /// @brief Default Constructor.
        GunnsElectRealDiode();
        /// @brief Default Destructor.
        virtual ~GunnsElectRealDiode();
        /// @brief Initialize method
        void initialize(const GunnsElectRealDiodeConfigData& configData,
                        const GunnsElectRealDiodeInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1);
        /// @brief Updates the state of the link.
        virtual void updateState(const double dt);
        /// @brief Minor Step method for non-linear Systems.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief Boolean for telling the solver if this is a non-linear link.
        virtual bool isNonLinear();
        /// @brief Returns the link's assessment of the solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief Sets the diode reverse-bias conductance.
        void         setReverseConductivity(const double conductivity);
        /// @brief Sets the diode junction voltage drop.
        void         setVoltageDrop(const double voltage);
        /// @brief Gets the diode junction voltage drop.
        double       getVoltageDrop() const;
        /// @brief Gets whether the diode is in reverse bias.
        bool         isReversedBias() const;

    protected:
        double mReverseConductivity; /**< (1/ohm) trick_chkpnt_io(**) Diode conductance for reverse bias. */
        double mVoltageDrop;         /**< (V)     trick_chkpnt_io(**) Diode junction voltage drop in forward bias. */
        bool   mReverseBias;         /**< (--)                        Diode is currently in reverse bias. */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Validates the link
        void validate() const;
        /// @brief Updates the bias direction and returns whether it changed.
        bool updateBias();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectRealDiode(const GunnsElectRealDiode& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectRealDiode& operator =(const GunnsElectRealDiode& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  Always true.
///
/// @details  This is always a non-linear link, as it must always be able to reject the network
///           solution when if flips bias direction.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectRealDiode::isNonLinear()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  conductivity  (1/ohm)  The given conductance value to use.
///
/// @details  Sets the reverse-bias conductivity drop to the given value.  The value is
///           lower-limited at zero, as negative values are not allowed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectRealDiode::setReverseConductivity(const double conductivity)
{
    mReverseConductivity = std::max(0.0, conductivity);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  voltage  (V)  The given voltage value to use.
///
/// @details  Sets the forward-bias diode junction voltage drop to the given value.  The value is
///           lower-limited at zero, as negative values are not allowed.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectRealDiode::setVoltageDrop(const double voltage)
{
    mVoltageDrop = std::max(0.0, voltage);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (V)  The current junction voltage drop of this diode.
///
/// @details  Returns the current junction voltage drop of this diode.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectRealDiode::getVoltageDrop() const
{
    return mVoltageDrop;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the diode is in reverse bias, False if it is forward bias.
///
/// @details  Returns whether the diode is in reverse bias.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectRealDiode::isReversedBias() const
{
    return mReverseBias;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  True if the diode flipped bias during this update.
///
/// @details  Computes the new bias direction based on current node potentials, and returns whether
///           the bias direction flipped.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectRealDiode::updateBias()
{
    const bool oldBias = mReverseBias;
    mReverseBias = (getDeltaPotential() < mVoltageDrop);
    return (mReverseBias != oldBias);
}

#endif
