

// Includes from ms-utils
#include "software/SimCompatibility/TsSimCompatibility.hh"
// Includes from gunns-ts-models
#include "common/controllers/fluid/TsPumpMotorController.hh"

class HarveySignal
{
    TS_MAKE_SIM_COMPATIBLE(HarveySignal);

    public:
        TsPumpMotorControllerConfigData pcConfig; /**< (--) config, ATCS pump motor & controller. */
        TsPumpMotorControllerInputData  pcInput;  /**< (--) input, ATCS pump motor & controller. */
        TsPumpMotorController pc;                 /**< (--) ATCS pump motor & controller. */

        HarveySignal() {
            pcConfig.mMotor.mWindingResistance = 1.6;
            pcConfig.mMotor.mTorqueConstant    = 2.7e-2;
            pcConfig.mMotor.mFrictionConstant  = 1.2e-5;
            pcConfig.mMotor.mFrictionMinSpeed  = 5000.0;
            pcConfig.mMotor.mInertia           = 1.0e-3;
            pcConfig.mForcingGain              = 0.2;
            pcConfig.mDampingGain              = 0.2;
            pcConfig.mDampingCutoff            = 0.01;
            pcConfig.mTemperatureTripLimit     = 400.0;
            pcConfig.mTemperatureTripReset     = 395.0;
            pcConfig.mMinVoltage               = 22.0;
            pcConfig.mMaxVoltage               = 40.0;
            pcConfig.mControllerPowerLoad      = 5.0;
            pcConfig.mNoiseAmplitude           = 0.0;
            pcConfig.mNoiseFrequency           = 0.0;
            pcConfig.mStartupCurrentLimit      = 20.0;    
            pcInput.mCommandEnable = true;
            pcInput.mCommandSpeed  = 7000.0;
            pcInput.mVoltage       = 32.0;
        }

        void initialize() {
            pc.initialize(pcConfig, pcInput, "harvey.pumpController");
        }
        void update(const double dt) {
            pc.setSensedSpeed(pc.mMotor.getSpeed());
            pc.step(dt);
        }
};
