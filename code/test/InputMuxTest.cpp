#include "unity.h"
#include <InputMux.h>

#define MUX_A 3
#define MUX_B 4
#define MUX_C 5
#define MUX_D 6
#define MUX_OUTPUT 7

#define DEFAULT_BUTTON_HOLD_INTERVAL 2000
#define DEFAULT_DEBOUNCE_INTERVAL 20
#define DEFAULT_PROP_DELAY 5

void setUp() {

}

void tearDown() {

}

void test_inputMux_2ChConstructor_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_OUTPUT, DEFAULT_PROP_DELAY, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));

    testMux.releaseMemory();
}

void test_inputMux_3ChConstructor_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_OUTPUT, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));

    testMux.releaseMemory();
}

void test_inputMux_4ChConstructor_allSelectLow() {
    InputMux testMux = InputMux(MUX_A, MUX_B, MUX_C, MUX_D, MUX_OUTPUT, DEFAULT_BUTTON_HOLD_INTERVAL, DEFAULT_DEBOUNCE_INTERVAL);

    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_A));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_B));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_C));
    TEST_ASSERT_EQUAL(LOW, digitalRead(MUX_D));

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

int runUnityTests() {
    UNITY_BEGIN();
    RUN_TEST(test_inputMux_2ChConstructor_allSelectLow);
    RUN_TEST(test_inputMux_3ChConstructor_allSelectLow);
    RUN_TEST(test_inputMux_4ChConstructor_allSelectLow);
    RUN_TEST(test_inputMux_2ChFirstIterationBeforePropDelay_allSelectLow);
    RUN_TEST(test_inputMux_2ChFirstIterationAfterPropDelay_muxAHigh);
    RUN_TEST(test_inputMux_2ChSecondIterationBeforePropDelay_muxAHigh);
    RUN_TEST(test_inputMux_2ChSecondIterationAfterPropDelay_muxBHigh);
    RUN_TEST(test_inputMux_2ChThirdIterationBeforePropDelay_muxBHigh);
    RUN_TEST(test_inputMux_2ChThirdIterationAfterPropDelay_muxAAndBHigh);
    RUN_TEST(test_inputMux_2ChFourthIterationBeforePropDelay_muxAAndBHigh);
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