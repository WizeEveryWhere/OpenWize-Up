
#ifdef __cplusplus
extern "C" {
#endif

#include "app_entry.h"
#include "atci.h"

extern void Sys_Init(void);
extern void Sys_Start(void);

void App_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
void app_entry(void)
{
  	/* Show the welcome message */
#ifndef HAS_NO_BANNER
  	printf("\n###########################################################\n");
  	printf("%s\n", WIZE_ALLIANCE_BANNER);
  	printf("\n###########################################################\n");
#endif
  	Sys_Init();
  	App_Init();
  	Sys_Start();
}

/******************************************************************************/

extern void Atci_Task(void const * argument);

#define APP_MAIN_TASK_NAME atci
#define APP_MAIN_TASK_FCT Atci_Task

#define APP_MAIN_STACK_SIZE 800
#define APP_MAIN_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+1)

SYS_TASK_CREATE_DEF(APP_MAIN_TASK_NAME, APP_MAIN_STACK_SIZE, APP_MAIN_PRIORITY);
TaskHandle_t hMain;

void App_Init(void)
{
	hMain = SYS_TASK_CREATE_CALL(APP_MAIN_TASK_NAME, APP_MAIN_TASK_FCT, NULL);
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif
