#include <Servo.h>
#include <SimpleTimer.h>
#include <Adafruit_NeoPixel.h>
#include "SPI.h"

SimpleTimer ledTimer;
int ledTimerId = 0;
uint8_t ledIndex = 0;

SimpleTimer timer;
int timerId = 0;
uint16_t timeoutTime = 5000;
uint8_t success = 0;
uint8_t failed = 0;

Servo clockServo;
int clockPos = 0;    // variable to store the servo position 

// 0 - 1024
const uint16_t kPotOffMin = 0;
const uint16_t kPotOffMax = 200;

const uint16_t kPotMedMin = 400;
const uint16_t kPotMedMax = 600;

const uint16_t kPotHotMin = 850;
const uint16_t kPotHotMax = 1200;

int potPinL = A0;    // select the input pin for the potentiometer
int potPinR = A1;    // select the input pin for the potentiometer
int valL = 0;       // variable to store the value coming from the sensor
int valR = 0;       // variable to store the value coming from the sensor

int waterSwitchPin = 7;
int waterOn = 0;

const uint8_t kStatusLed = 9;

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

const uint8_t kStepStateStart = 0;
const uint8_t kStepStateLoop = 1;
const uint8_t kStepStateEnd = 2;

const uint8_t veggies[] = {6, 13, 5, 10, 9, 11};

uint8_t stoveTemp = 0;
uint8_t useWater = 0;
uint8_t veggieOne = 0;
uint8_t veggieTwo = 0;
uint8_t veggieThree = 0;
uint8_t veggieFour = 0;

uint8_t gameStep = 0;
uint8_t stepState = 0;

int statusLEDPin = 22;
Adafruit_NeoPixel statusStrip = Adafruit_NeoPixel(9, statusLEDPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  
  randomSeed(analogRead(A0) + analogRead(A1));
  
  stoveTemp = random(1, 3);
  Serial.print("Stove Temp: ");
  Serial.println(stoveTemp);
  useWater = random(2);
  Serial.print("Use water: ");
  Serial.println(useWater);
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
  
  timerId = timer.setTimeout(timeoutTime, timedOut);
  timer.disable(timerId);
  
  ledTimerId = ledTimer.setTimeout(1000, ledTimedOut);
  ledTimer.disable(ledTimerId);
  
  waterLEDStrip.begin();
  waterLEDStrip.show(); 
  colorWipe(waterLEDStrip, waterLEDStrip.Color(0, 0, 255));
  
  statusStrip.begin();
  statusStrip.show();
  //statusColorWipe(statusStrip, waterLEDStrip.Color(0, 0, 255));
}

void loop() {
  timer.run();
  ledTimer.run();
  if (gameStep == 0) {
    if (stepState == kStepStateStart) {
      timer.enable(timerId);
      timer.restartTimer(timerId);
      if (stoveTemp == kStoveMedium) {
        statusStrip.setPixelColor(gameStep, statusStrip.Color(127, 0, 127));
      } else if (stoveTemp == kStoveHot) {
        statusStrip.setPixelColor(gameStep, statusStrip.Color(127, 0, 0));
      }
      statusStrip.show();
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      uint16_t potLVal = analogRead(potPinL);
      //Serial.println(potLVal);
      if (stoveTemp == kStoveMedium) {
        if ((potLVal >= kPotMedMin) && (potLVal <= kPotMedMax)) {
          Serial.print("Medium: ");
          Serial.println(potLVal);
          succeeded();
        }
      } else if (stoveTemp == kStoveHot) {
        if ((potLVal >= kPotHotMin) && (potLVal <= kPotHotMax)) {
          Serial.print("Hot: ");
          Serial.println(potLVal);
          succeeded();
        }
      }
      
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
    
  } else if (gameStep == 1) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 2) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 3) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 4) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 5) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 6) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 7) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 8) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 9) {
    if (stepState == kStepStateStart) {
      stepState = kStepStateLoop;
    } else if (stepState == kStepStateLoop) {
      
      succeeded();
    } else if (stepState == kStepStateEnd) {
      stepState = kStepStateStart;
      gameStep++;
    }
  } else if (gameStep == 10) {

  }
  
  
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

void succeeded() {
  timer.disable(timerId);
  success++;
  statusStrip.setPixelColor(kStatusLed, statusStrip.Color(0, 0, 127));
  statusStrip.show();
  ledIndex = gameStep;
  ledTimer.enable(ledTimerId);
  ledTimer.restartTimer(ledTimerId);
  stepState = kStepStateEnd;
}

void timedOut() {
    failed++;
    stepState = kStepStateEnd;
    statusStrip.setPixelColor(kStatusLed, statusStrip.Color(127, 0, 0));
    statusStrip.show();
    timer.disable(timerId);
}

void ledTimedOut() {
    statusStrip.setPixelColor(kStatusLed, statusStrip.Color(0, 0, 0));
    statusStrip.show();
    ledTimer.disable(ledTimerId);
}

void colorWipe(Adafruit_NeoPixel strip, uint32_t c) {
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
