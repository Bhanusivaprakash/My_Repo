# FPGA Arithmetic Coprocessor

## Overview

This project implements a simple arithmetic coprocessor in Verilog. The coprocessor contains an internal memory and supports multiple arithmetic and utility operations through an instruction interface.

The design is intended as a learning-oriented hardware accelerator that can be controlled by a microcontroller or processor.

---

## Features

### Arithmetic Operations

| Opcode | Operation |
| ------ | --------- |
| 0000   | CLEAR     |
| 0001   | ADD       |
| 0010   | SUB       |
| 0011   | MUL       |
| 0100   | DIV       |
| 0110   | ABS       |
| 10000  | MAC       |

### Memory Operations

| Opcode | Operation |
| ------ | --------- |
| 1110   | SWAP      |
| 1111   | STORE     |

---

## Architecture

### Internal Memory

```verilog
reg signed [15:0] memory [0:31];
```

* 32 locations
* 16-bit signed values

### Result Register

```verilog
reg signed [31:0] result;
```

Stores the latest operation result.

### Accumulator

```verilog
reg signed [31:0] acc;
```

Used for iterative arithmetic operations.

### Execution Pointer

```verilog
reg [4:0] execution_ptr;
```

Tracks the current operand being processed.

---

## Interface

### Inputs

| Signal         | Width | Description            |
| -------------- | ----- | ---------------------- |
| clk            | 1     | System clock           |
| rst            | 1     | Reset                  |
| data_in        | 16    | Input data             |
| write_enable   | 1     | Memory write pulse     |
| read_enable    | 1     | Result read pulse      |
| operation      | 5     | Opcode                 |
| address        | 5     | Memory address         |
| address_enable | 1     | Load execution address |
| execute        | 1     | Start operation        |

### Outputs

| Signal   | Width | Description        |
| -------- | ----- | ------------------ |
| data_out | 16    | Result output      |
| done     | 1     | Operation complete |
| error    | 1     | Error flag         |

---

## Supported Operations

### ADD

Adds all values from:

```text
memory[0] → memory[write_ptr-1]
```

Example:

```text
4 + 2 + 8 = 14
```

---

### SUB

Performs sequential subtraction:

```text
A - B - C - ...
```

Example:

```text
10 - 2 - 3 = 5
```

---

### MUL

Performs sequential multiplication:

```text
A × B × C × ...
```

Example:

```text
4 × 2 × 3 = 24
```

---

### DIV

Performs sequential division:

```text
A ÷ B ÷ C ...
```

Example:

```text
24 ÷ 2 ÷ 3 = 4
```

Division by zero sets:

```verilog
error = 1
```

---

### ABS

Computes absolute value.

Example:

```text
memory[0] = -15

Result = 15
```

---

### STORE

Stores the current result into memory.

Example:

```text
result = 36

STORE address 4

memory[4] = 36
```

---

### SWAP

Swaps adjacent memory locations.

Example:

```text
Address = 2

Before:
memory[2] = 10
memory[3] = 20

After:
memory[2] = 20
memory[3] = 10
```

---

### CLEAR

Resets:

```text
result
accumulator
sum accumulator
product accumulator
```

---

## Typical Execution Flow

### 1. Write Data

```verilog
write(4);
write(2);
```

Memory:

```text
memory[0] = 4
memory[1] = 2
```

---

### 2. Load Instruction

```verilog
load(4'b0100, 0);
```

Loads DIV instruction.

---

### 3. Execute

```verilog
exec();
```

Runs operation until:

```verilog
done == 1
```

---

### 4. Read Result

```verilog
read();
```

Returns:

```text
result = 2
```

---

## Example Simulation

Input:

```verilog
write(4);
write(2);

load(4'b0100,0); // DIV
exec();

read();
```

Output:

```text
WRITE: ptr=0 data=4
WRITE: ptr=1 data=2

DIV: ptr=1 val=2

result=2

Error = 0
Done = 1
```

---

## Testbench

The included testbench provides helper tasks:

### Write Data

```verilog
write(value);
```

### Load Instruction

```verilog
load(opcode,address);
```

### Execute

```verilog
exec();
```

### Read Result

```verilog
read();
```

These tasks simplify simulation and emulate how an external processor would interact with the coprocessor.

---

## Future Improvements

* Instruction queue
* Matrix operations
* Vector operations
* Interrupt-based completion signal
* Hardware accelerator interface for STM32/FPGA systems

---
