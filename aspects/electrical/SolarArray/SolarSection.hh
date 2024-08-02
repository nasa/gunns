#ifndef SolarSection_EXISTS
#define SolarSection_EXISTS

/**
@file
@brief      Solar Section declarations

@defgroup   TSM_GUNNS_ELECTRICAL_SolarSection SolarSection Model
@ingroup    TSM_GUNNS_ELECTRICAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (The SolarSection class serves as an interface to the thermal and environment subystems. A solar section is composed of any number
 of SolarString instances in parallel. Each solar section has will receive mSunAngleFromHorizontal, mSunIntensity, and mTemperature from outside
 models. Then we assume that every string in this section sees the same angle, intensity, and temperature. The section will provide
 its Geq and Ieq equivalent stamp to an array class that build up the Geq and Ieq by summing over all of the sections.

 ASSUMPTIONS AND LIMITATIONS:
 (
 - mSunAngleFromHorizontal, mSunIntensity, mIsShadowed, and mTemperature come from outside subsystems via an ICD.
 - The angle should be measured from the 0 degree horizontal to the line of site vector. The vector shoud be the z+ line. (The one pointing at target)
 - All strings in this section see the same values for the three variables above.
 - Sun intensity ranges from 0 to 1.0
 )


 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/SolarArray/SolarSection.o)
 )

 PROGRAMMERS:
 (
 (Nicholas Kaufmann) (L-3 Comm) (8/2013) (Initial Prototype)
 )
/////////////////////////////////////////////////////////////////////////////////
/// The Solar Section is ...
/////////////////////////////////////////////////////////////////////////////////
*/

#include <string>
#include "aspects/electrical/SolarArray/SolarString.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the config data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarSectionConfigData{

public:

    ///@brief Default Configuration Data Constructor
    SolarSectionConfigData(const int numStrings                           = 0,
                           const SolarStringConfigData& stringConfigData  = 0);

    ///@brief Default Configuration Data destructor
    virtual ~SolarSectionConfigData();

    int mNumStrings;                                   /**< (--) trick_chkpnt_io(**) the number of strings per section*/
    SolarStringConfigData mSolarStringConfigData;      /**< (--) trick_chkpnt_io(**) configuration data for this section's solar strings.*/

    //@brief Copy constructor
    SolarSectionConfigData(const SolarSectionConfigData& that);

private:
        ///@brief Operator = is not available since declared private and not implemented
    SolarSectionConfigData& operator = (const SolarSectionConfigData&);



};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the input data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarSectionInputData{
public:

    ///@brief Default Configuration Data Constructor
    SolarSectionInputData(const bool isShadowed                      = false,
                          const SolarStringInputData& strigInputData = 0);

    ///@brief Default Configuration Data destructor
    virtual ~SolarSectionInputData();

    bool mIsShadowed;                                 /**< (--)  trick_chkpnt_io(**)  whether this section has a line of site to the sun or not */
    SolarStringInputData mSolarStringInputData;       /**< (--)  trick_chkpnt_io(**)  the input data for this section's strings    */


    //@brief Copy constructor
    SolarSectionInputData(const SolarSectionInputData& that);

private:

        ///@brief Operator = is not available since declared private and not implemented
    SolarSectionInputData& operator = (const SolarSectionInputData&);


};



/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The  purpose of this class is to provide a data structure for the SolarSection
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarSection
{
            TS_MAKE_SIM_COMPATIBLE(SolarSection);

public:
        /// @name    Embedded objects.
        /// @{
        /// @details Embedded objects are public to allow access from the Trick events processor.
        SolarString* mStrings;        /**< (--) trick_chkpnt_io(**) pointer to array of strings belonging to this section */
        /// @}

        ///@brief Default Constructor
        SolarSection();
        ///@brief Default destructor
        virtual ~SolarSection();
        ///@brief initializes the section with configuration and input data
        void initialize(const SolarSectionConfigData& config, const SolarSectionInputData& input);
        ///@brief updates the section based on the most recent current and voltage
        void update(const bool isMinor, const double sectionCurrent, const double sectionVoltage);

        /// @brief setter to pass in fake orbit parameters ( sun angle and intensity) from solar array model
        void setFakeOrbitParms(const bool flag, const double Angle, const double intensity, const bool backlit);

        /// @brief function to reset the fake orbit flag if needed
        void setFakeOrbitFlag(const bool value);

        /// @brief return the state of the fake orbit flag
        bool getFakeOrbitFlag() {
            return mFakeOrbitFlag;
        }

        /// @details This is the amount of solar power hitting the surface of the section. Units are Watts. This comes from
        ///          the JEOD Solar Radiation Pressure model.
        double mSolarPowerOnSection;        /**<    (W) Solar power hitting the surface of the section */
        double mSunAngleFromHorizontal;  /**< (rad) the angle (radians) sun rays makes from 0 degree horizontal to line of site vector Z+ component. Read from ENV. Value passed through solar section. */
        double mSunIntensity;            /**< (--) the intensity of the sun as felt by this cell. Ranges 0 to 1 */

protected:
        ///////////////////////////////////////
        /// Configuration variables        ///
        //////////////////////////////////////
        int mNumStrings;              /**< *o (--) trick_chkpnt_io(**) the number of strings                                 */

        ///////////////////////////////////////////////////////
        /// INPUTS - variables to read from other subystems ///
        ///////////////////////////////////////////////////////
        double mTemperature;             /**< (K) the cell temperature. Read from THERMAL subsystem. Value passed through solar section. */
        double mLocalSunAngleScalar;     /**< (--) override at the SAW section level for the epsOverrides scalar which tunes SAW current generated at off angles */
        bool   mLocalSunAngleScalarFlag;  /**< (--) flag for local sun angle scalar override */
        bool   mZPlusLOSIsClear;         /**< (--)  Whether the LOS in Z+ direction can see its target. (Conducting Side )*/
        bool   mZMinusLOSIsClear;        /**< (--)  Whether the LOS vector in Z- direction can see its target (Back side) */

        bool   mFakeOrbitFlag;           /**< (--)  flag for whether to use sun angle and intensity from environment or override from solar array model */
        double mFakeSunIntensity;        /**< (--)  fake sun intensity to use when being overridden */
        double mFakeSunAngle;            /**< (rad)   fake sun angle to use when being overridden */
        bool   mFakeIsBackLit;           /**< (--)  fake flag to tell solar panel it is back-lit when overriding environment values */

        ///////////////////////
        /// State variables ///
        ///////////////////////
        bool   mIsShadowed;          /**< (--)  true if both z+ and z- LOS are blocked. This means there are no lines of sight to sun  */
        bool   mIsBackLit;           /**< (--)  Whether this section is being back lit, aka the back of array faces sun. This causes reduces Iphoto */
        int    mNumFailedStrings;    /**< (--)  The number of failed strings                                   */
        int    mNumFailedCells;      /**< (--)  The sum of all the failed cells for each string                */
        double mGeq;                 /**< (1/ohm)  The conductance contribution of this section                */
        double mIeq;                 /**< (amp)  The source vector contribution of this section                */
        double mVoltage;             /**< (V)   The voltage of this section                                    */
        double mCurrent;             /**< (amp) The total current of this section                              */

        /// @brief  Allocates the arrays based on the number of sections
        void allocateArrays();
        /// @brief  Deletes allocated memory objects
        void cleanup();

private:

    ///@brief validates the input and configuration data that is passed in through initialize.
    void validate();

    //@brief Copy constructor is not available since declared private and not implemented
    SolarSection(const SolarSection& that);

    //@brief Operator = is not available since declared private and not implemented
    SolarSection& operator =(const SolarSection& that);

public:

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details gets the equivalent conductance of this section
    ///@returns (1/ohm) mGeq, the equivalent conductance of the section
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getConductanceStamp() const{
        return mGeq;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details gets the equivalent source vector of this section
    ///@returns (amp) mIeq, the equivalent source vector of the section
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getSourceVectorStamp() const{
        return mIeq;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details getter for total number of failed strings
    ///@returns (--) mNumFailedStrings, the total number of failed strings
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline int getNumFailedStrings() const{
        return mNumFailedStrings;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details getter for total number of strings
    ///@returns (--) mNumStrings, the total number of strings
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline int getNumStrings() const{
        return mNumStrings;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns the number of cells for string configuration
    ///@returns (--) the total number of cells possible on strings in this section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline int getNumCells() const{
        return mStrings[0].getNumCells();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns the total number of failed cells in this section
    ///@returns (--) the total number of failed cells in this section.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline int getNumFailedCells() const{
        return mNumFailedCells;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns the open circuit voltage of a cell in this section
    ///@returns (V) the open circuit voltage of a cell in this section
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    double getRefCellVoc();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns the the source current of string 0 on this section
    ///@returns (amp) the source current of string 0 on this section
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    double getStringIsc();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns the the maximum source current of string 0 on this section
    ///@returns (amp) the source current of string 0 on this section
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    double getStringIscRef();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns whether this section is shadowed or not
    ///@returns (--) bool   whether this section is shadowed or not
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline bool isShadowed() const {
         return mIsShadowed;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns the current for a given string
    ///@returns (amps) double   the current output for the given string
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getStringCurrent(const int string) const {
        return mStrings[string].getCurrent();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns the temperature of this section
    ///@returns (k) double   the temperature of this section
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getTemperature() const {
        return mTemperature;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details for unit testing, function to allow the changing of the amount of sunlight
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setSunIntensity(const double intensity);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///@details returns the amount of sunlight falling on this section
    ///@returns (--) double   the sun intensity on this section, from 0.0 to 1.0
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    double getSunIntensity() const {
        return mSunIntensity;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details Setter method for setting the section sun angle.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setSunAngle(const double sunAngle) {
        mSunAngleFromHorizontal = sunAngle;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details Sets the sun angle scalar value.  Allows the user to tune the current the array generates
    ///          at off angles.  It implements a power function on the sin(angle from environment) component
    ///          of the math that calculates the current the cell generates at any particular angle.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setSunAngleScalar (const double scalar) {
        double value = scalar;

        if(mLocalSunAngleScalarFlag) {
            value = mLocalSunAngleScalar;
        }

        for (int i=0; i < mNumStrings; i++) {
            mStrings[i].setSunAngleScalar(value);
        }
    }

};
/// @}
#endif
