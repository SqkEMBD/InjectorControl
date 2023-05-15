#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <Arduino.h>
#include "main.hpp"

#define NOT_RUNNING 0
#define RUNNING 1

//# GPIO
// leonado
#define NEXT_STATE_PIN PB2
#define BUTTON_A PD3
#define BUTTON_B PD4
#define BUTTON_C PD5

enum ButtonState
{
    NOT_PRESSED = 0,
    PRESSED,
};

/**
 * @brief Global variables section.
 * 
 */
extern int stepCount;
extern int labNumber;
extern int switchEvent;
extern bool buttonState;
extern int labState;
extern int potVal;
extern int timePeriod;
extern unsigned long timeStamp;
extern int previous;

void checkButton(void);
void nextState(void);

#endif // __STATE_MACHINE_H__