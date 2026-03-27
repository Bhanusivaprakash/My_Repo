My_Repo

This repository contains my work in analog design, power electronics, and control systems, with a focus on real-world circuit behavior, switching dynamics, and hardware-oriented thinking.

🔥 Project 1: Analog Thermal Fan Controller

A fully analog temperature-based fan control system designed for stable and noise-resilient operation.

Key Highlights:
Designed using a comparator with hysteresis to eliminate output chatter near threshold temperatures
Implemented MOSFET-based fan driving for efficient switching
Modeled temperature sensor behavior using LTSpice behavioral voltage source (LM35 equivalent)
Tuned hysteresis band to ensure stable operation under noisy and fluctuating thermal conditions
Focused on practical issues like noise immunity and switching stability, not just ideal behavior
Status:
✅ Simulation completed in LTSpice
🛠 PCB design in progress
⚡ Project 2: 3-Phase SPWM Inverter (Motor Driver)

A three-phase inverter system designed using sinusoidal PWM concepts for motor control applications.

Key Highlights:
Designed 3-phase inverter topology for AC motor driving
Generated SPWM switching signals in LTSpice to simulate realistic inverter behavior
Implemented bootstrap-based high-side gate driving for MOSFET switching
Analyzed phase voltages, switching transitions, and dead-time effects
Defined approach for future MCU-based PWM generation using timer peripherals
Status:
✅ Simulation validated (waveforms verified)
🛠 PCB layout in progress
⚙️ Project 3: Buck Converter with Current Sensing (12V → 3.3V)

A switching regulator design focused on real-world power delivery, current monitoring, and practical hardware considerations.

Key Highlights:
Designed a 12V → 3.3V buck converter using the ADP2300
Integrated current sensing using LT6105 with a low-side shunt resistor
Generated a scaled analog current signal (~2 V/A) for monitoring and control applications
Followed datasheet-recommended application circuit and validated switching behavior in LTSpice
Observed switching characteristics, duty cycle behavior, and output ripple
Incorporated PCB layout considerations including high di/dt loop minimization and proper grounding strategy
Status:
✅ Simulation completed in LTSpice
🛠 PCB design and hardware validation in progress

More projects coming soon, including hardware validation and embedded control implementations.
