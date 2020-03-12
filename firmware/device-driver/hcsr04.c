#include "Arduino.h"
#include "hcsr04.h"

float getDistanceCentimeter() {
  unsigned long duration;
  float distance_cm;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 1000000);
  //distance_cm = duration * 0.034/2; WORKS ALSO
  distance_cm = (duration/2) / 29.1;

  return distance_cm;
}
