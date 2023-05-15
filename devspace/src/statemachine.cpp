#include "statemachine.hpp"

/**
 * @brief Global variables section.
 * 
 */
int stepCount = 0; // number of steps the motor has taken
static int labNumber = 0; // lab
int switchEvent = 0; // Event trigger 
bool buttonState = NOT_PRESSED; // button state initiilze.
int labState = NOT_RUNNING;
int potVal = 0;
int timePeriod = 500;
unsigned long timeStamp = 0;
int previous = 0; // the previous reading from the analog input

void checkButton(void)
{
    if (buttonState == NOT_PRESSED)
    {
        if (digitalRead(NEXT_STATE_PIN) == LOW)
        {
            //# Clicked !!
            Serial.println("buttonState = Pressed");
            delay(100);
            buttonState = PRESSED;
            digitalWrite(LED_BUILTIN, HIGH);
        }
    }
    else if (digitalRead(NEXT_STATE_PIN) == HIGH)
    {
        //# Released !
        Serial.println("buttonState = Un-Pressed");
        buttonState = NOT_PRESSED;
        switchEvent = 1;
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void nextState(void)
{
    switchEvent = 0;
    labNumber++;

    if (labNumber > 5)
    {
        labNumber = 1;
    }
}
