#ifndef BUTTONRESULT_H
#define BUTTONRESULT_H

enum ButtonResult : uint8_t {
    
    /**
     * The button is not pressed. This status will be set immediately upon
     * release of a button.
     */
    Released,

    /**
     * The button was just released from the `Pressed` status. This will be set
     * for one loop cycle before being set back to `Released`. If the status
     * was formerly `Held`, this status will not be set.
     */
    OnRelease,

    /**
     * The button is pressed. This will be set for `holdInterval` milliseconds before the status
     * transitions to `Held`.
     */
    Pressed,

    /**
     * The button is held. This will be set after the button is held for `holdInterval`
     * milliseconds
     */
    Held
};

#endif