#include <math.h>
#include <Arduino.h>
#include "..\inc\SensorHumedad.h"

#define ADC_PIN 34 // Pin analógico para el sensor de humedad
#define PWM_PIN 38 // Pin PWM simulacion analogico
#define SENS_DELAY 1000 // tiempo entre lecturas

float voltValue
const float acdMax = 4095.0 // Valor máximo del ADC (12 bits)
const float vcc = 3.3 // Voltaje de referencia del ADC

void SensorHInit(void){
  Serial.begin(115200);
     pinMode(38,OUTPUT);
}


float SensorHRead(){
int value;
float volt;
value = analogRead(ADC_PIN); // leer el valor analogico
volt = (value * vcc) / acdMax; // Convertir el valor analógico a voltaje (3.3V es el voltaje de referencia)


return volt;
}

void PWMWrite(float volt){
  int valorpwm = (volt * 255.0) / vcc; // Convertir el voltaje a un valor de 0 a 255 para el PWM
  analogWrite(PWM_PIN, valorpwm); // Escribir el valor de 0 a 255 en el pin PWM
}
