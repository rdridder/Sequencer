#include <Wire.h>
#include <splash.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>

#define NUMBOARDS 1
#define NUM_ENCODERS NUMBOARDS * 8 + 1

#pragma region encoder related stuff
const unsigned long encoderPollInterval = 5;
static long positions[NUM_ENCODERS] = { 0 };
static RotaryEncoder *encoders[NUM_ENCODERS];
static unsigned long previousMillis = 0;
static RotaryEncoder encoder1 = RotaryEncoder(52, 50, RotaryEncoder::LatchMode::FOUR0);
static RotaryEncoder encoder2 = RotaryEncoder(48, 46, RotaryEncoder::LatchMode::FOUR0);
static RotaryEncoder encoder3 = RotaryEncoder(44, 42, RotaryEncoder::LatchMode::FOUR0);
static RotaryEncoder encoder4 = RotaryEncoder(40, 38, RotaryEncoder::LatchMode::FOUR0);
static RotaryEncoder encoder5 = RotaryEncoder(36, 34, RotaryEncoder::LatchMode::FOUR0);
static RotaryEncoder encoder6 = RotaryEncoder(32, 30, RotaryEncoder::LatchMode::FOUR0);
static RotaryEncoder encoder7 = RotaryEncoder(28, 26, RotaryEncoder::LatchMode::FOUR0);
static RotaryEncoder encoder8 = RotaryEncoder(24, 22, RotaryEncoder::LatchMode::FOUR0);
static RotaryEncoder encoder9 = RotaryEncoder(2, 3, RotaryEncoder::LatchMode::FOUR0);
#pragma endregion encoder related stuff

#pragma region display related stuff
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
char encoder[] = "encoder";

#pragma endregion display related stuff

#pragma region button shift register related stuff
const unsigned long buttonShiftPollInterval = 4;
#define NUMBER_OF_SHIFT_CHIPS NUMBOARDS * 2
#define DATA_WIDTH NUMBER_OF_SHIFT_CHIPS * 8
#define BYTES_VAL_T unsigned long
int ploadPin = 8;			// Connects to Parallel load pin the 165
int clockEnablePin = 6;		// Connects to Clock Enable pin the 165
int sdataPin = 5;			// Connects to the Q7 pin the 165
int sclockPin = 7;			// Connects to the Clock pin the 165

BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;
#pragma endregion button shift register related stuff

#pragma region led shift register related stuff
uint8_t clockPin = 9;
uint8_t latchPin = 11;
uint8_t dataPin = 12;




#pragma endregion led shift register related stuff




// Global variable to hold the current milliseconds for delay purposes
unsigned long currentMillis;

void setup() {
	// Start serial
	Serial.begin(115200);
	
	// Encoders
	encoders[0] = &encoder1;
	encoders[1] = &encoder2;
	encoders[2] = &encoder3;
	encoders[3] = &encoder4;
	encoders[4] = &encoder5;
	encoders[5] = &encoder6;
	encoders[6] = &encoder7;
	encoders[7] = &encoder8;
	encoders[8] = &encoder9;

	// Display
	display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 0);
	display.display();

	// Button shift register
	pinMode(ploadPin, OUTPUT);
	pinMode(clockEnablePin, OUTPUT);
	pinMode(sclockPin, OUTPUT);
	pinMode(sdataPin, INPUT);
	digitalWrite(sclockPin, LOW);
	digitalWrite(ploadPin, HIGH);

	// Led shift register
	pinMode(latchPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	handleLedShiftRegister(0);
};

void loop() {
	currentMillis = millis();
	readShiftRegisters();
	tickEncoders();
}

void readShiftRegisters()
{
	if (currentMillis - previousMillis < buttonShiftPollInterval) {
		return;
	}
	long bitVal;
	BYTES_VAL_T bytesVal = 0;

	/* Trigger a parallel Load to latch the state of the data lines,
	*/
	digitalWrite(clockEnablePin, HIGH);
	digitalWrite(ploadPin, LOW);
	digitalWrite(ploadPin, HIGH);
	digitalWrite(clockEnablePin, LOW);

	/* Loop to read each bit value from the serial out line
	 * of the SN74HC165N.
	*/
	for (int i = 0; i < DATA_WIDTH; i++)
	{
		bitVal = digitalRead(sdataPin);

		/* Set the corresponding bit in bytesVal.
		*/
		bytesVal |= (bitVal << ((DATA_WIDTH - 1) - i));

		/* Pulse the Clock (rising edge shifts the next bit).
		*/
		digitalWrite(sclockPin, HIGH);
		digitalWrite(sclockPin, LOW);
	}

	pinValues = bytesVal;
	if (pinValues != oldPinValues)
	{
		oldPinValues = pinValues;
		displayButtonShiftPinValues();
		handleLedShiftRegister(pinValues);
	}
}

void displayButtonShiftPinValues()
{
	display.clearDisplay();
	display.setCursor(0, 0);
	display.println("buttons");
	for (int i = 0; i < DATA_WIDTH; i++)
	{
		if (i % 8 == 0) {
			display.println();
		}
		if ((pinValues >> i) & 1) {
			display.print(1);
		}
		else {
			display.print(0);
		}
		display.print(' ');
	}
	display.display();
}

void tickEncoders() {
	if (currentMillis - previousMillis >= encoderPollInterval) {
		for (int i = 0; i < NUM_ENCODERS; i++) {
			encoders[i]->tick();
			long newPos = encoders[i]->getPosition();
			if (positions[i] != newPos) {
				positions[i] = newPos;
				printEncoderValue(i, newPos);
			}
		}
	}
}

void handleLedShiftRegister(unsigned long buttonData) {
	int shift = buttonData & ((1 << 8) - 1);
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, shift);
	digitalWrite(latchPin, HIGH);
}

void printEncoderValue(int i, long pos) {
	display.clearDisplay();
	display.setCursor(0, 0);
	display.print(encoder);
	display.print(' ');
	display.print(i+1);
	display.print(' ');
	display.setCursor(0, 10);
	display.print(pos);
	display.print(' ');
	display.display();
}