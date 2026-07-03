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
#include <CommandId.h>
#include <SubCommandId.h>
#include <CmdToggleClutch.h>
#include <CmdStepHorizontally.h>
#include <CmdGoToPositionH.h>

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
                case ExternalCommand::ActionToggleClutch:       processToggleClutch();                              break;
                case ExternalCommand::ActionStepHorizontally:   processStepHorizontally(stream);                    break;
                case ExternalCommand::ActionGoToPositionH:      processGoToPositionH(stream);                       break;
                
                // Set Commands
                case ExternalCommand::SetSpeed:                 state->updateSpeed((TurntableSpeed)stream.read());  break;
                case ExternalCommand::SetRotateSpeed:           state->rotateSpeed();                               break;
                case ExternalCommand::SetCustomSpeed:           processSetCustomSpeed(stream);                      break;
                case ExternalCommand::SetSize:                  state->updateSize((RecordSize)stream.read());       break;
                case ExternalCommand::SetRotateSize:            state->rotateSize();                                break;
                case ExternalCommand::SetClearActionCommand:    state->currentCommand = nullptr;                    break;
                
                // Get Commands
                case ExternalCommand::GetHorizontalEncoderPos:  processGetHorizontalEncoderPos(stream);             break;
                case ExternalCommand::GetVerticalEncoderPos:    processGetVerticalEncoderPos(stream);               break;
                case ExternalCommand::GetLiftStatus:            stream.write(state->getLiftStatus());               break;
                case ExternalCommand::GetHomeStatus:            stream.write(state->getHomeStatus());               break;
                case ExternalCommand::GetCurrentCommand:        stream.write(processGetCurrentCommand());           break;
                case ExternalCommand::GetCurrentSubCommand:     stream.write(processGetCurrentSubCommand());        break;
                case ExternalCommand::GetCommandStatus:         stream.write(processGetCommandStatus());            break;
                case ExternalCommand::GetUpTime:                processGetUpTime(stream);                           break;
                case ExternalCommand::GetSpeedSetting:          stream.write(state->selectedSpeed);                 break;
                case ExternalCommand::GetSpeedTarget:           processGetSpeedTarget(stream);                      break;
                case ExternalCommand::GetSizeSetting:           stream.write(state->selectedSize);                  break;
                case ExternalCommand::GetAdvancedSuiteData:     processGetAdvancedSuiteData(stream);                break;
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
    int16_t stepCount = readInt16(stream);
    uint8_t speed = stream.read();

    state->currentCommand = std::make_unique<CmdProtoPlay>(state, stepCount, speed);
}

void StmSerial::processToggleClutch() {
    state->currentCommand = std::make_unique<CmdToggleClutch>(state);
}

void StmSerial::processStepHorizontally(Stream& stream) {
    int16_t stepCount = readInt16(stream);
    uint8_t speed = stream.read();

    state->currentCommand = std::make_unique<CmdStepHorizontally>(state, stepCount, speed);
}

void StmSerial::processGoToPositionH(Stream& stream) {
    int16_t position = readInt16(stream);
    uint8_t speed = stream.read();
    
    state->currentCommand = std::make_unique<CmdGoToPositionH>(state, position, speed);
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

void StmSerial::processGetHorizontalEncoderPos(Stream& stream) {
    uint16_t horizontalPosition = state->azEncoder.getNormalizedPosition();

    byte data[2];
    data[0] = horizontalPosition & 0x00FF;
    data[1] = (horizontalPosition >> 8) & 0x00FF;

    stream.write(data, 2);
}

void StmSerial::processGetVerticalEncoderPos(Stream& stream) {
    uint16_t verticalPosition = analogRead(Pin::VerticalPosition);

    byte data[2];
    data[0] = verticalPosition & 0x00FF;
    data[1] = (verticalPosition >> 8) & 0x00FF;

    stream.write(data, 2);
}

byte StmSerial::processGetCurrentCommand() {
    CommandId current = CommandId::NoCommand;
          
    if(state->currentCommand != nullptr) {
        current = state->currentCommand->getCommandId();
    }
          
    return current;
}

byte StmSerial::processGetCurrentSubCommand() {
    SubCommandId current = SubCommandId::NoSubCommand;

    if(state->currentCommand != nullptr) {
        current = state->currentCommand->currentSubCommandId();
    }

    return current;
}

byte StmSerial::processGetCommandStatus() {
    if(state->currentCommand != nullptr && state->currentCommand->getCommandId() == CommandId::Error) {
        BaseTurntableCommand* base = state->currentCommand.get();
        CmdError* errorCommand = static_cast<CmdError*>(base);
        return errorCommand->error;
    } else {
        return CommandResult::Running;
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
    float speed = state->getTargetSpeed();
          
    byte const* data = reinterpret_cast<byte const*>(&speed);

    stream.write(data, sizeof(float));
}

void StmSerial::processGetAdvancedSuiteData(Stream& stream) {
    uint8_t dataSize = 19;
    byte data[dataSize];

    // Vertical position
    uint16_t verticalPosition = analogRead(Pin::VerticalPosition);
    data[0] = verticalPosition & 0x00FF;
    data[1] = (verticalPosition >> 8) & 0x00FF;

    // Horizontal position
    uint16_t horizontalPosition = state->azEncoder.getNormalizedPosition();
    data[2] = horizontalPosition & 0x00FF;
    data[3] = (horizontalPosition >> 8) & 0x00FF;

    // Lift status
    data[4] = state->getLiftStatus();

    // Home status
    data[5] = state->getHomeStatus();

    // Current command
    data[6] = processGetCurrentCommand();

    // Current subcommand
    data[7] = processGetCurrentSubCommand();

    // Command status
    data[8] = processGetCommandStatus();

    // Up time
    data[9] = state->upTimeSeconds & 0x000000FF;
    data[10] = (state->upTimeSeconds >> 8) & 0x000000FF;
    data[11] = (state->upTimeSeconds >> 16) & 0x000000FF;
    data[12] = (state->upTimeSeconds >> 24) & 0x000000FF;

    // Speed Setting
    data[13] = state->selectedSpeed;

    // Speed Target
    float targetSpeed = state->getTargetSpeed();
    byte const* targetSpeedBytes = reinterpret_cast<byte const*>(&targetSpeed);
    data[14] = targetSpeedBytes[0];
    data[15] = targetSpeedBytes[1];
    data[16] = targetSpeedBytes[2];
    data[17] = targetSpeedBytes[3];

    // Size Setting
    data[18] = state->selectedSize;

    stream.write(data, dataSize);
}

int16_t StmSerial::readInt16(Stream& stream) {
    int16_t data1 = stream.read() & 0x00FF;
    int16_t data2 = stream.read() << 8 & 0xFF00;
    return data1 | data2;
}