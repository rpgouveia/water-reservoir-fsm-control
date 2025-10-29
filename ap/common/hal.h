#ifndef HAL_H
#define HAL_H

#include <stdint.h>

// dispositivos de entrada:
// tanque 1
bool sensor11();
bool sensor12();
// tanque 2
bool sensor21();
bool sensor22();
// tanque 3
bool sensor31();
bool sensor32();
float sensorTemp();

// dispositivos de saída:
// tanque 1
void valve1(bool state);
void valve1_led_error(bool state);
// tanque 2
void pump1(bool state);
void pump1_led_error(bool state);
// tanque 3
void valve2(bool state);
void valve2_led_error(bool state);
void resistor1(bool state);
void resistor1_led_error(bool state);

// inicialização
void init_hal();
uint32_t now();

#endif // HAL_H
