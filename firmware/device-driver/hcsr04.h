#ifndef _HCSR04_H    
#define _HCSR04_H   

#define ECHO_PIN D5 
#define TRIG_PIN D6 

#ifdef __cplusplus
extern "C" {
#endif

float getDistanceCentimeter();
#ifdef __cplusplus
}
#endif

#endif _HCSR04_H   