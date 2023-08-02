/***********************/

EXCH_AREA_SIZE = PAGE_SIZE;
EXCH_AREA_ORG  = RAM_1_ORG + RAM_1_SIZE - EXCH_AREA_SIZE;

/***********************/
/* boostrap partitioning */
BOOTSTRAP_ORG  = FLASH_ORG;
BOOTSTRAP_SIZE = PAGE_SIZE*2;

USER_PART_ORG  = BOOTSTRAP_ORG + BOOTSTRAP_SIZE;
USER_PART_SIZE = FLASH_SIZE - BOOTSTRAP_SIZE;

HEADER_SIZE = 512; /* required by VTOR re-mapping alignment*/

/***********************/
/* Constant */

MAGIC_WORD_8 = 0x0DAC0DAC; /* o dac o dac */
MAGIC_WORD_9 = 0xD0D0CACA; /* Dodo Caca */

MIN_STACK_SIZE = 0x1000;
MIN_HEAP_SIZE = 0x800;

/* Entry Point */
ENTRY(Reset_Handler);

/***************************/

PROVIDE(__header_size__ = HEADER_SIZE);
PROVIDE(__exchange_area_org__ = EXCH_AREA_ORG);
PROVIDE(__exchange_area_size__ = EXCH_AREA_SIZE);

/***************************/