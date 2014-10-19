#include "iotHackDayKitchen.h"
#include <Servo.h> 
#include "Adafruit_NeoPixel.h"
#include "LPD8806.h"
#include <SPI.h>

Servo clockServo;
int clockPos = 0;    // variable to store the servo position 

int potPinL = A0;    // select the input pin for the potentiometer
int potPinR = A1;    // select the input pin for the potentiometer
int valL = 0;       // variable to store the value coming from the sensor
int valR = 0;       // variable to store the value coming from the sensor

int waterSwitchPin = 7;
int waterOn = 0;

int waterLEDPin = 11;
Adafruit_NeoPixel waterLEDStrip = Adafruit_NeoPixel(3, waterLEDPin, NEO_GRB + NEO_KHZ800);

int statusDataPin  = 23;
int statusClockPin = 22;
LPD8806 statusStrip = LPD8806(10, statusDataPin, statusClockPin);

void setup() {
  Serial.begin(9600);
  
  clockServo.attach(9);
  
  pinMode(potPinL, INPUT);
  pinMode(potPinR, INPUT);
  
  pinMode(waterSwitchPin, OUTPUT);
  
  waterLEDStrip.begin();
  waterLEDStrip.show(); 
  colorWipe(waterLEDStrip, waterLEDStrip.Color(0, 0, 255));
  
  statusStrip.begin();
  statusStrip.show();
  statusColorWipe(statusStrip, waterLEDStrip.Color(0, 0, 255));
}

void loop() {
  valL = analogRead(potPinL); 
  valR = analogRead(potPinR); 
  Serial.println("Reading: " + String(valL) + " " + String(valR));
  
  clockServo.write(clockPos);
  clockPos++;
  if (clockPos > 180){
    clockPos = 0;
  }
  
  waterOn = digitalRead(waterSwitchPin);
  Serial.println("Water on: " + String(waterOn));
  
 // theaterChase(waterLEDStrip, waterLEDStrip.Color(0, 0, 127), 50); // Blue
 // theaterChase(waterLEDStrip, waterLEDStrip.Color(127, 0, 0), 50); // Red
  
  delay(15);
}

void colorWipe(Adafruit_NeoPixel strip, uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}

void statusColorWipe(LPD8806 strip, uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}

void theaterChase(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < waterLEDStrip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
