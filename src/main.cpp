/**
 * @file main.cpp
 * @author SquadQuiz(amornsakstp6@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-15
 * 
 * @copyright Copyright (c) 2023
 */

/**
 * @brief Import Arduino libraries.
 */
#include <Arduino.h>
#include <AccelStepper.h>

/**
 * @brief Private macros
 */
#define NOT_RUNNING 0
#define RUNNING 1
#define NOT_PRESSED 0
#define PRESSED 1
#define DBG(string) {Serial.println(string);}

/**
 * @brief Function Prototypes
 */
void checkButton(uint8_t buttonPin);
void nextLabState(void);
void programLab1(void);
void programLab2(void);

/**
 * @brief Global variables section.
 */
// ########## GPIO Pins Configuration ##########
const int NEXT_STATE_PIN = 2; 	// pin 2 go to next lab state.
const int BUTTON_A = 3;			// pin 3 button a, for lab trigger.
const int BUTTON_B = 4;			// pin 4 button b, for lab trigger.
const int BUTTON_C = 5;			// pin 5 button c, reserved.
const int stepPin = 8;			// pin 8 Connect to Pulse pin of Stepper/DC Driver.
const int pulsePin = stepPin;	// pin 8 Alias of stepPin.
const int dirPin = 9;			// pin 9 Connect to Direction pin of Stepper/DC Driver.
const int cwButton = BUTTON_A;	// Pin 2 Alias.
const int ccwButton = BUTTON_B; // Pin 3 Alias.
const int ledPin = LED_BUILTIN; // pin 13 Arduino Built-in LED.
const int airSensor = 12;		// pin 12 input of pressure sensor (Active Low)

// ########## Variables for Running Machine Lab State ##########
int labNumber = 0;				// lab running variable
int switchEvent = 0;			// Event trigger
bool buttonState = NOT_PRESSED; // button state initiilze.
int labState = NOT_RUNNING;		// Lab State status

// ########## Variables for debouncing a button ##########
//  Variables will change for debounce button:
int debouncebuttonState;   // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50;	// the debounce time; increase if the output flickers

// # Fixed Paremeters for auto-injection mode.
int time1 = 400;
int time2 = 20;
int speed1 = 1000;
int speed2 = 5000;
int pulseInterval = 400;

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

// # Injection interval level
enum
{
	INTERVAL_1 = 400,  // Injection interval 0.4 seconds
	INTERVAL_2 = 600,  // Injection interval 0.6 seconds
	INTERVAL_3 = 800,  // Injection interval 0.8 seconds
	INTERVAL_4 = 1000, // Injection interval 1.0 seconds
	INTERVAL_5 = 1200, // Injection interval 1.2 seconds
	INTERVAL_6 = 1400, // Injection interval 1.4 seconds
	INTERVAL_7 = 1600, // Injection interval 1.6 seconds
	INTERVAL_8 = 1800, // Injection interval 1.8 seconds
	INTERVAL_9 = 2000, // Injection interval 2.0 seconds
};

// # Injection speed level
enum
{
	INJECT_SPEED_1 = 1, // Linear Actuator Speed 0.45 mm/s, 45 Hz
	INJECT_SPEED_2 = 2, // Linear Actuator Speed 0.50 mm/s, 50 Hz
	INJECT_SPEED_3 = 3, // Linear Actuator Speed 0.58 mm/s, 58 Hz
	INJECT_SPEED_4 = 4, // Linear Actuator Speed 0.70 mm/s, 70 Hz
	INJECT_SPEED_5 = 5, // Linear Actuator Speed 0.80 mm/s, 80 Hz
	INJECT_SPEED_6 = 6, // Linear Actuator Speed 1.25 mm/s, 125 Hz
	INJECT_SPEED_7 = 7, // Linear Actuator Speed 1.50 mm/s, 150 Hz
	INJECT_SPEED_8 = 8, // Linear Actuator Speed 2.00 mm/s, 200 Hz
	INJECT_SPEED_9 = 9, // Linear Actuator Speed 3.00 mm/s, 300 Hz
};

// # Leak prevention level
enum 
{
	LEAK_PREVENT_1 = 1, // Plunger reverse time 100 ms
	LEAK_PREVENT_2 = 2, // Plunger reverse time 160 ms
	LEAK_PREVENT_3 = 3, // Plunger reverse time 230 ms
	LEAK_PREVENT_4 = 4, // Plunger reverse time 340 ms
	LEAK_PREVENT_5 = 5, // Plunger reverse time 440 ms
	LEAK_PREVENT_6 = 6, // Plunger reverse time 550 ms
};

// # Vacuum level
enum 
{
	VACUUM_THIN = 1, 	// Vacuum pressure 90~120 mmHg, 12~16 kPa
	VACUUM_GENERAL = 2, // Vacuum pressure 140~190 mmHg, 19~25 kPa
	VACUUM_THICK = 3,	// Vacuum pressure 210~300 mmHg, 28~40 kPa
	VACUMM_MORE = 4		// Vacuum pressure 370~480 mmHg, 49~65 kPa
};

void setup()
{
	Serial.begin(9600);

	// # initialize button switch:
	pinMode(NEXT_STATE_PIN, INPUT_PULLUP);
	pinMode(BUTTON_A, INPUT_PULLUP);
	pinMode(BUTTON_B, INPUT_PULLUP);
	pinMode(BUTTON_C, INPUT_PULLUP);

	// # initialize air pressure sensor:
	pinMode(airSensor, INPUT_PULLUP);

	// # initialize stepper motor:
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);

	// # Initialize LED built-in:
	pinMode(LED_BUILTIN, OUTPUT);

	// # Initialize Potentiometer:
	// pinMode(POT_PIN, INPUT);

	Serial.println("Program Starting");

	stepper.setMaxSpeed(1000);
	stepper.setSpeed(50);
}

void loop()
{
	// button if switch is short or pressed it will return HIGH or '1', otherwise will be LOW or '0'
	checkButton(NEXT_STATE_PIN);

	switch (labNumber)
	{
	case 1:
		programLab1();
		break;
	case 2:
		programLab2();
		break;
	default:
		break;
	}

	if (switchEvent)
	{
		nextLabState();
	}
	// stepper.runSpeed();
}

// Arduino Button Using Debouncing technique.
void checkButton(uint8_t buttonPin)
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

			// only trigger lab state if the new button state is LOW (Button Active-Low)
			if (debouncebuttonState == LOW) 
			{
				// Clicked !
				// Serial.println("Clicked");
				digitalWrite(ledPin, HIGH);
				switchEvent = 1;
			}
		}
	}

	// set the LED:
	digitalWrite(ledPin, LOW);

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonState = reading;
}

void nextLabState(void)
{
	switchEvent = 0;
	labNumber++;

	if (labNumber > 2)
	{
		labNumber = 1;
	}
}

void programLab1(void)
{
	if (labState == NOT_RUNNING)
	{
		Serial.println("Lab 1: ");
		labState = RUNNING;
	}
	if (labState == RUNNING)
	{
		if (digitalRead(cwButton) == LOW)
		{
			// # Enable Direction CW
			digitalWrite(dirPin, HIGH);
			// # Generation Pulse
			digitalWrite(stepPin, HIGH);
			delayMicroseconds(100);
			digitalWrite(stepPin, LOW);
			delayMicroseconds(100);
		}
		else if (digitalRead(ccwButton) == LOW)
		{
            // # Enable Direction CCW
            digitalWrite(dirPin, LOW);
            // # Generation Pulse
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(500);
            digitalWrite(stepPin, LOW);
			delayMicroseconds(500);
		}
		// # Delay for interval pulse signal.
		// delay(1000);
		// delayMicroseconds(100);
	}
	if (switchEvent)
	{
		labState = NOT_RUNNING;
	}
}

void programLab2(void)
{
	if (labState == NOT_RUNNING)
	{
		Serial.println("Lab 2: ");
		labState = RUNNING;
	}
	if (labState == RUNNING)
	{
		if (digitalRead(airSensor) == LOW)
		// if (digitalRead(BUTTON_A) == LOW)
		{
            for(int i = 0; i < time1; i++)
			{
                // # Enable Direction CW
                digitalWrite(dirPin, HIGH);
                // # Generation Pulse
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(speed1);
                digitalWrite(stepPin, LOW);
				delayMicroseconds(speed1);
            }
			for(int i = 0; i < time2; i++)
			{
                // # Enable Direction CW
                digitalWrite(dirPin, LOW);
                // # Generation Pulse
                digitalWrite(stepPin, HIGH);
                delayMicroseconds(speed2);
                digitalWrite(stepPin, LOW);
				delayMicroseconds(speed2);
            }
		}
		else if (digitalRead(ccwButton) == LOW)
		{
			for(int i = 0; i < time2; i++)
			{
				// # Enable Direction CCW
				digitalWrite(dirPin, LOW);
				// # Generation Pulse
				digitalWrite(stepPin, HIGH);
				delayMicroseconds(speed2);
				digitalWrite(stepPin, LOW);
				delayMicroseconds(speed2);
			}
			//# Delay for interval pulse signal
		}
		delay(pulseInterval);
	}
	if (switchEvent)
	{
		labState = NOT_RUNNING;
	}
}

