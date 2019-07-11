/* Linker script for Silicon Labs EFR32MG1P devices */
/*                                                                  */
/* This file is subject to the license terms as defined in ARM's    */
/* CMSIS END USER LICENSE AGREEMENT.pdf, governing the use of       */
/* Example Code.                                                    */
/*                                                                  */
/* Silicon Laboratories, Inc. 2015                                  */
/*                                                                  */
/* Version 4.1.1 */
/*                                                                  */
MEMORY
{
  FLASH_BOOTLOADER (rx) : ORIGIN = 0x0FE10000, LENGTH = 0x4000
  FLASH (rx) : ORIGIN = 0x0000, LENGTH = 0x00080000-0x0000
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 0x00010000
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __copy_table_start__
 *   __copy_table_end__
 *   __zero_table_start__
 *   __zero_table_end__
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 *   __Vectors_End
 *   __Vectors_Size
 */
ENTRY(Reset_Handler)

SECTIONS
{
  .text_bootloader :
  {
	KEEP(*(.binbootloader*))
    /* Align to flash page boundary*/
    . = ALIGN(0x4000);
  }> FLASH_BOOTLOADER 
  .text_apploader :
  {
	KEEP(*(.binapploader*))
  }> FLASH  
  .text_signature :
  {
    . = ALIGN(2048);
  }> FLASH  
    
  __text_start__ = .;

  .text_application :
  {	
    KEEP(*(.vectors))
    __Vectors_End = .;
    __Vectors_Size = __Vectors_End - __Vectors;
    __end__ = .;
    KEEP(*(.application_properties))
	KEEP(*(.gecko_configuration))
	KEEP(*(.xo_configuration))  
	KEEP(*(.gatt_header))
	KEEP(*(.gatt_data))			
    *(.text*)

    KEEP(*(.init))
    KEEP(*(.fini))

    /* .ctors */
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    /* .dtors */
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)

    *(.rodata*)

    KEEP(*(.eh_frame*))
	. = ALIGN(4);
  } > FLASH  

  .text_application_ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  __exidx_start = .;
  .text_application_ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH
  __exidx_end = .;
  
  __etext = .;
  /* .stack_dummy section doesn't contains any symbols. It is only
   * used for linker to calculate size of stack sections, and assign
   * values to stack symbols later */
  .stack_dummy (COPY):
  {
    KEEP(*(.stack*))
  } > RAM
  
  /* Set stack top to end of RAM, and stack limit move down by
   * size of stack_dummy section */
  __StackTop = ORIGIN(RAM) + SIZEOF(.stack_dummy);
  __StackLimit = ORIGIN(RAM);
  PROVIDE(__stack = __StackTop);

  .text_application_data : AT (__etext)
  {
    __data_start__ = .;
    *(vtable)
    *(.data*)
    . = ALIGN (4);
    *(.ram)

    . = ALIGN(4);
    /* preinit data */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    /* init data */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);

    . = ALIGN(4);
    /* finit data */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    KEEP(*(.jcr*))
    . = ALIGN(4);
    /* All data end */
    __data_end__ = .;

  } > RAM
  
  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > RAM

  .heap (COPY):
  {
    __HeapBase = .;
    __end__ = .;
    end = __end__;
    _end = __end__;
    KEEP(*(.heap*))
    __HeapLimit = .;
  } > RAM

  /* .nvm_dummy section doesn't contains any symbols. It is only
   * used for linker to calculate size of nvm section, and assign
   * values to nvm symbols later */
  .nvm_dummy (DSECT):
  {
    KEEP(*(.simee));
  } > FLASH
  
  /* Set NVM to end of FLASH*/
  __nvm3Base = 0x00080000- SIZEOF(.nvm_dummy);  
  ASSERT((__etext + SIZEOF(.text_application_data)) <= __nvm3Base, "FLASH memory overlapped with NVM section.")
}