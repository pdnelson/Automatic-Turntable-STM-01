#include <Arduino.h>

#ifndef STMSERIAL_H
#define STMSERIAL_H

// This data contains the following:
// - start key
// - current playlist index
// - profile size
// - playlist size
// - end key
#define PROFILE_PLAYLIST_KNOWN_DATA_SIZE 7

class TurntableState;

class StmSerial {
    public:
        StmSerial(TurntableState* state);

        void monitor();

    private:
        TurntableState* state;

        void readSerialData(Stream& stream);

        // Request processing methods
        void processInitKey(Stream& stream);
        void processProtoPlay(Stream& stream);
        void processMoveNStepsV(Stream& stream);
        void processToggleClutch();
        void processStepHorizontally(Stream& stream);
        void processGoToPositionH(Stream& stream);
        void processGoToPositionV(Stream& stream);
        void processSetCustomSpeed(Stream& stream);
        void processSetAzEncoderZero(Stream& stream);
        void processSetOverwriteRecordProfile(Stream& stream);
        void processSetPositionAtProfileIndex(Stream& stream);
        void processGetHorizontalEncoderPos(Stream& stream);
        void processGetVerticalEncoderPos(Stream& stream);
        byte processGetCurrentCommand();
        byte processGetCurrentSubCommand();
        byte processGetCommandStatus();
        void processGetUpTime(Stream& stream);
        void processGetSpeedTarget(Stream& stream);
        void processGetAdvancedSuiteData(Stream& stream);
        void processGetCalibrationValues(Stream& stream);
        void processGetProfilePlaylist(Stream& stream);
        void clearCommand();

        int16_t readInt16(Stream& stream);
};

#endif