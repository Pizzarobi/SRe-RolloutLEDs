// Schanzer Unterboden LEDs für Rollout
// by Robert Kalmar

// Ein nicht blockierender LED Controller mit WIFI unterstützung

// Libraries
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

// LED Stats
#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 42
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

// PIN Numbers
#define BUTTON_PIN 1
#define LED_PIN_1 2
#define LED_PIN_2 3

// create FASTLED array
CRGB leds[NUM_LEDS];


// Functional Stuff
#define PATTERNS 5
unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 0;       // Current Pattern Number
int           patternInterval = 3000;   // Pattern Interval (ms)
int           pixelInterval = 50;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
uint16_t      pixelCurrent = 0;         // Pattern Current Pixel Number
uint16_t      pixelNumber = NUM_LEDS;  // Total Number of Pixels
uint8_t       wantedPattern = 0;        // Wanted Pattern


void setup() {
  // configure Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // tell FASTLED we have multiple strips
  FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  

  



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
      case 5:
        //static
        changeColor(200,50,50);
        break;
      case 4:
        rainbow_wave(10,10);
        break;
      case 3:
        //theaterChase(strip.Color(127, 127, 127), 50); // White
        break;
      case 2:
        //colorWipe(strip.Color(0, 0, 255), 50); // Blue
        break;
      case 1:
        //colorWipe(strip.Color(0, 255, 0), 50); // Green
        break;        
      default:
        //colorWipe(strip.Color(255, 0, 0), 50); // Red
        break;
    }
  }
}

// Change whole strip to a static color
void changeColor(int r, int g, int b){
  for(int i = 0; i < pixelNumber; i++){
    leds[i].setRGB(r,g,b);
  }
}

void rainbow_wave(uint8_t thisSpeed, uint8_t deltaHue) {     // The fill_rainbow call doesn't support brightness levels.
// uint8_t thisHue = beatsin8(thisSpeed,0,255);                // A simple rainbow wave.
 uint8_t thisHue = beat8(thisSpeed,255);                     // A simple rainbow march.
  
 fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);            // Use FastLED's fill_rainbow routine.
 
} // rainbow_wave()
