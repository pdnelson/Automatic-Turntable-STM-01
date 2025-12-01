#include "unity.h"
#include <InputMux.h>

#define MUX_A 3
#define MUX_B 4
#define MUX_C 5
#define MUX_D 6
#define MUX_OUTPUT 7

#define BUTTON_HOLD_INTERVAL_MS 2000
#define BUTTON_HOLD_INTERVAL_MICROS BUTTON_HOLD_INTERVAL_MS * 1000

#define DEBOUNCE_INTERVAL_MS 20
#define DEBOUNCE_INTERVAL_MICROS DEBOUNCE_INTERVAL_MS * 1000

#define PROP_DELAY 10

void setUp() {
    digitalWrite(MUX_A, LOW);
    digitalWrite(MUX_B, LOW);
    digitalWrite(MUX_C, LOW);
    digitalWrite(MUX_D, LOW);
    digitalWrite(MUX_OUTPUT, LOW);
}

/* Begin selector tests */

void test_inputMux_1ChConstructor_ALow() {
    digitalWrite(MUX_A, HIGH);
    digitalWrite(MUX_B, HIGH);
    digitalWrite(MUX_C, HIGH);
    digitalWrite(MUX_D, HIGH);
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));

    // Unused selects should remain untouched
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_2ChConstructor_ABLow() {
    digitalWrite(MUX_A, HIGH);
    digitalWrite(MUX_B, HIGH);
    digitalWrite(MUX_C, HIGH);
    digitalWrite(MUX_D, HIGH);
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

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
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

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
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_D, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_firstIterationBeforePropDelay_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    testMux.monitor(0);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_secondIterationBeforePropDelay_muxAHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    testMux.monitor(PROP_DELAY);
    testMux.monitor(PROP_DELAY * 2 - 1);

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_thirdIterationBeforePropDelay_muxBHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    testMux.monitor(PROP_DELAY);
    testMux.monitor(PROP_DELAY * 2);
    testMux.monitor(PROP_DELAY * 3 - 1);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_clockOverflow_nextSelectPinsAllHigh() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);
    
    // Normal iteration
    testMux.monitor(PROP_DELAY);

    // Jump to right before overflow is about to occur. Adding PROP_DELAY to this value will overflow.
    unsigned long almostOverflowed = ULONG_MAX - (PROP_DELAY / 2);
    testMux.monitor(almostOverflowed);

    // Overflow
    unsigned long overflowed = almostOverflowed + PROP_DELAY;
    testMux.monitor(overflowed);

    unsigned long expectedOverflowedValue = (PROP_DELAY / 2) - 1;

    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(expectedOverflowedValue, overflowed);
}

void test_inputMux_2ChSixteenIterations_selectPinsAlignWithBinaryValues() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    testMux.monitor(PROP_DELAY);        // 01
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 2);    // 10
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 3);    // 11
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 4);    // 00
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 5);    // 01
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 6);    // 10
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 7);    // 11
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 8);    // 00
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 9);        // 01
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 10);    // 10
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 11);    // 11
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 12);    // 00
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 13);    // 01
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 14);    // 10
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 15);    // 11
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 16);    // 00
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    // C and D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_3ChSixteenIterations_selectPinsAlignWithBinaryValues() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    testMux.monitor(PROP_DELAY);        // 001
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 2);    // 010
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 3);    // 011
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 4);    // 100
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 5);    // 101
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 6);    // 110
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 7);    // 111
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 8);    // 000
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 9);     // 001
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 10);    // 010
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 11);    // 011
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 12);    // 100
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 13);    // 101
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 14);    // 110
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 15);    // 111
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 16);    // 000
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    // D should remain untouched
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

void test_inputMux_4ChSixteenIterations_selectPinsAlignWithBinaryValues() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_D, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    testMux.monitor(PROP_DELAY);        // 0001
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 2);    // 0010
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 3);    // 0011
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 4);    // 0100
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 5);    // 0101
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 6);    // 0110
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 7);    // 0111
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 8);    // 1000
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 9);     // 1001
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 10);    // 1010
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 11);    // 1011
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 12);    // 1100
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 13);    // 1101
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 14);    // 1110
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 15);    // 1111
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(HIGH, digitalRead(MUX_D));

    testMux.monitor(PROP_DELAY * 16);    // 0000
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

    testMux.releaseMemory();
}

/* End selector tests */

/* Begin value tests */

void test_inputMux_justInitialized_released() {
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    ButtonResult result = testMux.getValue(0);

    TEST_ASSERT_EQUAL(ButtonResult::Released, result);

    testMux.releaseMemory();
}

void test_inputMux_transitioningFromLowToHighBeforeDebounce_released() {
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    digitalWrite(MUX_OUTPUT, HIGH);
    testMux.monitor(DEBOUNCE_INTERVAL_MICROS - 1);

    ButtonResult result = testMux.getValue(0);

    TEST_ASSERT_EQUAL(ButtonResult::Released, result);

    testMux.releaseMemory();
}

void test_inputMux_transitioningFromLowToHighAfterDebounce_onPress() {
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    digitalWrite(MUX_OUTPUT, HIGH);
    testMux.monitor(DEBOUNCE_INTERVAL_MICROS);

    ButtonResult result = testMux.getValue(0);

    TEST_ASSERT_EQUAL(ButtonResult::OnPress, result);

    testMux.releaseMemory();
}

void test_inputMux_statusOnPressNextIterationStillHigh_pressed() {
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    unsigned long totalClock = 0;

    // Set index 0 to OnPress
    digitalWrite(MUX_OUTPUT, HIGH);
    totalClock += DEBOUNCE_INTERVAL_MICROS;
    testMux.monitor(totalClock);

    // Set index 0 to Pressed
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);

    ButtonResult result = testMux.getValue(0);

    TEST_ASSERT_EQUAL(ButtonResult::Pressed, result);

    testMux.releaseMemory();
}

void test_inputMux_buttonHeldHighJustUnderHoldInterval_pressed() {
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    unsigned long totalClock = 0;
    unsigned long pressStartTime = DEBOUNCE_INTERVAL_MICROS;

    // Set index 0 to OnPress
    digitalWrite(MUX_OUTPUT, HIGH);
    totalClock += DEBOUNCE_INTERVAL_MICROS;
    testMux.monitor(totalClock);

    // Pass index 1
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);

    // Set index 0 to just below the hold time
    totalClock += (BUTTON_HOLD_INTERVAL_MICROS - (totalClock - pressStartTime)) - 1;
    testMux.monitor(totalClock);

    ButtonResult result = testMux.getValue(0);

    TEST_ASSERT_EQUAL(ButtonResult::Pressed, result);

    testMux.releaseMemory();
}

void test_inputMux_buttonHeldHighForHoldInterval_held() {
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    unsigned long totalClock = 0;
    unsigned long pressStartTime = DEBOUNCE_INTERVAL_MICROS;

    // Set index 0 to OnPress
    digitalWrite(MUX_OUTPUT, HIGH);
    totalClock += DEBOUNCE_INTERVAL_MICROS;
    testMux.monitor(totalClock);

    // Pass index 1
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);

    // Set index 0 to the hold time
    totalClock += BUTTON_HOLD_INTERVAL_MICROS - (totalClock - pressStartTime);
    testMux.monitor(totalClock);

    ButtonResult result = testMux.getValue(0);

    TEST_ASSERT_EQUAL(ButtonResult::Held, result);

    testMux.releaseMemory();
}

void test_inputMux_fullButtonCycleFromReleasedToHeldToReleased_transitionFromReleasedToOnPressToPressedToHeldToReleased() {
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    unsigned long totalClock = 0;
    unsigned long pressStartTime = DEBOUNCE_INTERVAL_MICROS;

    // Set index 0 to OnPress
    digitalWrite(MUX_OUTPUT, HIGH);
    totalClock += DEBOUNCE_INTERVAL_MICROS;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::OnPress, testMux.getValue(0));
    
    // Set index 0 to Pressed
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::Pressed, testMux.getValue(0));

    // Set index 0 to just below the hold time
    totalClock += (BUTTON_HOLD_INTERVAL_MICROS - (totalClock - pressStartTime)) - 1;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::Pressed, testMux.getValue(0));

    // Pass index 1
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);

    // Set index 0 to the hold time
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::Held, testMux.getValue(0));

    // Pass index 1
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);

    // Index 0 goes straight from held to released (to not double-execute events)
    digitalWrite(MUX_OUTPUT, LOW);
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::Released, testMux.getValue(0));

    testMux.releaseMemory();
}

void test_inputMux_fullButtonCycleFromReleasedToPressedToReleased_transitionFromReleasedToOnPressToPressedToOnReleasedToReleased() {
    InputMux testMux = InputMux(MUX_A, MUX_OUTPUT, PROP_DELAY, BUTTON_HOLD_INTERVAL_MS, DEBOUNCE_INTERVAL_MS);

    unsigned long totalClock = 0;
    unsigned long pressStartTime = DEBOUNCE_INTERVAL_MICROS;

    // Set index 0 to OnPress
    digitalWrite(MUX_OUTPUT, HIGH);
    totalClock += DEBOUNCE_INTERVAL_MICROS;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::OnPress, testMux.getValue(0));

    // After 1 loop cycle, index 0 is now pressed
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::Pressed, testMux.getValue(0));

    // Set index 0 to Pressed
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);

    // Pass index 1
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);

    // Set index 0 to just below the hold time
    // 2 prop delay is to account for the next two cycles
    totalClock += (BUTTON_HOLD_INTERVAL_MICROS - (totalClock - pressStartTime)) - (PROP_DELAY * 2) - 1;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::Pressed, testMux.getValue(0));

    // Pass index 1
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);

    // Index 0 is released now, so status becomes OnRelease because it was previously Pressed
    digitalWrite(MUX_OUTPUT, LOW);
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::OnRelease, testMux.getValue(0));

    // After one cycle index 0 is now released
    totalClock += PROP_DELAY;
    testMux.monitor(totalClock);
    TEST_ASSERT_EQUAL(ButtonResult::Released, testMux.getValue(0));

    testMux.releaseMemory();
}

/* End value tests */

int runUnityTests() {
    UNITY_BEGIN();

    // Selector Tests
    RUN_TEST(test_inputMux_1ChConstructor_ALow);
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

    // Value read tests
    RUN_TEST(test_inputMux_justInitialized_released);
    RUN_TEST(test_inputMux_transitioningFromLowToHighBeforeDebounce_released);
    RUN_TEST(test_inputMux_transitioningFromLowToHighAfterDebounce_onPress);
    RUN_TEST(test_inputMux_statusOnPressNextIterationStillHigh_pressed);
    RUN_TEST(test_inputMux_buttonHeldHighJustUnderHoldInterval_pressed);
    RUN_TEST(test_inputMux_buttonHeldHighForHoldInterval_held);
    RUN_TEST(test_inputMux_fullButtonCycleFromReleasedToHeldToReleased_transitionFromReleasedToOnPressToPressedToHeldToReleased);
    RUN_TEST(test_inputMux_fullButtonCycleFromReleasedToPressedToReleased_transitionFromReleasedToOnPressToPressedToOnReleasedToReleased);

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