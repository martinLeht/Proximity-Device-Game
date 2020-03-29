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
void animateIntro();
void animateRngApproximation(int);
void animateCountDown(char* playerName);
void animteMeasureDistanceTime();
void animateAndDisplayWinner(char *winner, int, int, int);
void animateScoreBoard(int[], int[]);
void animateSavePrompt();
void printTop(char* text, int, int indentX = 25);
void printMiddle(char* text, int, int indentX = 25);
void printBottom(char* text, int, int indentX = 25);
void determineWinner(char **result, int[], int[], int, int, int);
char* combineStringAndDigit(const char* string, int, bool digitAfterString = true);
char* concatenate(const char* first, const char* second);

void setup() {
  Serial.begin (9600);
  srand(time(0));
  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Reset the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  
  delay(600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.setTextColor(WHITE);
  display.clearDisplay();

  getDistanceCentimeter();
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

  
  display.clearDisplay();

  // Game driver logic
  animateIntro();
  delay(500);

  // Calculates a random distance (cm) within 300cm (3m)
  int upperBound = 300;
  int randDistance = (rand() % (upperBound - 1 + 1)) + 1; 

  // Passes the random distance to the animation method
  animateRngApproximation(randDistance);

  
  delay(2000);

  int greenScore[3];
  int redScore[3];
  int distance_cm = 0;
  // Each player gets 3 times to try to measure the distance within a 5 seconds laps
  for (int i = 0; i < 3; ++i) {
      // Starts with greens turn
      // Animates countdown to green player
      animateCountDown("g");
      animateMeasureDistanceTime();
      distance_cm = (int) getDistanceCentimeter();
      Serial.print("Green measured: ");
      Serial.println(distance_cm);
      
      greenScore[i] = distance_cm;
      displayMeasurement(distance_cm);

      // Then reds turn
      // Animates countdown to red player
      animateCountDown("r");
      animateMeasureDistanceTime();
      distance_cm = (int) getDistanceCentimeter();
      Serial.print("Red measured: ");
      Serial.println(distance_cm);
      
      redScore[i] = distance_cm;

      displayMeasurement(distance_cm);
  }

  char *winnerResult;
  int greenClosest = 0, redClosest = 0;

  // Determine the winner: assign the result and each players closest score to dedicated variablse
  determineWinner(&winnerResult, greenScore, redScore, &greenClosest, &redClosest, randDistance);
  
  Serial.print("Green Closest: ");
  Serial.println(greenClosest);
  Serial.print("Red Closest: ");
  Serial.println(redClosest);

  animateAndDisplayWinner(winnerResult, greenClosest, redClosest, randDistance);
  
  animateScoreBoard(greenScore, redScore);
  animateSavePrompt();
  
  Serial.println("------GAME DONE------");
  
  display.clearDisplay();
  ledsOff();
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
  display.clearDisplay();
  printMiddle("Press the button", 1, 10);
  printBottom("when ready", 1);
  display.display();
  while(!buttonState) {
    buttonDebounce();
  }
  buttonState = LOW;

  // Starts countdown to game start (10 sec)
  for(int i=10; i > 0; --i) {
    const char* startLabel = "Game starts in: ";
    
    // Combine label with start counter
    char* labelWithCounter = combineStringAndDigit(startLabel, i);
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

  for(int i=0; i < 3; ++i) {
    switch(i) {
      case 0:
        printMiddle(".", 2, 45);
        break;
      case 1:
        printMiddle(".", 2, 55);
        break;
      case 2:
        printMiddle(".", 2, 65);
        break;
    }
    display.display();
    delay(1000);
  }
  const char* centimeter = "cm";
  
  // Combine distance digit with cm extension
  char* distanceCm = combineStringAndDigit(centimeter, randDistance, false);
  
  display.clearDisplay();
  printTop("Distance goal:", 1);
  printMiddle(distanceCm, 2, 45);
  display.display();
  free(distanceCm);
  delay(4000);
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
  printMiddle("BEGIN!", 2, 30);
  display.display();
  delay(1000);
}

void displayMeasurement(int distance) {
  const char* centimeter = "cm";
  char* distanceCm = combineStringAndDigit(centimeter, distance, false);
  display.clearDisplay();
  printTop("You measured:", 1);
  printMiddle(distanceCm, 2, 40);
  display.display();
  free(distanceCm);
  delay(2000);
  
}

void animateMeasureDistanceTime() {
  int distance = 0;
  
  // Starts countdown to submit measurement
  for(int i=5; i > 0; --i) {
    const char* startLabel = "Time to measure: ";
    char* labelWithCounter = combineStringAndDigit(startLabel, i);
        
    display.clearDisplay();
    printTop(labelWithCounter, 1, 10);
    display.display();
    free(labelWithCounter);
    delay(1000);
   
  }
}

// Displays the winner and game scores
void animateAndDisplayWinner(char* winner, int greenClosest, int redClosest, int goal) {
  const char* closestLabel = "Closest: ";
  const char* goalLabel = "Goal: ";
  const char* centimeter = "cm";
  char* distanceCm;
  char* closestDistanceLabel;
  char* winnerScoresLabel;
  Serial.print("The winner: ");
  Serial.println(winner);

  display.clearDisplay();
  printTop("The winner is: ", 1);
  
  if (winner == "Green") {
    printMiddle(winner, 2, 30);
    
    distanceCm = combineStringAndDigit(centimeter, greenClosest, false);
    closestDistanceLabel = concatenate(closestLabel, distanceCm);
    greenLedOn();
  } else if (winner== "Red") {
    printMiddle(winner, 2, 40);
    
    distanceCm = combineStringAndDigit(centimeter, redClosest, false);
    closestDistanceLabel = concatenate(closestLabel, distanceCm);
    redLedOn();
  }
  display.display();
  delay(3000);
  free(distanceCm);

  distanceCm = combineStringAndDigit(centimeter, goal, false);
  char* goalDistanceLabel = concatenate(goalLabel, distanceCm);
  free(distanceCm);
  // Printting the the scores
  display.clearDisplay();
  printTop("Winner scores", 1, 20);
  printMiddle(closestDistanceLabel, 1, 10);
  printBottom(goalDistanceLabel, 1, 10);
  display.display();
  delay(5000);
  
  free(closestDistanceLabel);
  free(goalDistanceLabel);
  ledsOff();
}

void animateScoreBoard(int greenScores[], int redScores[]) {
  char* greenScoreRowLabel = "Green: ";
  char* redScoreRowLabel = "Red: ";
  
  for (int i = 0; i < 3; ++i) {
    greenScoreRowLabel = combineStringAndDigit(greenScoreRowLabel, greenScores[i]);
    redScoreRowLabel = combineStringAndDigit(redScoreRowLabel, redScores[i]);
    if (i < 2) {
      greenScoreRowLabel = concatenate(greenScoreRowLabel, ", ");
      redScoreRowLabel = concatenate(redScoreRowLabel, ", ");
    }
  }

  display.clearDisplay();
  
  printTop("Measurements (cm):", 1, 10);
  printMiddle(greenScoreRowLabel, 1, 5);
  printBottom(redScoreRowLabel, 1, 5);
  display.display();

  free(greenScoreRowLabel);
  free(redScoreRowLabel);
  delay(10000);
}

void animateSavePrompt() {
  display.clearDisplay();
  printTop("Press the button", 1, 20);
  printMiddle("if you want to", 1, 20);
  printBottom("save the scores", 1, 20);
  display.display();

  for (int i = 0; i < 50; ++i) {
    buttonDebounce();
    if (buttonState) {
      Serial.println("Saving scores to external database...");
      buttonState = LOW;
      break;
    }
    delay(100);
  }
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


// Determines the winner of the game. Compares absolute value of scores difference
// to the goal value to determine closest values.
void determineWinner(char **result, int greenScores[], int redScores[], int *greenClosest, int *redClosest, int goalValue) {
  *greenClosest = greenScores[0];
  *redClosest =  redScores[0];
  // Loopping through the scores, and by calculating the absolute value 
  // from the difference of a score and the goal value, we get the closest score to the goal.
  for (int i = 0; i < 3; ++i) {
    if (abs(greenScores[i] - goalValue) < abs(*greenClosest - goalValue)) *greenClosest = greenScores[i];
    if (abs(redScores[i] - goalValue) < abs(*redClosest - goalValue) ) *redClosest = redScores[i];
  }

  // Determine which one of the closest values is closer to goal
  if (abs(*greenClosest - goalValue) < abs(*redClosest - goalValue)) {
    *result = (char*)malloc((5 + 1) * sizeof(char));
    if (*result == NULL) return;
    *result = "Green";
  } else {
    *result = (char*)malloc((3 + 1) * sizeof(char));
    if (*result == NULL) return;
    *result = "Red";
  }
}


// Helper method to combine a string with a digit
char* combineStringAndDigit(const char* string, int num, bool digitAfterString) {
  char digitBuf[sizeof(num) + 1];
  snprintf(digitBuf, sizeof(digitBuf), "%d", num);

  char* stringWithDigit  =(char*) malloc(strlen(string) + strlen(digitBuf) + 1);
  // Combine string with digit, determine which comes first
  if (digitAfterString) {
    strcpy(stringWithDigit, string);
    strcat(stringWithDigit, digitBuf);
  } else {
    strcpy(stringWithDigit, digitBuf);
    strcat(stringWithDigit, string);
  } 

  return stringWithDigit;
}

char* concatenate(const char* first, const char* second) {
    const size_t len1 = strlen(first);
    const size_t len2 = strlen(second);
    char* result = (char*) malloc(len1 + len2 + 1);
    
    memcpy(result, first, len1);
    memcpy(result + len1, second, len2 + 1);
    return result;
}
