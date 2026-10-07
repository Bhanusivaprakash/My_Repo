SECTIONS
{
	.isr_vector:
	{
		KEEP (*(.isr_vector))	
	} > FLASH
	
	.text:
	{
		*(.text)
	} > FLASH	
}


MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2M
}
