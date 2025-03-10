#ifndef GunnsLosslessSource_EXISTS
#define GunnsLosslessSource_EXISTS

/**
@file
@brief    GUNNS Lossless Source Link declarations

@defgroup  TSM_GUNNS_ELECT_LINK_LOSSLESS_SOURCE_LINK    GUNNS Lossless Source Link
@ingroup   TSM_GUNNS_ELECT_LINK_LOSSLESS_SOURCE

@copyright Copyright 2019 United States Government as represented by the Administrator of the
           National Aeronautics and Space Administration.  All Rights Reserved.

@details
PURPOSE:
- (Classes for the GUNNS Lossless Source Model.)

REFERENCE:
- (TBD)

ASSUMPTIONS AND LIMITATIONS:
- (TBD)

LIBRARY_DEPENDENCY:
- (
   (./GunnsLosslessSource.o)
  )

PROGRAMMERS:
- (
   (Tristan Mansfield) (Axiom Space) (2025-03) (Specialized Source to allow for power conservation and an efficiency term) 
  )

@{
*/

#include "math/approximation/TsLinearInterpolator.hh"
#include "software/SimCompatibility/TsSimCompatibility.hh"
#include "software/exceptions/TsInitializationException.hh"
#include "core/GunnsBasicSource.hh"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lossless Source Configuration Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Lossless Source
///           configuration data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLosslessSourceConfigData : public GunnsBasicSourceConfigData
{
  public:
    /// @brief Default constructs this Lossless Source configuration data.
    GunnsLosslessSourceConfigData(
      const std::string& name  = "",
      GunnsNodeList*     nodes = 0,
      TsLinearInterpolator* efficiencyTable = 0
    );

    /// @brief Default destructs this Lossless Source configuration data.
    virtual ~GunnsLosslessSourceConfigData();

    /// @brief Copy constructs this Lossless Source configuration data.
    GunnsLosslessSourceConfigData(const GunnsLosslessSourceConfigData& that);
    TsLinearInterpolator* mEfficiencyTable; /**< (1) trick_chkpnt_io(**) Pointer to the converter efficiency vs. power fraction table. */
  protected:

  private:
    /// @brief Assignment operator unavailable since declared private and not implemented.
    GunnsLosslessSourceConfigData& operator =(const GunnsLosslessSourceConfigData&);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Lossless Source Input Data
///
/// @details  The sole purpose of this class is to provide a data structure for the Lossless Source
///           input data.
////////////////////////////////////////////////////////////////////////////////////////////////////
class GunnsLosslessSourceInputData : public GunnsBasicSourceInputData
{
  public:
    /// @brief    Default constructs this Lossless Source input data.
    GunnsLosslessSourceInputData(
      const bool   malfBlockageFlag  = false,
      const double malfBlockageValue = 0.0,
      const double sourceFlux        = 0.0,
      const double efficiency = 0.0,
      const double referencePower = 0.0,
      const bool   staticEfficiency = true
    );

    /// @brief    Default destructs this Lossless Source input data.
    virtual ~GunnsLosslessSourceInputData();

    /// @brief    Copy constructs this Lossless Source input data.
    GunnsLosslessSourceInputData(const GunnsLosslessSourceInputData& that);
    double mEfficiency;
    double mReferencePower; /**< (W) trick_chkpnt_io(**) Initial reference power load for efficiency calculation. */
    bool   mStaticEfficiency;
  protected:

  private:
    /// @details  Assignment operator unavailable since declared private and not implemented.
    GunnsLosslessSourceInputData& operator =(const GunnsLosslessSourceInputData&);

};

class GunnsLosslessSource : public GunnsBasicSource
{
  /**
   * TODO_: Make this inherit from GunnsBasicLink instead of GunnsBasicSource
   * && write all logic, instead of doing this weird inheritance dance between 
   * BasicSource and BasicLink -- It's not worth the headache of tracing nested
   * virtual calls
   * 
   * For now, the call-graph is:
   * GunnsBasicSource::step {
   *   GunnsBasicLink::processUserPortCommand
   *   GunnsBasicLink::updateState
   *   ... Apply mMalfBlockageFlag ...
   *   GunnsLosslessSource::buildSource {
   *     ...
   *   }
   *   Much later..
   *   GunnsLosslessSource::computeFlows{
   *     GunnsLosslessSource::computePower
   *     GunnsLosslessSource::transportFlux
   *   }
   * ... Some more stuff
   * }
   */
  TS_MAKE_SIM_COMPATIBLE(GunnsLosslessSource);

  public:
    /// @brief Default Constructor
    GunnsLosslessSource();

    /// @brief Default Destructor
    virtual ~GunnsLosslessSource();

    /// @brief Initializes the link
    void initialize(
      const GunnsLosslessSourceConfigData& configData,
      const GunnsLosslessSourceInputData&  inputData,
      std::vector<GunnsBasicLink*>&     networkLinks,
      const int                         port0,
      const int                         port1
    );

    inline double getEfficiency() {
      return this->mEfficiency;
    };

    inline bool setEfficiency(double newEfficiency) {
      if (newEfficiency <= 1.0 && newEfficiency > 0.0) {
        this->mEfficiency = newEfficiency;
        return true;
      }
      return false;
    }

    void validate(
      const GunnsLosslessSourceConfigData& configData,
      const GunnsLosslessSourceInputData& inputData
    );

    inline double getWastePower() {
      return this->mWastePower;
    }

    inline double estimateEfficiencyAtLoad(double power) {
      // If we're using dynamic efficiency, return the eff at given load, otherwise return current efficiency
      double eff = this->mEfficiency;
      if (!this->mStaticEfficiency && this->mSourceFlux > 0.0 && this->mEfficiencyTable != nullptr && this->mEfficiencyTable->isInitialized()) {
        eff = this->mEfficiencyTable->get(power);
      }
      return eff;
    }

    // Member vars
    double mReferencePower; /**< (1) Reference power load for efficiency calculation. Please just set this to 1. */

  protected:
    /// @brief Calculates flow across the link
    /// - This was only written to make the link use GunnsLossessSource::transportFlux
    virtual void computeFlows(const double dt) override;

    /// @brief Computes mPower given potentials and fluxes at each port (should be near 0.0)
    virtual void computePower() override;

    /// @brief Influx / Outflux on nodes does not = mFlux
    void transportFlux();
    void transportFlux(const int, const int) override;

    bool mStaticEfficiency; /**< (--) cio(*io) io(*io) If true, don't update efficiency based on power throughput */
    double mEfficiency;  /**< (%) trick_chkpnt_io(*io) io(*io) Efficiency of source; if not mStaticEfficiency, then calculated from mEfficiencyTable each timestep -- I_in = I_out / mEfficiency ; I_out = mSourceFlux */
    // Copying GunnsElectConverterInput 's 'power fraction' approach, but recommend to use mReferencePower to 1.0 for simplicity
    TsLinearInterpolator* mEfficiencyTable; /**< (1) trick_chkpnt_io(**) Pointer to the converter efficiency vs. power fraction table. */
    double mWastePower; /**< (W) trick_chkpnt_io(*io) io(*o) How much power is wasted from mEfficiency; mWasteHeat = Power_in - Power_out */

    double mOutputPower; /**< (W) cio(*io) io(*io) power to output node */
    double mInputPower; /**< (W) cio(*io) io(*io) power from input node */

  private:
    /// @details Define the number of ports this link class has.  All objects of the same link
    ///          class always have the same number of ports.  We use an enum rather than a
    ///          static const int so that we can reuse the NPORTS name and allow each class to
    ///          define its own value.
    enum {NPORTS = 2};
    /// @brief Builds the source vector terms of the links contribution to the network
    virtual void buildSource() override;

    /// @brief Copy constructor unavailable since declared private and not implemented.
    GunnsLosslessSource(const GunnsLosslessSource& that);

    /// @brief Assignment operator unavailable since declared private and not implemented.
    GunnsLosslessSource& operator =(const GunnsLosslessSource& that);
};

#endif