#include <MegunoLink.h>
#include <CommandHandler.h>
#include <TCPCommandHandler.h>
#include <ArduinoTimer.h>
#include <CircularBuffer.h>
#include <EEPROMStore.h>
#include <Filter.h>

// These constants won't change:
const int switchPin = 7;    // pin that the switch is attached to
const int ledPin = 13;       // pin that the LED is attached to

// variables:
bool sensorValue;
int measuredDistance = 0;    // Variable to store measured distance
int baseMeasurement = 15;    // Distance in cm between index and thumb tips
bool alreadyTouched = false;

void setup() {

  // initialize serial communication with computer:
  Serial.begin(9600);

  // reset measured distance
  measuredDistance = 0;

  // indicate that the led pin will be used as output and turn it off
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // indicate that the sensor pin will be used as input
  pinMode(switchPin, INPUT);
}

void loop() {

  // read the sensor:
  sensorValue = digitalRead(switchPin);
  if (sensorValue == HIGH && alreadyTouched == false) {  // the finger tips have touched
    alreadyTouched = true;
    digitalWrite(ledPin, HIGH);
    measuredDistance += baseMeasurement; // add one base measurement to the total distance measured
  } else if (sensorValue == LOW && alreadyTouched == true){
    alreadyTouched = false;
    digitalWrite(ledPin, LOW);
  } else if (sensorValue == LOW && alreadyTouched == false){
    digitalWrite(ledPin, LOW);
  }

  // send it to the computer
  //Serial.println(sensorValue);
  Serial.println(measuredDistance);
  delay(100);        // delay in between reads for stability

}
