#ifndef GunnsFluidLeak_EXISTS
#define GunnsFluidLeak_EXISTS

/**
@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_LEAK    Leak Model
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Fluid Leak link model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsFluidLeak.o))

PROGRAMMERS:
- ((Kenneth McMurtrie) (Tietronix Software) (Add) (2011-10))

@{
*/

#include "core/GunnsFluidConductor.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Leak Model Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Leak link model configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLeakConfigData : public GunnsFluidConductorConfigData
{
    public:
        /// @brief    Default constructs this Leak configuration data.
        GunnsFluidLeakConfigData(const std::string& name                 = "",
                                 GunnsNodeList*     nodes                = 0,
                                 const double       maxConductivity      = 0.0,
                                 const double       expansionScaleFactor = 0.0);
        /// @brief    Copy constructs this Leak configuration data.
        GunnsFluidLeakConfigData(const GunnsFluidLeakConfigData& that);
        /// @brief    Default destructs this Leak configuration data.
        virtual ~GunnsFluidLeakConfigData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidLeakConfigData& operator =(const GunnsFluidLeakConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Fluid Leak Model Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Fluid
///           Leak link model input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLeakInputData : public GunnsFluidConductorInputData
{
    public:
        bool          mMalfLeakHoleFlag;   /**< (--)   Leak effective hole area malfunction flag */
        double        mMalfLeakHoleValue;  /**< (m2)   Leak effective hole area malfunction value */
        bool          mMalfLeakRateFlag;   /**< (--)   Leak rate malfunction flag */
        double        mMalfLeakRateValue;  /**< (kg/s) Leak rate malfunction value */
        /// @brief    Default constructs this Leak input data.
        GunnsFluidLeakInputData(const bool   malfBlockageFlag  = false,
                                const double malfBlockageValue = 0.0,
                                const bool   malfLeakHoleFlag  = false,
                                const double malfLeakHoleValue = 0.0,
                                const bool   malfLeakRateFlag  = false,
                                const double malfLeakRateValue = 0.0);
        /// @brief    Copy constructs this Leak input data.
        GunnsFluidLeakInputData(const GunnsFluidLeakInputData& that);
        /// @brief    Default destructs this Leak input data.
        virtual ~GunnsFluidLeakInputData();
    protected:
    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidLeakInputData& operator =(const GunnsFluidLeakInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Leak Model
///
/// @details  The GUNNS Fluid Leak link model provides a generic leak capability between two Nodes.
///           A leak is specified by a mass flow rate or effective area either at initialization or
///           dynamically (via simbus). The equivalent link conductivity is computed at malfunction
///           insertion and then maintained until the malfunction is removed. The leak link has the
///           source Node (leak from) on port 0 and the destination Node (leak to), usually either
///           vacuum or ambient, on port 1.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsFluidLeak : public  GunnsFluidConductor
{
    TS_MAKE_SIM_COMPATIBLE(GunnsFluidLeak);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool          mMalfLeakHoleFlag;   /**< (--)   Leak effective hole area malfunction flag */
        double        mMalfLeakHoleValue;  /**< (m2)   Leak effective hole area malfunction value */
        bool          mMalfLeakRateFlag;   /**< (--)   Leak initial rate malfunction flag */
        double        mMalfLeakRateValue;  /**< (kg/s) Leak initial rate malfunction value */
        /// @}

        /// @brief    Default constructs this Leak.
        GunnsFluidLeak();
        /// @brief    Default destructs this Leak.
        virtual ~GunnsFluidLeak();
        /// @brief    Initializes this Leak with configuration and input data.
        void initialize(const GunnsFluidLeakConfigData& configData,
                        const GunnsFluidLeakInputData&  inputData,
                        std::vector<GunnsBasicLink*>&   networkLinks,
                        const int                       port0,
                        const int                       port1);
        /// @brief    Sets and resets the leak effective hole area malfunction.
        void setMalfLeakHole(const bool flag = false, const double value = 0.0);
        /// @brief    Sets and resets the leak initial rate malfunction.
        void setMalfLeakRate(const bool flag = false, const double value = 0.0);

    protected:
        double        mPreviousLeakHole;   /**< (m2)   Previous leak hole area value */
        double        mPreviousLeakRate;   /**< (kg/s) Previous leak rate value */
        double        mLeakConductivity;   /**< (m2)   Conductivity equivalent to the leak */
        /// @brief    Validates the initialization of this Leak.
        void validate() const;
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief    Updates the state of this Leak.
        virtual void updateState(const double dt);
        /// @brief    Updates the leak conductance.
        virtual void updateLeakConductance();

    private:
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Copy constructor unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidLeak(const GunnsFluidLeak&);
        ////////////////////////////////////////////////////////////////////////////////////////////
        /// @details  Assignment operator unavailable since declared private and not implemented.
        ////////////////////////////////////////////////////////////////////////////////////////////
        GunnsFluidLeak& operator =(const GunnsFluidLeak&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Sets mLeakConductivity to the value that will create the desired mMalfLeakRateValue at
///           the current port node conditions.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsFluidLeak::updateLeakConductance()
{
    mLeakConductivity = GunnsFluidUtils::predictConductivity(mMalfLeakRateValue,
                                                             mMinLinearizationPotential,
                                                             mNodes[0]->getOutflow(),
                                                             mNodes[1]->getOutflow());
}

#endif
