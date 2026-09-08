#include "SensorTemperatura.h"
#define pin_PWM 13
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
        Serial.println("Error al leer la temperatura del sensor DHT11");
        return -1.0; // Indicar error
    }
int pwmValue = map(temperatura, 0, 50, 0, 255); // Mapear la temperatura a un valor PWM
    ledcWrite(PWM_CHANNEL, pwmValue); // Ajustar el valor PWM según la temperatura leída
    return temperatura;
}

void sensorTemperaturaEnviar(float temperatura) {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
}