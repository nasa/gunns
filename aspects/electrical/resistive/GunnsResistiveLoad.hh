#ifndef GunnsResistiveLoad_EXISTS
#define GunnsResistiveLoad_EXISTS

/**
@defgroup  TSM_GUNNS_ELECTRICAL_RESISTIVE_LOAD    GUNNS Resistive Load Link
@ingroup   TSM_GUNNS_ELECTRICAL_RESISTIVE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Resistive Load Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((aspects/electrical/resistive/GunnsResistiveLoad.o))

PROGRAMMERS:
- ((Mike Moore) (L3) (2013-05)
@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "math/MsMath.hh"
#include "aspects/electrical/resistive/GunnsElectricalResistor.hh"
#include "aspects/electrical/Switch/GunnsElectSwitchUtil.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Resistive Load Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Resistive Load
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsResistiveLoadConfigData : public GunnsElectricalResistorConfigData
{
    public:
        double mUnderVoltageLimit;        /**< (V) trick_chkpnt_io(**) Minimum voltage that this load can operate at. */
        double mLoadSwitchResistance;     /**< (ohm) trick_chkpnt_io(**) Resistance of the switch in front of the load. */
        double mPosTripLimit;             /**< (amp) trick_chkpnt_io(**) Load's positive current limit */
        double mNegTripLimit;             /**< (amp) trick_chkpnt_io(**) Load's negative current limit */
        int    mTripPriority;             /**< (--) trick_chkpnt_io(**) minor frame that the load switch is allowed to trip open at to ensure
                                                    that a series of switches trip in the correct order */
        /// @brief Default constructs this resistive load configuration data.
        GunnsResistiveLoadConfigData(const std::string& name                  = "",
                                      GunnsNodeList*     nodes                = 0,
                                      const double       loadResistance       = 0.0,
                                      const double       electricalEfficiency = 0.0,
                                      const double       underVoltageLimit    = 0.0,
                                      const double       switchResistance     = 0.0,
                                      const double       posTripLimit         = 0.0,
                                      const double       negTripLimit         = 0.0,
                                      const int          tripPriority         = 0);

        /// @brief Default destructs this resistive load configuration data.
        virtual ~GunnsResistiveLoadConfigData();

        /// @brief Copy constructs this resistive load configuration data.
        GunnsResistiveLoadConfigData(const GunnsResistiveLoadConfigData& that);
    protected:

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsResistiveLoadConfigData& operator =(const GunnsResistiveLoadConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Gunns Resistive Load Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the resistive load
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsResistiveLoadInputData : public GunnsElectricalResistorInputData
{
    public:
        bool mInitialSwitchPosition;         /**< (V)  trick_chkpnt_io(**) Inital position of the load switch. */
        /// @brief    Default constructs this resistive load input data.
        GunnsResistiveLoadInputData(const bool   malfBlockageFlag  = false,
                                     const double malfBlockageValue = 0.0,
                                     const bool   initialSwitchPosition = true);

        /// @brief    Default destructs this resistive load input data.
        virtual ~GunnsResistiveLoadInputData();

        /// @brief    Copy constructs this resistive load input data.
        GunnsResistiveLoadInputData(const GunnsResistiveLoadInputData& that);
    protected:

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsResistiveLoadInputData& operator =(const GunnsResistiveLoadInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Resistive Load Class
///
/// @details  The Resistive Load class is designed to create a resistive electrical load within
///           a GUNNS network. Unlike classes derived from UserLoadBase, this class derives from
///           the GunnsElectricalResistor link. This allows you to add simple resistive loads
///           directly into a GUNNS network without the overhead associated with using UserLoads.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsResistiveLoad : public GunnsElectricalResistor
{
        TS_MAKE_SIM_COMPATIBLE(GunnsResistiveLoad);

    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsElectSwitchUtil           mLoadSwitch;        /**< *i (--) Switch utility object for modeling the load switch (behaves like a fuse)*/
        GunnsElectSwitchUtilConfigData mLoadSwitchConfig;  /**< *i (--) Switch utility requires config data to be initialized */
        GunnsElectSwitchUtilInputData  mLoadSwitchInput;   /**< *i (--) Switch utility requires input data to be initialized */
        /// @}

        /// @brief Default Resistive Load Constructor
        GunnsResistiveLoad();

        /// @brief Default Resistive Load Destructor
        virtual ~GunnsResistiveLoad();

        /// @brief Resistive Load process outputs. Where PowerValid is set
        virtual void processOutputs();

        /// @brief Initialize method
        void initialize(const GunnsResistiveLoadConfigData& configData,
                        const GunnsResistiveLoadInputData&  inputData,
                        std::vector<GunnsBasicLink*>&       networkLinks,
                        const int                           port0,
                        const int                           port1);

        /// @brief Resistive Load update state
        virtual void updateState(const double dt);

        /// @brief Returns the resistive load's voltage.
        double         getVoltage() const;

        /// @brief Returns the resistive load's under voltage limit.
        double         getUnderVoltageLimit() const;

        /// @brief Returns the power valid flag - set true by step method if voltage is above
        /// the under voltage limit.
        bool getPowerValid() const;

        /// @brief Setter for the load's switch state. Pass true to close, and false to open.
        void setLoadSwitchClosed(const bool switchCmd);


    protected:
        /// @brief Validates the link
        void validate() const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        double mVoltage;                     /**< *i (V) Voltage that the load sees */
        double mUnderVoltageLimit;           /**< (V)  trick_chkpnt_io(**) Minimum voltage that this load can operate at. */
        double mLoadConfigResistance;        /**< (ohm)  trick_chkpnt_io(**) Configured load resistance value. Used if the resistance is not updated by sim-bus. */

        bool mPowerValid;                    /**< (--) Flag indicating if the load has valid power.   */
        bool mSwitchPrevClosed;              /**< (--) Flag indicating if the switch status on the last pass.   */

    private:

        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsResistiveLoad(const GunnsResistiveLoad& that);

        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsResistiveLoad& operator =(const GunnsResistiveLoad& that);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The voltage of the resistive load.
///
/// @details  Returns the voltage that this resistive load sees.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsResistiveLoad::getVoltage() const
{
    return mVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double -- The under voltage limit of the resistive load
///
/// @details  Returns the under voltage limit of the resistive load
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsResistiveLoad::getUnderVoltageLimit() const
{
    return mUnderVoltageLimit;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool -- The power valid status of the resistive load
///
/// @details  Returns the power valid status of the resistive load
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsResistiveLoad::getPowerValid() const
{
    return mPowerValid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details Sets the load switch's state. Pass true to close, and false to open.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsResistiveLoad::setLoadSwitchClosed(const bool switchCmd)
{
    mLoadSwitch.setSwitchCommandedClosed(switchCmd);
}

/// @}

#endif
