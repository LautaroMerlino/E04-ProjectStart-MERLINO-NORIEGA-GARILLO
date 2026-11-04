#include "../include/Temperature.h"

int temperatureSensorpin = A0; // Pin connected to the temperature sensor

void initTemperatureSensor(int pin) {
    temperatureSensorpin = pin; // Initialize the pin for the temperature sensor


}

void initMotor(int pin) {
    motorPin = pin; // Initialize the pin for the motor
}

float readTemperature() {
    
    uint16_t sensorValue;
    sensorValue = analogRead(temperatureSensorpin); // Read the analog value from the temperature sensor

    uint8_t output = sensorValue/4;
    analogWrite(motorPin, output);
    float voltaje = sensorValue * (3.3 / 4095.0); // Convert the analog value to voltage

    return voltaje;
}