#include "valve_sm.h"
#include "hal.h"
#include "chronometer.h"

chrono valve1_chrono;

STATE(valve1_init) {
    valve1(false);  // Desliga a válvula 1
    valve1_led_error(false);

    // ATENÇÃO! CUIDADO COM A CONCORRÊNCIA
    // Deve ler os estados dos sensores apenas uma vez
    // Condições de acionamento
    // Lógica adequada
    if(sensor12()) {
        if(sensor11()) {
            NEXT_STATE(valve1_closed);
        } else {
            NEXT_STATE(valve1_error);
        }
    } else {
        NEXT_STATE(valve1_open);
    }
}

STATE(valve1_closed) {
    valve1(false);
    valve1_led_error(false);
    // Usamos ENTRY para cronometrar no momento que muda de estado
    if(ENTRY){
        chrono_start(&valve1_chrono, 15000);
    }
    if(sensor12()) {
        if(!sensor11()) {
            NEXT_STATE(valve1_error);
        }
    } else {
        // momento de encerramento
        if(chrono_is_finished(&valve1_chrono)){
            NEXT_STATE(valve1_open);
        }
    }
}

STATE(valve1_open) {
    valve1(true);
    valve1_led_error(false);
    if(sensor12()) {
        if(sensor11()) {
            NEXT_STATE(valve1_closed);
        } else {
            NEXT_STATE(valve1_error);
        }
    }
}

/*
Próximo estado irá disparar apenas um aviso que pode ser ignorado
Caso queira evitar o aviso use isto:
#pragma GCC diagnostic push: Salva o estado atual das configurações de warnings.
#pragma GCC diagnostic ignored "-Wunused-parameter": Ignora o warning de parâmetro não utilizado.
*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
STATE(valve1_error) {
    valve1(false);
    valve1_led_error(true);
}
