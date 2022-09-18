#include "Adafruit_LiquidCrystal.h" // Library for LCD display.

// Define I/O pins
#define thermistorPin A0 // Thermistor is connected to analog pin 0.
#define inputCLK 7       // Input clock pin for the rotary encoder.
#define inputDT 6        // Input data pin for the rotary encoder.
#define btn 8            // Button pin for the rotary encoder.
#define motor 9          // Motor pin is connected to digital pin 9.

// LCD PINS
const int rs = 10, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // LCD pins.
Adafruit_LiquidCrystal lcd(rs, en, d4, d5, d6, d7);         // LCD object.

// Thermistor Variables
int Vo;                                             // Thermistor voltage.
float R1 = 10000;                                   // Resistance of thermistor.
float R2, T;                                        // Resistance of thermistor and temperature.
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; // Thermistor coefficients.

// Rotary Encoder Variables
float preferredTemp = 21;                           // Preferred temperature (default 21 degrees Celsius).
// float preferredTemp = 69.8;                         // Preferred temperature (default 69.8 degrees Fahrenheit) (uncomment this line if you want to use Fahrenheit).
float currentTemp;                                  // Current temperature.
int currentStateCLK;                                // Current state of the input clock pin.
int previousStateCLK;                               // Previous state of the input clock pin.

int currentMotorToggleState;                        // Current toggle state of the motor.
int previousMotorToggleState;                       // Previous toggle state of the motor.
String motorToggleState = "on";                     // Motor toggle state.

void setup() {
   lcd.begin(16, 2);                                // Initialize LCD.

   pinMode(thermistorPin, INPUT);                   // Set thermistor pin as input.

   pinMode (inputCLK, INPUT);                       // Set input clock pin as input.
   pinMode (inputDT, INPUT);                        // Set input data pin as input.

   pinMode (btn, INPUT);                            // Set button pin as input.

   pinMode (motor, OUTPUT);                         // Set motor pin as output.

   currentMotorToggleState = digitalRead(btn);      // Set current toggle state of the motor.
}

/**
   * Sets the preferred temperature.
   */
void setPreferredTemp() {
   // Read the current state of inputCLK
   currentStateCLK = digitalRead(inputCLK);

   // If the previous and the current state of the inputCLK are different then a pulse has occured
   if (currentStateCLK != previousStateCLK){
      // If the inputDT state is different than the inputCLK state then
      // the encoder is rotating counterclockwise
      if (digitalRead(inputDT) == currentStateCLK) {
         preferredTemp++;
      } else {
         // Encoder is rotating clockwise
         preferredTemp--;
      }
   }

   // Update previousStateCLK with the current state
   previousStateCLK = currentStateCLK;
}

/**
   * Returns the current temperature that is measured by the thermistor.
   */
float getTemp() {
   Vo = analogRead(thermistorPin); // Read the voltage from the thermistor.
   R2 = R1 * (1023.0 / (float)Vo - 1.0); // Calculate the resistance of the thermistor.
   currentTemp = (1.0 / (c1 + (c2 * log(R2)) + (c3 * log(R2) * log(R2) * log(R2)))) - 273.15; // Calculate the temperature in degrees Celsius.
   // currentTemp = (currentTemp * 9.0 / 5.0) + 32.0; // Convert the temperature to degrees Fahrenheit (uncomment this line if you want to use Fahrenheit).

   return currentTemp;
}

/**
   * Returns the preferred temperature.
   */
float getPreferredTemp() {
   return preferredTemp;
}

/**
   * Turns the motor on or off.
   * If the current temperature is higher than the preferred temperature, the motor will be turned on.
   * @param currentMotorToggleState: "on" or "off"
   */
void setMotorToggleState(int CurrentMotorToggleState) {
   if ((currentMotorToggleState == HIGH) && (previousMotorToggleState == LOW)) {
      if (motorToggleState == "on") {
         motorToggleState = "off";
      } else {
         motorToggleState = "on";
      }
   }

   previousMotorToggleState = currentMotorToggleState;
}

/**
   * Returns the current state of the motor.
   */
String getMotorToggleState() {
   return motorToggleState;
}

/**
   * Turns the motor on or off depending on the current state of the motor.
   */
void activateMotor() {
   if (getMotorToggleState().equals("on")) {
      if (getTemp() > getPreferredTemp()) {
         analogWrite(motor, 128);
      } else {
         analogWrite(motor, 0);
      }
   } else {
      analogWrite(motor, 0);
   }
}

/**
   * Prints the current temperature and the preferred temperature on the LCD.
   */
void updateLCD() {
   lcd.setCursor(0, 0);
   lcd.print("Temp: ");
   lcd.print(getTemp(), 1);
   lcd.print(" ");
   lcd.print((char)223);
   lcd.print("C");
   lcd.setCursor(0, 1);
   lcd.print("Set: ");
   lcd.print(getPreferredTemp(), 1);
   lcd.print(" ");
   lcd.print((char)223);
   lcd.print("C");

   lcd.setCursor(13, 1);
   lcd.print(getMotorToggleState());

   delay(100);

   lcd.clear();
}

void loop() {
   float currentTemp = getTemp();
   float preferredTemp = getPreferredTemp();
   int currentMotorToggleState = digitalRead(btn);
   setPreferredTemp();
   setMotorToggleState(currentMotorToggleState);
   activateMotor();
   updateLCD();
}

