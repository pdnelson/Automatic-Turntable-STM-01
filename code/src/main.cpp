#include <Arduino.h>
#include <Pin.h>
#include <MuxPin.h>
#include <InputMux.h>
#include <Constants.h>
#include <ActionCommand.h>

void monitorCommandInput();
void executeCommand();
void updateClockMicros();

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

ActionCommand runningCommand = ActionCommand::None;

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
  pinMode(Pin::LiftStatus, INPUT);
  pinMode(Pin::HomeStatus, INPUT);


  // MUX TESTING ONLY! TODO: REMOVE WHEN FINISHED TESTING
  pinMode(33, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(39, OUTPUT);

  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, LOW);
  digitalWrite(36, LOW);
  digitalWrite(37, LOW);
  digitalWrite(38, LOW);
  digitalWrite(39, LOW);
}

void loop() {
  updateClockMicros();
  monitorCommandInput();
  //executeCommand();
}

void monitorCommandInput() {
  inputMux.monitor(clockMicros);

  ButtonResult sizeSelect = inputMux.getValue(MuxPin::BtnSizeSelect);
  ButtonResult speedSelect = inputMux.getValue(MuxPin::BtnSpeedSelect);
  ButtonResult play = inputMux.getValue(MuxPin::BtnPlay);
  ButtonResult pause = inputMux.getValue(MuxPin::BtnPause);
  ButtonResult testMode = inputMux.getValue(MuxPin::BtnTestMode);
  ButtonResult resetSettings = inputMux.getValue(MuxPin::BtnResetSettings);
  ButtonResult calibration = inputMux.getValue(MuxPin::BtnCalibration);

  if(sizeSelect == ButtonResult::Pressed) {
    digitalWrite(33, HIGH);
  } else {
    digitalWrite(33, LOW);
  }

  if(speedSelect == ButtonResult::Pressed) {
    digitalWrite(34, HIGH);
  } else {
    digitalWrite(34, LOW);
  }

  if(play == ButtonResult::Pressed) {
    digitalWrite(35, HIGH);
  } else {
    digitalWrite(35, LOW);
  }

  if(pause == ButtonResult::Pressed) {
    digitalWrite(36, HIGH);
  } else {
    digitalWrite(36, LOW);
  }

  if(testMode == ButtonResult::Pressed) {
    digitalWrite(37, HIGH);
  } else {
    digitalWrite(37, LOW);
  }

  if(resetSettings == ButtonResult::Pressed) {
    digitalWrite(38, HIGH);
  } else {
    digitalWrite(38, LOW);
  }

  if(calibration == ButtonResult::Pressed) {
    digitalWrite(39, HIGH);
  } else {
    digitalWrite(39, LOW);
  }
}

void executeCommand() {
  switch(runningCommand) {
    case ActionCommand::None:
      // Do nothing.
      break;
    case ActionCommand::PauseUnPause:
      // todo: pause code
      break;
  }
}

void updateClockMicros() {
  clockMicros = micros();
}