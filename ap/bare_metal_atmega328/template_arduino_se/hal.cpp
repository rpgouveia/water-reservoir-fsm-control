#include "../../common/hal.h"
#include <Arduino.h>

// Definir os pinos reais
#define SW0 2   // sensor11
#define SW1 3   // sensor12
#define SW2 4   // sensor21
#define SW3 5   // sensor22
#define SW4 6   // sensor31
#define SW5 7   // sensor32
#define LED3 8  // valve1
#define LED2 9  // valve2
#define LED1 10 // pump1
#define LED0 11 // resistor1

#define LED7 12 // valve1 led error
#define LED6 13 // pump1 led error

// Pino analógico para sensor de temperatura
#define TEMP_SENSOR A0

// Dispositivos de Entrada
// Tanque 1
bool sensor11() {
	return digitalRead(SW0);
}

bool sensor12() {
	return digitalRead(SW1);
}

// Tanque 2
bool sensor21() {
	return digitalRead(SW2);
}

bool sensor22() {
	return digitalRead(SW3);
}

// Tanque 3
bool sensor31() {
	return digitalRead(SW4);
}

bool sensor32() {
	return digitalRead(SW5);
}

float sensorTemp() {
	int value = analogRead(TEMP_SENSOR);
	// Mapear a faixa de 0-1023 para 40.0-60.0°C
	float temperature = 40.0 + (value / 1023.0) * 20.0;
	return temperature;
}

// Dispositivos de saída
// Tanque 1
void valve1(bool state) {
	digitalWrite(LED3, state);
}

void valve1_led_error(bool state) {
	digitalWrite(LED7, state);
}

// Tanque 2
void pump1(bool state) {
	digitalWrite(LED1, state);
}

void pump1_led_error(bool state) {
	digitalWrite(LED6, state);
}

// Tanque 3
void valve2(bool state) {
	digitalWrite(LED2, state);
}

void valve2_led_error(bool state) {
	// Não implementado por falta de pinos no Arduino
	// Preciso usar um LED adicional
}

void resistor1(bool state) {
	digitalWrite(LED0, state);
}

void resistor1_led_error(bool state) {
	// Não implementado por falta de pinos no Arduino
	// Preciso usar um LED adicional
}

// Inicialização
void init_hal() {
	// inicialização do arduíno
	init();
	// pinos de entrada
	pinMode(SW0, INPUT);
	pinMode(SW1, INPUT);
	pinMode(SW2, INPUT);
	pinMode(SW3, INPUT);
	pinMode(SW4, INPUT);
	pinMode(SW5, INPUT);
	
	// pinos de saída (LEDs)
	pinMode(LED0, OUTPUT);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	
	// Inicializar LEDs desligados
	digitalWrite(LED0, LOW);
	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);
	digitalWrite(LED3, LOW);
}

// Cronômetro
uint32_t now() {
	return millis();
}