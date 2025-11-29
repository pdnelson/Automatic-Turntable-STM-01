#include <InputMux.h>

InputMux::InputMux(uint8_t muxA, uint8_t muxOutput, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval) {
    this->initializeBaseValues(muxA, 0, 0, 0, muxOutput, 1, propagationDelay, holdInterval, debounceInterval);
    this->allocateMemory(2);
}

InputMux::InputMux(uint8_t muxA, uint8_t muxB, uint8_t muxOutput, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval) {
    this->initializeBaseValues(muxA, muxB, 0, 0, muxOutput, 2, propagationDelay, holdInterval, debounceInterval);
    this->allocateMemory(4);
}

InputMux::InputMux(uint8_t muxA, uint8_t muxB, uint8_t muxC, uint8_t muxOutput, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval) {
    this->initializeBaseValues(muxA, muxB, muxC, 0, muxOutput, 3, propagationDelay, holdInterval, debounceInterval);
    this->allocateMemory(8);
}

InputMux::InputMux(uint8_t muxA, uint8_t muxB, uint8_t muxC, uint8_t muxD, uint8_t muxOutput, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval) {
    this->initializeBaseValues(muxA, muxB, muxC, muxD, muxOutput, 4, propagationDelay, holdInterval, debounceInterval);
    this->allocateMemory(16);
}

void InputMux::allocateMemory(uint8_t arraySize) {
    this->arraySize = arraySize;

    int i;

    // Initialize all last button click MS values to 0
    this->inputPressMs = (unsigned long*)malloc(arraySize * sizeof(unsigned long));
    for(i = 0; i < arraySize; i++) {
        this->inputPressMs[i] = 0;
    }

    // Initialize all last button status values to false
    this->currentValue = (ButtonResult*)malloc(arraySize * sizeof(ButtonResult));
    for(i = 0; i < arraySize; i++) {
        this->currentValue[i] = ButtonResult::Released;
    }
}

void InputMux::initializeBaseValues(uint8_t muxA, uint8_t muxB, uint8_t muxC, uint8_t muxD, uint8_t muxOutput, uint8_t selectorCount, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval) {
    this->currentIndex = 0;
    this->lastIterationClockMicros = 0;
    this->selectorCount = selectorCount;
    this->muxOutput = muxOutput;
    this->propagationDelay = propagationDelay;
    this->holdInterval = holdInterval;
    this->debounceInterval = debounceInterval;

    this->muxA = muxA;
    digitalWrite(this->muxA, LOW);

    if(this->selectorCount >= 2) {
        this->muxB = muxB;
        digitalWrite(this->muxB, LOW);

        if(this->selectorCount >= 3) {
            this->muxC = muxC;
            digitalWrite(this->muxC, LOW);

            if(this->selectorCount >= 4) {
                this->muxD = muxD;
                digitalWrite(this->muxD, LOW);
            }
        }
    }
}

void InputMux::releaseMemory() {
    free(this->inputPressMs);
    free(this->currentValue);
}

void InputMux::monitor(unsigned long clockMicros) {
    if(clockMicros - this->lastIterationClockMicros >= this->propagationDelay) {
        this->lastIterationClockMicros = clockMicros;

        ButtonResult lastValue = getValue(this->currentIndex);

        // Only perform any "starting" operations if we are within the debounce interval
        if(clockMicros - this->inputPressMs[this->currentIndex] >= this->debounceInterval) {
            boolean result = digitalRead(this->muxOutput);

            if(result) {
                // If the button was last released, then change to OnPress
                if(lastValue == ButtonResult::Released) {
                    this->inputPressMs[this->currentIndex] = clockMicros;
                    setValue(this->currentIndex, ButtonResult::OnPress);
                } 
            } else {
                // If the button was last held, then go straight to Released, to prevent Held and OnReleased events
                // from both triggering.
                if(lastValue == ButtonResult::Held) {
                    this->inputPressMs[this->currentIndex] = clockMicros;
                    setValue(this->currentIndex, ButtonResult::Released);
                }

                // If the button was last pressed, or transitioning to pressed, then set it to
                // OnRelease
                else if(lastValue == ButtonResult::OnPress || lastValue == ButtonResult::Pressed) {
                    this->inputPressMs[this->currentIndex] = clockMicros;
                    setValue(this->currentIndex, ButtonResult::OnRelease);
                }
            }
        } 
        
        // Transitional operations (OnPress -> Pressed, OnRelease -> Released) do not depend on
        // the debounce interval; they only depend on the last status that was set, so they 
        // can run whenever.

        // If the button was last OnPress, then change it to Pressed
        if(lastValue == ButtonResult::OnPress) {
            setValue(this->currentIndex, ButtonResult::Pressed);
        } 
        
        // If the button was last Pressed, and the hold interval has elapsed, then change it to Held
        else if(lastValue == ButtonResult::Pressed && clockMicros - this->inputPressMs[this->currentIndex] >= this->holdInterval) {
            setValue(this->currentIndex, ButtonResult::Held);
        }

        // If the button was last OnRelease, then set it to Released.
        else if(lastValue == ButtonResult::OnRelease) {
            setValue(this->currentIndex, ButtonResult::Released);
        }

        // Increment index, or set it to 0 if we are at the last index
        if(this->currentIndex + 1 < this->arraySize) {
            this->currentIndex++;
        } else {
            this->currentIndex = 0;
        }

        digitalWrite(this->muxA, this->currentIndex & 0x01);

        if(this->selectorCount >= 2) {
            digitalWrite(this->muxB, (this->currentIndex >> 1) & 0x01);
            
            if(this->selectorCount >= 3) {
                digitalWrite(this->muxC, (this->currentIndex >> 2) & 0x01);

                if(this->selectorCount >= 4) {
                    digitalWrite(this->muxD, (this->currentIndex >> 3) & 0x01);
                }
            }
        }
    }
}

ButtonResult InputMux::getValue(uint8_t index) {
    if(index < this->arraySize) {
        return this->currentValue[index];
    } else {
        return ButtonResult::Released;
    }
}

void InputMux::setValue(uint8_t index, ButtonResult value) {
    this->currentValue[index] = value;
}