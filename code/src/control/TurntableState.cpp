#include <TurntableState.h>
#include <Arduino.h>
#include <CommandResult.h>
#include <Constants.h>
#include <HomeStatus.h>
#include <InputMux.h>
#include <LiftStatus.h>
#include <memory>
#include <MuxPin.h>
#include <Pin.h>
#include <RecordSize.h>
#include <Stepper.h>
#include <StmShift.h>
#include <TurntableSpeed.h>
#include <CmdPause.h>
#include <CmdUnPause.h>
#include <CmdProtoPlay.h>

TurntableState::TurntableState() : 
    outputShift(Pin::OutputShiftSda, Pin::OutputShiftScl),
    inputMux(Pin::InputMuxA, Pin::InputMuxB, Pin::InputMuxC, Pin::InputMuxResult, MUX_POLL_INTERVAL, BUTTON_HOLD_INTERVAL, BUTTON_DEBOUNCE_INTERVAL),
    movementStepper(STEPS_PER_REVOLUTION, Pin::MovementStep1, Pin::MovementStep3, Pin::MovementStep2, Pin::MovementStep4),
    clutchStepper(STEPS_PER_REVOLUTION, Pin::HorizontalClutchStep1, Pin::HorizontalClutchStep3, Pin::HorizontalClutchStep2, Pin::HorizontalClutchStep4)
{
    pinMode(Pin::VerticalPosition, INPUT);
    pinMode(Pin::HorizontalClutchSwitch, INPUT_PULLUP);

    // Lift
    pinMode(Pin::Lift, INPUT_PULLUP);
    lastLiftStatus = digitalRead(Pin::Lift);

    // Home
    pinMode(Pin::HomeMount, INPUT_PULLUP);
    lastHomeStatus = digitalRead(Pin::HomeMount);

    // Turn on the power LED
    outputShift.setValue(StmShiftPin::LedPower, true);

    updateSpeed(selectedSpeed);
    updateSize(selectedSize);

    clutchStepper.setSpeed(CLUTCH_SPEED);
}

void TurntableState::monitor() {
    updateClockMicros();
    advanceCounts();
    // todo: monitor serial input
    monitorCommandInput();
    executeCommand();

    // Output statuses
    blinkCustomSpeedIndicator();
    outputShift.monitor();
}

void TurntableState::executeCommand() {
    if(currentCommand != nullptr) {
        CommandResult result = currentCommand->execute();

        if(result > CommandResult::Running) {
            currentCommand = nullptr;
            // todo : initialize error command
        } else if(result == CommandResult::Success) {
            currentCommand = nullptr;
        }
    }
}

void TurntableState::rotateSpeed() {
    switch(selectedSpeed) {
        case TurntableSpeed::Rpm33:
            updateSpeed(TurntableSpeed::Rpm45);
            break;
        case TurntableSpeed::Rpm45:
            updateSpeed(TurntableSpeed::Rpm78);
            break;
        case TurntableSpeed::Rpm78:
            updateSpeed(TurntableSpeed::RpmAuto);
            break;
        case TurntableSpeed::RpmAuto:
            updateSpeed(TurntableSpeed::Rpm33);
            break;
        case TurntableSpeed::RpmCustom:
            updateSpeed(TurntableSpeed::RpmAuto);
            break;
    }
}

LiftStatus TurntableState::getLiftStatus() {
    bool status = digitalRead(Pin::Lift);

    if(status == lastLiftStatus) {
        liftDebounce = clockMicros;
        return (LiftStatus) status;
    } else if(clockMicros - liftDebounce > LIFT_DEBOUNCE_MICROS) {
        lastLiftStatus = status;
        return (LiftStatus) status;
    } else {
        return (LiftStatus) !status;
    }
}

HomeStatus TurntableState::getHomeStatus() {
    bool status = digitalRead(Pin::HomeMount);

    if(status == lastHomeStatus) {
        homeDebounce = clockMicros;
        return (HomeStatus) status;
    } else if(clockMicros - homeDebounce > HOME_DEBOUNCE_MICROS) {
        lastHomeStatus = status;
        return (HomeStatus) status;
    } else {
        return (HomeStatus) !status;
    }
}

bool TurntableState::isPaused() {
    return outputShift.getValue(StmShiftPin::LedPauseStatus);
}

void TurntableState::updateSpeed(TurntableSpeed newSpeed) {
    // First set all speed LEDs to off
    outputShift.setValue(StmShiftPin::Led78Rpm, false);
    outputShift.setValue(StmShiftPin::Led45Rpm, false);
    outputShift.setValue(StmShiftPin::Led33Rpm, false);
    outputShift.setValue(StmShiftPin::LedAutoSpeed, false);

    switch(newSpeed) {
    case TurntableSpeed::Rpm33:
        targetSpeed = 33.3333;
        outputShift.setValue(StmShiftPin::Led33Rpm, true);
        break;
    case TurntableSpeed::Rpm45:
        targetSpeed = 45.0;
        outputShift.setValue(StmShiftPin::Led45Rpm, true);
        break;
    case TurntableSpeed::Rpm78:
        targetSpeed = 78.0;
        outputShift.setValue(StmShiftPin::Led78Rpm, true);
        break;
    case TurntableSpeed::RpmAuto:
        // TODO: If playing a record, go based on the last-played size

        outputShift.setValue(StmShiftPin::LedAutoSpeed, true);
        break;
    case TurntableSpeed::RpmCustom:
        /* do nothing */
        break;
    }

    selectedSpeed = newSpeed;
    customSpeedIndicatorCounter = clockMicros;
}

void TurntableState::rotateSize() {
    switch(selectedSize) {
    case RecordSize::In7:
        updateSize(RecordSize::In10);
        break;
    case RecordSize::In10:
        updateSize(RecordSize::In12);
        break;
    case RecordSize::In12:
        updateSize(RecordSize::InAuto);
        break;
    case RecordSize::InAuto:
        updateSize(RecordSize::In7);
        break;
    }
}

void TurntableState::updateSize(RecordSize newSize) {
    outputShift.setValue(StmShiftPin::Led7In, false);
    outputShift.setValue(StmShiftPin::Led10In, false);
    outputShift.setValue(StmShiftPin::Led12In, false);
    outputShift.setValue(StmShiftPin::LedAutoSize, false);

    switch(newSize) {
    case RecordSize::In7:
        outputShift.setValue(StmShiftPin::Led7In, true);
        break;
    case RecordSize::In10:
        outputShift.setValue(StmShiftPin::Led10In, true);
        break;
    case RecordSize::In12:
        outputShift.setValue(StmShiftPin::Led12In, true);
        break;
    case RecordSize::InAuto:
        outputShift.setValue(StmShiftPin::LedAutoSize, true);
        break;
    }

    selectedSize = newSize;
}

void TurntableState::monitorCommandInput() {
    inputMux.monitor(clockMicros);

    // Only read action command values if there is currently no action running
    if(currentCommand == nullptr) {
        if(inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
            if(isPaused() || getLiftStatus() == LiftStatus::Lifted) {
                currentCommand = std::make_unique<CmdUnPause>(this);
            } else {
                currentCommand = std::make_unique<CmdPause>(this);
            }
        } 
        
        else if(inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease) {
            if(getHomeStatus() == HomeStatus::Homed) {
                currentCommand = std::make_unique<CmdProtoPlay>(this, 140);
            } else {
                currentCommand = std::make_unique<CmdProtoPlay>(this, -350);
            }
        }

        else if(inputMux.getValue(MuxPin::BtnCalibration) == ButtonResult::OnRelease) {
            // todo
        }

        else if(inputMux.getValue(MuxPin::BtnTestMode) == ButtonResult::OnRelease) {
            // todo
        }
    }

    // Settings buttons 
    if(inputMux.getValue(MuxPin::BtnSizeSelect) == ButtonResult::OnRelease) {
        rotateSize();
    }

    else if(inputMux.getValue(MuxPin::BtnSpeedSelect) == ButtonResult::OnRelease) {
        rotateSpeed();
    }
}

void TurntableState::advanceCounts() {
    if(clockMicros - countCounter > ONE_SECOND_MICROS) {
        countCounter = clockMicros;
        upTimeSeconds += 1;
    }
}

void TurntableState::updateClockMicros() {
    clockMicros = micros();
}

void TurntableState::blinkCustomSpeedIndicator() {
    if(selectedSpeed == TurntableSpeed::RpmCustom && clockMicros - customSpeedIndicatorCounter > ONE_SECOND_MICROS) {
        customSpeedIndicatorCounter = clockMicros;
        outputShift.setValue(StmShiftPin::LedAutoSpeed, !outputShift.getValue(StmShiftPin::LedAutoSpeed));
    }
}