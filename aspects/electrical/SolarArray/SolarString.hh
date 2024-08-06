#ifndef SolarString_EXISTS
#define SolarString_EXISTS

/**
@file
@brief      Solar String declarations

@defgroup   TSM_GUNNS_ELECTRICAL_SolarString SolarString Model
@ingroup    TSM_GUNNS_ELECTRICAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (The SolarString class models a string of photo cells in series with bipass diodes every n cells and a single blocking
  diode at the end of the string of cells.)


 ASSUMPTIONS AND LIMITATIONS:
 (- The string has a single reference cell. All of the cells on the string will be identical to this reference cell.
  - Any number of cells up to the total number of cells may be failed. If all cells are failed, the string is failed.
  - This class will return an equivalent Ieq and Geq stamp for the string to be built up inside a Gunns link.
  )

 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/SolarArray/SolarString.o)
 )

 PROGRAMMERS:
 (
 (Nicholas Kaufmann) (L-3 Comm) (8/2013) (Initial Prototype)
 )

*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "aspects/electrical/SolarArray/PVCellCompanionModel.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the SolarString
///           Config data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarStringConfigData {

public:
    int mNumCells;                            /**< (--) trick_chkpnt_io(**) number of cells that this solar string has*/
    double mBlockingDiodeVoltageDrop;         /**< (V) trick_chkpnt_io(**) the voltage drop across the diode at end of string*/
    double mBipassDiodeVoltageDrop;           /**< (V) trick_chkpnt_io(**) the voltage drop across each bipass diode.             */
    int mBipassDiodeInterval;                 /**< (--) trick_chkpnt_io(**) the number of cells per ever one bipass diode         */
    PVCellCompanionModelConfigData mRefCellConfigData; /**< (--) trick_chkpnt_io(**) configuration data for this section's reference cell.*/

    ///@brief Default SolarString Configuration Data Constructor
    SolarStringConfigData(const int numCells                             = 0,
                          const double blockingDiodeVoltageDrop          = 0.0,
                          const double bipassDiodeVoltageDrop            = 0.0,
                          const int bipassDiodeInterval                  = 0.0,
                          const PVCellCompanionModelConfigData& refCellConfigData = 0);

    ///@brief Default SolarString Configuration Data Destructor
    virtual ~SolarStringConfigData();

    //@brief Copy constructor
    SolarStringConfigData(const SolarStringConfigData& that);

private:
        ///@brief Operator = is not available since declared private and not implemented
    SolarStringConfigData& operator = (const SolarStringConfigData&);

};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the SolarString
///           input data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarStringInputData{

public:

    bool mIsFailed;                                   /**< (--)  trick_chkpnt_io(**) Whether this string is failed or not.    */
    int mNumFailedCells;                              /**< (--)  trick_chkpnt_io(**) the number of failed cells in the string */
    PVCellCompanionModelInputData mRefCellInputData;  /**< (--)  trick_chkpnt_io(**) input data for this section's reference cell.*/

    ///@brief Default SolarString Input Data Constructor
    SolarStringInputData(const bool isFailed                          = false,
                         const int numFailedCells                     = 0,
                         const PVCellCompanionModelInputData& refCellInputData = 0);

    ///@brief Default SolarString Input Data Destructor
    virtual ~SolarStringInputData();

    //@brief Copy constructor
    SolarStringInputData(const SolarStringInputData& that);

private:
    ///@brief Operator = is not available since declared private and not implemented
    SolarStringInputData& operator = (const SolarStringInputData&);

};


/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The  purpose of this class is to provide a data structure for the SolarString
/////////////////////////////////////////////////////////////////////////////////////////////////
class SolarString
{
    TS_MAKE_SIM_COMPATIBLE(SolarString);

public:
    /// @name    Embedded objects.
    /// @{
    /// @details Embedded objects are public to allow access from the Trick events processor.
    PVCellCompanionModel mRefCell;     /**< (--) pointer to a reference cell */
    /// @}

    /// @name    Malfunction terms.
    /// @{
    /// @details Malfunction targets are public to allow access from the Trick events processor.
    bool   mMalfFailString;            /**< (--) string failure malfunction flag */
    /// @}

     ///@brief Default SolarStringElect Constructor
     SolarString();

     ///@brief Default SolarStringElect Destructor
     virtual ~SolarString();

     ///@brief Initialize method as Gunns network links
     void initialize(const SolarStringConfigData& config, const SolarStringInputData& input);

     /// @brief Step method for updating the section
     void update(const bool isMinor, const double stringCurrent, const double stringVoltage, const double sunAngle, const double t, const double sunIntensity, const bool backSideLit);

     /// @brief Sets and resets the string failure malfunction.
     void setMalfFailString(const bool flag = false);

     double mLocalSunAngleScalar;     /**< (--) override at the SAW section level for the epsOverrides scalar which tunes SAW current generated at off angles */
     bool   mLocalSunAngleScalarFlag;  /**< (--) flag for local sun angle scalar override */

protected:

     double mBipassDiodeVoltageDrop;    /**< (V) trick_chkpnt_io(**) the voltage drop across each bypass diode.             */
     int    mBipassDiodeInterval;       /**< (--) trick_chkpnt_io(**) the number of cells per every one bypass diode         */
     double mBlockingDiodeVoltageDrop;  /**< (V) trick_chkpnt_io(**) the voltage drop across blocking diodes which can be configured to occur at N interval.*/
     int    mNumCells;         /**< (--) trick_chkpnt_io(**) the number of cells that compose this string*/
     int    mNumFailedCells;   /**< (--) the number of failed cells in the string*/
     double mVoltage;                   /**< (V)  the voltage  of string      */
     double mCurrent;                   /**< (amp) the current of string      */
     double mGeq;                       /**< (1/ohm) total equivalent conductance for string   */
     double mIeq;                       /**< (amp) total equivalent source vector for string */

private:
    ///@brief validates the input and configuration data that is passed in through initialize.
    void validate();

    //@brief Copy constructor is not available since declared private and not implemented
    SolarString(const SolarString& that);

    //@brief Operator = is not available since declared private and not implemented
    SolarString& operator =(const SolarString& that);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the total source vector contribution of this string
    /// @return (amp)- double SolarString::value representing total source vector of the string, Ieq_string
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getSourceVectorStamp() const{
        return mIeq;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the total conductance contribution of this string
    /// @return (1/ohm) double SolarString::value representing total conductance of the string, Geq_string
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getConductanceStamp() const{
        return mGeq;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the total string voltage
    /// @return (V) the total string voltage
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getVoltage() const{
        return mVoltage;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the strings current for the most recent operating conditions
    /// @return (amp) the total string current
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getCurrent() const{
        return mCurrent;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns whether the string is failed or not
    /// @return (--) if true, the entire string is failed
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    inline bool getIsFailed() const {
        return mMalfFailString;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the number of cells on this string
    /// @return (--) the total number of working cells on this string
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    inline int getNumCells()const{
        return mNumCells;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the number of cells on this string
    /// @return (--) the total number of failed cells on this string
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    inline int getNumFailedCells() const{
        return mNumFailedCells;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the open circuit voltage of this string's ref cell
    /// @return (V)- the voltage of this string's reference cell at open circuit conditions
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getRefCellVoc() const {
        return mRefCell.getVoc();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the source current
    /// @return (amp)- the source current
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getIsc() const {
        return mRefCell.getIsc();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the maximum source current
    /// @return (amp)- the source current
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getIscRef() const {
        return mRefCell.getIscRef();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details Sets the sun angle scalar value.  Allows the user to tune the current the array generates
    ///          at off angles.  It implements a power function on the sin(angle from environment) component
    ///          of the math that calculates the current the cell generates at any particular angle.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setSunAngleScalar(const double scalar) {
        double value = scalar;

        if(mLocalSunAngleScalarFlag) {
            value = mLocalSunAngleScalar;
        }

        mRefCell.setSunAngleScalar(value);
    }

};
/// @}
#endif
