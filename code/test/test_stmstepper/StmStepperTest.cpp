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

void test_movementCompleted_movingPositiveNoToleranceOnDestination_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;

    stepper.setEncoderRange(start, end, 0);

    // at the end
    bool result = stepper.movementCompleted(end);

    TEST_ASSERT_EQUAL(true, result);
}

void test_movementCompleted_movingNegativeNoToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;

    stepper.setEncoderRange(start, end, 0);

    // 1 encoder tick before the end
    bool result = stepper.movementCompleted(end + 1);

    TEST_ASSERT_EQUAL(false, result);
}

void test_movementCompleted_movingNegativeNoToleranceOnDestination_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;

    stepper.setEncoderRange(start, end, 0);

    // 1 encoder tick before the end
    bool result = stepper.movementCompleted(end);

    TEST_ASSERT_EQUAL(true, result);
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_movementCompleted_movingPositiveNoToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingPositiveNoToleranceOnDestination_true);
    RUN_TEST(test_movementCompleted_movingNegativeNoToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingNegativeNoToleranceOnDestination_true);

    return UNITY_END();
}

void setup() {
    delay(2000);

    runUnityTests();
}

void loop() { /* do nothing */}