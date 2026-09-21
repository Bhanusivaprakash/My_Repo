# Pulse-Controlled Parallel Bus (PCPB)

A simple FPGA-based memory interface demonstrating a custom **Pulse-Controlled Parallel Bus (PCPB)** for transferring data between an external controller and FPGA memory.

The design implements a 32-byte memory with pulse-controlled write and read operations, sequential addressing, bus handshaking through control pulses, and an accumulator-based read path.

The project was developed in **Verilog RTL** and implemented/debugged on an FPGA using **Vivado and the Xilinx Integrated Logic Analyzer (ILA)**.

---

## Architecture

```text
             External Controller
                    │
                    │
          ┌─────────┴─────────┐
          │                   │
     write_enable        read_enable
          │                   │
          └─────────┬─────────┘
                    │
                    ▼
          ┌───────────────────┐
          │    PCPB Control   │
          │  Pulse Detection  │
          │  & Access Locks   │
          └─────────┬─────────┘
                    │
                    ▼
          ┌───────────────────┐
          │   32 × 8 Memory   │
          │                   │
          │  memory[0:31]     │
          └─────────┬─────────┘
                    │
                    ▼
               Read Path
                    │
                    ▼
              Accumulator
```


<img width="1536" height="1024" alt="PCPB" src="https://github.com/user-attachments/assets/008d2f31-22c6-4f1d-b3b7-30ffc618ab87" />

---



## Key Features

* Custom **Pulse-Controlled Parallel Bus (PCPB)**
* 8-bit bidirectional data bus
* 32 × 8-bit internal FPGA memory
* Sequential write addressing
* Sequential read addressing
* Pulse-based write and read control
* Write/read lock mechanism to prevent repeated operations while a control pulse remains asserted
* FPGA-side accumulator
* Synchronous RTL design
* Vivado ILA integration for hardware debugging

---

## PCPB Concept

The PCPB interface uses control signals as **pulses** to trigger individual memory transactions.

Instead of continuously interpreting an asserted control signal as multiple operations, the design detects the beginning of an asserted pulse and performs a single transaction.

For example:

```text
write_enable

      ┌───────┐
──────┘       └────────────
      ↑
      │
   One write
```

The `write_lock` register ensures that the memory write occurs only once during the pulse.

The same mechanism is used for reads through `read_lock`.

This provides a simple hardware-level transaction mechanism without requiring a conventional serial protocol.

---

# Interface

## Module

```verilog
module custom_memory(
    input wire write_enable,
    input wire read_enable,
    inout wire [7:0] data,
    input wire rst,
    input wire clk
);
```

## Signals

| Signal         | Direction     | Width | Description              |
| -------------- | ------------- | ----: | ------------------------ |
| `clk`          | Input         |     1 | FPGA system clock        |
| `rst`          | Input         |     1 | Synchronous reset        |
| `write_enable` | Input         |     1 | Initiates a memory write |
| `read_enable`  | Input         |     1 | Initiates a memory read  |
| `data`         | Bidirectional |     8 | PCPB data bus            |

---

# Memory Architecture

The design contains:

```verilog
reg [7:0] memory [0:31];
```

This provides:

* **32 memory locations**
* **8-bit data per location**
* **256 bits total storage**

Two pointers control sequential access:

```verilog
reg [4:0] write_ptr;
reg [4:0] read_ptr;
```

### Write pointer

Every accepted write stores the bus value at the current `write_ptr` and increments the pointer.

```text
write #1 → memory[0]
write #2 → memory[1]
write #3 → memory[2]
...
```

### Read pointer

Every accepted read accesses the current `read_ptr` and increments the pointer.

```text
read #1 → memory[0]
read #2 → memory[1]
read #3 → memory[2]
...
```

---

# Write Operation

A write occurs when:

```verilog
write_enable && !write_lock
```

The current bus value is stored:

```verilog
memory[write_ptr] <= data;
```

The pointer then advances:

```verilog
write_ptr <= write_ptr + 1;
```

Finally, the lock is asserted:

```verilog
write_lock <= 1;
```

The lock prevents the same asserted pulse from causing multiple writes.

Once `write_enable` returns low, the lock is released:

```verilog
else if(!write_enable)
    write_lock <= 0;
```

### Write sequence

```text
Controller
    │
    │ Put data on bus
    │
    │ write_enable ↑
    ▼
PCPB Interface
    │
    │ Store data
    ▼
memory[write_ptr]
    │
    │
    ▼
write_ptr++
```

---

# Read Operation

A read transaction occurs when:

```verilog
read_enable && !read_lock
```

The current memory location is accessed and accumulated:

```verilog
acc <= acc + memory[read_ptr];
```

The read pointer advances:

```verilog
read_ptr <= read_ptr + 1;
```

The read lock prevents repeated reads during the same control pulse.

```verilog
read_lock <= 1;
```

The lock is released when `read_enable` returns low.

---

# Accumulator

The read path contains an 8-bit accumulator:

```verilog
reg [7:0] acc = 0;
```

Every accepted read performs:

```text
acc = acc + memory[read_ptr]
```

For example, if memory contains:

```text
memory[0] = 10
memory[1] = 20
memory[2] = 30
```

successive reads produce:

```text
Initial acc = 0

Read memory[0] → acc = 10
Read memory[1] → acc = 30
Read memory[2] → acc = 60
```

This demonstrates that the bus interface can be coupled directly to FPGA-side computation rather than treating the FPGA purely as passive memory.

---

# Bidirectional Data Bus

The interface uses:

```verilog
inout wire [7:0] data;
```

The bus is driven by the FPGA only when:

```verilog
read_enable == 1
```

through:

```verilog
assign data = (read_enable) ? data_out : 8'bz;
```

Otherwise the FPGA releases the bus into high impedance:

```text
Read disabled
     │
     ▼
FPGA ──► Z
     │
     └── External controller may drive bus
```

This allows the same physical bus to be used for bidirectional communication.

---

# Hardware Debugging

The design includes a **Vivado Integrated Logic Analyzer (ILA)** instance.

The following internal and external signals are monitored:

| Probe    | Signal         |
| -------- | -------------- |
| `probe0` | `data`         |
| `probe1` | `write_ptr`    |
| `probe2` | `write_enable` |
| `probe3` | `acc`          |
| `probe4` | `read_enable`  |
| `probe5` | `read_ptr`     |
| `probe6` | `rst`          |

This allows the write/read transactions and pointer progression to be observed directly on FPGA hardware.

---

# RTL Design Concepts Demonstrated

This project demonstrates several fundamental digital-hardware concepts:

* Verilog RTL
* Synchronous sequential logic
* FPGA memory arrays
* Bidirectional buses
* High-impedance (`Z`) bus control
* Sequential addressing
* Control-pulse detection
* Transaction locking
* Read/write datapaths
* Accumulation
* FPGA on-chip debugging
* Custom hardware communication interfaces

---

# Design Flow

```text
Verilog RTL
     │
     ▼
Simulation / Verification
     │
     ▼
Vivado Synthesis
     │
     ▼
FPGA Implementation
     │
     ▼
Bitstream
     │
     ▼
Hardware Testing
     │
     ▼
Vivado ILA Debug
```

---

# Project Significance

The PCPB concept was developed as a simple custom parallel communication mechanism between an external controller and FPGA logic.

The memory interface serves as the foundational version of the PCPB architecture. The same basic idea can be extended from simple memory transfers toward **MCU–FPGA data movement and hardware acceleration**, where control pulses initiate operations inside FPGA datapaths.

Possible extensions include:

* Address-controlled memory access
* Explicit read data return
* ACK/DONE handshaking
* Wider data paths
* Multiple operation codes
* FPGA arithmetic units
* DSP operations
* DMA-style transfers
* Programmable compute accelerators
* MCU-controlled FPGA coprocessors

---

## Tools

* **Verilog HDL**
* **AMD/Xilinx Vivado**
* **Vivado ILA**
* **FPGA development board**

---

## Author

**Bhanusivaprakash Lekkala**

RTL / FPGA Design Engineer focused on computational digital hardware, FPGA acceleration, and hardware–software interfaces.
