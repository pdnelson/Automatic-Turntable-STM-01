#include <InputMux.h>
#include <StmBasicStepper.h>
#include <TurntableSpeed.h>
#include <RecordSize.h>
#include <LiftStatus.h>
#include <HomeStatus.h>
#include <memory>
#include <BaseTurntableCommand.h>
#include <StmShift.h>
#include <StmSerial.h>
#include <StmEncoder.h>
#include <ClutchStatus.h>
#include <CalibrationValues.h>
#include <Settings.h>
#include <StmHStepper.h>

#ifndef TurntableState_h
#define TurntableState_h

class TurntableState {
    public:
        TurntableState();

        StmShift outputShift;
        StmSerial serialComm;
        InputMux inputMux;
        StmBasicStepper verticalStepper;
        StmBasicStepper clutchStepper;
        StmEncoder azEncoder;
        StmHStepper horizontalStepper;
        CalibrationValues calibration;
        Settings settings;
        std::unique_ptr<BaseTurntableCommand> currentCommand;

        unsigned long clockMicros = 0;

        // Count variables
        unsigned long countCounter = 0;
        unsigned long upTimeSeconds = 0;

        // Speed variables
        TurntableSpeed selectedSpeed = TurntableSpeed::RpmAuto;
        unsigned long customSpeedIndicatorCounter = 0;

        void monitor();
        void executeCommand();
        void rotateSpeed();
        LiftStatus getLiftStatus();
        HomeStatus getHomeStatus();
        bool isPaused();
        ClutchStatus clutchEngaged();
        void updateCustomSpeed(float newSpeed);
        void updateSpeed(TurntableSpeed newSpeed);
        void rotateSize();
        void updateSize(RecordSize newSize);
        void pauseOrUnPause();
        void playOrReturn();
        void beginCalibrationRoutine();
        float getTargetSpeed();
        uint16_t getVerticalEncoderPos();
        void saveSettings();

    private:
        unsigned long liftDebounce = 0;
        uint8_t lastLiftStatus = LiftStatus::Lifted;

        void monitorCommandInput();
        void advanceCounts();
        void updateClockMicros();
        void blinkCustomSpeedIndicator();
};

#endif