#ifndef __DEBOUNCE_BUTTON_H__
#define __DEBOUNCE_BUTTON_H__

#include <Arduino.h>
#include "main.hpp"

// constants won't change. They're used here to set pin numbers:
extern const int buttonPin; // the number of the pushbutton pin
// extern const int ledPin;   // the number of the LED pin

// Variables will change:
extern int ledState;       // the current state of the output pin
extern int debouncebuttonState;           // the current reading from the input pin
extern int lastButtonState; // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
extern unsigned long lastDebounceTime; // the last time the output pin was toggled
extern unsigned long debounceDelay;   // the debounce time; increase if the output flickers

void buttonChecked();
void buttonChecked(uint8_t buttonPin);

#endif // __DEBOUNCE_BUTTON_H__