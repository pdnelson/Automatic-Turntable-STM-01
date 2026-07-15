#include "unity.h"
#include <Arduino.h>
#include <StmStepper.h>

// Positive movement completed tests without tolerance

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

// Negative movement completed tests without tolerance

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

// Positive movement completed tests with tolerance
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

// note: after destination is still considered complete, because a movement
// can only move one direction at a time. So, no matter how far, if it has gone past
// the destination, then the movement is complete.
void test_movementCompleted_movingPositiveWithToleranceAfterDestination_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.movementCompleted(end + tol + 1);

    TEST_ASSERT_EQUAL(true, result);
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

// Negative movement completed tests with tolerance
// note: before destination is still considered complete, because a movement
// can only move one direction at a time. So, no matter how far, if it has gone past
// the destination, then the movement is complete.
void test_movementCompleted_movingNegativeWithToleranceBeforeDestination_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.movementCompleted(end - tol - 1);

    TEST_ASSERT_EQUAL(true, result);
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

// Positive boundary tests without tolerance

void test_onBoundary_movingPositiveNoToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;

    stepper.setEncoderRange(start, end, 0);

    bool result = stepper.onBoundary(end - 1, end, 0);

    TEST_ASSERT_EQUAL(false, result);
}

void test_onBoundary_movingPositiveNoToleranceOnDestination_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;

    stepper.setEncoderRange(start, end, 0);

    bool result = stepper.onBoundary(end, end, 0);

    TEST_ASSERT_EQUAL(true, result);
}

// Negative boundary tests without tolerance

void test_onBoundary_movingNegativeNoToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;

    stepper.setEncoderRange(start, end, 0);

    bool result = stepper.onBoundary(end + 1, end, 0);

    TEST_ASSERT_EQUAL(false, result);
}

void test_onBoundary_movingNegativeNoToleranceOnDestination_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;

    stepper.setEncoderRange(start, end, 0);

    bool result = stepper.onBoundary(end, end, 0);

    TEST_ASSERT_EQUAL(true, result);
}

// Positive boundary tests with tolerance
void test_onBoundary_movingPositiveWithToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.onBoundary(end - tol - 1, end, tol);

    TEST_ASSERT_EQUAL(false, result);
}

void test_onBoundary_movingPositiveWithToleranceOnDestinationLowerBoundary_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    // 1 encoder tick before the end
    bool result = stepper.onBoundary(end - tol, end, tol);

    TEST_ASSERT_EQUAL(true, result);
}

void test_onBoundary_movingPositiveWithToleranceAfterDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.onBoundary(end + tol + 1, end, tol);

    TEST_ASSERT_EQUAL(false, result);
}

void test_onBoundary_movingPositiveWithToleranceOnDestinationUpperBound_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 100;
    uint16_t end = 200;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.onBoundary(end + tol, end, tol);

    TEST_ASSERT_EQUAL(true, result);
}

// Negative boundary tests with tolerance
void test_onBoundary_movingNegativeWithToleranceBeforeDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.onBoundary(end - tol - 1, end, tol);

    TEST_ASSERT_EQUAL(false, result);
}

void test_onBoundary_movingNegativeWithToleranceOnDestinationLowerBoundary_true() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    // 1 encoder tick before the end
    bool result = stepper.onBoundary(end - tol, end, tol);

    TEST_ASSERT_EQUAL(true, result);
}

void test_onBoundary_movingNegativeWithToleranceAfterDestination_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.onBoundary(end + tol + 1, end, tol);

    TEST_ASSERT_EQUAL(false, result);
}

void test_onBoundary_movingNegativeWithToleranceOnDestinationUpperBound_false() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);

    uint16_t start = 200;
    uint16_t end = 100;
    uint8_t tol = 5;

    stepper.setEncoderRange(start, end, tol);

    bool result = stepper.onBoundary(end + tol, end, tol);

    TEST_ASSERT_EQUAL(true, result);
}

// ticksToBoundarySoFar calculations (positive movement)
void test_ticksToBoundarySoFar_beforeStartOfEncoderTicksPositiveMovement_zero() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t current = range - 1;
    uint16_t end = 10;
    stepper.setEncoderRange(0, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(0, result);
}

void test_ticksToBoundarySoFar_atStartOfEncoderTicksPositiveMovement_zero() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t current = range;
    uint16_t end = 10;
    stepper.setEncoderRange(0, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(0, result);
}

void test_ticksToBoundarySoFar_oneAboveStartOfEncoderTicksPositiveMovement_one() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t current = range + 1;
    uint16_t end = 10;
    stepper.setEncoderRange(0, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(1, result);
}

void test_ticksToBoundarySoFar_twoAboveStartOfEncoderTicksPositiveMovement_two() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t current = range + 2;
    uint16_t end = 10;
    stepper.setEncoderRange(0, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(2, result);
}

void test_ticksToBoundarySoFar_oneBeforeEndOfEncoderTicksPositiveMovement_four() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t end = 10;
    uint16_t current = end - 1;
    stepper.setEncoderRange(0, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(4, result);
}

void test_ticksToBoundarySoFar_atEndOfEncoderTicksPositiveMovement_max() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t end = 10;
    uint16_t current = end;
    stepper.setEncoderRange(0, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(range, result);
}

void test_ticksToBoundarySoFar_beyondEncoderTicksPositiveMovement_max() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t end = 10;
    uint16_t current = end + 1;
    stepper.setEncoderRange(0, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(range, result);
}

// ticksToBoundarySoFar calculations (negative movement)
void test_ticksToBoundarySoFar_beforeStartOfEncoderTicksNegativeMovement_zero() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t current = range + 1;
    uint16_t end = 0;
    uint16_t start = 10;
    stepper.setEncoderRange(start, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(0, result);
}

void test_ticksToBoundarySoFar_atStartOfEncoderTicksNegativeMovement_zero() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t end = 0;
    uint16_t start = 10;
    uint16_t current = range;
    stepper.setEncoderRange(start, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(0, result);
}

void test_ticksToBoundarySoFar_oneAboveStartOfEncoderTicksNegativeMovement_one() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t current = range - 1;
    uint16_t end = 0;
    uint16_t start = 10;
    stepper.setEncoderRange(start, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(1, result);
}

void test_ticksToBoundarySoFar_twoAboveStartOfEncoderTicksNegativeMovement_two() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t current = range - 2;
    uint16_t end = 0;
    uint16_t start = 10;
    stepper.setEncoderRange(start, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(2, result);
}

void test_ticksToBoundarySoFar_oneBeforeEndOfEncoderTicksNegativeMovement_four() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t end = 0;
    uint16_t start = 10;
    uint16_t current = end + 1;
    stepper.setEncoderRange(start, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(4, result);
}

void test_ticksToBoundarySoFar_atEndOfEncoderTicksNegativeMovement_min() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t end = 0;
    uint16_t current = end;
    uint16_t start = 10;
    stepper.setEncoderRange(start, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(range, result);
}

void test_ticksToBoundarySoFar_beyondEncoderTicksNegativeMovement_min() {
    StmStepper stepper = StmStepper(0, 0, 0, 0);
    
    uint16_t range = 5;
    uint16_t end = 1;
    uint16_t current = end - 1;
    uint16_t start = 10;
    stepper.setEncoderRange(start, end, 0);

    uint16_t result = stepper.ticksToBoundarySoFar(current, end, range);

    TEST_ASSERT_EQUAL(range, result);
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_movementCompleted_movingPositiveNoToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingPositiveNoToleranceOnDestination_true);
    RUN_TEST(test_movementCompleted_movingNegativeNoToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingNegativeNoToleranceOnDestination_true);
    RUN_TEST(test_movementCompleted_movingPositiveWithToleranceBeforeDestination_false);
    RUN_TEST(test_movementCompleted_movingPositiveWithToleranceOnDestinationLowerBoundary_true);
    RUN_TEST(test_movementCompleted_movingPositiveWithToleranceAfterDestination_true);
    RUN_TEST(test_movementCompleted_movingPositiveWithToleranceOnDestinationUpperBound_false);
    RUN_TEST(test_movementCompleted_movingNegativeWithToleranceBeforeDestination_true);
    RUN_TEST(test_movementCompleted_movingNegativeWithToleranceOnDestinationLowerBoundary_true);
    RUN_TEST(test_movementCompleted_movingNegativeWithToleranceAfterDestination_false);
    RUN_TEST(test_movementCompleted_movingNegativeWithToleranceOnDestinationUpperBound_false);

    RUN_TEST(test_onBoundary_movingPositiveNoToleranceBeforeDestination_false);
    RUN_TEST(test_onBoundary_movingPositiveNoToleranceOnDestination_true);
    RUN_TEST(test_onBoundary_movingNegativeNoToleranceBeforeDestination_false);
    RUN_TEST(test_onBoundary_movingNegativeNoToleranceOnDestination_true);
    RUN_TEST(test_onBoundary_movingPositiveWithToleranceBeforeDestination_false);
    RUN_TEST(test_onBoundary_movingPositiveWithToleranceOnDestinationLowerBoundary_true);
    RUN_TEST(test_onBoundary_movingPositiveWithToleranceAfterDestination_false);
    RUN_TEST(test_onBoundary_movingPositiveWithToleranceOnDestinationUpperBound_false);
    RUN_TEST(test_onBoundary_movingNegativeWithToleranceBeforeDestination_false);
    RUN_TEST(test_onBoundary_movingNegativeWithToleranceOnDestinationLowerBoundary_true);
    RUN_TEST(test_onBoundary_movingNegativeWithToleranceAfterDestination_false);
    RUN_TEST(test_onBoundary_movingNegativeWithToleranceOnDestinationUpperBound_false);

    RUN_TEST(test_ticksToBoundarySoFar_beforeStartOfEncoderTicksPositiveMovement_zero);
    RUN_TEST(test_ticksToBoundarySoFar_atStartOfEncoderTicksPositiveMovement_zero);
    RUN_TEST(test_ticksToBoundarySoFar_oneAboveStartOfEncoderTicksPositiveMovement_one);
    RUN_TEST(test_ticksToBoundarySoFar_twoAboveStartOfEncoderTicksPositiveMovement_two);
    RUN_TEST(test_ticksToBoundarySoFar_oneBeforeEndOfEncoderTicksPositiveMovement_four);
    RUN_TEST(test_ticksToBoundarySoFar_atEndOfEncoderTicksPositiveMovement_max);
    RUN_TEST(test_ticksToBoundarySoFar_beyondEncoderTicksPositiveMovement_max);
    RUN_TEST(test_ticksToBoundarySoFar_beforeStartOfEncoderTicksNegativeMovement_zero);
    RUN_TEST(test_ticksToBoundarySoFar_atStartOfEncoderTicksNegativeMovement_zero);
    RUN_TEST(test_ticksToBoundarySoFar_oneAboveStartOfEncoderTicksNegativeMovement_one);
    RUN_TEST(test_ticksToBoundarySoFar_twoAboveStartOfEncoderTicksNegativeMovement_two);
    RUN_TEST(test_ticksToBoundarySoFar_oneBeforeEndOfEncoderTicksNegativeMovement_four);
    RUN_TEST(test_ticksToBoundarySoFar_atEndOfEncoderTicksNegativeMovement_min);
    RUN_TEST(test_ticksToBoundarySoFar_beyondEncoderTicksNegativeMovement_min);

    return UNITY_END();
}

void setup() {
    delay(2000);

    runUnityTests();
}

void loop() { /* do nothing */}