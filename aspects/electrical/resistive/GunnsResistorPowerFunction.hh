#ifndef GunnsResistorPowerFunction_EXISTS
#define GunnsResistorPowerFunction_EXISTS

/**
@file
@brief     GUNNS Resistor With Power Function declarations

@defgroup  TSM_GUNNS_ELECTRICAL_RESISTIVE_POWER_FUNCTION    GUNNS Resistor With Power Function Link
@ingroup   TSM_GUNNS_ELECTRICAL_RESISTIVE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Resistor With Power Function Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- ((GunnsResistorPowerFunction.o))

PROGRAMMERS:
- ((Jason Harvey) (CACI) (2016-05) (Initial))

@{
*/

#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicConductor.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Resistor With Power Function Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Resistor With
///           Power Function configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsResistorPowerFunctionConfigData : public GunnsBasicLinkConfigData
{
    public:
        double mResistance;     /**< (--) trick_chkpnt_io(**) Resistance to flow. */
        double mExponent;       /**< (--) trick_chkpnt_io(**) Exponent on the power function. */
        bool   mUseTangentLine; /**< (--) trick_chkpnt_io(**) Flag to enable tangent-line approximation. */
        /// @brief Default constructs this Resistor With Power Function configuration data.
        GunnsResistorPowerFunctionConfigData(const std::string& name           = "",
                                             GunnsNodeList*     nodes          = 0,
                                             const double       resistance     = 0.0,
                                             const double       exponent       = 0.0,
                                             const bool         useTangentLine = false);
        /// @brief Default destructs this Resistor With Power Function configuration data.
        virtual ~GunnsResistorPowerFunctionConfigData();
        /// @brief Copy constructs this Resistor With Power Function configuration data.
        GunnsResistorPowerFunctionConfigData(const GunnsResistorPowerFunctionConfigData& that);

    private:
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsResistorPowerFunctionConfigData& operator =(const GunnsResistorPowerFunctionConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Resistor With Power Function Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Resistor With
///           Power Function input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsResistorPowerFunctionInputData : public GunnsBasicLinkInputData
{
    public:
        /// @brief    Default constructs this Resistor With Power Function input data.
        GunnsResistorPowerFunctionInputData(const bool   malfBlockageFlag  = false,
                                            const double malfBlockageValue = 0.0);
        /// @brief    Default destructs this Resistor With Power Function input data.
        virtual ~GunnsResistorPowerFunctionInputData();
        /// @brief    Copy constructs this Resistor With Power Function input data.
        GunnsResistorPowerFunctionInputData(const GunnsResistorPowerFunctionInputData& that);

    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsResistorPowerFunctionInputData& operator =(const GunnsResistorPowerFunctionInputData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Resistor With Power Function Class
///
/// @details  This modifies the normal Ohm's Law implementation of a resistor (V=IR) by adding a
///           configurable exponent X on the current term as:
///
///             V = R*I^X,
///
///           which is a power function.  X can be tailored for different uses.  As an example, X=2
///           changes this to the Bernoulli equation for fluid volumetric flow, allowing simple
///           analysis of incompressible fluid circuits in a GUNNS Basic network without needing the
///           overhead of fluid properties and a fluid network.  More info about the Bernoulli
///           application can be found in this link's GunnShow link help page.
///
/// @note     Because of this link's flexibility in different aspects, we declare all variables
///           to be unit-less.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsResistorPowerFunction : public GunnsBasicLink
{
        TS_MAKE_SIM_COMPATIBLE(GunnsResistorPowerFunction);

    public:
        /// @brief Default Resistor With Power Function Constructor.
        GunnsResistorPowerFunction();
        /// @brief Default Resistor With Power Function Destructor.
        virtual ~GunnsResistorPowerFunction();
        /// @brief Initialize method.
        void initialize(const GunnsResistorPowerFunctionConfigData& configData,
                        const GunnsResistorPowerFunctionInputData&  inputData,
                        std::vector<GunnsBasicLink*>&               networkLinks,
                        const int                                   port0,
                        const int                                   port1);
        /// @brief step method for updating the link
        virtual void step(const double dt);
        /// @brief Minor Step method for non-linear Systems
        virtual void minorStep(const double dt, const int minorStep);
        /// @brief Method for computing the flows across the link
        virtual void computeFlows(const double dt);
        /// @brief Boolean for telling the solver if it is a non-linear link
        virtual bool isNonLinear();
        /// @brief Returns the link's assessment of the solution
        virtual SolutionResult confirmSolutionAcceptable(const int convergedStep,
                                                         const int absoluteStep);
        /// @brief Sets the flow resistance of the link.
        void   setResistance(const double resistance);
        /// @brief Returns the flow resistance of the link.
        double getResistance() const;

    protected:
        double mResistance;           /**< (--)                     Resistance to flow. */
        double mExponent;             /**< (--)                     Exponent in the power function. */
        bool   mUseTangentLine;       /**< (--)                     Flag to enable tangent-line approximation. */
        double mSystemAdmittance;     /**< (--) trick_chkpnt_io(**) Limited conductance for the system admittance matrix. */
        double mSystemSource;         /**< (--) trick_chkpnt_io(**) Source flux for the system source vector. */
        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();
        /// @brief Validates the link.
        void validate() const;
        /// @brief Updates the link's contributions to the system admittance matrix.
        void buildAdmittance();
        /// @brief Updates the link's contributions to the system source vector.
        void buildSource();
        /// @brief Updates the thru-flux due to the latest network potentials.
        virtual void computeFlux();

    private:
        /// @details Define the number of ports this link class has.  All objects of the same link
        ///          class always have the same number of ports.  We use an enum rather than a
        ///          static const int so that we can reuse the NPORTS name and allow each class to
        ///          define its own value.
        enum {NPORTS = 2};
        /// @brief Copy constructor unavailable since declared private and not implemented.
        GunnsResistorPowerFunction(const GunnsResistorPowerFunction& that);
        /// @brief Assignment operator unavailable since declared private and not implemented.
        GunnsResistorPowerFunction& operator =(const GunnsResistorPowerFunction& that);
};

/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   bool  (--)  Returns true when the link is non-linear.
///
/// @details  This returns true (non-linear) whenever the exponent is not equal to 1, and false
///           otherwise.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool GunnsResistorPowerFunction::isNonLinear()
{
    return (mExponent != 1.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  convergedStep  (--)  Not used.
/// @param[in]  absoluteStep   (--)  Not used.
///
/// @return   GunnsBasicLink::SolutionResult  (--)  Always returns CONFIRM.
///
/// @details  This link never has a reason to reject or delay the solution so it always confirms.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline GunnsBasicLink::SolutionResult GunnsResistorPowerFunction::confirmSolutionAcceptable(
        const int convergedStep __attribute__((unused)),
        const int absoluteStep  __attribute__((unused)))
{
    return GunnsBasicLink::CONFIRM;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param[in]  resistance  (--)  The flow resistance of the Link.
///
/// @details  Sets the flow resistance of the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsResistorPowerFunction::setResistance(const double resistance)
{
    mResistance = resistance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @return   double  (--)  The flow resistance of the Link
///.
/// @details  Returns the flow resistance of the link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline double GunnsResistorPowerFunction::getResistance() const
{
    return mResistance;
}

#endif
