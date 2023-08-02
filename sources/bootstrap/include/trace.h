
#include <stdint.h>

typedef enum
{
	TRACE_MSG_ENTER,
	TRACE_MSG_SWAP,
	TRACE_MSG_REBOOT,
	// ---
	TRACE_MSG_APP,
	TRACE_MSG_BL2,
	TRACE_MSG_SYS,
	// ---
	TRACE_MSG_REQ_NONE,
	TRACE_MSG_REQ_UPDATE,
	TRACE_MSG_REQ_LOCAL,
	TRACE_MSG_REQ_UNK,
	// ---
	TRACE_MSG_FAILURE,
	// ---
	TRACE_MSG_NB,
} trace_id_e;

void trace_init(void);
void trace(uint8_t id);

#ifdef USE_BOOTSTRAP_TRACE
#define TRACE_INIT() trace_init()
#define TRACE(id) trace(id)
#else
#define TRACE_INIT()
#define TRACE(id)
#endif
