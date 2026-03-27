# My_Repo

This repository contains my work in **analog design, power electronics, and control systems**, with a focus on real-world circuit behavior, switching dynamics, and hardware-oriented thinking.

---

## 🔥 Project 1: Analog Thermal Fan Controller

A fully analog temperature-based fan control system designed for stable and noise-resilient operation.

### Key Highlights:

* Designed using a **comparator with hysteresis** to eliminate output chatter near threshold temperatures
* Implemented **MOSFET-based fan driving** for efficient switching
* Modeled temperature sensor behavior using **LTSpice behavioral voltage source (LM35 equivalent)**
* Tuned hysteresis band to ensure **stable operation under noisy and fluctuating thermal conditions**
* Focused on **practical issues like noise immunity and switching stability**, not just ideal behavior

### Status:

* ✅ Simulation completed in LTSpice
* 🛠 PCB design in progress

---

## ⚡ Project 2: 3-Phase SPWM Inverter (Motor Driver)

A three-phase inverter system designed using sinusoidal PWM concepts for motor control applications.

### Key Highlights:

* Designed **3-phase inverter topology** for AC motor driving
* Generated **SPWM switching signals in LTSpice** to simulate realistic inverter behavior
* Implemented **bootstrap-based high-side gate driving** for MOSFET switching
* Analyzed **phase voltages, switching transitions, and dead-time effects**
* Defined approach for **future MCU-based PWM generation using timers**

### Status:

* ✅ Simulation validated (waveforms verified)
* 🛠 PCB layout in progress

---

## ⚙️ Project 3: DC-DC Buck Converter (12V → 10V)

A switching regulator design focused on understanding high duty-cycle behavior and practical power circuit considerations.

### Key Highlights:

* Designed a **12V → 10V buck converter** using the **ADP2300 regulator IC**
* Followed **datasheet-recommended application circuit** for implementation
* Observed **high duty-cycle operation** and its impact on switching intervals
* Implemented **over-current protection** using shunt-based current sensing
* Simulated **switching behavior and output ripple** in LTSpice
* Focused on **PCB layout practices** including loop minimization and grounding

### Status:

* ✅ Simulation completed in LTSpice
* 🛠 Hardware validation in progress

---

More projects coming soon, including hardware validation and embedded control implementations.
