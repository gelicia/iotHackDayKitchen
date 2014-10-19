#include <Servo.h> 
#include <Adafruit_NeoPixel.h>
#include "LPD8806.h"
#include "SPI.h"

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

uint8_t touchOnePin = 14;
uint8_t touchTwoPin = 15;
uint8_t touchThreePin = 16;
uint8_t touchFourPin = 17;

const uint8_t kNoWater = 0;
const uint8_t kWater = 1;
const uint8_t kStoveCold = 0;
const uint8_t kStoveMedium = 1;
const uint8_t kStoveHot = 2;

const uint8_t veggies[] = {6, 13, 5, 10, 9, 11};

uint8_t stoveTemp = 0;
uint8_t useWater = 0;
uint8_t veggieOne = 0;
uint8_t veggieTwo = 0;
uint8_t veggieThree = 0;
uint8_t veggieFour = 0;

int statusDataPin  = 23;
int statusClockPin = 22;
LPD8806 statusStrip = LPD8806(10, statusDataPin, statusClockPin);

void setup() {
  Serial.begin(9600);
  
  randomSeed(analogRead(0));
  
  stoveTemp = random(1, 3);
  useWater = random(2);
  veggieOne = random(6);
  veggieTwo = random(6);
  veggieThree = random(6);
  veggieFour = random(6);
  
  clockServo.attach(9);
  
  pinMode(potPinL, INPUT);
  pinMode(potPinR, INPUT);
  
  pinMode(waterSwitchPin, OUTPUT);
  
  pinMode(touchOnePin, INPUT);
  pinMode(touchTwoPin, INPUT);
  pinMode(touchThreePin, INPUT);
  pinMode(touchFourPin, INPUT);
  
  waterLEDStrip.begin();
  waterLEDStrip.show(); 
  colorWipe(waterLEDStrip, waterLEDStrip.Color(0, 0, 255));
  
  statusStrip.begin();
  statusStrip.show();
  //statusColorWipe(statusStrip, waterLEDStrip.Color(0, 0, 255));
}

void loop() {
  touchLoop();
  valL = analogRead(potPinL); 
  valR = analogRead(potPinR); 
  //Serial.println("Reading: " + String(valL) + " " + String(valR));
  
  clockServo.write(clockPos);
  clockPos++;
  if (clockPos > 180){
    clockPos = 0;
  }
  
  waterOn = digitalRead(waterSwitchPin);
  //Serial.println("Water on: " + String(waterOn));
  
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
