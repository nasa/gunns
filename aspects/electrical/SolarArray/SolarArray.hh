#ifndef SolarArray_EXISTS
#define SolarArray_EXISTS

/**
@file
@brief      Solar Array declarations

@defgroup   TSM_GUNNS_ELECTRICAL_SolarArray SolarArray Model
@ingroup    TSM_GUNNS_ELECTRICAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (The SolarArray class is a GUNNS link which models the behavior of a solar array with X number of sections with Y number of
  parallel string per section where each string has Z number of cells in series. The cell performance changes as the temperature,
  sun intensity, sun angle, network load, and degradation affects change with time. )

 REFERENCE:

    1.) Najim, Farid N. Circuit Simulation. Hoboken, NJ: Wiley, 2010. Print
            - Information about network companion models, and general network solving tips and solutions.
            - Information about logarithmic damping of nonlinear curves with step feedback.
    2.) Patel, Mukund R. Spacecraft Power Systems. Boca Raton: CRC, 2005. Print.
            - Example network of a solar cell
            - Current and Voltage curve characteristics
            - General behavior and background for solar arrays. See Solar Array chapter.
    3.) See the various scratch-work and design documents I have created in Alfresco under the Ts-21 ISS design work folder.

   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // This is a loose schematic giving useful insight into how to build a single companion model into an array.                           //
   //  ==========================================                                                                                         //
   //                  SECTION 1                                                                                                          //
   //                                                                                                                                     //
   //      GeqCell          GeqCell                                          GeqString_1                                                  //
   //    |-/\/\/\-|      |-/\/\/\-|                                          |-/\/\/\-|                                                   //
   //----| IeqCell|------|        |--- <Diode> + ... + N   reduces to     ---|        |---- \                     GeqSect_1               //
   //    |---(=>)-|      |--(=>)--|                                          |--(=>)--|      \                   |-/\/\/\-|               //
   //                                                                        IeqString_1      \__  reduces to ---|        |----<NxDiode>  //
   //     GeqCell           GeqCell                                          GeqString_2      /                  |--(=>)--|               //
   //    |-/\/\/\-|      |-/\/\/\-|                                          |-/\/\/\-|      /                     IeqSect_1              //
   //----| IeqCell|------|        |--- <Diode>   + ... + N  reduces to    ---|        |---- /                                             //
   //    |---(=>)-|      |--(=>)--|                                          |--(=>)--|                                                   //
   //                                                                        IeqString_2                                                  //
   //                      +                                                                                                              //
   //  ==========================================                                                                                         //
   //                 SECTION 2                                                                                                           //
   //                    ...                                                                                                              //
   //  ==========================================                                                                                         //
   //                      +                                                                                                              //
   //  ==========================================                                                                                         //
   //                 SECTION N                                                                                                           //
   //                    ...                                                                                                              //
   //  ==========================================                                                                                         //
   //              can be reduced to SOLAR ARRAY                                                                                          //
   //                                                                                                                                     //
   //              GeqArray                                                                                                               //
   //              |/\/\/\-|                                                                                                              //
   //         -----|       |----                                                                                                          //
   //              |--(=>)-|                                                                                                              //
   //              IeqArray                                                                                                               //
   //                                                                                                                                     //
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

     //Here are some useful derived equalities. Use KCL on the equivalent circuit diagram to calculate V and I when needed.
     //=====================================================================================================================
     GeqArray   = GeqSection * NumSections per array
     GeqSection = GeqString * NumStrings per section
     GeqString  = GeqCell / NumCells per string
     GeqCell    = (derived from circuit network shown in reference #2)

     IeqCell    =  IeqArray/numSections per array /numStrings per section
     IeqString  =  IeqArray/numSections per array /numStrings per section
     IeqSection =  IeqArray/mNumSections per array

 ASSUMPTIONS AND LIMITATIONS:
 (- Assume every cell on a string is the same
  - An array is divided into sections of strings in parallel, each section holds an environment interface (thermal, sun, shadow, etc)
    all strings in a section see the same environment affects.
  - Each section is divided into a string of cells in series with bipass diodes spaces throughout and a blocking diode at the end.
  - All cells on a string are identical. There is one model which serves as a reference to how all of the cells should behave.
  - Each section requires an interface to other subsystems to get updated thermal, and env effects.
  - This model is a two port link Gunns link. Port zero should go to ground, and port 1 to the network load.
  )

 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/SolarArray/SolarArray.o)
 )

 PROGRAMMERS:
 (
 (Nicholas Kaufmann) (L-3 Comm) (9/2013) (Initial)
 (Mike Moore) (L-3 Comm) (02/2015) (Updated to support arrays outside of LEO)
 )
 */

#include <string>
#include <vector>
#include "SolarSection.hh"
#include "core/GunnsBasicLink.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the config data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarArrayConfigData : public GunnsBasicLinkConfigData {

public:

    ///@brief Default Configuration Data Constructor
    SolarArrayConfigData(const std::string name                = "",
            GunnsNodeList *nodes                  = 0,
            const int numSections                 = 0,
            const int numStrings                  = 0,
            const int numCells                    = 0,
            const double blockingDiodeVoltageDrop = 0.0,
            const double bipassDiodeVoltageDrop   = 0.0,
            const int bipassDiodeInterval         = 0.0,
            const double vocRef                   = 0.0,
            const double vmpRef                   = 0.0,
            const double vocTempCoeff             = 0.0,
            const double iscRef                   = 0.0,
            const double impRef                   = 0.0,
            const double iscTempCoeff             = 0.0,
            const double iSat                     = 0.0,
            const double temperatureRef           = 0.0,
            const double cellDegradation          = 0.0,
            const double seriesResistance         = 0.0,
            const double shuntResistance          = 0.0,
            const double vCritc                   = 0.0,
            const double backSideIscReduction     = 0.0,
            const double cellEfficiency           = 0.0,
            const double cellArea                 = 0.0
    );



    ///@brief Default Configuration Data Destructor
    virtual ~SolarArrayConfigData();

    int mNumSections;                /**< (--) trick_chkpnt_io(**) number of sections that this solar array is divided into                     */
    double mSectionArea;             /**< (--) trick_chkpnt_io(**) area of the sections within the array. (all assumed to have equal area)      */
    int mNumStrings;                 /**< (--) trick_chkpnt_io(**) the number of strings per section                                            */
    int mNumCells;                   /**< (--) trick_chkpnt_io(**) number of cells that this solar string has                                   */
    double mBlockingDiodeVoltageDrop;/**< (V) trick_chkpnt_io(**) the voltage drop across the diode at end of string                            */
    double mBipassDiodeVoltageDrop;  /**< (V) trick_chkpnt_io(**) the voltage drop across each bipass diode.                                    */
    int mBipassDiodeInterval;        /**< (--) trick_chkpnt_io(**) the number of cells per ever one bipass diode                                */
    double mVocRef;                  /**< (V) trick_chkpnt_io(**) the open circuit voltage reference value.                                     */
    double mVmpRef;                  /**< (V) trick_chkpnt_io(**) the voltage max power reference value.                                        */
    double mVocTempCoefficient;      /**< (--) trick_chkpnt_io(**) the open circuit voltage coefficient. see page (XYZ) of (XYZ) document       */
    double mIscRef;                  /**< (amp) trick_chkpnt_io(**) the source current reference value.                                         */
    double mImpRef;                  /**< (amp) trick_chkpnt_io(**) the max power current reference value.                                      */
    double mIscTempCoefficient;      /**< (--) trick_chkpnt_io(**) the source current temperature coefficient. see page (XYZ) of (XYZ) document */
    double mIsat;                    /**< (amp) trick_chkpnt_io(**) the saturation current of the cell. aka "Dark Current"                      */
    double mTemperatureRef;          /**< (K)  trick_chkpnt_io(**) the temperature at which the reference values were recorded.                 */
    double mCellDegradation;         /**< (--) trick_chkpnt_io(**) the percent amount to degrade cell by. This accounts for age, damage, etc    */
    double mRs;                      /**< (ohm) trick_chkpnt_io(**) the cell series resistance                                                  */
    double mRsh;                     /**< (ohm) trick_chkpnt_io(**) the cell shunt resistance                                                   */
    double mVCrit;                   /**< (--) trick_chkpnt_io(**) a value multiplier for voc, after Vcrit*mVoc, model  apply damping to dV     */
    double mBackSideIscReduction;    /**< (--) trick_chkpnt_io(**) The percentage Isc drops by when the cell is back lit onl. Ranges from 0 to 1*/
    double mCellEfficiency;          /**< (--) trick_chkpnt_io(**) The ratio of maximum electrical output power (Voc*Isc) to input solar
                                                                 power (SolarFlux*CellArea). */
    double mCellArea;                /**< (m^2) trick_chkpnt_io(**) The reference cell's area. */

    SolarSectionConfigData mSectionConfigData; /**< (--) trick_chkpnt_io(**) the section configuration data */

    ///@brief copy constructor
    SolarArrayConfigData(const SolarArrayConfigData& that);

private:
    ///@brief Operator
    SolarArrayConfigData& operator = (const SolarArrayConfigData&);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the input data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarArrayInputData : public GunnsBasicLinkInputData{

public:
    ///@brief Default Input Data Constructor
    SolarArrayInputData(const bool   malfBlockageFlag             = false,
            const double malfBlockageValue            = 0.0,
            const bool isShadowed                     = false,
            const bool isFailed                       = false,
            const int numFailedCells                  = 0,
            const bool cellPowerMalfIsEnabled         = false,
            const bool cellDegradationMalfIsEnabled   = false,
            const double cellDegradationOverrideValue = 1.0,
            const bool isInLeo                        = true);

    ///@brief Default Input Data Destructor
    virtual ~SolarArrayInputData();
    bool   mIsShadowed;                          /**< (--)  trick_chkpnt_io(**)  whether this section has a line of site to the sun or not */
    bool   mIsFailed;                            /**< (--)  trick_chkpnt_io(**)  Whether this string is failed or not.    */
    int    mNumFailedCells;                      /**< (--)  trick_chkpnt_io(**)  the number of failed cells in the string */
    bool   mCellPowerMalfunctionIsEnabled;       /**< (--)  trick_chkpnt_io(**)  bool represents initial state of cell power malfunction.                                */
    bool   mCellDegradationMalfunctionIsEnabled; /**< (--)  trick_chkpnt_io(**)  bool represents the inital state of the cell degradation malfunction                    */
    double mCellDegradationOverrideValue;        /**< (--)  trick_chkpnt_io(**)  a double value which overrides default cell degradation when assoicated boolean is true */
    bool   mIsInLeo;                             /**< (--)  trick_chkpnt_io(**)  bool represents whether or not this array is in low earth orbit. Default is true.       */


    SolarSectionInputData mSectionInputData; /**<(--) the section input data*/

    ///@brief copy constructor
    SolarArrayInputData(const SolarArrayInputData& that);

private:
    ///@brief Operator = is not available since declared private and not implemented
    SolarArrayInputData& operator = (const SolarArrayInputData&);



};


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The  purpose of this class is to provide a data structure for the SolarArray
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarArray : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(SolarArray);

public:
    /// @name    Embedded objects.
    /// @{
    /// @details Embedded objects are public to allow access from the Trick events processor.
    SolarSection* mSections;         /**< (--) trick_chkpnt_io(**) pointer to array of sections             */
    /// @}

    ///@brief Default Constructor, this link is deprecated due to obsolescence by GunnsElectPvArray.
    SolarArray() __attribute__ ((deprecated));

    ///@brief Default Destructor
    virtual ~SolarArray();

    ///@brief Initialize method as Gunns network link
    void initialize(const SolarArrayConfigData& config,
            const SolarArrayInputData& input,
            std::vector<GunnsBasicLink*> &networkLinks,
            const int port0,
            const int port1);

    /// @brief Step method for updating the link
    void step(const double dt);

    /// @brief Minor step method for non-linear
    void minorStep(const double dt, const int minorStep);

    /// @brief Method for computing the flows across the Link
    void computeFlows(const double dt);

    /// @brief  Returns if the link is non-linear
    bool isNonLinear();

    /// @brief  Returns true if it needs an admittance update
    bool needAdmittanceUpdate();

    /// @brief  Resets the last minor step potential vector
    bool resetLastMinorStep(int convergedStep,int absoluteStep);

    double getAdmittanceValue(const int value) const {
        return mAdmittanceMatrix[value];
    }

    /// @details  Returns TRUE if the network needs re-solving for a trip condition
    GunnsBasicLink::SolutionResult confirmSolutionAcceptable(const int convergedStep, const int absoluteStep);

    double mSectionArea;             /**< (--) trick_chkpnt_io(**) area of the sections within the array. (all assumed to have equal area)      */
    bool   mIsInLeo;                 /**<    (--)  trick_chkpnt_io(**)  bool represents whether or not this array is in low earth orbit. Default is true.       */
    double mMinimumSolarPower;       /**<    (W) Minimum power level for the solar array. If the input power from ENV is below this value, then shunt all current.  */
    double mSunAngleScalar;          /**< (--) exponent on the sine function of the fall off of the solar array power with respect to the sun angle */
    double mLocalSunAngleScalar;     /**< (--) override at the SAW section level for the epsOverrides scalar which tunes SAW current generated at off angles */
    bool   mLocalSunAngleScalarFlag;  /**< (--) flag for local sun angle scalar override */
    double mReferenceInsolation;     /**<    (W/m2) Reference value for percent insolation */

protected:
    static const int NUM_PORTS = 2;  /**<    (--)  trick_chkpnt_io(**)  the number of ports for this link        */
    int    mNumSections;             /**< *o (--)  trick_chkpnt_io(**) number of sections for this array        */
    double mArrayCurrent;            /**<    (amp) the total array output current          */
    double mArrayVoltage;            /**<    (V) the total array output voltage            */
    bool   mIsMinorStep;             /**<    (--) whether or not it is a gunns minor step  */
    double mIeq;                     /**<    (amp) the source vector of the array          */
    double mGeq;                     /**<    (1/ohm) the conductance of the array          */
    double mMaxPossibleVoltage;      /**<    (V) the max possible voltage of the array     */
    double mMaxPossibleCurrent;      /**<    (amp) the max possible current of the array   */
    bool   mFakeOrbitFlag;           /**<    (--)  flag to override sun angle and intensity coming from environment */
    double mFakeOrbitTime;           /**<    (min) the current time of the fake orbit, set to 0.0 to be at start of insolation, 60.0 to be at start eclipse */
    double mFakeOrbitTimeSet;        /**<    (min) the orbit time from the overrides object */
    bool   mFakeOrbitTimeSetFlag;    /**<    (--)  flag to control setting the orbit time */
    double mFakeSunIntensity;        /**<    (--)  sun intensity to use in override */
    double mFakeSunAngle;            /**<    (rad) sun angle to use in override */
    double mFakeSunAngleRate;        /**<    (rad/s) rate to change the fake sun angle */
    double mPercentInsolation;       /**<    (--)  guesstimate of the amount of sun falling on the array */


    /// @brief Virtual method for derived links to perform their restart functions.
    virtual void restartModel();

private:
    /// @brief  Allocates the arrays based on the number of sections
    void allocateArrays();

    ///@brief Initializes the allocated sections with config and input data
    void initializeSections(const SolarSectionConfigData& cd, const SolarSectionInputData& id);

    /// @brief  validates the initial configuration
    void validate(const SolarArrayConfigData& config, const SolarArrayInputData& input);

    /// @brief  Deletes allocated memory objects
    void cleanup();

    /// @brief Populates the admittance matrix with the latest Geq
    void setupAdmittanceMatrix();

    /// @brief Populates the source vector with the latest Ieq
    void setupSourceVector();

    /// @brief a function to override the sun intensity and angle coming from Environment
    void fakeOrbit(const double dt);

    /// @brief Function to dynamically calculate the cell reference current so that the
    ///        cells produce current that is actually a function of the solar flux
    ///        magnitude, cell area, and cell performance.
    void updateNonLeoArray();

    //@brief Copy constructor is not available since declared private and not implemented
    SolarArray(const SolarArray& that);

    //@brief Operator = is not available since declared private and not implemented
    SolarArray& operator =(const SolarArray& that);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the total array voltage
    /// @return (V) double value representing total array voltage
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getArrayVoltage() const {
        return mArrayVoltage;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the total array current
    /// @return (amp)- double value representing total array current
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getArrayCurrent() const {
        return mArrayCurrent;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the total number of sections in the array
    /// @return (--)- int   count of how many sections are in this array
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getNumSections() const {
        return mNumSections;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns whether a given section is shadowed
    /// @return (--)- bool   flag for whether a given section is shadowed
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline bool getSectionIsShadowed(const int section) const {
        return mSections[section].isShadowed();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the number of strings in a given section
    /// @return (--)- int   the number of strings in the given section
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline int getNumStrings(const int section) const {
        return mSections[section].getNumStrings();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the current for a given string in a given section
    /// @return (amps)- double   the output current for the given string in the given section
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getStringCurrent(const int section, const int string) const {
        return mSections[section].getStringCurrent(string);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the  max possible current of the array at the present state
    /// @return (amps)- double   the max possible current of the array based on string and cell states
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getMaxPossibleCurrent() const {
        return mMaxPossibleCurrent;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the max possible voltage for the array at the present state
    /// @return (V)- double   the max possible voltage of the array based on string and cell states
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getMaxPossibleVoltage() const {
        return mMaxPossibleVoltage;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  set the state of the fake orbit flag
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setFakeOrbitFlag(const bool flag) {
        mFakeOrbitFlag = flag;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  set the current point of time in the fake orbit.
    /// @note     A fake orbit is 60 minutes of insolation, followed by 30 minutes of eclipse, then it
    ///           starts over.  Setting the fake orbit time to 59.0 will give you one minute of sunlight,
    ///           then thirty minutes of night, then starts over at 60 minutes of sunlight
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setFakeOrbitTime(const double time) {
        mFakeOrbitTime = time;
    }
};
/// @}
#endif
