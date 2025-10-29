#include "resistor_sm.h"
#include "hal.h"

// constantes de controle
const float temp_low = 49.0;
const float temp_high = 51.0;
const float temp_min = 0.0;
const float temp_max = 100.0;

STATE(resistor1_init) {
    resistor1(false);
    resistor1_led_error(false);

    // leitura única dos estados dos sensores
    bool s31 = sensor31();
    float temp = sensorTemp();

    // verifcar se a leitura do sensor está dentro da faixa válida
    if(temp < temp_min || temp > temp_max) {
        NEXT_STATE(resistor1_error);
    } else {
        // se houver água no tanque 3 verifica a temperatura
        if(s31) {
            if(temp <= temp_low) {
                NEXT_STATE(resistor1_on);
            } else {
                NEXT_STATE(resistor1_off);
            }
        } else {
            NEXT_STATE(resistor1_off);
        }
    }
}

STATE(resistor1_off) {
    resistor1(false);
    resistor1_led_error(false);

    bool s31 = sensor31();
    float temp = sensorTemp();

    if(temp < temp_min || temp > temp_max) {
        NEXT_STATE(resistor1_error);
    } else {
        if(s31) {
            if(temp <= temp_low) {
                NEXT_STATE(resistor1_on);
            }
        }
    }
}

STATE(resistor1_on) {
    resistor1(true);
    resistor1_led_error(false);

    bool s31 = sensor31();
    float temp = sensorTemp();

    if(temp < temp_min || temp > temp_max) {
        NEXT_STATE(resistor1_error);
    } else {
        if(!s31) {
            // proteção crítica contra aquecimento a seco
            NEXT_STATE(resistor1_off);
        } else {
            if(temp >= temp_high) {
                NEXT_STATE(resistor1_off);
            }
        }
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
STATE(resistor1_error) {
    resistor1(false);
    resistor1_led_error(true);
}
