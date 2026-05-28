#include <Arduino.h>
#include <CommandError.h>
#include <VerticalDirection.h>
#include <LiftStatus.h>
#include <HomeStatus.h>
#include <TurntableSpeed.h>
#include <RecordSize.h>
#include <step/result/LowerTonearmResult.h>

#ifndef PROTOTYPES_H
#define PROTOTYPES_H

void monitorSerialInputs();
void monitorCommandInput();
void executeCommand();
void updateClockMicros();
void initPlayAction(int16_t stepCount, uint8_t speed);
void runPlayAction();
void initPauseUnpauseAction();
void runPauseAction();
void runUnPauseAction();
void endActionCommand();
void endUnPauseAction();
void initErrorAction(CommandError error);
void errorActionCommand();
void endErrorAction();
bool liftToCalibratedPosition();
bool waitForLiftStatus();
LowerTonearmResult lowerUntilTonearmReleased();
bool lowerBelowRecord();
bool engageAzimuthClutch();
bool disengageAzimuthClutch();
bool checkVerticalStall(VerticalDirection direction, int currentPosition);
void readSerial(Stream& stream);
void advanceCounts();
LiftStatus getLiftStatus();
HomeStatus getHomeStatus();
void rotateSpeed();
void updateSpeed(TurntableSpeed newSpeed);
void blinkCustomSpeedIndicator();
void rotateSize();
void updateSize(RecordSize newSize);

#endif