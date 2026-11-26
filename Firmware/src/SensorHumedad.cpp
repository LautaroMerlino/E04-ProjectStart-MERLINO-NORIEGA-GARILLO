#include <math.h>
#include <Arduino.h>
#include "SensorHumedad.h"
#include "SensorTemperatura.h"

#define PWM_PIN 38 // Pin PWM simulacion analogico
#define PWM_CHANNEL 1 // Canal distinto del de la temperatura (que usa el 0)
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8

void SensorHInit(void){
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);
}


float SensorHRead(){
  return sensorHumedadLeer(); // % de humedad relativa medido por el DHT11 (-1.0 si falla)
}

void PWMWrite(float humedad){
  // PWM proporcional a la humedad: 0 a 100 % -> 0 a 255
  int valorpwm = (int)(constrain(humedad, 0.0, 100.0) * 255.0 / 100.0 + 0.5);
  ledcWrite(PWM_CHANNEL, valorpwm); // Escribir el valor de 0 a 255 en el canal PWM
}