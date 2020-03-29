/*
 * File name: hcsr04.c
 * Authors: Martin Lehtomaa, Joni Tuomisto
 * Description: File contains logic to retrun a distance in cm
 */

#include "Arduino.h"
#include "hcsr04.h"

float getDistanceCentimeter() {
  unsigned long duration;
  float distance_cm = 0;

   
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  
    duration = pulseIn(ECHO_PIN, HIGH, 1000000);
    //distance_cm = duration * 0.034/2; WORKS ALSO
    distance_cm = (duration/2) / 29.1;
  
  return distance_cm;
}
float measure() {
  float measureSum = 0;
  for (int i = 0; i < 10; i++)
  {
    delay(200);
    measureSum += getDistanceCentimeter();
  }
  return measureSum / 10;
}
