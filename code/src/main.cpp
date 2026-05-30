#include <memory>
#include <Arduino.h>
#include <Pin.h>
#include <MuxPin.h>
#include <InputMux.h>
#include <ClutchDirection.h>
#include <ClutchStatus.h>
#include <Constants.h>
#include <CommandError.h>
#include <ActionCommand.h>
#include <Stepper.h>
#include <LiftStatus.h>
#include <HomeStatus.h>
#include <VerticalDirection.h>
#include <MovementAxis.h>
#include <step/PauseStep.h>
#include <step/PlayStep.h>
#include <step/UnPauseStep.h>
#include <step/result/LowerTonearmResult.h>
#include <StmShift.h>
#include <StmShiftPin.h>
#include <ExternalCommand.h>
#include <TurntableSpeed.h>
#include <RecordSize.h>
#include <AzimuthDirection.h>
#include <Prototypes.h>

class TurntableState {
  public:
    TurntableState(): outputShift() {
      // Lift
      pinMode(Pin::Lift, INPUT_PULLUP);
      lastLiftStatus = digitalRead(Pin::Lift);

      // Home
      pinMode(Pin::HomeMount, INPUT_PULLUP);
      lastHomeStatus = digitalRead(Pin::HomeMount);

      // Output shift
      pinMode(Pin::OutputShiftSda, OUTPUT);
      pinMode(Pin::OutputShiftScl, OUTPUT);
      outputShift.initialize();
      outputShift.setValue(StmShiftPin::LedPower, true);
    }

    StmShift outputShift;

    void monitor(unsigned long clockMicros) {
      outputShift.monitor();
    }

    LiftStatus getLiftStatus(unsigned long clockMicros) {
      bool status = digitalRead(Pin::Lift);

      if(status == this->lastLiftStatus) {
        this->liftDebounce = clockMicros;
        return (LiftStatus) status;
      } else if(clockMicros - this->liftDebounce > LIFT_DEBOUNCE_MICROS) {
        this->lastLiftStatus = status;
        return (LiftStatus) status;
      } else {
        return (LiftStatus) !status;
      }
    }

    HomeStatus getHomeStatus(unsigned long clockMicros) {
      bool status = digitalRead(Pin::HomeMount);

      if(status == this->lastHomeStatus) {
        this->homeDebounce = clockMicros;
        return (HomeStatus) status;
      } else if(clockMicros - this->homeDebounce > HOME_DEBOUNCE_MICROS) {
        this->lastHomeStatus = status;
        return (HomeStatus) status;
      } else {
        return (HomeStatus) !status;
      }
    }

    bool isPaused() {
      return outputShift.getValue(StmShiftPin::LedPauseStatus);
    }

  private:
    unsigned long liftDebounce = 0;
    uint8_t lastLiftStatus = LiftStatus::Lifted;

    unsigned long homeDebounce = 0;
    uint8_t lastHomeStatus = HomeStatus::Homed;
};

struct CommandResult {
  bool complete;
  CommandError error;
};

class BaseTurntableCommand {
  public:
    BaseTurntableCommand(TurntableState state): state(state) {
      // do nothing!!!
    } 

    virtual ~BaseTurntableCommand() = default;
    virtual ActionCommand getCommandId() = 0;
    CommandResult execute(unsigned long clockMicros) {
      initialize(clockMicros);

      CommandResult result = doExecute(clockMicros);

      if(result.complete || result.error != CommandError::NoError) {
        uninitialize(clockMicros);
      }

      return result;
    }
    TurntableState state;

  private:
    virtual CommandResult doExecute(unsigned long clockMicros) = 0;
    virtual void doInitialize(unsigned long clockMicros) = 0;
    virtual void doUninitialize(unsigned long clockMicros) = 0;
    bool initialized = false;
    bool uninitialized = false;

    void initialize(unsigned long clockMicros) {
      if(!this->initialized) {
        this->doInitialize(clockMicros);
        this->initialized = true;
      }
    }

    void uninitialize(unsigned long clockMicros) {
      if(!this->uninitialized) {
        this->doUninitialize(clockMicros);
        this->uninitialized = true;
      }
    }
};

class BaseTurntableSubCommand {
  public:
    BaseTurntableSubCommand(TurntableState state): state(state) {
      // do nothin'
    }

    TurntableState state;

    CommandResult execute(unsigned long clockMicros) {
      this->initialize(clockMicros);

      if(this->result.error != CommandError::NoError) {
        this->uninitialize(clockMicros);
        return this->result;
      } else if(this->result.complete) {
        this->uninitialize(clockMicros);
        if(this->nextSubCommand == nullptr) {
          return { true, CommandError::NoError };
        } else {
          return this->nextSubCommand->execute(clockMicros);
        }
      } else {
        this->result = doExecute(clockMicros);
        return { false, this->result.error };
      }
    }

    std::unique_ptr<BaseTurntableSubCommand> nextSubCommand;
    
  private:
    CommandResult result = { false, CommandError::NoError };
    virtual CommandResult doExecute(unsigned long clockMicros) = 0;
    virtual void doInitialize(unsigned long clockMicros) = 0;
    virtual void doUninitialize(unsigned long clockMicros) = 0;
    bool initialized = false;
    bool uninitialized = false;

    void initialize(unsigned long clockMicros) {
      if(!this->initialized) {
        this->doInitialize(clockMicros);
        this->initialized = true;
      }
    }

    void uninitialize(unsigned long clockMicros) {
      if(!this->uninitialized) {
        this->doUninitialize(clockMicros);
        this->uninitialized = true;
      }
    }
};

class BaseLiftSubCommand : public BaseTurntableSubCommand {
  public:
    BaseLiftSubCommand(TurntableState state, Stepper movementMotors, uint8_t speed): BaseTurntableSubCommand(state), movementMotors(movementMotors) {
      this->speed = speed;
    }

    bool checkVerticalStall(VerticalDirection direction, int currentPosition) {
      this->verticalStallCounter++;

      // If going down, verticalStallPosition > currentPosition
      // If going up, currentPosition > verticalStallPosition
      int greaterThan = direction == VerticalDirection::Down ? this->verticalStallPosition : currentPosition;
      int lessThan = direction == VerticalDirection::Down ? currentPosition : this->verticalStallPosition;

      // Reset the stall counter when an encoder tick (in the proper direction) occurs
      if(greaterThan > lessThan) {
        this->verticalStallPosition = currentPosition;
        this->verticalStallCounter = 0;
      } 
      
      // We've stalled.
      else if(this->verticalStallCounter >= VERTICAL_STALL_STEPS) {
        return true;
      }

      return false;
    }

    void baseInitialize() {
      digitalWrite(Pin::MovementSelect, MovementAxis::Vertical);
      this->verticalStallPosition = analogRead(Pin::VerticalPosition);
      this->movementMotors.setSpeed(this->speed);
    }

    Stepper movementMotors;
    uint8_t speed = 0;

  private:
    int16_t verticalStallCounter = 0;
    int16_t verticalStallPosition = 0;

};

class SubCmdLiftTonearm : public BaseLiftSubCommand {
  public:
    SubCmdLiftTonearm(TurntableState state, Stepper movementMotors, uint8_t speed) : BaseLiftSubCommand(state, movementMotors, speed) {
      // Do nothing
    }

  private:
    bool reachedLimit = false;
    unsigned long timeLimitReached = 0;

    void doInitialize(unsigned long clockMicros) override {
        baseInitialize();
    }

    CommandResult doExecute(unsigned long clockMicros) override {
      this->movementMotors.step(VerticalDirection::Up);
      int currentPosition = analogRead(Pin::VerticalPosition);

      bool stalled = checkVerticalStall(VerticalDirection::Up, currentPosition);

      bool completed = false;
      CommandError error = CommandError::NoError;

      if(stalled) {
        error = CommandError::LiftStalledMovingUp;
      } else {
        if(!this->reachedLimit && currentPosition >= TEST_VERTICAL_UPPER_LIMIT) {
          this->reachedLimit = true;
          this->timeLimitReached = clockMicros;
        } else if(this->reachedLimit) {
          if(this->state.getLiftStatus(clockMicros) == LiftStatus::Lifted) {
            completed = true;
          } else if(clockMicros - this->timeLimitReached > LIFT_BOUNCE_TIMEOUT_MICROS) {
            error = CommandError::NotLifted;
          }
        }
      }

      return { completed, error }; 
    }

    void doUninitialize(unsigned long clockMicros) override {
      // do nothing
    }
};

class SubCmdSetDownTonearm : public BaseLiftSubCommand {
  public:
    SubCmdSetDownTonearm(TurntableState state, Stepper movementMotors, uint8_t speed) : BaseLiftSubCommand(state, movementMotors, speed) {
      // Do nothing
    }

  private:
    bool isSetDown = false;
    int setDownPosition = 0;

    void doInitialize(unsigned long clockMicros) override {
        baseInitialize();
    }

    CommandResult doExecute(unsigned long clockMicros) override {
      movementMotors.step(VerticalDirection::Down);
      int currentPosition = analogRead(Pin::VerticalPosition);

      bool stalled = checkVerticalStall(VerticalDirection::Down, currentPosition);

      bool completed = false;
      CommandError error = CommandError::NoError;

      if(stalled) {
        error = CommandError::LiftStalledMovingUp;
      } else if(!this->isSetDown) {
        if(currentPosition <= TEST_VERTICAL_LOWER_LIMIT) {
          completed = true;
        } else if(!this->isSetDown && this->state.getLiftStatus(clockMicros) == LiftStatus::SetDown) {
          this->isSetDown = true;
          this->setDownPosition = currentPosition;
        } else if(this->isSetDown && setDownPosition - currentPosition >= TICKS_BELOW_RECORD) {
          completed = true;
        }
      }

      return { completed, error };
    }

    void doUninitialize(unsigned long clockMicros) override {
      // do nothing
    }
};

class CmdPause : public BaseTurntableCommand {
  public:
    CmdPause(TurntableState state, Stepper movementMotors) : BaseTurntableCommand(state), liftTonearm(state, movementMotors, 10) {
      // Do nothing
    }

    ActionCommand getCommandId() override {
      return ActionCommand::Pause;
    }

    void doInitialize(unsigned long clockMicros) override {
      state.outputShift.setValue(StmShiftPin::LedPauseStatus, true);
      state.outputShift.setValue(StmShiftPin::AudioCutOff, true);
    }

    CommandResult doExecute(unsigned long clockMicros) override {
      return liftTonearm.execute(clockMicros);
    }

    void doUninitialize(unsigned long clockMicros) override {
      // do nothing
    }

  private:
    SubCmdLiftTonearm liftTonearm;
};

class CmdUnPause : public BaseTurntableCommand {
  public:
    CmdUnPause(TurntableState state, Stepper movementMotors) : BaseTurntableCommand(state), setDownTonearm(state, movementMotors, 3) {
      // Do nothing
    }

    ActionCommand getCommandId() override {
      return ActionCommand::UnPause;
    }

    void doInitialize(unsigned long clockMicros) override {
      // do nothing
    }

    CommandResult doExecute(unsigned long clockMicros) override {
      return setDownTonearm.execute(clockMicros);
    }

    void doUninitialize(unsigned long clockMicros) override {
      state.outputShift.setValue(StmShiftPin::LedPauseStatus, false);
      state.outputShift.setValue(StmShiftPin::AudioCutOff, false);
    }

  private:
    SubCmdSetDownTonearm setDownTonearm;
};

Stepper movementStepper = Stepper(
  STEPS_PER_REVOLUTION,
  Pin::MovementStep1,
  Pin::MovementStep3,
  Pin::MovementStep2,
  Pin::MovementStep4
);

Stepper clutchStepper = Stepper(
  STEPS_PER_REVOLUTION,
  Pin::HorizontalClutchStep1,
  Pin::HorizontalClutchStep3,
  Pin::HorizontalClutchStep2,
  Pin::HorizontalClutchStep4
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

// Count variables
unsigned long countCounter = 0;
unsigned long upTimeSeconds = 0;

// Speed variables
TurntableSpeed selectedSpeed = TurntableSpeed::RpmAuto;
float targetSpeed = -1;
unsigned long customSpeedIndicatorCounter = 0;

// Size variables
RecordSize selectedSize = RecordSize::InAuto;

TurntableState state;

std::unique_ptr<BaseTurntableCommand> currentCommand;

void setup() {
  // Input mux
  pinMode(Pin::InputMuxA, OUTPUT);
  pinMode(Pin::InputMuxB, OUTPUT);
  pinMode(Pin::InputMuxC, OUTPUT);
  pinMode(Pin::InputMuxResult, INPUT);

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
  pinMode(Pin::HorizontalClutchSwitch, INPUT_PULLUP);

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
  pinMode(Pin::VerticalPosition, INPUT);

  // Various Sensors
  pinMode(Pin::HeadshellPhotoDiode, INPUT);

  // Various Status
  pinMode(Pin::PowerOnStatusIn, INPUT);

  state = TurntableState();

  updateSpeed(selectedSpeed);
  updateSize(selectedSize);

  clutchStepper.setSpeed(CLUTCH_SPEED);

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
  state.monitor(clockMicros);
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
      ExternalCommand incomingCommand = (ExternalCommand)stream.read();

      // If a command is already running, and an action command comes through, then throw it out.
      if(currentCommand != nullptr && incomingCommand < 31 && incomingCommand > 0) {
        return;
      }

      switch(incomingCommand) {
        case ExternalCommand::ConnectionTest: 
          stream.write(SERIAL_COMMAND_CONNECTION_SUCCESS);
          break;
        case ExternalCommand::ActionPauseUnPause: {
          currentCommand = std::make_unique<CmdPause>(state, movementStepper);
          break;
        }
        case ExternalCommand::ActionProtoPlay: {
          //int16_t data1 = stream.read() & 0x00FF;
          //int16_t data2 = stream.read() << 8 & 0xFF00;
//
          //int16_t stepCount = data1 | data2;

          // todo: init the play action
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
          currentCommand = nullptr;
          break;
        }
        case ExternalCommand::GetHorizontalEncoderPos: {
          // todo: implement
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
          stream.write(state.getLiftStatus(clockMicros));
          break;
        }
        case ExternalCommand::GetHomeStatus: {
          stream.write(state.getHomeStatus(clockMicros));
          break;
        }
        case ExternalCommand::GetCurrentCommand: {
          ActionCommand current = ActionCommand::NoAction;
          if(currentCommand != nullptr) {
            currentCommand->getCommandId();
          }
          
          stream.write(current);
          break;
        }
        case ExternalCommand::GetErrorCode: {
          if(currentCommand != nullptr && currentCommand->getCommandId() == ActionCommand::Error) {
            //stream.write(actionVariable1); // actionVariable1 holds the error code
            // todo: write error code here
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
          if(state.getHomeStatus(clockMicros) == HomeStatus::NotHomed) {
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
  if(currentCommand == nullptr) {

    if(inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
      if(state.isPaused() || state.getLiftStatus(clockMicros) == LiftStatus::Lifted) {
        currentCommand = std::make_unique<CmdUnPause>(state, movementStepper);
      } else {
        currentCommand = std::make_unique<CmdPause>(state, movementStepper);
      }
    } 
    
    else if(inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease) {
      // todo
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

void executeCommand() {
  if(currentCommand != NULL) {
    CommandResult result = currentCommand->execute(clockMicros);

    if(result.error != CommandError::NoError) {
      currentCommand = nullptr;
      // todo : initialize error command
    } else if(result.complete) {
      currentCommand = nullptr;
    }
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
  state.outputShift.setValue(StmShiftPin::Led78Rpm, false);
  state.outputShift.setValue(StmShiftPin::Led45Rpm, false);
  state.outputShift.setValue(StmShiftPin::Led33Rpm, false);
  state.outputShift.setValue(StmShiftPin::LedAutoSpeed, false);

  switch(newSpeed) {
    case TurntableSpeed::Rpm33:
      targetSpeed = 33.3333;
      state.outputShift.setValue(StmShiftPin::Led33Rpm, true);
      break;
    case TurntableSpeed::Rpm45:
      targetSpeed = 45.0;
      state.outputShift.setValue(StmShiftPin::Led45Rpm, true);
      break;
    case TurntableSpeed::Rpm78:
      targetSpeed = 78.0;
      state.outputShift.setValue(StmShiftPin::Led78Rpm, true);
      break;
    case TurntableSpeed::RpmAuto:
      // TODO: If playing a record, go based on the last-played size

      state.outputShift.setValue(StmShiftPin::LedAutoSpeed, true);
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
  state.outputShift.setValue(StmShiftPin::Led7In, false);
  state.outputShift.setValue(StmShiftPin::Led10In, false);
  state.outputShift.setValue(StmShiftPin::Led12In, false);
  state.outputShift.setValue(StmShiftPin::LedAutoSize, false);

  switch(newSize) {
    case RecordSize::In7:
      state.outputShift.setValue(StmShiftPin::Led7In, true);
      break;
    case RecordSize::In10:
      state.outputShift.setValue(StmShiftPin::Led10In, true);
      break;
    case RecordSize::In12:
      state.outputShift.setValue(StmShiftPin::Led12In, true);
      break;
    case RecordSize::InAuto:
      state.outputShift.setValue(StmShiftPin::LedAutoSize, true);
      break;
  }

  selectedSize = newSize;
}

void blinkCustomSpeedIndicator() {
  if(selectedSpeed == TurntableSpeed::RpmCustom && clockMicros - customSpeedIndicatorCounter > ONE_SECOND_MICROS) {
    customSpeedIndicatorCounter = clockMicros;
    state.outputShift.setValue(StmShiftPin::LedAutoSpeed, !state.outputShift.getValue(StmShiftPin::LedAutoSpeed));
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