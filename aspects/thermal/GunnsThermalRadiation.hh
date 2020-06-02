#ifndef GunnsThermalRadiation_EXISTS
#define GunnsThermalRadiation_EXISTS

/********************** TRICK HEADER **************************************************************
@defgroup  TSM_GUNNS_THERMAL_RADIATION    GUNNS Thermal Radiation Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
  (Models a thermal radiation effect. The basic function of this class is to change how admittance
   is calculated in order to create this thermal radiation effect. The GUNNS link BasicConductor
   serves as the parent class for ThermalRadiation. Due to this, it felt redundant to declare
   thermal-radiation-specific members. To remedy this, members declared in BasicConductor are used
   in ThermalRadiation methods (e.g. conductivity in the context of thermal radiation is the
   radiation coefficient. Instead of declaring members like radiationk, member nomenclature from
   BasicConductor is carried into ThermalRadiation).
  )

REFERENCE:
  ()

ASSUMPTIONS AND LIMITATIONS:
  (GunnsThermalRadiation assumes Radiation link has constant conductivity.)

LIBRARY DEPENDENCY:
  (
   (GunnsThermalRadiation.o)
  )

PROGRAMMERS:
  (
   (Jason Harvey)      (L3) (2011-02) (Initial Prototype)
   (Kevin Supak)       (L3) (2011-02) (Updated to Coding Standards)
   (Ulisses Hernandez) (L3) (2012-01) (Updates, unit-test and code review)
  )

@{
**************************************************************************************************/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsThermalRadiation Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal
///           Radiation configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalRadiationConfigData : public GunnsBasicConductorConfigData
{
    public:
        /// @brief   Default constructs this GunnsThermalRadiation configuration data.
        GunnsThermalRadiationConfigData(const std::string& name = "unnamed radiation",
                                           GunnsNodeList* nodes = 0,
                               const double defaultConductivity = 0.0);

        /// @brief   Default destructs this GunnsThermalRadiation configuration data.
        virtual ~GunnsThermalRadiationConfigData();

        /// @brief   Copy constructs this GunnsThermalRadiation configuration data.
        GunnsThermalRadiationConfigData(const GunnsThermalRadiationConfigData& that);

    protected:
        // Nothing

    private:
        /// @brief   Assignment operator unavailable since declared private and not implemented.
        GunnsThermalRadiationConfigData& operator = (const GunnsThermalRadiationConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsThermalRadiation Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Thermal
///           Radiation input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalRadiationInputData : public GunnsBasicConductorInputData
{
    public:
        double iViewScalar; /**< (--) trick_chkpnt_io(**) view scalar, which scales mEffectiveConductivity;
                                                          used to simulate operations like radiator retraction */

        /// @brief   Default constructs this Thermal Radiation input data.
        GunnsThermalRadiationInputData(const bool   malfBlockageFlag  = false,
                                       const double malfBlockageValue = 0.0,
                                       const double viewScalar        = 1.0);

        /// @brief   Default destructs this Thermal Radiation input data.
        virtual ~GunnsThermalRadiationInputData();

        /// @brief   Copy constructs this Thermal Radiation input data.
        GunnsThermalRadiationInputData(const GunnsThermalRadiationInputData& that);

    protected:
        // Nothing

    private:
        /// @details Assignment operator unavailable since declared private and not implemented.
        GunnsThermalRadiationInputData& operator = (const GunnsThermalRadiationInputData& that);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GunnsThermalRadiation Class
///
/// @details  The purpose of this class is to create the thermal radiation effect, where heat flux
///           is a function of T^4. The updateState method is overridden from GunnsBasicConductor
///           to compute the linearized admittance.
///////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalRadiation : public GunnsBasicConductor
{
        TS_MAKE_SIM_COMPATIBLE(GunnsThermalRadiation);

    public:

        /// @brief   Default Constructor
        GunnsThermalRadiation();

        /// @brief   Default Destructor
        virtual ~GunnsThermalRadiation();

        /// @brief   ThermalRadiation initialization method
        void initialize(const GunnsThermalRadiationConfigData& configData,
                        const GunnsThermalRadiationInputData&  inputData,
                        std::vector<GunnsBasicLink*>&          networkLinks,
                        const int                              port0,
                        const int                              port1);

        /// @brief   Sets the mViewScalar value
        void   setViewScalar(const double scalar);

        /// @brief   Returns the mViewScalar value
        double getViewScalar() const;

    protected:
        double mViewScalar; /**< (--) view scalar, which scales mEffectiveConductivity;
                                      used to simulate operations like radiator retraction */

        /// @brief   Calculates the Admittance for the Radiation Link
        virtual void updateState(const double dt);

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @brief   Copy constructor unavailable since declared private and not implemented.
        GunnsThermalRadiation(const GunnsThermalRadiation& that);

        /// @brief   Assignment operator unavailable since declared private and not implemented.
        GunnsThermalRadiation& operator = (const GunnsThermalRadiation& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Value to scale the effective conductivity by.
///
/// @details  Sets the mViewScalar value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsThermalRadiation::setViewScalar(const double scalar)
{
    mViewScalar = scalar;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  Current value of the effective conductivity view scalar.
///
/// @details  Returns the mViewScalar value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsThermalRadiation::getViewScalar() const
{
    return mViewScalar;
}

#endif
