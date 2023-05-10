// Schanzer Unterboden LEDs für Rollout
// by Robert Kalmar

// Ein nicht blockierender LED Controller mit WIFI unterstützung

// Libraries
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

// LED Stats
#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 42
#define NUM_LEDS 84

// PIN Numbers
#define BUTTON_PIN 1
#define LED_PIN_A 2
#define LED_PIN_B 3

// create FASTLED array
CRGB leds[NUM_LEDS];


// Functional Stuff
#define PATTERNS 3
unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 0;       // Current Pattern Number
int           patternInterval = 5000;   // Pattern Interval (ms)
int           pixelInterval = 50;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
uint16_t      pixelCurrent = 0;         // Pattern Current Pixel Number
uint16_t      pixelNumber = NUM_LEDS;  // Total Number of Pixels
uint8_t       wantedPattern = 0;        // Wanted Pattern


void setup() {
  // configure Button
  //pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(57600);
  Serial.println("starting");

  // tell FASTLED we have multiple strips
  FastLED.addLeds<WS2812, LED_PIN_A, RGB>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, LED_PIN_B, RGB>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  
  FastLED.setBrightness(69);
}



void loop() {
  // put your main code here, to run repeatedly:

  // Button check - Not implemented for now. WIFI is main interest
  boolean patternChangeWanted = digitalRead(BUTTON_PIN);

  // Test Code
  // for(int i = 0; i < NUM_LEDS; i++){
  //   led
  // }

  unsigned long currentMillis = millis(); // Update current time
  if((currentMillis - patternPrevious) >= patternInterval) {  //  Check for expired time or new pattern wanted (not implemented)
    patternPrevious = currentMillis;
    patternCurrent++;                                         //  Advance to next pattern
    if(patternCurrent >= PATTERNS)
      patternCurrent = 0;
  }

  if(currentMillis - pixelPrevious >= pixelInterval) {        //  Check for expired time
    pixelPrevious = currentMillis;                            //  Run current frame
    switch (patternCurrent) {
      case 2:
        // running lights
        runningLightSync(20,30,200,25);
        break;
      case 1:
        rainbow_wave(30,10);
        break;      
      default:
        // static
        changeColor(00,200,00);
        break;
    }
  }
}

// running light, equal on both strips
void runningLightSync(int r, int g, int b, int wait){
  if(pixelInterval != wait)
    pixelInterval = wait;
  leds[pixelCurrent].setRGB(r,g,b);
  leds[NUM_LEDS / 2 + pixelCurrent].setRGB(r,g,b);
  FastLED.show();
  pixelCurrent++;
  if(pixelCurrent >= pixelNumber/2)
    pixelCurrent = 0;
  }

// Change whole strip to a static color
void changeColor(int r, int g, int b){
  for(int i = 0; i < pixelNumber; i++){
    leds[i].setRGB(r,g,b);
  }
  FastLED.show();
}

void rainbow_wave(uint8_t thisSpeed, uint8_t deltaHue) {     // The fill_rainbow call doesn't support brightness levels.
// uint8_t thisHue = beatsin8(thisSpeed,0,255);                // A simple rainbow wave.
 uint8_t thisHue = beat8(thisSpeed,255);                     // A simple rainbow march.
  
 fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);         // Use FastLED's fill_rainbow routine.
 FastLED.show();
 
} // rainbow_wave()
