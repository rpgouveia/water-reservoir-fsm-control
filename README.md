# Water Reservoir FSM Control

A multi-phase embedded system for automated water management using Finite State Machines (FSM). 
Controls a three-tank system (cistern, upper reservoir, and boiler) with Qt simulation and 
Arduino hardware implementation.

## Features
- 4 independent state machines (valve control, pump control, bidirectional valve, thermal control)
- Qt-based simulation with real-time visualization
- Arduino UNO hardware deployment
- Temperature control with hysteresis (50°C ±1°C)
- Fail-safe protection mechanisms
