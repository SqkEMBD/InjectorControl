#include "debounceButton.hpp"

void buttonChecked();

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2; // the number of the pushbutton pin
// const int ledPin = 13;   // the number of the LED pin

// Variables will change:
int ledState = HIGH;       // the current state of the output pin
int debouncebuttonState;           // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers

void buttonChecked(void)
{
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState)
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != debouncebuttonState)
        {
            debouncebuttonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (debouncebuttonState == HIGH)
            {
                // Clicked !
                // ledState = !ledState;
                // Serial.println("b");

                // Serial.println("buttonState = Pressed");
                // delay(100);
                buttonState = PRESSED;
                digitalWrite(ledPin, HIGH);
            }
            switchEvent = 1;
        }
    }

    // set the LED:
    digitalWrite(ledPin, HIGH);
    // Serial.println("c");

    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState = reading;
}

void buttonChecked(uint8_t buttonPin)
{
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState)
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != debouncebuttonState)
        {
            debouncebuttonState = reading;

            // only toggle the LED if the new button state is HIGH
            if (debouncebuttonState == HIGH)
            {
                // Clicked !
                // ledState = !ledState;
                // Serial.println("b");

                // delay(100);
                // buttonState = PRESSED;
                Serial.println("Clicked");
                digitalWrite(ledPin, HIGH);
                switchEvent = 1;
            }
        }
    }

    // set the LED:
    digitalWrite(ledPin, LOW);
    // Serial.println("c");

    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState = reading;
}