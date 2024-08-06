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
 ((FetSwitchElect.o))

 PROGRAMMERS:
 ((Minh Tran) (L-3) (Date) (Initial Prototype))
 **************************************************************************************************/
#ifndef _FetSwitchElect_HH_
#define _FetSwitchElect_HH_

#include "core/GunnsBasicLink.hh"
#include "aspects/electrical/FetSwitch/FetSwitch.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fet Switch Configuration Data
///
/// @details  The  purpose of this class is to provide a data structure for the Fet Switch
///           configuration data.
//////////////////////////////////////////////////////////////////////////////////////////////////
class FetSwitchElectConfigData : public GunnsBasicLinkConfigData
{
    public:

       FetSwitchConfigData mSwitchConfigData;  /**< (--) trick_chkpnt_io(**) TODO needs comment */


       FetSwitchElectConfigData(const std::string name   = "Unnamed Link",
                                GunnsNodeList*   nodes  = 0,
                                const FetSwitchConfigData& config = 0);

        virtual ~FetSwitchElectConfigData();

        FetSwitchElectConfigData(const FetSwitchElectConfigData& that);
    protected:

    private:
        FetSwitchElectConfigData& operator =(const FetSwitchElectConfigData&);
};


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fet Switch Input Data
///
/// @details  The  purpose of this class is to provide a data structure for the Fet Switch
///           input data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class FetSwitchElectInputData: public GunnsBasicLinkInputData
{

   public:
        int mSwitchMalf;

        /// @brief Default Fet Switch Input Data Constructor
        FetSwitchElectInputData(const bool   malfBlockageFlag  = false,
                                const double malfBlockageValue = 0.0,
                                 const int switchMalf = 0);

        /// @brief Default Fet Switch Input DataDestructor
        virtual ~FetSwitchElectInputData();

        /// @brief Copy constructs Fet Switch Input Data.
        FetSwitchElectInputData(const FetSwitchElectInputData& that);

    private:
        /// @brief  Operator = is not available since declared private and not implemented.
        FetSwitchElectInputData& operator =(const FetSwitchElectInputData&);
};



//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Fet Switch
///
/// @details  The  purpose of this class is to provide a data structure for the Fet Switch.
///
//////////////////////////////////////////////////////////////////////////////////////////////////
class FetSwitchElect: public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE (FetSwitchElect);
    public:
        FetSwitchElect() __attribute__ ((deprecated));
        virtual ~FetSwitchElect();
        void initialize(const FetSwitchElectConfigData& configData,
                        const FetSwitchElectInputData& inputData,
                        std::vector<GunnsBasicLink*>&   networkLinks,
                        const int  port0,
                        const int  port1);
        void initializeSwitch(const FetSwitchElectConfigData& configData,
                              const FetSwitchElectInputData& inputData);
        virtual void step(double timeStep);
        void updateSwitchState();
        virtual void minorStep(const double timeStep, const int minorStep);
        virtual void computeFlows(double timeStep);
        void updateSwitchFlow(double current);
        virtual bool isNonLinear();
        virtual GunnsBasicLink::SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                                         const int absoluteStep);

    protected:
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:

        enum
        {
            NPORTS = 2                          // --  number of ports
        };

        FetSwitch mSwitch;
        bool mFetPreviousState;                 // --  Fet Previous State
        bool mDiodePreviousReverseBias;         // --  Is diode previos in reverse bias


};

#endif
