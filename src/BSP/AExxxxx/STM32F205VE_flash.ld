/*------------------------------------------------------------------------------*/
/*	Linker Script File															*/
/*	j.daheron																	*/
/*	STM32F205VE																	*/
/*	2013/01/21																	*/
/*------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------*/
/*	LIBRAIRIES																	*/
/*------------------------------------------------------------------------------*/

GROUP(libgcc.a libc.a)


/*------------------------------------------------------------------------------*/
/*	MEMOIRES																	*/
/*------------------------------------------------------------------------------*/
MEMORY
{
	FLASH (rx)		: ORIGIN = 0x08000000, LENGTH = 128K
	APP (rx)		: ORIGIN = 0x08020000, LENGTH = 1024K-128K
	SRAM (rwx)		: ORIGIN = 0x20000000, LENGTH = 128K
}


/*------------------------------------------------------------------------------*/
/*	STACK / HEAP																*/
/*------------------------------------------------------------------------------*/

/* Generate a link error if heap and stack don't fit into RAM */
_Heap_Size =	1*1024;		/* required amount of heap  */
_Stack_Size =	8*1024;		/* required amount of stack */

/* Highest address of the user mode stack */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);



/*------------------------------------------------------------------------------*/
/*	RESET																		*/
/*------------------------------------------------------------------------------*/
ENTRY(Reset_Handler)


/*------------------------------------------------------------------------------*/
/*	SECTIONS																	*/
/*------------------------------------------------------------------------------*/
SECTIONS
{

	/*----------------------------------------------------------*/
	/* MAIN TEXT SECTION										*/
	/*----------------------------------------------------------*/
	.text : ALIGN(4)
	{
		_text = .;

		FILL(0xff)
		KEEP(*(.isr_vector))

		. = ALIGN(4);

		*(.after_vectors*)
	    *(.text)           /* .text sections (code) */
		*(.text*)          /* .text* sections (code) */
		*(.rodata)         /* .rodata sections (constants, strings, etc.) */
		*(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
		*(.glue_7)         /* glue arm to thumb code */
		*(.glue_7t)        /* glue thumb to arm code */

		. = ALIGN(4);

		_etext = .;

	} > FLASH

	/*----------------------------------------------------------*/
	/*	ARM SECTIONS											*/
	/*----------------------------------------------------------*/
	/* for exception handling/unwind - some Newlib functions
	 * (in common with C++ and STDC++) use this.		*/

	.ARM.extab : ALIGN(4)
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)

	} > FLASH

	.ARM.exidx : ALIGN(4)
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)

	} > FLASH

	/*----------------------------------------------------------*/
	/* DEBUT DE POSITIONNEMENT DES DATA EN FLASH				*/
	/*----------------------------------------------------------*/
	_flash_data = .;

	/*----------------------------------------------------------*/
	/*	POSITIONS APPLICATIF ET BACKUP							*/
	/*----------------------------------------------------------*/
	.app : ALIGN(4)
	{
		_AppStartAddr = .;

	} > APP

	.bkp : ALIGN(4)
	{
		_BkpStartAddr = .;

	} > BKP

	/*----------------------------------------------------------*/
	/*	DATA SECTIONS											*/
	/*----------------------------------------------------------*/
	.data : ALIGN(4)
	{
		_data = .;
		*(.data*)
		_edata = .;

	} > SRAM  AT > FLASH


	/*----------------------------------------------------------*/
	/*	BSS SECTIONS											*/
	/*----------------------------------------------------------*/
	.bss : ALIGN(4)
	{
		_bss = .;
		*(.bss*)
		. = ALIGN(8);
		*(COMMON)
		_ebss = .;

	} > SRAM


	/*----------------------------------------------------------*/
	/*	DIVERS			 										*/
	/*----------------------------------------------------------*/

	/* User_heap_stack section, used to check that there is enough RAM left */
	._user_heap_stack :
	{
		. = ALIGN(4);
		. = . + _Heap_Size;
		. = . + _Stack_Size;
		. = ALIGN(4);
	} > SRAM

	/* Remove information from the standard libraries */
	/DISCARD/ :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}

	/* Calcul de la taille du hex en flash */
	_Size = _flash_data - _text + _edata - _data;
}




