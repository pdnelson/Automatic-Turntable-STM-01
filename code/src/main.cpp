#include <memory>
#include <Arduino.h>
#include <Pin.h>
#include <MuxPin.h>
#include <InputMux.h>
#include <ClutchDirection.h>
#include <ClutchStatus.h>
#include <Constants.h>
#include <Stepper.h>
#include <LiftStatus.h>
#include <HomeStatus.h>
#include <VerticalDirection.h>
#include <MovementAxis.h>
#include <StmShift.h>
#include <StmShiftPin.h>
#include <ExternalCommand.h>
#include <TurntableSpeed.h>
#include <RecordSize.h>
#include <AzimuthDirection.h>
#include <Prototypes.h>
#include <state/TurntableState.h>
#include <command/pause/CmdPause.h>

TurntableState* state;

void setup() {
  // Encoders
  pinMode(Pin::SpeedEncoderA, INPUT);
  pinMode(Pin::SpeedEncoderB, INPUT);
  pinMode(Pin::HorizontalPositionEncoderA, INPUT);
  pinMode(Pin::HorizontalPositionEncoderB, INPUT);

  // Motor Phases
  pinMode(Pin::TurntableMotorPhase1, OUTPUT);
  pinMode(Pin::TurntableMotorPhase2, OUTPUT);
  pinMode(Pin::TurntableMotorPhase3, OUTPUT);

  // Various Sensors
  pinMode(Pin::HeadshellPhotoDiode, INPUT);

  // Various Status
  pinMode(Pin::PowerOnStatusIn, INPUT);

  state = new TurntableState();

  STM_SERIAL_USB.begin(SERIAL_SPEED);
  STM_SERIAL_1.begin(SERIAL_SPEED);
  STM_SERIAL_2.begin(SERIAL_SPEED);
  STM_SERIAL_3.begin(SERIAL_SPEED);
  STM_SERIAL_4.begin(SERIAL_SPEED);
  STM_SERIAL_5.begin(SERIAL_SPEED);
}

void loop() {
  state->monitor();
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
      if(state->currentCommand != nullptr && incomingCommand < 31 && incomingCommand > 0) {
        return;
      }

      switch(incomingCommand) {
        case ExternalCommand::ConnectionTest: 
          stream.write(SERIAL_COMMAND_CONNECTION_SUCCESS);
          break;
        case ExternalCommand::ActionPauseUnPause: {
          state->currentCommand = std::make_unique<CmdPause>(state);
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
          state->updateSpeed((TurntableSpeed)stream.read());
          break;
        }
        case ExternalCommand::SetRotateSpeed: {
          state->rotateSpeed();
          break;
        }
        case ExternalCommand::SetCustomSpeed: {
          uint32_t data1 = stream.read() & 0x000000FF;
          uint32_t data2 = stream.read() << 8 & 0x0000FF00;
          uint32_t data3 = stream.read() << 16 & 0x00FF0000;
          uint32_t data4 = stream.read() << 24 & 0xFF000000;

          uint32_t dataCombined = data1 | data2 | data3 | data4;
          float* finalSpeed = reinterpret_cast<float*>((void*) &dataCombined);

          state->targetSpeed = *finalSpeed;

          if(state->targetSpeed < 33.33333 && state->targetSpeed > 33.3) {
            state->updateSpeed(TurntableSpeed::Rpm33);
          } else if(state->targetSpeed == 45.0) {
            state->updateSpeed(TurntableSpeed::Rpm45);
          } else if(state->targetSpeed == 78.0) {
            state->updateSpeed(TurntableSpeed::Rpm78);
          } else {
            state->updateSpeed(TurntableSpeed::RpmCustom);
          }
          break;
        }
        case ExternalCommand::SetSize: {
          state->updateSize((RecordSize)stream.read());
          break;
        }
        case ExternalCommand::SetRotateSize: {
          state->rotateSize();
          break;
        }
        case ExternalCommand::SetClearActionCommand: {
          state->currentCommand = nullptr;
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
          stream.write(state->getLiftStatus());
          break;
        }
        case ExternalCommand::GetHomeStatus: {
          stream.write(state->getHomeStatus());
          break;
        }
        case ExternalCommand::GetCurrentCommand: {
          ActionCommand current = ActionCommand::NoAction;
          if(state->currentCommand != nullptr) {
            state->currentCommand->getCommandId();
          }
          
          stream.write(current);
          break;
        }
        case ExternalCommand::GetErrorCode: {
          if(state->currentCommand != nullptr && state->currentCommand->getCommandId() == ActionCommand::Error) {
            //stream.write(actionVariable1); // actionVariable1 holds the error code
            // todo: write error code here
          } else {
            stream.write((uint8_t)0); // 0 represents no error
          }
          break;
        }
        case ExternalCommand::GetUpTime: {
          byte data[4];
          data[0] = state->upTimeSeconds & 0x000000FF;
          data[1] = (state->upTimeSeconds >> 8) & 0x000000FF;
          data[2] = (state->upTimeSeconds >> 16) & 0x000000FF;
          data[3] = (state->upTimeSeconds >> 24) & 0x000000FF;

          stream.write(data, 4);
          break;
        }
        case ExternalCommand::GetSpeedSetting: {
          stream.write(state->selectedSpeed);
          break;
        }
        case ExternalCommand::GetSpeedTarget: {
          float speed = -1;
          if(state->getHomeStatus() == HomeStatus::NotHomed) {
            speed = state->targetSpeed;
          }
          
          byte const * data = reinterpret_cast<byte const *>(&speed);
          stream.write(data, sizeof(float));
          break;
        }
        case ExternalCommand::GetSizeSetting: {
          stream.write(state->selectedSize);
          break;
        }
      }
  
    }
  }
}