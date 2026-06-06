#include <Arduino.h>

#ifndef STMSERIAL_H
#define STMSERIAL_H
class TurntableState;

class StmSerial {
    public:
        StmSerial(TurntableState* state);
        TurntableState* state;

        void monitor();

    private:
        void readSerialData(Stream& stream);

        // Request processing methods
        void processInitKey(Stream& stream);
        void processProtoPlay(Stream& stream);
        void processSetCustomSpeed(Stream& stream);
        void processGetVerticalEncoderPos(Stream& stream);
        void processGetCurrentCommand(Stream& stream);
        void processGetCurrentSubCommand(Stream& stream);
        void processGetErrorCode(Stream& stream);
        void processGetUpTime(Stream& stream);
        void processGetSpeedTarget(Stream& stream);
};

#endif