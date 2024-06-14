#ifndef GunnsElectPvString2_EXISTS
#define GunnsElectPvString2_EXISTS

/**
@file     GunnsElectPvString2.hh
@brief    GUNNS Electrical Photovoltaic String Model version 2 declarations

@defgroup  GUNNS_ELECTRICAL_PHOTOVOLTAIC_STRING    Photovoltaic String Model version 2
@ingroup   GUNNS_ELECTRICAL_PHOTOVOLTAIC

@copyright Copyright 2024 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Photovoltaic String Model version 2.  Refer to the GunnsElectPvString2
   class details below.)

REFERENCE:
- (("Lambert W-function simplified expressions for photovoltaic current-voltage modelling" TODO))

ASSUMPTIONS AND LIMITATIONS:
- ((Limitation: for some cell configurations, the equivalent circuit model's computeVoltage function
    can't compute cell voltage for the full range of currents.)
   (Limitation: the equivalent circuit model's computeVoltage and computeCurrent functions are
    generally only accurate to within 5% error.))

LIBRARY DEPENDENCY:
- ((GunnsElectPvString2.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2024-05) (Initial))

@{
*/

#include <string>
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "GunnsElectPvString.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic Cell Version 2 Equivalent Circuit Properties.
///
/// @details  This is a single-diode equivalent circuit model of a PV cell for use by the
///           Photovoltaic Cell Version 2 model.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvCellEquivCircuit2 : public GunnsElectPvCellEquivCircuit
{
    public:
        /// @brief Default constructs this Photovoltaic Cell Version 2 Equivalent Circuit Properties.
        GunnsElectPvCellEquivCircuit2();
        /// @brief Default destructs this Photovoltaic Cell Version 2 Equivalent Circuit Properties.
        virtual ~GunnsElectPvCellEquivCircuit2();
        /// @brief Initializes this Photovoltaic Cell Version 2 Equivalent Circuit Properties.
        virtual void initialize(const GunnsElectPvCellConfigData* configData, const std::string& name);
        /// @brief Updates this Photovoltaic Cell Version 2 Equivalent Circuit Properties.
        virtual void update(const GunnsElectPvCellEquivCircuit* refCell, const double temperature,
                            const double photoFlux, const double degradation = 0.0);
        /// @brief Computes remaining equivalent circuit properties after initialization or update.
        virtual void derive();
        /// @brief Returns the cell current at the given voltage.
        virtual double computeCurrent(const double voltage) const;
        /// @brief Returns the cell voltage at the given current.
        virtual double computeVoltage(const double current) const;

    protected:
        /// @brief Computes the efficiency and fill factor.
        void computeEfficiency();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvCellEquivCircuit2(const GunnsElectPvCellEquivCircuit2& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Photovoltaic String Model version 2.
///
/// @details  This models a string of photovoltaic cells in series as an equivalent circuit.  This
///           is a redesign of GunnsElectPvString with several important changes:
///
///           - Reference cell configuration data more closely aligns with the parameters that are
///             typically provided by a hardware vendor, i.e. open-circuit, short-circuit, and
///             maximum power points.
///           - Fixes the bad implementation of cell efficiency from the original model.  Here,
///             efficiency is estimated as an output of the model for verification, rather than as
///             an input.
///           - More accurately models the performance curve of the cell, at the expense of more
///             compute time.
///           - Models a configurable equivalent diode ideality value.
///           - Although this has improved realism when loaded at a given voltage, this model has
///             limited ability to load at arbitrary power loads or conductances, and is only suited
///             for use with shunting and MPP tracking regulators.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectPvString2 : public GunnsElectPvString
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectPvString2);
    public:
        /// @brief Default constructor for this Photovoltaic String Utility version 2.
        GunnsElectPvString2();
        /// @brief Normal constructor for this Photovoltaic String Utility version 2.
        GunnsElectPvString2(const GunnsElectPvStringConfigData* configData,
                            const GunnsElectPvStringInputData*  inputData);
        /// @brief Destructs this Photovoltaic String Utility version 2.
        virtual ~GunnsElectPvString2();
        /// @brief Initializes this Photovoltaic String Utility version 2.
        virtual void initialize(const std::string& name);
        /// @brief Updates this Photovoltaic String Utility version 2 state.
        virtual void update();
        /// @brief Loads the string at the given power output.
        virtual void loadAtPower(const double power, const bool shortSide);
        /// @brief Loads the string at the given terminal voltage.
        virtual void loadAtVoltage(const double v1);
        /// @brief Loads the string with the given conductive load.
        virtual void loadAtConductance(const double g);
        /// @brief Returns current output of the string at the given terminal voltage.
        virtual double predictCurrentAtVoltage(const double voltage) const;

    protected:
        /// @brief Validates the initialization of this Gunns Photovoltaic String Utility version 2.
        virtual void validate() const;
        /// @brief Computes the Maximum Power Point parameters.
        void updateMpp();

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsElectPvString2(const GunnsElectPvString2& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsElectPvString2& operator =(const GunnsElectPvString2& that);
};

/// @}

#endif
