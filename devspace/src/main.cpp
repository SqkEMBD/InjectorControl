#include "main.hpp"

// ############## Stepping motor Without Library #################
#ifdef STEP_WITHOUT_LIB
void setup()
{
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	// pinMode(enPin,OUTPUT);
	// digitalWrite(enPin,LOW);
}

void loop()
{
	digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
	for (int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(500);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(500);
	}
	delay(1000); // One second delay

	digitalWrite(dirPin, LOW); // Changes the direction of rotation
	for (int x = 0; x < stepsPerRevolution; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(500);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(500);
	}
	delay(1000);
}

#endif

// ############## Stepping motor Using Accel Library #################
#ifdef STEP_WITH_ACCEL
// Define a stepper motor 1 for arduino
// direction Digital 9 (CW), pulses Digital 8 (CLK)
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);
// Stepper myStepper(2, );

int maxDistance = 24000;
int minDistance = 0;
int zeroPos = 0;

void setup()
{
	Serial.begin(9600);

	// # initialize button switch:
	pinMode(NEXT_STATE_PIN, INPUT_PULLUP);
	pinMode(BUTTON_A, INPUT_PULLUP);
	pinMode(BUTTON_B, INPUT_PULLUP);
	pinMode(BUTTON_C, INPUT_PULLUP);

	// # Initialize LED built-in:
	pinMode(LED_BUILTIN, OUTPUT);

	// # Initialize Potentiometer:
	//  pinMode(POT_PIN, INPUT);

	// # Initialize Stepper motor:
	//  Change these to suit your stepper if you want
	stepper.setMaxSpeed(1000.0); // 1100
	stepper.setAcceleration(1100.0);

	// Here you could put a homing sequence:
	stepper.setCurrentPosition(0);

	// motor 1.8 deg
	// driver 1.8/2 deg
	// travel length = 120 mm
	// lead screw 2 mm/revolution
	// start to end = 60 revs
	// 1 rev = 360 deg
	// 360/0.9 = 400 step/rev
	// 400 steps/rev x 60 rev = 24,000 step
	// stepper.moveTo(maxDistance); //

	// Serial.println("Program Starting");
}

void loop()
{

	// button if switch is short or pressed it will return HIGH or '1', otherwise will be LOW or '0'
	
	// checkButton();
	buttonChecked(NEXT_STATE_PIN);

	switch (labNumber)
	{
	case 0:
		// do nothing.....
		break;
	case 1:
		lab_1();
		// Serial.println("Lab 1");
		break;
	case 2:
		lab_2();
		break;
	case 3:
		lab_3();
		break;
	case 4:
		lab_4();
		break;
	case 5:
		lab_5();
		break;
	default:
		break;
	}

	if (switchEvent)
	{
		nextState();
	}
}

#endif

void lab_1(void)
{
	if (labState == NOT_RUNNING)
	{
		Serial.println("Lab 1: ");
		labState = RUNNING;
		stepper.moveTo(maxDistance); //
	}
	if (labState == RUNNING)
	{
		if (digitalRead(BUTTON_A) == LOW)
		{
			// Serial.println("labNumber 1: ");
			// stepper.moveTo(maxDistance); //
			// If at the end of travel go to the other end
			if (stepper.distanceToGo() == 0)
			{
				// stepper.moveTo(-stepper.currentPosition());
				stepper.moveTo(zeroPos); //
				// Serial.println("Move Complete");
			}
		}
		stepper.run();
	}
	if (switchEvent)
	{
		labState = NOT_RUNNING;
	}
}

void lab_2(void)
{
	if (labState == NOT_RUNNING)
	{
		Serial.println("Lab 2: ");
		labState = RUNNING;
	}
	if (labState == RUNNING)
	{
		if (digitalRead(BUTTON_A) == LOW)
		{
			// # Enable Direction CW
			digitalWrite(dirPin, HIGH);
			// # Generation Pulse
			digitalWrite(stepPin, HIGH);
			delayMicroseconds(500);
			digitalWrite(stepPin, LOW);
		}
		else if (digitalRead(BUTTON_B) == LOW)
		{
			// # Enable Direction CCW
			digitalWrite(dirPin, LOW);
			// # Generation Pulse
			digitalWrite(stepPin, HIGH);
			delayMicroseconds(500);
			digitalWrite(stepPin, LOW);
		}
		// # Delay for interval pulse signal.
		delayMicroseconds(100);
	}
	if (switchEvent)
	{
		labState = NOT_RUNNING;
	}
}

void lab_3(void)
{
	int state = 0;

	if (labState == NOT_RUNNING)
	{
		Serial.println("Lab 3: ");
		labState = RUNNING;
	}
	if (labState == RUNNING)
	{
		if (state == NOT_PRESSED)
		{
			if (digitalRead(BUTTON_C) == LOW)
			{
				Serial.println("Set Current position = 0");
				stepper.setCurrentPosition(0);
				delay(100);
				state = 1;
			}
			else if (digitalRead(BUTTON_C) == HIGH)
			{
				state = 0;
			}
		}
		if (digitalRead(BUTTON_A) == LOW)
		{
			Serial.println("Start Auto Pattern");
			stepper.runToNewPosition(940);
			delay(1500);
			stepper.runToNewPosition(1880);
			delay(1500);
			stepper.runToNewPosition(2820);
			delay(1500);
			stepper.runToNewPosition(3760);
			delay(1500);
			stepper.runToNewPosition(4700);
			delay(1500);
			stepper.runToNewPosition(5640);
			delay(1500);
			stepper.runToNewPosition(6580);
			delay(1500);
			stepper.runToNewPosition(7520);
			delay(1500);
			stepper.runToNewPosition(8460);
			delay(1500);
			stepper.runToNewPosition(9400);
			delay(1500);
			Serial.println("Done");
		}
	}
	// Auto Injection

	if (switchEvent)
	{
		labState = NOT_RUNNING;
	}
}

void lab_4(void)
{
	if (labState == NOT_RUNNING)
	{
		Serial.println("Lab 4: ");
		labState = RUNNING;
	}
	if (labState == RUNNING)
	{
	}
	if (switchEvent)
	{
		labState = NOT_RUNNING;
	}
}

void lab_5(void)
{
	if (labState == NOT_RUNNING)
	{
		Serial.println("Lab 5: ");
		labState = RUNNING;
	}
	if (labState == RUNNING)
	{
	}
	if (switchEvent)
	{
		labState = NOT_RUNNING;
	}
}

// ############## Stepping motor Using Stepper Library #################
#ifdef STEP_WITH_BUILTIN_LIB

const int stepsPerRevolution = 200;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9);

int stepCount = 0; // number of steps the motor has taken

void setup()
{
	// initialize the serial port:
	Serial.begin(9600);
}

void loop()
{
	myStepper.step(200);
	Serial.print("steps:");
	Serial.println(stepCount);
	stepCount++;
	delay(500);
}

#endif

// ############## Push Button #################
#ifdef PUSH_BUTTON

// int buttonState = 0; // variable for reading the pushbutton status
// int cwBtnState = 0;
// int ccwBtnState = 0;

// void setup()
// {
//   Serial.begin(9600); // open the serial port at 9600 bps:
//   // initialize the clockwise button pin as an input:
//   pinMode(cwButton, INPUT_PULLUP);
//   // initialize the counter clockwise button pin as an input:
//   pinMode(ccwButton, INPUT_PULLUP);
//   // initialize the built-in led pin as an output:
//   pinMode(ledPin, OUTPUT);

// }

// void loop()
// {
//   // read the state of the pushbutton value:
//   cwBtnState = digitalRead(cwButton);

//   // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
//   if (cwBtnState != HIGH)
//   {
//     // turn LED on:
//     digitalWrite(ledPin, HIGH);
//     Serial.println("CW Clicked");
//   }
//   else
//   {
//     digitalWrite(ledPin, LOW);
//   }
//   delay(50);

//   ccwBtnState = digitalRead(ccwButton);
//   //  check if the pushbutton is pressed. If it is, the buttonState is HIGH:
//   if (ccwBtnState != HIGH)
//   {
//     // turn LED on:
//     digitalWrite(ledPin, HIGH);
//     Serial.println("CCW Clicked");
//   }
//   else
//   {
//     digitalWrite(ledPin, LOW);
//   }
//   delay(50);

// }

#endif