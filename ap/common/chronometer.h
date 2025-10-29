#pragma once

#include <stdint.h>

// Passo 17: recuperar os arquivos de cronometro do projeto anterior
typedef uint32_t chrono;

void chrono_start(chrono* c, uint32_t dt);
bool chrono_is_finished(chrono* c);

