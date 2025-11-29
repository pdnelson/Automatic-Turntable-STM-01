#ifndef BUTTONRESULT_H
#define BUTTONRESULT_H

enum ButtonResult : uint8_t {

    /**
     * The button was just released from the `Pressed` status. This will be set
     * for one mux monitor cycle before being set back to `Released`. If the status
     * was formerly `Held`, this status will not be set.
     */
    OnRelease = 0,
    
    /**
     * The button is not pressed. This status will be set after the status was previously OnRelease,
     * or by default, if the button has had no activity.
     */
    Released = 1,

    /**
     * The button was just pressed. This will be set for one mux monitor cycle before being set to `Pressed`.
     */
    OnPress = 2,

    /**
     * The button is pressed. This will be set for `holdInterval` milliseconds before the status
     * transitions to `Held`, after the status was `OnPress`.
     */
    Pressed = 3,

    /**
     * The button is held. This will be set after the button is held for `holdInterval`
     * milliseconds
     */
    Held = 4
};

#endif