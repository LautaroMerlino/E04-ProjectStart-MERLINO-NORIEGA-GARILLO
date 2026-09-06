#include "SensorTemperatura.h"

DHT dht(PIN_SENSOR_TEMP, DHT_TYPE);

void sensorTemperaturaInit(void) {
    dht.begin();
}

float sensorTemperaturaLeer(void) {
    float temperatura = dht.readTemperature();
    if (isnan(temperatura)) {
        Serial.println("Error al leer la temperatura del sensor DHT11");
        return -1.0; // Indicar error
    }
    return temperatura;
}

void sensorTemperaturaEnviar(float temperatura) {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
}