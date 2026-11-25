#include "SensorTemperatura.h"
#define PIN_PWM 13
#define PWM_CHANNEL 0
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8

DHT dht(PIN_SENSOR_TEMP, DHT_TYPE);

void sensorTemperaturaInit(void) {
    dht.begin();
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(PIN_PWM, PWM_CHANNEL);
}

float sensorTemperaturaLeer(void) {
    float temperatura = dht.readTemperature();
    if (isnan(temperatura)) {
        return -1.0; // Indicar error (el mensaje ERROR lo envia uart.cpp)
    }
    // PWM proporcional a la temperatura: 0 a 50 C -> 0 a 255
    int pwmValue = (int)(constrain(temperatura, 0.0, 50.0) * 255.0 / 50.0 + 0.5);
    ledcWrite(PWM_CHANNEL, pwmValue); // Ajustar el valor PWM segun la temperatura leida
    return temperatura;
}

void sensorTemperaturaEnviar(float temperatura) {
    Serial.print("T:");
    Serial.println(temperatura, 1);
}

float sensorHumedadLeer(void) {
    float humedad = dht.readHumidity(); // % de humedad relativa del mismo DHT11
    if (isnan(humedad)) {
        return -1.0; // Indicar error
    }
    return humedad;
}

void sensorHumedadEnviar(float humedad) {
    Serial.print("H:");
    Serial.println(humedad, 1);
}