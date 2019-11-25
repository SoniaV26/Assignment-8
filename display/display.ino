#include <SimpleDHT.h>

#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "ame3942";
const char* password =  "1234567890";

 
// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 21
int pinDHT11 = 21;
SimpleDHT11 dht11(pinDHT11);

#include <TTGO.h>
TTGOClass *ttgo;


void sendData(float t, float h)
{
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin("http://34.216.181.26:3000/setValue?t=" + String(t) + "&h=" + String(h)); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
}

void setup() {

  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  ttgo->eTFT->fillScreen(TFT_BLUE);
  ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLUE);
  ttgo->eTFT->drawString("Waiting for data",  5, 100, 4);

  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  //while (WiFi.status() != WL_CONNECTED) {
    //delay(1000);
    //Serial.println("Connecting to WiFi..");
  //}
 
  Serial.println("Connected to the WiFi network");
}

void loop() {
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

  // This code works!
  String celsiusString = " *C, ";
  String humidityString = " H";
  String message = temperature + celsiusString + humidity + humidityString;
  Serial.println(message);
  ttgo->eTFT->drawString(message,  5, 100, 4);
  
  sendData((float)temperature, (float)humidity);
  // DHT11 sampling rate is 1HZ.
  delay(1500);
}
