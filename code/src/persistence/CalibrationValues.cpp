#include <CalibrationValues.h>
#include <Constants.h>
#include <VerticalDirection.h>
#include <AzimuthDirection.h>

CalibrationValues::CalibrationValues() {
    // Do nothing (For now...)
}

void CalibrationValues::load() {
    // to do
    verticalUpperLimit = TEST_VERTICAL_UPPER_LIMIT;
    verticalLowerLimit = TEST_VERTICAL_LOWER_LIMIT;
    polarity = StmEncoderPolarity::REVERSED;
    verticalUp = VerticalDirection::Up;
    verticalDown = VerticalDirection::Down;

    // In theory, these horizontal calibrations should no longer be relevant once I switch motors
    horizontalClockwise = AzimuthDirection::Clockwise;
    horizontalCounterclockwise = AzimuthDirection::CounterClockwise;
}

void CalibrationValues::persist() {
    // to do
}