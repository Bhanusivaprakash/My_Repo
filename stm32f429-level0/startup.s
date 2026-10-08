.global Reset_Handler
.global _sidata
.global _sdata
.global _edata
.global _sbss
.global _ebss

.section .isr_vector

.word 0x20040000
.word Reset_Handler

.section .text

.thumb_func
Reset_Handler:
	LDR R0, =_sidata	// Address of data in FLASH
	LDR R1, =_sdata		// Destination address of data in RAM
	LDR R3, =_edata		// End of destination address of data in RAM

	LDR R4, =_sbss
	LDR R5, =_ebss

	MOV R6, #0

	copy_data:
		LDR R2, [R0]		// Load the 32-bit value present in the address held by R0

		STR R2, [R1]		// STORE the 32-bit value into RAM location of address held by R1
		
		ADD R0, R0, #4
		ADD R1, R1, #4		// Increasing R0 and R1 by 4 bytes to write the upcoming data from FLASH into RAM

		CMP R1, R3			// Checking if the CPU has reached _edata

		BNE copy_data		// If _edata not yet reached, repeat.

	initialize_data:
		STR R6, [R4]		// Writes 0 to the address held by R4
		ADD R4, R4, #4		// Increases R4 by 4 bytes, so that next address is also zeroed.
		CMP R4, R5			// Compares _sbss and _ebss. If not zero, repeat.
		BNE initialize_data

		B main

	
