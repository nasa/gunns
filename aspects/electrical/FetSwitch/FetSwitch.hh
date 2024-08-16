/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (Purpose of the Class)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 (none)

 LIBRARY DEPENDENCY:
 ((FetSwitch.o))

 PROGRAMMERS:
 ((Minh Tran) (L-3) (Date) (Initial Prototype))
 **************************************************************************************************/
#ifndef _FetSwitch_HH_
#define _FetSwitch_HH_

#include "software/SimCompatibility/TsSimCompatibility.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fet Switch Configuration Data
///
/// @details  The  purpose of this class is to provide a data structure for the Fet Switch
///           configuration data.
//////////////////////////////////////////////////////////////////////////////////////////////////
class FetSwitchConfigData
{
    public:
        double mFetOnResistance;                     // (ohm)    trick_chkpnt_io(**) On Resistance
        double mFetOffResistance;                    // (ohm)    trick_chkpnt_io(**) Fet off equivalent resistance
        double mDiodeReverseResistance;              // (ohm)    trick_chkpnt_io(**) Reverse Diode equivalent resistance
        double mDiodeForwardResistance;              // (ohm)    trick_chkpnt_io(**) Forward Diode equivalent resistance
        bool mOverCurrentProtection;                 // (--)     trick_chkpnt_io(**) Fet over current protection
        double mOverCurrentLimit;                    // (amps)   trick_chkpnt_io(**) Over Curent Limit

        /// @brief Default Fet Switch Configuration Data Constructor
        FetSwitchConfigData(     const double fetOnResistance = .001,
                                  const double fetOffResistance = 100000000,
                                  const double diodeReverseResistance = 100000000,
                                  const double diodeForwardResistance =.01,
                                  const bool overCurrentProtection = false,
                                  const double overCurrentLimit = 4.4);


        /// @brief Fet Switch Configuration Data Initialize
        void init (const double fetOnResistance,
                   const double fetOffResistance ,
                   const double diodeReverseResistance,
                   const double diodeForwardResistance,
                   const bool overCurrentProtection,
                   const double overCurrentLimit
                   );

        virtual ~FetSwitchConfigData();

        FetSwitchConfigData(const FetSwitchConfigData& that);

    protected:

    private:
        //FetSwitchConfigData& operator =(const FetSwitchConfigData&);
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fet Switch Input Data
///
/// @details  The  purpose of this class is to provide a data structure for the Fet Switch
///           input data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class FetSwitchInputData
{
    public:
        enum EpsSwitchMalf{
            NONE = 0,
            FAIL_OPENED = 1,
            FAIL_CLOSED = 2
        };

    public:
        bool mSwitchMalfFlag;
        int mSwitchMalf;
        bool mSwitchCmd;

        /// @brief Default Fet Switch Input Data Constructor
        FetSwitchInputData(const bool switchMalfFlag = false,
                           const int switchMalf = 0,
                           const bool switchCmd = true);

        /// @brief Fet Switch Input Data Initialize
         void init (const bool switchMalfFlag,
                     const int switchMalf,
                     const bool switchCmd );

        /// @brief Default Fet Switch Input DataDestructor
        virtual ~FetSwitchInputData();

        /// @brief Copy constructs Fet Switch Input Data.
        FetSwitchInputData(const FetSwitchInputData& that);

    private:
        /// @brief  Operator = is not available since declared private and not implemented.
        //FetSwitchInputData& operator =(const FetSwitchInputData&);
};


//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fet Switch
///
/// @details  The  purpose of this class is to provide a data structure for the Fet Switch.
///
//////////////////////////////////////////////////////////////////////////////////////////////////
class FetSwitch
{
    TS_MAKE_SIM_COMPATIBLE (FetSwitch);
    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool mSwitchMalfFlag;
        int mSwitchMalf;                             // --     Switch Malfunction
        /// @}

        FetSwitch();
        virtual ~FetSwitch();
        void initialize(const FetSwitchConfigData& configData,
                        const FetSwitchInputData& inputData);
        void updateSwitchState();
        void updateSwitchFlow(double current);
        double getCurrent();
        bool getFetState();
        bool getDiodeReverseBias();
        double getConductance();
        double getPowerDissipation();
        void setDiodeReverseBias(bool diodeReverseBias);
        /// @brief Sets and resets the switch malfunction.
        void setMalfSwitch(const bool flag = false, const int value = 0);

    private:

        double mCurrent;                        // amps   Switch current
        bool mFetCmd;                           // --     Switch Command 1 closed; 0 open
        bool mFetState;                         // --     Switch state: 1 closed; 0 open
        bool mDiodeReverseBias;                 // --  Is diode in reverse bias
        bool mTrip;                             // --     Switch trip status
        bool mTripReset;                        // --     Switch trip reset


        double mFetOnResistance;                     // (ohm)    trick_chkpnt_io(**) On Resistance
        double mFetOffResistance;                    // (ohm)    trick_chkpnt_io(**) Fet off equivalent resistance
        double mDiodeReverseResistance;              // (ohm)    trick_chkpnt_io(**) Reverse Diode equivalent resistance
        double mDiodeForwardResistance;              // (ohm)    trick_chkpnt_io(**) Forward Diode equivalent resistance
        bool mOverCurrentProtection;                 // (--)     trick_chkpnt_io(**) Fet over current protection
        double mOverCurrentLimit;                    // (amps)   trick_chkpnt_io(**) Over Curent Limit

        double mPowerDissipation;                    // --     power dissipation

        static const double mConductanceLimit;
};

#endif
