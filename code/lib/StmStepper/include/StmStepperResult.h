#ifndef STM_STEPPER_RESULT_H
#define STM_STEPPER_RESULT_H

struct StmStepperResult {
    bool movementCompleted = false;
    bool stepTaken = false;
};

#endif