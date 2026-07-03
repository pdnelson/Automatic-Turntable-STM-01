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
        void processToggleClutch();
        void processStepHorizontally(Stream& stream);
        void processGoToPositionH(Stream& stream);
        void processSetCustomSpeed(Stream& stream);
        void processGetHorizontalEncoderPos(Stream& stream);
        void processGetVerticalEncoderPos(Stream& stream);
        byte processGetCurrentCommand();
        byte processGetCurrentSubCommand();
        byte processGetCommandStatus();
        void processGetUpTime(Stream& stream);
        void processGetSpeedTarget(Stream& stream);
        void processGetAdvancedSuiteData(Stream& stream);

        int16_t readInt16(Stream& stream);
};

#endif