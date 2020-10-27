#include <MegunoLink.h>
#include <CommandHandler.h>
#include <TCPCommandHandler.h>
#include <ArduinoTimer.h>
#include <CircularBuffer.h>
#include <EEPROMStore.h>
#include <Filter.h>

// These constants won't change:
const int switchPin = 16;    // pin that the switch is attached to
const int ledPin = 13;       // pin that the LED is attached to

// variables:
int measuredDistance = 0;    // Variable to store measured distance
int baseMeasurement = 15;    // Distance in cm between index and thumb tips

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
  if (digitalRead(switchPin) == HIGH) {  // the finger tips have touched
    digitalWrite(ledPin, HIGH);
    measuredDistance += baseMeasurement; // add one base measurement to the total distance measured
  }

  // send it to the computer
  Serial.println(measuredDistance);
  delay(100);        // delay in between reads for stability

}
