#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "hcsr04.h"


#define LED_PIN D2

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET  LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin (9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  delay(100);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay(); 
  
}
 
void loop() {
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
  
  Serial.println(distance_cm);
}
