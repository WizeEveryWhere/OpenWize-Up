/* Memories definition */
MEMORY
{
  FLASH	   (rx)	 : ORIGIN = FLASH_IMG_ORG,	LENGTH = FLASH_IMG_SIZE
  NVM      (r)   : ORIGIN = NVM_IMG_ORG,    LENGTH = NVM_IMG_SIZE
  RAM	   (xrw) : ORIGIN = RAM_IMG_ORG,	LENGTH = RAM_IMG_SIZE
  RAM2	   (xrw) : ORIGIN = RAM2_IMG_ORG,	LENGTH = RAM2_IMG_SIZE
}

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

_Min_Heap_Size = MIN_HEAP_SIZE;   /* required amount of heap  */
_Min_Stack_Size = MIN_STACK_SIZE; /* required amount of stack */

__nvm_org__ = ORIGIN(NVM);
__nvm_size__ = LENGTH(NVM);

/* Sections */
SECTIONS
{
  /* The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)
    _etext = .;        /* define a global symbols at end of code */
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) *
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .data : 
  {
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */    
  } >RAM AT> FLASH
  
  .bss :
  {
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM
  
  .nvm :
  {
    . = ALIGN(4);
    *(.nvm)        
    *(.nvm*)       
    . = ALIGN(4);
  } >NVM

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}