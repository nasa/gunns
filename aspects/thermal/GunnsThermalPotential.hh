#ifndef GunnsThermalPotential_EXISTS
#define GunnsThermalPotential_EXISTS

/************************** TRICK HEADER **********************************************************
@defgroup  TSM_GUNNS_THERMAL_POTENTIAL    GUNNS Thermal Potential Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (The basic function of this class is to apply a constant temperature to a node. It serves as the
    electrical analog of a voltage source for the thermal aspect. GunnsThermalPotential is derived
    from GunnsBasicPotential, but has the added capability of storing the net heat flux into the
    node it's attached to. GunnsThermalPotential provides the most up-to-date value of this heat
    flux to networks connected to it via the simbus.
   )

REFERENCE:
   ()

ASSUMPTIONS AND LIMITATIONS:
   ()

LIBRARY_DEPENDENCY:
   (
    (GunnsThermalPotential.o)
   )

PROGRAMMERS:
   (
    (Joe Valerioti)     (L3) (2011-11) (Initial Prototype)
    (Ulisses Hernandez) (L3) (2012-01) (Unit-testing and code review)
   )
@{
**************************************************************************************************/

#include "core/GunnsBasicPotential.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Potential Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal
///           Potential configuration data.
///////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPotentialConfigData : public GunnsBasicPotentialConfigData
{
    public:
        /// @brief   Default constructs this Thermal Potential configuration data.
        GunnsThermalPotentialConfigData(const std::string& name = "unnamed potential",
                                           GunnsNodeList* nodes = 0,
                              const double defaultConductivity  = 0.0);

        /// @brief   Default destructs this Thermal Potential configuration data.
        virtual ~GunnsThermalPotentialConfigData();

        /// @brief   Copy constructs this Thermal Potential configuration data.
        GunnsThermalPotentialConfigData(const GunnsThermalPotentialConfigData& that);

    protected:
        // Nothing

    private:
        /// @brief   Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPotentialConfigData& operator =(const GunnsThermalPotentialConfigData&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Potential Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal
///           Potential input data.
///////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPotentialInputData : public GunnsBasicPotentialInputData
{
    public:
        /// @brief   Default constructs this Thermal Potential input data.
        GunnsThermalPotentialInputData(const bool    malfBlockageFlag   = false,
                                       const double  malfBlockageValue  = 0.0,
                                       const double  sourcePotential    = 0.0);

        /// @brief   Default destructs this Thermal Potential input data.
        virtual ~GunnsThermalPotentialInputData();

        /// @brief   Copy constructs this Thermal Potential input data.
        GunnsThermalPotentialInputData(const GunnsThermalPotentialInputData& that);

    protected:
        // Nothing

    private:
        /// @details Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPotentialInputData& operator =(const GunnsThermalPotentialInputData&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Thermal Potential Model
///
/// @details  The GUNNS Thermal Potential models a potential difference within a network.
///////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalPotential : public GunnsBasicPotential
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermalPotential);

    public:
        /// @brief Default Constructor
        GunnsThermalPotential();

        /// @brief Default Destructor
        virtual ~GunnsThermalPotential();

        /// @brief Initialize method
        void initialize(const GunnsThermalPotentialConfigData&  configData,
                        const GunnsThermalPotentialInputData&   inputData,
                        std::vector<GunnsBasicLink*>&           networkLinks,
                        const int                               port0,
                        const int                               port1);

        /// @brief  Prepares the object for a simbus write by setting the mHeatFluxIntoNode member.
        ///         Overwrites a base-class method that executes right after GUNNS updates every
        ///         link. Used here to update mHeatFluxIntoNode.
        void processOutputs();

        /// @brief  Checks for valid implementation-specific port node assignment by overwriting
        ///         a method in GunnsBasicLink.
        virtual bool checkSpecificPortRules(const int port, const int node) const;

    protected:
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        double mHeatFluxIntoNode; /**< (W)  Net heat flux into node this potential is linked to. */

    private:
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsThermalPotential(const GunnsThermalPotential& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsThermalPotential& operator =(const GunnsThermalPotential& that);
};

/// @}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Prepares the object for simbus write by updating the mHeatFluxIntoNode of the
///           potential. The processOutputs() method is the very last call by the Gunns::step()
///           method. It overwrites the virtual method in the BasicLink base-class. When the
///           GunnsThermalPotential is connected to other networks via the simbus, it needs to pass
///           the most updated value of net heat flux. Member mHeatFluxIntoNode is saved as the
///           negative of the heat flux because mFlux reports the flux from port0, which is
///           connected to vacuum, to port1.
///////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsThermalPotential::processOutputs()
{
    mHeatFluxIntoNode = -mFlux;
}

#endif
