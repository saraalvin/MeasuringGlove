#include <MegunoLink.h>
#include <CommandHandler.h>
#include <TCPCommandHandler.h>
#include <ArduinoTimer.h>
#include <CircularBuffer.h>
#include <EEPROMStore.h>
#include <Filter.h>

//  http://www.arduino.cc/en/Tutorial/Smoothing

// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.

// These constants won't change:
const int sensorPin = A2;    // pin that the sensor is attached to
const int ledPin = 13;       // pin that the LED is attached to
const int numReadings = 10;  // number of samples taken to average

// variables:
int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value

long FilterWeight = 5;
ExponentialFilter<long> ADCFilter(FilterWeight, 0);

void setup() {

  // initialize serial communication with computer:
  Serial.begin(9600);

  // turn on LED to signal the start of the calibration period:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // calibrate during the first five seconds
  while (millis() < 5000) {
    sensorValue = analogRead(sensorPin);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }

  // signal the end of the calibration period
  digitalWrite(13, LOW);
}

void loop() {

  // read the sensor:
  sensorValue = analogRead(sensorPin);

  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);

  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);

  // filter raw value
  ADCFilter.Filter(sensorValue);

  TimePlot Plot;
  //Plot.SendData("Raw", sensorValue);
  Plot.SendData("Filtered", ADCFilter.Current());

  // send it to the computer as ASCII digits
  Serial.println(ADCFilter.Current());
  delay(100);        // delay in between reads for stability

  // fade the LED using the calibrated value:
  analogWrite(13, ADCFilter.Current());

}
