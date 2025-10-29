#include "chronometer.h"
#include "hal.h"

void chrono_start(chrono* c, uint32_t dt) {
	*c = now() + dt;
}

bool chrono_is_finished(chrono* c) {
	return (*c <= now());
}