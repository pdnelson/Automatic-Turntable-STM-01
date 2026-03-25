#include <Arduino.h>
#include <Pin.h>
#include <MuxPin.h>
#include <InputMux.h>
#include <Constants.h>
#include <CommandError.h>
#include <ActionCommand.h>
#include <Stepper.h>
#include <LiftStatus.h>
#include <HomeStatus.h>
#include <VerticalDirection.h>
#include <MovementAxis.h>
#include <step/PauseStep.h>
#include <step/UnPauseStep.h>
#include <StmShift.h>
#include <StmShiftPin.h>
#include <ExternalCommand.h>
#include <TurntableSpeed.h>
#include <RecordSize.h>

void monitorSerialInputs();
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
void readSerial(Stream& stream);
void advanceCounts();
LiftStatus getLiftStatus();
HomeStatus getHomeStatus();
void rotateSpeed();
void updateSpeed(TurntableSpeed newSpeed);
void blinkCustomSpeedIndicator();
void rotateSize();
void updateSize(RecordSize newSize);

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
long actionVariable3 = 0;

int verticalStallPosition = 0;
int verticalStallCounter = 0;

unsigned long liftDebounce = 0;
uint8_t lastLiftStatus = LiftStatus::Lifted;

unsigned long homeDebounce = 0;
uint8_t lastHomeStatus = HomeStatus::Homed;

// Count variables
unsigned long countCounter = 0;
unsigned long upTimeSeconds = 0;

// Speed variables
TurntableSpeed selectedSpeed = TurntableSpeed::RpmAuto;
float targetSpeed = -1;
unsigned long customSpeedIndicatorCounter = 0;

// Size variables
RecordSize selectedSize = RecordSize::InAuto;

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
  pinMode(Pin::HomeMount, INPUT_PULLUP);

  // Set initial values
  lastLiftStatus = digitalRead(Pin::Lift);
  lastHomeStatus = digitalRead(Pin::HomeMount);

  outputShift.initialize();
  outputShift.setValue(StmShiftPin::LedPower, true);
  updateSpeed(selectedSpeed);
  updateSize(selectedSize);

  STM_SERIAL_USB.begin(SERIAL_SPEED);
  STM_SERIAL_1.begin(SERIAL_SPEED);
  STM_SERIAL_2.begin(SERIAL_SPEED);
  STM_SERIAL_3.begin(SERIAL_SPEED);
  STM_SERIAL_4.begin(SERIAL_SPEED);
  STM_SERIAL_5.begin(SERIAL_SPEED);
}

void loop() {
  updateClockMicros();
  advanceCounts();
  monitorSerialInputs();
  monitorCommandInput();
  executeCommand();

  // Output statuses
  blinkCustomSpeedIndicator();
  outputShift.monitor();
}

void monitorSerialInputs() {
  readSerial(STM_SERIAL_5);
  readSerial(STM_SERIAL_4);
  readSerial(STM_SERIAL_3);
  readSerial(STM_SERIAL_2);
  readSerial(STM_SERIAL_1);
  readSerial(STM_SERIAL_USB);
}

void readSerial(Stream& stream) {
  if(stream.available() > 0) {
    uint8_t key = stream.read();

    // If the key is the init key, send back model and version info
    if(key == SERIAL_COMMAND_INIT_KEY) {
      byte data[4];
      data[0] = SERIAL_COMMAND_MODEL_KEY;
      data[1] = VERSION_MAJOR;
      data[2] = VERSION_MINOR;
      data[3] = VERSION_PATCH;

      stream.write(data, 4);
    }

    // If the key is for this model turntable, execute the command
    else if(key == SERIAL_COMMAND_MODEL_KEY) {
      switch(stream.read()) {
        case ExternalCommand::ConnectionTest: 
          stream.write(SERIAL_COMMAND_CONNECTION_SUCCESS);
          break;
        case ExternalCommand::ActionPauseUnPause: {
          initPauseUnpauseAction();
          break;
        }
        case ExternalCommand::SetSpeed: {
          updateSpeed((TurntableSpeed)stream.read());
          break;
        }
        case ExternalCommand::SetRotateSpeed: {
          rotateSpeed();
          break;
        }
        case ExternalCommand::SetCustomSpeed: {
          uint32_t data1 = stream.read() & 0x000000FF;
          uint32_t data2 = stream.read() << 8 & 0x0000FF00;
          uint32_t data3 = stream.read() << 16 & 0x00FF0000;
          uint32_t data4 = stream.read() << 24 & 0xFF000000;

          uint32_t dataCombined = data1 | data2 | data3 | data4;
          float* finalSpeed = reinterpret_cast<float*>((void*) &dataCombined);

          targetSpeed = *finalSpeed;

          if(targetSpeed < 33.33333 && targetSpeed > 33.3) {
            updateSpeed(TurntableSpeed::Rpm33);
          } else if(targetSpeed == 45.0) {
            updateSpeed(TurntableSpeed::Rpm45);
          } else if(targetSpeed == 78.0) {
            updateSpeed(TurntableSpeed::Rpm78);
          } else {
            updateSpeed(TurntableSpeed::RpmCustom);
          }
          break;
        }
        case ExternalCommand::SetSize: {
          updateSize((RecordSize)stream.read());
          break;
        }
        case ExternalCommand::SetRotateSize: {
          rotateSize();
          break;
        }
        case ExternalCommand::SetClearActionCommand: {
          switch(actionCommand) {
            case ActionCommand::Error:
              endErrorAction();
              break;
            case ActionCommand::UnPause:
              endUnPauseAction();
            default:
              endActionCommand();
          }
          break;
        }
        case ExternalCommand::GetVerticalEncoderPos: {
          uint16_t verticalPosition = analogRead(Pin::VerticalPosition);

          byte data[2];
          data[0] = verticalPosition & 0x00FF;
          data[1] = (verticalPosition >> 8) & 0x00FF;

          stream.write(data, 2);
          break;
        }
        case ExternalCommand::GetLiftStatus: {
          stream.write(getLiftStatus());
          break;
        }
        case ExternalCommand::GetHomeStatus: {
          stream.write(getHomeStatus());
          break;
        }
        case ExternalCommand::GetCurrentCommand: {
          stream.write(actionCommand);
          break;
        }
        case ExternalCommand::GetErrorCode: {
          if(actionCommand == ActionCommand::Error) {
            stream.write(actionVariable1); // actionVariable1 holds the error code
          } else {
            stream.write((uint8_t)0); // 0 represents no error
          }
          break;
        }
        case ExternalCommand::GetUpTime: {
          byte data[4];
          data[0] = upTimeSeconds & 0x000000FF;
          data[1] = (upTimeSeconds >> 8) & 0x000000FF;
          data[2] = (upTimeSeconds >> 16) & 0x000000FF;
          data[3] = (upTimeSeconds >> 24) & 0x000000FF;

          stream.write(data, 4);
          break;
        }
        case ExternalCommand::GetSpeedSetting: {
          stream.write(selectedSpeed);
          break;
        }
        case ExternalCommand::GetSpeedTarget: {
          float speed = -1;
          if(getHomeStatus() == HomeStatus::NotHomed) {
            speed = targetSpeed;
          }
          
          byte const * data = reinterpret_cast<byte const *>(&speed);
          stream.write(data, sizeof(float));
          break;
        }
        case ExternalCommand::GetSizeSetting: {
          stream.write(selectedSize);
          break;
        }
      }
  
    }
  }
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
    rotateSize();
  }

  else if(inputMux.getValue(MuxPin::BtnSpeedSelect) == ButtonResult::OnRelease) {
    rotateSpeed();
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
  actionVariable3 = outputShift.getValues();
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
  outputShift.setValues(actionVariable3);
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

HomeStatus getHomeStatus() {
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

void rotateSpeed() {
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

void updateSpeed(TurntableSpeed newSpeed) {
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

void rotateSize() {
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

void updateSize(RecordSize newSize) {
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

void blinkCustomSpeedIndicator() {
  if(selectedSpeed == TurntableSpeed::RpmCustom && clockMicros - customSpeedIndicatorCounter > ONE_SECOND_MICROS) {
    customSpeedIndicatorCounter = clockMicros;
    outputShift.setValue(StmShiftPin::LedAutoSpeed, !outputShift.getValue(StmShiftPin::LedAutoSpeed));
  }
}

void advanceCounts() {
  if(clockMicros - countCounter > ONE_SECOND_MICROS) {
    countCounter = clockMicros;
    upTimeSeconds += 1;
  }
}

void updateClockMicros() {
  clockMicros = micros();
}