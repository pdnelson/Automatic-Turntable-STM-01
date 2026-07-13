#include "unity.h"
#include <Arduino.h>
#include <StmStepper.h>

// Positive boundary tests without tolerance

void test_movementCompleted_movingPositiveNoToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;

    stepper.setEncoderRange(start, end, 0);

    bool result = stepper.movementCompleted(end - 1);

    TEST_ASSERT_EQUAL(false, result);
}

void test_movementCompleted_movingPositiveNoToleranceOnDestination_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;

    stepper.setEncoderRange(start, end, 0);

    bool result = stepper.movementCompleted(end);

    TEST_ASSERT_EQUAL(true, result);
}

// Negative boundary tests without tolerance

void test_movementCompleted_movingNegativeNoToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;

    stepper.setEncoderRange(start, end, 0);

    bool result = stepper.movementCompleted(end + 1);

    TEST_ASSERT_EQUAL(false, result);
}

void test_movementCompleted_movingNegativeNoToleranceOnDestination_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;

    stepper.setEncoderRange(start, end, 0);

    bool result = stepper.movementCompleted(end);

    TEST_ASSERT_EQUAL(true, result);
}

// Positive boundary tests with tolerance
void test_movementCompleted_movingPositiveWithToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.movementCompleted(end - tol - 1);

    TEST_ASSERT_EQUAL(false, result);
}

void test_movementCompleted_movingPositiveWithToleranceOnDestinationLowerBoundary_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    // 1 encoder tick before the end
    bool result = stepper.movementCompleted(end - tol);

    TEST_ASSERT_EQUAL(true, result);
}

void test_movementCompleted_movingPositiveWithToleranceAfterDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.movementCompleted(end + tol + 1);

    TEST_ASSERT_EQUAL(false, result);
}

void test_movementCompleted_movingPositiveWithToleranceOnDestinationUpperBound_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.movementCompleted(end + tol);

    TEST_ASSERT_EQUAL(true, result);
}

// Negative boundary tests with tolerance
void test_movementCompleted_movingNegativeWithToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.movementCompleted(end - tol - 1);

    TEST_ASSERT_EQUAL(false, result);
}

void test_movementCompleted_movingNegativeWithToleranceOnDestinationLowerBoundary_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    // 1 encoder tick before the end
    bool result = stepper.movementCompleted(end - tol);

    TEST_ASSERT_EQUAL(true, result);
}

void test_movementCompleted_movingNegativeWithToleranceAfterDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.movementCompleted(end + tol + 1);

    TEST_ASSERT_EQUAL(false, result);
}

void test_movementCompleted_movingNegativeWithToleranceOnDestinationUpperBound_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.movementCompleted(end + tol);

    TEST_ASSERT_EQUAL(true, result);
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_movementCompleted_movingPositiveNoToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingPositiveNoToleranceOnDestination_true);
    RUN_TEST(test_movementCompleted_movingNegativeNoToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingNegativeNoToleranceOnDestination_true);
    RUN_TEST(test_movementCompleted_movingPositiveWithToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingPositiveWithToleranceOnDestinationLowerBoundary_true);
    RUN_TEST(test_movementCompleted_movingPositiveWithToleranceAfterDestination_false);
    RUN_TEST(test_movementCompleted_movingPositiveWithToleranceOnDestinationUpperBound_false);
    RUN_TEST(test_movementCompleted_movingNegativeWithToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingNegativeWithToleranceOnDestinationLowerBoundary_true);
    RUN_TEST(test_movementCompleted_movingNegativeWithToleranceAfterDestination_false);
    RUN_TEST(test_movementCompleted_movingNegativeWithToleranceOnDestinationUpperBound_false);

    return UNITY_END();
}

void setup() {
    delay(2000);

    runUnityTests();
}

void loop() { /* do nothing */}