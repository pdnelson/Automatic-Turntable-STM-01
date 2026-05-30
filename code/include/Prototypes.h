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
void endActionCommand();
void endUnPauseAction();
void initErrorAction(CommandError error);
void errorActionCommand();
void endErrorAction();
bool engageAzimuthClutch();
bool disengageAzimuthClutch();
void readSerial(Stream& stream);
void advanceCounts();
void rotateSpeed();
void updateSpeed(TurntableSpeed newSpeed);
void blinkCustomSpeedIndicator();
void rotateSize();
void updateSize(RecordSize newSize);

#endif