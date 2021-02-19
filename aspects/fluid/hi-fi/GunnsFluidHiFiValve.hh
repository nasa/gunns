#ifndef GunnsFluidHiFiValve_EXISTS
#define GunnsFluidHiFiValve_EXISTS

/**
@file
@brief    GUNNS Fluid High-Fidelity Valve Model declarations

@defgroup  TSM_GUNNS_FLUID_HIFI_VALVE    High-Fidelity Valve Model
@ingroup   TSM_GUNNS_FLUID_HIFI

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Hi-Fi Valve link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidHiFiValve.o))

PROGRAMMERS:
- ((Jason Harvey) (L-3 Communications) (Prototype) (2015-09))

@{
*/

#include "aspects/fluid/hi-fi/GunnsFluidHiFiOrifice.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hi-Fi Valve Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Hi-Fi Valve link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiValveConfigData : public GunnsFluidHiFiOrificeConfigData
{
    public:
        /// @brief  Default constructs this Hi-Fi Valve configuration data.
        GunnsFluidHiFiValveConfigData(const std::string& name                 = "",
                                      GunnsNodeList*     nodes                = 0,
                                      const CoeffTypes   coefficientType      = DISCHARGE_COEFF,
                                      const double       coefficientValue     = 0.0,
                                      const double       throatDiameter       = 0.0,
                                      const double       criticalReynolds     = 0.0,
                                      const double       expansionScaleFactor = 0.0,
                                      const double       flowTuningFactor     = 1.0);
        /// @brief  Copy constructs this Hi-Fi Valve configuration data.
        GunnsFluidHiFiValveConfigData(const GunnsFluidHiFiValveConfigData& that);
        /// @brief  Default destructs this Hi-Fi Valve configuration data.
        virtual ~GunnsFluidHiFiValveConfigData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFiValveConfigData& operator =(const GunnsFluidHiFiValveConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hi-Fi Valve Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Hi-Fi Valve link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiValveInputData : public GunnsFluidHiFiOrificeInputData
{
    public:
        double mPosition;          /**< (--)   trick_chkpnt_io(**) (0-1) fractional position of this valve. */
        bool   mMalfLeakThruFlag;  /**< (--)   trick_chkpnt_io(**) Leak through rate malfunction flag. */
        double mMalfLeakThruValue; /**< (kg/s) trick_chkpnt_io(**) Leak through rate malfunction value. */
         /// @brief  Default constructs this Hi-Fi Valve input data.
        GunnsFluidHiFiValveInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0,
                                     const double position          = 0.0,
                                     const bool   malfLeakThruFlag  = false,
                                     const double malfLeakThruValue = 0.0);
        /// @brief  Copy constructs this Hi-Fi Valve input data.
        GunnsFluidHiFiValveInputData(const GunnsFluidHiFiValveInputData& that);
        /// @brief  Default destructs this Hi-Fi Valve input data.
        virtual ~GunnsFluidHiFiValveInputData();

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFiValveInputData& operator =(const GunnsFluidHiFiValveInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Hi-Fi Valve Model
///
/// @details  The GUNNS Fluid Hi-Fi Valve link model is the base class from which other hi-fi valves
///           derive. This extends the Hi-Fi Orifice model with a dynamic position and leak-thru
///           malfunction that modify the effective orifice flow area.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidHiFiValve : public GunnsFluidHiFiOrifice
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidHiFiValve);
    public:
        /// @brief  Enumeration of the position states of a Hi-Fi Valve.
        enum State {
            CLOSED        = 0,  ///< Valve is complete closed.
            OPENING       = 1,  ///< Valve is opening.
            TRANSITIONING = 2,  ///< Valve is transitioning between opening & closing.
            CLOSING       = 3,  ///< Valve is closing.
            OPEN          = 4   ///< Valve is completely opened.
        };

        /// @name   Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfLeakThruFlag;  /**< (--)   Malfunction leak thru rate flag. */
        double mMalfLeakThruValue; /**< (kg/s) Malfunction leak thru rate value. */
        /// @}

        /// @brief  Default constructs this Hi-Fi Valve.
        GunnsFluidHiFiValve();
        /// @brief  Default destructs this Hi-Fi Valve.
        virtual ~GunnsFluidHiFiValve();
        /// @brief  Initializes this Hi-Fi Valve with configuration and input data.
        void           initialize(const GunnsFluidHiFiValveConfigData& configData,
                                  const GunnsFluidHiFiValveInputData&  inputData,
                                  std::vector<GunnsBasicLink*>&        networkLinks,
                                  const int                            port0,
                                  const int                            port1);
        /// @brief  Returns the valve position of this Hi-Fi Valve.
        double         getPosition() const;
        /// @brief  Sets the valve position on this Hi-Fi Valve.
        void           setPosition(const double position);
        /// @brief  Sets the leak thru malfunction controls.
        void           setMalfLeakThru(const bool flag = false, const double value = 0.0);

    protected:
        double mPosition;          /**< (--)   Fractional position of this valve. */
        double mLeakArea;          /**< (m2)   Effective throat area of the thru-leak. */
        double mLastLeakRate;      /**< (kg/s) Previous leak thru malfunction value. */
        /// @brief  Validates the initialization of this Hi-Fi Valve.
        void           validate() const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void   restartModel();
        /// @brief  Updates the state of this Hi-Fi Valve.
        virtual void   updateState(const double dt);
        /// @brief  Returns the effective throat area of the device.
        virtual double getEffectiveArea() const;

    private:
        /// @brief  Define the number of ports this link class has.  All objects of the same link
        ///         class always have the same number of ports.  We use an enum rather than a static
        ///         const int so that we can reuse the NPORTS name and allow each class to define
        ///         its own value.
        enum {NPORTS = 2};
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsFluidHiFiValve(const GunnsFluidHiFiValve&);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsFluidHiFiValve& operator =(const GunnsFluidHiFiValve&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double  (--)  The fractional (0-1) open/close position of this valve.
///
/// @details  Getter method that returns the effective valve position including actual valve
///           position, blockage and leak-thru malfunctions.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsFluidHiFiValve::getPosition() const
{
    return mPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  position  (--)  Input commanded position (0-1).
///
/// @details  Sets the fractional valve position of this GUNNS Fluid Hi-Fi Valve link model.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidHiFiValve::setPosition(const double position)
{
    mPosition = position;
}

#endif

