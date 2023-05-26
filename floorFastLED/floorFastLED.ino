// Schanzer Unterboden LEDs für Rollout
// by Robert Kalmar

// Ein LED Controller mit WIFI unterstützung

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

// WiFi AP Libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// WiFi Defines
#define APSSID "schanzerPOWER"
#define APSSK "pimmelberger"

#define STASSID "schanzerPower"
#define STAPSK "pimmelberger"

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

// set WiFi Credentials
const char* ssid = STASSID;
const char* password = STAPSK;

// Functional Stuff
#define PATTERNS 4
unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 3;       // Current Pattern Number
int           patternInterval = 5000;   // Pattern Interval (ms)
int           pixelInterval = 50;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
uint16_t      pixelCurrent = 0;         // Pattern Current Pixel Number
uint16_t      pixelNumber = NUM_LEDS;   // Total Number of Pixels
uint8_t       wantedPattern = 0;        // Wanted Pattern

int strandColor = 0x009EE0; // Color of the Leds on the strip
uint8_t turnedOff = 0;

ESP8266WebServer server(80);            // Create a webserver object that listens for HTTP request on port 80

// Test Message to see if WiFi and Server are working
void handleRoot() {
  server.send(200, "text/html", "<h1>Willkommen im Schanzer LED System!</h1>Moegliche Calls: Pattern, Brightness, Color, standby<br><br>Pattern:<br>0: changeColor<br>1: rainbow_wave<br>2: runningLightSync<br>3: runningPixelTrail<br>4: Pixels Off!<br><br> Brightness: 0-255 <br><br> Color: Farbe in Hex<br><br>Schanzer Hex: 0x009EE0");
}

// If handle is not found, print out error and passed Arguments (usefull for debugging Arguments)
void handleNotFound(){
  String message = "File not found! \n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404,"text/plain", message);
}

// Control LED Patterns
void handleLEDs(){
  uint8_t argCount = server.args();
  String message = "Found these Arguments.\n\n";
  
  for(uint8_t i = 0; i < argCount; i++){
    if(server.argName(i) == "pattern"){
      // print info message
      message += "Changing to Pattern: ";
      message += server.arg(i);
      message += "\n\n\n";
      // call Pattern changer
      changePattern(server.arg(i));
    }else if(server.argName(i) == "color"){
      // print info message
      message += "Setting Color: ";
      message += server.arg(i);
      message += "\n\n\n";
      setColor(server.arg(i));
    }else if(server.argName(i) == "brightness"){
      // print info message
      message += "Setting Brightness: ";
      message += server.arg(i);
      message += "\n\n\n";
      setBrightness(server.arg(i));
    }else if(server.argName(i) == "standby"){
      //print info message
      message += "Going into Standby mode for : ";
      message += server.arg(i);
      message += "\n\n\n";
      setStandby(server.arg(i));
    } else {
      // print info message
      message += "Not sure what to do?: ";
      message += server.argName(i);
      message += " ";
      message += server.arg(i);
      message += "\n\n\n";
    }
  }

  // print Info message
  server.send(200,"text/plain",message);
}

// change Pattern to the one specified in arg
void changePattern(String arg){
  int argInt = arg.toInt();
  patternCurrent = argInt;
  

  //Serial.println("A: "+ argInt);
  //Serial.println("B: " + arg);
}

// get color hexstring and set it to strandColor
void setColor(String arg){
  int ans = 0;

  // cast hex string to int value
  sscanf(arg.c_str(), "%x", &ans);

  strandColor = ans;

  //Serial.println("Color: " + ans);
}

// Set brightness
void setBrightness(String arg){
  int argInt = arg.toInt();
  FastLED.setBrightness(argInt);

  //Serial.println("Brightness: " + argInt);
}

// put device into a "standby mode" basically just limiting loop time
void setStandby(String arg){
  int argInt = arg.toInt();
  // setStandby
  turnedOff = argInt;
}

void setup() {
  // configure Button
  // pinMode(BUTTON_PIN, INPUT_PULLUP);

  //Serial.begin(57600);
  //Serial.println("starting");

  // Configure FastLED
  FastLED.addLeds<WS2812, LED_PIN_A, GRB>(leds, NUM_LEDS_PER_STRIP); // Strip 1
  FastLED.addLeds<WS2812, LED_PIN_B, GRB>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); //Strip 2
  FastLED.setBrightness(255);

  // Configure WiFi AP
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
  //WiFi.softAP(ssid, password);
  //IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);

  // Configure WiFi Client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);

  // Wait for connection
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

  // Set Server Functions
  server.on("/", handleRoot);
  server.on("/led",handleLEDs);
  server.onNotFound(handleNotFound);
  
  server.begin();

  //Serial.println("HTTP server started");
  //Serial.println("Finished Configuration. Waiting 5 Seconds");
  
  //delay(5000);
}



void loop() {
  // put your main code here, to run repeatedly:

  // Button check - Not implemented for now. WIFI is main interest
  // boolean patternChangeWanted = digitalRead(BUTTON_PIN);

  // server handler call
  server.handleClient();

  unsigned long currentMillis = millis(); // Update current time

  // Timebased Patternchange
  // if((currentMillis - patternPrevious) >= patternInterval) {  //  Check for expired time or new pattern wanted (not implemented)
  //   patternPrevious = currentMillis;
  //   patternCurrent++;                                         //  Advance to next pattern
  //   if(patternCurrent >= PATTERNS)
  //     patternCurrent = 0;
  //   Serial.println("currentpattern "+patternCurrent);
  // }

  // Check time against pixelInterval and change Pattern if wanted/needed
  if(currentMillis - pixelPrevious >= pixelInterval) {        //  Check for expired time
    pixelPrevious = currentMillis;                            //  Run current frame
    switch (patternCurrent) {
      case 4:
        turnOffStrip();
        break;
      case 3:
        runningPixelTrail(35, 30);
        break;
      case 2:
        // running lights
        runningLightSync(25);
        break;
      case 1:
        rainbow_wave(30,10);
        break;      
      default:
        // static
        changeColor();
        break;
    }
  }
}

// PATTERNS

void turnOffStrip(){
  turnedOff = 1;
  for(int i = 0; i < pixelNumber; i++){
    leds[i] = 0x000000;
  }
  FastLED.show();
}

// A running Pixel with a trail that is going slightly darker
void runningPixelTrail(int trail, int wait){
  //Serial.println("runningPixelTrail");
  if(pixelInterval != wait){
    pixelInterval = wait;
  }
  int fadePixelBy = trail;
  int fadePixel = 0;

  leds[pixelCurrent] = strandColor;

  for(int i = 1; i < trail; i++){
    fadePixel = pixelCurrent-i;
    if(fadePixel<0){
      leds[NUM_LEDS+fadePixel].fadeToBlackBy(fadePixelBy);
    }else{
      leds[fadePixel].fadeToBlackBy(fadePixelBy);
    }
  }

  FastLED.show();

  pixelCurrent++;
  if(pixelCurrent >= pixelNumber){
    pixelCurrent = 0;
  }
}

// running light, equal on both strips
void runningLightSync(int wait){
  //Serial.println("runningLightSync");
  if(pixelInterval != wait)
    pixelInterval = wait;

  leds[pixelCurrent] = strandColor;
  leds[NUM_LEDS / 2 + pixelCurrent] = strandColor;

  FastLED.show();
  pixelCurrent++;
  if(pixelCurrent >= pixelNumber/2)
    pixelCurrent = 0;
}

// Change entire led Array to a static color
void changeColor(){
  //Serial.println("changeColor");
  for(int i = 0; i < pixelNumber; i++){
    leds[i] = strandColor;
  }
  FastLED.show();
}

void rainbow_wave(uint8_t bpm, uint8_t deltaHue) {     // The fill_rainbow call doesn't support brightness levels.
  //Serial.println("rainbow");
  //uint8_t thisHue = beatsin8(bpm,0,255);               // A simple rainbow wave.
  uint8_t thisHue = beat8(bpm,255);           // A simple rainbow march.
    
 fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);      // Use FastLED's fill_rainbow routine.
 FastLED.show();
}