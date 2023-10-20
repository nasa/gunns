#ifndef GunnsThermalSource_EXISTS
#define GunnsThermalSource_EXISTS
/********************************** TRICK HEADER ***************************************************
@defgroup  TSM_GUNNS_THERMAL_SOURCE    Gunns ThermalSource Link
@ingroup   TSM_GUNNS_THERMAL

@copyright Copyright 2023 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
   (The GunnsThermalSource is a medium to distribute a heat flux across multiple nodes in a Gunns
    thermal network. It is a multi-port link, and the proportion of the overall flux that is applied
    to each specific port is given in a flux-distribution-fraction array.

    GunnsThermalSource will multiply the demanded flux by a tuning scalar, which can represent how
    well a source converts electrical power or solar radiation into thermal heatflux. The resulting
    heatflux, with units still in Watts, is then distributed to each of its nodes in the network.

    The GunnsThermalSource can be malfunctioned with a blockage, in which a percentage of the
    demanded flux is not transferred to the nodes. This capability is already provided by the
    BasicLink's blockage attribute, set in the InputData. The GunnsThermalSource also has an
    override function, which will deliver a given value of flux regardless of other inputs.)

REQUIREMENTS:
    ()

REFERENCE:
    ()

ASSUMPTIONS AND LIMITATIONS:
    ()

LIBRARY DEPENDENCY:
    ((GunnsThermalSource.o))

PROGRAMMERS:
    ((Joe Valerioti) (L3) (Jan 2013) (derived from GunnsBasicLink))
@{
***************************************************************************************************/
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "core/GunnsBasicLink.hh"
#include <vector>
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Source Configuration Data
///
/// @details  This class provides a data structure for the ThermalSource ConfigData.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalSourceConfigData: public GunnsBasicLinkConfigData
{
    public:
        /// @details  Scales the demanded flux before it is applied as heat.
        double cTuningScalar; /**< (--) trick_chkpnt_io(**) Scalar for tuning the applied heat flux */

        /// @details  Vector of variable size, defines the fraction of flux distributed to each port.
        std::vector<double> cFluxDistributionFractions; /**< (--) trick_chkpnt_io(**) Vector of flux-distribution-fractions*/

        /// @details  Number of ports override value. For use in super networks where the number of ports is different than in the subnetwork.
        int    cNumPortsOverride;    /**< (--) Number of ports override  (optional) */

        /// @brief  Default constructs this Thermal Source configuration data.
        GunnsThermalSourceConfigData(const std::string& name = "unnamed source",
                                        GunnsNodeList* nodes =   0,
                                   const double tuningScalar = 1.0,
              std::vector<double>* fluxDistributionFractions =   0);

        /// @brief  Default destructs this ThermalSource ConfigData.
        virtual ~GunnsThermalSourceConfigData();
        /// @brief  Copy constructs this ThermalSource ConfigData.
        GunnsThermalSourceConfigData(const GunnsThermalSourceConfigData& that);
        /// @brief  Override cFluxDistributionFractions.
        void overrideFluxDistributionFractions(double* fractions, const int numFractions);

    protected:
        // nothing
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalSourceConfigData& operator=( const GunnsThermalSourceConfigData&);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Thermal Source Input Data
///
/// @details  This class provides a data structure for the ThermalSource InputData.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalSourceInputData: public GunnsBasicLinkInputData
{
    public:
        double iDemandedFlux;            /**< (--) trick_chkpnt_io(**) Demanded flux of the link */
        bool   iMalfFluxOverrideFlag;    /**< (--) trick_chkpnt_io(**) Malfunction flag for heat flux override */
        double iMalfFluxOverrideValue;   /**< (W)  trick_chkpnt_io(**) Malfunction value for heat flux override */

        /// @brief   Default constructs this ThermalSource InputData.
        GunnsThermalSourceInputData(const bool malfBlockageFlag        = false,
                                    const double malfBlockageValue     = 0.0,
                                    const double demandedFlux          = 0.0,
                                    const bool malfFluxOverrideFlag    = false,
                                    const double malfFluxOverrideValue = 0.0);

        /// @brief   Default destructs this ThermalSource InputData.
        virtual ~GunnsThermalSourceInputData();
        /// @brief   Copy constructs this ThermalSource InputData.
        GunnsThermalSourceInputData(const GunnsThermalSourceInputData& that);

    protected:
        // nothing
    private:
        /// @details  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalSourceInputData& operator=(const GunnsThermalSourceInputData&);
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    ThermalSource class compatible in a GUNNS thermal network.
///
/// @details  The GunnsThermalSource distributes a heat flux across multiple nodes. The flux is
///           scaled by a tuning scalar, as well as a block malfunction (if active). The proportion
///           of the overall flux that is applied to each specific port is given by the
///           flux-distribution-fraction array.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsThermalSource: public GunnsBasicLink
{
    TS_MAKE_SIM_COMPATIBLE(GunnsThermalSource);

    public:
        /// @name    Malfunction terms.
        /// @{
        /// @details Malfunction targets are public to allow access from the Trick events processor.
        bool   mMalfFluxOverrideFlag;    /**< (--) Malfunction flag for heat flux override */
        double mMalfFluxOverrideValue;   /**< (W)  Malfunction value for heat flux override */
        /// @}

        /// @brief  Default Constructor.
        GunnsThermalSource();
        /// @brief  Default Destructor.
        virtual ~GunnsThermalSource();

        /// @brief  Initializes the link with Config and Input data
        void initialize(const GunnsThermalSourceConfigData& configData,
                        const GunnsThermalSourceInputData&  inputData,
                        std::vector<GunnsBasicLink*>& networkLinks,
                        std::vector<int>* portsVector);

        /// @brief  Step method for updating the link.
        void step(const double dt);

        /// @brief  Updates the state of the link.
        virtual void updateState(const double dt);

        /// @brief  Method for computing the flows across the link.
        void computeFlows(const double dt);

        /// @brief Sets and resets the flux override malfunction.
        void setMalfFluxOverride(const bool flag = false, const double value = 0.0);

    protected:
        static const double FRACTION_TOLERANCE; /**< (--) trick_chkpnt_io(**) Fraction normalization threshold */

        bool   mTemperatureOverrideFlag;        /**< (--)                     Activates the all-ports temperature override */
        double mTemperatureOverrideValue;       /**< (K)                      Value used for the all-ports temperature override */

        /// @details  Actual flux is a product of demanded flux times blockage and tuningScalar.
        double mDemandedFlux;                   /**< (W)                      Demanded heat flux through the link */

        /// @details  Scales the demanded flux before it is applied as heat.
        double mTuningScalar;                   /**< (--) trick_chkpnt_io(**) Scalar for tuning the applied heat flux */

        /// @details  Array of variable size, defines the fraction of flux distributed to each port.
        double* mFluxDistributionFractions;     /**< (--) trick_chkpnt_io(**) Array of flux-distribution-fractions*/

        /// @details  Average value of temperature among the Source's array of port nodes.
        double mAvgPortTemperature;             /**< (K)                      Average temperature of ports*/

        /// @brief Implements an empty admittance map since this is a source-only link.
        virtual void createAdmittanceMap();

        /// @brief Implements an empty admittance matrix since this is a source-only link.
        virtual void allocateAdmittanceMatrix();

        /// @brief  Builds the source vector terms of the links contribution to the network.
        virtual void buildSourceVector();

        /// @brief  Validates the ThermalSource Config and Input data, called internally.
        void validate(const GunnsThermalSourceConfigData& configData,
                      const GunnsThermalSourceInputData&  inputData) const;

        /// @brief Virtual method for derived links to perform their restart functions.
        virtual void restartModel();

        /// @brief  Determines and sets the AvgPortTemperature.
        void determineAvgPortTemperature();

        /// @brief  Checks for valid implementation-specific port node assignment.
        virtual bool checkSpecificPortRules(const int port, const int node) const;

        /// @brief  Skips updating the admittance map, which is unused by this link.
        virtual void updateAdmittanceMap();

    private:
        /// @brief  Copy constructor unavailable since declared private and not implemented.
        GunnsThermalSource(const GunnsThermalSource& that);
        /// @brief  Assignment operator unavailable since declared private and not implemented.
        GunnsThermalSource& operator=(const GunnsThermalSource& that);
};
/// @}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This link only updates the source vector, and never the admittance matrix, since this
///           only implmements the flow source effect and no other effects that use the admittance
///           matrix.  Therefore we leave the admittance map in its default-constructed, empty state
///           and the network solver will not copy and admittance matrix values from this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsThermalSource::createAdmittanceMap()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This link only updates the source vector, and never the admittance matrix, since this
///           only implmements the flow source effect and no other effects that use the admittance
///           matrix.  Therefore we leave the admittance matrix un-allocated, and the network solver
///           will not copy and admittance matrix values from this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsThermalSource::allocateAdmittanceMatrix()
{
    // nothing to do
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @details  This link only updates the source vector, and never the admittance matrix, since this
///           only implmements the flow source effect and no other effects that use the admittance
///           matrix.  Therefore we leave the admittance map un-updated, and the network solver
///           will not copy and admittance matrix values from this link.
////////////////////////////////////////////////////////////////////////////////////////////////////
inline void GunnsThermalSource::updateAdmittanceMap()
{
    // nothing to do
}

#endif
