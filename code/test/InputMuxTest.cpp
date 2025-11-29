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
    digitalWrite(MUX_A, LOW);
    digitalWrite(MUX_B, LOW);
    digitalWrite(MUX_C, LOW);
    digitalWrite(MUX_D, LOW);
    digitalWrite(MUX_OUTPUT, LOW);
}

void test_inputMux_2ChConstructor_ABLow() {
    digitalWrite(MUX_A, HIGH);
    digitalWrite(MUX_B, HIGH);
    digitalWrite(MUX_C, HIGH);
    digitalWrite(MUX_D, HIGH);
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // Unused selects should remain untouched
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_3ChConstructor_ABCLow() {
    digitalWrite(MUX_A, HIGH);
    digitalWrite(MUX_B, HIGH);
    digitalWrite(MUX_C, HIGH);
    digitalWrite(MUX_D, HIGH);
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // Unused selects should remain untouched
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_4ChConstructor_ABCDLow() {
    digitalWrite(MUX_A, HIGH);
    digitalWrite(MUX_B, HIGH);
    digitalWrite(MUX_C, HIGH);
    digitalWrite(MUX_D, HIGH);
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_D, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_firstIterationBeforePropDelay_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(0);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_secondIterationBeforePropDelay_muxAHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2 - 1);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_thirdIterationBeforePropDelay_muxBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);
    testMux.monitor(DEFAULT_PROP_DELAY * 2);
    testMux.monitor(DEFAULT_PROP_DELAY * 3 - 1);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_clockOverflow_nextSelectPinsAllHigh() {
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

void test_inputMux_2ChSixteenIterations_selectPinsAlignWithBinaryValues() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);        // 01
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 2);    // 10
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 3);    // 11
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 4);    // 00
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 5);    // 01
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 6);    // 10
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 7);    // 11
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 8);    // 00
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 9);        // 01
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 10);    // 10
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 11);    // 11
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 12);    // 00
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 13);    // 01
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 14);    // 10
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 15);    // 11
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 16);    // 00
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_3ChSixteenIterations_selectPinsAlignWithBinaryValues() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);        // 001
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 2);    // 010
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 3);    // 011
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 4);    // 100
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 5);    // 101
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 6);    // 110
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 7);    // 111
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 8);    // 000
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 9);     // 001
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 10);    // 010
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 11);    // 011
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 12);    // 100
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 13);    // 101
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 14);    // 110
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 15);    // 111
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 16);    // 000
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_4ChSixteenIterations_selectPinsAlignWithBinaryValues() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_D, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    testMux.monitor(DEFAULT_PROP_DELAY);        // 0001
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 2);    // 0010
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 3);    // 0011
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 4);    // 0100
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 5);    // 0101
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 6);    // 0110
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 7);    // 0111
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 8);    // 1000
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 9);     // 1001
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 10);    // 1010
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 11);    // 1011
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 12);    // 1100
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 13);    // 1101
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 14);    // 1110
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 15);    // 1111
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(DEFAULT_PROP_DELAY * 16);    // 0000
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_inputMux_2ChConstructor_ABLow);
    RUN_TEST(test_inputMux_3ChConstructor_ABCLow);
    RUN_TEST(test_inputMux_4ChConstructor_ABCDLow);
    RUN_TEST(test_inputMux_firstIterationBeforePropDelay_allSelectLow);
    RUN_TEST(test_inputMux_secondIterationBeforePropDelay_muxAHigh);
    RUN_TEST(test_inputMux_thirdIterationBeforePropDelay_muxBHigh);
    RUN_TEST(test_inputMux_clockOverflow_nextSelectPinsAllHigh);
    RUN_TEST(test_inputMux_2ChSixteenIterations_selectPinsAlignWithBinaryValues);
    RUN_TEST(test_inputMux_3ChSixteenIterations_selectPinsAlignWithBinaryValues);
    RUN_TEST(test_inputMux_4ChSixteenIterations_selectPinsAlignWithBinaryValues);

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