#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include <Stepper.h>
#include <AccelStepper.h>

#include "statemachine.hpp"
#include "debounceButton.hpp"

// #define STEP_WITHOUT_LIB 
#define STEP_WITH_ACCEL 
// #define STEP_WITH_BUILTIN_LIB 
// #define PUSH_BUTTON 

const int stepPin = 8;
const int pulsePin = stepPin;
const int dirPin = 9;
const int cwButton = 2; // Pin 2
const int ccwButton = 3; // Pin 3
const int ledPin = LED_BUILTIN;
// const int enPin = 8;

// /**
//  * @brief Global variables section.
//  * 
//  */
// int stepCount = 0; // number of steps the motor has taken
// int labNumber = 0;
// int switchEvent = 0;            //
// bool buttonState = NOT_PRESSED; // button state initiilze.
// int labState = NOT_RUNNING;
// int potVal = 0;
// int timePeriod = 500;
// unsigned long timeStamp = 0;
// int previous = 0; // the previous reading from the analog input

void lab_1(void);
void lab_2(void);
void lab_3(void);
void lab_4(void);
void lab_5(void);

#endif