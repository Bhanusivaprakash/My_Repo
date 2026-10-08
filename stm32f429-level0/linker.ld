ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2M
	RAM	  (xrw) : ORIGIN = 0x20000000, LENGTH = 192K
}

SECTIONS
{
	/* isr_vector output from the linker, KEEP ensures that isr_vector is not discarded by the linker as it has no labels*/
	.isr_vector 0x08000000 :
	{
		KEEP (*(.isr_vector))	
	} > FLASH


	/* Text section output from the linker, exists in FLASH*/
	.text 0x08000008 :
	{
		*(.text)
	} > FLASH	

	/* Data section, contains the variable and constant values used in user written code, sits in RAM, while its initialization happens from FLASH
	as boot-up happens with values moving from FLASH to RAM.*/
	
	.data :
	{
		_sdata = .;		/* Start of .data in RAM */
		*(.data)
		_edata = .;		/* End of .data in RAM */
	} > RAM AT > FLASH	/* Syntax that defines RAM <-> FLASH relationship */

	_sidata = LOADADDR(.data);	/* Defines the address to be used by the startup.s, to load data from start of the data in FLASH into .data, which is in RAM. */

	.bss :	/* Contains the un-initialized values like int x; rather than int x = 5; */
	{
		_sbss = .;		
		*(.bss)
		_ebss = .;
	} > RAM
}
