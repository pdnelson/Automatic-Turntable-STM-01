#include "unity.h"
#include <Arduino.h>

void test_context_condition_consequence() {
    
}

int runUnityTests() {
    UNITY_BEGIN();

    RUN_TEST(test_context_condition_consequence);

    return UNITY_END();
}

void setup() {
    delay(2000);

    runUnityTests();
}

void loop() { /* do nothing */}