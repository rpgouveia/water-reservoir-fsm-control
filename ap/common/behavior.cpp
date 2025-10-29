// Passo 11: definir o behavior
#include "behavior.h"
#include "valve_sm.h"
#include "valve2_sm.h"
#include "pump_sm.h"
#include "resistor_sm.h"
#include "hal.h"

StateMachine valve_sm, pump_sm, valve2_sm, resistor_sm;

void behavior() {
    init_hal();
    INIT(valve_sm, valve1_init);       // sm1: Controle da válvula 1
    INIT(pump_sm, pump1_init);         // sm2: Controle da bomba 1
    INIT(valve2_sm, valve2_init);      // sm3: Controle da válvula 2
    INIT(resistor_sm, resistor1_init); // sm4: Controle de temperatura
    while(true){
        // executar as máquinas de estado de forma sequencial
        EXEC(valve_sm);
        EXEC(pump_sm);
        EXEC(valve2_sm);
        EXEC(resistor_sm);
    }
}
