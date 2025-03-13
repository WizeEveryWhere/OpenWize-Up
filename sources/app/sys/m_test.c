

#include "bsp.h"
#include "platform.h"
#include "auto_clk.h"

#include "stm32l4xx_hal.h"

#define USE_M_TEST_TRACE

#if defined (USE_M_TEST_TRACE)
#include <stdio.h>
#define TRACE_M_TEST(...) do { printf(__VA_ARGS__); } while(0)
#else
#define TRACE_M_TEST(...) {}
#endif

#define _IS_DBG_STATE_() (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
#define IS_DBG_STATE() (_IS_DBG_STATE_())?(1):(0)

void m_test()
{
	uint8_t ret;
	ret = BSP_Uart_Open(SERIAL_ID_LOG);
	ret |= BSP_Uart_Init(SERIAL_ID_LOG, ' ', UART_MODE_NONE);
	TRACE_M_TEST("MAIN Init: status %d\r\n",ret);

	if (_IS_DBG_STATE_())
	{
		TRACE_M_TEST("Debugger connected\r\n");
	}

#ifdef M_TEST_TRY_ACCMEAS
	Sys_PreInit();
	AutoClk_MeasAccurate();
#endif

extern uint32_t EX_PHY_SelfTest(void);
	uint32_t eStatus;
	eStatus = EX_PHY_SelfTest();
	TRACE_M_TEST("SelfTest status : %x\r\n", eStatus);


extern union clk_cfg_param_u g_sClk;
extern uint32_t Setup_GetClockCfg(void);

	union clk_cfg_param_u clk;
	clk.clk_cfg = Setup_GetClockCfg();
	//TRACE_M_TEST("LSE failed to start\r\n");
	//TRACE_M_TEST("Set LSC to %d\r\n", u8Lsc);
	if (g_sClk.osc.source != clk.osc.source)
	{
		//TRACE_M_TEST("OSC %d failed to start\r\n", (uint8_t)clk.osc.source);
		TRACE_M_TEST("%s failed to start\r\n", osc_str[(uint8_t)clk.osc.source]);
	}
	//TRACE_M_TEST("Set OSC to %d\r\n", (uint8_t)g_sClk.osc.source);
	TRACE_M_TEST("Set to %s\r\n", osc_str[(uint8_t)g_sClk.osc.source]);

	ret = BSP_Uart_Open(SERIAL_ID_COM);
	ret |= BSP_Uart_Init(SERIAL_ID_COM, '\r', UART_MODE_NONE);

	char str[50];
	size_t len = snprintf(str, 49, "Init: status %d\r\n", ret);
	BSP_Console_Send(str, (uint16_t)len);



}
