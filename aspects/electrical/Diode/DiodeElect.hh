/*********************** TRICK HEADER *************************************************************
@copyright Copyright 2021 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (EPS Diode Elect class)

 REQUIREMENTS:
 (none)

 REFERENCE:
 (none)

 ASSUMPTIONS AND LIMITATIONS:
 (none)

 LIBRARY DEPENDENCY:
 ((DiodeElect.o))

 PROGRAMMERS:
 (
 (Minh Tran) (L-3) (July 2011) (Initial Prototype)
 (Nicholas Kaufmann (L-3) (Oct 2011) (Added Config and Input Classes)
 )
 **************************************************************************************************/
#ifndef _Diode_HH_
#define _Diode_HH_

#include "core/GunnsBasicLink.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Diode Configuration Data
///
/// @details  The  purpose of this class is to provide a data structure for the Diode
///           configuration data.
//////////////////////////////////////////////////////////////////////////////////////////////////
class DiodeElectConfigData: public GunnsBasicLinkConfigData {

public:
    double mDiodeReverseResistance; /**< (ohm)    trick_chkpnt_io(**) Reverse Diode equivalent resistance */
    double mDiodeForwardResistance; /**< (ohm)    trick_chkpnt_io(**) Forward Diode equivalent resistance */

    /// @brief Default Diode Configuration Data Constructor
    DiodeElectConfigData(const std::string& name = "",
                 GunnsNodeList* nodes = 0,
                 double diodeReverseResistance = 100000000.0,
                 double diodeForwardResistance = 0.01);

    /// @brief Default Diode Configuration Data Destructor
    virtual ~DiodeElectConfigData();

    /// @brief Copy constructs Diode Configuration Data.
    DiodeElectConfigData(const DiodeElectConfigData& that);

private:
    /// @brief  Operator = is not available since declared private and not implemented.
    DiodeElectConfigData& operator =(const DiodeElectConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Diode Elect Input Data
///
/// @details  This provides a data structure for the Diode Elect input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class DiodeElectInputData : public GunnsBasicLinkInputData
{
    public:

        /// @brief Default constructs this Diode Elect input data.
        DiodeElectInputData(const bool   malfBlockageFlag  = false,
                            const double malfBlockageValue = 0.0,
                            const bool   initReverseBiased = false);

        /// @brief Copy constructs this Diode Elect input data.
        DiodeElectInputData(const DiodeElectInputData& that);

        /// @brief Default destructs this Diode Elect input data.
        virtual ~DiodeElectInputData();

        bool mInitReverseBiased;              /**< (--)  Flag for whether the diode should be forward or reverse biased at sim startup */

    protected:


    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        DiodeElectInputData& operator =(const DiodeElectInputData&);
};


//////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Diode
///
/// @details  The  purpose of this class is to provide a data structure for the Diode.
///
//////////////////////////////////////////////////////////////////////////////////////////////////
class DiodeElect: public GunnsBasicLink {
    TS_MAKE_SIM_COMPATIBLE (DiodeElect);

public:
    /// @brief Default Constructor, deprecated due to obsolescence by GunnsElectRealDiode.
    DiodeElect() __attribute__ ((deprecated));

    /// @brief Default Destructor.
    virtual ~DiodeElect();

    /// @brief Initialize method as Gunns network links
    /// Port0: Anode (A); Port1: Cathode(K)
    virtual void initialize(const DiodeElectConfigData& configData,
                            const DiodeElectInputData& inputData,
                            std::vector<GunnsBasicLink*>& networkLinks,
                            const int port0,
                            const int port1);

    /// @brief Step method for updating the link
    void step(double timeStep);

    /// @brief Minor step method for non-linear
    void minorStep(double timeStep, const int minorStep);

    /// @brief Method for computing the flows across the Fet
    void computeFlows(double timeStep);

    /// @brief Returns if the link is non-linear
    bool isNonLinear();

    /// @brief Method for confirming if the voltages is acceptable
    enum GunnsBasicLink::SolutionResult confirmSolutionAcceptable(const int minorStep, const int absoluteStep);

protected:
    enum {
        NPORTS = 2                    /**< (--)  trick_chkpnt_io(**) Number of ports */
    };

    double mConductance;              /**< (--)                      Diode conductance */
    double mActiveConductance;        /**< (--)                      Active Conductance */
    double mCurrent;                  /**< (amp)                     Diode Current */
    bool   mDiodeReverseBias;         /**< (--)                      Is diode in reverse bias */
    bool   mDiodePreviousReverseBias; /**< (--)                      Was diode previously in reverse bias */
    double mDiodeReverseResistance;   /**< (ohm) trick_chkpnt_io(**) Reverse Diode equivalent resistance */
    double mDiodeForwardResistance;   /**< (ohm) trick_chkpnt_io(**) Forward Diode equivalent resistance */
    bool   mInitReversBiased;         /**< (--)                      Flag for whether diode should be forward or reverse biased at sim startup */

    /// @brief Virtual method for derived links to perform their restart functions.
    virtual void restartModel();

private:
    /// @brief Copy constructor is not available since declared private and not implemented.
    DiodeElect(const DiodeElect& that);

    /// @brief  Operator = is not available since declared private and not implemented.
    DiodeElect& operator =(const DiodeElect& that);
};

#endif
