#include <StmSerial.h>
#include <Arduino.h>
#include <TurntableState.h>
#include <Constants.h>
#include <ExternalCommand.h>
#include <CmdError.h>
#include <CmdPause.h>
#include <CmdProtoPlay.h>
#include <Pin.h>
#include <memory>
#include <BaseTurntableCommand.h>

StmSerial::StmSerial(TurntableState* state) {
    this->state = state;
    STM_SERIAL_USB.begin(SERIAL_SPEED);
    STM_SERIAL_1.begin(SERIAL_SPEED);
    STM_SERIAL_2.begin(SERIAL_SPEED);
    STM_SERIAL_3.begin(SERIAL_SPEED);
    STM_SERIAL_4.begin(SERIAL_SPEED);
    STM_SERIAL_5.begin(SERIAL_SPEED);
}

void StmSerial::monitor() {
    readSerialData(STM_SERIAL_USB);
    readSerialData(STM_SERIAL_1);
    readSerialData(STM_SERIAL_2);
    readSerialData(STM_SERIAL_3);
    readSerialData(STM_SERIAL_4);
    readSerialData(STM_SERIAL_5);
}

void StmSerial::readSerialData(Stream& stream) {
    if(stream.available() > 0) {
        uint8_t key = stream.read();

        // If the key is the init key, send back model and version info
        if(key == SERIAL_COMMAND_INIT_KEY) {
            processInitKey(stream);
        }

        // If the key is for this model turntable, execute the command
        else if(key == SERIAL_COMMAND_MODEL_KEY) {
            ExternalCommand incomingCommand = (ExternalCommand)stream.read();

            // If a command is already running, and an action command comes through, then throw it out.
            if(state->currentCommand != nullptr && incomingCommand < 31 && incomingCommand > 0) {
                return;
            }

            switch(incomingCommand) {
                // Misc.
                case ExternalCommand::ConnectionTest:           stream.write(SERIAL_COMMAND_CONNECTION_SUCCESS);    break;
                
                // Action Commands
                case ExternalCommand::ActionPauseUnPause:       state->pauseOrUnPause();                            break;
                case ExternalCommand::ActionProtoPlay:          processProtoPlay(stream);                           break;
                
                // Set Commands
                case ExternalCommand::SetSpeed:                 state->updateSpeed((TurntableSpeed)stream.read());  break;
                case ExternalCommand::SetRotateSpeed:           state->rotateSpeed();                               break;
                case ExternalCommand::SetCustomSpeed:           processSetCustomSpeed(stream);                      break;
                case ExternalCommand::SetSize:                  state->updateSize((RecordSize)stream.read());       break;
                case ExternalCommand::SetRotateSize:            state->rotateSize();                                break;
                case ExternalCommand::SetClearActionCommand:    state->currentCommand = nullptr;                    break;
                
                // Get Commands
                case ExternalCommand::GetHorizontalEncoderPos:  break; // todo
                case ExternalCommand::GetVerticalEncoderPos:    processGetVerticalEncoderPos(stream);               break;
                case ExternalCommand::GetLiftStatus:            stream.write(state->getLiftStatus());               break;
                case ExternalCommand::GetHomeStatus:            stream.write(state->getHomeStatus());               break;
                case ExternalCommand::GetCurrentCommand:        processGetCurrentCommand(stream);                   break;
                case ExternalCommand::GetErrorCode:             processGetErrorCode(stream);                        break;
                case ExternalCommand::GetUpTime:                processGetUpTime(stream);                           break;
                case ExternalCommand::GetSpeedSetting:          stream.write(state->selectedSpeed);                 break;
                case ExternalCommand::GetSpeedTarget:           processGetSpeedTarget(stream);                      break;
                case ExternalCommand::GetSizeSetting:           stream.write(state->selectedSize);                  break;
            }
        }
    }
}

void StmSerial::processInitKey(Stream& stream) {
    byte data[4];
    data[0] = SERIAL_COMMAND_MODEL_KEY;
    data[1] = VERSION_MAJOR;
    data[2] = VERSION_MINOR;
    data[3] = VERSION_PATCH;

    stream.write(data, 4);
}

void StmSerial::processProtoPlay(Stream& stream) {
    int16_t data1 = stream.read() & 0x00FF;
    int16_t data2 = stream.read() << 8 & 0xFF00;
    int16_t stepCount = data1 | data2;

    state->currentCommand = std::make_unique<CmdProtoPlay>(state, stepCount, stream.read());
}

void StmSerial::processSetCustomSpeed(Stream& stream) {
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
}

void StmSerial::processGetVerticalEncoderPos(Stream& stream) {
    uint16_t verticalPosition = analogRead(Pin::VerticalPosition);

    byte data[2];
    data[0] = verticalPosition & 0x00FF;
    data[1] = (verticalPosition >> 8) & 0x00FF;

    stream.write(data, 2);
}

void StmSerial::processGetCurrentCommand(Stream& stream) {
    ActionCommand current = ActionCommand::NoAction;
          
    if(state->currentCommand != nullptr) {
        current = state->currentCommand->getCommandId();
    }
          
    stream.write(current);
}

void StmSerial::processGetErrorCode(Stream& stream) {
    if(state->currentCommand != nullptr && state->currentCommand->getCommandId() == ActionCommand::Error) {
        BaseTurntableCommand* base = state->currentCommand.get();
        CmdError* errorCommand = static_cast<CmdError*>(base);
        stream.write(errorCommand->error);
    } else {
        stream.write((byte)0); // 0 represents no error
    }
}

void StmSerial::processGetUpTime(Stream& stream) {
    byte data[4];
    data[0] = state->upTimeSeconds & 0x000000FF;
    data[1] = (state->upTimeSeconds >> 8) & 0x000000FF;
    data[2] = (state->upTimeSeconds >> 16) & 0x000000FF;
    data[3] = (state->upTimeSeconds >> 24) & 0x000000FF;

    stream.write(data, 4);
}

void StmSerial::processGetSpeedTarget(Stream& stream) {
    float speed = -1;
    if(state->getHomeStatus() == HomeStatus::NotHomed) {
        speed = state->targetSpeed;
    }
          
    byte const* data = reinterpret_cast<byte const*>(&speed);
    stream.write(data, sizeof(float));
}