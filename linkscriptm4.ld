ENTRY(Reset_Handler)

_estack = 0x10040000;

_Min_Heap_Size = 0x1000;
_Min_Stack_Size = 0x400;

MEMORY
{
	m_interrupts	(RX) 	: ORIGIN = 0x00000000, LENGTH = 0x00000440
	m_text			(RX)	: ORIGIN = 0x10000000, LENGTH = 0x00040000
	m_data			(RW)	: ORIGIN = 0xC0100000, LENGTH = 2M
	m_audio			(RW)	: ORIGIN = 0xC0300000, LENGTH = 508M
}

SECTIONS
{
	.isr_vector :
	{
		. = ALIGN(8);
		KEEP(*(.isr_vector))
		. = ALIGN(8);
	}	> m_interrupts

	.text :
	{
		. = ALIGN(8);
		*(.text)
		*(.text*)
		*(.glue_7)
		*(.glue_7t)
		*(.eh_frame)
		
		KEEP (*(.init))
		KEEP (*(.fini))
		
		. = ALIGN(8);
		_etext = .;
	} > m_text

	.rodata :
	{
		. = ALIGN(8);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(8);
	} > m_text
	
	.ARM.extab :
	{
		. = ALIGN(8);
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		. = ALIGN(8);
	} > m_text
	
	.ARM : 
	{
		. = ALIGN(8);
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
		. = ALIGN(8);
	} > m_text
	
	.preinit_array :
	{
		. = ALIGN(8);
		PROVIDE_HIDDEN(__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN(__preinit_array_end = .);
		. = ALIGN(8);
	} > m_text
	
	.init_array :
	{
		. = ALIGN(8);
		PROVIDE_HIDDEN(__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN(__init_array_end = .);
		. = ALIGN(8);
	} > m_text
	
	.fini_array :
	{
		. = ALIGN(8);
    	PROVIDE_HIDDEN (__fini_array_start = .);
    	KEEP (*(SORT(.fini_array.*)))
    	KEEP (*(.fini_array*))
    	PROVIDE_HIDDEN (__fini_array_end = .);
    	. = ALIGN(8);
	} > m_text
	
	__DATA_ROM = .;
	_sidata = LOADADDR(.data);
	
	.data : AT(__DATA_ROM)
	{
		. = ALIGN(8);
		_sdata = .;
		*(.data)
		*(.data*)
		
		. = ALIGN(8);
		_edata = .;
	} > m_data
	
	__DATA_END = __DATA_ROM + (_edata - _sdata);
 	text_end = ORIGIN(m_text) + LENGTH(m_text);
	ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")
	
	.resource_table :
  	{
    	. = ALIGN(4);
    	KEEP (*(.resource_table*))
    	. = ALIGN(4);
  	} > m_data
	
	. = ALIGN(8);
	
	.bss :
	{
		_sbss = .;
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss*)
		*(COMMON)
		
		. = ALIGN(8);
		_ebss = .;
		__bss_end__ = _ebss;
	} > m_data
	
	._user_heap_stack :
	{
		. = ALIGN(8);
		PROVIDE( end = .);
		PROVIDE( _end = .);
		_sheap = .;
		. = . + _Min_Heap_Size;
		_eheap = .;
		. = . + _Min_Stack_Size;
		. = ALIGN(8);
	} > m_data
	
	/DISCARD/ :
	{
		libc.a(*)
		libm.a(*)
		libgcc.a(*)
	}
	
	.ARM.attributes 0 : {*(.ARM.attributes)}

}