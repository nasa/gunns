#ifndef PVCellCompanionModel_EXISTS
#define PVCellCompanionModel_EXISTS
/**
@file
@brief      PVCellCompanionModel declarations

@defgroup   TSM_GUNNS_ELECTRICAL_PVCellCompanionModel PVCellCompanionModel Model
@ingroup    TSM_GUNNS_ELECTRICAL

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

 PURPOSE:
 (The PVCellCompanionModel class represents the companion model for a photo cell network as seen in reference 2 in the Solar Power chapter.
 The equivalent cell circuit is reduced down to an equivalent Geq and Ieq model, using implicit differentiation for dI/dV. This class serves
 to model a single cell on a solar array.


 REFERENCE:
    1.) Najim, Farid N. Circuit Simulation. Hoboken, NJ: Wiley, 2010. Print
            - Information about network companion models, and general network solving tips and solutions.
            - Information about logarithmic damping of nonlinear curves with step feedback.
    2.) Patel, Mukund R. Spacecraft Power Systems. Boca Raton: CRC, 2005. Print.
            - Example network of a solar cell
            - Current and Voltage curve characteristics
            - General behavior and background for solar arrays. See Solar Array chapter.


              Photocell Circuit
  //===========================================
  //
  //                                  Rs
  //      --------------------------/\/\/\----- V+
  //      |        |          |
  //     (↑) Isc   ⊽   Id   /\/\/\ Rsh
  //      |        |          |
  //      ------------------------------------- V-
  //
  //===============================================

 ASSUMPTIONS AND LIMITATIONS:
 (
 - Assume IscRef is derived from the cell current per cm^2 * the light collecting area of the cell
 - Assume initial conditions using mVocRef, mVmpRef, mIscRef, mImpRef, mTemperatureRef. These conditions taken together
   will pin down the corners of the I-V curve for the cell.
 - Assume all cell degradations can be factored into one degradation term, mCellDegradation, which affects the
   solar intensity parameter.
 - The cell should operate in the near constant current portion of the I-V curve for maximum stability. If operating
   near Voc, you will need to tune the Vcritical point to aid convergence of this link in a Gunns network.

 LIBRARY DEPENDENCY:
 (
 (aspects/electrical/SolarArray/PVCellCompanionModel.o)
 )

 PROGRAMMERS:
 (
 (Nicholas Kaufmann) (L-3 Comm) (8/2013) (Initial Prototype)
 )
/////////////////////////////////////////////////////////////////////////////////
/// The PVCellCompanionModel is ...
/////////////////////////////////////////////////////////////////////////////////
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the config data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class PVCellCompanionModelConfigData{

public:
        ///@brief Default Configuration Data Constructor
        PVCellCompanionModelConfigData(const double vocRef               = 0.0,
                                       const double vmpRef               = 0.0,
                                       const double vocTempCoeff         = 0.0,
                                       const double iscRef               = 0.0,
                                       const double impRef               = 0.0,
                                       const double iscTempCoeff         = 0.0,
                                       const double iSat                 = 0.0,
                                       const double temperatureRef       = 0.0,
                                       const double cellDegradation      = 0.0,
                                       const double seriesResistance     = 0.0,
                                       const double shuntResistance      = 0.0,
                                       const double vCritc               = 0.0,
                                       const double backSideIscReduction = 0.0,
                                       const double cellEfficiency       = 0.0, // used only by arrays outside of LEO.
                                       const double cellArea             = 0.0  // used only by arrays outside of LEO.
                                       );

    ///@brief Default Configuration Data destructor
    virtual ~PVCellCompanionModelConfigData();

    //@brief Copy constructor
    PVCellCompanionModelConfigData(const PVCellCompanionModelConfigData& that);

    double mVocRef;                /**< (V) trick_chkpnt_io(**) the open circuit voltage reference value.                                     */
    double mVmpRef;                /**< (V) trick_chkpnt_io(**) the voltage max power reference value.                                        */
    double mVocTempCoefficient;    /**< (--) trick_chkpnt_io(**) the open circuit voltage coefficient. see page (XYZ) of (XYZ) document       */
    double mIscRef;                /**< (amp) trick_chkpnt_io(**) the source current reference value.                                         */
    double mImpRef;                /**< (amp) trick_chkpnt_io(**) the max power current reference value.                                      */
    double mIscTempCoefficient;    /**< (--) trick_chkpnt_io(**) the source current temperature coefficient. see page (XYZ) of (XYZ) document */
    double mIsat;                  /**< (amp) trick_chkpnt_io(**) the saturation current of the cell. aka "Dark Current"                      */
    double mTemperatureRef;        /**< (K)  trick_chkpnt_io(**) the temperature at which the reference values were recorded.                 */
    double mCellDegradation;       /**< (--) trick_chkpnt_io(**) the percent amount to degrade cell by. This accounts for age, damage, etc    */
    double mRs;                    /**< (ohm) trick_chkpnt_io(**) the cell series resistance                                                  */
    double mRsh;                   /**< (ohm) trick_chkpnt_io(**) the cell shunt resistance                                                   */
    double mVCrit;                 /**< (--) trick_chkpnt_io(**) a value multiplier for voc, after Vcrit*mVoc, model  apply damping to dV     */
    double mBackSideIscReduction;  /**< (--) trick_chkpnt_io(**) The percentage Isc drops by when the cell is back lit onl. Ranges from 0 to 1*/
    double mCellEfficiency;        /**< (--) trick_chkpnt_io(**) The ratio of maximum electrical output power (Voc*Isc) to input solar
                                                                 power (SolarFlux*CellArea). */
    double mCellArea;              /**< (m^2) trick_chkpnt_io(**) The reference cell's area. */



private:
    ///@brief Operator = is not available since declared private and not implemented
    PVCellCompanionModelConfigData& operator = (const PVCellCompanionModelConfigData&);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The purpose of this class is to provide a data structure for the input data.
/////////////////////////////////////////////////////////////////////////////////////////////////
class PVCellCompanionModelInputData{

public:

    ///@brief Default Input Data Constructor
    PVCellCompanionModelInputData(const bool cellPowerMalfIsEnabled = false,
                                  const bool cellDegradationMalfIsEnabled = false,
                                  const double cellDegradationOverrideValue = 1.0);

    ///@brief Default Input Data destructor
    virtual ~PVCellCompanionModelInputData();

    //@brief Copy constructor
    PVCellCompanionModelInputData(const PVCellCompanionModelInputData& that);

    bool   mCellPowerMalfunctionIsEnabled;       /**< (--)  trick_chkpnt_io(**) bool represents initial state of cell power malfunction.                                */
    bool   mCellDegradationMalfunctionIsEnabled; /**< (--)  trick_chkpnt_io(**) bool represents the inital state of the cell degradation malfunction                    */
    double mCellDegradationOverrideValue;        /**< (--)  trick_chkpnt_io(**) a double value which overrides default cell degradation when assoicated boolean is true */

private:
        ///@brief Operator = is not available since declared private and not implemented
    PVCellCompanionModelInputData& operator = (const PVCellCompanionModelInputData&);

};

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  The  purpose of this class is to provide a data structure for the PVCellCompanionModel
/////////////////////////////////////////////////////////////////////////////////////////////////
class PVCellCompanionModel
{
    TS_MAKE_SIM_COMPATIBLE(PVCellCompanionModel);

public:
    /// @name    Malfunction terms.
    /// @{
    /// @details Malfunction targets are public to allow access from the Trick events processor.
    bool   mCellPowerMalfunctionIsEnabled;       /**< (--) bool represents initial state of cell power malfunction.                                */
    bool   mCellDegradationMalfunctionIsEnabled; /**< (--) bool represents the inital state of the cell degradation malfunction                    */
    double mCellDegradationOverrideValue;        /**< (--) a double value which overrides default cell degradation when assoicated boolean is true */
    /// @}

    ///@brief Default Constructor
    PVCellCompanionModel();
    ///@brief Default desonstructor
    virtual ~PVCellCompanionModel();
    ///@brief Initializes the model with configuration and input data
    void initialize(const PVCellCompanionModelConfigData& cd, const PVCellCompanionModelInputData& id);
    ///@brief updates the reference cell to account for changes in parameters which affect cell performance.
    void update(const bool isMinor, const double V, const double I, const double sunAngle, const double T, const double sunIntensity,const bool backSideLit);
    /// @brief Sets and resets the cell power malfunction.
    void setMalfCellPower(const bool flag = false);
    /// @brief Sets and resets the cell degradation malfunction.
    void setMalfCellDegrade(const bool flag = false, const double value = 0.0);

private:
    ///@brief updates physical parameters of the cell when operating conditions have changed.
    void updatePhysicalParameters();
    ///@brief updates the companion model of this cell.
    void updateCompanionModel();
    ///@brief checks the actual values against physically possible boundaries, and limits them if needed.
    void dampAndBoundIVCurve();
    ///@brief constructs the effective conductance of the cell under the latest operating conditions.
    void setupConductance();
    ///@brief constructs the effective source vector of the cell under the latest operating conditions.
    void setupSourceVector();
    ///@brief returns a double which is used to apply a degrading effects to cell performance.
    double degrade();
    ///@brief validates the passed in configuration and input data of initializeXYZXYZ
    void validate();
    ///@brief Operator = is not available since declared private and not implemented
    PVCellCompanionModel& operator = (const PVCellCompanionModel&);

protected:
    static const double mMaxDegradation;    /**< (--)  trick_chkpnt_io(**) */

    //////////////////////////////////////
    /// Curve Reference parameters     ///
    //////////////////////////////////////
    ///These parameters are values that define the shape of the I-V curve. It pins down the edge locations. Most values can be found in solar cell spec sheets.
    ///Many variables affect the shape of the IV curve, some can be simplified to linear relationships. In this case, we  only need a reference point for the
    /// performance of the cell at some starting condition. Then as parameters change, say the temperature, we can shift the curve in some pre defined manner.
    double mVocRef;               /**< (V) trick_chkpnt_io(**) the open circuit voltage reference point                                          */
    double mVmpRef;               /**< (V) trick_chkpnt_io(**) the voltage at max power reference point.                                         */
    double mVocTempCoefficient;   /**< (--) trick_chkpnt_io(**) the open circuit voltage coefficient. see (ref 2) document                       */
    double mIscRef;               /**< (amp) trick_chkpnt_io(**) the source current reference point                                              */
    double mImpRef;               /**< (amp) trick_chkpnt_io(**) the current at max power reference point.                                       */
    double mIscTempCoefficient;   /**< (--) trick_chkpnt_io(**) the source current temperature coefficient. see (ref 2) document                 */
    double mIsat;                 /**< (amp) trick_chkpnt_io(**) the saturation current of the solar cell. Also known as dark current.           */
    double mTemperatureRef;       /**< (K) trick_chkpnt_io(**) the source current reference point                                                */
    double mCellDegradation;      /**< (--) trick_chkpnt_io(**) the percentage amount to degrade cell by. This accounts for age, meteorites, etc */
    double mRs;                   /**< (ohm) trick_chkpnt_io(**) the cell series resistance                                                      */
    double mRsh;                  /**< (ohm) trick_chkpnt_io(**) the cell shunt resistance                                                       */
    double mVCrit;                /**< (--) trick_chkpnt_io(**) a value multiplier for voc, after Vcrit*mVoc, model will apply damping to dV     */
    double mBackSideIscReduction; /**< (--) trick_chkpnt_io(**) The percentage Isc drops by when the cell is back lit only.                      */
    double mCellEfficiency;        /**< (--) trick_chkpnt_io(**) The ratio of maximum electrical output power (Voc*Isc) to input solar
                                                                 power (SolarFlux*CellArea). */
    double mCellArea;              /**< (m2) trick_chkpnt_io(**) The reference cell's area. */

    ///////////////////////////////////////////////////////
    /// INPUTS - variables to read from other subystems ///
    ///////////////////////////////////////////////////////
    double mTemperature;  /**< (K) the cell temperature. Read from THERMAL subsystem. Value passed through solar section.          */
    double mSunAngle;     /**< (rad) the angle sun rays make with plane of cell. Read from ENV. Value passed through solar section.  */
    double mSunAngleFromEnv;  /**< (rad)  original sun angle before being bounded                                                    */
    double mSunAngleScalar;   /**< (--) exponent on the sine function of the fall off of the solar array power with respect to the sun angle */
    double mSunIntensity; /**< (--) the intensity of the sun as felt by this cell. Ranges from 0 to 1                              */
    bool   mIsBackSideLit;/**< (--) Back of cell is lit if flag set to true, sun intensity reduced by mBackSideIscReduction %      */

    ///////////////////////
    /// State variables ///
    ///////////////////////
    double mV;        /**< (V) voltage at the present operating conditions                                                                      */
    double mVlast;    /**< (V) voltage at the last operating conditions                                                                         */
    double mVmp;      /**< (V) the voltage which would give maximum power at the present operating conditions                                   */
    double mVoc;      /**< (V) the open circuit voltage at the present operating conditions                                                     */
    double mVd;       /**< (V) the voltage at the diode used in the companion model network which models the physical behavior of the photocell */

    double mI;         /**< (amp) current at the present operating conditions                                                                   */
    double mIlast;     /**< (amp) current at the last operating conditions                                                                      */
    double mImp;       /**< (amp) the current which would give maximum power at the present operating conditions                                */
    double mIsc;       /**< (amp) the short circuit current at the present operating conditions                                                 */
    double mId;        /**< (amp) the current through the diode used in the network which models the physical behavior of the photocell         */
    double mIl;        /**< (amp) the current to the load used in the network which models the physical behavior of the photocell */

    double mIeqCell;   /**< (amp) the equivalent current for the reference cell companion model stamp     */
    double mGeqCell;   /**< (1/ohm) the equivalent conductance for the reference cell companion model stamp */
    double mLambda;    /**< (--) this value is the q/K*T . It helps us simplify calculations             */

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Equivalent conductance of the cell
    /// @return (1/ohm) the equivalent conductance of the cell.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getConductanceStamp() const{
        return mGeqCell;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Equivalent source vector of the cell
    /// @return (amp) the equivalent source vector of the cell.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getSourceVectorStamp() const {
        return mIeqCell;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the maximum power current
    /// @return (amp)- the current at the maximum power point on the I-V curve
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getImp() const {
        return mImp;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the maximum power voltage
    /// @return (V)- the voltage at the maximum power point on the I-V curve
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getVmp() const {
        return mVmp;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the voltage
    /// @return (V)- the voltage of the cell at the present point on the I-V curve
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getV() const {
        return mV;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the open circuit voltage
    /// @return (V)- the voltage of the cell at open circuit conditions
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getVoc() const {
        return mVoc;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the source current
    /// @return (amp)- the source current
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getIsc() const {
        return mIsc;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the cell's efficiency
    /// @return (--)- the solar cell's efficiency.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getCellEfficiency() const {
        return mCellEfficiency;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the cell's area
    /// @return (m^2)- the solar cell's area.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getCellArea() const {
        return mCellArea;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details  Returns the maximum source current
    /// @return (amp)- the source current
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    inline double getIscRef() const {
        return mIscRef;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details Sets the IscRef value. This is a function of cell efficiency, solar flux, cell area,
    ///          and cell open circuit voltage. For deep space arrays where solar flux magnitude varies
    ///          significantly over the course of the mission, the IscRef is calculated for the cell
    ///          by the SolarArray class and passed in prior to updating this cell companion model.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setIscRef(const double iscRef) {
        mIscRef = iscRef;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @details Sets the sun angle scalar value.  Allows the user to tune the current the array generates
    ///          at off angles.  It implements a power function on the sin(angle from environment) component
    ///          of the math that calculates the current the cell generates at any particular angle.
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void setSunAngleScalar(const double scalar) {
        mSunAngleScalar = scalar;
    }

};

/// @}
#endif
