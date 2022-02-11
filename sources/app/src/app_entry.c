
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
  	Sys_Init();
  	App_Init();
  	Sys_Start();
}

/******************************************************************************/
void* hTask;
extern void Atci_Task(void const * argument);
#define APP_TASK_NAME atci
#define APP_TASK_FCT Atci_Task
#define APP_STACK_SIZE 800
#define APP_PRIORITY (UBaseType_t)(tskIDLE_PRIORITY+1)
SYS_TASK_CREATE_DEF(atci, APP_STACK_SIZE, APP_PRIORITY);


void App_Init(void)
{
	hTask = SYS_TASK_CREATE_CALL(atci, APP_TASK_FCT, NULL);
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif
