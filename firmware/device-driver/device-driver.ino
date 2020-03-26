/*
 * File name: device-driver.ino
 * Authors: Martin Lehtomaa, Joni Tuomisto
 * Description: The driver code to run the program. All UI functionality is made in the driver,
 *              but the functionality from components, like HC-SR04 sensor, is remotely called
 *              from through methods from implemented libraries.
 */


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Local libraries
#include "hcsr04.h"
#include "led.h"

// Pin definitions
#define BUTTON_PIN A0

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET  LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

// Global variables to implement button interaction
int lastButtonState = LOW;
int buttonState = LOW;

unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;  

// Function declerations to UI print methods
void printTop(char* text, int textSize, int indentX = 25);
void printMiddle(char* text, int textSize, int indentX = 25);
void printBottom(char* text, int textSize, int indentX = 25);

void setup() {
  Serial.begin (9600);
  srand(time(0));
  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  delay(100);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.setTextColor(WHITE);
  display.clearDisplay();
  
  ledsOff();
}
 
void loop() {
  /* This blcok prints real-time distance measuring on the UI screen.
   * It is commented out for now, once the game functionality is at place,
   * we might add it as an option/tool to the device.
  float distance_cm = getDistanceCentimeter();
  int brightness = map((int)distance_cm, 0, 15, 0, 255);
  
  display.setCursor(25,0); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Distance Meter");
  
  display.setCursor(10,10); //oled display
  display.setTextColor(WHITE);
  display.println(distance_cm);
  display.setCursor(90,10);
  display.println("cm");
  Serial.print(distance_cm);
  Serial.println("cm");

  if (distance_cm <= 15) {
    analogWrite(LED_PIN, 255 - brightness);
    Serial.println("Getting too close!");
    display.println("Getting too close!");
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  display.display();
  
  delay(500);
  display.clearDisplay();
  
  */  

  // Game driver logic
  animateIntro();
  delay(500);

  // Calculates a random distance (cm) within 300cm (3m)
  int upperBound = 300;
  int randDistance = (rand() % (upperBound - 1 + 1)) + 1; 

  // Passes the random distance to the animation method
  animateRngApproximation(randDistance);

  
  delay(2000);
  // Animates countdown to green player
  animateCountDown("g");
  
  delay(500);
  // Animates countdown to red player
  animateCountDown("r");
  
  
}

// Button debounce method
void buttonDebounce() {
  delay(10);
  int reading = 0;
  if (analogRead(BUTTON_PIN) >= 500) {
    reading = HIGH;
  } else {
    reading = LOW;
  }
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
 
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }
  lastButtonState = reading;
}

// Animates the intro of the game. 
// Includes choosing a pair to play with and the countdown to the start of the game
void animateIntro() {
  display.clearDisplay();
  printTop("Welcome!", 1, 35);
  display.display();
  delay(1000);
  printMiddle("This is the", 1, 30);
  printBottom("Proximity Game", 1);
  display.display();
  delay(2000);

  display.clearDisplay();
  printMiddle("You are Green", 1);
  printBottom("for now...", 1, 35);
  display.display();
  delay(2000);
  display.clearDisplay();
  
  // Choosing your partner, continues when the player pushes the button
  printMiddle("Choose an opponent", 1, 10);
  printBottom("as Red!", 1, 40);
  display.display();
  delay(3000);
  Serial.println(buttonState);
  display.clearDisplay();
  printMiddle("Press the button", 1, 10);
  printBottom("when ready", 1);
  display.display();
  while(!buttonState) {
    Serial.println(buttonState);
    buttonDebounce();
  }
  buttonState = LOW;
  display.clearDisplay();
  printMiddle("OK!", 2, 40);
  display.display();

  // Starts countdown to game start (5 sec)
  delay(5000);
  for(int i=10; i > 0; --i) {
    char countBuf[3];
    snprintf(countBuf, sizeof(countBuf), "%d", i);
    const char* startLabel = "Game starts in: ";
    
    // Combine label with start counter
    char* labelWithCounter;
    labelWithCounter =(char*) malloc(strlen(startLabel) + strlen(countBuf) + 1);
    strcpy(labelWithCounter, startLabel);
    strcat(labelWithCounter, countBuf);
     
    display.clearDisplay();
    printTop(labelWithCounter, 1, 10);
    display.display();
    delay(1000);
    free(labelWithCounter);
  }
}

// Method to display the random distance on UI
void animateRngApproximation(int randDistance) {
  display.clearDisplay();
  printTop("Distance goal:", 1);
  display.display();

  for(int i=0; i < 4; ++i) {
    switch(i) {
      case 0:
        printMiddle(".", 2, 35);
        break;
      case 1:
        printMiddle(".", 2, 45);
        break;
      case 2:
        printMiddle(".", 2, 55);
        break;
      case 3:
        printMiddle(".", 2, 65);
        break;
    }
    display.display();
    delay(1000);
  }
  char randDistanceBuf[5];
  snprintf(randDistanceBuf, sizeof(randDistanceBuf), "%d", randDistance);
  const char* centimeter = "cm";
  
  // Combine distance digit with cm extension
  char* distanceCm;
  distanceCm =(char*) malloc(strlen(randDistanceBuf) + strlen(centimeter) + 1);
  strcpy(distanceCm, randDistanceBuf);
  strcat(distanceCm, centimeter);
  
  display.clearDisplay();
  printTop("Distance goal:", 1);
  printMiddle(distanceCm, 2);
  display.display();
  free(distanceCm);
  delay(5000);
}

// Animates countdown to start measuring for green or red,
// Turns on green or red led according to which players countdown
// Params: "g" for green
//         "r" for red
void animateCountDown(char* playerName) {
  char* getReadyLabel;
  if (strcmp(playerName, "g") == 0) {
    getReadyLabel = "Green get ready...";
    greenLedOn();
  } else if (strcmp(playerName, "r") == 0) {
    getReadyLabel = "Red get ready...";
    redLedOn();
  }
  
  for(int i=3; i > 0; --i) {
    char countBuf[2];
    snprintf(countBuf, sizeof(countBuf), "%d", i);
    display.clearDisplay();
    printTop(getReadyLabel, 1, 10);
    printMiddle(countBuf, 2, 50);
    display.display();
    delay(1000);
  }
  ledsOff();
  display.clearDisplay();
  printMiddle("BEGIN!", 2);
  display.display();
  delay(500);
}


// Prints the provided text on top of the the UI screen with fixed Y positioning
// Params: "text" text to be displayed
//         "textSize" character size on UI
//         "indentX" indentation on X axis, by default set to 25
void printTop(char* text, int textSize, int indentX) {
  display.setCursor(indentX,0); 
  display.setTextSize(textSize);
  display.print(text);
}

// Prints the provided text on middle of the the UI screen with fixed Y positioning
// Params: "text" text to be displayed
//         "textSize" character size on UI
//         "indentX" indentation on X axis, by default set to 25
void printMiddle(char* text, int textSize, int indentX) {
  display.setCursor(indentX,10);
  display.setTextSize(textSize);
  display.print(text);
}

// Prints the provided text on bottom of the the UI screen with fixed Y positioning
// Params: "text" text to be displayed
//         "textSize" character size on UI
//         "indentX" indentation on X axis, by default set to 25
void printBottom(char* text, int textSize, int indentX) {
  display.setCursor(indentX,20);
  display.setTextSize(textSize);
  display.print(text);
}
