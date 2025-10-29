#include "../common/hal.h"
#include <time.h>

/*
 * Criar pinos virtuais para simulação da cisterna
 * Devemos associar os pinos na interface
 * Podemos testar usando valor true
 *
 * Criar variáveis para monitorar os tanques
 * Podemos testar usando valores numéricos
 */

// pinos virtuais do tanque 1
bool pin_sensor11;
bool pin_sensor12;
bool pin_valve1;
bool pin_valve1_led_error;

// pinos virtuais do tanque 2
bool pin_sensor21;
bool pin_sensor22;
bool pin_pump1;
bool pin_pump1_led_error;

// pinos virtuais do tanque 3
bool pin_sensor31;
bool pin_sensor32;
bool pin_valve2;
bool pin_valve2_led_error;
bool pin_resistor1;
bool pin_resistor1_led_error;

// variáveis de monitoramento
float level_tank1;
float level_tank2;
float level_tank3;
float temp_tank3;

// atribuições do tanque 1
bool sensor11(){
    return pin_sensor11;
}

bool sensor12(){
    return pin_sensor12;
}

void valve1(bool state){
    pin_valve1 = state;
}

void valve1_led_error(bool state){
    pin_valve1_led_error = state;
}

// atribuições do tanque 2
bool sensor21(){
    return pin_sensor21;
}

bool sensor22(){
    return pin_sensor22;
}

void pump1(bool state){
    pin_pump1 = state;
}

void pump1_led_error(bool state){
    pin_pump1_led_error = state;
}

// atribuições do tanque 3
bool sensor31(){
    return pin_sensor31;
}

bool sensor32(){
    return pin_sensor32;
}

float sensorTemp(){
    return temp_tank3;
}

void valve2(bool state){
    pin_valve2 = state;
}

void valve2_led_error(bool state){
    pin_valve2_led_error = state;
}

void resistor1(bool state){
    pin_resistor1 = state;
}

void resistor1_led_error(bool state){
    pin_resistor1_led_error = state;
}

// inicialização
void init_hal(){
    // tanque 1
    pin_sensor11 = false;
    pin_sensor12 = false;
    pin_valve1 = false;
    pin_valve1_led_error = false;

    // tanque 2
    pin_sensor21 = false;
    pin_sensor22 = false;
    pin_pump1 = false;
    pin_pump1_led_error = false;

    // tanque 3
    pin_sensor31 = false;
    pin_sensor32 = false;
    pin_valve2 = false;
    pin_valve2_led_error = false;
    pin_resistor1 = false;
    pin_resistor1_led_error = false;

    // níveis e temperatura
    level_tank1 = 0;
    level_tank2 = 0;
    level_tank3 = 0;
    temp_tank3 = 25.0;
}

/*
 * No SO windows é possível executar a função clock
 * No sistema embarcado precisamos construir a lógica de cronômetro
 */
uint32_t now(){
    return clock();
}
