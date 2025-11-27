#include <InputMux.h>

InputMux::InputMux(uint8_t muxA, uint8_t muxB, uint8_t muxOutput, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval) {
    this->initializeBaseValues(muxA, muxB, muxC, muxD, muxOutput, 2, propagationDelay, holdInterval, debounceInterval);
    this->allocateMemory(4);
}

InputMux::InputMux(uint8_t muxA, uint8_t muxB, uint8_t muxC, uint8_t muxOutput, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval) {
    this->initializeBaseValues(muxA, muxB, muxC, muxD, muxOutput, 3, propagationDelay, holdInterval, debounceInterval);
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
    this->inputLastMs = (unsigned long*)malloc(arraySize * sizeof(unsigned long));
    for(i = 0; i < arraySize; i++) {
        this->inputLastMs[i] = 0;
    }

    // Initialize all last button status values to false
    this->inputLast = (boolean*)malloc(arraySize * sizeof(boolean));
    for(i = 0; i < arraySize; i++) {
        this->inputLast[i] = false;
    }

    // Initialize all current button status values to false
    this->inputCurrent = (boolean*)malloc(arraySize * sizeof(boolean));
    for(i = 0; i < arraySize; i++) {
        this->inputCurrent[i] = false;
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
    free(this->inputLastMs);
    free(this->inputLast);
    free(this->inputCurrent);
}

void InputMux::monitor(unsigned long clockMicros) {
    if(clockMicros - this->lastIterationClockMicros >= this->propagationDelay) {
        this->lastIterationClockMicros = clockMicros;

        // TODO: Do something with this value
        boolean result = digitalRead(this->muxOutput);

        // Increment index, or set it to 0 if we are at the last index
        if(this->currentIndex - 1 < this->arraySize) {
            this->currentIndex++;
        } else {
            this->currentIndex = 0;
        }

        digitalWrite(this->muxA, this->currentIndex & 1);
        digitalWrite(this->muxB, (this->currentIndex >> 1) & 1);
        
        if(this->selectorCount >= 3) {
            digitalWrite(this->muxC, (this->currentIndex >> 2) & 1);

            if(this->selectorCount >= 4) {
                digitalWrite(this->muxD, (this->currentIndex >> 3) & 1);
            }
        }
    }
}

ButtonResult InputMux::getValue(uint8_t index) {
    if(index < this->arraySize) {
        
    } else {
        return ButtonResult::Released;
    }

    if(this->inputCurrent && !this->inputLast) {
        return ButtonResult::Pressed;
    } else if(this->inputCurrent && this->inputLast) {
        // todo: held implementation
        return ButtonResult::Held;
    } else if(!this->inputCurrent && this->inputLast) {
        return ButtonResult::OnRelease;
    } else {
        return ButtonResult::Released;
    }
}