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
  MUX_POLL_INTERVAL,
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
      // todo: implement
    }

    else if(inputMux.getValue(MuxPin::BtnCalibration) == ButtonResult::OnRelease) {
      actionCommand = ActionCommand::Calibration;
    }

    else if(inputMux.getValue(MuxPin::BtnTestMode) == ButtonResult::OnRelease) {
      actionCommand = ActionCommand::TestMode;
    }

    // Though reset settings is a settings button, we only want to do this if a command
    // isn't running, because the command could depend on one of those settings.
    else if(inputMux.getValue(MuxPin::BtnResetSettings) == ButtonResult::OnHold) {
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
    case ActionCommand::Play:
      endActionCommand(); // TODO: Implement.
      break;
    case ActionCommand::Home:
      endActionCommand(); // TODO: Implement.
      break;
    case ActionCommand::Calibration:
      endActionCommand(); // TODO: Implement.
      break;
    case ActionCommand::TestMode:
      endActionCommand(); // TODO: Implement.
      break;
  }
}

void initPauseUnpauseAction() {
  digitalWrite(Pin::MovementSelect, MovementAxis::Vertical);
  int verticalPosition = analogRead(Pin::VerticalPosition);

  // If the tonearm is not making contact with the lift (implying it's on a record or home), OR the tonearm is at the lower limit (below a record), then "pause" (or lift it up)
  // TODO: Pull this value from home calibration. TEST_VERTICAL_LOWER_LIMIT is only for testing.
  if(digitalRead(Pin::Lift) == LiftStatus::SetDown || verticalPosition - ENCODER_DELTA <= TEST_VERTICAL_LOWER_LIMIT) {
    movementStepper.setSpeed(12);
    actionCommand = ActionCommand::Pause;
  } else {
    // TODO: Go fast when over home, go slow when not over home
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

      // Keep stepping until we are at position TEST_VERTICAL_UPPER_LIMIT. 
      // TODO: Pull this value from home calibration. TEST_VERTICAL_UPPER_LIMIT is only for testing.
      if(analogRead(Pin::VerticalPosition) >= TEST_VERTICAL_UPPER_LIMIT) {
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

  switch(actionStep) {
    case UnPauseStep::LowerUntilToneArmReleased:
      movementStepper.step(VerticalDirection::Down);

      // Once the tonearm is set down, initiate the next step
      if(digitalRead(Pin::Lift) == LiftStatus::SetDown) {
        actionVariable = analogRead(Pin::VerticalPosition);
        actionStep = UnPauseStep::LowerBelowRecord;
      } 
      
      // If the tonearm reaches the bottom limit, then end the routine.
      // TODO: Pull this value from home calibration. TEST_VERTICAL_LOWER_LIMIT is only for testing.
      else if(analogRead(Pin::VerticalPosition) <= TEST_VERTICAL_LOWER_LIMIT) {
        endActionCommand();
      }
      break;
    case UnPauseStep::LowerBelowRecord:
      movementStepper.step(VerticalDirection::Down);

      int verticalPosition = analogRead(Pin::VerticalPosition);

      // TODO: Pull this value from home calibration. TEST_VERTICAL_LOWER_LIMIT is only for testing.
      if(actionVariable - verticalPosition >= TICKS_BELOW_RECORD || verticalPosition <= TEST_VERTICAL_LOWER_LIMIT) {
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