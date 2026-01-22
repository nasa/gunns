
#include "core/GunnsNetworkSpotter.hh"
#include "aspects/fluid/potential/GunnsLiquidCentrifugalPump.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Configuration Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class PumpMotorSpotterConfigData : public GunnsNetworkSpotterConfigData
{
    public:
        PumpMotorSpotterConfigData(const std::string& name):
            GunnsNetworkSpotterConfigData(name) {;}
        virtual ~PumpMotorSpotterConfigData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Input Data
////////////////////////////////////////////////////////////////////////////////////////////////////
class PumpMotorSpotterInputData : public GunnsNetworkSpotterInputData
{
    public:
        PumpMotorSpotterInputData() {;}
        virtual ~PumpMotorSpotterInputData() {;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief    Test Example Derived Network Spotter Class.
////////////////////////////////////////////////////////////////////////////////////////////////////
class PumpMotorSpotter : public GunnsNetworkSpotter
{
    TS_MAKE_SIM_COMPATIBLE(PumpMotorSpotter);
    public:
        friend class UtGunnsNetworkSpotter;
        PumpMotorSpotter(GunnsLiquidCentrifugalPump& pumpRef) : mPumpRef(pumpRef) {;}
        virtual     ~PumpMotorSpotter() {;}
        virtual void initialize(const GunnsNetworkSpotterConfigData* configData,
                                const GunnsNetworkSpotterInputData*  inputData) {;}
        virtual void stepPreSolver(const double dt) {}
        virtual void stepPostSolver(const double dt) {;}

        void setMotorSpeed(const double setSpeed) {mPumpRef.setMotorSpeed(setSpeed);}

    protected:
        GunnsLiquidCentrifugalPump& mPumpRef;              /**< (--) example reference to a network link */
        const PumpMotorSpotterConfigData* validateConfig(const GunnsNetworkSpotterConfigData* config);
        const PumpMotorSpotterInputData*  validateInput (const GunnsNetworkSpotterInputData* input);
};