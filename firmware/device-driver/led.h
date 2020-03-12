#ifndef _LED_H    
#define _LED_H  

#define BLUE_LED_PIN D2
#define GREEN_LED_PIN D7
#define RED_LED_PIN D8

#ifdef __cplusplus
extern "C" {
#endif

void redLedOn();
void greenLedOn();
void blueLedOn();
void ledsOff();

#ifdef __cplusplus
}
#endif

#endif _LED_H  
