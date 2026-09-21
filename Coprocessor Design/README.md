# FPGA Arithmetic Coprocessor

A programmable arithmetic coprocessor implemented in **Verilog RTL** on an AMD/Xilinx Artix-7 FPGA. The design provides a memory-backed arithmetic datapath that can be controlled by an external microcontroller through a 16-bit bidirectional interface.

The coprocessor performs arithmetic operations inside the FPGA and returns results to the host, demonstrating RTL datapath design, fixed-point arithmetic, memory operations, control logic, clock-domain synchronization, and FPGA hardware integration.

## Architecture

```text
                 STM32 / Host MCU
                       │
                 16-bit Data Bus
                       │
        ┌──────────────┴──────────────┐
        │       Control Interface     │
        │                             │
        │  WRITE / READ / ADDRESS    │
        │  EXECUTE / OPERATION       │
        └──────────────┬──────────────┘
                       │
                ┌──────▼──────┐
                │   Control   │
                │    Logic    │
                └──────┬──────┘
                       │
              ┌────────▼────────┐
              │  Internal RAM   │
              │  32 × 32-bit    │
              └────────┬────────┘
                       │
              ┌────────▼────────┐
              │ Arithmetic      │
              │ Datapath        │
              │                 │
              │ ADD / SUB / MUL │
              │ MAC / ABS       │
              └────────┬────────┘
                       │
              ┌────────▼────────┐
              │ Result / Acc.   │
              │    32-bit       │
              └─────────────────┘
```

<img width="1280" height="853" alt="image" src="https://github.com/user-attachments/assets/e3dd70b3-8387-45d2-96dd-4a9c77dbaeb9" />


## Features

* Verilog RTL implementation
* 32 × 32-bit internal memory
* 32-bit signed arithmetic datapath
* 32-bit result register
* 32-bit accumulator
* 16-bit bidirectional host data bus
* Two 16-bit transfers for 32-bit memory/result values
* Programmable operation selection
* Sequential arithmetic processing
* Fixed-point multiplication with 16-bit fractional scaling
* Asynchronous host control inputs synchronized into the FPGA clock domain
* `ACK`, `DONE`, and `ERROR` control signals
* RTL simulation and testbench support
* Integrated Vivado ILA debug probes

## Supported Operations

| Opcode  | Operation | Description                              |
| ------- | --------- | ---------------------------------------- |
| `0000`  | CLEAR     | Clears arithmetic accumulators           |
| `0001`  | ADD       | Sequentially adds memory operands        |
| `0010`  | SUB       | Sequential subtraction                   |
| `0011`  | MUL       | Sequential fixed-point multiplication    |
| `0110`  | ABS       | Absolute value of addressed memory value |
| `1110`  | SWAP      | Swaps two adjacent memory locations      |
| `1111`  | STORE     | Stores the current result into memory    |
| `10000` | MAC       | Multiply-accumulate operation            |

## Internal Storage

The coprocessor contains:

```verilog
reg signed [31:0] memory [0:31];
```

This provides:

* 32 memory locations
* 32-bit signed data per location
* 5-bit addressing

The main datapath registers are:

```verilog
reg signed [31:0] acc;
reg signed [31:0] result;
```

`acc` maintains intermediate arithmetic state while `result` stores the latest completed result.

## Host Interface

The host communicates with the FPGA through a **16-bit bidirectional data bus**.

### Inputs

| Signal                 | Width | Description            |
| ---------------------- | ----: | ---------------------- |
| `clk`                  |     1 | FPGA system clock      |
| `rst`                  |     1 | Reset                  |
| `data`                 |    16 | Bidirectional data bus |
| `write_enable_async`   |     1 | Host write request     |
| `read_enable_async`    |     1 | Host read request      |
| `operation`            |     5 | Operation opcode       |
| `address_enable_async` |     1 | Load execution address |
| `address`              |     5 | Memory address         |
| `execute_async`        |     1 | Start operation        |

### Outputs

| Signal  | Width | Description                     |
| ------- | ----: | ------------------------------- |
| `data`  |    16 | Result data returned to host    |
| `done`  |     1 | Operation completion            |
| `ack`   |     1 | Bus transaction acknowledgement |
| `error` |     1 | Error indication                |

## 32-bit Data Transfer

Because the external interface is 16 bits wide while the internal datapath is 32 bits wide, each 32-bit value is transferred in two 16-bit transactions.

### Write

```text
Host
 │
 ├── Lower 16 bits ──► memory[address][15:0]
 │
 └── Upper 16 bits ──► memory[address][31:16]
```

### Read

```text
result[15:0]  ──► Host
result[31:16] ──► Host
```

`word_select` controls the lower/upper half-word transfer.

## Clock-Domain Synchronization

The host control signals are treated as asynchronous inputs and synchronized into the FPGA clock domain using two-stage flip-flop synchronizers.

Implemented for:

* Write enable
* Read enable
* Address enable
* Execute

Example:

```verilog
always @(posedge clk) begin
    write_ff1 <= write_enable_async;
    write_ff2 <= write_ff1;
end
```

The synchronized signal is then used by the main sequential control logic.

This prevents asynchronous host control signals from being used directly by synchronous FPGA logic.

## Arithmetic Operations

### ADD

Sequentially accumulates values stored in memory.

For:

```text
memory[0] = 4
memory[1] = 2
memory[2] = 8
```

the operation produces:

```text
4 + 2 + 8 = 14
```

### SUB

Performs sequential subtraction:

```text
A - B - C - ...
```

Example:

```text
10 - 2 - 3 = 5
```

### MUL

Performs sequential fixed-point multiplication.

The multiplication produces a 64-bit intermediate value:

```verilog
temp_res = prod_acc * memory[execution_ptr];
```

The result is then shifted right by 16 bits:

```verilog
prod_acc <= temp_res >>> 16;
```

This implements a **16-bit fractional fixed-point scaling scheme**.

### MAC

Performs multiply-accumulate processing over pairs of memory operands:

```text
acc = acc + (A × B)
```

with the multiplication scaled using a 16-bit fractional shift.

### ABS

Computes the absolute value of the addressed signed 32-bit memory element.

```verilog
result <= (memory[address][31]) ?
          -memory[address] :
           memory[address];
```

### STORE

Writes the current `result` back into the selected memory location.

```verilog
memory[address] <= result;
```

### SWAP

Swaps two adjacent memory locations:

```text
memory[address]
        ↕
memory[address - 1]
```

For example:

```text
Before:
memory[2] = 10
memory[3] = 20

SWAP address 3

After:
memory[2] = 20
memory[3] = 10
```

### CLEAR

Resets the arithmetic accumulators:

```verilog
sum_acc  <= 0;
prod_acc <= 32'h00010000;
```

`32'h00010000` represents **1.0 in Q16.16-style fixed-point representation**, rather than integer value `1`.

## Execution Model

A typical host transaction follows this sequence:

```text
1. Write operand
       ↓
2. Write operand
       ↓
3. Select operation
       ↓
4. Load execution address
       ↓
5. Assert EXECUTE
       ↓
6. FPGA processes operation
       ↓
7. DONE asserted
       ↓
8. Host reads result
```

The design uses `ACK` to acknowledge bus transactions and `DONE` to indicate completion of an operation.

## Verification

The RTL was developed with simulation support and a testbench interface that models host-side transactions.

The testbench provides transaction-level helper operations for:

```text
WRITE
LOAD / operation selection
EXECUTE
READ
```

This allows the FPGA interface to be exercised in a manner similar to an external microcontroller.

## FPGA Debugging

The design includes a Vivado **Integrated Logic Analyzer (ILA)** instance.

Debugged signals include:

* `result`
* Internal memory
* `operation`
* `address`
* `reset`
* `write_enable`
* `read_enable`
* `address_enable`
* `execute`
* `done`
* `ack`

This allows internal datapath and host-interface behavior to be observed directly on FPGA hardware.

## Hardware

Target platform:

**Digilent Cmod A7-35T**

FPGA:

**AMD/Xilinx Artix-7 XC7A35T**

The coprocessor was synthesized, implemented, and hardware-tested on the FPGA.

An **STM32F429-Discovery** board can act as the external host controller, providing operands, control signals, and reading computation results.

## Tools

* Verilog HDL
* Icarus Verilog
* AMD/Xilinx Vivado
* Vivado ILA
* Digilent Cmod A7-35T
* STM32F429-Discovery

## What This Project Demonstrates

* RTL architecture and implementation
* Arithmetic datapath design
* Signed fixed-point arithmetic
* Memory-based computation
* Sequential datapath control
* FPGA implementation
* Asynchronous control-input synchronization
* Bidirectional FPGA interfaces
* MCU–FPGA hardware integration
* RTL simulation and verification
* On-chip FPGA debugging
* Hardware validation

## Future Extensions

Potential extensions include:

* Vector arithmetic operations
* Matrix operations
* Expanded instruction set
* Instruction queue
* Interrupt-driven completion
* Wider datapaths
* Dedicated hardware accelerator interfaces
* Integration with higher-level compute kernels
