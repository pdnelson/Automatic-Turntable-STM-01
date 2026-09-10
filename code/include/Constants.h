#ifndef CONSTANTS_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_PATCH 1

#define SERIAL_COMMAND_INIT_KEY 0b00001100
#define SERIAL_COMMAND_MODEL_KEY 0b01101101 // 109 in decimal; 'm' in ASCII
#define SERIAL_COMMAND_CONNECTION_SUCCESS 0b10010010

#define SERIAL_ADVANCED_START_KEY 0b01010111
#define SERIAL_ADVANCED_END_KEY 0b10101000

// The speed at which serial devices should communicate with this turntable.
#define SERIAL_SPEED 115200

// The amount of time, in milliseconds, that a mux button must be held for the `ButtonResult::Held` status to be triggered.
#define BUTTON_HOLD_INTERVAL 2000

// The debounce interval, in milliseconds, for a mux button. I.e. the button cannot be pressed/released more than once within this duration.
#define BUTTON_DEBOUNCE_INTERVAL 20

// The interval, in microseconds, at which to poll the input multiplexer.
#define MUX_POLL_INTERVAL 100

// One seconds, in microseconds.
#define ONE_SECOND_MICROS 1000000

/**
 * Constants related to the pause/unpause commands -------------
 */

// A margin of error for the slide potentiometer "encoders."
#define VERTICAL_ENCODER_TOLERANCE 5

// The number of encoder ticks to move the tonearm down after contact with the lift has been terminated.
#define TICKS_BELOW_RECORD 15 // Set to 0 because the debounce ends up doing this well enough for now (or forever, maybe)

// The pause status is determined by the upper limit, and this is the margin of error for what can be considered "paused"
#define PAUSE_ERROR 50

// The amount of time, in microseconds, that must pass between status changes of the lift.
#define LIFT_DEBOUNCE_MICROS 50000

// The amount of time, in microseconds, that must pass between status changes of the home stand.
#define HOME_DEBOUNCE_MICROS 100000

// The number of steps the vertical stepper must reach before it is considered "stalled".
#define VERTICAL_STALL_STEPS 100

// If the tonearm is hovering over the "home" position, it should go down quickly, as opposed to slowly. This threshold
// helps determine if it's over "home".
#define VERTICAL_HOME_THRESHOLD 100

// When the tonearm lifts up toward the center of the platter, it might bounce a little bit. This timeout represents the maximum
// amount of time we allow for the tonearm to bounce, before it should level out.
#define LIFT_BOUNCE_TIMEOUT_MICROS 1500000

// How fast the tonearm should move when lifting.
#define LIFT_UP_SPEED 10

// How fast the tonearm should move when being set down gently.
#define SET_DOWN_SLOWLY 3

// How fast the tonearm should move when being set down quickly.
#define SET_DOWN_QUICKLY 14

/**
 * Constants related to the horizontal movement -----------
 */

// The RPM at which the clutch stepper spins.
#define CLUTCH_SPEED 14

// The number of steps for the clutch to engage, after the limit switch is released.
#define CLUTCH_ENGAGE_STEPS 250

// If the clutch switch doesn't release after this number of steps, the clutch movement failed.
// Similarly, if the clutch switch doesn't press after this value + CLUTCH_ENGAGE_STEPS, the clutch movement failed.
#define CLUTCH_TIMEOUT_STEPS 100

/**
 * Calibration constants
 */

// The number of encoder ticks above the upper threshold that the turntable should move. This value is applied at the time of calibration.
#define VERTICAL_BOUNDARY_UPPER_BUFFER 80

// The number of encoder ticks below the lower threshold that the turntable should move. This value is applied at the time of calibration.
#define VERTICAL_BOUNDARY_LOWER_BUFFER 30

// When zeroing the azimuth encoder, this is the INITIAL offset when beginning the calibration routine. This value is compatible with an incorrect polarity,
// because it's pretty close to the middle of both bounds.
#define HOME_ZERO_OFFSET 8000

/**
 * Below here are Serial aliases to make it easier to identify the STM-01's serial ports on the back in the code.
 */

#define STM_SERIAL_USB Serial
#define STM_SERIAL_1 Serial1
#define STM_SERIAL_2 Serial2
#define STM_SERIAL_3 Serial7
#define STM_SERIAL_4 Serial8
#define STM_SERIAL_5 Serial3

#endif