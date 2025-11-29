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

ActionCommand actionCommand = ActionCommand::NoAction;

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
}

void loop() {
  updateClockMicros();
  monitorCommandInput();
  executeCommand();
}

void monitorCommandInput() {
  inputMux.monitor(clockMicros);

  if(actionCommand == ActionCommand::NoAction && inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
    actionCommand = ActionCommand::PauseUnPause;
  }
}

void executeCommand() {
  switch(actionCommand) {
    case ActionCommand::NoAction:
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