#ifndef GunnsElectConverterInput_EXISTS
#define GunnsElectConverterInput_EXISTS

/**
@file
@brief    GUNNS Electrical Converter Input Link declarations

@defgroup  TSM_GUNNS_ELECTRICAL_CONVERTER_INPUT_LINK    GUNNS Electrical Converter Input Link
@ingroup   TSM_GUNNS_ELECTRICAL_CONVERTER

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Electrical Converter Input link.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY DEPENDENCY:
- ((GunnsElectConverterInput.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2019-10) (Initial))

@{
*/

#include "core/GunnsBasicLink.hh"
#include "aspects/electrical/TripLogic/GunnsTripLogic.hh"
#include "core/GunnsSensorAnalogWrapper.hh"
#include "math/approximation/TsLinearInterpolator.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"

// Forward-declaration of types.
class GunnsElectConverterInputConfigData;
class GunnsElectConverterInputInputData;
class GunnsElectConverterOutput;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Converter Input link.
///
/// @details  This models the input side of a DC-DC converter, or the supply side of a general
///           supply-demand interface between models.  This pairs with a GunnsElectConverterOutput
///           link, which may be in the same network, a different network or even a different sim.
///
///           This supplies voltage to the output side link (mInputVoltage), and receives a power
///           load (mInputPower) in response.  It creates the power load on the local node as a
///           current source by I = P/V, and updates I in minor step iterations for a converged
///           solution resulting in the correct P.
///
///           This has logic to handle cases where the demanded power load is too great for the
///           local network; this is called the 'overloaded state' (mOverloadedState).  It fails
///           over to this state if the node voltage goes negative, which happens when the network
///           can't supply the demanded power.  The overloaded state places a large conductance on
///           the node, instead of a current source.  This causes the network to converge to near-
///           zero voltage and high current, which should cause a realistic system to under-volt or
///           over-current trip off upstream.
///
///           This link can be connected to the Ground node for storage as a spare.  When connected
///           to the Ground node, it does nothing.  A converter input-output pair can be used to
///           simulate a short-circuit between separate networks.
///
///           This contains optional trip logic for input under-volt and over-volt trips.  These can
///           be associated with optional sensors in the network, or use 'truth' values.  Ideally,
///           you should always configure the under-volt trip to be active with a trip limit > 0 V;
///           this helps the network converge faster when overloaded.  Note that if the under-volt
///           trip is used, then to avoid tripping off when an upstream switch is opened, the
///           converter should be disabled at the same time or beforehand, set mEnabled = false.
///
///           This has an optional pointer to a paired GunnsElectConverterOutput link, set by the
///           output link.  This should be used when and only when both links are in the same
///           network.  This causes the two links to share input voltage and load values between
///           the network minor steps, for less latency in the supply-demand loop.
////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO extract new CPL to a utility class for reuse by other links what do CPL...
class GunnsElectConverterInput : public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsElectConverterInput);

    public:
        /// @brief  Default Constructor.
        GunnsElectConverterInput();
        /// @brief  Default Destructor.
        virtual ~GunnsElectConverterInput();
        /// @brief  Initialize the link with config data and port assignments.
        void initialize(      GunnsElectConverterInputConfigData& configData,
                        const GunnsElectConverterInputInputData&  inputData,
                        std::vector<GunnsBasicLink*>&             networkLinks,
                        const int                                 port0);
        /// @brief  Checks if the given node list is the same as this link's node list.
        void checkNodeList(GunnsNodeList* nodeList);
        /// @brief  Updates the link contributions to the system of equations.
        virtual void step(const double dt);
        /// @brief  Minor step method for non-linear iterations.
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief  Calculates flows across the link.
        virtual void computeFlows(const double);
        /// @brief  Returns the link's assessment of the network solution.
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief  Resets the link back to the previous minor step iteration.
        virtual bool resetLastMinorStep(const int convergedStep, const int absoluteStep);
        /// @brief  Computes and returns the input channel voltage and validity.
        bool computeInputVoltage(double& inputVoltage);
        /// @brief  Returns if the link is non-linear.
        virtual bool isNonLinear();
        /// @brief  Registers the given output side link with this input side link.
        void registerOutputLink(GunnsElectConverterOutput* outputLink);
        /// @brief  Resets any trips.
        void resetTrips();
        /// @brief  Sets the enabled flag.
        void setEnabled(const bool enabled);
        /// @brief  Sets the input power.
        void setInputPower(const double inputPower);
        /// @biref  Sets the reference power for efficiency calculation.
        void setReferencePower(const double referencePower);
        /// @brief  Returns the input voltage.
        double getInputVoltage() const;
        /// @brief  Returns the input voltage valid flag.
        bool getInputVoltageValid() const;
        /// @brief  Returns the input under-voltage trip logic.
        GunnsTripLogic* getInputUnderVoltageTrip();
        /// @brief  Returns the input over-voltage trip logic.
        GunnsTripLogic* getInputOverVoltageTrip();

    protected:
        SensorAnalog*              mInputVoltageSensor;    /**<    (1) trick_chkpnt_io(**) Pointer to the input voltage sensor. */
        SensorAnalog*              mInputCurrentSensor;    /**<    (1) trick_chkpnt_io(**) Pointer to the input current sensor. */
        TsLinearInterpolator*      mEfficiencyTable;       /**<    (1) trick_chkpnt_io(**) Pointer to the converter efficiency vs. power fraction table. */
        GunnsElectConverterOutput* mOutputLink;            /**< *o (1) trick_chkpnt_io(**) Pointer to the converter output side link. */
        bool                       mEnabled;               /**<    (1)                     Operation is enabled. */
        double                     mInputPower;            /**<    (W)                     Input channel power load received from the output side. */
        bool                       mInputPowerValid;       /**<    (1) trick_chkpnt_io(**) The input channel power load value is valid. */
        bool                       mResetTrips;            /**<    (1) trick_chkpnt_io(**) Input command to reset trips. */
        double                     mReferencePower;        /**<    (1)                     Reference power load for efficiency calculation. */
        double                     mInputVoltage;          /**<    (V)                     Input channel voltage sent to the output side. */
        bool                       mInputVoltageValid;     /**<    (1) trick_chkpnt_io(**) Input channel voltage value is valid. */
        GunnsTripLessThan          mInputUnderVoltageTrip; /**<    (1)                     Input under-voltage trip function. */
        GunnsTripGreaterThan       mInputOverVoltageTrip;  /**<    (1)                     Input over-voltage trip function. */
        double                     mConverterEfficiency;   /**<    (1) trick_chkpnt_io(**) The power conversion efficiency (0-1). */
        double                     mTotalPowerLoss;        /**<    (W)                     Total power loss through converter efficiency. */
        bool                       mLeadsInterface;        /**< *o (1) trick_chkpnt_io(**) This precedes the mOutputLink in the network. */
        bool                       mOverloadedState;       /**<    (1) trick_chkpnt_io(**) Network can't supply the power load. */
        bool                       mLastOverloadedState;   /**<    (1) trick_chkpnt_io(**) Last pass value of mOverloadedState. */
        /// @brief  Validates the configuration and input data.
        void validate(const GunnsElectConverterInputConfigData& configData,
                      const GunnsElectConverterInputInputData&  inputData) const;
        /// @brief  Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 1};
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsElectConverterInput(const GunnsElectConverterInput& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectConverterInput& operator =(const GunnsElectConverterInput& that);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Converter Input link configuration data.
///
/// @details  This is a data structure for the Electrical Converter Input link configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectConverterInputConfigData: public GunnsBasicLinkConfigData
{
    public:
        GunnsSensorAnalogWrapper* mInputVoltageSensor;         /**< (1) trick_chkpnt_io(**) Pointer to the input voltage sensor spotter. */
        GunnsSensorAnalogWrapper* mInputCurrentSensor;         /**< (1) trick_chkpnt_io(**) Pointer to the input current sensor spotter. */
        unsigned int              mTripPriority;               /**< (1) trick_chkpnt_io(**) Priority of trips in the network. */
        float                     mInputUnderVoltageTripLimit; /**< (V) trick_chkpnt_io(**) Input under-voltage trip limit. */
        float                     mInputOverVoltageTripLimit;  /**< (V) trick_chkpnt_io(**) Input over-voltage trip limit. */
        TsLinearInterpolator*     mEfficiencyTable;            /**< (1) trick_chkpnt_io(**) Pointer to the converter efficiency vs. power fraction table. */
        /// @brief  Default constructs this Electrical Converter Input configuration data.
        GunnsElectConverterInputConfigData(
                const std::string&        name                       = "",
                GunnsNodeList*            nodes                      = 0,
                GunnsSensorAnalogWrapper* inputVoltageSensor         = 0,
                GunnsSensorAnalogWrapper* inputCurrentSensor         = 0,
                const unsigned int        tripPriority               = 0,
                const float               inputUnderVoltageTripLimit = 0.0,
                const float               inputOverVoltageTripLimit  = 0.0,
                TsLinearInterpolator*     efficiencyTable            = 0);
        /// @brief  Default destructs this Electrical Converter Input configuration data.
        virtual ~GunnsElectConverterInputConfigData();
        /// @brief  Copy constructs this Electrical Converter Input configuration data.
        GunnsElectConverterInputConfigData(const GunnsElectConverterInputConfigData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectConverterInputConfigData& operator =(const GunnsElectConverterInputConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Electrical Converter Input link input data.
///
/// @details  This is a data structure for the Electrical Converter Input link input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsElectConverterInputInputData : public GunnsBasicLinkInputData
{
    public:
        bool   mEnabled;            /**< (1) trick_chkpnt_io(**) Initial operation enabled state. */
        double mInputVoltage;       /**< (V) trick_chkpnt_io(**) Initial input voltage. */
        double mInputPower;         /**< (W) trick_chkpnt_io(**) Initial input power load. */
        double mReferencePower;     /**< (W) trick_chkpnt_io(**) Initial reference power load for efficiency calculation. */
        /// @brief  Default constructs this Electrical Converter Input input data.
        GunnsElectConverterInputInputData(const bool   malfBlockageFlag  = false,
                                          const double malfBlockageValue = 0.0,
                                          const bool   enabled           = false,
                                          const double inputVoltage      = 0.0,
                                          const double inputPower        = 0.0,
                                          const double referencePower    = 0.0);
        /// @brief  Default destructs this Electrical Converter Input input data.
        virtual ~GunnsElectConverterInputInputData();
        /// @brief  Copy constructs this Electrical Converter Input input data.
        GunnsElectConverterInputInputData(const GunnsElectConverterInputInputData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsElectConverterInputInputData& operator =(const GunnsElectConverterInputInputData&);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (1)  Always returns true.
///
/// @details  The link is always non-linear.  Even though the load type may be linear, we still use
///           minor steps to run the trip logic.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterInput::isNonLinear()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  outputLink  (--)  Pointer to an Electrical Converter Output link.
///
/// @details  Sets mOutputLink to the given pointer.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterInput::registerOutputLink(GunnsElectConverterOutput* outputLink)
{
    mOutputLink = outputLink;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  Resets the tripped state of the contained trip logic functions.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterInput::resetTrips()
{
    mInputUnderVoltageTrip.resetTrip();
    mInputOverVoltageTrip.resetTrip();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  enabled  (--)  Enabled flag value to use.
///
/// @details  Sets mEnabled to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterInput::setEnabled(const bool enabled)
{
    mEnabled = enabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  inputPower  (W)  Input power value to use.
///
/// @details  Sets mInputPower to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterInput::setInputPower(const double inputPower)
{
    mInputPower = inputPower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  referencePower  (W)  Reference power value to use.
///
/// @details  Sets mReferencePower to the given value.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsElectConverterInput::setReferencePower(const double referencePower)
{
    mReferencePower = referencePower;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  double (V)  Input voltage.
///
/// @details  Returns the value of mInputVoltage.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsElectConverterInput::getInputVoltage() const
{
    return mInputVoltage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  bool  (--)  Whether the input-side voltage value is valid.
///
/// @details  Returns the value of mInputVoltageValid.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsElectConverterInput::getInputVoltageValid() const
{
    return mInputVoltageValid;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the input under-voltage trip logic.
///
/// @details  Returns the address of the mInputUnderVoltageTrip attribute;
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectConverterInput::getInputUnderVoltageTrip()
{
    return &mInputUnderVoltageTrip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @returns  GunnsTripLogic*  (--)  Pointer to the input over-voltage trip logic.
///
/// @details  Returns the address of the mInputOverVoltageTrip attribute;
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsTripLogic* GunnsElectConverterInput::getInputOverVoltageTrip()
{
    return &mInputOverVoltageTrip;
}

#endif
