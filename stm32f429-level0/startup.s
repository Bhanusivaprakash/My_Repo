.global Reset_Handler

.section .isr_vector

.word 0x20030000
.word Reset_Handler

.section .text

Reset_Handler:
	B Reset_Handler	// Keep executing the Reset_Handler 
