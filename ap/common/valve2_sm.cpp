#include "valve2_sm.h"
#include "hal.h"
#include "chronometer.h"

chrono valve2_chrono;

STATE(valve2_init){
    // atuadores
    valve2(false);
    valve2_led_error(false);

    // lógica aninhada para leitura única!
    // se o tanque 3 estiver cheio
    if(sensor32()) {
        if(sensor31()) {
            NEXT_STATE(valve2_closed);
        } else {
            NEXT_STATE(valve2_error);
        }
    } else {
        // caso tanque 3 precise abastecer, abra valve2
        // mas só pode abrir se houver água no tanque 2
        if(sensor21()) {
            NEXT_STATE(valve2_open);
        } else {
            NEXT_STATE(valve2_closed);
        }
    }
}

STATE(valve2_closed){
    valve2(false);
    valve2_led_error(false);

    // proteção de 15 segundos usando temporizador
    if(ENTRY) {
        chrono_start(&valve2_chrono, 15000);
    }

    if(sensor32()) {
        if(!sensor31()) {
            NEXT_STATE(valve2_error);
        }
    } else {
        if(chrono_is_finished(&valve2_chrono) && sensor21()) {
            NEXT_STATE(valve2_open);
        }
    }

}

STATE(valve2_open){
    valve2(true);
    valve2_led_error(false);

    if(sensor32()) {
        if(sensor31()) {
            NEXT_STATE(valve2_closed);
        } else {
            NEXT_STATE(valve2_error);
        }
    } else {
        if(!sensor21()) {
            NEXT_STATE(valve2_closed);
        }
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
STATE(valve2_error){
    valve2(false);
    valve2_led_error(true);
}
