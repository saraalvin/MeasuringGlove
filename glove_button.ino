//#include <MegunoLink.h>
#include <CommandHandler.h>
#include <TCPCommandHandler.h>
#include <ArduinoTimer.h>
#include <CircularBuffer.h>
#include <EEPROMStore.h>
#include <Filter.h>
#include <Adafruit_NeoPixel.h>

// These constants won't change:
const int switchPin = 7;    // pin that the switch is attached to
const int ledPin = 13;
const int neoPixPin = 12;       // pin that the LED is attached to
const int nPixels = 1;
Adafruit_NeoPixel pixels(nPixels, neoPixPin, NEO_GRB + NEO_KHZ800);

// variables:
bool sensorValue;
int measuredDistance = 0;    // Variable to store measured distance
int baseMeasurement = 15;    // Distance in cm between index and thumb tips
bool alreadyTouched = false;
bool longTouch = false;
unsigned long currentTime = 0;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void setup() {

  // initialize serial communication with computer:
  Serial.begin(9600);

  // reset measured distance
  measuredDistance = 0;

  // indicate that the led pin will be used as output and turn it off
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();            // Turn OFF all pixels ASAP
  pixels.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  // indicate that the sensor pin will be used as input
  pinMode(switchPin, INPUT);
}

void loop() {

  // read the sensor:
  sensorValue = digitalRead(switchPin);
  if (sensorValue == HIGH && alreadyTouched == false) {  // the finger tips have touched

    alreadyTouched = true;

    measuredDistance += baseMeasurement; // add one base measurement to the total distance measured

    digitalWrite(ledPin, HIGH);

    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(14,   106,   159));         //  Set pixel's color (in RAM)
    pixels.show();



  } else if (sensorValue == LOW && alreadyTouched == true){
    alreadyTouched = false;
    elapsedTime = 0;
    longTouch = false;
    digitalWrite(ledPin, LOW);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();

  } else if (sensorValue == LOW && alreadyTouched == false){
    digitalWrite(ledPin, LOW);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();

    elapsedTime = 0;
    longTouch = false;

  } else if (sensorValue == HIGH && alreadyTouched == true && longTouch == false){
    startTime = millis();
    longTouch = true;
  }

  // send it to the computer
  //Serial.println(sensorValue);

  if(longTouch == true) {
    currentTime = millis();
    elapsedTime = currentTime - startTime;
    Serial.println(elapsedTime);

    if (elapsedTime >= 5000){
      measuredDistance = 0;
      longTouch = false;
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.show();
      Serial.println("Reset");
      delay(2000);
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
    }
  }

  Serial.println(measuredDistance);
  delay(100);        // delay in between reads for stability

}
