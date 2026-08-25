#include "../include/Temperature.h"

int temperatureSensorpin = A0; // Pin connected to the temperature sensor

void initTemperatureSensor(int pin) {
    temperatureSensorpin = pin; // Initialize the pin for the temperature sensor




}

float readTemperature() {
    
    uint16_t sensorValue;
    sensorValue = analogRead(temperatureSensorpin); // Read the analog value from the temperature sensor
    float voltaje = sensorValue * (3.3 / 4095.0); // Convert the analog value to voltage

    return voltaje;
}