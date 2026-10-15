#include "unity.h"
#include <Arduino.h>

// Sometimes, I just need a little playground when I want to figure out what C++ does in
// certain scenarios.

void test_clang_whyIsMySpeedNotWorkingWhenIPassADecimal_itWorksHopefully() {

    uint16_t stepsPerRev = 6400;
    float speed = 0.5;
    
    unsigned long result = (unsigned long)((float)60000000L / (float)stepsPerRev / speed);

    TEST_ASSERT_EQUAL(18750, result);
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_clang_whyIsMySpeedNotWorkingWhenIPassADecimal_itWorksHopefully);

    return UNITY_END();
}

void setup() {
    delay(2000);

    runUnityTests();
}

void loop() { /* do nothing */ }