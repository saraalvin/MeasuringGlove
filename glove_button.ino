#include <ArduinoTimer.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// These constants won't change
const int baseMeasurement = 15;    // Distance in cm between index and thumb tips
String baseUnitStr = String(baseMeasurement);
const int switchPin = 7;    // pin that the switch is attached to
const int neoPixPin = 6;   // pin that the Neopixel(s) are attached to
const int nPixels = 1;      // number of Neopixels
Adafruit_NeoPixel pixels(nPixels, neoPixPin, NEO_GRB + NEO_KHZ800);

// OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Variables
bool sensorValue;
int measuredDistance = 0;    // Variable to store measured distance
bool alreadyTouched = false;
bool longTouch = false;
unsigned long currentTime = 0;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void setup() {

  // initialize serial communication with computer:
  Serial.begin(9600);
  //while (!Serial) ;

  // reset measured distance
  measuredDistance = 0;

  // indicate that the sensor pin will be used as input
  pinMode(switchPin, INPUT);

  // setup Neopixel strip
  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();            // Turn OFF all pixels ASAP
  pixels.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)



  // setup OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  drawText(2, 10, 25, "Hello! :)");
  delay(2000);
  display.clearDisplay();
  display.display();

  Serial.println("Base unit: " + baseUnitStr + " cm");

}

void loop() {

  // read the sensor:
  sensorValue = digitalRead(switchPin);
  if (sensorValue == HIGH && alreadyTouched == false) {  // the finger tips have touched

    alreadyTouched = true;

    measuredDistance += baseMeasurement; // add one base measurement to the total distance measured

    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(14,   106,   159));         //  Set pixel's color (in RAM)
    pixels.show();

    //display.clearDisplay();
    display.fillRect(10, 35, SCREEN_WIDTH, 14, SSD1306_BLACK);
    String measuredDistanceStr = String(measuredDistance);
    drawText(2, 10, 35, String(measuredDistanceStr + " cm"));

  } else if (sensorValue == LOW && alreadyTouched == true){
    alreadyTouched = false;
    elapsedTime = 0;
    longTouch = false;

    //display.clearDisplay();
    display.fillRect(10, 50, SCREEN_WIDTH, 7, SSD1306_BLACK);

    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();

  } else if (sensorValue == LOW && alreadyTouched == false){

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

  if(longTouch == true && measuredDistance != 0) {
    currentTime = millis();
    elapsedTime = currentTime - startTime;
    //Serial.println(elapsedTime);
    int remainingTime = (5000 - elapsedTime)/1000;

    if(remainingTime >= 0) {
      //display.clearDisplay();
      display.fillRect(10, 50, SCREEN_WIDTH, 7, SSD1306_BLACK);
      String remainingTimeStr = String(remainingTime);
      drawText(1, 10, 50, String("Reset in " + remainingTimeStr + "sec.") );
    }

    if (elapsedTime >= 5000){
      measuredDistance = 0;
      longTouch = false;
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
      pixels.show();

      //display.clearDisplay();
      display.fillRect(10, 50, SCREEN_WIDTH, 7, SSD1306_BLACK);
      display.fillRect(10, 35, SCREEN_WIDTH, 14, SSD1306_BLACK);
      display.display();

      drawText(1, 10, 50, String("Reset done!"));

      //Serial.println("Reset");
      delay(1000);
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
    }
  }

  //6Serial.println(measuredDistance);
  drawText(1, 10, 10, String("Base unit: " + baseUnitStr + " cm"));
  drawText(1, 10, 20, "Measured distance:");
  String measuredDistanceStr = String(measuredDistance);
  drawText(2, 10, 35, String(measuredDistanceStr + " cm"));
  delay(100);        // delay in between reads for stability

}

void drawText(int siz, int x, int y, String str) {

  display.setTextSize(siz);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(x, y);     // Start at top-left corner
  display.println(str);
  display.display();
}
