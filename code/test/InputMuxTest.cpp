#include "unity.h"
#include <InputMux.h>

#define MUX_A 3
#define MUX_B 4
#define MUX_C 5
#define MUX_D 6
#define MUX_OUTPUT 7

#define DEFAULT_BUTTON_HOLD_INTERVAL 2000
#define DEFAULT_DEBOUNCE_INTERVAL 20
#define DEFAULT_PROP_DELAY 10

void setUp() {

}

void tearDown() {

}

/* 2-channel select tests start */

void test_inputMux_2ChConstructor_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChFirstIterationBeforePropDelay_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(0);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChFirstIterationAfterPropDelay_muxAHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChSecondIterationBeforePropDelay_muxAHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2 - 1);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChSecondIterationAfterPropDelay_muxBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChThirdIterationBeforePropDelay_muxBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3 - 1);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChThirdIterationAfterPropDelay_muxAAndBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChFourthIterationBeforePropDelay_muxAAndBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4 - 1);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChFourthIterationAfterPropDelay_muxAAndBLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_2ChClockOverflow_nextSelectPinsAllHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);
    
    // Normal iteration
    testMux.monitor(DEFAULT_PROP_DELAY);

    // Jump to right before overflow is about to occur. Adding DEFAULT_PROP_DELAY to this value will overflow.
    unsigned long almostOverflowed = ULONG_MAX - (DEFAULT_PROP_DELAY / 2);
    testMux.monitor(almostOverflowed);

    // Overflow
    unsigned long overflowed = almostOverflowed + DEFAULT_PROP_DELAY;
    testMux.monitor(overflowed);

    unsigned long expectedOverflowedValue = (DEFAULT_PROP_DELAY / 2) - 1;

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(expectedOverflowedValue, overflowed);
}

/* 2-channel select tests end */

/* 3-channel select tests start */

void test_inputMux_3ChConstructor_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChFirstIterationBeforePropDelay_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(0);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChFirstIterationAfterPropDelay_muxAHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChSecondIterationBeforePropDelay_muxAHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2 - 1);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChSecondIterationAfterPropDelay_muxBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChThirdIterationBeforePropDelay_muxBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3 - 1);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChThirdIterationAfterPropDelay_muxAAndBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChFourthIterationBeforePropDelay_muxAAndBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4 - 1);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChFourthIterationAfterPropDelay_muxCHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChFifthIterationBeforePropDelay_muxCHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);
    testMux.monitor(DEFAULT_PROP_DELAY * 5 - 1);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChFifthIterationAfterPropDelay_muxAAndCHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);
    testMux.monitor(DEFAULT_PROP_DELAY * 5);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChSixthIterationBeforePropDelay_muxAAndCHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);
    testMux.monitor(DEFAULT_PROP_DELAY * 5);
    testMux.monitor(DEFAULT_PROP_DELAY * 6 - 1);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChSixthIterationAfterPropDelay_muxBAndCHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);
    testMux.monitor(DEFAULT_PROP_DELAY * 5);
    testMux.monitor(DEFAULT_PROP_DELAY * 6);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChSeventhIterationBeforePropDelay_muxBAndCHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);
    testMux.monitor(DEFAULT_PROP_DELAY * 5);
    testMux.monitor(DEFAULT_PROP_DELAY * 6);
    testMux.monitor(DEFAULT_PROP_DELAY * 7 - 1);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChSeventhIterationAfterPropDelay_allSelectHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);
    testMux.monitor(DEFAULT_PROP_DELAY * 5);
    testMux.monitor(DEFAULT_PROP_DELAY * 6);
    testMux.monitor(DEFAULT_PROP_DELAY * 7);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChEighthIterationBeforePropDelay_allSelectHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);
    testMux.monitor(DEFAULT_PROP_DELAY * 5);
    testMux.monitor(DEFAULT_PROP_DELAY * 6);
    testMux.monitor(DEFAULT_PROP_DELAY * 7);
    testMux.monitor(DEFAULT_PROP_DELAY * 8 - 1);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChEighthIterationAfterPropDelay_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3);
    testMux.monitor(DEFAULT_PROP_DELAY * 4);
    testMux.monitor(DEFAULT_PROP_DELAY * 5);
    testMux.monitor(DEFAULT_PROP_DELAY * 6);
    testMux.monitor(DEFAULT_PROP_DELAY * 7);
    testMux.monitor(DEFAULT_PROP_DELAY * 8);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_3ChClockOverflow_nextSelectPinsAAndBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);
    
    // Normal iteration
    testMux.monitor(DEFAULT_PROP_DELAY);

    // Jump to right before overflow is about to occur. Adding DEFAULT_PROP_DELAY to this value will overflow.
    unsigned long almostOverflowed = ULONG_MAX - (DEFAULT_PROP_DELAY / 2);
    testMux.monitor(almostOverflowed);

    // Overflow
    unsigned long overflowed = almostOverflowed + DEFAULT_PROP_DELAY;
    testMux.monitor(overflowed);

    unsigned long expectedOverflowedValue = (DEFAULT_PROP_DELAY / 2) - 1;

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(expectedOverflowedValue, overflowed);
}

/* 3-channel select tests end */

/* 4-channel select tests start */

void test_inputMux_4ChConstructor_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_D, MUX_OUTPUT, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

/* 4-channel select tests end */

int runUnityTests() {
    UNITY_BEGIN();

    // 2-channel select tests
    RUN_TEST(test_inputMux_2ChConstructor_allSelectLow);
    RUN_TEST(test_inputMux_2ChFirstIterationBeforePropDelay_allSelectLow);
    RUN_TEST(test_inputMux_2ChFirstIterationAfterPropDelay_muxAHigh);
    RUN_TEST(test_inputMux_2ChSecondIterationBeforePropDelay_muxAHigh);
    RUN_TEST(test_inputMux_2ChSecondIterationAfterPropDelay_muxBHigh);
    RUN_TEST(test_inputMux_2ChThirdIterationBeforePropDelay_muxBHigh);
    RUN_TEST(test_inputMux_2ChThirdIterationAfterPropDelay_muxAAndBHigh);
    RUN_TEST(test_inputMux_2ChFourthIterationBeforePropDelay_muxAAndBHigh);
    RUN_TEST(test_inputMux_2ChFourthIterationAfterPropDelay_muxAAndBLow);
    RUN_TEST(test_inputMux_2ChClockOverflow_nextSelectPinsAllHigh);

    // 3-channel select tests
    RUN_TEST(test_inputMux_3ChConstructor_allSelectLow);
    RUN_TEST(test_inputMux_3ChFirstIterationBeforePropDelay_allSelectLow);
    RUN_TEST(test_inputMux_3ChFirstIterationAfterPropDelay_muxAHigh);
    RUN_TEST(test_inputMux_3ChSecondIterationBeforePropDelay_muxAHigh);
    RUN_TEST(test_inputMux_3ChSecondIterationAfterPropDelay_muxBHigh);
    RUN_TEST(test_inputMux_3ChThirdIterationBeforePropDelay_muxBHigh);
    RUN_TEST(test_inputMux_3ChThirdIterationAfterPropDelay_muxAAndBHigh);
    RUN_TEST(test_inputMux_3ChFourthIterationBeforePropDelay_muxAAndBHigh);
    RUN_TEST(test_inputMux_3ChFourthIterationAfterPropDelay_muxCHigh);
    RUN_TEST(test_inputMux_3ChFifthIterationBeforePropDelay_muxCHigh);
    RUN_TEST(test_inputMux_3ChFifthIterationAfterPropDelay_muxAAndCHigh);
    RUN_TEST(test_inputMux_3ChSixthIterationBeforePropDelay_muxAAndCHigh);
    RUN_TEST(test_inputMux_3ChSixthIterationAfterPropDelay_muxBAndCHigh);
    RUN_TEST(test_inputMux_3ChSeventhIterationBeforePropDelay_muxBAndCHigh);
    RUN_TEST(test_inputMux_3ChSeventhIterationAfterPropDelay_allSelectHigh);
    RUN_TEST(test_inputMux_3ChEighthIterationBeforePropDelay_allSelectHigh);
    RUN_TEST(test_inputMux_3ChEighthIterationAfterPropDelay_allSelectLow);
    RUN_TEST(test_inputMux_3ChClockOverflow_nextSelectPinsAAndBHigh);

    // 4-channel select tests
    RUN_TEST(test_inputMux_4ChConstructor_allSelectLow);
    return UNITY_END();
}

void setup() {
    delay(2000);

    pinMode(MUX_A, OUTPUT);
    pinMode(MUX_B, OUTPUT);
    pinMode(MUX_C, OUTPUT);
    pinMode(MUX_D, OUTPUT);
    pinMode(MUX_OUTPUT, OUTPUT);

    runUnityTests();
}

void loop() { /* do nothing */ }