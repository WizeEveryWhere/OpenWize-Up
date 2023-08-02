/***********************/

EXCH_AREA_SIZE = PAGE_SIZE;
EXCH_AREA_ORG  = RAM_1_ORG + RAM_1_SIZE - EXCH_AREA_SIZE;

/***************************/

BOOTSTRAP_ORG  = FLASH_ORG;
BOOTSTRAP_SIZE = PAGE_SIZE*2;

/* USER_PART_ORG  = BOOTSTRAP_ORG + BOOTSTRAP_SIZE; */
/* USER_PART_SIZE = FLASH_SIZE - BOOTSTRAP_SIZE; */

USER_PART_ORG  = FLASH_ORG;
USER_PART_SIZE = 84*PAGE_SIZE;

HEADER_SIZE = 512; /* required by VTOR re-mapping alignment*/

/***************************/
/* Application memory view */
FLASH_IMG_ORG = USER_PART_ORG;
FLASH_IMG_SIZE = 84*PAGE_SIZE;

NVM_IMG_ORG = FLASH_IMG_ORG + FLASH_IMG_SIZE  + BOOTSTRAP_SIZE;
NVM_IMG_SIZE = 2*PAGE_SIZE;

RAM_IMG_ORG = RAM_1_ORG;
RAM_IMG_SIZE = RAM_1_SIZE - EXCH_AREA_SIZE;

RAM2_IMG_ORG = RAM_2_ORG;
RAM2_IMG_SIZE = RAM_2_SIZE;

/****************/
/* Sanity check */
NVM_ALIGN = NVM_IMG_SIZE % PAGE_SIZE;
ASSERT( NVM_ALIGN == 0, "!!! NVM part is not aligned on PAGE !!!");
NVM_ALIGN = NVM_IMG_ORG % PAGE_SIZE;
ASSERT( NVM_ALIGN == 0, "!!! NVM part is not aligned on PAGE !!!");

/****************/
/* Application constant */
MIN_STACK_SIZE = 0x1000;
MIN_HEAP_SIZE = 0x800;

/* Entry Point */
ENTRY(Reset_Handler);

/***************************/

PROVIDE(__header_size__ = HEADER_SIZE);
PROVIDE(__exchange_area_org__ = EXCH_AREA_ORG);
PROVIDE(__exchange_area_size__ = EXCH_AREA_SIZE);

/***************************/