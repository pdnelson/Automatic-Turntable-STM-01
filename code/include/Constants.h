#define SERIAL_SPEED 115200

// The amount of time, in milliseconds, that a mux button must be held for the `ButtonResult::Held` status to be triggered.
#define BUTTON_HOLD_INTERVAL 2000

// The debounce interval, in milliseconds, for a mux button. I.e. the button cannot be pressed/released more than once within this duration.
#define BUTTON_DEBOUNCE_INTERVAL 20

// The interval, in microseconds, at which to poll the input multiplexer.
#define MUX_POLL_INTERVAL 100

// The number of steps to make a full revolution on a 28BYJ-48 stepper motor.
#define STEPS_PER_REVOLUTION 2048

// A margin of error for the slide potentiometer "encoders."
#define ENCODER_DELTA 5

// The number of encoder ticks to move the tonearm down after contact with the lift has been terminated.
#define TICKS_BELOW_RECORD 100

// A hard-coded test value for the lower vertical encoder limit. This will eventually be replaced by a calibration value.
#define TEST_VERTICAL_LOWER_LIMIT 20

// A hard-coded test value for the upper vertical encoder limit. This will eventually be replaced by a calibration value.
#define TEST_VERTICAL_UPPER_LIMIT 1000