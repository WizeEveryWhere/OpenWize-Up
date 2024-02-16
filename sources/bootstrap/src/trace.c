#if defined(__cplusplus)
extern "C"
{
#endif

#include "def.h"
#include "trace.h"
#include "uart.h"

#define msg_enter "\r\nBootstrap\r\n"
#define msg_swap_enter "Swapping img\r\n"
#define msg_br_app "Go to App\r\n"
#define msg_reboot "Reboot\r\n"
#define msg_failure "Failure\r\n"
#define msg_not_bootable "Img can't boot\r\n"
#define msg_br_bl2 "Go to BL2\r\n"
#define msg_br_sys "Go to SYS\r\n"
#define msg_req_0 "REQ_NONE\r\n"
#define msg_req_1 "REQ_UPDATE\r\n"
#define msg_req_2 "REQ_LOCAL\r\n"
#define msg_req_x "REQ_?????\r\n"
#define msg_RDP_0 "OB-RDP0\r\n"
#define msg_RDP_1 "OB-RDP1\r\n"
#define msg_RDP_2 "OB-RDP2\r\n"

static const char * const trace_str[] =
{
	[TRACE_MSG_ENTER] = msg_enter,
	[TRACE_MSG_SWAP] = msg_swap_enter,
	[TRACE_MSG_REBOOT] = msg_reboot,
	// ---
	[TRACE_MSG_APP] = msg_br_app,
	[TRACE_MSG_BL2] = msg_br_bl2,
	[TRACE_MSG_SYS] = msg_br_sys,
	// ---
	[TRACE_MSG_REQ_NONE] = msg_req_0,
	[TRACE_MSG_REQ_UPDATE] = msg_req_1,
	[TRACE_MSG_REQ_LOCAL] = msg_req_2,
	[TRACE_MSG_REQ_UNK] = msg_req_x,
	[TRACE_MSG_FAILURE] = msg_failure,
	[TRACE_MSG_NOT_BOOTABLE] = msg_not_bootable,
	// ---
#ifdef HAS_RDP_TRACE
	[TRACE_MSG_RDP0] = msg_RDP_0,
	[TRACE_MSG_RDP1] = msg_RDP_1,
	[TRACE_MSG_RDP2] = msg_RDP_2,
#endif
};

static const uint8_t trace_len[TRACE_MSG_NB] =
{
	[TRACE_MSG_ENTER] = sizeof(msg_enter),
	[TRACE_MSG_SWAP] = sizeof(msg_swap_enter),
	[TRACE_MSG_REBOOT] = sizeof(msg_reboot),
	// ---
	[TRACE_MSG_APP] = sizeof(msg_br_app),
	[TRACE_MSG_BL2] = sizeof(msg_br_bl2),
	[TRACE_MSG_SYS] = sizeof(msg_br_sys),
	// ---
	[TRACE_MSG_REQ_NONE] = sizeof(msg_req_0),
	[TRACE_MSG_REQ_UPDATE] = sizeof(msg_req_1),
	[TRACE_MSG_REQ_LOCAL] = sizeof(msg_req_2),
	[TRACE_MSG_REQ_UNK] = sizeof(msg_req_x),
	[TRACE_MSG_FAILURE] = sizeof(msg_failure),
	[TRACE_MSG_NOT_BOOTABLE] = sizeof(msg_not_bootable),
	// ---
#ifdef HAS_RDP_TRACE
	[TRACE_MSG_RDP0] = sizeof(msg_RDP_0),
	[TRACE_MSG_RDP1] = sizeof(msg_RDP_1),
	[TRACE_MSG_RDP2] = sizeof(msg_RDP_2),
#endif
};

uint8_t trace_enable;

void RAMFUNCTION trace_init(void)
{
	TRACE_EN_GPIO_CLK_ENABLE();
	TRACE_EN_GPIO_SETUP();
	// Init Uart
	hal_uart_init();
	// Get GPIO PB6
	trace_enable = TRACE_IS_ENABLE();
}

void RAMFUNCTION trace(uint8_t id)
{
	register uint8_t i;
	register char *str = trace_str[id];

	if (id < TRACE_MSG_NB)
	{
		for (i = 0; i <  trace_len[id]; i++)
		{
			if (trace_enable)
			{
				hal_uart_send(str[i]);
			}
		}
	}
}

void RAMFUNCTION trace_fini(void)
{
	//LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	LL_AHB2_GRP1_ForceReset(LL_AHB2_GRP1_PERIPH_GPIOB | LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_AHB2_GRP1_ReleaseReset(LL_AHB2_GRP1_PERIPH_GPIOB | LL_AHB2_GRP1_PERIPH_GPIOA);
	USARTx_DeInit();
}
/******************************************************************************/
#if defined(__cplusplus)
}
#endif
