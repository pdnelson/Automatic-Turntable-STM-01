#include <Arduino.h>
#include <Pin.h>
#include <MuxPin.h>
#include <InputMux.h>
#include <Constants.h>
#include <ActionCommand.h>
#include <Stepper.h>
#include <LiftStatus.h>
#include <VerticalDirection.h>
#include <MovementAxis.h>
#include <step/PauseStep.h>
#include <step/UnPauseStep.h>

void monitorCommandInput();
void executeCommand();
void updateClockMicros();
void initPauseUnpauseAction();
void runPauseAction();
void runUnPauseAction();
void endActionCommand();
LiftStatus readLiftStatus();

Stepper movementStepper = Stepper(
  STEPS_PER_REVOLUTION,
  Pin::MovementStep1,
  Pin::MovementStep3,
  Pin::MovementStep2,
  Pin::MovementStep4
);

InputMux inputMux = InputMux(
  Pin::InputMuxA, 
  Pin::InputMuxB, 
  Pin::InputMuxC, 
  Pin::InputMuxResult,
  MUX_PROPAGATION_DELAY,
  BUTTON_HOLD_INTERVAL,
  BUTTON_DEBOUNCE_INTERVAL
);
unsigned long clockMicros = 0;

ActionCommand actionCommand = ActionCommand::NoAction;
uint8_t actionStep = 0;
long actionCounter = 0;
long actionVariable = 0;

void setup() {
  // Input mux
  pinMode(Pin::InputMuxA, OUTPUT);
  pinMode(Pin::InputMuxB, OUTPUT);
  pinMode(Pin::InputMuxC, OUTPUT);
  pinMode(Pin::InputMuxResult, INPUT);

  // Output shift
  pinMode(Pin::OutputShiftSda, OUTPUT);
  pinMode(Pin::OutputShiftScl, OUTPUT);

  // Movement steppers
  pinMode(Pin::MovementStep1, OUTPUT);
  pinMode(Pin::MovementStep2, OUTPUT);
  pinMode(Pin::MovementStep3, OUTPUT);
  pinMode(Pin::MovementStep4, OUTPUT);
  pinMode(Pin::MovementSelect, OUTPUT);

  // Clutch stepper
  pinMode(Pin::HorizontalClutchStep1, OUTPUT);
  pinMode(Pin::HorizontalClutchStep2, OUTPUT);
  pinMode(Pin::HorizontalClutchStep3, OUTPUT);
  pinMode(Pin::HorizontalClutchStep4, OUTPUT);

  // Encoders
  pinMode(Pin::SpeedEncoderA, INPUT);
  pinMode(Pin::SpeedEncoderB, INPUT);
  pinMode(Pin::HorizontalPositionEncoderA, INPUT);
  pinMode(Pin::HorizontalPositionEncoderB, INPUT);

  // Motor Phases
  pinMode(Pin::TurntableMotorPhase1, OUTPUT);
  pinMode(Pin::TurntableMotorPhase2, OUTPUT);
  pinMode(Pin::TurntableMotorPhase3, OUTPUT);

  // Positioning Potentiometers
  pinMode(Pin::HorizontalClutchPosition, INPUT);
  pinMode(Pin::VerticalPosition, INPUT);

  // Various Sensors
  pinMode(Pin::HeadshellPhotoDiode, INPUT);

  // Various Status
  pinMode(Pin::PowerOnStatusIn, INPUT);
  pinMode(Pin::Lift, INPUT_PULLUP);
  pinMode(Pin::HomeStatus, INPUT_PULLUP);
}

void loop() {
  updateClockMicros();
  monitorCommandInput();
  executeCommand();
}

void monitorCommandInput() {
  inputMux.monitor(clockMicros);

  // Only read action command values if there is currently no action running
  if(actionCommand == ActionCommand::NoAction) {

    if(inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
      initPauseUnpauseAction();
    } 
    
    else if(inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease) {
      actionCommand = ActionCommand::PlayHome;
    }

    else if(inputMux.getValue(MuxPin::BtnCalibration) == ButtonResult::OnRelease) {
      actionCommand = ActionCommand::Calibration;
    }

    else if(inputMux.getValue(MuxPin::BtnTestMode) == ButtonResult::OnRelease) {
      actionCommand = ActionCommand::TestMode;
    }

    // Though reset settings is a settings button, we only want to do this if a command
    // isn't running, because the command could depend on one of those settings.
    else if(inputMux.getValue(MuxPin::BtnResetSettings) == ButtonResult::Held) {
      // todo: reset settings
    }
  }

  // Settings buttons 
  if(inputMux.getValue(MuxPin::BtnSizeSelect) == ButtonResult::OnRelease) {
    // todo: rotate through size buttons
  }

  else if(inputMux.getValue(MuxPin::BtnSpeedSelect) == ButtonResult::OnRelease) {
    // todo: rotate through size buttons
  }
}

void executeCommand() {
  switch(actionCommand) {
    case ActionCommand::NoAction: break;
    case ActionCommand::Pause:
      runPauseAction();
      break;
    case ActionCommand::UnPause:
      runUnPauseAction();
      break;
    case ActionCommand::PlayHome:
      // todo: play/home code
      break;
    case ActionCommand::Calibration:
      // todo: calibration code
      break;
    case ActionCommand::TestMode:
      // todo: test mode code
      break;
  }
}

void initPauseUnpauseAction() {
  digitalWrite(Pin::MovementSelect, MovementAxis::Vertical);

  // TODO: Should not determine pause/unpause using the lift status. Should determine this based
  // on vertical pot calibration value.
  if(readLiftStatus() == LiftStatus::SetDown) {
    movementStepper.setSpeed(12);
    actionCommand = ActionCommand::Pause;
  } else {
    movementStepper.setSpeed(5);
    actionCommand = ActionCommand::UnPause;
  }
}

void runPauseAction() {
  // TODO: Error/stall checking.
  // To check: 
  //   - Correct number of steps per encoder tick
  //   - Encoder is moving the correct direction
  //   - At the end of the routine, lift status should be lifted

  switch(actionStep) {
    case PauseStep::LiftToCalibratedPosition:
      movementStepper.step(VerticalDirection::Up);

      // Keep stepping until we are at position 800
      if(analogRead(Pin::VerticalPosition) >= 800) {
        endActionCommand();
      }
      break;
  }
}

void runUnPauseAction() {
  // TODO: Error/stall checking
  // To check:
  //  - Correct number of steps per encoder tick
  //  - Encoder is moving the correct direction
  //  - Stop if we reach the lower limit determined by calibration (i.e. unpause was initiated while the tonearm is not over a record)

  switch(actionStep) {
    case UnPauseStep::LowerUntilToneArmReleased:
      movementStepper.step(VerticalDirection::Down);

      // Once the tonearm is set down, initiate the next step
      if(readLiftStatus() == LiftStatus::SetDown) {
        actionVariable = analogRead(Pin::VerticalPosition);
        actionStep = UnPauseStep::LowerBelowRecord;
      }
      break;
    case UnPauseStep::LowerBelowRecord:
      movementStepper.step(VerticalDirection::Down);

      if(actionVariable - analogRead(Pin::VerticalPosition) >= TICKS_BELOW_RECORD) {
        endActionCommand();
      }
      break;
  }
}

void endActionCommand() {
  actionCommand = ActionCommand::NoAction;
  actionCounter = 0;
  actionStep = 0;
  actionVariable = 0;
  movementStepper.releaseMotorCurrent();
}

void updateClockMicros() {
  clockMicros = micros();
}

LiftStatus readLiftStatus() {
  return digitalRead(Pin::Lift) ? LiftStatus::SetDown : LiftStatus::Lifted;
}