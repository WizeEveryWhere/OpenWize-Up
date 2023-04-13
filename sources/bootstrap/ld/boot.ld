/* Memories definition */
MEMORY
{
  BOOTSTRAP (rx)  : ORIGIN = BOOTSTRAP_ORG,  LENGTH = BOOTSTRAP_SIZE
  USER_PART (rx)  : ORIGIN = USER_PART_ORG,  LENGTH = USER_PART_SIZE
 
  RAM1      (xrw) : ORIGIN = RAM_1_ORG,  LENGTH = RAM_1_SIZE
  RAM2      (xrw) : ORIGIN = RAM_2_ORG,  LENGTH = RAM_2_SIZE 

  RAM_CONT  (xrw) : ORIGIN = RAM_1_ORG,    LENGTH = RAM_1_SIZE + RAM_2_SIZE 
}

REGION_ALIAS("RAM", RAM1);

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);

_Min_Heap_Size = MIN_HEAP_SIZE;   /* required amount of heap  */
_Min_Stack_Size = MIN_STACK_SIZE; /* required amount of stack */

/* Sections */
SECTIONS
{
  /* The startup code into "BOOTSTRAP" Rom type memory */
  .isr_vector :
  {
    . = ALIGN(4);
    __vtors_location = LOADADDR(.isr_vector);
    __vtors_start = ABSOLUTE(.);   
    KEEP(*(.isr_vector)) /* Startup code */
    KEEP(*(.isr_vector*)) 
    __vtors_end = ABSOLUTE(.);   
    . = ALIGN(4);
  } >BOOTSTRAP
  
    /* The program code and other data into "BOOTSTRAP" Rom type memory */
  .boot :
  {
    . = ALIGN(4);
    *(.boot)           /* .boot sections (code) */
    *(.boot*)          /* .boot* sections (code) */
    . = ALIGN(4);
    _eboot = .;        /* define a global symbols at end of code */
  } >BOOTSTRAP
  
  /* The program code and other data into "BOOTSTRAP" Rom type memory */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >BOOTSTRAP

  /* Constant data into "BOOTSTRAP" Rom type memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >BOOTSTRAP

  .ARM.extab   : { 
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >BOOTSTRAP
  
  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >BOOTSTRAP

/*
  .preinit_array     :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >BOOTSTRAP
  
  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >BOOTSTRAP
  
  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >BOOTSTRAP
*/
  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections into "RAM" Ram type memory */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
    
  } >RAM AT> BOOTSTRAP
  
  _eidata = _sidata + SIZEOF(.data);

  .got :
  {
		. = ALIGN(4);
	    __got = .;
	    __got_location = LOADADDR(.got) ;
	    __got_start = ABSOLUTE(.);
	    *(.got)
	    *(.got*)
	    /* .got            : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) }  */
	    __got_end = ABSOLUTE(.);
	    . = ALIGN(4);
    
	} >RAM AT> BOOTSTRAP
	__end_prog = __got_location + SIZEOF(.got);

  /* Uninitialized data section into "RAM" Ram type memory */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  /* User_heap_stack section, used to check that there is enough "RAM" Ram  type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
