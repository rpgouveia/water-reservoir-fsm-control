/*
 * template_arduino_se.cpp
 *
 * Traditional embedded C/C++ programming style, without the setup() and loop() functions.
 * Author: afonsomiguel
 */

// Including the main Arduino framework header is still necessary.
#include <Arduino.h>
#include "../../common/behavior.h"

/**
 * @brief The program's main and single entry point.
 */
int main(void) {
	
	// Chamar o behavior do nosso sistema embarcado
    behavior();
	
    return 0; // This line is never reached.
}