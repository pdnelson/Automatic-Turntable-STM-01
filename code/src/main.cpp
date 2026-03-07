#include <Arduino.h>
#include <Pin.h>
#include <MuxPin.h>
#include <InputMux.h>
#include <Constants.h>
#include <CommandError.h>
#include <ActionCommand.h>
#include <Stepper.h>
#include <LiftStatus.h>
#include <VerticalDirection.h>
#include <MovementAxis.h>
#include <step/PauseStep.h>
#include <step/UnPauseStep.h>
#include <StmShift.h>
#include <StmShiftPin.h>

void monitorCommandInput();
void executeCommand();
void updateClockMicros();
void initPauseUnpauseAction();
void runPauseAction();
void runUnPauseAction();
void endActionCommand();
void endUnPauseAction();
void initErrorAction(CommandError error);
void errorActionCommand();
void endErrorAction();
void checkVerticalStall(VerticalDirection direction, int currentPosition);
LiftStatus getLiftStatus();

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

StmShift outputShift = StmShift();

unsigned long clockMicros = 0;

ActionCommand actionCommand = ActionCommand::NoAction;
uint8_t actionStep = 0;
long actionCounter = 0;
long actionVariable1 = 0;
long actionVariable2 = 0;

int verticalStallPosition = 0;
int verticalStallCounter = 0;

unsigned long liftDebounce = 0;
uint8_t lastLiftStatus = LiftStatus::Lifted;

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

  // Set initial values
  lastLiftStatus = digitalRead(Pin::Lift);

  outputShift.initialize();
  outputShift.setValue(StmShiftPin::LedPower, true);

  Serial.begin(SERIAL_SPEED);
}

void loop() {
  updateClockMicros();
  monitorCommandInput();
  executeCommand();
  outputShift.monitor();
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
    case ActionCommand::Error: 
      errorActionCommand();
      break;
  }
}

void initPauseUnpauseAction() {
  digitalWrite(Pin::MovementSelect, MovementAxis::Vertical);
  verticalStallPosition = analogRead(Pin::VerticalPosition);

  // If the tonearm is not making contact with the lift (implying it's on a record or home), OR the tonearm is at the lower limit (below a record), then "pause" (or lift it up)
  // TODO: Pull this value from home calibration. TEST_VERTICAL_LOWER_LIMIT is only for testing.
  if(getLiftStatus() == LiftStatus::SetDown || !(verticalStallPosition + VERTICAL_ENCODER_DELTA >= TEST_VERTICAL_UPPER_LIMIT)) {
    movementStepper.setSpeed(10);
    actionCommand = ActionCommand::Pause;
    outputShift.setValue(StmShiftPin::LedPauseStatus, true);
    outputShift.setValue(StmShiftPin::AudioCutOff, true);
  } else {
    // TODO: Go fast when over home, go slow when not over home
    movementStepper.setSpeed(3);
    actionCommand = ActionCommand::UnPause;
  }
}

void runPauseAction() {
  movementStepper.step(VerticalDirection::Up);
  int currentPosition = analogRead(Pin::VerticalPosition);

  switch(actionStep) {
    case PauseStep::LiftToCalibratedPosition:

      // Keep stepping until we are at position TEST_VERTICAL_UPPER_LIMIT. 
      // TODO: Pull this value from home calibration. TEST_VERTICAL_UPPER_LIMIT is only for testing.
      if(currentPosition >= TEST_VERTICAL_UPPER_LIMIT) {

        // If the tonearm is lifted at the end of the routine, then the command succeeded.
        if(getLiftStatus() == LiftStatus::Lifted) {
          endActionCommand();
        } 
        
        // Otherwise, the command failed.
        else {
          initErrorAction(CommandError::NotLifted);
        }
      }
      break;
  }

  checkVerticalStall(VerticalDirection::Up, currentPosition);
}

void runUnPauseAction() {
  movementStepper.step(VerticalDirection::Down);
  int currentPosition = analogRead(Pin::VerticalPosition);

  switch(actionStep) {
    case UnPauseStep::LowerUntilToneArmReleased: {

      // Once the tonearm is set down, initiate the next step
      if(getLiftStatus() == LiftStatus::SetDown) {
        actionVariable1 = currentPosition;
        actionStep = UnPauseStep::LowerBelowRecord;
      } 
      
      // If the tonearm reaches the bottom limit, then end the routine.
      // TODO: Pull this value from home calibration. TEST_VERTICAL_LOWER_LIMIT is only for testing.
      else if(currentPosition <= TEST_VERTICAL_LOWER_LIMIT) {
        endUnPauseAction();
      }

      break;
    }

    /**
     * Action variable 1: The encoder starting position when this step begins.
     */
    case UnPauseStep::LowerBelowRecord: {

      // TODO: Pull this value from home calibration. TEST_VERTICAL_LOWER_LIMIT is only for testing.
      if(actionVariable1 - currentPosition >= TICKS_BELOW_RECORD || currentPosition <= TEST_VERTICAL_LOWER_LIMIT) {
        endUnPauseAction();
      }

      break;
    }
  }
  
  checkVerticalStall(VerticalDirection::Down, currentPosition);
}

void endUnPauseAction() {
  outputShift.setValue(StmShiftPin::LedPauseStatus, false);
  outputShift.setValue(StmShiftPin::AudioCutOff, false);
  endActionCommand();
}

/**
 * This executes once per loop cycle. Every time a step is completed, this will increment the verticalStallCounter one time.
 * When an encoder tick occurs, the verticalStallCounter is reset. If the verticalStallCounter increments VERTICAL_STALL_STEPS before
 * an encoder tick occurs, then we can assume the lift motor has stalled.
 */
void checkVerticalStall(VerticalDirection direction, int currentPosition) {
  verticalStallCounter++;

  int greaterThan = direction == VerticalDirection::Down ? verticalStallPosition : currentPosition;
  int lessThan = direction == VerticalDirection::Down ? currentPosition : verticalStallPosition;

  if(greaterThan > lessThan) {
    verticalStallPosition = currentPosition;
    verticalStallCounter = 0;
  } else if(verticalStallCounter >= VERTICAL_STALL_STEPS) {
    initErrorAction(direction == VerticalDirection::Down ? CommandError::LiftStalledMovingDown : CommandError::LiftStalledMovingUp);
  }
}

void initErrorAction(CommandError error) {
  endActionCommand();
  actionVariable1 = error;
  actionVariable2 = clockMicros;
  actionCommand = ActionCommand::Error;

  // Action lights get turned off
  outputShift.setValue(StmShiftPin::LedPauseStatus, false);
  outputShift.setValue(StmShiftPin::LedPlayStatus, false);

  // Size and speed lights become a binary code, representing the current error
  outputShift.setValue(StmShiftPin::LedAutoSize, error & 1);
  outputShift.setValue(StmShiftPin::Led78Rpm, error >> 1 & 1);
  outputShift.setValue(StmShiftPin::Led45Rpm, error >> 2 & 1);
  outputShift.setValue(StmShiftPin::Led33Rpm, error >> 3 & 1);
  outputShift.setValue(StmShiftPin::LedAutoSpeed, error >> 4 & 1);
  outputShift.setValue(StmShiftPin::Led12In, error >> 5 & 1);
  outputShift.setValue(StmShiftPin::Led10In, error >> 6 & 1);
  outputShift.setValue(StmShiftPin::Led7In, error >> 7 & 1);
}

/**
 * This routine blinks the power LED until the user presses the "Pause" or "Play" commands.
 * 
 * actionVariable1: The error that occurred
 * actionVariable2: The time the error occurred
 */
void errorActionCommand() {
  if(clockMicros - actionCounter > ONE_SECOND_MICROS) {
    actionCounter = clockMicros;
    outputShift.setValue(StmShiftPin::LedPower, !outputShift.getValue(StmShiftPin::LedPower));
  }

  if(clockMicros - actionVariable2 > ONE_SECOND_MICROS && (inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease || inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease)) {
    endErrorAction();
  }
}

void endErrorAction() {
  outputShift.setValue(StmShiftPin::LedPower, true);

  // TODO: Return size/speed LEDs to what they were at before
  outputShift.setValue(StmShiftPin::LedAutoSize, false);
  outputShift.setValue(StmShiftPin::Led78Rpm, false);
  outputShift.setValue(StmShiftPin::Led45Rpm, false);
  outputShift.setValue(StmShiftPin::Led33Rpm, false);
  outputShift.setValue(StmShiftPin::LedAutoSpeed, false);
  outputShift.setValue(StmShiftPin::Led12In, false);
  outputShift.setValue(StmShiftPin::Led10In, false);
  outputShift.setValue(StmShiftPin::Led7In, false);

  endActionCommand();
}

void endActionCommand() {
  actionCommand = ActionCommand::NoAction;
  actionCounter = 0;
  actionStep = 0;
  actionVariable1 = 0;
  actionVariable2 = 0;
  verticalStallCounter = 0;
  verticalStallPosition = 0;
  movementStepper.releaseMotorCurrent();
}

LiftStatus getLiftStatus() {
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

void updateClockMicros() {
  clockMicros = micros();
}