#include <InputMux.h>
#include <Stepper.h>
#include <TurntableSpeed.h>
#include <RecordSize.h>
#include <LiftStatus.h>
#include <HomeStatus.h>
#include <memory>
#include <BaseTurntableCommand.h>
#include <StmShift.h>

#ifndef TurntableState_h
#define TurntableState_h

class TurntableState {
    public:
        TurntableState();

        StmShift outputShift;
        InputMux inputMux;
        Stepper movementStepper;
        Stepper clutchStepper;
        std::unique_ptr<BaseTurntableCommand> currentCommand;

        unsigned long clockMicros = 0;

        // Count variables
        unsigned long countCounter = 0;
        unsigned long upTimeSeconds = 0;

        // Speed variables
        TurntableSpeed selectedSpeed = TurntableSpeed::RpmAuto;
        float targetSpeed = -1;
        unsigned long customSpeedIndicatorCounter = 0;

        // Size variables
        RecordSize selectedSize = RecordSize::InAuto;

        void monitor();
        void executeCommand();
        void rotateSpeed();
        LiftStatus getLiftStatus();
        HomeStatus getHomeStatus();
        bool isPaused();
        void updateSpeed(TurntableSpeed newSpeed);
        void rotateSize();
        void updateSize(RecordSize newSize);

    private:
        unsigned long liftDebounce = 0;
        uint8_t lastLiftStatus = LiftStatus::Lifted;

        unsigned long homeDebounce = 0;
        uint8_t lastHomeStatus = HomeStatus::Homed;

        void monitorCommandInput();
        void advanceCounts();
        void updateClockMicros();
        void blinkCustomSpeedIndicator();
};

#endif