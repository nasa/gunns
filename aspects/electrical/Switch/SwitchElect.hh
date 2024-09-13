#ifndef SwitchElect_EXISTS
#define SwitchElect_EXISTS

/// @defgroup TSM_GUNNS_ELECTRICAL_SWITCH_LINK Link
/// @ingroup  TSM_GUNNS_ELECTRICAL_SWITCH
/// @copyright Copyright 2019 United States Government as represented by the Administrator of the
///            National Aeronautics and Space Administration.  All Rights Reserved.
/// @{
/*********************** TRICK HEADER *************************************************************

 PURPOSE:
 (The SwitchElect class is a standalone switch object that can be placed in a GUNNS network, as
  opposed to the switch class which is not a GUNNS object)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 (none)

 LIBRARY DEPENDENCY:
 (
 (SwitchElect.o)
 )

 PROGRAMMERS:
 (
 (Minh Tran) (L-3) (July 2011) (Initial Prototype)
 (Nicholas Kaufmann (L-3) (Oct 2011) (Refactor code to derive from basic conductor)
 (Philip Trottman) (USA) (Mar 2012) (Refactor code to support Switch class updates)
 (Mark Wiederholt) (Lockheed) (January 2014) (Fixed the trip level checking to work properly and work with SwitchElectWithSensor)
 )
 **************************************************************************************************/
#include <vector>
#include <string>
#include "core/GunnsBasicConductor.hh"
#include "aspects/electrical/Switch/GunnsElectSwitchUtil.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SwitchElect Configuration Data
/// -
/// @details  The  purpose of this class is to provide a data structure for the SwitchElect
///           configuration data.
//////////////////////////////////////////////////////////////////////////////////////////////////
class SwitchElectConfigData: public GunnsBasicConductorConfigData {
 public:

    GunnsElectSwitchUtilConfigData mSwitchConfigData; /**< (--) trick_chkpnt_io(**) Config data for embedded switch */
    std::string mSwitchName;            /**< (--) trick_chkpnt_io(**) Embedded switch name for messages */

    /// @brief Default SwitchElect Configuration Data Constructor
    SwitchElectConfigData(const std::string name = "Unnamed Link",
            GunnsNodeList* nodes = 0,
            const GunnsElectSwitchUtilConfigData& config = 0);

    /// @brief Default SwitchElect Configuration Data Destructor
    virtual ~SwitchElectConfigData();

    /// @brief Copy constructs SwitchElect Configuration Data.
    SwitchElectConfigData(const SwitchElectConfigData& that);

 private:
    /// @brief  Operator = is not available since declared private and not implemented.
    SwitchElectConfigData& operator =(const SwitchElectConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SwitchElect Input Data
/// -
/// @details  This provides a data structure for the Switch Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class SwitchElectInputData : public GunnsBasicConductorInputData {
 public:

    GunnsElectSwitchUtilInputData mSwitchInputData;  /**< (--) trick_chkpnt_io(**) */


    /// @brief Default constructs this Switch Elect input data.
    SwitchElectInputData(const bool   malfBlockageFlag  = false,
                         const double malfBlockageValue = 0.0,
                         const GunnsElectSwitchUtilInputData& inputData = 0);

    /// @brief Copy constructs this Switch Elect input data.
    SwitchElectInputData(const SwitchElectInputData& that);

    /// @brief Default destructs this Switch Elect input data.
    virtual ~SwitchElectInputData();

 private:
    /// @brief  Assignment operator unavailable since declared private and not implemented.
    SwitchElectInputData& operator =(const SwitchElectInputData&);
};

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    SwitchElect
/// -
/// @details  The  purpose of this class is to provide a data structure for the SwitchElect.
//////////////////////////////////////////////////////////////////////////////////////////////////
class SwitchElect: public GunnsBasicConductor {
    TS_MAKE_SIM_COMPATIBLE(SwitchElect);

 public:
    /// @name    Embedded objects.
    /// @{
    /// @details Embedded objects are public to allow access from the Trick events processor.
    GunnsElectSwitchUtil mSwitch;                  /**< (--)  Switch Class  */
    /// @}

    /// @brief Default SwitchElect Configuration Data Constructor
    SwitchElect();

    /// @brief Default SwitchElect Destructor
    virtual ~SwitchElect();

    /// @brief Initialize method as Gunns network links
    void initialize(const SwitchElectConfigData& configData,
                    const SwitchElectInputData& inputData,
                    std::vector<GunnsBasicLink*>& networkLinks,
                    const int port0,
                    const int port1);

    /// @brief Updates the link during a frame
    virtual void updateState(const double timeStep);

    /// @brief Updates the link during after solution found but before confirmSolutionAcceptable
    virtual void computeFlows(const double timeStep);


    // @brief  Returns whether the link confirms, rejects, or delays the solution.
    virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                     const int absoluteStep);


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @return   bool  Returns true if the link is non-linear
    ///
    /// @details  Tell gunns this in non-linear so confirmSolutionAcceptable is called.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual bool isNonLinear()
    {
        return true;
    }
    /// @brief Minor Step method for the non-linear link.
    virtual void minorStep(const double dt, const int minorStep);

    // setters and getters for controlling the switch object through the switchElect
    bool isSwitchTripped();
    bool isSwitchPosTrip();
    bool isSwitchNegTrip();
    bool isSwitchJustTripped();
    // bool isSwitchVoltTrip();

    bool isSwitchClosed();

    double getSwitchCurrent();
    double getSwitchInputVoltage();
    double getSwitchOutputVoltage();

    void setSwitchCommandedClosed(const bool switchCommandedClosed);
    void setSwitchTripReset();

 protected:
    double mCurrent;                 /**< (amp) current */
    double mVoltage;                 /**< (V)   voltage */
    double mTimeStep;                /**< (s) time interval for calling */

    /// @brief Virtual method for derived links to perform their restart functions.
    virtual void restartModel();

 private:
    /// @brief Copy constructor unavailable since declared private and not implemented.
    SwitchElect(const SwitchElect& that);

    /// @details  Assignment operator unavailable since declared private and not implemented.
    SwitchElect& operator =(const SwitchElect& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in] dt        (s)  Frame delta time
/// @param[in] minorStep (--) The minor step number that the network is on
///
/// @details  For each network minor step, this simply passes execution on to the base class step
///           method.  This allows the link admittance matrix to be updated on minor steps after it
///           has tripped open, during the same major frame.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void SwitchElect::minorStep(const double dt,
                                   const int    minorStep __attribute__((unused)))
{
    step(dt);
}

#endif
