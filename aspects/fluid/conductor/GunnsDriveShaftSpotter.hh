#ifndef GunnsDriveShaftSpotter_EXISTS
#define GunnsDriveShaftSpotter_EXISTS
/**
@file
@brief     GUNNS Drive Shaft Drive Shaft Network Spotter declarations

@defgroup  TSM_GUNNS_FLUID_CONDUCTOR_DRIVE_SHAFT_SPOTTER    GUNNS Drive Shaft Network Spotter
@ingroup   TSM_GUNNS_FLUID_CONDUCTOR

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

PURPOSE:   (Provides the classes for the GUNNS Drive Shaft Network Spotter.)

@details
REFERENCE:
- ()

ASSUMPTIONS AND LIMITATIONS:
- ()

LIBRARY DEPENDENCY:
- ((GunnsDriveShaftSpotter.o))

PROGRAMMERS:
- ((Chris Brownschidle) (CACI) (2016-09) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsNetworkSpotter.hh"
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Drive Shaft Network Spotter Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Drive
///           Shaft Network Spotter configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDriveShaftSpotterConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        double mFrictionConstant;   /**< (N*m*min/revolution) trick_chkpnt_io(**) Dynamic friction torque constant         */
        double mFrictionMinSpeed;   /**< (revolution/min)     trick_chkpnt_io(**) Minimum speed for dynamic friction       */
        double mInertia;            /**< (kg*m2)              trick_chkpnt_io(**) Inertia of the Drive shaft system        */
        /// @brief  Default constructs this GUNNS Drive Shaft Network Spotter configuration data.
        GunnsDriveShaftSpotterConfigData(const std::string& name,
                                         const double       frictionConstant = 0.0,
                                         const double       frictionMinSpeed = 0.0,
                                         const double       inertia = 0.0);
        /// @brief  Default destructs this GUNNS Drive Shaft Network Spotter configuration data.
        virtual ~GunnsDriveShaftSpotterConfigData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsDriveShaftSpotterConfigData(const GunnsDriveShaftSpotterConfigData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsDriveShaftSpotterConfigData& operator =(const GunnsDriveShaftSpotterConfigData& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Drive Shaft Network Spotter Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the GUNNS Drive
///           Shaft Network Spotter input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDriveShaftSpotterInputData : public GunnsNetworkSpotterInputData
{
    public:
        double mMotorSpeed;             /**< (revolution/min) Initial motor speed                    */
        bool   mMalfJamFlag;            /**< (--)             Initial Jam malf flag                  */
        double mMalfJamValue;           /**< (--)             Initial (>0) Jam malf value            */
        bool   mMalfSpeedOverrideFlag;  /**< (--)             Initial Speed override malf flag       */
        double mMalfSpeedOverrideValue; /**< (revolution/min) Initial Speed override malf value      */
        /// @brief  Default constructs this GUNNS Drive Shaft Network Spotter input data.
        GunnsDriveShaftSpotterInputData(const double motorSpeed   = 0.0);
        /// @brief  Default destructs this GUNNS Drive Shaft Network Spotter input data.
        virtual ~GunnsDriveShaftSpotterInputData();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsDriveShaftSpotterInputData(const GunnsDriveShaftSpotterInputData& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsDriveShaftSpotterInputData& operator =(const GunnsDriveShaftSpotterInputData& that);
};

// Forward-declare link classes used as pointers.
class GunnsFluidConductor;
class GunnsGasFan;
class GunnsGasTurbine;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    GUNNS Drive Shaft Network Spotter Class.
///
/// @details  This class implements a drive shaft, used to couple a gas turbine to a gas
///           fan/compressor. The drive shaft sums the external torques of all objects attached to
///           it, then calculates its shaft speed. Any number of fans and turbines can be attached
///           to the shaft. This speed is then given to all of the connected fans and turbines.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsDriveShaftSpotter : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(GunnsDriveShaftSpotter);
    public:
        /// @name    Malfunction targets.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfJamFlag;            /**< (--)              Jam malf flag               */
        double mMalfJamValue;           /**< (--)              (>0) Jam malf value         */
        bool   mMalfSpeedOverrideFlag;  /**< (--)              Speed override malf flag    */
        double mMalfSpeedOverrideValue; /**< (revolution/min)  Speed override malf value   */
        /// @}
        /// @brief   Default constructor.
        GunnsDriveShaftSpotter();
        /// @brief   Default destructor.
        virtual     ~GunnsDriveShaftSpotter();
        /// @brief   Initializes the GUNNS Drive Shaft Network Spotter with configuration and
        ///          input data.
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData);
        /// @brief   Steps the GUNNS Drive Shaft Network Spotter prior to the GUNNS solver step.
        virtual void stepPreSolver(const double dt);
        /// @brief   Steps the GUNNS Drive Shaft Network Spotter after the GUNNS solver step.
        virtual void stepPostSolver(const double dt);
        /// @brief   Adds object pointers to the turbine and fan vectors
        void addImpeller(GunnsFluidConductor* object);

    protected:
        double                          mFrictionConstant;  /**< (N*m*min/revolution) trick_chkpnt_io(**) Dynamic friction torque constant      */
        double                          mFrictionMinSpeed;  /**< (revolution/min)     trick_chkpnt_io(**) Minimum speed for dynamic friction    */
        double                          mInertia;           /**< (kg*m2)              trick_chkpnt_io(**) Inertia of the motor+load mass        */
        double                          mMotorSpeed;        /**< (revolution/min)                         Motor speed                           */
        bool                            mInitFlag;          /**< (--)                 trick_chkpnt_io(**) Instance has been initialized         */
        std::vector<GunnsGasTurbine*>   mTurbRef;           /**< (--)                 trick_chkpnt_io(**) vector of pointers to turbines        */
        std::vector<GunnsGasFan*>       mFanRef;            /**< (--)                 trick_chkpnt_io(**) vector of pointers to fans            */
        double                          mFrictionTorque;    /**< (N*m)                                    Dynamic friction torque               */
        double                          mTotalExternalLoad; /**< (N*m)                                    Total external torque load on shaft   */

        /// @brief   Validates the supplied configuration data.
        const GunnsDriveShaftSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        /// @brief   Validates the supplied input data.
        const GunnsDriveShaftSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsDriveShaftSpotter(const GunnsDriveShaftSpotter& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsDriveShaftSpotter& operator =(const GunnsDriveShaftSpotter& that);
};

/// @}

#endif
