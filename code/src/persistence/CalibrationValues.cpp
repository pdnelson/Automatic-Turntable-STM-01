#include <CalibrationValues.h>
#include <Constants.h>

CalibrationValues::CalibrationValues() {
    // Do nothing (For now...)
}

void CalibrationValues::load() {
    // to do
    home = 2045;
    in12 = 2983;
    in10 = 3257;
    verticalUpperLimit = TEST_VERTICAL_UPPER_LIMIT;
    verticalLowerLimit = TEST_VERTICAL_LOWER_LIMIT;
    polarity = StmEncoderPolarity::REVERSED;
}

void CalibrationValues::persist() {
    // to do
}