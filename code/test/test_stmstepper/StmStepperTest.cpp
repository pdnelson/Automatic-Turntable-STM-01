#include "unity.h"
#include <Arduino.h>
#include <StmStepper.h>

void test_movementCompleted_movingPositiveNoToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;

    stepper.setEncoderRange(start, end, 0);

    // 1 encoder tick before the end
    bool result = stepper.movementCompleted(end - 1);

    TEST_ASSERT_EQUAL(false, result);
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_movementCompleted_movingPositiveNoToleranceBeforeDestination_false);

    return UNITY_END();
}

void setup() {
    delay(2000);

    runUnityTests();
}

void loop() { /* do nothing */}