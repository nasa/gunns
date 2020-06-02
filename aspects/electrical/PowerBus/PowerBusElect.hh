#ifndef PowerBusElect_EXISTS
#define PowerBusElect_EXISTS

/**
@file
@brief      Power Bus declarations

@defgroup    TSM_GUNNS_ELECTRICAL_POWERBUSELECT PowerBusElect Model
@ingroup    TSM_GUNNS_ELECTRICAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (The PowerBusElect class is the electrical aspect of a a single input with multiple outputs to supply Voltage to and receive loads from a
 configurable number of loads.)

 REQUIREMENTS:
 (Power Bus must have at least one User Defined Load)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 (User will set the number of Loads, number of Loads must be greater than 0)

 LIBRARY DEPENDENCY:
 (
 (PowerBusElect.o)
 )

 PROGRAMMERS:
 (
 (Carlo Bocatto) (L-3 Comm) (5/2013) (Initial Prototype)
 )
/////////////////////////////////////////////////////////////////////////////////
/// The Power Bus is a non-intelligent link that distributes current and voltage
/// to a user set amount of UserLoads. The number of loads must be greater than zero but
/// has no maximum amount. The power bus has no trip logic of its own and thus all trips
/// will be handled either upstream of the power bus or in the user defined loads
/// themselves.
/////////////////////////////////////////////////////////////////////////////////
*/

#include "core/GunnsBasicLink.hh"
#include "aspects/electrical/UserLoad/UserLoadBase.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

class GunnsBasicJumperPlug;

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the PowerBusElect
///           Config data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class PowerBusElectConfigData : public GunnsBasicLinkConfigData {
    public:

        int mNumLoads;                   /**< (--) trick_chkpnt_io(**) number of loads total*/
        int* mPorts;                     /**< (--) trick_chkpnt_io(**) port mapping for the input port*/
        GunnsBasicJumperPlug* mPlug0;    /**< (--) trick_chkpnt_io(**) Pointer to jumper plug for port 0 */
        bool mSelfSealing0;              /**< (--) trick_chkpnt_io(**) Plug 0 self-seals when un-plugged */

        ///@brief Default PowerBusElect Configuration Data Constructor
        PowerBusElectConfigData(const std::string& name      = "Unnamed PowerBus",
                                GunnsNodeList*   nodes       = 0,
                                const int     numLoads       = 0,
                                GunnsBasicJumperPlug* plug0  = 0,
                                const bool  selfSealing0     = true);

        ///@brief Default PowerBusElect Configuration Data Destructor
        virtual ~PowerBusElectConfigData();

        ///@brief Copy constructs Load Switch Input Data
        PowerBusElectConfigData(const PowerBusElectConfigData& that);

        /// @brief Frees dynamic memory.
        void cleanup();
    private:

        ///@brief Operator = is not available since declared private and not implemented
        PowerBusElectConfigData& operator = (const PowerBusElectConfigData&);

};
/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the PowerBusElect
///           input data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class PowerBusElectInputData:public GunnsBasicLinkInputData {
    public:
        double mMaxConductance;  /**< (1/ohm) maximum conductance through the PowerBus*/
        double mMinConductance;  /**< (1/ohm) minimum conductance through the PowerBus*/
        int mConnection0;        /**< (--) Index of the initial plug 0 socket */

        ///@brief Default PowerBusElect Input Data Constructor
        PowerBusElectInputData(const bool   malfBlockageFlag  = false,
                const double malfBlockageValue = 0.0,
                const double maxConductance = 1.3E5,
                const double minConductance = 1.0E-8,
                const int    connection0       = -1);

        ///@brief Default PowerBusElect Input Data Destructor
        virtual ~PowerBusElectInputData();

        ///@brief Copy constructs Load Switch Input Data
        PowerBusElectInputData(const PowerBusElectInputData& that);

    private:
        ///@brief Operator = is not available since declared private and not implemented
        PowerBusElectInputData& operator = (const PowerBusElectInputData&);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The  purpose of this class is to provide a data structure for the PowerBusElect
/////////////////////////////////////////////////////////////////////////////////////////////////
class PowerBusElect:public GunnsBasicLink {
    TS_MAKE_SIM_COMPATIBLE(PowerBusElect);

    public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        GunnsBasicJumperPlug* mPlug[1];         /**< (--) trick_chkpnt_io(**) Pointer to the jumper plug       */
        UserLoadBase          **mLoad;          /**< (--) trick_chkpnt_io(**) Pointer to userLoads connected to PowerBus */
        /// @}

        ///@brief Default PowerBusElect Constructor
        PowerBusElect();

        ///@brief Default PowerBusElect Destructor
        virtual ~PowerBusElect();

        ///@brief Initialize method as Gunns network links
        void initialize(      PowerBusElectConfigData& configData,
                const PowerBusElectInputData& inputData,
                std::vector<GunnsBasicLink*>& networkLinks,
                UserLoadBase ** loads,
                const int* ports);

        /// @brief Step method for updating the link
        virtual void step(const double dt);

        /// @brief Minor step method for non-linear
        virtual void minorStep(const double dt, const int minorStep);

        /// @brief Method for computing the flows across the Link
        virtual void computeFlows(const double dt);

        /// @brief validates the PowerBus' Config Data
        void validateData(const PowerBusElectConfigData &configData,
                const PowerBusElectInputData &inputData,
                UserLoadBase **loads);

        ///@brief Ensures Resistive load conductivity stays within set boundaries.
        void checkResLoadConductance();

        ///@brief Ensures Resistive load conductivity stays within set boundaries.
        void checkCPLoadConductance();

        /// @brief Returns TRUE if the link is Non-Linear
        void determineLinearity();

        /// @ brief returns True to GUNNS if link is Non-Linear
        virtual bool isNonLinear();

    protected:
        /// @brief  Allocate arrays based on the number of loads
        void allocateArrays();

        /// @brief  Deletes allocated memory objects
        void cleanup();

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief Updates the State of the Basic Jumper.
        virtual void updateState(const double = 0.1);

        double                mLoadChangeTolerance;               /**<    (--) threshold for how much the conductance of a load has to change before bothering to re-calculate the GUNNS network */
        int                   mNumLoads;                          /**< *o (--) trick_chkpnt_io(**) Number of load instances to create */
        double                mBusVoltage;                        /**<    (V)  Bus Voltage */
        double                mTotalCPowerLoadsPower;             /**<    (W)  Total Power of all Constant Power Loads */
        double                mFlux;                              /**<    (amp) Input current into the PowerBus */
        // double                mPower;                             /**< (W) Sum of all power loads */
        double                mResLoadsConductance;               /**<    (1/ohm) Total Conductance of resistive Loads */
        double                mCPowerLoadsConductance;            /**<    (1/ohm) Total Conductance of constant power loads */
        double                mActiveResLoadsConductance;         /**<    (1/ohm) Active conductance of Resistive Loads */
        double                mActiveCPowerLoadsConductance;      /**<    (1/ohm) Active conductance of Constant Power Loads */
        double                mMaxConductance;                    /**<    (1/ohm) Maximum allowed conductance */
        double                mMinConductance;                    /**<    (1/ohm) Minimum allowed conductance */
        double                mMaxResistance;                     /**<    (ohm) inverse of mMinConductance, used for sanity check on resistive user loads */
        bool                  mNonLinear;                         /**<    (--) Flag for whether this is a non-linear link */
        bool                  mSelfSealing0;                      /**<    (--) trick_chkpnt_io(**) Plug 0 self-seals when un-plugged */
        bool                  mSealed;                            /**<    (--) Plug 0 if sealed then has no conductivity */

    private:

        PowerBusElect(const PowerBusElect& that);
        PowerBusElect& operator =(const PowerBusElect& that);
 };

/// @}
#endif
