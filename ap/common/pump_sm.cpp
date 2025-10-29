#include "pump_sm.h"
#include "hal.h"
#include "chronometer.h"

chrono pump1_chrono;

STATE(pump1_init) {
    pump1(false);
    pump1_led_error(false);

    if(sensor22()) {
        if(sensor21()) {
            NEXT_STATE(pump1_off);
        } else {
            NEXT_STATE(pump1_error);
        }
    } else {
        // Verificar se há água na cisterna
        if(sensor11()) {
            NEXT_STATE(pump1_on);
        } else {
            // não há água para abastecer o tanque 2
            NEXT_STATE(pump1_off);
        }

    }
}

STATE(pump1_off) {
    pump1(false);
    pump1_led_error(false);

    if(ENTRY){
        chrono_start(&pump1_chrono, 5000);
    }

    if(sensor22()) {
        if(!sensor21()) {
            NEXT_STATE(pump1_error);
        }
    } else {
        if(chrono_is_finished(&pump1_chrono) && sensor11()){
            NEXT_STATE(pump1_on);
        }
    }
}

STATE(pump1_on) {
    pump1(true);
    pump1_led_error(false);
    if(sensor22()) {
        if(sensor21()) {
            NEXT_STATE(pump1_off);
        } else {
            NEXT_STATE(pump1_error);
        }
    } else {
        if (!sensor11()){
            NEXT_STATE(pump1_off);
        }
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
STATE(pump1_error) {
    pump1(false);
    valve1(false);
    pump1_led_error(true);
}
