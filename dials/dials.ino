/* Modified from Sweep by Barragan */

#include <ESP32Servo.h>
#include <TTGO.h>
TTGOClass *ttgo;

String inData = "";

Servo tempServo;        // servo for temperature
Servo humidityServo;    // servo for humidity

int tempPos = 0;        // position of temperature servo
int humidityPos = 0;    // position of humidity servo

int humidity = 0;       // humidity reading
int temperature = 0;    // temperature reading

int tempPin = 21;       // pin for temperature
int humidityPin = 22;   // pin for humidity

void setup() {

  Serial.begin(115200);  // Open serial port

  // For printing to screen
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();

  // Prepare screen
  ttgo->eTFT->fillScreen(TFT_BLUE);
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLUE);
  ttgo->eTFT->drawString("Ready and waiting",  15, 100, 4);

  // Servo setup
  
  tempServo.setPeriodHertz(50);    // standard 50 hz servo
  tempServo.attach(tempPin, 1000, 2000); // attach to pin

  humidityServo.setPeriodHertz(50);    // standard 50 hz servo
  humidityServo.attach(humidityPin, 1000, 2000); // attach to pin
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
}

void loop() {

    // Instructions:
    // Attach temperature to pin 21 and humidity to pin 22.
    
    // Open the serial and type the approximate number
    // of degrees (for % humidity) you want it to rotate.
    // To move backwards, type a negative number.
    
    // This is NOT PRECISE and results may vary.
    // We recommend using small numbers to stay cautious.
    // We made this using trial and error and it's far from perfect.
    
    // derived from SerialRead, example on Sept 16
    while (Serial.available() > 0) {
        Serial.println("Start loop");
        char recieved = Serial.read();
        Serial.println(recieved);
        inData += recieved; 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            Serial.print("Message Received: ");
            Serial.print(inData);

            // Print to screen (optional)
            ttgo->eTFT->fillScreen(TFT_BLUE);
            ttgo->eTFT->drawString(inData,  100, 100, 4);

            // "Move" holds the approximate # of degrees you want the dial to move
            int move = inData.toInt();

            // "Absolute Move" is the absolute value of move
            int absoluteMove = move;
            if (move < 0) {
              absoluteMove = move * -1;
            }

            // The time value
            // We found that 5 seems to be about the correct multiplier
            int time = absoluteMove * 5;

            // Write to motors

            Serial.println("Moving");

            // move backwards if negative
            if (move < 0) {
              humidityServo.writeMicroseconds(2000);
              tempServo.writeMicroseconds(2000);
            }
            // move forwards if positive
            else {
              humidityServo.writeMicroseconds(1000);
              tempServo.writeMicroseconds(1000);
            }

            // This sets how long it will keep moving.
            delay(time);

            // Then stop. Don't forget this part!
            humidityServo.writeMicroseconds(1500);
            tempServo.writeMicroseconds(1500);

            Serial.println("Done moving");

            // Clear variables
            // Perhaps we're overzealous, but we've had too many bugs
            // to not play it safe
            inData = "";
            move = 0;
            recieved = ' ';
        }
    }

}
