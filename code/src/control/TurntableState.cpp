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
#include <StmBasicStepper.h>
#include <StmShift.h>
#include <TurntableSpeed.h>
#include <CmdError.h>
#include <CmdPause.h>
#include <CmdUnPause.h>
#include <CmdProtoPlay.h>
#include <StmEncoder.h>
#include <ClutchStatus.h>
#include <StmPolarity.h>
#include <CmdGoToPositionH.h>
#include <CmdCalibration.h>

TurntableState::TurntableState() : 
    outputShift(Pin::ReservedI2CSda, Pin::ReservedI2CScl),
    serialComm(this),
    inputMux(Pin::InputMuxA, Pin::InputMuxB, Pin::InputMuxC, Pin::InputMuxResult, MUX_POLL_INTERVAL, BUTTON_HOLD_INTERVAL, BUTTON_DEBOUNCE_INTERVAL),
    verticalStepper(Pin::MovementStep1, Pin::MovementStep3, Pin::MovementStep2, Pin::MovementStep4),
    clutchStepper(Pin::HorizontalClutchStep1, Pin::HorizontalClutchStep3, Pin::HorizontalClutchStep2, Pin::HorizontalClutchStep4),
    azEncoder(Pin::ReservedI2CSda, Pin::ReservedI2CScl),
    horizontalStepper(azEncoder, Pin::HorizontalEnable, Pin::HorizontalSleep, Pin::HorizontalStep, Pin::HorizontalDirection),
    calibration(),
    settings()
{
    pinMode(Pin::VerticalPosition, INPUT);
    pinMode(Pin::HorizontalClutchSwitch, INPUT_PULLUP);

    // Lift
    pinMode(Pin::Lift, INPUT_PULLUP);
    lastLiftStatus = digitalRead(Pin::Lift);

    // Turn on the power LED
    outputShift.setValue(StmShiftPin::LedPower, true);

    settings.load();

    updateCustomSpeed(settings.speed);
    updateSize(settings.size);

    clutchStepper.setSpeed(CLUTCH_SPEED);

    calibration.load();

    azEncoder.setPolarity(calibration.polarityH);
    azEncoder.zeroOutEncoder(calibration.azEncoderOffset);
    horizontalStepper.setPolarity(calibration.hStepperPolarity);
}

void TurntableState::monitor() {
    updateClockMicros();
    advanceCounts();
    serialComm.monitor();
    monitorCommandInput();
    executeCommand();
    azEncoder.monitor(clockMicros);

    // Output statuses
    blinkCustomSpeedIndicator();
    outputShift.monitor();
}

void TurntableState::executeCommand() {
    if(currentCommand != nullptr) {
        CommandResult result = currentCommand->execute();

        if(result == CommandResult::Success) {

            // When coming out of the calibration command, go home (if we aren't already there)
            if(currentCommand->getCommandId() == CommandId::Calibration && getHomeStatus() != HomeStatus::Homed) {
                currentCommand = std::make_unique<CmdGoToPositionH>(this, calibration.home, 10, 14);
            } else {
                currentCommand = nullptr;
            }
        } else if(result > CommandResult::Success) {
            currentCommand = std::make_unique<CmdError>(this, result);
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
            updateSpeed(TurntableSpeed::Rpm33);
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
    uint16_t difference = abs((int16_t)azEncoder.getNormalizedPosition() - (int16_t)calibration.home);

    if(difference <= HOME_THRESHOLD) {
        return HomeStatus::Homed;
    }

    return HomeStatus::NotHomed;
}

uint16_t TurntableState::getVerticalEncoderPos() {
    if(calibration.polarityV == StmPolarity::Normal) {
        return analogRead(Pin::VerticalPosition);
    } else {
        return V_ENCODER_MAX_VALUE - analogRead(Pin::VerticalPosition);
    }
}

bool TurntableState::isPaused() {
    return getVerticalEncoderPos() >= (calibration.verticalUpperLimit - PAUSE_ERROR);
}

ClutchStatus TurntableState::clutchEngaged() {
    return (ClutchStatus)digitalRead(Pin::HorizontalClutchSwitch);
}

void TurntableState::updateCustomSpeed(float newSpeed) {
    if(newSpeed < 33.33333 && newSpeed > 33.3) {
        updateSpeed(TurntableSpeed::Rpm33);
    } else if(newSpeed == 45.0) {
        updateSpeed(TurntableSpeed::Rpm45);
    } else if(newSpeed == 78.0) {
        updateSpeed(TurntableSpeed::Rpm78);
    } else if(newSpeed == 0.0) {
        updateSpeed(TurntableSpeed::RpmAuto);
    } else {
        settings.speed = newSpeed;
        updateSpeed(TurntableSpeed::RpmCustom);
    }
}

void TurntableState::updateSpeed(TurntableSpeed newSpeed) {
    // First set all speed LEDs to off
    outputShift.setValue(StmShiftPin::Led78Rpm, false);
    outputShift.setValue(StmShiftPin::Led45Rpm, false);
    outputShift.setValue(StmShiftPin::Led33Rpm, false);
    outputShift.setValue(StmShiftPin::LedAutoSpeed, false);

    switch(newSpeed) {
        case TurntableSpeed::Rpm33:
            settings.speed = 33.3333;
            outputShift.setValue(StmShiftPin::Led33Rpm, true);
            break;
        case TurntableSpeed::Rpm45:
            settings.speed = 45.0;
            outputShift.setValue(StmShiftPin::Led45Rpm, true);
            break;
        case TurntableSpeed::Rpm78:
            settings.speed = 78.0;
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
    switch(settings.size) {
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

    settings.size = newSize;
}

void TurntableState::pauseOrUnPause() {
    if(isPaused()) {
        currentCommand = std::make_unique<CmdUnPause>(this);
    } else {
        currentCommand = std::make_unique<CmdPause>(this);
    }
}

void TurntableState::playOrReturn() {
    if(getHomeStatus() == HomeStatus::Homed) {
        uint16_t position = 0;

        switch(settings.size) {
            case RecordSize::In7: position = calibration.in7; break;
            case RecordSize::In10: position = calibration.in10; break;
            case RecordSize::In12: position = calibration.in12; break;
            case RecordSize::InAuto: break;
        }

        if(position != 0) {
            currentCommand = std::make_unique<CmdGoToPositionH>(this, position, 2, 3);
        } else {
            // to do: implement automatic routine
        }
    } else {
        currentCommand = std::make_unique<CmdGoToPositionH>(this, calibration.home, 10, 14);
    }
}

void TurntableState::beginCalibrationRoutine() {
    currentCommand = std::make_unique<CmdCalibration>(this);
}

float TurntableState::getTargetSpeed() {
    if(getHomeStatus() == HomeStatus::NotHomed) {
        return settings.speed;
    } else {
        return 0;
    }
}

void TurntableState::monitorCommandInput() {
    inputMux.monitor(clockMicros);

    // Only read action command values if there is currently no action running
    if(currentCommand == nullptr) {
        if(inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
            pauseOrUnPause();
        } 
        
        else if(inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease) {
            playOrReturn();
        }

        else if(inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnHold && inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnHold) {
            beginCalibrationRoutine();
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

void TurntableState::saveSettings() {
    settings.persist();
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