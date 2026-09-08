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
    polarityH = StmEncoderPolarity::REVERSED;
    polarityV = StmEncoderPolarity::NORMAL;
}

void CalibrationValues::persist() {
    // to do
}