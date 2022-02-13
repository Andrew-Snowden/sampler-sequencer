OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_Reset)

/*__HEAP_SIZE 		= 0x1F400000;	*/ /* 500 MB Heap Size */

__USER_STACK_SIZE	= 0x00010000;	/* 64  KB */
__AUX_STACK_SIZE	= 0x00010000;	/* 64  KB */

__IRQ_STACK_SIZE	= 0x00000100;	/* 256 B  */
__FIQ_STACK_SIZE	= 0x00000100;	/* 256 B  */
__SVC_STACK_SIZE	= 0x00002000;	/* 8   KB */

MEMORY
{
	ROM 	(rx) 	: ORIGIN = 0xDFF00000, LENGTH = 1M
	RAM 	(rxw)	: ORIGIN = 0xC0000000, LENGTH = 1M
}



SECTIONS
{
	/* For instructions */
	.text : {
				. = ALIGN(8);
				*(.vector_table)
				*(.text)
				*(.text*)
				
				*(.glue_7)
				*(.glue_7t)
				*(.eh_frame)

				KEEP (*(.init))
				KEEP (*(.fini))

       			. = ALIGN(8);
			} > ROM
			
	.rodata : 	{
					. = ALIGN(8);
					*(.rodata)
					*(.rodata*)
					. = ALIGN(8);
				} > ROM
				
	.ARM.extab	 : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >ROM
	
	.ARM : {
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
	} >ROM				
				
	.preinit_array :
				{
					. = ALIGN(8);
   					PROVIDE_HIDDEN (__preinit_array_start = .);
    				KEEP (*(.preinit_array*))
    				PROVIDE_HIDDEN (__preinit_array_end = .);
					. = ALIGN(8);
				} > ROM
				
	.init_array :
				{
					. = ALIGN(8);
    				PROVIDE_HIDDEN (__init_array_start = .);
    				KEEP (*(SORT(.init_array.*)))
    				KEEP (*(.init_array*))
    				PROVIDE_HIDDEN (__init_array_end = .);
					. = ALIGN(8);
				} > ROM
				
	.fini_array :
				{
					. = ALIGN(8);
    				PROVIDE_HIDDEN (__fini_array_start = .);
    				KEEP (*(SORT(.fini_array.*)))
    				KEEP (*(.fini_array*))
    				PROVIDE_HIDDEN (__fini_array_end = .);
					. = ALIGN(8);
				} > ROM
				
	_text_end = .;
	
	.stacks : 
	{
		/*. = 0xC0000000;*/
		_user_stack_start 	= .;
		_user_stack_end 	= _user_stack_start + __USER_STACK_SIZE;
		
		_aux_stack_start	= _user_stack_end;
		_aux_stack_end 		= _aux_stack_start + __AUX_STACK_SIZE;
		
		_fiq_stack_start 	= _aux_stack_end;
		_fiq_stack_end		= _fiq_stack_start + __FIQ_STACK_SIZE;
		
		_irq_stack_start 	= _fiq_stack_end;
		_irq_stack_end 		= _irq_stack_start + __IRQ_STACK_SIZE;
		
		_svc_stack_start	= _irq_stack_end;
		_svc_stack_end		= _svc_stack_start + __SVC_STACK_SIZE;
		
		_irq_stack_size 	= _irq_stack_end - _irq_stack_start;
		_fiq_stack_size 	= _fiq_stack_end - _fiq_stack_start;
		_user_stack_size 	= _user_stack_end - _user_stack_start;
		_aux_stack_size		= _aux_stack_end - _aux_stack_start;
		_svc_stack_size		= _svc_stack_end - _svc_stack_start;
		
		. = _svc_stack_end;
		. = ALIGN(8);
	} > RAM 
	
	.bss : {
		. = ALIGN(8);
		_bss_start = .;
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(8);
		_bss_end = .;
	} > RAM
	
	
	.data : AT(_text_end)
	{
		. = ALIGN(8);
		_data_start = .;
		*(.data)
		*(.data*)
		. = ALIGN(8);
		_data_end = .;
	} > RAM
	

/*	.heap(NOLOAD):
	{
		. = ALIGN(8);
		_sheap = .;
		. += __HEAP_SIZE;
		_eheap = .;
	} > RAM 
*/
	
	  /DISCARD/ :
	  {
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	  }

	.ARM.attributes 0 : { *(.ARM.attributes) }
}
