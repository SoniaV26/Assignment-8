/* Modified from Sweep by Barragan */

#include <ESP32Servo.h>

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
  
  tempServo.setPeriodHertz(50);    // standard 50 hz servo
  tempServo.attach(tempPin, 1000, 2000); // attach to pin

  humidityServo.setPeriodHertz(50);    // standard 50 hz servo
  humidityServo.attach(humidityPin, 1000, 2000); // attach to pin
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
}

void loop() {

    String inData = "";
  
    while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        inData += recieved; 

        // Process message when new line character is recieved
        if (recieved == '\n')
        {
            Serial.print("Message Received: ");
            Serial.print(inData);

            int move = inData.toInt();

            humidityServo.write(move);
            tempServo.write(move);

            inData = ""; // Clear recieved buffer
            move = 0;
        }
    }

}
