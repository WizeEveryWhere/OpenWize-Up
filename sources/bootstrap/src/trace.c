
#include "trace.h"
#include "uart.h"

#define msg_enter "\r\nBootstrap\r\n"
#define msg_swap_enter "Swapping img\r\n"
#define msg_br_app "Go to App\r\n"
#define msg_reboot "Reboot\r\n"
#define msg_failure "Failure\r\n"
#define msg_br_bl2 "Go to BL2\r\n"
#define msg_br_sys "Go to SYS\r\n"
#define msg_req_0 "REQ_NONE\r\n"
#define msg_req_1 "REQ_UPDATE\r\n"
#define msg_req_2 "REQ_LOCAL\r\n"
#define msg_req_x "REQ_?????\r\n"

static const char *trace_str[] =
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
};

void trace_init(void)
{
	hal_uart_init();
}

void trace(uint8_t id)
{
	register uint8_t i;
	register char *str = trace_str[id];

	if (id < TRACE_MSG_NB)
	{
		for (i = 0; i <  trace_len[id]; i++)
		{
			hal_uart_send(str[i]);
		}
	}
}

