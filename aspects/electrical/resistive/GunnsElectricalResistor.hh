#ifndef GunnsElectricalResistor_EXISTS
#define GunnsElectricalResistor_EXISTS

/**
@defgroup  TSM_GUNNS_ELECTRICAL_RESISTIVE_LINK   GUNNS Electrical Resistor Link
@ingroup   TSM_GUNNS_ELECTRICAL_RESISTIVE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Resistor Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsElectricalResistor.o))

PROGRAMMERS:
- ((Mike Moore) (L3) (2013-05)
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "math/MsMath.hh"
#include "core/GunnsBasicLink.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Resistor Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Electrical
///           Reistor configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectricalResistorConfigData : public GunnsBasicConductorConfigData
{
    public:
        double mResistance;             /**< (ohm) trick_chkpnt_io(**) Default resistance of the link */
        double mElectricalEfficiency;   /**< (--) trick_chkpnt_io(**) Default electrical efficiency of the link */

        /// @brief Default constructs this electrical resistor configuration data.
        GunnsElectricalResistorConfigData(const std::string& name                = "",
                                      GunnsNodeList*     nodes               = 0,
                                      const double       resistance = 0.0,
                                      const double       electricalEfficiency = 0.0);

        /// @brief Default destructs this electrical resistor configuration data.
        virtual ~GunnsElectricalResistorConfigData();

        /// @brief Copy constructs this electrical resistor configuration data.
        GunnsElectricalResistorConfigData(const GunnsElectricalResistorConfigData& that);
    protected:

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectricalResistorConfigData& operator =(const GunnsElectricalResistorConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Resistor Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Electrical
///           Resistor input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectricalResistorInputData : public GunnsBasicConductorInputData
{
    public:
        /// @brief    Default constructs this electrical resistor input data.
        GunnsElectricalResistorInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0);

        /// @brief    Default destructs this electrical resistor input data.
        virtual ~GunnsElectricalResistorInputData();

        /// @brief    Copy constructs this electrical resistor input data.
        GunnsElectricalResistorInputData(const GunnsElectricalResistorInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsElectricalResistorInputData& operator =(const GunnsElectricalResistorInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Resistor Class
///
/// @details  The Electrical Resistor class is the equivalent of an electrical conduction path.
///           This class simply derives from basic conductor, adds a resistance term, and inverts
///           the resistance in the updateState method before passing it as conductance to the
///           solver.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectricalResistor : public GunnsBasicConductor
{
        TS_MAKE_SIM_COMPATIBLE(GunnsElectricalResistor);

    public:
        /// @brief Default Electrical Resistor Constructor
        GunnsElectricalResistor();

        /// @brief Default Electrical Resistor Destructor
        virtual ~GunnsElectricalResistor();

        /// @brief Electrical Resistor update state
        virtual void updateState(const double dt);

        /// @brief Method for computing the flows across the link
        virtual void computeFlows(const double dt);

        /// @brief Method for computing the electrical power.
        virtual void computePower();

        /// @brief Initialize method
        void initialize(const GunnsElectricalResistorConfigData& configData,
                        const GunnsElectricalResistorInputData&  inputData,
                        std::vector<GunnsBasicLink*>&        networkLinks,
                        const int                            port0,
                        const int                            port1);

        /// @brief Returns the electrical resistance
        double         getResistance() const;

        /// @brief Returns the voltage into the link
        double         getVoltageIn() const;

        /// @brief Returns the voltage out of the link
        double         getVoltageOut() const;

        /// @brief Returns the voltage of the link
        double         getVoltageDrop() const;

        /// @brief Returns the current through the link
        double         getCurrent() const;

        /// @brief Returns the electrical efficiency of the link
        double         getElectricalEfficiency() const;

        /// @brief Returns the electrical waste heat generated by the link
        double         getWasteHeat() const;

    protected:
        /// @brief Calculates the waste heat of the link
        double calculateWasteHeat() const;

        /// @brief Validates the link
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        double mResistance;             /**< (ohm) trick_chkpnt_io(**) Electrical resistance of the link */
        double mVoltageIn;              /**< (V)   Voltage into the resistor. This is defined as
                                                   the potential of the node hooked up to port 0
                                                   of the link */
        double mVoltageOut;             /**< (V)   Voltage out of the resistor. This is defined as
                                                   the potential of the node hooked up to port 1
                                                   of the link */
        double mVoltageDrop;            /**< (V)   Voltage drop across the resistor */
        double mCurrent;                /**< (amp) Current through the resistor */
        double mElectricalEfficiency;   /**< (--) trick_chkpnt_io(**) Electrical efficiency of the resistor */
        double mWasteHeat;              /**< (W)  Waste electrical heat generated by the resistor */

    private:

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectricalResistor(const GunnsElectricalResistor& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectricalResistor& operator =(const GunnsElectricalResistor& that);
};


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Override base class implementation in order to get the sign correct.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectricalResistor::computePower()
{
    mPower = mFlux * getDeltaPotential(0, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The electrical resistance of the link
///
/// @details  Returns the electrical resistance of the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectricalResistor::getResistance() const
{
    return mResistance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The voltage into the link
///
/// @details  Returns the voltage into the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectricalResistor::getVoltageIn() const
{
    return mVoltageIn;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The voltage out of the link
///
/// @details  Returns the voltage out of the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectricalResistor::getVoltageOut() const
{
    return mVoltageOut;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The voltage drop across the resistor
///
/// @details  Returns the voltage drop across the resistor
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectricalResistor::getVoltageDrop() const
{
    return mVoltageDrop;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The current through the link
///
/// @details  Returns the current through the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectricalResistor::getCurrent() const
{
    return mCurrent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The electrical efficiency of the link
///
/// @details  Returns the electrical efficiency of the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectricalResistor::getElectricalEfficiency() const
{
    return mElectricalEfficiency;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The waste heat generated by the link
///
/// @details  Returns the waste heat generated by the link
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectricalResistor::getWasteHeat() const
{
    return mWasteHeat;
}
/// @}

#endif
