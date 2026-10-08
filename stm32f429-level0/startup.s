.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global _sidata
.global _sdata
.global _edata
.global _sbss
.global _ebss
.global _estack

.section .isr_vector, "a", %progbits

.word _estack
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
		CMP R1, R3
		BEQ initialize_data

		LDR R2, [R0]
		STR R2, [R1]

		ADD R0, R0, #4
		ADD R1, R1, #4

		B copy_data

	initialize_data:
		CMP R4, R5
		BEQ boot_main

		STR R6, [R4]
		ADD R4, R4, #4

		B initialize_data

	boot_main:
		B main

	
