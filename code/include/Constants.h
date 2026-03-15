#define SERIAL_SPEED 115200

// The amount of time, in milliseconds, that a mux button must be held for the `ButtonResult::Held` status to be triggered.
#define BUTTON_HOLD_INTERVAL 2000

// The debounce interval, in milliseconds, for a mux button. I.e. the button cannot be pressed/released more than once within this duration.
#define BUTTON_DEBOUNCE_INTERVAL 20

// The interval, in microseconds, at which to poll the input multiplexer.
#define MUX_POLL_INTERVAL 100

// The number of steps to make a full revolution on a 28BYJ-48 stepper motor.
#define STEPS_PER_REVOLUTION 2048

// One seconds, in microseconds.
#define ONE_SECOND_MICROS 1000000

/**
 * Constants related to the pause/unpause commands -------------
 */

// A margin of error for the slide potentiometer "encoders."
#define VERTICAL_ENCODER_DELTA 5

// The number of encoder ticks to move the tonearm down after contact with the lift has been terminated.
#define TICKS_BELOW_RECORD 10

// A hard-coded test value for the lower vertical encoder limit. This will eventually be replaced by a calibration value.
#define TEST_VERTICAL_LOWER_LIMIT 20

// A hard-coded test value for the upper vertical encoder limit. This will eventually be replaced by a calibration value.
#define TEST_VERTICAL_UPPER_LIMIT 1000

// The amount of time, in microseconds, that must pass between status changes of the lift.
#define LIFT_DEBOUNCE_MICROS 200000

// The amount of time, in microseconds, that must pass between status changes of the home stand.
#define HOME_DEBOUNCE_MICROS 200000

// The number of steps the vertical stepper must reach before it is considered "stalled".
#define VERTICAL_STALL_STEPS 50

/**
 * Below here are Serial aliases to make it easier to identify the STM-01's serial ports on the back in the code.
 */

#define STM_SERIAL_USB Serial
#define STM_SERIAL_1 Serial1
#define STM_SERIAL_2 Serial2
#define STM_SERIAL_3 Serial7
#define STM_SERIAL_4 Serial8
#define STM_SERIAL_5 Serial3

#define SERIAL_COMMAND_KEY 0b01101101 // 109 in decimal; 'm' in ASCII
#define SERIAL_COMMAND_CONNECTION_SUCCESS 0b10010010